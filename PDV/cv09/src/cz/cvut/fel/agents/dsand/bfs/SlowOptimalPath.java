package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.Simulation;

public class SlowOptimalPath {
    public static void main(String[] args) {
        Simulation sim = new Simulation(new DSConfig(2, 1.0));
        sim.run();
    }
}
