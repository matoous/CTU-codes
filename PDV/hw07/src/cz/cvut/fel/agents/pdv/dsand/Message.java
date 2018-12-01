package cz.cvut.fel.agents.pdv.dsand;

import java.io.Serializable;

public abstract class Message implements Serializable {
    public String sender;
    public String recipient;
}
