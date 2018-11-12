package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.DSProcess;
import cz.cvut.fel.agents.dsand.Message;

import java.util.*;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;

public class DSConfig implements cz.cvut.fel.agents.dsand.DSConfig {

    private final int num_agents;
    private final String[] agents;
    private final List<List<String>> graph;
    private final Map<String,Integer> agentMap;

    private final int optPathDelay;
    private final double node2Reliability;

    public DSConfig(int optPathDelay, double node2Reliability){
        this.optPathDelay = optPathDelay;
        this.node2Reliability = node2Reliability;

        List<List<Integer>> graph = Graph.getGraph();
        num_agents = graph.size();
        agents = new String[num_agents];
        agentMap = new HashMap<>();
        for(int i = 0; i < num_agents; i++) {
            agents[i] = Integer.toString(i);
            agentMap.put(agents[i], i);
        }

        this.graph = graph.stream().map( adjList ->
                adjList.stream().map( idx -> agents[idx]).collect(Collectors.toList())
        ).collect(Collectors.toList());
    }

    @Override
    public String[] getProcessIDs() {
        return agents;
    }

    @Override
    public DSProcess instantiateProcess(String id, Queue<Message> inbox, BiConsumer<String,Message> outbox, String[] directory) {
        int idx = Arrays.asList(agents).indexOf(id);
        List<String> neighbors = graph.get(idx);
        return new BFSProcess(id, inbox, outbox, neighbors, idx == 0, idx == graph.size()-1);
    }

    @Override
    public long getDeliveryDelay(String src, String dst) {
        return agentMap.get(src) < 4 ? optPathDelay : 1;
    }

    @Override
    public double getReliability(String src, String dst) {
        return agentMap.get(src) == 2 ? node2Reliability : 1.0;
    }

    @Override
    public List<String> getProcessesToWake() {
        return Arrays.asList(agents);
    }

    @Override
    public boolean isRunning() {
        return true;
    }
}
