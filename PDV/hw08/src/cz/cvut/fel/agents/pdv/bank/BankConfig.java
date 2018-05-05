package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.dsand.DSConfig;
import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.Arrays;
import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.function.BiConsumer;

public class BankConfig implements DSConfig {

    public int numAccounts;
    public int numOfficers;

    private String processes[];
    private String officers[];

    private Random rnd = new Random();

    public BankConfig(int numAccounts, int numOfficers) {
        this.numAccounts = numAccounts;
        this.numOfficers = numOfficers;

        processes = new String[numOfficers + 1];
        officers = new String[numOfficers];
        processes[0] = "Datastore";
        for(int i = 0 ; i < numOfficers ; i++) {
            officers[i] = "Officer" + (i+1);
            processes[i+1] = "Officer" + (i+1);
        }
    }

    @Override
    public String[] getProcessIDs() {
        return processes;
    }

    @Override
    public DSProcess instantiateProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, String[] directory) {
        if(id.startsWith("Officer")) {
            return new BankOfficerProcess(id, inbox, outbox, numAccounts, officers, "Datastore");
        } else {
            return new DatastoreProcess(id, inbox, outbox, numAccounts);
        }
    }

    @Override
    public long getDeliveryDelay(String src, String dst) {
        return rnd.nextInt(20);
    }

    @Override
    public double getReliability(String src, String dst) {
        return 1.0;
    }

    @Override
    public List<String> getProcessesToWake() {
        return Arrays.asList(processes);
    }

    @Override
    public boolean isRunning() {
        return true;
    }
}
