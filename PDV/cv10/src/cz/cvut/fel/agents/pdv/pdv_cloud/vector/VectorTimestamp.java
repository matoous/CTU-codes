package cz.cvut.fel.agents.pdv.pdv_cloud.vector;

import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IClock;

import java.util.Arrays;

/**
 * Casova znamka pro vektorove hodiny, ktera se posila ve zprave
 */
public class VectorTimestamp implements IClock<int[]> {

    private final int[] vec;

    public VectorTimestamp(int[] vec) {
        this.vec = vec;
    }

    @Override
    public int[] getTime() {
        return vec;
    }

    @Override
    public String toString() {
        return Arrays.toString(vec);
    }
}
