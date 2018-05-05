package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class OkMessage extends ClockedMessage {
    public String sectionName;

    public OkMessage(String sectionName){
        this.sectionName = sectionName;
    }
}
