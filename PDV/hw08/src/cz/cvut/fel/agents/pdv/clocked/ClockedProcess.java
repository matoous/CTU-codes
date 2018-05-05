package cz.cvut.fel.agents.pdv.clocked;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.Queue;
import java.util.function.BiConsumer;

public abstract class ClockedProcess extends DSProcess {

    int currentTime = 0;

    public ClockedProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox) {
        super(id, inbox, outbox);
    }

    /**
     * Pomoci teto zpravy muzete prijmout dalsi zpravu z 'inbox' (Tato metoda nahrazuje volani 'inbox.poll()').
     * Po prijeti zpravy dojde automaticky k aktualizaci Lamportova logickeho casu.
     *
     * @return Prijata zprava
     */
    public ClockedMessage receive() {
        ClockedMessage msg = (ClockedMessage)inbox.poll();
        currentTime = Math.max(currentTime, msg.sentOn) + 1;
        msg.receivedOn = currentTime;

        return msg;
    }

    /**
     * Odeslani zpravy msg procesu s identifikatorem rcpt. Ke zprave se automaticky doplni cas jejiho odeslani.
     * POZOR! Muzete odesilat pouze zpravy typu ClockedMessage!
     *
     * @param rcpt Prijemce
     * @param msg  Zprava
     */
    @Override
    public void send(String rcpt, Message msg) {
        if(!(msg instanceof ClockedMessage)) throw new InvalidMessageException("Only ClockedMessage can be sent!");

        ClockedMessage cmsg = (ClockedMessage)msg;
        cmsg.sentOn = currentTime;

        super.send(rcpt, cmsg);
    }

    /**
     * Pred kazdou vyznamnou udalosti (zejmena odeslanim zpravy) byste meli inkrementovat logicky cas pomocit teto
     * metody.
     */
    public void increaseTime() {
        currentTime++;
    }

    /**
     * Metoda pro zjisteni aktualniho logickeho casu.
     *
     * @return Aktualni logicky cas
     */
    public int getTime() {
        return currentTime;
    }

}
