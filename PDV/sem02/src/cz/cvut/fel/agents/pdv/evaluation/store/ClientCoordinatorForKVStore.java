package cz.cvut.fel.agents.pdv.evaluation.store;

import static cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums.APPEND;
import static cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums.GET;
import static cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums.PUT;

import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequest;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWithContent;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseConfirm;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseWithContent;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Objects;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * Tento scenar doplnime. Bude slouzit k otestovani plne funkcni distribuovane key/value databaze
 */
public class ClientCoordinatorForKVStore extends ClientCoordinator<String> {

  // pozadavky, na ktere nemame odpoved
  private final Map<String, RequestMetadata> requests = new HashMap<>();
  // odeslane pozadavky - obcas klienti poslou stejny pozadavek vicekrat
  private final List<Pair<String, RequestMetadata>> confirmedRequests = new ArrayList<>();
  // pouzite klice v databazi - pro generovani GET dotazu
  private final List<String> usedKeys = new ArrayList<>();
  // abeceda, ze ktere vybirame znaky, ktere posilame v pozadavcich
  private final static char[] alphabet = "abcd".toUpperCase().toCharArray();
  // zde si kumulujeme ocekavane vysledky na zaklade odpovedi od procesu a nasich pozadavku
  // je treba pocitat s ruznymi kombinacemi vysledku, nemame zarucene poradi
  // (ani pri vyzvedavani zprav z inboxu)
  private final List<List<RequestMetadata>> expectedResults = new ArrayList<>();
  private long lastTick = -1;
  private final List<StoreOperationEnums> operationsToChooseFrom = Stream.of(APPEND, PUT)
      .collect(Collectors.toList());

  ClientCoordinatorForKVStore(int retryInterval, long stopSendRequests, int requestPeriod,
      int maxRequestsPerTick) {
    super(retryInterval, stopSendRequests, requestPeriod, maxRequestsPerTick);
  }

  @Override
  protected Pair<Pair<ClientRequest, String>, Boolean> getNextRequest() {

    if (!usedKeys.isEmpty() && operationsToChooseFrom.size() == 2) {

      // pokud uz mame v databazi nejaky klic, muzeme volat i GET
      operationsToChooseFrom.add(GET);
    }

    // obcas posleme jednu potvrzenou operaci znovu
    if (!confirmedRequests.isEmpty() && RANDOM.nextDouble() >= 0.75) {

      // vybereme pozadavek
      Pair<String, RequestMetadata> resend = confirmedRequests
          .get(RANDOM.nextInt(confirmedRequests.size()));

      // vytvorime pozadavek
      ClientRequestWithContent<StoreOperationEnums, Pair<String, String>> request =
          new ClientRequestWithContent<>(resend.getFirst(), resend.getSecond().operationEnums,
              resend.getSecond().content);

      // kdyz nechame prijemce na null, rozhodnuti, komu poslat bude na klientovi
      return new Pair<>(new Pair<>(request, null), true);
    }

    // vytvarime pozadavky s jednou z vybranych operaci - GET, APPEND, PUT
    StoreOperationEnums operation = operationsToChooseFrom
        .get(RANDOM.nextInt(operationsToChooseFrom.size()));
    Pair<String, String> pair;
    // v pripade operaci PUT a APPEND nahodne vybereme klic a k nemu hodnotu pro nas pozadavek
    // (oba jsou znaky v abesede)
    switch (operation) {
      case PUT:
        pair = new Pair<>(getRandomCharacterFromAlphabet(), getRandomCharacterFromAlphabet());
        break;
      case GET:
        pair = new Pair<>(usedKeys.get(RANDOM.nextInt(usedKeys.size())), null);
        break;
      default:
        pair = new Pair<>(getRandomCharacterFromAlphabet(), getRandomCharacterFromAlphabet());
        break;
    }

    // vytvorime pozadavek a zaregistrujeme ho
    ClientRequestWithContent<StoreOperationEnums, Pair<String, String>> request =
        new ClientRequestWithContent<>(generateNewID(), operation, pair);
    requests.put(request.getRequestId(), new RequestMetadata(operation, pair,
        request.getRequestId()));

    // kdyz nechame prijemce na null, rozhodnuti, komu poslat bude na klientovi
    return new Pair<>(new Pair<>(request, null), true);
  }

