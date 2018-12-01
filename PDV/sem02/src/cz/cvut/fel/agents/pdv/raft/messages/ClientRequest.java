package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Abstraktni trida pozadavku klienta na proces v clusteru
 */
public abstract class ClientRequest extends ClientServerMessage {

  ClientRequest(String requestId) {
    super(requestId);
  }
}
