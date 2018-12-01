package cz.cvut.fel.agents.pdv.evaluation.leader;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.evaluation.Scenario;
import cz.cvut.fel.agents.pdv.evaluation.ScenarioBuilder;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * Scenar urceni leadera.
 */
public class ScenarioForLeaderSelection extends Scenario<ClientCoordinatorForLeaderSelection> {

  /**
   * Vrati instanci stavitele pro nakonfigurovani a nasledne vytvoreni instance scenare
   */
  public static ScenarioBuilder<ScenarioForLeaderSelection> builder() {
    return new ScenarioBuilder<ScenarioForLeaderSelection>() {
      @Override
      public ScenarioForLeaderSelection build() {
        return new ScenarioForLeaderSelection(numberOfProcessesInCluster,
            reliabilityOfNetworkConnections, processToKill, restartProcess, reconnectProcess,
            expectedNumberOfProcessesInPartition, stepsOfSimulation, networkDelays, lengthOfFailure,
            spanBetweenFailures, countOfFailures, numberOfClients, requestPeriod,
            stopRequestBeforeEndOfSimulation, retryInterval, maxRequestsPerTick);
      }
    };
  }

  private ScenarioForLeaderSelection(int numberOfProcessesInCluster,
      double reliabilityOfNetworkConnections, int processToKill, boolean restartProcess,
      boolean reconnectProcess, int expectedNumberOfProcessesInPartition, long stepsOfSimulation,
      int networkDelays, int lengthOfFailure, int spanBetweenFailure, int countOfFailure,
      int numberOfClients, int requestPeriod, int stopRequestBeforeEndOfSimulation,
      int retryInterval, int maxRequestsPerTick) {
    super(() -> new ClientCoordinatorForLeaderSelection(retryInterval,
            stepsOfSimulation - stopRequestBeforeEndOfSimulation, requestPeriod,
            maxRequestsPerTick), numberOfProcessesInCluster, reliabilityOfNetworkConnections,
        processToKill, restartProcess, reconnectProcess, expectedNumberOfProcessesInPartition,
        stepsOfSimulation,
        networkDelays, lengthOfFailure, spanBetweenFailure, countOfFailure, numberOfClients);
  }

  @Override
  public boolean evaluateSimulation(List<Client<String>> clients,
      List<RaftProcess<Map<String, String>>> clusterProcesses,
      ClientCoordinatorForLeaderSelection clientCoordinator, List<Set<String>> partitions,
      Set<String> deadProcesses, boolean recoveredFromFailure) {

    // v metode overujeme, ze procesy urcily sveho leadra
    System.out.println("Doslo k obnoveni site: " + (recoveredFromFailure ? "ano" : "ne"));

    // vypiseme leadery, ktere maji procesy
    String leaders = clusterProcesses.stream()
        .map(mapRaftProcess -> mapRaftProcess.getId() + " -> " + mapRaftProcess.getCurrentLeader())
        .collect(Collectors.joining(", "));
    System.out.println("leaderi pro procesy: " + leaders);

    boolean isLeaderSame = false;

    // system se zotavil ze selhani nebo k nemu ani nedoslo, data by mela byt konzistentni
    // v takovem pripade by vsechny procesy mely mit stejneho leadera
    if (recoveredFromFailure) {

      // leader prvniho procesu
      String leader = clusterProcesses.get(0).getCurrentLeader();

      // vsechny procesy urcily sveho leadera a leader je stejny
      if (clusterProcesses.stream()
          .map(RaftProcess::getCurrentLeader)
          .noneMatch(Objects::isNull)
          && clusterProcesses.stream()
          .map(RaftProcess::getCurrentLeader)
          .allMatch(s -> s.equals(leader))) {
        isLeaderSame = true;
      }
    } else {

      // nedoslo k zotaveni systemu. jeden spolecny leader neni vubec zaruceny, proto
      // overime, ze nejvetsi skupina - podsit nebo zive procesy - vybraly stejneho leadera

      //vsechny procesy, ktere nas zajimaji
      List<String> processes = !deadProcesses.isEmpty() ? clusterProcesses.stream()
          .map(DSProcess::getId)
          .filter(s -> !deadProcesses.contains(s))
          .collect(Collectors.toList()) : new ArrayList<>(partitions.stream()
          .max(Comparator.comparingInt(Set::size))
          .get());

      //urceny leader jednoho z nich
      String leader = clusterProcesses.stream()
          .filter(mapRaftProcess -> processes.contains(mapRaftProcess.getId()))
          .map(RaftProcess::getCurrentLeader)
          .findAny().orElse(null);

      // vsechny procesy, ktere nas zajimaji, urcily sveho leadera a leader je stejny
      if (clusterProcesses.stream()
          .filter(mapRaftProcess -> processes.contains(mapRaftProcess.getId()))
          .map(RaftProcess::getCurrentLeader)
          .noneMatch(Objects::isNull) && clusterProcesses.stream()
          .filter(mapRaftProcess -> processes.contains(mapRaftProcess.getId()))
          .map(RaftProcess::getCurrentLeader)
          .allMatch(s -> s.equals(leader))) {
        isLeaderSame = true;
      }
    }

    // vysledek evaluace
    if (isLeaderSame) {
      System.out.println("Vsechny procesy, ktere nas zajimali, urcily stejneho leadera.");
      return true;
    } else {
      System.err.println("Aspon dva procesy, ktere nas zajimali, maji ruzneho leadera.");
      return false;
    }
  }
}
