package cz.cvut.atg.zui.astar;

import eu.superhub.wp5.planner.planningstructure.GraphEdge;
import eu.superhub.wp5.planner.planningstructure.GraphNode;

import java.util.List;

/**
 * Interface for the ZUI task 1. Implement this interface to create your awesome planner.
 * Author: Ondrej Vanek (e-mail: ondrej.vanek@agents.fel.cvut.cz)
 * Date: 2/14/13
 * Time: 4:17 PM.
 */
public interface PlannerInterface<T> {

    /**
     * Plans the path given the graph, an origin and a destination.
     * @param graph the graph on which we plan.
     * @param origin our agent starts here.
     * @param destination ...and the agent tries to get here.
     * @return a plan leading from origin to destination.
     */
    public List<GraphEdge> plan(RoadGraph graph, GraphNode origin, GraphNode destination);

    /**
     * Returns your open list (so we can get the Counter).
     * @return open list.
     */
    public AbstractOpenList<T> getOpenList();
}
