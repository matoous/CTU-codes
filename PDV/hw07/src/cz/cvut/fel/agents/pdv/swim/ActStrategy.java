package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.util.*;
import java.util.Map.Entry;
import java.util.stream.Collectors;

import static java.lang.Integer.min;

public class ActStrategy {

    private final int maxDelayForMessages;
    private String me;
    private int index;
    private List<String> otherProcesses;

    private final HashMap<String, Boolean> requestedPings;
    private final HashMap<String, HashSet<String>> callbacks;
    private Random randomGenerator;
    private int tick;
    private final Map<String, Integer> lastSend;

    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {

        this.maxDelayForMessages = maxDelayForMessages;
        this.randomGenerator = new Random();
        Collections.shuffle(otherProcesses);
        this.otherProcesses = otherProcesses.subList(0, 25);
        this.index = this.randomGenerator.nextInt(this.otherProcesses.size());
        this.requestedPings = new HashMap<>();
        this.callbacks = new HashMap<>();
        this.lastSend = new HashMap<>();
        this.me = "unknown";
    }

    public String nextNeighbour() {
        this.index = (this.index+1)%this.otherProcesses.size();
        String toPing = this.otherProcesses.get(this.index);
        while(this.requestedPings.containsKey(toPing)){
            this.index = (this.index+1)%this.otherProcesses.size();
            toPing = this.otherProcesses.get(this.index);
        }
        return toPing;
    }

    public String randomNeighbour(){
        return this.otherProcesses.get(this.randomGenerator.nextInt(this.otherProcesses.size()));
    }

    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {
        List<Pair<String, Message>> outbox = new ArrayList<>();
        while(!inbox.isEmpty()){
            Message tmpmsg = inbox.remove();
            if(this.me == "unknown"){
                this.me = tmpmsg.recipient;
            }

            // Ping
            if(tmpmsg instanceof PingMessage){
                // Respond to ping msg
                PingMessage pingmsg = (PingMessage)tmpmsg;
                outbox.add(new Pair<>(pingmsg.sender, new AckMessage(pingmsg.recipient)));
                if(this.callbacks.containsKey(pingmsg.sender)){
                    for(String waiter : this.callbacks.get(pingmsg.sender)){
                        //System.out.println("Telling " + waiter + " about " + pingmsg.getProcessID() + " by " + this.me);
                        outbox.add(new Pair<>(waiter, pingmsg));
                    }
                }
                this.lastSend.remove(pingmsg.getProcessID());
                this.requestedPings.remove(pingmsg.getProcessID());
            }

            // Ping request
            else if(tmpmsg instanceof PingRequest){
                PingRequest pingreqmsg = (PingRequest)tmpmsg;
                //System.out.println(pingreqmsg.sender + " asks about " + pingreqmsg.getProcessID() + " " + this.me);
                if(this.callbacks.containsKey(pingreqmsg.getProcessID())){
                    this.callbacks.get(pingreqmsg.getProcessID()).add(pingreqmsg.sender);
                } else {
                    this.callbacks.put(pingreqmsg.getProcessID(), new HashSet<>());
                    this.callbacks.get(pingreqmsg.getProcessID()).add(pingreqmsg.sender);
                }
                outbox.add(new Pair<>(pingreqmsg.getProcessID(), new PingMessage(pingreqmsg.getProcessID())));
            }

            // Ack
            else if(tmpmsg instanceof AckMessage){
                AckMessage ackmsg = (AckMessage)tmpmsg;
                // acknowledge if anyone is waiting for this
                if(this.callbacks.containsKey(ackmsg.getProcessID())){
                    for(String waiter : this.callbacks.get(ackmsg.getProcessID())){
                        //System.out.println("Telling " + waiter + " about " + ackmsg.getProcessID() + " by " + this.me);
                        outbox.add(new Pair<>(waiter, ackmsg));
                    }
                }
                this.lastSend.remove(ackmsg.getProcessID());
                this.requestedPings.remove(ackmsg.getProcessID());
            }

            // DeadProcessMessage
            else if(tmpmsg instanceof DeadProcessMessage){
                DeadProcessMessage deadmsg = (DeadProcessMessage)tmpmsg;
                this.otherProcesses = this.otherProcesses.stream().filter(x -> !x.equals(deadmsg.getProcessID())).collect(Collectors.toList());
            }
        }

        if(this.tick%2 == 1){
            // Ping random neighbour
            String currPingTarget = this.nextNeighbour();
            PingMessage msg = new PingMessage(currPingTarget);
            outbox.add(new Pair<>(currPingTarget, msg));
            lastSend.put(currPingTarget, this.tick);
        }


        // Check dead neighbours
        String outnode = "unknown";
        for(Entry<String, Integer> pinged : this.lastSend.entrySet()){
            if(pinged.getValue() < this.tick - 5*this.maxDelayForMessages){
                // Process is dead and not even neighbours helped
                //System.out.println(this.me + " reporting " + pinged.getKey() + " ad dead");
                outbox.add(new Pair<>(disseminationProcess, new PFDMessage(pinged.getKey())));
                outnode = pinged.getKey();
                continue;
            }
            if(pinged.getValue() < this.tick - 2*this.maxDelayForMessages && !this.requestedPings.containsKey(pinged.getKey())){
                List<String> cpy = this.otherProcesses;
                Collections.shuffle(cpy);
                cpy = cpy.subList(0,10);
                // Process didn't respond, try asking neighbours
                for(int i = 0; i < 7; i++){
                    String tmp = cpy.get(i);
                    PingRequest tmpmsg = new PingRequest(pinged.getKey());
                    outbox.add(new Pair<>(tmp, tmpmsg));
                }
                this.requestedPings.put(pinged.getKey(), true);
            }
        }

        if(outnode != "unknown"){
            this.lastSend.remove(outnode);
        }

        this.tick++;
        outbox = outbox.stream().distinct().collect(Collectors.toList());
        return outbox;
    }

}
