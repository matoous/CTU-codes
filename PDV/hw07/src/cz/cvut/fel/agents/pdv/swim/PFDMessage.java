package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

/**
 * Zprava pro DisseminationProcess obsahujici informaci o procesu, ktery byl detekovan jako
 * ukonceny. Pokud zpravu poslete DisseminationProcess, bude vzdy dorucena.
 */
public class PFDMessage extends Message {

  private final String detectedProcess;

  public PFDMessage(String detectedProcess) {
    this.detectedProcess = detectedProcess;
  }

  public String getDetectedProcess() {
    return detectedProcess;
  }
}
