package cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils;

import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.Queue;
import java.util.function.BiConsumer;

/**
 * Databazovy proces. Databaze uklada data od klienta, posila vyzadana data klientovi a snazi se udrzet data
 * konzistentni napric databazemi pomoci replikace.
 * Typ T, L je genericky, s generikou jste se jiz setkali v C++ v podobe templatu. My zde pouzivame genericky typ pro
 * vyuziti dvou typu hodin.
 */
public class Database<L, T extends IProcessClock<L>> extends CloudProcess<T> {

    // dalsi uloziste s replikaci dat
    private final String anotherStorageAddress;

    //nastav databazi na 0
    private int database = 0;

    //maji se hodnoty poslat k replikaci
    private boolean replicate = false;
    private final int replicationsFromClient;

    public Database(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, T clock,
                    String anotherStorageAddress, int replicationsFromClient) {
        super(id, inbox, outbox, clock);
        this.anotherStorageAddress = anotherStorageAddress;
        this.replicationsFromClient = replicationsFromClient;
    }

    @Override
    public void act() {

        // posleme data k replikaci
        if (replicate) {
            //nova udalost
            clock.onNewEvent();

            System.out.println(
                    clock.getAsTimeStamp().toString() + ": Databaze " + id + " posila zadost k replikaci na "
                            + anotherStorageAddress);

            send(anotherStorageAddress, new SystemMessage<>(SystemMessage.Messages.REPLICATE, clock.getAsTimeStamp(), database));
            replicate = false;
        }

        //prijmeme pozadavky k zapisu a cteni
        while (!inbox.isEmpty()) {

            // nacteme zpravu z fronty
            SystemMessage<IClock<L>> message = (SystemMessage<IClock<L>>) inbox.poll();

            boolean violation = message.getType().equals(SystemMessage.Messages.REPLICATE) && clock
                    .isCausalityForProcessViolated(message.getTimestamp(), replicationsFromClient);

            //upravime lokalni hodiny podle zpravy
            clock.update(message.getTimestamp());

            if (message.getType().equals(SystemMessage.Messages.SAVE)) {

                // prepisujeme data
                database = message.getContent();

                //nova udalost
                clock.onNewEvent();

                // v pristim case je replikujeme. chceme, aby byla konzistentni napric databazemi
                replicate = true;

                System.out.println(clock.getAsTimeStamp().toString() + ": Databaze " + id + " uklada hodnotu " + message
                        .getContent() + " od " + message.sender);

            } else if (message.getType().equals(SystemMessage.Messages.REPLICATE)) {

                // zkontrolujeme hodnoty
                if (violation) {
                    System.err.println("Poruseni kauzality bylo detekovano.");
                }

                // dostali jsme data, chceme, aby byla konzistentni napric databazemi
                database = message.getContent();

                //nova udalost
                clock.onNewEvent();

                System.out.println(clock.getAsTimeStamp().toString() + ": Databaze " + id + " replikuje hodnotu " + message
                        .getContent() + " od " + message.sender);

            } else if (message.getType().equals(SystemMessage.Messages.READ)) {

                //nova udalost
                clock.onNewEvent();
                send(message.sender, new SystemMessage<>(SystemMessage.Messages.PRINT, clock.getAsTimeStamp(), database));

                if(!message.sender.equals("B")) {
                    System.out.println(clock.getAsTimeStamp().toString() + ": Databaze " + id + " posila hodnotu " + database
                            + " na klienta " + message.sender);
                }
            }
        }

    }

}
