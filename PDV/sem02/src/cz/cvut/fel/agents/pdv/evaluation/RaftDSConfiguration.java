package cz.cvut.fel.agents.pdv.evaluation;

import cz.cvut.fel.agents.pdv.dsand.DSConfig;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Random;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 * Trida pro vytvoreni konfigurace simulace na zaklade scenare
 */
public class RaftDSConfiguration<T extends ClientCoordinator<String>> implements DSConfig {

  // konfigurace se inicializuje na zaklade scenare
  private final Scenario<T> configuration;

  private static final Random RANDOM = new Random();

  // procesy v systemu
  private final Set<String> clients;
  private final Set<String> processesInCluster;

  // nazev simulace
  private final String simulationName;

  RaftDSConfiguration(Scenario<T> configuration, String simulationName) {
    this.configuration = configuration;

    // identifikatory klientu - v podobe C_{poradi_klienta}
    this.clients = IntStream.range(0, configuration.numberOfClients())
        .boxed()
        .map(i -> "C_" + i)
        .collect(Collectors.toSet());

    // procesy v clusteru - v podobe CP_{poradi_procesu}
    this.processesInCluster = IntStream.range(0, configuration.numberOfProcessesInCluster())
        .boxed()
        .map(i -> "CP_" + i)
        .collect(Collectors.toSet());

    this.simulationName = simulationName;
  }

  @Override
  public String[] getProcessIDs() {
    return Stream.concat(clients.stream(), processesInCluster.stream())
        .toArray(String[]::new);
  }

  @Override
  public long getDeliveryDelay(String src, String dst) {

    // zpozdovani site - v nejlepsim pripade dorazi zprava v dalsim kroku simulace
    return Math.max(RANDOM.nextInt(configuration.networkDelays() + 1), 1);
  }

  @Override
  public double getReliability(String src, String dst, List<Set<String>> partitioning) {

    // klient je prijemce nebo odesilatel
    if (clients.contains(src) || clients.contains(dst)) {

      // v tomto pripade se zpravy neztraceji
      return 1.0;
    }

    // zjisti, v jake siti je odesilatel a prijemce
    int subnetOfSender = partitioning.size() <= 1 ? 0 : IntStream.range(0, partitioning.size())
        .boxed()
        .filter(i -> partitioning.get(i).contains(src))
        .findAny().get();
    int subnetOfDestination = partitioning.size() <= 1 ? 0 : IntStream.range(0, partitioning.size())
        .boxed()
        .filter(i -> partitioning.get(i).contains(dst))
        .findAny().get();

    if (subnetOfDestination != subnetOfSender) {

      // pokud jsou v jine subsiti, zprava se neposle - site jsou rozdelene
      return -1;
    }

    // spolehlivost komunikace mezi procesy v clusteru zavisi na konfiguraci
    return configuration.reliabilityOfNetworkConnections();
  }

  /**
   * Vrat rozdeleni systemu na podsite (na zaklade konfigurace)
   */
  public List<Set<String>> getPartition() {
    if (configuration.expectedNumberOfProcessesInPartition() == configuration
        .numberOfProcessesInCluster()) {

      // vratime celou sit, vsechny procesy jsou v podsiti
      return Collections.singletonList(processesInCluster);
    }

    List<String> processes = new ArrayList<>(processesInCluster);
    List<Set<String>> partitions = new ArrayList<>();

    // rozdelime procesy na podsite
    while (!processes.isEmpty()) {
      int partitionSize = Math
          .min(configuration.expectedNumberOfProcessesInPartition(), processes.size());
      Set<String> partition = new HashSet<>();
      for (int i = 0; i < partitionSize; i++) {
        String process = processes.remove(RANDOM.nextInt(processes.size()));
        partition.add(process);
      }
      partitions.add(partition);
    }
    return partitions;
  }

  /**
   * Na zaklade konfigurace se urci procesy, ktere maji selhat
   */
  public Set<String> getDeadProcesses() {
    if (processesInCluster.size() <= configuration.processToKill()) {
      return new HashSet<>(processesInCluster);
    }
    List<String> processes = new ArrayList<>(processesInCluster);
    int index = RANDOM.nextInt(processes.size());
    return IntStream.range(0, configuration.processToKill())
        .boxed()
        .map(i -> processes.get((index + i) % processes.size()))
        .collect(Collectors.toSet());
  }

  public boolean isRunning(long simulationSteps) {
    return simulationSteps < configuration.stepsOfSimulation();
  }

  public int networkDelays() {
    return configuration.networkDelays();
  }

  Set<String> getProcessesInCluster() {
    return processesInCluster;
  }

  Set<String> getClients() {
    return clients;
  }

  /**
   * Metoda vraci selhani systemu, ke kteremu ma dojit. Navratova hodnota je optional - muze byt
   * prazdna (nedojde k selhani)
   */
  public Optional<FailureEnums> failureEvent(long currentStep, int numberOfFailures,
      long lastEndedFailure) {

    // nema dojit k zadnemu selhani, uz vsechna probehla nebo selhani probehlo nedavno
    if (configuration.countOfFailures() <= numberOfFailures || (configuration.processToKill() <= 0
        && configuration.expectedNumberOfProcessesInPartition() == configuration
        .numberOfProcessesInCluster())
        || lastEndedFailure + configuration.spanBetweenFailures() > currentStep) {
      return Optional.empty();
    }

    // mozna selhani
    List<FailureEnums> failureEnums = new ArrayList<>();
    if (configuration.processToKill() > 0) {
      failureEnums.add(FailureEnums.PROCESS_FAILURE);
    }
    if (configuration.expectedNumberOfProcessesInPartition() < configuration
        .numberOfProcessesInCluster()) {
      failureEnums.add(FailureEnums.NETWORK_PARTITION);
    }

    // vyber selhani
    return Optional.of(failureEnums.get(RANDOM.nextInt(failureEnums.size())));
  }

  /**
   * Ma se system zotavit ze selhani
   */
  public boolean shouldRecoverFromFailure(long currentStep, long startOfFailure,
      FailureEnums failure) {
    return startOfFailure + configuration.lengthOfFailure() <= currentStep
        && ((failure.equals(FailureEnums.PROCESS_FAILURE) && configuration.restartProcess())
        || (failure.equals(FailureEnums.NETWORK_PARTITION) && configuration.reconnectProcess()));
  }

  /**
   * Vytvori novou instanci koordinatora na zaklade konfigurace
   */
  T createInstanceClientCoordinator() {
    return configuration.getClientCoordinatorCreationStrategy().createInstance();
  }

  /**
   * Kolik procesu je vetsina
   */
  public int majority() {
    return (configuration.numberOfProcessesInCluster() / 2) + (
        configuration.numberOfProcessesInCluster() % 2);
  }

  /**
   * Metoda k vyhodnoceni simulace
   */
  public boolean evaluateSimulation(List<Client<String>> clients,
      List<RaftProcess<Map<String, String>>> clusterProcesses, T clientCoordinator,
      List<Set<String>> partitions, Set<String> deadProcesses, boolean recoveredFromFailure) {
    try {
      return configuration.evaluateSimulation(clients, clusterProcesses, clientCoordinator,
          partitions, deadProcesses, recoveredFromFailure);
    } catch (Exception e) {
      System.err.println("Reseni vyhodilo vyjimku: " + e.toString());
      e.printStackTrace();
      return false;
    }
  }

  public String getSimulationName() {
    return simulationName;
  }
}
