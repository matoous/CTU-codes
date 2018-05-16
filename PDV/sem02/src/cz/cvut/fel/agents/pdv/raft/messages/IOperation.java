package cz.cvut.fel.agents.pdv.raft.messages;

import java.io.Serializable;

/**
 * Kontrakt pro identifikaci operace, kterou ma provest proces v clusteru
 */
public interface IOperation extends Serializable {

  int getOrder();

  String getName();

}
