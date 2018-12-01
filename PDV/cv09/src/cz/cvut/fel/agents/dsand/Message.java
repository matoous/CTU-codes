package cz.cvut.fel.agents.dsand;

import java.io.Serializable;

public abstract class Message implements Serializable {
    public String sender;
    public String recipient;
}
