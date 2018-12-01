package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class AckMessage extends Message {

    private final String processID;

    public AckMessage(String processID) {
        this.processID = processID;
    }

    public String getProcessID() {
        return processID;
    }

    @Override
    public String toString() {
        return "ACK " + this.processID;
    }
}