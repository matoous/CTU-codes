package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Pozadavek klienta na proces v clusteru - s otazkou, kdo je leader Na tento pozadavek byste
 * klientovi meli idealne poslat ServerResponseLeader instanci s odpovedi, kdo je leader (muze byt i
 * null, pokud ho proces nevi)
 */
public class ClientRequestWhoIsLeader extends ClientRequest {

  public ClientRequestWhoIsLeader(String requestId) {
    super(requestId);
  }
}
