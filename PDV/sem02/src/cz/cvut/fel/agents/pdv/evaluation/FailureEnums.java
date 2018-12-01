package cz.cvut.fel.agents.pdv.evaluation;

/**
 * Vycisleni vsech moznych selhani v systemu
 */
public enum FailureEnums {
  // rozdeleni site - system se rozpadne na subsystemy bez moznosti vzajemen komunikace
  NETWORK_PARTITION("rozdeleni site"),
  // havarovane procesy - havaruji procesy - nektere najednou prestanou fungovat
  PROCESS_FAILURE("havarovane procesy");

  private final String failureName;

  FailureEnums(String failureName) {
    this.failureName = failureName;
  }

  public String getFailureName() {
    return failureName;
  }
}
