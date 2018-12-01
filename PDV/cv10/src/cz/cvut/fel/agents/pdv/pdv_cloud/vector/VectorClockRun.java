package cz.cvut.fel.agents.pdv.pdv_cloud.vector;

import cz.cvut.fel.agents.pdv.dsand.Simulation;

/**
 * Trida s main metodou pro spusteni simulace
 */
public class VectorClockRun {

    public static void main(String[] args) {
        Simulation sim = new Simulation(new VectorDSConfig());
        sim.run();
    }

}
