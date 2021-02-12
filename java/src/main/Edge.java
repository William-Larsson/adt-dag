
public class Edge <T> {
    private Vertex<T> from;
    private Vertex<T> to;

    public Edge(Vertex<T> from, Vertex<T> to) {
        this.from = from;
        this.to = to;
    }
}
