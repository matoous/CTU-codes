package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Sablona pro odpoved serveru !!! V TOMTO PRIPADE JE 'requestId' ROVNO POZADAVKU KLIENTA, NA KTERY
 * ODPOVIDATE !!!
 */
public abstract class ServerResponse extends ClientServerMessage {

  ServerResponse(String requestId) {
    super(requestId);
  }
}
