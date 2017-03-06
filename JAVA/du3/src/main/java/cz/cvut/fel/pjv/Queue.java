package cz.cvut.fel.pjv;

/**
 * A collection representing F-I-F-O type queue with basic operations.
 */
public interface Queue {

    /**
     * Returns the number of stored elements in the queue. Note, that the the number of the elements is
     * <strong>not</strong> the size of the underlying data structure (array in this case specifically).
     *
     * @return the number of elements in the queue
     */
    int size();

    /**
     * Returns {@code true} iff the queue is empty, {@code false} otherwise. If the returned value is {@code true},
     * then the {@link #size()} method must return value {@code 0}.
     *
     * @return {@code true} if the queue is empty.
     */
    boolean isEmpty();

    /**
     * Returns {@code true} iff the queue is full, {@code false} otherwise. If the queue is full, then
     * {@link #enqueue(String)} method call returns {@code false} and no element is enqued.
     *
     * @return {@code true} iff the queue is full
     */
    boolean isFull();

    /**
     * Inserts the specified element into this queue if it is possible to do so immediately without violating
     * capacity restrictions, returning  {@code true} upon success and {@code false} if no space is currently
     * available or the {@code element} is {@code null}. The {@code null} element is not added to the queue.
     *
     * @param element the element to add, might be {@code null}
     * @return {@code true} iff the element is added, {@code false} otherwise.
     * @see #isFull()
     */
    boolean enqueue(String element);

    /**
     * Retrieves and removes the head of this queue, or returns {@code null} if this queue is empty.
     *
     * @return the head of this queue, or {@code null} if this queue is empty
     */
    String dequeue();

    /**
     * Prints all elements in the queue on the stdout. Each element is printed on single line. This method is inteded
     * for debugging purposes.
     */
    void printAllElements();
}