package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

/**
 * Zprava, kterou posila DisseminationProcess o mrtvych procesech. Tato zprava je od DisseminationProcess vzdy dorucena.
 */
public class DeadProcessMessage extends Message {

  private final String processID;

  public DeadProcessMessage(String processID) {
    this.processID = processID;
  }

  public String getProcessID() {
    return processID;
  }
}
