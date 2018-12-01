package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

class RequestVoteResponse extends Message {
    RequestVoteResponse(Integer term, Boolean voteGranted) {
        this.term = term;
        this.voteGranted= voteGranted;
    }

    int term;
    boolean voteGranted;
}
