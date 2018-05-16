package cz.cvut.fel.agents.pdv.dsand;

import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.function.Function;
import java.util.stream.Collectors;

public abstract class Simulation<V extends DSConfig> implements ITickCounter {

  protected final V config;
  protected final Map<String, DSProcess> processes;
  protected final CommunicationLayer comm;

  public Simulation(V config) {
    this.config = config;
    this.comm = new CommunicationLayer(config, config.getProcessIDs());
    this.processes = Arrays.stream(config.getProcessIDs())
        .collect(Collectors.toMap(Function.identity(), id -> instantiateProcess(id,
            comm.getInboxOfAgent(id),
            (String r, Message m) -> comm.send(id, r, m, getPartitioning(), getDeadProcesses()),
            config.getProcessIDs())));
  }

  protected abstract DSProcess instantiateProcess(String id, Queue<Message> inbox,
      BiConsumer<String, Message> outbox, String[] directory);

  public abstract void run();

  public abstract long getCurrentTick();

  protected abstract List<Set<String>> getPartitioning();

  protected abstract Set<String> getDeadProcesses();

}
