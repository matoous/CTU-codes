package cz.cvut.fel.agents.pdv.raft.messages;

/**
 * Potvrzeni pozadavku - odpoved na ClientRequestWithContent, ktera chce vratit nejakou hodnotu.
 * Odpoved posilejte jen kdyz je pozadavek vykonan nebo byl vykonan - viz RAFT.
 */
public class ServerResponseWithContent<T> extends ServerResponse {

  private final T response;

  public ServerResponseWithContent(String requestId, T response) {
    super(requestId);
    this.response = response;
  }

  public T getResponse() {
    return response;
  }
}
