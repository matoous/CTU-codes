package cz.cvut.fel.agents.pdv.student;

import java.util.*;
import java.util.stream.Collectors;

public class Log {
    private List<LogEntry> log;

    Log(){
        log = new ArrayList<>();
    }

    public int size(){
        return log.size();
    }

    void add(LogEntry x){
        log.add(x);
    }

    public LogEntry get(int idx){
        if(idx > size() || idx < 1) return null;
        return log.get(idx-1);
    }

    int getTermOf(int idx){
        if(idx > size() || idx < 1) return 0;
        return log.get(idx-1).term;
    }

    void pop(){
        log.remove(size()-1);
    }

    List<LogEntry> sub(int from, int to){
        return new ArrayList<>(log.subList(from, to));
    }

    int lastLogTerm(){
        if(size() > 0)
            return get(size()).term;
        return 0;
    }

    Set<String> entryIds(){
        return log.stream().map(x -> x.action.requestId).collect(Collectors.toSet());
    }
}
