package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Potvrzeni pozadavku - odpoved na ClientRequestWithContent. Odpoved posilejte jen kdyz je
 * pozadavek vykonan nebo byl vykonan - viz RAFT.
 */
public class ServerResponseConfirm extends ServerResponse {

  public ServerResponseConfirm(String requestId) {
    super(requestId);
  }
}
