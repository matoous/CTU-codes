package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class RequestMessage extends ClockedMessage {
    public String sectionName;

    public RequestMessage(String sectionName){
        this.sectionName = sectionName;
    }

    @Override
    public String toString() {
        return "RQM: " + this.sectionName + " s: "+this.sentOn + " r: "+ this.receivedOn + " f: " +this.senderId() + " t: " +this.recipientId();
    }

    int senderId(){
        int num = 0;
        String d = this.sender;
        for(Character c : d.toCharArray()){
            if(c < '0' || c > '9') continue;
            num *= 10;
            num += Integer.parseInt(c.toString());
        }
        return num;
    }

    int recipientId(){
        int num = 0;
        String d = this.recipient;
        for(Character c : d.toCharArray()){
            if(c < '0' || c > '9') continue;
            num *= 10;
            num += Integer.parseInt(c.toString());
        }
        return num;
    }
}
