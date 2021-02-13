
public class Edge <T> {
    private Vertex<T> from;
    private Vertex<T> to;
    private T weight;

    public Edge(Vertex<T> from, Vertex<T> to, T weight) {
        this.from = from;
        this.to = to;
        this.weight = weight;
    }

    public Vertex<T> getFrom() {
        return from;
    }

    public Vertex<T> getTo() {
        return to;
    }

    public T getWeight() {
        return weight;
    }
}
