package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;

import java.util.Arrays;
import java.util.Queue;
import java.util.function.BiConsumer;

public class DatastoreProcess extends ClockedProcess {

    private int accounts[];

    public DatastoreProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, int numAccounts) {
        super(id, inbox, outbox);

        accounts = new int[numAccounts];
        for(int i = 0 ; i < numAccounts ; i++) {
            accounts[i] = 100000;
        }
    }

    @Override
    public void act() {
        while(!inbox.isEmpty()) {
            ClockedMessage msg = receive();
            if(msg instanceof ReadMessage) {
                int key = ((ReadMessage)msg).key;
                System.out.printf("Reading accounts[%d]=%d on behalf of %s\n", key, accounts[key], msg.sender);

                increaseTime();
                send(msg.sender, new ValueMessage(key, accounts[key]));
            } else if(msg instanceof WriteMessage) {
                WriteMessage wm = (WriteMessage) msg;
                accounts[wm.key] = wm.value;
                System.out.printf("Writing accounts[%d]=%d on behalf of %s    Current balances: %s\n", wm.key, wm.value, wm.sender, Arrays.toString(accounts));

                increaseTime();
                send(msg.sender, new WriteAcknowledgedMessage(wm.key));
            } else {
                throw new RuntimeException("Unknown message of type " + msg.getClass().getSimpleName());
            }
        }
    }
}
