package cz.cvut.fel.agents.pdv.evaluation.leader;

import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequest;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWhoIsLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseConfirm;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseWithContent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Implementace koordinatora klientu pro scenar urceni leadera.
 */
public class ClientCoordinatorForLeaderSelection extends ClientCoordinator<String> {

  // procesy, kterych se jeste zbyva zeptat na leadera v aktualnim kole
  private List<String> serversToAskForLeader = new ArrayList<>();

  ClientCoordinatorForLeaderSelection(int retryInterval, long stopSendRequests, int requestPeriod,
      int maxRequestsPerTick) {
    super(retryInterval, stopSendRequests, requestPeriod, maxRequestsPerTick);
  }

  @Override
  protected Pair<Pair<ClientRequest, String>, Boolean> getNextRequest() {
    if (serversToAskForLeader.isEmpty()) {

      // pokud uz jsme se vsech procesu zeptali na leadera, zacneme znovu
      serversToAskForLeader = new ArrayList<>(servers);
      Collections.shuffle(serversToAskForLeader, RANDOM);
    }

    // vytvorime novy request a posleme ho vybranemu procesu
    ClientRequest request = new ClientRequestWhoIsLeader(generateNewID());
    return new Pair<>(new Pair<>(request, serversToAskForLeader.remove(0)), false);
  }

  @Override
  public void handleServerResponse(Client<String> client, ServerResponseConfirm responseConfirm,
      long currentTick) {

    // prazdna metoda - pro tuto ulohu je netreba, jen se ptame na leadera
  }

  @Override
  public String handleServerResponse(Client<String> client, ServerResponseLeader responseLeader,
      long currentTick) {
    return responseLeader.getResponse();
  }

  @Override
  public void handleServerResponse(Client<String> client,
      ServerResponseWithContent<String> serverResponseWithContent, long currentTick) {

    // prazdna metoda - pro tuto ulohu je netreba, zadne hodnoty od procesu necteme
  }
}
