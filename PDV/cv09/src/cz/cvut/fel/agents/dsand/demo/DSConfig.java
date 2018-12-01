package cz.cvut.fel.agents.dsand.demo;

import cz.cvut.fel.agents.dsand.DSProcess;
import cz.cvut.fel.agents.dsand.Message;

import java.util.Arrays;
import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.function.BiConsumer;

public class DSConfig implements cz.cvut.fel.agents.dsand.DSConfig {
    private Random rnd = new Random();

    private int agentIdx = 0;

    @Override
    public String[] getProcessIDs() {
        return new String[] { "a", "b", "c" };
    }

    @Override
    public DSProcess instantiateProcess(String id, Queue<Message> inbox, BiConsumer<String,Message> outbox, String[] directory) {
        return new TestProcess(Arrays.asList(getProcessIDs()).indexOf(id), inbox, outbox, directory, getProcessIDs().length);
    }

    @Override
    public long getDeliveryDelay(String src, String dst) {
        return 1;//rnd.nextInt(10);
    }

    @Override
    public double getReliability(String src, String dst) {
        return 1.0;
    }

    @Override
    public List<String> getProcessesToWake() {
        return Arrays.asList("a", "b", "c");
    }

    @Override
    public boolean isRunning() {
        return true;
    }
}
