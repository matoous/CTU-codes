package cz.cvut.fel.agents.pdv.evaluation;

import cz.cvut.fel.agents.pdv.evaluation.leader.ClientCoordinatorForLeaderSelection;
import cz.cvut.fel.agents.pdv.evaluation.leader.ScenarioForLeaderSelection;
import cz.cvut.fel.agents.pdv.evaluation.log.ClientCoordinatorForLogReplication;
import cz.cvut.fel.agents.pdv.evaluation.log.ScenarioForLogReplication;
import cz.cvut.fel.agents.pdv.evaluation.store.ClientCoordinatorForKVStore;
import cz.cvut.fel.agents.pdv.evaluation.store.ScenarioForKVStore;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * Zacnete prostudovanim oficialniho zadani na strankach predmetu, kde najdete kompletni zadani
 * semestralni ulohy s tipy pro jeji reseni.
 *
 * Vasi implementaci budeme testovat ve 3 scenarich (s ruznou konfiguraci systemu). Nasim cilem je,
 * abyste zvladli implementovat jednoduchou distribuovanou key/value databazi s garancemi podle RAFT
 * (3. scenar). Scenare na sebe navazuji a dovedou Vas az k implementaci takoveto databaze.
 *
 * Spusteni testovacich scenaru. Zatim jsou pritomny pouze 2 (bude i 3) s ukazkami zakladni
 * konfigurace.
 *
 * Zacnete tim, ze se podivate na student.ClusterProcess, kde najdete dalsi navod. Podivejte se take
 * na implementace scenaru, abyste zjistili, co delaji jednotlive parametry.
 */
public class RaftRun {

  public static void main(String[] args) {

    printLineDelimiter();

    // Scenar: Volba leadera
    ScenarioForLeaderSelection scenarioForLeaderSelection = ScenarioForLeaderSelection.builder()
        .requestPeriod(1)
        .countOfFailures(1)
        .lengthOfFailure(20)
        .spanBetweenFailures(50)
        .stepsOfSimulation(120)
        .expectedNumberOfProcessesInPartition(3)
        .reconnectProcess(false)
        .build();
    RaftSimulation<ClientCoordinatorForLeaderSelection> simulationLeader = new RaftSimulation<>(
        new RaftDSConfiguration<>(scenarioForLeaderSelection, "Volba leadera"));
    simulationLeader.run();

    printLineDelimiter();

    // Scenar: Replikace logu napric clusterem
    ScenarioForLogReplication scenarioForLogReplication = ScenarioForLogReplication.builder()
        .countOfFailures(1)
        .lengthOfFailure(20)
        .spanBetweenFailures(50)
        .expectedNumberOfProcessesInPartition(3)
        .stepsOfSimulation(200)
        .stopRequestBeforeEndOfSimulation(80)
        .reconnectProcess(true)
        .retryInterval(1)
        .maxUncommittedRequest(1)
        .build();
    RaftSimulation<ClientCoordinatorForLogReplication> simulationLog = new RaftSimulation<>(
        new RaftDSConfiguration<>(scenarioForLogReplication, "Replikace logu napric clusterem"));
    simulationLog.run();

    printLineDelimiter();

    // Scenar: Distribuovana key/value databaze
    ScenarioForKVStore scenarioForKVStore = ScenarioForKVStore.builder()
        .countOfFailures(1)
        .lengthOfFailure(20)
        .spanBetweenFailures(50)
        .expectedNumberOfProcessesInPartition(3)
        .stepsOfSimulation(250)
        .stopRequestBeforeEndOfSimulation(100)
        .reconnectProcess(false)
        .retryInterval(1)
        .maxUncommittedRequest(1)
        .requestPeriod(3)
        .build();
    RaftSimulation<ClientCoordinatorForKVStore> simulationStore = new RaftSimulation<>(
        new RaftDSConfiguration<>(scenarioForKVStore, "Funkcni key/value databaze"));
    simulationStore.run();

    printLineDelimiter();

  }

  /**
   * Vytiskne oddelovac radku pro lepsi prehlednost mezi senari
   */
  private static void printLineDelimiter() {
    String del = IntStream.range(0, 40)
        .boxed()
        .map(i -> "-")
        .collect(Collectors.joining(""));
    System.out.println(del);
  }

}
