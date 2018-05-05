package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class PingRequest extends Message {

    private final String processID;

    public PingRequest(String processID) {
        this.processID = processID;
    }

    public String getProcessID() {
        return processID;
    }

    @Override
    public String toString() {
        return "PINGREQ " + this.processID;
    }
}