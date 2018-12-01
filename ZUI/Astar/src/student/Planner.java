package student;

import cz.cvut.atg.zui.astar.AbstractOpenList;
import cz.cvut.atg.zui.astar.PlannerInterface;
import cz.cvut.atg.zui.astar.RoadGraph;
import eu.superhub.wp5.planner.planningstructure.GraphEdge;
import eu.superhub.wp5.planner.planningstructure.GraphNode;

import java.util.*;

import static cz.cvut.atg.zui.astar.Utils.distanceInKM;

import java.util.List;

class Node{

    public long id;
    public boolean closed;
    public double g;
    public double h;
    public double f;
    public boolean isInOpenList;
    public Node parent;

    public Node(long id, double g, double h, Node parent){
        this.id = id;
        this.closed = false;
        this.g = g;
        this.h = h;
        this.f = g + h;
        this.isInOpenList=false;
        this.parent = parent;

    }
}

class OpenList extends AbstractOpenList<Node> {
    private PriorityQueue<Node> queue;

    OpenList() {
        queue = new PriorityQueue<>(32, Comparator.comparingDouble(a -> a.f));
    }

    @Override
    protected boolean addItem(Node item) {
        queue.add(item);
        return true;
    }

    public Node pop() {
        return queue.remove();
    }

    public boolean empty () {
        return queue.isEmpty();
    }

    public void removeNode(Node node){
        queue.remove(node);
    }

    public void clear(){
        queue.clear();
    }
}



public class Planner implements PlannerInterface {

    public RoadGraph graf;
    public OpenList openList;

    private List<GraphEdge> getPath(Node start, Node end) {
        List<GraphEdge> edges = new ArrayList<>();
        Node tmp = end;

        while(tmp.id != start.id){
            Node parent = tmp.parent;
            edges.add(graf.getEdge(parent.id, tmp.id));
            tmp = parent;
        }

        Collections.reverse(edges);
        return edges;
    }


    public List<GraphEdge> astar(Node start, long end){
        openList.clear();
        Map<Long, Node> mapa = new HashMap<>();

        start.isInOpenList = true;
        openList.add(start);
        mapa.put(start.id, start);



        while(!openList.empty()){
            Node node = openList.pop();

            if(node.id == end){
                return getPath(start, node);
            }

            List<GraphEdge> outEdges = graf.getNodeOutcomingEdges(node.id);
            if(outEdges == null) continue;
            for(GraphEdge edge : outEdges){
                double edgeCost = (edge.getLengthInMetres()/1000) / edge.getAllowedMaxSpeedInKmph();
                double pathCost = node.g + edgeCost;

                Node nextNode;
                if(mapa.containsKey(edge.getToNodeId())){
                    nextNode = mapa.get(edge.getToNodeId());
                }else{
                    nextNode = new Node(edge.getToNodeId(), pathCost, distanceInKM(graf.getNodeByNodeId(edge.getToNodeId()), graf.getNodeByNodeId(end))/120.0, node);
                }

                if(!nextNode.isInOpenList && !nextNode.closed){
                    nextNode.isInOpenList = true;
                    openList.add(nextNode);
                    mapa.put(nextNode.id, nextNode);
                }
                else if(nextNode.isInOpenList && !nextNode.closed){
                    if(pathCost < nextNode.g){
                        openList.removeNode(nextNode);
                        nextNode.g = pathCost;
                        nextNode.f = nextNode.g + nextNode.h;
                        nextNode.parent = node;
                        openList.addItem(nextNode);
                    }
                }
            }
            node.closed = true;
        }
        return null;
    }

    @Override
    public List<GraphEdge> plan(RoadGraph graph, GraphNode origin, GraphNode destination) {
        openList = new OpenList();
        graf = graph;
        Node start = new Node(origin.getId(), 0.0, distanceInKM(origin, destination)/120.0, null);
        return astar(start, destination.getId());
    }

    @Override
    public AbstractOpenList getOpenList() {
        return openList;
    }
}