package cz.cvut.fel.agents.pdv.raft;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;
import java.util.Optional;
import java.util.Queue;
import java.util.function.BiConsumer;

/**
 * Sablona pro proces v clusteru, ktery budete implementovat podle RAFT protokolu. S timto procesem
 * budou komunikovat klienti
 *
 * Pro vice detailu se podivejte do souboru student.ClusterProcess, ktery budete implementovat a
 * evaluation.RaftRun s popisem ulohy
 */
public abstract class RaftProcess<T> extends DSProcess {

  protected RaftProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox) {
    super(id, inbox, outbox);
  }

  /**
   * Metoda by mela vratit posledni zaznam v logu Pokud je log prazdny, vratte Optional.empty(),
   * jinak posledni zaznam "zablate" do Optional
   */
  public abstract Optional<T> getLastSnapshotOfLog();

  /**
   * Metoda k navraceni leadera procesu
   */
  public abstract String getCurrentLeader();

}
