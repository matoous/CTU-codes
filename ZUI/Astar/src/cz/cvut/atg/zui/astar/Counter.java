package cz.cvut.atg.zui.astar;

/**
 * Wrapper for counter.
 * Author: Ondrej Vanek (e-mail: ondrej.vanek@agents.fel.cvut.cz)
 * Date: 2/14/13
 * Time: 4:36 PM.
 */
public final class Counter {

    private volatile int counter;
    private static Counter instance;

    private Counter(){
        counter=0;
    }

    public static Counter getInstance(){
        if(instance==null){
            synchronized(Counter.class){
                Counter inst = instance;
                if(inst==null){
                    synchronized(Counter.class){
                        instance = new Counter();
                    }
                }
            }
        }
        return instance;
    }

    public synchronized final void increment(){
        counter++;
    }

    public final int getCount(){
        return counter;
    }

    public void clearData() {
        counter = 0;
    }
}
