package cz.cvut.atg.zui.astar;

import eu.superhub.wp5.graphcommon.graph.Graph;
import eu.superhub.wp5.graphcommon.graph.GraphBuilder;
import eu.superhub.wp5.planner.planningstructure.GraphEdge;
import eu.superhub.wp5.planner.planningstructure.GraphNode;
import eu.superhub.wp5.plannerdataimporter.kml.RoadGraphKmlCreator;
import org.openstreetmap.osm.data.coordinates.LatLon;
import student.Planner;

import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Executor of your planner. Wraps data file loading, calling your planner and extracting a KML file.
 * Author: Ondrej Vanek (e-mail: ondrej.vanek@agents.fel.cvut.cz)
 * Date: 2/12/13
 * Time: 11:08 AM.
 */
public class PlannerExecutor {

    public static final String DATA_FILE = "./data/ukhigh_filtered.dat";

    public static void main(String[] args) {
        PlannerExecutor executor = new PlannerExecutor();
        try {
            executor.run(DATA_FILE);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void run(String mapFileName) throws FileNotFoundException {
        // load the serialized graph
        Graph<GraphNode, GraphEdge> newroadGraph = deserialize(mapFileName);
        RoadGraph roadGraph = new RoadGraph(newroadGraph);

        System.out.println("Graph loaded.");

        //picks origin and destination nodes (feel free to modify)
        GraphNode origin = newroadGraph.getNodeByNodeId(26746953);
        GraphNode destination = newroadGraph.getNodeByNodeId(1037726044);

        System.out.println("Planning between: "+ origin.getId() + " and "+ destination.getId());

        //Here will be reference to your planner
        PlannerInterface astar = new Planner();

        long time = System.currentTimeMillis();
        //here you plan!
        List<GraphEdge> plan = astar.plan(roadGraph,origin, destination);
        System.out.println("Time taken: "+ (System.currentTimeMillis()-time));
        System.out.println("Added to Open: " + astar.getOpenList().getCounter().getCount()+" times.");

        printPlanProperties(origin,destination,plan);

        exportPlan(newroadGraph, origin, destination, plan);

    }

    /**
     * Prints properties of the plan.
     * @param origin origin node.
     * @param destination destination node.
     * @param plan plan to be analysed.
     */
    private void printPlanProperties(GraphNode origin, GraphNode destination, List<GraphEdge> plan) {
        if(plan!=null){

            double planLength = 0;
            double planTime = 0;

            for(GraphEdge edge: plan){
                planLength+=edge.getLengthInMetres()/1000;
                planTime +=edge.getLengthInMetres()/1000.0/edge.getAllowedMaxSpeedInKmph();
            }

            System.out.println("Plan length (km):" + planLength);
            System.out.println("Distance in air (km): "+ LatLon.distanceInMeters(origin.getLatitude(), origin.getLongitude(), destination.getLatitude(), destination.getLongitude())/1000.0);
            System.out.println("Time to travel (hrs): "+ planTime );
        } else{
            System.out.println("There is no plan!");
        }
    }

    /**
     * Creates a KML from a plan.
     * @param newroadGraph the graph on which it is planned.
     * @param origin origin node.
     * @param destination destination node.
     * @param plan plan to be exported.
     */
    private void exportPlan(Graph<GraphNode, GraphEdge> newroadGraph, GraphNode origin, GraphNode destination, List<GraphEdge> plan) {
        if(plan!=null){
            double planLength = 0;
            double planTime = 0;

            //create graph builder
            GraphBuilder<GraphNode,GraphEdge> gb = new GraphBuilder();
            gb.addNode(newroadGraph.getNodeByNodeId(plan.get(0).getFromNodeId()));

            //add all nodes in the plan
            for(GraphEdge edge: plan){
                gb.addNode(newroadGraph.getNodeByNodeId(edge.getToNodeId()));
                planLength+=edge.getLengthInMetres()/1000;
                planTime +=edge.getLengthInMetres()/1000.0/edge.getAllowedMaxSpeedInKmph();
            }
            //add all edges
            for(GraphEdge edge:plan){
                gb.addEdge(edge);
            }
            //build a graph from the plan
            Graph<GraphNode, GraphEdge> graph = gb.createGraph();
            RoadGraphKmlCreator.GraphProperty property = new RoadGraphKmlCreator.GraphProperty("plan",graph, Color.RED,2.0,0.5);

            List<RoadGraphKmlCreator.GraphProperty> props = new ArrayList<>(1);
            props.add(property);

            try {
                RoadGraphKmlCreator.createKml(props, new
                        File("plan"+System.currentTimeMillis()+".kml"));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }

            System.out.println("Plan length (km):" + planLength);
            System.out.println("Distance in air (km): "+ LatLon.distanceInMeters(origin.getLatitude(), origin.getLongitude(), destination.getLatitude(), destination.getLongitude())/1000.0);
            System.out.println("Time to travel (hrs): "+ planTime );
        } else{
            System.out.println("There is no plan!");
        }
    }


    /**
     * Deserializes a graph from a data file.
     * @param filename    file with the graph.
     * @return deserialized graph.
     */
    private Graph<GraphNode, GraphEdge> deserialize(String filename) {
        try{
            //use buffering
            InputStream file = new FileInputStream(filename);
            InputStream buffer = new BufferedInputStream( file );
            ObjectInput input = new ObjectInputStream ( buffer );
            try{
                //deserialize the graph
                Graph<GraphNode, GraphEdge> roadGraph = (Graph<GraphNode, GraphEdge>) input.readObject();
                return roadGraph;
            }
            finally{
                input.close();
            }
        }
        catch(Exception ex){
            ex.printStackTrace();
        }
        //This shouldn't happen! Luckily, one does not simply reach this line.
        System.exit(1);
        return null;
    }

    /**
     * Gets a dummy planner doing nothing. We hope you will be better than this ;)
     * @return Ghost in the shell.
     */
    public PlannerInterface getDummyPlanner() {
        return new PlannerInterface() {
            @Override
            public List<GraphEdge> plan(RoadGraph graph, GraphNode origin, GraphNode destination) {
                return new ArrayList<>();
            }

            @Override
            public AbstractOpenList getOpenList() {
                return new AbstractOpenList() {
                    @Override
                    protected boolean addItem(Object item) {
                        return false;
                    }
                };
            }
        };
    }
}
