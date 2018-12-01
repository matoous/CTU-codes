package cz.cvut.fel.agents.pdv.pdv_cloud.vector;

import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IClock;
import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IProcessClock;

import java.util.Arrays;

import static java.lang.Math.max;

/**
 * Trida reprezentujici vektorove hodiny procesu.
 */
public class VectorClock implements IProcessClock<int[]> {

    private final int[] vectorTime;

    // id procesu - vlastnika hodin
    private final int myID;

    public VectorClock(int processes, int myID) {
        this.vectorTime = new int[processes];
        this.myID = myID;
    }

    @Override
    public void update(IClock<int[]> timestamp) {

        // TODO
        // implementujte pravidlo pro aktualizaci lokalnich hodin
        // na zaklade casove znamky z prijate zpravy
        int[] times = timestamp.getTime();
        for(int i = 0; i < times.length; i++){
            vectorTime[i] = max(vectorTime[i], times[i]);
        }
    }

    @Override
    public void onNewEvent() {
        vectorTime[myID]++;
    }

    @Override
    public IClock<int[]> getAsTimeStamp() {
        return new VectorTimestamp(vectorTime.clone());
    }

    @Override
    public int[] getTime() {
        return vectorTime;
    }

    @Override
    public boolean isCausalityForProcessViolated(IClock<int[]> timestamp, int process) {

        int[] times = timestamp.getTime();
        for(int i = 0; i < times.length; i++){
            if(times > )
        }
        return false;
    }

    @Override
    public String toString() {
        return Arrays.toString(vectorTime);
    }
}
