package cz.cvut.fel.agents.dsand.bfs;

import java.util.Arrays;
import java.util.List;

public class Graph {

    public static List<List<Integer>> getGraph() {
        return Arrays.asList(
                Arrays.asList(1, 4),
                Arrays.asList(0, 4, 5, 6, 2),
                Arrays.asList(1, 6, 7, 8, 3),
                Arrays.asList(2, 8, 9, 10, 11),
                Arrays.asList(0, 1, 5),
                Arrays.asList(1, 4, 6),
                Arrays.asList(1, 2, 5, 7),
                Arrays.asList(2, 6, 8),
                Arrays.asList(2, 3, 7, 9),
                Arrays.asList(3, 8, 10),
                Arrays.asList(3, 9, 11),
                Arrays.asList(3, 10)
        );
    }

}
