package cz.cvut.fel.agents.dsand;

import java.util.Queue;
import java.util.function.BiConsumer;

public abstract class DSProcess {
    private boolean running = true;
    protected String id;
    protected Queue<Message> inbox;
    protected BiConsumer<String,Message> outbox;

    public abstract void act();

    public DSProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox) {
        this.id = id;
        this.inbox = inbox;
        this.outbox = outbox;
    }

    public void terminateAll() {
        outbox.accept(id, new TerminateAllMessage());
    }

    public void terminate() {
        running = false;
    }

    public boolean isTerminated() {
        return !running;
    }

    protected void send(String rcpt, Message message){
        outbox.accept(rcpt, message);
    }
}
