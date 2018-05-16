package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

import java.io.Serializable;
import java.util.List;

class AppendEntries extends Message implements Serializable {
    int term;
    String leaderId;
    int prevLogIndex;
    int prevLogTerm;
    List<LogEntry> logEntries;
    int leaderCommit;

    AppendEntries(int term, String leaderId, int prevLogIndex, int prevLogTerm, List<LogEntry> logEntries, int leaderCommit) {
        this.term = term;
        this.leaderId = leaderId;
        this.prevLogIndex = prevLogIndex;
        this.prevLogTerm = prevLogTerm;
        this.logEntries = logEntries;
        this.leaderCommit = leaderCommit;
    }
}

