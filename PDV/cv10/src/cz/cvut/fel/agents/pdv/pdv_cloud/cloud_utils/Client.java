package cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils;

import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.Queue;
import java.util.function.BiConsumer;

/**
 * Klientsky proces. Klient uklada data v databazi, posila notifikace ostatnim klientum a stahuje si data z databaze
 * Typ T, L je genericky, s generikou jste se jiz setkali v C++ v podobe templatu. My zde pouzivame genericky typ pro
 * vyuziti dvou typu hodin.
 */
public class Client<L, T extends IProcessClock<L>> extends CloudProcess<T> {

    private final double pptOfUpdate;
    private boolean notifyAnotherClient = false;

    // adresy uzlu v siti
    private final String anotherClientAddress;
    // lokalni uloziste
    private final String localStorageAddress;

    //lokalni hodnota, kterou zadal klient
    private int value = 0;

    //muze upravovat hodnoty
    private boolean canEdit;

    public Client(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, T clock,
                  double pptOfUpdate, String anotherClientAddress, String localStorageAddress,
                  boolean canEdit) {
        super(id, inbox, outbox, clock);
        this.pptOfUpdate = pptOfUpdate;
        this.anotherClientAddress = anotherClientAddress;
        this.localStorageAddress = localStorageAddress;
        this.canEdit = canEdit;
    }

    @Override
    public void act() {

        // v predchozi iteraci jsme poslali data na databazi, notifikuj klienta na druhem konci sveta,
        // ze data se zmenila
        if (notifyAnotherClient) {

            //s novou udalosti inkrementujeme lokalni hodiny
            clock.onNewEvent();

            //posleme zpravu klientovi
            send(anotherClientAddress, new SystemMessage<>(SystemMessage.Messages.REFRESH, clock.getAsTimeStamp(), value));
            notifyAnotherClient = false;

            System.out.println(clock.getAsTimeStamp().toString() + ": Klient " + id + " posila zadost o refresh klientovi "
                    + anotherClientAddress);
        }

        if (canEdit && RANDOM.nextDouble() <= pptOfUpdate) {
            //uzivatel zadal nova data, posleme je na databazi
            value = RANDOM.nextInt();

            //s novou udalosti inkrementujeme lokalni hodiny
            clock.onNewEvent();

            //posleme zpravu ulozisti s hodnotou k ulozeni
            send(localStorageAddress, new SystemMessage<>(SystemMessage.Messages.SAVE, clock.getAsTimeStamp(), value));
            notifyAnotherClient = true;
            canEdit = false;

            System.out.println(clock.getAsTimeStamp().toString() + ": Klient " + id + " uklada hodnotu " + value + " do "
                    + localStorageAddress);
        }

        // cteni zprav
        while (!inbox.isEmpty()) {

            // nacteme zpravu z fronty
            SystemMessage<IClock<L>> message = (SystemMessage<IClock<L>>) inbox.poll();

            //upravime lokalni hodiny podle zpravy
            clock.update(message.getTimestamp());

            if (message.getType().equals(SystemMessage.Messages.REFRESH)) {
                // symetricky klient nam rika, abychom si updatovali hodnotu
                //posleme pozadavek na databazi

                //nova udalost
                clock.onNewEvent();

                send(localStorageAddress,
                        new SystemMessage<>(SystemMessage.Messages.READ, clock.getAsTimeStamp(), message.getContent()));

                System.out.println(clock.getAsTimeStamp().toString() + ": Klient " + id + " posila zadost o cteni hodnoty na " + localStorageAddress);
            } else if (message.getType().equals(SystemMessage.Messages.PRINT)) {

                //nova udalost
                clock.onNewEvent();

                //vytiskneme obsah zpravy
                System.out.println(clock.getAsTimeStamp().toString() + ": Klient " + id + " obdrzel hodnotu k vytisteni: "
                        + message.getContent() + " od " + message.sender);
                canEdit = true;
            }

        }

    }
}
