package cz.cvut.fel.agents.pdv.failure_detector;

import cz.cvut.fel.agents.pdv.dsand.Simulation;

/**
 * Trida s main metodou pro spusteni simulace
 */
public class MainFD {

    public static void main(String[] args) {
        // v simulaci muzete pocitat s tim, ze v nejhorsim pripade se zprava zpozdi, ale nikdy se neztrati
        Simulation sim = new Simulation(new DetectorDSConfig(5, 0.1, 2));
        sim.run();
    }
}