  private String getRandomCharacterFromAlphabet() {
    return Character.toString(alphabet[RANDOM.nextInt(alphabet.length)]);
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
    RequestMetadata toAdd = requests.get(responseConfirm.getRequestId());
    if (toAdd != null && !toAdd.operationEnums.equals(GET)) {

      // ignorujeme GET, ten potvrzovat nechceme - na ten chceme vratit odpoved s aktualni hodnotou
      expectedResults.get(expectedResults.size() - 1).add(toAdd);
      requests.remove(responseConfirm.getRequestId());
      confirmedRequests.add(new Pair<>(responseConfirm.getRequestId(), toAdd));

      // pridej klic
      if (!usedKeys.contains(toAdd.content.getFirst())) {
        usedKeys.add(toAdd.content.getFirst());
      }

    } else if (expectedResults.get(expectedResults.size() - 1).isEmpty()) {

      // alokovali jsme seznam, ale nic jsme do nej nepridali - potvrzeni slo na GET
      expectedResults.remove(expectedResults.size() - 1);
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

    // asociace operace podle aktualniho tiku
    RequestMetadata toAdd = requests.get(serverResponseWithContent.getRequestId());
    if (toAdd != null && toAdd.operationEnums.equals(GET)) {

      // smazeme z cekajicich a pridame do pozadavku, ktere se maji opakovat
      requests.remove(serverResponseWithContent.getRequestId());
      confirmedRequests.add(new Pair<>(serverResponseWithContent.getRequestId(), toAdd));
    }
  }

  @Override
  public void deleteRequest(Client<String> client, String id) {

    // smaz pouze v pripade, ze request je vyrizeny - potvrzene zpracovani (ulozeni do logu)
    if (!requests.containsKey(id)) {
      requestsWaitingForResponse.get(client)
          .removeIf(pair -> pair.getFirst().getFirst().getRequestId().equals(id));
    }
  }

  /**
   * Trida pro uchovani informaci o pozadavku
   */
  private static class RequestMetadata implements Serializable {

    private final StoreOperationEnums operationEnums;
    private final Pair<String, String> content;
    private final String id;

    private RequestMetadata(StoreOperationEnums operationEnums,
        Pair<String, String> content, String id) {
      this.operationEnums = operationEnums;
      this.content = content;
      this.id = id;
    }

    @Override
    public boolean equals(Object o) {
      if (this == o) {
        return true;
      }
      if (o == null || getClass() != o.getClass()) {
        return false;
      }
      RequestMetadata metadata = (RequestMetadata) o;
      return Objects.equals(id, metadata.id);
    }

    @Override
    public int hashCode() {

      return Objects.hash(id);
    }
  }

  /**
   * Prevede ocekavane vysledky na seznam moznych retezcu pro klic
   */
  Map<String, List<String>> getExpectedResults() {

    // pro vsechny pouzite klice vytvorime prehled pozadavku v poradi doruceni
    Map<String, List<List<RequestMetadata>>> reqByKey = usedKeys.stream()
        .collect(Collectors.toMap(Function.identity(), o -> new ArrayList<>()));
    expectedResults.forEach(reqs -> reqs.stream()
        .collect(Collectors.groupingBy(o -> o.content.getFirst(), Collectors.toList()))
        .forEach((s, results) -> reqByKey.get(s).add(results)));

    // prevedeme na vsechny mozne kombinace retezcu, ktere nasledne vratime
    return reqByKey.entrySet().stream()
        .collect(Collectors.toMap(Entry::getKey, entry -> {
          List<String> combinations = Collections.singletonList("");

          // vytvoreni moznych kombinaci
          for (List<RequestMetadata> list : entry.getValue()) {

            // budeme rozsirovat vsechny mozne predchozi kombinace
            combinations = getCombinations(combinations, list);
          }

          return combinations;
        }));
  }

  /**
   * Vytvori mozne kombinace hodnot pro klic
   */
  private List<String> getCombinations(List<String> combinations, List<RequestMetadata> metadata) {
    if (metadata.isEmpty()) {
      return combinations;
    }
    // vytvorime vsechny dalsi mozne kombinace
    return metadata.stream()
        .flatMap(requestMetadata -> {
              switch (requestMetadata.operationEnums) {
                case APPEND:
                  // pridame kombinace a vratime jako novy seznam
                  return getCombinations(combinations.stream()
                      // pridani znaku k soucasne hodnote
                      .map(s -> s + requestMetadata.content.getSecond())
                      .collect(Collectors.toList()), metadata.stream()
                      // rozsirime o dalsi mozne kombinace neobsahujici tento pozadavek
                      .filter(rm -> !rm.equals(requestMetadata))
                      .collect(Collectors.toList())).stream();
                case PUT:
                  // pridame kombinace a vratime jako novy seznam
                  return getCombinations(combinations.stream()
                      // nahrazeni soucasne hodnoty
                      .map(s -> requestMetadata.content.getSecond())
                      .collect(Collectors.toList()), metadata.stream()
                      // rozsirime o dalsi mozne kombinace neobsahujici tento pozadavek
                      .filter(rm -> !rm.equals(requestMetadata))
                      .collect(Collectors.toList())).stream();
              }
              return Stream.empty();
            }
        )
        // chceme jen unikatni retezce
        .distinct()
        .collect(Collectors.toList());
  }

  /**
   * Kolik zustalo nezpracovanych pozadavku
   */
  int countOfUncommittedRequests() {
    return requests.size();
  }

}
