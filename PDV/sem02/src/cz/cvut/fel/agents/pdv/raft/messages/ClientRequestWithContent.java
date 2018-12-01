package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Pozadavek klienta na proces v clusteru, aby vykonal nejakou operaci s obsahem Na tento pozadavek
 * byste klientovi meli idealne poslat ServerResponseConfirm nebo ServerResponseWithContent v
 * zavislosti na operaci.
 *
 * Pokud proces neni leader, poslete v nejlepsim pripade ServerResponseLeader instanci s odpovedi,
 * kdo je leader (muze byt i null, pokud ho proces nevi)
 *
 * V kazdem pripade se ridte implementaci RAFT. Neposilejte potvrzeni nebo odpoved s obsahem na
 * pozadavek, ktery jeste nebyl proveden. Klient pocita se selhanim, tak Vam muze pozadavek poslat
 * vickrat
 */
public class ClientRequestWithContent<V extends IOperation, T> extends ClientRequest {

  private final T content;
  private final V operation;

  public ClientRequestWithContent(String requestId, V operation, T content) {
    super(requestId);
    this.content = content;
    this.operation = operation;
  }

  public T getContent() {
    return content;
  }

  public V getOperation() {
    return operation;
  }
}
