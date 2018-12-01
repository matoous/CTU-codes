package cz.cvut.fel.agents.pdv.dsand;

import java.util.List;
import java.util.Queue;
import java.util.function.BiConsumer;

public interface DSConfig {

    String[] getProcessIDs();
    DSProcess instantiateProcess(String id, Queue<Message> inbox, BiConsumer<String,Message> outbox, String[] directory);

    long getDeliveryDelay(String src, String dst);
    double getReliability(String src, String dst);

    List<String> getProcessesToWake();

    boolean isRunning();

}
