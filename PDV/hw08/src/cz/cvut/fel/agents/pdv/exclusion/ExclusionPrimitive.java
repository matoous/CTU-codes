package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ExclusionPrimitive {

    private int intId;

    /**
     * Stavy, ve kterych se zamek muze nachazet.
     */
    enum AcquisitionState {
        RELEASED,    // Uvolneny   - Proces, ktery vlastni aktualni instanci ExclusionPrimitive o pristup do kriticke
                     //              sekce nezada

        WANTED,      // Chteny     - Proces, ktery vlastni aktualni instanci ExclusionPrimitive zada o pristup do
                     //              kriticke sekce. Ten mu ale zatim nebyl odsouhlasen ostatnimi procesy.

        HELD         // Vlastneny  - Procesu bylo prideleno pravo pristupovat ke sdilenemu zdroji.
    }

    private ClockedProcess owner;            // Proces, ktery vlastni aktualni instanci ExclusionPrimitive

    private String criticalSectionName;      // Nazev kriticke sekce. POZOR! V aplikaci se muze nachazet vice kritickych
                                             // sekci s ruznymi nazvy!

    private String[] allAccessingProcesses;  // Seznam vsech procesu, ktere mohou chtit pristupovat ke kriticke sekci
                                             // s nazvem 'criticalSectionName' (a tak spolurozhoduji o udelovani pristupu)

    private AcquisitionState state;          // Aktualni stav zamku (vzhledem k procesu 'owner').
                                             // state==HELD znamena, ze proces 'owner' muze vstoupit do kriticke sekce

    private Map<String, Boolean> oks;

    private int requestTime;

    private List<RequestMessage> fm;

    // Doplnte pripadne vlastni datove struktury potrebne pro implementaci
    // Ricart-Agrawalova algoritmu pro vzajemne vylouceni

    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;

        // Na zacatku je zamek uvolneny
        this.state = AcquisitionState.RELEASED;
        this.oks = new HashMap<>();
        this.fm = new ArrayList<>();
        this.requestTime = 0;

        this.intId = 0;
        for(Character c : this.owner.id.toCharArray()){
            if(c < '0' || c > '9') continue;
            this.intId *= 10;
            this.intId += Integer.parseInt(c.toString());
        }
    }

    /**
     * Metoda pro zpracovani nove prichozi zpravy
     *
     * @param m    prichozi zprava
     * @return 'true', pokud je zprava 'm' relevantni pro aktualni kritickou sekci.
     */
    public boolean accept(ClockedMessage m) {

        // Request message
        if(m instanceof RequestMessage){
            RequestMessage rm = (RequestMessage)m;
            if(!rm.sectionName.equals(this.criticalSectionName)) return false;

            if(this.state == AcquisitionState.HELD
                    || (this.state == AcquisitionState.WANTED
                        && (this.requestTime < rm.sentOn || (this.requestTime == rm.sentOn && this.intId < rm.senderId())))){
                fm.add(rm);
            } else {
                this.owner.send(rm.sender, new OkMessage(this.criticalSectionName));
            }
            return true;
        }

        // Ok message
        if(m instanceof OkMessage) {
            OkMessage om = (OkMessage) m;
            if (!this.criticalSectionName.equals(om.sectionName)) return false;

            this.oks.put(om.sender, true);

            // chack if all permissions granted
            boolean hasAll = true;
            for (Map.Entry<String, Boolean> e : this.oks.entrySet()) {
                if (!e.getValue()) {
                    hasAll = false;
                    break;
                }
            }
            if (hasAll) {
                this.owner.increaseTime();
                this.state = AcquisitionState.HELD;
            }
            return true;
        }
        return false;
    }

    public void requestEnter() {
        this.owner.increaseTime();
        this.state = AcquisitionState.WANTED;
        this.requestTime = this.owner.getTime();
        for(String other : this.allAccessingProcesses){
            if(other.equals(this.owner.id)) continue;
            this.owner.send(other, new RequestMessage(this.criticalSectionName));
            this.oks.put(other, false);
        }
    }

    public void exit() {
        this.owner.increaseTime();
        this.state = AcquisitionState.RELEASED;
        this.owner.increaseTime();
        for(RequestMessage m : this.fm){
            this.owner.send(m.sender, new OkMessage(this.criticalSectionName));
        }
        this.fm.clear();
    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }

}
