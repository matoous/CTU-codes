package cz.cvut.atg.zui.astar;

import eu.superhub.wp5.planner.planningstructure.GraphNode;
import org.openstreetmap.osm.data.coordinates.LatLon;

/**
 * Methods supporting easy and comfortable planning.
 * Author: Ondrej Vanek (e-mail: ondrej.vanek@agents.fel.cvut.cz)
 * Date: 2/14/13
 * Time: 4:21 PM.
 */
public class Utils {

    /**
     * Gets distance in kilometers between node n1 and node n2. Uses Eucleidian distance, not Haversine distance.
     * So techically, you are not planning on the Globe.
     * @param n1
     * @param n2
     * @return distance in KM.
     */
    public static double distanceInKM(GraphNode n1, GraphNode n2){
        return LatLon.distanceInMeters(n1.getLatitude(), n1.getLongitude(), n2.getLatitude(), n2.getLongitude())/1000.0;
    }


    /**
     * Gets distance in kilometers between node n1 and node n2. Uses Eucleidian distance, not Haversine distance.
     * So techically, you are not planning on the Globe.
     * @param lat1
     * @param lon1
     * @param lat2
     * @param lon2
     * @return  distance in KM.
     */
    public static double distanceInKM(double lat1, double lon1, double lat2, double lon2)    {
        return LatLon.distanceInMeters(lat1, lon1, lat2, lon2)/1000.0;
    }
}
