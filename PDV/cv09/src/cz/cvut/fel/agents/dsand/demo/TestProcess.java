package cz.cvut.fel.agents.dsand.demo;

import cz.cvut.fel.agents.dsand.DSProcess;
import cz.cvut.fel.agents.dsand.Message;

import java.util.Queue;
import java.util.function.BiConsumer;

public class TestProcess extends DSProcess {
    private final int intId;
    private final String[] directory;
    private final int numAgents;

    private long counter = 0;

    public TestProcess(int id, Queue<Message> inbox, BiConsumer<String,Message> outbox, String[] directory, int numAgents) {
        super(Integer.toString(id), inbox, outbox);
        this.intId = id;
        this.directory = directory;
        this.numAgents = numAgents;
    }

    @Override
    public void act() {
        System.out.printf("Agent %s has %d messages in his inbox and acts\n", intId, inbox.size());
        if(intId == 0 && counter == 0){
            String rcpt = Character.toString((char) ('a' + (intId == (numAgents - 1) ? 0 : intId + 1)));
            outbox.accept(rcpt, new DummyMessage(++counter));
            System.out.printf("Sending %d to %s\n", counter, rcpt);
        }
        while(!inbox.isEmpty()) {
            System.out.println(inbox.poll());

            String rcpt = Character.toString((char) ('a' + (intId == (numAgents - 1) ? 0 : intId + 1)));
            send(rcpt, new DummyMessage(++counter));
            System.out.printf("Sending %d to %s\n", counter, rcpt);
        }
    }

    @Override
    public boolean isTerminated() {
        return false;
    }
}
