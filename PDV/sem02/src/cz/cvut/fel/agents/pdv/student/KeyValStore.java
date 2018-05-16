package cz.cvut.fel.agents.pdv.student;

import java.util.HashMap;
import java.util.Map;

public class KeyValStore {
    private Map<String, String> data;

    KeyValStore(){
        data = new HashMap<>();
    }

    void put(String key, String val){
        data.put(key, val);
    }

    void append(String key, String val){
        if(data.containsKey(key))
            data.put(key, data.get(key) + val);
        else
            data.put(key, val);
    }

    public String get(String key){
        return this.data.get(key);
    }

    void apply(LogEntry e){
        if(e.action.op.equals("PUT")){
            this.put(e.action.key, e.action.val);
        }
        if(e.action.op.equals("APPEND")){
            this.append(e.action.key, e.action.val);
        }
    }

    Map<String, String> pure(){
        return data;
    }
}
