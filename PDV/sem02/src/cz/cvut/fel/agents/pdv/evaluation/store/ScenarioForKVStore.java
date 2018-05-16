package cz.cvut.fel.agents.pdv.evaluation.store;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.evaluation.Scenario;
import cz.cvut.fel.agents.pdv.evaluation.ScenarioBuilder;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * Tento scenar doplnime. Bude slouzit k otestovani plne funkcni distribuovane key/value databaze
 */
public class ScenarioForKVStore extends Scenario<ClientCoordinatorForKVStore> {

  // maximalni pocet nezpracovanych pozadavku - <0,N>
  private final int maxUncommittedRequest;

  /**
   * Vrati instanci stavitele pro nakonfigurovani a nasledne vytvoreni instance scenare
   */
  public static ScenarioBuilder<ScenarioForKVStore> builder() {
    return new ScenarioBuilder<ScenarioForKVStore>() {
      @Override
      public ScenarioForKVStore build() {
        return new ScenarioForKVStore(numberOfProcessesInCluster,
            reliabilityOfNetworkConnections, processToKill, restartProcess, reconnectProcess,
            expectedNumberOfProcessesInPartition, stepsOfSimulation, networkDelays, lengthOfFailure,
            spanBetweenFailures, countOfFailures, numberOfClients, requestPeriod,
            stopRequestBeforeEndOfSimulation, retryInterval, maxRequestsPerTick,
            maxUncommittedRequest);
      }
    };
  }

  private ScenarioForKVStore(int numberOfProcessesInCluster,
      double reliabilityOfNetworkConnections, int processToKill, boolean restartProcess,
      boolean reconnectProcess, int expectedNumberOfProcessesInPartition, long stepsOfSimulation,
      int networkDelays, int lengthOfFailure, int spanBetweenFailure, int countOfFailure,
      int numberOfClients, int requestPeriod, int stopRequestBeforeEndOfSimulation,
      int retryInterval, int maxRequestsPerTick, int maxUncommittedRequest) {
    super(() -> new ClientCoordinatorForKVStore(retryInterval,
            stepsOfSimulation - stopRequestBeforeEndOfSimulation, requestPeriod,
            maxRequestsPerTick), numberOfProcessesInCluster, reliabilityOfNetworkConnections,
        processToKill, restartProcess, reconnectProcess, expectedNumberOfProcessesInPartition,
        stepsOfSimulation,
        networkDelays, lengthOfFailure, spanBetweenFailure, countOfFailure, numberOfClients);
    this.maxUncommittedRequest = maxUncommittedRequest;
  }

  @Override
  public boolean evaluateSimulation(List<Client<String>> clients,
      List<RaftProcess<Map<String, String>>> clusterProcesses,
      ClientCoordinatorForKVStore clientCoordinator, List<Set<String>> partitions,
      Set<String> deadProcesses, boolean recoveredFromFailure) throws Exception {

    // v metode overujeme, zda data v databazi jsou konzistentni napric clusterem
    System.out.println("Doslo k obnoveni site: " + (recoveredFromFailure ? "ano" : "ne"));

    // existuji nejake nevyrizene pozadavky?
    if (clientCoordinator.countOfUncommittedRequests() > 0) {
      System.out.println("Zustalo celkem " + clientCoordinator.countOfUncommittedRequests()
          + " nezpracovanych pozadavku");

      // je vice nezpracovanych pozadavku, nez je limit
      if (clientCoordinator.countOfUncommittedRequests() > maxUncommittedRequest) {
        System.err.println("Nezpracovanych pozadavku je vice, nez povoluje limit.");
        return false;
      }
    }

    // vypiseme vysledky logu klientu a ocekavane vysledky
    String data = clusterProcesses.stream()
        .map(mapRaftProcess -> mapRaftProcess.getId() + ": " +
            mapRaftProcess.getLastSnapshotOfLog().get().entrySet().stream()
                .map(entry -> entry.getKey() + " -> " + entry.getValue())
                .collect(Collectors.joining(", ")))
        .collect(Collectors.joining("\n"));
    System.out.println("Databaze z jednotlivych procesu:");
    System.out.println(data);

    Map<String, List<String>> expectedResults = clientCoordinator.getExpectedResults();
    String expectedData = expectedResults.entrySet().stream()
        .map(entry -> entry.getKey() + " -> " + entry.getValue().stream()
            .collect(Collectors.joining(" | ")))
        .collect(Collectors.joining(", "));
    System.out.println("Kombinace ocekavanych podob databaze:");
    System.out.println(expectedData);

    boolean correctResults = false;

    // system se zotavil ze selhani nebo k nemu ani nedoslo, data by mela byt konzistentni
    // v takovem pripade by vsechny logy mely byt shodne
    if (recoveredFromFailure) {

      // vybereme log nejakeho procesu
      Map<String, String> snapshotCheck = clusterProcesses.get(0).getLastSnapshotOfLog().get();

      // jsou vsechny snapshoty stejne
      if (clusterProcesses.stream()
          .map(RaftProcess::getLastSnapshotOfLog)
          .map(Optional::get)
          .allMatch(snapshotCheck::equals)) {
        correctResults = match(expectedResults, snapshotCheck);
      }
    } else {

      // nedoslo k zotaveni systemu. jeden spolecny leader neni vubec zaruceny, proto
      // overime, ze nejvetsi skupina - podsit nebo zive procesy - ma stejny log

      //vsechny procesy, ktere nas zajimaji
      List<String> processes = !deadProcesses.isEmpty() ? clusterProcesses.stream()
          .map(DSProcess::getId)
          .filter(s -> !deadProcesses.contains(s))
          .collect(Collectors.toList()) : new ArrayList<>(partitions.stream()
          .max(Comparator.comparingInt(Set::size))
          .get());
      List<RaftProcess<Map<String, String>>> raftProcesses = clusterProcesses.stream()
          .filter(process -> processes.contains(process.getId()))
          .collect(Collectors.toList());

      // vybereme log nejakeho procesu
      Map<String, String> snapshotCheck = raftProcesses.get(0).getLastSnapshotOfLog().get();

      // jsou vsechny snapshoty stejne
      if (raftProcesses.stream()
          .map(RaftProcess::getLastSnapshotOfLog)
          .map(Optional::get)
          .allMatch(snapshotCheck::equals)) {
        correctResults = match(expectedResults, snapshotCheck);
      }
    }

    // vysledek evaluace
    if (correctResults) {
      System.out.println("Vsechny procesy, ktere nas zajimali, maji stejnou databazi a shoduji se "
          + "s ocekavanymi vysledky.");
      return true;
    } else {
      System.err.println("Aspon dva procesy, ktere nas zajimali, maji ruznou databazi nebo se "
          + "neshoduji s ocekavanymi vysledky.");
      return false;
    }
  }

  /**
   * V metode zkontrolujeme, jestli bylo mozne vygenerovat snapshot, ktery obsahuji s procesy s
   * ohledem na zasilane pozadavky a poradi (kombinaci) jejich prijimani. Je treba pocitat s ruznymi
   * kombinacemi vysledku, nemame zarucene poradi (ani pri vyzvedavani zprav z inboxu)
   */
  private boolean match(Map<String, List<String>> expectedResults,
      Map<String, String> snapshotCheck) {
    for (Entry<String, List<String>> entry : expectedResults.entrySet()) {
      String value = snapshotCheck.get(entry.getKey());
      if (!entry.getValue().contains(value)) {
        return false;
      }
    }
    return true;
  }

}
