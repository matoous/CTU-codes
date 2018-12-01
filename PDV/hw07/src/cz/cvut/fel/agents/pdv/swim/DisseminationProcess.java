package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.function.BiConsumer;

/**
 * Proces, kteremu se reportuji "mrtve" procesy. Proces posila multi-cast vsem ostatnim procesum o
 * umrti detekovaneho procesu.
 */
public class DisseminationProcess extends DSProcess {
    private final List<String> processes;
    private final Set<String> reportedProcesses = new HashSet<>();
    private final Map<String, Integer> failedProcesses = new HashMap<>();

    // casovy interval na detekci mrtveho procesu
    private final int timeToDetectKilledProcess;
    private int counter = 0;
    private final int processesToKill;
    private int lastKilledProcessDetected = -1;

    // sledovani chybne detekovanych procesu
    private final int accuracy;
    private int wrongDetections = 0;

    public DisseminationProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                                List<String> processes, int timeToDetectKilledProcess, int processesToKill, int accuracy) {
        super(id, inbox, outbox);
        this.processes = processes;
        this.timeToDetectKilledProcess = timeToDetectKilledProcess;
        this.processesToKill = processesToKill;
        this.accuracy = accuracy;
    }

    @Override
    public void act() {

        while (!inbox.isEmpty()) {
            Message message = inbox.poll();
            if (message instanceof PFDMessage) {
                // Precti informaci o havarovanem procesu (alespon podle detekce jednoho z procesu)
                String detectedProcess = ((PFDMessage) message).getDetectedProcess();
                if (!failedProcesses.containsKey(detectedProcess)) {
                    // tento proces neni ukonceny

                    wrongDetections++;
                    System.err.println("Proces " + detectedProcess + " byl detekovan chybne jako ukonceny.");

                    if (wrongDetections >= accuracy) {
                        // byl prekrocen limit na presnost
                        System.err.println("Prekrocen limit chybne detekovanych procesu.");
                        terminateAll();
                    }
                } else if (!reportedProcesses.contains(detectedProcess)) {
                    // "mrtvy" proces byl spravne detekovan

                    System.out.println("Spravne detekovan ukonceny proces " + detectedProcess
                            + " za " + failedProcesses.get(detectedProcess) + " jednotek casu. (tick  " + this.counter + ")");
                    reportedProcesses.add(detectedProcess);

                    // byl detekovan posledni proces
                    if (reportedProcesses.size() == processesToKill) {
                        lastKilledProcessDetected = counter;
                    }

                    // rozesli zpravu o ukoncenem procesu
                    DeadProcessMessage msg = new DeadProcessMessage(detectedProcess);
                    processes.stream()
                            // nebudeme posilat zpravu "mrtvym" procesum
                            .filter(s -> !failedProcesses.containsKey(s))
                            .forEach(s -> send(s, msg));
                }
            }
        }

        // zkontroluj, ze vsechny "mrtve" procesy byly detekovany v casovem intervalu
        failedProcesses.entrySet().stream()
                // spravne reportovane procesy naz nezajimaji
                .filter(entry -> !reportedProcesses.contains(entry.getKey()))
                // chceme jen procesy, u kterych byl prekrocen interval
                .filter(entry -> entry.getValue() + timeToDetectKilledProcess <= counter)
                .forEach(entry -> {
                    System.err.println("Nepodarilo se detekovat mrtvy proces "
                            + entry.getKey() + " v casovem intervalu.");
                    terminateAll();
                });

        // byl detekovan posledni proces a ubehl nejaky cas od teto udalosti
        if (reportedProcesses.size() == processesToKill && lastKilledProcessDetected != -1
                && lastKilledProcessDetected + 30 <= counter) {
            // reseni proslo
            System.out.println("OK");
            terminateAll();
        }
    }

    public void killedProcessNotification(String idOfKilledProcess) {
        failedProcesses.put(idOfKilledProcess, counter);
    }

    public void incrementCounter() {
        counter++;
    }

}
