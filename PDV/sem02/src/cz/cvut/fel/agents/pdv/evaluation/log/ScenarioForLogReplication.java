package cz.cvut.fel.agents.pdv.evaluation.log;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.evaluation.Scenario;
import cz.cvut.fel.agents.pdv.evaluation.ScenarioBuilder;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * Scenar replikace logu.
 */
public class ScenarioForLogReplication extends Scenario<ClientCoordinatorForLogReplication> {

  // maximalni pocet nezpracovanych pozadavku - <0,N>
  private final int maxUncommittedRequest;

  /**
   * Vrati instanci stavitele pro nakonfigurovani a nasledne vytvoreni instance scenare
   */
  public static ScenarioBuilder<ScenarioForLogReplication> builder() {
    return new ScenarioBuilder<ScenarioForLogReplication>() {
      @Override
      public ScenarioForLogReplication build() {
        return new ScenarioForLogReplication(numberOfProcessesInCluster,
            reliabilityOfNetworkConnections, processToKill, restartProcess, reconnectProcess,
            expectedNumberOfProcessesInPartition, stepsOfSimulation, networkDelays, lengthOfFailure,
            spanBetweenFailures, countOfFailures, numberOfClients, requestPeriod,
            stopRequestBeforeEndOfSimulation, retryInterval, maxRequestsPerTick,
            maxUncommittedRequest);
      }
    };
  }

  private ScenarioForLogReplication(int numberOfProcessesInCluster,
      double reliabilityOfNetworkConnections, int processToKill, boolean restartProcess,
      boolean reconnectProcess, int expectedNumberOfProcessesInPartition, long stepsOfSimulation,
      int networkDelays, int lengthOfFailure, int spanBetweenFailure, int countOfFailure,
      int numberOfClients, int requestPeriod, int stopRequestBeforeEndOfSimulation,
      int retryInterval, int maxRequestsPerTick, int maxUncommittedRequest) {
    super(() -> new ClientCoordinatorForLogReplication(retryInterval,
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
      ClientCoordinatorForLogReplication clientCoordinator, List<Set<String>> partitions,
      Set<String> deadProcesses, boolean recoveredFromFailure) {

    // v metode overujeme, ze procesy urcily sveho leadra
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
    String logs = clusterProcesses.stream()
        .map(mapRaftProcess -> mapRaftProcess.getId() + ": " +
            mapRaftProcess.getLastSnapshotOfLog().get().get("A"))
        .collect(Collectors.joining(", "));
    System.out.println("Logy procesu: " + logs);
    String expectedLog = clientCoordinator.getExpectedResults().stream()
        .map(strings -> strings.stream()
            .collect(Collectors.joining("|")))
        .collect(Collectors.joining(","));
    System.out.println("Kombinace v ocekavanem logu: " + expectedLog);

    boolean correctResults = false;

    // system se zotavil ze selhani nebo k nemu ani nedoslo, data by mela byt konzistentni
    // v takovem pripade by vsechny logy mely byt shodne
    if (recoveredFromFailure) {

      // vybereme log nejakeho procesu
      String snapshotCheck = clusterProcesses.get(0).getLastSnapshotOfLog().get().get("A");

      // jsou vsechny snapshoty stejne
      if (clusterProcesses.stream()
          .map(RaftProcess::getLastSnapshotOfLog)
          .map(map -> map.get().get("A"))
          .allMatch(snapshotCheck::equals)) {
        correctResults = match(clientCoordinator, snapshotCheck);
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
      String snapshotCheck = raftProcesses.get(0).getLastSnapshotOfLog().get().get("A");

      // jsou vsechny snapshoty stejne
      if (raftProcesses.stream()
          .map(RaftProcess::getLastSnapshotOfLog)
          .map(map -> map.get().get("A"))
          .allMatch(snapshotCheck::equals)) {
        correctResults = match(clientCoordinator, snapshotCheck);
      }
    }

    // vysledek evaluace
    if (correctResults) {
      System.out.println("Vsechny procesy, ktere nas zajimali, maji stejny log a shoduji se "
          + "s ocekavanymi vysledky.");
      return true;
    } else {
      System.err.println("Aspon dva procesy, ktere nas zajimali, maji ruzny log nebo se "
          + "neschoduji s ocekavanymi vysledky.");
      return false;
    }
  }

  /**
   * V metode zkontrolujeme, jestli bylo mozne vygenerovat snapshot, ktery obsahuji s procesy s
   * ohledem na zasilane pozadavky a poradi (kombinaci) jejich prijimani. Je treba pocitat s ruznymi
   * kombinacemi vysledku, nemame zarucene poradi (ani pri vyzvedavani zprav z inboxu)
   */
  private boolean match(ClientCoordinatorForLogReplication clientCoordinator,
      String snapshotCheck) {
    int index = 0;
    for (int i = 0; i < clientCoordinator.getExpectedResults().size(); i++) {

      // znaky, ktere ocekavame v subsekvenci
      List<String> expectedSequence = new ArrayList<>(
          clientCoordinator.getExpectedResults().get(i));

      // cast podsekvence ke kontrole
      String sequence = snapshotCheck.substring(index, index + expectedSequence.size());

      // kontrola, zda jsou pritomny vsechny ocekavane znaky v subsekvenci
      for (String anExpectedSequence : expectedSequence) {
        int ind = sequence.indexOf(anExpectedSequence);
        if (ind >= 0) {
          StringBuilder sb = new StringBuilder(sequence);
          sb.deleteCharAt(ind);
          sequence = sb.toString();
        } else {
          return false;
        }
      }
      index = index + expectedSequence.size();
    }
    return true;
  }

}
