package cz.cvut.fel.agents.pdv.raft;

import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequest;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Random;
import java.util.Set;
import java.util.UUID;

/**
 * Abstraktni trida (sablona) pro koordinatora klientu. Ukolem instance je generovat nove pozadavky
 * a odesilat stare, tak ze se urceje klienta, kteremu se novy/stary pozadavek priradi. Chovani
 * zavisi na scenari a konkretni implementaci metod podle IServerResponseLogger. Implementace
 * urcuje, jak se naklada s odpovedmi
 */
public abstract class ClientCoordinator<K> implements IServerResponseLogger<K> {

  // registr pouzitych id
  private final Set<String> takenIDs = new HashSet<>();
  // databaze pozadavku cekajici na odpoved podle klientu. Ukladame si prijemce, pozadavek a cas
  // poslni pro pripadne znovuposlani
  protected final Map<Client<K>, List<Pair<Pair<ClientRequest, String>, Long>>> requestsWaitingForResponse = new HashMap<>();

  // procesy v clusteru
  protected final Set<String> servers = new HashSet<>();

  protected static final Random RANDOM = new Random();

  // pocet kroku, kdy se klient pokusi znovu odeslat pozadavek
  private final int retryInterval;
  // kdy prestat posilat operace na procesy
  private final long stopSendRequests;
  // jak casto posilat operace na proces
  private final int requestPeriod;
  // kolik maximalne generovat pozadavku v tiku
  private final int maxRequestsPerTick;

  protected ClientCoordinator(int retryInterval, long stopSendRequests, int requestPeriod,
      int maxRequestsPerTick) {
    this.retryInterval = retryInterval;
    this.stopSendRequests = stopSendRequests;
    this.requestPeriod = requestPeriod;
    this.maxRequestsPerTick = maxRequestsPerTick;
  }

  /**
   * Zaregistruj proces z clusteru
   */
  public void addServer(String server) {
    this.servers.add(server);
  }

  /**
   * Zaregistruj klienta, ktery bude koordinovan touto instanci
   */
  public void addClient(Client<K> client) {
    this.requestsWaitingForResponse.put(client, new ArrayList<>());
  }

  /**
   * Obdoba metody act() u procesu. Generujeme nove pozadavky a odesilame stare, tak ze urcime
   * klienta, kteremu se novy/stary pozadavek priradi
   */
  public void act(long stepOfSimulation) {

    // posli znovu vsechny requesty, na ktere nebyla obdrzena odpoved v casovem limitu
    // nebo byla hodnota odeslani nastavena na -1
    requestsWaitingForResponse.forEach((key, value) -> value.stream()
        .filter(pair -> pair.getSecond() + retryInterval <= stepOfSimulation
            || pair.getSecond() == -1)
        .forEach(pair -> {

          // restartuj cas odeslani na aktualni krok simulace
          pair.setSecond(stepOfSimulation);

          // a posli znovu request pres klienta
          key.addRequestOnServer(pair.getFirst().getFirst(), pair.getFirst().getSecond());
        }));

    // vygeneruj nove requesty klientu, pokud se neblizi konec simulace.
    // uvazujeme periodu posilani requestu
    if (stopSendRequests >= stepOfSimulation && stepOfSimulation % requestPeriod == 0) {

      // posleme aspon jeden pozadavek
      int requestsToSend = Math.max(RANDOM.nextInt(maxRequestsPerTick), 1);
      for (int i = 0; i < requestsToSend; i++) {

        // vytvorime pozadavek
        Pair<Pair<ClientRequest, String>, Boolean> request = getNextRequest();

        // request priradime klientovi s nejmensim poctem requestu
        Client<K> client = getClientWithLeastRequests();
        client.addRequestOnServer(request.getFirst().getFirst(), request.getFirst().getSecond());

        // ma se poslani requestu opakovat
        if (request.getSecond()) {
          requestsWaitingForResponse.get(client)
              .add(new Pair<>(request.getFirst(), stepOfSimulation));
        }
      }
    }
  }

  @Override
  public void deleteRequest(Client<K> client, String id) {
    requestsWaitingForResponse.get(client)
        .removeIf(pair -> pair.getFirst().getFirst().getRequestId().equals(id));
  }

  /**
   * Vygenerujeme nove unikatni ID pro nas pozadavek
   */
  protected synchronized String generateNewID() {
    String uniqueID = UUID.randomUUID().toString();
    if (!takenIDs.contains(uniqueID)) {
      takenIDs.add(uniqueID);
      return uniqueID;
    }
    return generateNewID();
  }

  /**
   * Vybereme klienta s nejmensi zatezi
   */
  private Client<K> getClientWithLeastRequests() {
    return requestsWaitingForResponse.entrySet().stream()
        .min(Comparator.comparingInt(o -> o.getValue().size()))
        .map(Entry::getKey)
        .get();
  }

  /**
   * Vygenerovani noveho pozadavku. Dostaneme pozadavek, recipienta a indikator, zda se ma pozadavek
   * poslat znovu, kdyz na nej nedostaneme odpoved do urciteho casu podle konfigurace
   */
  protected abstract Pair<Pair<ClientRequest, String>, Boolean> getNextRequest();
}
