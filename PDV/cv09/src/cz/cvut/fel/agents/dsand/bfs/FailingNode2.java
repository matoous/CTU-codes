package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.Simulation;

public class FailingNode2 {
    public static void main(String[] args) {
        Simulation sim = new Simulation(new DSConfig(1, 0.5));
        sim.run();
    }
}
