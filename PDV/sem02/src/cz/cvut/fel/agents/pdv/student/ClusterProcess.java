package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import cz.cvut.fel.agents.pdv.raft.messages.*;

import java.util.*;
import java.util.function.BiConsumer;

import static cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums.GET;
import static java.lang.StrictMath.max;
import static java.util.concurrent.ThreadLocalRandom.current;

public class ClusterProcess extends RaftProcess<Map<String, String>> {
    private static boolean DEBUG = true;
    private static int BATCH_SIZE = 4;

    private int electionTimeout() {
        return current().nextInt(2 * networkDelays, 6 * networkDelays) + 2;
    }

    // ostatni procesy v clusteru
    private final List<String> otherProcessesInCluster;
    // maximalni spozdeni v siti
    private final int networkDelays;

    // State machine for store
    private final KeyValStore store;
    // State of this server
    private StateEnums state;

    // FOR EVERYONE
    // latest term server has seen (initialized to 0 on first boot, increases monotonically)
    private int currentTerm;
    // candidateId that received vote in current term (or null if none)
    private String voteFor;
    // log entries; each entry contains command for state machine, and term when entry was received by leader (first index is 1)
    private Log log;
    // index of highest log entry known to be committed (initialized to 0, increases monotonically)
    private int commitIndex;

    // FOR LEADER
    // for each server, index of the next log entry to send to that server (initialized to leader last log index + 1)
    private Map<String, Integer> nextIndex;
    // for each server, index of highest log entry known to be replicated on server (initialized to 0, increases monotonically)
    private Map<String, Integer> matchIndex;
    private Map<String, Integer> rpcDue;
    private Map<String, Integer> heartBeatDue;
    private int tick;

    // helpers
    private int electionTimer;
    private String currentLeader;
    private Set<String> votesForMe;

