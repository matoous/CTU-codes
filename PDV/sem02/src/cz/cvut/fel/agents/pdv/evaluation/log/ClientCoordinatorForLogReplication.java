package cz.cvut.fel.agents.pdv.evaluation.log;

import static cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums.APPEND;

import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequest;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWithContent;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseConfirm;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseWithContent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Implementace koordinatora klientu pro scenar replikace logu.
 */
public class ClientCoordinatorForLogReplication extends ClientCoordinator<String> {

  // pozadavky, na ktere nemame odpoved
  private final Map<String, String> requests = new HashMap<>();
  // abeceda, ze ktere vybirame znaky, ktere posilame v pozadavcich
  private final static char[] alphabet = "abcdefghijklmnopqrstuvwxyz".toUpperCase().toCharArray();
  // zde si kumulujeme ocekavane vysledky na zaklade odpovedi od procesu a nasich pozadavku
  // je treba pocitat s ruznymi kombinacemi vysledku, nemame zarucene poradi
  // (ani pri vyzvedavani zprav z inboxu)
  private final List<List<String>> expectedResults = new ArrayList<>();
  private long lastTick = -1;

  ClientCoordinatorForLogReplication(int retryInterval, long stopSendRequests, int requestPeriod,
      int maxRequestsPerTick) {
    super(retryInterval, stopSendRequests, requestPeriod, maxRequestsPerTick);
  }

  @Override
  protected Pair<Pair<ClientRequest, String>, Boolean> getNextRequest() {

    // vytvarime pouze pozadavky s operaci APPEND na klic "A" s nahodne vybranym znakem abecedy
    // klienti si budou pouze pridavat retezec k atualnimu retezci
    ClientRequestWithContent<StoreOperationEnums, Pair<String, String>> request =
        new ClientRequestWithContent<>(generateNewID(), APPEND, new Pair<>("A",
            Character.toString(alphabet[RANDOM.nextInt(alphabet.length)])));
    requests.put(request.getRequestId(), request.getContent().getSecond());

    // kdyz nechame prijemce na null, rozhodnuti, komu poslat bude na klientovi
    return new Pair<>(new Pair<>(request, null), true);
  }

  @Override
  public void deleteRequest(Client<String> client, String id) {

    // smaz pouze v pripade, ze request je vyrizeny - potvrzene zpracovani (ulozeni do logu)
    if (!requests.containsKey(id)) {
      requestsWaitingForResponse.get(client)
          .removeIf(pair -> pair.getFirst().getFirst().getRequestId().equals(id));
    }
  }

  @Override
  public void handleServerResponse(Client<String> client, ServerResponseConfirm responseConfirm,
      long currentTick) {

    //vytvoreni nove skupiny - pro mozne podoby logu na urcitem indexu
    if (currentTick > lastTick && requests.containsKey(responseConfirm.getRequestId())) {
      expectedResults.add(new ArrayList<>());
      lastTick = currentTick;
    }

    // asociace operace podle aktualniho tiku
    String toAdd = requests.remove(responseConfirm.getRequestId());
    if (toAdd != null) {
      expectedResults.get(expectedResults.size() - 1).add(toAdd);
    }
  }

  @Override
  public String handleServerResponse(Client<String> client, ServerResponseLeader responseLeader,
      long currentTick) {

    // posleme pozadavek znovu, pokud je pozadavek, na ktery reaguje tato odpoved, v seznamu
    // cekaajicich
    requestsWaitingForResponse.get(client).stream()
        // existuje nejaky pozadavek s timto id
        .filter(pair -> pair.getFirst().getFirst().getRequestId()
            .equals(responseLeader.getRequestId()))
        .findAny()
        // pokud ano, pozadavek se posle znovu pri nejblizsi prilezitosti
        .ifPresent(pair -> pair.setSecond(-1L));

    // vratime leadera ze zpravy klientovi
    return responseLeader.getResponse();
  }

  @Override
  public void handleServerResponse(Client<String> client,
      ServerResponseWithContent<String> serverResponseWithContent, long currentTick) {

    // prazdna metoda - pro tuto ulohu je netreba. v tomto scenari nas zpravy od klienta na ziskani
    // dat nezajimaji, nevolame GET
  }

  /**
   * Kolik zustalo nezpracovanych pozadavku
   */
  int countOfUncommittedRequests() {
    return requests.size();
  }

  List<List<String>> getExpectedResults() {
    return expectedResults;
  }
}
