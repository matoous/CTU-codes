package cz.cvut.fel.agents.dsand.demo;

import cz.cvut.fel.agents.dsand.Simulation;

public class Main {
    public static void main(String[] args) {
        Simulation sim = new Simulation(new DSConfig());
        sim.run();
    }
}
