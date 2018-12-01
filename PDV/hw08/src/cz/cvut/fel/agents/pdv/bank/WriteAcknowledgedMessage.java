package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class WriteAcknowledgedMessage extends ClockedMessage {
    public int key;

    public WriteAcknowledgedMessage(int key) {
        this.key = key;
    }
}
