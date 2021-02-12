import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Dag<T> {

    private Map<Vertex<T>, List<Vertex<T>>> edges = new HashMap<>();
    private List<Vertex<T>> vertices = new ArrayList<>();

    public Vertex<T> addVertex(T w) {
        Vertex<T> v = new Vertex<>(w);
        vertices.add(v);
        return v;
    }

    public void addEdge(Vertex<T> a, Vertex<T> b, T w) {

    }

    public List<Vertex<T>> topologicalOrdering() {

        return new ArrayList<>();
    }

    public T weightOfLongestPath(Vertex<T> a, Vertex<T> b, Weights<T> f, Weights<T> g) {
        return null;
    }

}