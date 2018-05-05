package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import java.util.List;
import java.util.Queue;
import java.util.function.BiConsumer;

/**
 * Implementace Failure Detectoru podle SWIM protokolu
 */
public class FailureDetectorProcess extends DSProcess {

  // proces, kteremu se reportuji
  private final String disseminationProcess;

  // implementace metody act
  private final ActStrategy actStrategy;

  // maximalni hranice poctu odeslanych zprav pro tento proces
  private final int upperBoundOnMessages;

  // citac odeslanych zprav
  private int messageCounter = 0;

  public FailureDetectorProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
      String disseminationProcess, ActStrategy actStrategy, int upperBoundOnMessages) {
    super(id, inbox, outbox);
    this.disseminationProcess = disseminationProcess;
    this.actStrategy = actStrategy;
    this.upperBoundOnMessages = upperBoundOnMessages;
  }

  @Override
  public void act() {

    // zavolani vasi implementace metody act
    List<Pair<String, Message>> messagesToSend = actStrategy.act(inbox, disseminationProcess);
    // posleme zpravy
    messagesToSend.forEach(pair -> send(pair.getFirst(), pair.getSecond()));

    // kontrola, ze nedoslo k prekroceni celkoveho maximalniho poctu odeslanych zprav
    messageCounter = messageCounter + messagesToSend.size();
    if (messageCounter > upperBoundOnMessages) {
      System.err.println("Proces " + id + " prekrocil hranici povoleneho poctu odeslanych zprav.");
      terminateAll();
    }
  }
}
