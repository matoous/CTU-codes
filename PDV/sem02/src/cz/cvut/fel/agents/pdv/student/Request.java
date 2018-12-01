package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.raft.messages.IOperation;

import java.io.Serializable;

class Request implements Serializable {
    final String clientId;
    final String requestId;
    final String op;
    final String key;
    final String val;

    Request(String clientId, String requestId, IOperation op, String key, String val){
        this.clientId = clientId;
        this.requestId = requestId;
        this.op = op.toString();
        this.key = key;
        this.val = val;
    }
}
