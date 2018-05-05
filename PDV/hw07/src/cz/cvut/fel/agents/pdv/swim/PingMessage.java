package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class PingMessage extends Message {

    private final String processID;

    public PingMessage(String processID) {
        this.processID = processID;
    }

    public String getProcessID() {
        return processID;
    }

    @Override
    public String toString() {
        return "PING " + this.processID;
    }
}