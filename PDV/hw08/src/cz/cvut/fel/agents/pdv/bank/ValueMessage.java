package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class ValueMessage extends ClockedMessage {
    public int key;
    public int value;

    public ValueMessage(int key, int value) {
        this.key = key;
        this.value = value;
    }
}
