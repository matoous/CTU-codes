package cz.cvut.atg.zui.astar;

import eu.superhub.wp5.graphcommon.graph.Graph;
import eu.superhub.wp5.planner.planningstructure.GraphEdge;
import eu.superhub.wp5.planner.planningstructure.GraphNode;

import java.util.Collection;
import java.util.List;

/**
 * Class encapsulating a road graph provided by the OSM parser and providing necessary methods to the user.
 * The graph is simple, oriented, with multiple components, contains cycles.
 * Author: Ondrej Vanek (e-mail: ondrej.vanek@agents.fel.cvut.cz)
 * Date: 2/14/13
 * Time: 4:18 PM.
 */
public class RoadGraph {

    /**
     * The graph data structure.
     */
    private Graph<GraphNode,GraphEdge> graph;

    public RoadGraph(Graph<GraphNode, GraphEdge> graph) {
        this.graph = graph;
    }

    /**
     * gets all edges in the graph.
     * @return  all edges in the graph.
     */
    public Collection<GraphEdge> getAllEdges() {
        return graph.getAllEdges();
    }

    /**
     * Gets all nodes in the graph.
     * @return all nodes in the graph.
     */
    public Collection<GraphNode> getAllNodes(){
        return graph.getAllNodes();
    }

    /**
     * Gets a node with a specific ID (this ID is taken from the OSM maps).
     * @param id
     * @return a node with a specific ID.
     */
    public GraphNode getNodeByNodeId(long id) {
        return graph.getNodeByNodeId(id);
    }

    /**
     * Gets an edge leading from a node with ID "from" to a node with ID "to".
     * @param from
     * @param to
     * @return
     */
    public GraphEdge getEdge(long from, long to) {
        return graph.getEdges(from, to);
    }

    /**
     * All edges going from a node with ID "nodeId".
     * @param nodeId
     * @return
     */
    public List<GraphEdge> getNodeOutcomingEdges(long nodeId) {
        return graph.getNodeOutcomingEdges(nodeId);
    }

    /**
     * All edge going to a node with ID "nodeId".
     * @param nodeId
     * @return
     */
    public List<GraphEdge> getNodeIncomingEdges(long nodeId) {
        return graph.getNodeIncomingEdges(nodeId);
    }

}
