package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.dsand.Simulation;

public class Main {
    public static void main(String[] args) {
        Simulation sim = new Simulation(new BankConfig(4, 40));
        sim.run();
    }
}
