package cz.cvut.fel.agents.pdv.raft;

import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseConfirm;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseWithContent;

/**
 * Kontrakt pro ClientCoordinator - pro nakladani s odpovedmi z procesu v clusteru. Vice v tride
 * ClientCoordinator. Kontrakt slouzi pro koordinaci mezi koordinatorem klientu a klienty
 */
public interface IServerResponseLogger<K> {

  /**
   * Metoda pro vyrizeni potvrzeni, ktere obdrzel klient
   */
  void handleServerResponse(Client<K> client, ServerResponseConfirm responseConfirm,
      long currentTick);

  /**
   * Metoda precte odpoved od serveru a vrati leadera na zaklade odpovedi
   */
  String handleServerResponse(Client<K> client, ServerResponseLeader responseLeader,
      long currentTick);

  /**
   * Metoda pro vyrizeni odpovedi s obsahem, kterou obdrzel klient
   */
  void handleServerResponse(Client<K> client,
      ServerResponseWithContent<K> serverResponseWithContent, long currentTick);

  /**
   * Smazani pozadavku - napr. aby nedochazelo k opakovanym odeslanim stejneho pozadavku
   */
  void deleteRequest(Client<K> client, String id);

}
