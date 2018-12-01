package cz.cvut.fel.agents.pdv.raft;

/**
 * Kontrakt pro kontrolu leadera - zda se mu maji odesilat zpravy
 */
public interface ILeaderCheck {

  /**
   * Ma se poslat zprava tomuto leaderovi - pokud leader neni mrtvy nebo v podsiti s mensionou, tak
   * mu ji posleme
   */
  boolean shouldSendToThisLeader(String leader);

}
