package cz.cvut.fel.agents.pdv.dsand;

import java.util.HashMap;
import java.util.Map;

public class Simulation {

    private final DSConfig config;

    private final Map<String,DSProcess> processes;
    private final CommunicationLayer comm;

    private boolean terminated = false;


    public Simulation(DSConfig config) {
        this.config = config;

        comm = new CommunicationLayer(config, config.getProcessIDs());

        processes = new HashMap<>();
        for(String id : config.getProcessIDs()) {
            final String s = id;
            processes.put(id, config.instantiateProcess(
                    id,
                    comm.getInboxOfAgent(id),
                    (String r, Message m) -> {
                        if(m instanceof TerminateAllMessage) terminated = true;
                        else comm.send(s,r,m);
                    },
                    config.getProcessIDs()));
        }
    }

    public void run() {
        while(config.isRunning() && !allAgentsTerminated()) {
            comm.deliveryCycle();

            for(String agent : config.getProcessesToWake()) {
                processes.get(agent).act();
                if(terminated) break;
            }

            try {
                Thread.sleep(200);
            } catch(InterruptedException ie) {}
        }
    }

    protected boolean allAgentsTerminated(){
        if(terminated) return true;

        boolean allAgentsTerminated = true;
        for(DSProcess DSProcess : processes.values()){
            if (!DSProcess.isTerminated()){
                allAgentsTerminated = false;
                break;
            }
        }
        return allAgentsTerminated;
    }

}
