package cz.cvut.fel.agents.pdv.evaluation;

import cz.cvut.fel.agents.pdv.raft.ClientCoordinator;

/**
 * Strategie pro vytvoreni instance ClientCoordinator, aby scenare mohly pouzivat koordinatory s
 * ruznou funkcionalitou (pro konkretni scenar)
 */
public interface IClientCoordinatorCreationStrategy<T extends ClientCoordinator<String>> {

  /**
   * Vytvori instanci ClientCoordinator
   */
  T createInstance();
}
