package exceptions;

/**
 * This exception will be thrown whenever one attempts to add an edge to a graph
 * that would result in a cycle.
 */
public class CyclicGraphException extends RuntimeException {

    public CyclicGraphException() {
        super("Cannot add edge - would result in cycle!");
    }

    public CyclicGraphException(String message) {
        super(message);
    }
}
