package cz.cvut.fel.agents.pdv.raft.messages;

import cz.cvut.fel.agents.pdv.dsand.Message;

/**
 * Sablona pro zpravy mezi klinty a procesy v clusteru
 */
public abstract class ClientServerMessage extends Message {

  private final String requestId;

  ClientServerMessage(String requestId) {
    this.requestId = requestId;
  }

  public String getRequestId() {
    return requestId;
  }
}
