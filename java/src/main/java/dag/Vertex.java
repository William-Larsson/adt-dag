package dag;

public class Vertex<T> {

    private T weight;

    public Vertex(T w) {
        this.weight = w;
    }

    public T getWeight() {
        return this.weight;
    }
}
