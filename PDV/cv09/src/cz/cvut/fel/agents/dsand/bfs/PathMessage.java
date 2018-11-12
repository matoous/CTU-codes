package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.Message;

public class PathMessage extends Message {
    public String path;

    public PathMessage(String me){
        path += " " + me;
    }
}
