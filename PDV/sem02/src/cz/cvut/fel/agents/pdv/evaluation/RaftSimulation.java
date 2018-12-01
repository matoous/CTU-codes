package cz.cvut.fel.agents.pdv.evaluation;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Simulation;
import cz.cvut.fel.agents.pdv.raft.Client;
import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;
import cz.cvut.fel.agents.pdv.raft.ILeaderCheck;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import cz.cvut.fel.agents.pdv.student.ClusterProcess;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Queue;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * Trida k vykonani a evaluaci simulace na zaklade konfigurace
 */
public class RaftSimulation<T extends ClientCoordinator<String>> extends
    Simulation<RaftDSConfiguration<T>> implements ILeaderCheck {

  // vlastnosti site
  private final List<Set<String>> partitions;
  private final Set<String> deadProcesses;

  // koordinator klientu
  private final T clientCoordinator;

  // parametry selhani
  private long failureTime = 0;
  // na zacatku je hodnota prazdna - k zadnemu selhani nedoslo
  private Optional<FailureEnums> failure = Optional.empty();
  private int countOfFailures = 0;
  private boolean recoveredFromFailure = true;

  // krok simulace
  private long tick = 0;

  RaftSimulation(RaftDSConfiguration<T> config) {
    super(config);

    // nova instance koordinatora klientu
    this.clientCoordinator = config.createInstanceClientCoordinator();

    // na zacatku jsou vsechny procesy v jedne siti a zadny neni mrtvy
    this.deadProcesses = new HashSet<>();
    this.partitions = Stream.of(config.getProcessesInCluster())
        .collect(Collectors.toList());

    // zaregistruj procesy
    this.config.getProcessesInCluster().forEach(clientCoordinator::addServer);

    // zaregistruj klienty
    this.getClients().forEach(this.clientCoordinator::addClient);

    // urci klientum koordinatora
    this.getClients().forEach(client -> client.setServerResponseLogger(clientCoordinator));
  }

  public void run() {
    System.out.println(config.getSimulationName());

    // po konecnem poctu kroku simulaci ukoncime
    do {

      // rozesleme zpravy a navysime hodiny
      comm.deliveryCycle();

      // vsechny funkcni procesy vykonaji svuj kod
      Arrays.stream(config.getProcessIDs())
          // preskocime mrtve mprocesy
          .filter(s -> !deadProcesses.contains(s))
          .forEach(agent -> processes.get(agent).act());

      // chceme, aby se prihodila nejaka nehoda
      if (!failure.isPresent()) {

        // stala se nejaka nehoda?
        failure = config.failureEvent(getCurrentTick(), countOfFailures, failureTime);

        if (failure.isPresent()) {
          System.out.println(getCurrentTick() + ": nova nehoda - "
              + failure.get().getFailureName());
          countOfFailures++;
          failureTime = getCurrentTick();
          recoveredFromFailure = false;

          // nastaveni systemu podle nehody
          switch (failure.get()) {
            case PROCESS_FAILURE:
              deadProcesses.clear();
              deadProcesses.addAll(config.getDeadProcesses());
              System.out.println("Selhaly procesy: " + deadProcesses.stream()
                  .collect(Collectors.joining(",")));
              break;
            case NETWORK_PARTITION:
              partitions.clear();
              partitions.addAll(config.getPartition());
              System.out.println("Podsite: " + partitions.stream()
                  .map(strings -> strings.stream()
                      .collect(Collectors.joining(",", "(", ")")))
                  .collect(Collectors.joining(",")));
              break;
          }
        }
      } else if (config.shouldRecoverFromFailure(getCurrentTick(), failureTime, failure.get())) {

        // zotavujeme se z nehody
        System.out.println(getCurrentTick() + ": zotavuji se z nehody - "
            + failure.get().getFailureName());
        failureTime = getCurrentTick();
        recoveredFromFailure = true;
        switch (failure.get()) {
          case PROCESS_FAILURE:
            deadProcesses.clear();
            break;
          case NETWORK_PARTITION:
            partitions.clear();
            partitions.add(config.getProcessesInCluster());
            break;
        }
        failure = Optional.empty();
      }

      // vygenerujeme pozadavky klientum a znovu posleme ty, u kterych vyprsel cas na odpoved
      clientCoordinator.act(getCurrentTick());

      tick++;
    } while (config.isRunning(getCurrentTick()));

    // vyhodnoceni simulace
    System.out.println("Trvani simulace " + getCurrentTick());
    boolean correct = config.evaluateSimulation(getClients(), getClusterProcesses(),
        clientCoordinator, getPartitioning(), getDeadProcesses(), recoveredFromFailure);
    if (correct) {
      System.out.println("Vysledek simulace: OK");
    } else {
      System.err.println("Vysledek simulace: CHYBA");
    }
  }

  @Override
  public long getCurrentTick() {
    return tick;
  }

  @Override
  protected DSProcess instantiateProcess(String id, Queue<Message> inbox,
      BiConsumer<String, Message> outbox, String[] directory) {

    // vytvoreni procesu - klienta nebo procesu v clusteru
    if (config.getClients().contains(id)) {
      return new Client<>(id, inbox, outbox, new ArrayList<>(config.getProcessesInCluster()),
          this, this);
    } else if (config.getProcessesInCluster().contains(id)) {
      return new ClusterProcess(id, inbox, outbox, config.getProcessesInCluster().stream()
          .filter(s -> !s.equals(id))
          .collect(Collectors.toList()), config.networkDelays());
    }
    return null;
  }

  @Override
  protected List<Set<String>> getPartitioning() {
    return partitions;
  }

  @Override
  protected Set<String> getDeadProcesses() {
    return deadProcesses;
  }

  private List<Client<String>> getClients() {
    return config.getClients().stream()
        .map(s -> (Client<String>) processes.get(s))
        .collect(Collectors.toList());
  }

  private List<RaftProcess<Map<String, String>>> getClusterProcesses() {
    return config.getProcessesInCluster().stream()
        .map(s -> (RaftProcess<Map<String, String>>) processes.get(s))
        .collect(Collectors.toList());
  }

  @Override
  public boolean shouldSendToThisLeader(String leader) {
    return !deadProcesses.contains(leader) && partitions.stream()
        .filter(strings -> strings.contains(leader))
        .findAny().map(Set::size).orElse(0) >= config.majority();
  }
}
