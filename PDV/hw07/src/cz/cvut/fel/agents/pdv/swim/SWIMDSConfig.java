package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.HashSet;
import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 * Konfiguracni trida pro scenar ulohy
 */
public class SWIMDSConfig implements cz.cvut.fel.agents.pdv.dsand.DSConfig {

    private static final Random rnd = new Random();

    private final String disseminationProcessId = "0";
    // maximalni zpozdovani zprav
    private final int maxDelay;
    // kolik je casu na detekovani mrtveho procesu od doby jeho ukonceni
    private final int timeToDetectKilledProcess;
    // vsechny procesy krome disseminationProcess
    private final List<String> processes;
    // kolik procesu ma byt ukonceno
    private final int processesToKill;
    // pravdepodobnost, ze v case t nejaky proces selze
    private final double pptOfFailure;
    // limit na pocet odeslanych zprav pro jeden proces
    private final int upperBoundOnMessages;
    // mnozina ukoncenych procesu
    private final Set<String> killedProcesses = new HashSet<>();
    private DisseminationProcess disseminationProcess = null;
    // sledovani chybne detekovanych procesu
    private final int accuracy;

    // spolehlivosti jednotlivych linku
    private final double[][] reliabilities;

    public SWIMDSConfig(int maxDelay, int processesToKill, double pptOfFailure, int processes,
                        int upperBoundOnMessages, double minReliability, int accuracy) {
        this.maxDelay = maxDelay;
        this.processesToKill = processesToKill;
        this.pptOfFailure = pptOfFailure;
        this.processes = IntStream.range(1, processes + 1)
                .boxed()
                .map(integer -> integer + "")
                .collect(Collectors.toList());

        // davame vam dostatecnou toleranci, abyste zvladli detekovat ukonceny proces
        this.timeToDetectKilledProcess = (maxDelay + 2) * 9;
        this.upperBoundOnMessages = upperBoundOnMessages;
        this.accuracy = accuracy;

        reliabilities = new double[processes][processes];
        for(int i = 0 ; i < processes ; i++) {
            for(int j = 0 ; j < processes ; j++) {
                if(i == j) {
                    // Loopback je zcela spolehlivy
                    reliabilities[i][j] = 1.0;
                } else if(i < j) {
                    // Urcite procento spojeni je nespolehlivych
                    reliabilities[i][j] = (rnd.nextDouble() < 0.2) ? minReliability : 1.0;
                } else {
                    // Uvazujeme, ze spolehlivost spojeni i->j je stejna jako spolehlivost spojeni j->i
                    reliabilities[i][j] = reliabilities[j][i];
                }
            }
        }
    }

    @Override
    public String[] getProcessIDs() {
        return Stream.concat(Stream.of(disseminationProcessId), processes.stream())
                .toArray(String[]::new);
    }

    @Override
    public DSProcess instantiateProcess(String id, Queue<Message> inbox,
                                        BiConsumer<String, Message> outbox, String[] directory) {
        if (id.equals(disseminationProcessId)) {
            // inicializujeme disseminationProcess
            disseminationProcess = new DisseminationProcess(id, inbox, outbox, processes,
                    timeToDetectKilledProcess, processesToKill, accuracy);
            return disseminationProcess;
        } else {
            // inicializujeme instanci FailureDetectorProcess s vasi implementaci ActStrategy
            return new FailureDetectorProcess(id, inbox, outbox, disseminationProcessId,
                    new ActStrategy(maxDelay + 1,
                            processes.stream()
                                    .filter(s -> !s.equals(id))
                                    .collect(Collectors.toList()), timeToDetectKilledProcess, upperBoundOnMessages),
                    upperBoundOnMessages);
        }
    }

    @Override
    public long getDeliveryDelay(String src, String dst) {
        // simulace zpozdovani zprav
        return rnd.nextInt(maxDelay) + 1;
    }

    @Override
    public double getReliability(String src, String dst) {
        // v pripade, ze prijemce/odesilatel je disseminationProcess, zpravy se neztraceji
        if (src.equals(disseminationProcessId) || dst.equals(disseminationProcessId)) {
            return 1;
        }
        return reliabilities[Integer.parseInt(src)-1][Integer.parseInt(dst)-1];
    }

    @Override
    public List<String> getProcessesToWake() {
        // dokud jsme jeste neukoncili vsechny pozadovane procesy, pokousime se sukoncit novy proces
        if (killedProcesses.size() < processesToKill && pptOfFailure >= rnd.nextDouble()) {
            // dostupne procesy
            List<String> available = processes.stream()
                    .filter(s -> !killedProcesses.contains(s))
                    .collect(Collectors.toList());
            // z dostupnych vybereme nahodne jeden proces a ten ukoncime
            String toKill = available.get(rnd.nextInt(available.size()));
            killedProcesses.add(toKill);
            disseminationProcess.killedProcessNotification(toKill);
            System.out.println("Ukoncuji proces: " + toKill);
        }

        // navysime citac disseminationProcess
        disseminationProcess.incrementCounter();

        // vratime vsechny procesy krome ukoncenych...
        return Stream.concat(Stream.of(disseminationProcessId), processes.stream())
                .filter(s -> !killedProcesses.contains(s))
                .collect(Collectors.toList());
    }

    @Override
    public boolean isRunning() {
        return true;
    }
}
