package cz.cvut.fel.agents.pdv.student;

import java.io.Serializable;

class LogEntry implements Serializable {
    final Integer term;
    final Request action;

    LogEntry(Integer term, Request req){
        this.term = term;
        this.action = req;
    }
}
