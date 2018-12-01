package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.exclusion.ExclusionPrimitive;

import java.util.Arrays;
import java.util.Comparator;

public class MultiLock {

    private ExclusionPrimitive[] locks;
    private int requested = -1;
    private int held = -1;

    public MultiLock(ExclusionPrimitive... locks) {
        this.locks = Arrays.copyOf(locks, locks.length);

        // Zamky 'locks' budeme zamykat v lexikografickem poradi (podle jmena kriticke sekce)
        Arrays.sort(this.locks, Comparator.comparing((ExclusionPrimitive x) -> x.getName()));

        update();
    }

    public boolean isHeld() {
        update();
        return held+1 == locks.length;
    }

    public void update() {
        if(requested == held) {
            // Ziskali jsme vsechny pozadovane zamky - muzeme pozadat o dalsi zamek v poradi.
            if(requested+1 < locks.length) locks[++requested].requestEnter();
        } else if(locks[requested].isHeld()) {
            // Dostali jsme prideleny dalsi zamek ze seznamu
            held = requested;

            // Zavolame znovu metodu update, abychom pozadali o dalsi zamek v poradi.
            update();
        }
    }

    public void exit() {
        if(!isHeld()) {
            throw new RuntimeException("Releasing MultiLock that is not yet held!");
        }

        for(ExclusionPrimitive rap : locks) {
            // Uvolnime vsechny zamky
            rap.exit();
        }
    }

}
