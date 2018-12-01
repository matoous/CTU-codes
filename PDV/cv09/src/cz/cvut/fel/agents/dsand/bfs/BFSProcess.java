package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.DSProcess;
import cz.cvut.fel.agents.dsand.Message;

import java.util.List;
import java.util.Queue;
import java.util.function.BiConsumer;

public class BFSProcess extends DSProcess {

    /**
     * Seznam nasledniku uzlu spravovaneho aktualnim procesem
     */
    private List<String> neighbors;

    /**
     * Je uzel spravovany aktualnim procesem 'root' prohledavani?
     */
    private boolean isRoot;

    /**
     * Je uzel spravovany aktualnim procesem cilovym uzlem?
     */
    private boolean isGoal;

    /**
     * Identifikator predchazejiciho procesu ('uzlu' na nejkratsi ceste)
     */
    private String predecessor = null;

    public BFSProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                      List<String> neighbors, boolean isRoot, boolean isGoal) {
        super(id, inbox, outbox);
        this.neighbors = neighbors;
        this.isRoot = isRoot;
        this.isGoal = isGoal;

        // Pro jednoduchost nastavime predchazeji proces korene na koren samotny.
        if(isRoot) predecessor = id;
    }

    @Override
    public void act() {
        System.out.println(id + " acting.");
        if(isGoal){
            System.out.println("DONE!\n");
            terminateAll();
            return;
        }

        for(Message m : inbox){
            if(!m.recipient.equals(id) && (this.predecessor == null)){
                send(m.recipient, m);
                break;
            }
        }

        for(String neightbor : neighbors){
            if(!neightbor.equals(predecessor)){
                Message m = new SearchMessage();
                send(neightbor, m);
            }
        }
        terminate();
    }

    @Override
    public boolean isTerminated() {
        return false;
    }
}
