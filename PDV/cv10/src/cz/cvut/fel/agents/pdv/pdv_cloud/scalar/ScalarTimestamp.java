package cz.cvut.fel.agents.pdv.pdv_cloud.scalar;

import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IClock;

/**
 * Casova znamka pro skalarni hodiny, ktera se posila ve zprave
 */
public class ScalarTimestamp implements IClock<Integer> {

    private final int value;

    public ScalarTimestamp(int value) {
        this.value = value;
    }

    @Override
    public Integer getTime() {
        return value;
    }

    @Override
    public String toString() {
        return "" + value;
    }
}
