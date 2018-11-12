package cz.cvut.fel.agents.pdv.failure_detector;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.MessageWrapper;

import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * Konfigurace simulace...
 */
public class DetectorDSConfig implements cz.cvut.fel.agents.pdv.dsand.DSConfig {

    private static final Random RANDOM = new Random();
    private final int numberOfProcesses;

    // pravdepodobnost ukonceni nejakeho procesu
    private final double pptOfFailure;

    // ukonceny proces, ktery mate detekovat. na zacatku zadny neni :)
    private Integer killedProcess = null;

    // maximalni zpozdeni zpravy
    private final int delay;

    public DetectorDSConfig(int numberOfProcesses, double pptOfFailure, int delay) {
        this.numberOfProcesses = numberOfProcesses;
        this.pptOfFailure = pptOfFailure;
        this.delay = delay;
    }

    @Override
    public String[] getProcessIDs() {
        // vrati id procesu v rozsahu 0 az pocet procesu
        return IntStream.range(0, numberOfProcesses)
                .boxed()
                .map(Object::toString)
                .toArray(String[]::new);
    }

    @Override
    public DSProcess instantiateProcess(String id, Queue<Message> inbox,
                                        BiConsumer<String, Message> outbox, String[] directory) {
        // vytvor proces
        return new DetectorProcess(id, inbox, outbox, delay,
                // vytvori seznam id procesu, kde neni id aktualniho procesu
                IntStream.range(0, numberOfProcesses)
                        .boxed()
                        .map(Object::toString)
                        .filter(integer -> !integer.equals(id))
                        .collect(Collectors.toList()));
    }

    @Override
    public long getDeliveryDelay(String src, String dst) {
        // zpravy se zpozduji
        return 1 + RANDOM.nextInt(delay);
    }

    @Override
    public double getReliability(String src, String dst) {
        // zpravy se neztraceji
        return 1.0;
    }

    @Override
    public List<String> getProcessesToWake() {
        // pokud jsme jeste neukoncili proces, zkousime ho ukoncit
        if (killedProcess == null && RANDOM.nextDouble() < pptOfFailure) {
            killedProcess = RANDOM.nextInt(numberOfProcesses);
            System.out.println("Killing process " + killedProcess);
        }
        // vratime vsechny neukoncene procesy
        return IntStream.range(0, numberOfProcesses)
                .boxed()
                .filter(integer -> killedProcess == null || integer != killedProcess.intValue())
                .map(integer -> integer + "")
                .collect(Collectors.toList());
    }

    @Override
    public boolean isRunning() {
        return true;
    }
}
