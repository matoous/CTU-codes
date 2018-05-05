package cz.cvut.fel.agents.pdv.clocked;

import cz.cvut.fel.agents.pdv.dsand.Message;

/**
 * Typ zpravy, ktery obsahuje logicky (Lamportuv) cas odeslani a prijeti. Vsechny Vase
 * zpravy by mely dedit od teto tridy.
 */
public class ClockedMessage extends Message {
    public int sentOn;      // Cas prijeti
    public int receivedOn;  // Cas odeslani
}
