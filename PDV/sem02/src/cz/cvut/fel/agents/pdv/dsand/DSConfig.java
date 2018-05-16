package cz.cvut.fel.agents.pdv.dsand;

import java.util.List;
import java.util.Set;

public interface DSConfig {

  String[] getProcessIDs();

  long getDeliveryDelay(String src, String dst);

  double getReliability(String src, String dst, List<Set<String>> partitioning);

  boolean isRunning(long simulationSteps);

}
