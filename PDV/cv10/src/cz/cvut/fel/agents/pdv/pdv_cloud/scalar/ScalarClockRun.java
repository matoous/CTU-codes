package cz.cvut.fel.agents.pdv.pdv_cloud.scalar;

import cz.cvut.fel.agents.pdv.dsand.Simulation;

/**
 * Trida s main metodou pro spusteni simulace
 */
public class ScalarClockRun {

    public static void main(String[] args) {
        Simulation sim = new Simulation(new ScalarDSConfigBombarding());
        sim.run();
    }

}
