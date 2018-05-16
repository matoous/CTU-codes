package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.raft.messages.IOperation;

/**
 * Operace, ktere chceme, aby vykonaval nas cluster
 */
public enum StateEnums implements IOperation {
  // Operace: PUT(KEY,VALUE) - nahradi nebo nahraje hodnotu ke klici
  FOLLOWER,
  // Operace: APPEND(KEY,VALUE) - pokud klic neni v databazi - nahraje hodnotu ke klici, jinak
  // zretezi aktualni hodnotu pod klicem se zadanou
  CANDIDATE,
  // Operace: GET(KEY) - vrati aktualni hodnotu prirazenou ke klici
  LEADER;

  @Override
  public int getOrder() {
    return this.ordinal();
  }

  @Override
  public String getName() {
    return this.name();
  }
}
