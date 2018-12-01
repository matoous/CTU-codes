package cz.cvut.fel.agents.pdv.pdv_cloud.scalar;

import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IClock;
import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IProcessClock;

import static java.lang.Integer.max;

/**
 * Trida reprezentujici skalarni hodiny procesu.
 */
public class ScalarClock implements IProcessClock<Integer> {
    private int logicalTime = 0;

    @Override
    public void update(IClock<Integer> timestamp) {
        logicalTime = max(logicalTime, timestamp.getTime()) + 1;
    }

    @Override
    public void onNewEvent() {
        logicalTime++;
    }

    @Override
    public IClock<Integer> getAsTimeStamp() {
        return new ScalarTimestamp(logicalTime);
    }

    @Override
    public Integer getTime() {
        return logicalTime;
    }

    @Override
    public String toString() {
        return "" + logicalTime;
    }

    @Override
    public boolean isCausalityForProcessViolated(IClock<Integer> timestamp, int process) {

        if(logicalTime > process){
            return true;
        }
        return false;
    }

}
