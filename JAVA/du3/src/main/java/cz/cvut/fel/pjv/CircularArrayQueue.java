package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {
    
    int capacity;
    int rear;
    int end;
    int element_count;
    String[] elements;
    
    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        this.capacity = 5;
        this.rear = 0;
        this.end = 0;
        this.element_count = 0;
        this.elements = new String[this.capacity];
    }


    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        this.capacity = capacity;
        this.rear = 0;
        this.end = -1;
        this.element_count = 0;
        this.elements = new String[this.capacity];
    }

    @Override
    public int size() {
        return this.element_count;
    }

    @Override
    public boolean isEmpty() {
        return !(this.element_count > 0);
    }

    @Override
    public boolean isFull() {
        return this.element_count == this.capacity;
    }

    @Override
    public boolean enqueue(String obj) {
        if(isFull() || obj == null){
            return false;
        }
        else{
            this.end = (this.end++ == this.capacity) ? 0 : this.end;
            this.elements[this.end] = obj;
            this.element_count++;
            return true;
        }
    }

    @Override
    public String dequeue() {
        String popped = this.elements[this.rear++];
        this.element_count--;
        if(this.rear == this.capacity) this.rear = 0;
        return popped;
    }

    @Override
    public void printAllElements() {
        for(int i = 0; i < this.element_count; i++){
            System.out.println(this.elements[(i+this.rear >= this.capacity)?(i-this.capacity+this.rear):(i+this.rear)]); // huehue
        }
    }
}
