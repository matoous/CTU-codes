package cz.cvut.fel.agents.pdv.dsand;

import java.util.Objects;
import java.util.Queue;
import java.util.function.BiConsumer;

public abstract class DSProcess {

  private boolean running = true;
  protected String id;
  protected Queue<Message> inbox;
  protected BiConsumer<String, Message> outbox;

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

  protected void send(String rcpt, Message message) {
    outbox.accept(rcpt, message);
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) {
      return true;
    }
    if (o == null || getClass() != o.getClass()) {
      return false;
    }
    DSProcess dsProcess = (DSProcess) o;
    return Objects.equals(id, dsProcess.id);
  }

  @Override
  public int hashCode() {

    return Objects.hash(id);
  }
}