    public ClusterProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                          List<String> otherProcessesInCluster, int networkDelays) {
        super(id, inbox, outbox);
        this.networkDelays = max(1, networkDelays);
        this.otherProcessesInCluster = otherProcessesInCluster;
        store = new KeyValStore();
        currentTerm = 0;
        log = new Log();
        commitIndex = 0;
        state = StateEnums.FOLLOWER;
        currentLeader = null;
        votesForMe = new HashSet<>();
        nextIndex = new HashMap<>();
        rpcDue = new HashMap<>();
        matchIndex = new HashMap<>();
        heartBeatDue = new HashMap<>();
        for (String neighbour : otherProcessesInCluster) {
            nextIndex.put(neighbour, 1);
            matchIndex.put(neighbour, 0);
            rpcDue.put(neighbour, 0);
            heartBeatDue.put(neighbour, 0);
        }

        tick = 0;
        electionTimer = 0;
        resetElectionTimer();
    }

    private void stepDown(int term) {
        currentTerm = term;
        state = StateEnums.FOLLOWER;
        voteFor = null;
        if (electionTimer < tick || electionTimer == Integer.MAX_VALUE) {
            resetElectionTimer();
        }
    }

    private void startNewElection() {
        if ((state == StateEnums.FOLLOWER || state == StateEnums.CANDIDATE) && electionTimer < tick) {
            resetElectionTimer();
            currentTerm++;
            voteFor = getId();
            state = StateEnums.CANDIDATE;
            votesForMe = new HashSet<>();
            nextIndex = new HashMap<>();
            rpcDue = new HashMap<>();
            matchIndex = new HashMap<>();
            heartBeatDue = new HashMap<>();
            for (String neighbour : otherProcessesInCluster) {
                nextIndex.put(neighbour, 1);
                matchIndex.put(neighbour, 0);
                rpcDue.put(neighbour, 0);
                heartBeatDue.put(neighbour, 0);
            }
        }
    }

    private void sendRequestVote(String peer) {
        if (state == StateEnums.CANDIDATE && rpcDue.get(peer) <= tick) {
            rpcDue.put(peer, 3 * networkDelays);
            send(peer, new RequestVote(
                    currentTerm,
                    getId(),
                    log.size(),
                    log.lastLogTerm()));
        }
    }

    private void becomeLeader() {
        if (state == StateEnums.CANDIDATE && votesForMe.size() + 1 > otherProcessesInCluster.size() / 2) {
            state = StateEnums.LEADER;
            currentLeader = getId();
            nextIndex = new HashMap<>();
            rpcDue = new HashMap<>();
            heartBeatDue = new HashMap<>();
            for (String neighbour : otherProcessesInCluster) {
                nextIndex.put(neighbour, log.size() + 1);
                rpcDue.put(neighbour, Integer.MAX_VALUE);
                heartBeatDue.put(neighbour, 0);
            }
            electionTimer = Integer.MAX_VALUE;
        }
    }

    private void sendAppendEntries(String peer) {
        if (state == StateEnums.LEADER &&
                (heartBeatDue.get(peer) <= tick ||
                        (nextIndex.get(peer) <= log.size() &&
                                rpcDue.get(peer) <= tick))) {
            int prevIndex = nextIndex.get(peer) - 1;
            int lastIndex = Math.min(prevIndex + BATCH_SIZE, log.size());
            if (matchIndex.get(peer) + 1 < nextIndex.get(peer)) {
                lastIndex = prevIndex;
            }
            this.send(peer, new AppendEntries(
                    currentTerm,
                    getId(),
                    prevIndex,
                    log.getTermOf(prevIndex),
                    log.sub(prevIndex, lastIndex),
                    Math.min(commitIndex, lastIndex)
            ));
            rpcDue.put(peer, tick + networkDelays);
            heartBeatDue.put(peer, tick + networkDelays * 2 - 1);
        }
    }

    private void advanceCommitIndex() {
        List<Integer> list = new ArrayList<>(matchIndex.values());
        list.sort(Comparator.comparing(Integer::intValue));
        int n = list.get(otherProcessesInCluster.size() / 2);
        if (state == StateEnums.LEADER && log.getTermOf(n) == currentTerm) {
            int newCommitIndex = Math.max(commitIndex, n);
            if (newCommitIndex != commitIndex) {
                for (int i = commitIndex + 1; i <= newCommitIndex; i++) {
                    LogEntry e = log.get(i);
                    store.apply(e);
                    send(e.action.clientId, new ServerResponseConfirm(e.action.requestId));
                }
            }
            commitIndex = newCommitIndex;
        }
    }

    private void handleVoteRequest(RequestVote requestVote) {
        // If RPC request or response contains term T > currentTerm: set currentTerm = T, convert to follower
        if (currentTerm < requestVote.term) {
            stepDown(requestVote.term);
        }
        boolean granted = false;
        if (currentTerm == requestVote.term &&
                (voteFor == null || voteFor.equals(requestVote.sender))
                && (requestVote.lastLogTerm > log.lastLogTerm()) ||
                (requestVote.lastLogTerm == log.lastLogTerm() && requestVote.lastLogIndex >= log.size())) {
            granted = true;
            voteFor = requestVote.sender;
            resetElectionTimer();
        }
        send(requestVote.candidateId, new RequestVoteResponse(currentTerm, granted));
    }

    private void handleRequestVoteReply(RequestVoteResponse requestVoteResponse) {
        if (currentTerm < requestVoteResponse.term) {
            stepDown(requestVoteResponse.term);
        }
        if (state == StateEnums.CANDIDATE && currentTerm == requestVoteResponse.term) {
            rpcDue.put(requestVoteResponse.sender, Integer.MAX_VALUE);
            if (requestVoteResponse.voteGranted) {
                votesForMe.add(requestVoteResponse.sender);
            }
        }
    }

    private void handleAppendEntries(AppendEntries appendEntries) {
        boolean success = false;
        int matchIndex = 0;
        if (currentTerm < appendEntries.term) {
            stepDown(appendEntries.term);
        }
        if (currentTerm == appendEntries.term) {
            state = StateEnums.FOLLOWER;
            currentLeader = appendEntries.leaderId;
            resetElectionTimer();
            if (appendEntries.prevLogIndex == 0 ||
                    (appendEntries.prevLogIndex <= log.size() &&
                            log.getTermOf(appendEntries.prevLogIndex) == appendEntries.prevLogTerm)) {
                success = true;
                int idx = appendEntries.prevLogIndex;
                for (int i = 0; i < appendEntries.logEntries.size(); i++) {
                    idx++;
                    if (log.getTermOf(idx) != appendEntries.logEntries.get(i).term) {
                        while (log.size() > idx - 1)
                            log.pop();
                        log.add(appendEntries.logEntries.get(i));
                    }
                }
                matchIndex = idx;
                int newCommitIndex = Math.max(commitIndex, appendEntries.leaderCommit);
                if (newCommitIndex != commitIndex) {
                    for (int i = commitIndex + 1; i <= newCommitIndex; i++) {
                        store.apply(log.get(i));
                    }
                }
                commitIndex = newCommitIndex;
            }
        }
        send(appendEntries.sender, new AppendEntriesResponse(currentTerm, success, matchIndex));
    }

    private void handleAppendEntriesReply(AppendEntriesResponse response) {
        if (currentTerm < response.term) {
            stepDown(response.term);
        }
        if (state == StateEnums.LEADER && currentTerm == response.term) {
            if (response.success) {
                matchIndex.put(response.sender, Math.max(matchIndex.get(response.sender), response.matchIndex));
                nextIndex.put(response.sender, response.matchIndex + 1);
            } else {
                nextIndex.put(response.sender, Math.max(1, nextIndex.get(response.sender) - 1));
            }
            rpcDue.put(response.sender, 0);
        }
    }

    @Override
    public Optional<Map<String, String>> getLastSnapshotOfLog() {
        return Optional.of(store.pure());
    }

    @Override
    public String getCurrentLeader() {
        return currentLeader;
    }

    private void handleClientRequest(ClientRequestWithContent clientRequest) {
        if (state == StateEnums.LEADER) {
            Pair<String, String> data = (Pair<String, String>) clientRequest.getContent();
            if(clientRequest.getOperation() == GET){
                send(clientRequest.sender, new ServerResponseWithContent<>(clientRequest.getRequestId(), store.get(data.getFirst())));
            } else {
                Request req = new Request(clientRequest.sender, clientRequest.getRequestId(), clientRequest.getOperation(), data.getFirst(), data.getSecond());
                if (log.entryIds().contains(req.requestId)) return;
                log.add(new LogEntry(currentTerm, req));
            }
        } else {
            send(clientRequest.sender, new ServerResponseLeader(clientRequest.getRequestId(), getCurrentLeader()));
        }
    }

    private void handleClientRequestWhoIsLeader(ClientRequestWhoIsLeader clientRequest) {
        send(clientRequest.sender, new ServerResponseLeader(clientRequest.getRequestId(), getCurrentLeader()));
    }

    @Override
    public void act() {

        tick++;
        startNewElection();
        becomeLeader();
        advanceCommitIndex();
        for (String neighbour : otherProcessesInCluster) {
            sendRequestVote(neighbour);
            sendAppendEntries(neighbour);
        }

        // Handle incoming messages
        for (Message m : inbox) {
            // Vote request
            if (m instanceof RequestVote) {
                RequestVote requestVote = (RequestVote) m;
                handleVoteRequest(requestVote);
            }
            // Append entries
            else if (m instanceof AppendEntries) {
                AppendEntries appendEntries = (AppendEntries) m;
                handleAppendEntries(appendEntries);
            }
            // Vote request response
            else if (m instanceof RequestVoteResponse) {
                RequestVoteResponse requestVoteResponse = (RequestVoteResponse) m;
                handleRequestVoteReply(requestVoteResponse);
            }
            // Append Entries response
            else if (m instanceof AppendEntriesResponse) {
                AppendEntriesResponse response = (AppendEntriesResponse) m;
                handleAppendEntriesReply(response);
            }
            // Client request with content
            else if (m instanceof ClientRequestWithContent) {
                ClientRequestWithContent clientRequest = (ClientRequestWithContent) m;
                handleClientRequest(clientRequest);
            }
            // Client request who is leader
            else if (m instanceof ClientRequestWhoIsLeader) {
                ClientRequestWhoIsLeader clientRequest = (ClientRequestWhoIsLeader) m;
                handleClientRequestWhoIsLeader(clientRequest);
            }
        }
        // clear the inbox
        inbox.clear();
    }

    private void resetElectionTimer() {
        electionTimer = tick + electionTimeout();
    }
}
