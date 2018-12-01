package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class ReadMessage extends ClockedMessage {
    public int key;

    public ReadMessage(int key) {
        this.key = key;
    }
}
