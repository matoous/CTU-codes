package cz.cvut.fel.agents.pdv.raft;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.ITickCounter;
import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequest;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponse;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseConfirm;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseWithContent;
import java.util.ArrayList;
import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.function.BiConsumer;

/**
 * Proces klienta v nasem systemu, ktery posila a zpracovava pozadavky od procesu v clusteru.
 * Pozadavky klienta jsou koordinovany instanci ClientCoordinator a zavisi na scenari
 */
public class Client<K> extends DSProcess {

  private static final Random RANDOM = new Random();

  // seznam pozadavku, ktere ma klient poslat
  private final List<Pair<ClientRequest, String>> requestsOnServersToSend = new ArrayList<>();

  // o kom si klient mysli, ze je leader
  private String leader = null;
  // procesy v clusteru
  private final List<String> servers;
  // tato instance je vetsinou koordinator, preposilame ji zpravy od procesu, aby mohla
  // koordinovat klienty
  private IServerResponseLogger<K> serverResponseLogger = null;
  // sledovani kroku simulace - pro snadnejsi koordinaci
  private final ITickCounter tickCounter;
  // kontrola posilani zprav leaderovi - ma se poslat zprava aktualnimu leaderovi - pokud leader neni
  // mrtvy nebo v podsiti s mensionou, tak mu ji posleme
  private final ILeaderCheck leaderCheck;

  public Client(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
      List<String> servers, ITickCounter tickCounter, ILeaderCheck leaderCheck) {
    super(id, inbox, outbox);
    this.servers = servers;
    this.tickCounter = tickCounter;
    this.leaderCheck = leaderCheck;
  }

  /**
   * Nastaveni koordinatora, kteremu preposilame odpovedi - kvuli koordniaci
   */
  public void setServerResponseLogger(IServerResponseLogger<K> serverResponseLogger) {
    this.serverResponseLogger = serverResponseLogger;
  }

  /**
   * Pridani procesu z clusteru
   */
  void addRequestOnServer(ClientRequest request, String recipient) {
    requestsOnServersToSend.add(new Pair<>(request, recipient));
  }

  @Override
  public void act() {
    long currentTick = tickCounter.getCurrentTick();

    // vyrid vsechny zpravy z inboxu
    while (!inbox.isEmpty()) {
      Message response = inbox.poll();
      if (response instanceof ServerResponseLeader) {

        // zpracuj odpoved - kdo je leader
        this.leader = serverResponseLogger
            .handleServerResponse(this, (ServerResponseLeader) response, currentTick);
      } else if (response instanceof ServerResponseConfirm) {

        // zpracuj odpoved - potvrzeni operace
        serverResponseLogger
            .handleServerResponse(this, (ServerResponseConfirm) response, currentTick);
      } else if (response instanceof ServerResponseWithContent) {

        // zpracuj odpoved - odpoved s obsahem
        serverResponseLogger
            .handleServerResponse(this, (ServerResponseWithContent<K>) response, currentTick);
      }

      // smaz pozadavek na zaklade odpovedi
      if (response instanceof ServerResponse) {
        serverResponseLogger.deleteRequest(this, ((ServerResponse) response).getRequestId());
      }
    }

    // posli zpravy na procesy v clusteru
    requestsOnServersToSend.forEach(pair -> {
      if (pair.getSecond() != null) {

        // pokud zprava obsahuje prijemce, posli ji tomuto prijemci
        send(pair.getSecond(), pair.getFirst());
      } else if (leader != null && leaderCheck.shouldSendToThisLeader(leader)) {

        // jinak posleme zpravu leadrovi- o kom si to myslime
        send(leader, pair.getFirst());
      } else {

        // nezname leadra, vybereme nahodne nejaky proces z clusteru
        send(servers.get(RANDOM.nextInt(servers.size())), pair.getFirst());
      }

    });
    requestsOnServersToSend.clear();
  }
}
