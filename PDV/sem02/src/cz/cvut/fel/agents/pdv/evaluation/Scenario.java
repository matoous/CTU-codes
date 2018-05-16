package cz.cvut.fel.agents.pdv.evaluation;

import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Abstraktni trida pro konfiguraci scenare
 */
public abstract class Scenario<T extends ClientCoordinator<String>> {

  // strategie pro vytvoreni koordinatora klientu
  private final IClientCoordinatorCreationStrategy<T> clientCoordinator;
  // pocet procesu v clusteru - N
  private final int numberOfProcessesInCluster;
  // spolehlivost spojeni mezi procesy - (0,1>
  private final double reliabilityOfNetworkConnections;
  // kolik procesu se ma ukoncit - <0,N>
  private final int processToKill;
  // maji se ukoncene procesy restartovat
  private final boolean restartProcess;
  // dojde k opetovnemu spojeni site
  private final boolean reconnectProcess;
  // pocet procesu, ktere ocekavame, ze budou v jedne podsiti.
  // tyto procesy komunikuji jen v ramci podsite - <0,numberOfProcessesInCluster>
  private final int expectedNumberOfProcessesInPartition;
  // pocet kroku simulace - <1,N>
  private final long stepsOfSimulation;
  // spozdeni zprav v siti - pokud je 0, zpravy se nespozduji - <0,N>
  private final int networkDelays;
  // pocet kroku simulace trvani nehody - <0,N>
  private final int lengthOfFailure;
  // pocet kroku simulace mezi nehodami - <1,N>
  private final int spanBetweenFailures;
  // pocet nehod - <0,N>
  private final int countOfFailures;
  // pocet klientu - <1,N>
  private final int numberOfClients;

  protected Scenario(IClientCoordinatorCreationStrategy<T> clientCoordinator,
      int numberOfProcessesInCluster, double reliabilityOfNetworkConnections, int processToKill,
      boolean restartProcess, boolean reconnectProcess, int expectedNumberOfProcessesInPartition,
      long stepsOfSimulation, int networkDelays, int lengthOfFailure, int spanBetweenFailures,
      int countOfFailures, int numberOfClients) {
    this.clientCoordinator = clientCoordinator;
    this.numberOfProcessesInCluster = numberOfProcessesInCluster;
    this.reliabilityOfNetworkConnections = reliabilityOfNetworkConnections;
    this.processToKill = processToKill;
    this.restartProcess = restartProcess;
    this.reconnectProcess = reconnectProcess;
    this.expectedNumberOfProcessesInPartition = expectedNumberOfProcessesInPartition;
    this.stepsOfSimulation = stepsOfSimulation;
    this.networkDelays = networkDelays;
    this.lengthOfFailure = lengthOfFailure;
    this.spanBetweenFailures = spanBetweenFailures;
    this.countOfFailures = countOfFailures;
    this.numberOfClients = numberOfClients;
  }

  int numberOfProcessesInCluster() {
    return numberOfProcessesInCluster;
  }

  double reliabilityOfNetworkConnections() {
    return reliabilityOfNetworkConnections;
  }

  int processToKill() {
    return processToKill;
  }

  boolean restartProcess() {
    return restartProcess;
  }

  int expectedNumberOfProcessesInPartition() {
    return expectedNumberOfProcessesInPartition;
  }

  long stepsOfSimulation() {
    return stepsOfSimulation;
  }

  int networkDelays() {
    return networkDelays;
  }

  int lengthOfFailure() {
    return lengthOfFailure;
  }

  int spanBetweenFailures() {
    return spanBetweenFailures;
  }

  int countOfFailures() {
    return countOfFailures;
  }

  int numberOfClients() {
    return numberOfClients;
  }

  IClientCoordinatorCreationStrategy<T> getClientCoordinatorCreationStrategy() {
    return clientCoordinator;
  }

  boolean reconnectProcess() {
    return reconnectProcess;
  }

  /**
   * Metoda na vyhodnoceni simulace. Scenare maji ruzne evaluace
   */
  public abstract boolean evaluateSimulation(List<Client<String>> clients,
      List<RaftProcess<Map<String, String>>> clusterProcesses, T clientCoordinator,
      List<Set<String>> partitions, Set<String> deadProcesses, boolean recoveredFromFailure)
      throws Exception;

}
