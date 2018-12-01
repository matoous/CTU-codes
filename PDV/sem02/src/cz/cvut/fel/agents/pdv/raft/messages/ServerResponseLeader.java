package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Odpoved na dotaz ClientRequestWhoIsLeader pripadne na pozadavek s operaci na server, ktery nesel
 * na leadera
 */
public class ServerResponseLeader extends ServerResponseWithContent<String> {

  public ServerResponseLeader(String requestId, String leader) {
    super(requestId, leader);
  }
}
