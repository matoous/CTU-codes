package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

class AppendEntriesResponse extends Message {
    final Integer matchIndex;
    int term;
    boolean success;

    AppendEntriesResponse(Integer term, boolean success, int matchIndex) {
        this.term = term;
        this.success = success;
        this.matchIndex = matchIndex;
    }
}