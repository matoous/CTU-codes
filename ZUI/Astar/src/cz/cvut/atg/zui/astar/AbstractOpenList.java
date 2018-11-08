package cz.cvut.atg.zui.astar;

/**
 * Template for the open list.
 * Author: Ondrej Vanek (e-mail: ondrej.vanek@agents.fel.cvut.cz)
 * Date: 2/14/13
 * Time: 4:32 PM.
 */
public abstract class AbstractOpenList<T> {


    /**
     * Counter counting access (addition) to the open list.
     */
    private final Counter counter;

    public AbstractOpenList(){
        counter = Counter.getInstance();
    }


    /**
     * Yes, you have to call this method so we can use the counting!
     * @param item
     * @return
     */
    public final boolean add(T item){
        counter.increment();
        return addItem(item);
    }

    /**
     * And implement this method for add operations over your open list.
     * @param item
     * @return
     */
    protected abstract boolean addItem(T item);

    /**
     * Well, we need the access to the Counter, don't we?
     * @return the Counter, duh.
     */
    public final Counter getCounter() {
        return counter;
    }
}
