import java.util.*;

public class Dag<T> {

    private List<Vertex<T>> vertices = new ArrayList<>();

    // Maps a vertex to a list of all edges that exist from that vertex.
    private Map<Vertex<T>, List<Edge<T>>> edgeMap = new HashMap<>();
    private List<Edge<T>> allEdges = new ArrayList<>();

    public Dag() {}

    /**
     * Makes a shallow copy of the given dag. The copy will have the same
     * objects, but removing elements from the list will not affect the original
     * graphs internal structure.
     * @param dag The dag to make a shallow copy of.
     */
    public Dag(Dag<T> dag) {
        this.vertices.addAll(dag.vertices);
        this.edgeMap.putAll(dag.edgeMap);
    }

    public Vertex<T> addVertex(T w) {
        Vertex<T> v = new Vertex<>(w);
        vertices.add(v);
        return v;
    }

    /**
     * Adds a new edge from vertex a to b, with the weight w.
     * @param a From
     * @param b To
     * @param w The edge's weight
     */
    public void addEdge(Vertex<T> a, Vertex<T> b, T w) {
        if (connected(b, a)) {
            // TODO: Create a custom checked exception for this.
            throw new NullPointerException("Cannot add edge - would result in cycle!");
        }
        if (!edgeMap.containsKey(a)) {
            List<Edge<T>> edgeList = new ArrayList<>();
            edgeMap.put(a, edgeList);
        }
        Edge<T> e = new Edge<T>(a, b, w);
        allEdges.add(e);
        edgeMap.get(a).add(e);
    }

    /**
     * Removes the edge from Vertex a to Vertex b, if such an edge exists.
     * @param a
     * @param b
     */
    public void removeEdge(Vertex<T> a, Vertex<T> b) {
        List<Edge<T>> edges = edgeMap.get(a);
        edges.removeIf(e -> e.getFrom() == a && e.getTo() == b);
        allEdges.removeIf(e -> e.getFrom() == a && e.getTo() == b);
    }

    public boolean hasIncomingEdge(Vertex<T> a) {
        boolean hasIncoming = false;
        for (Edge<T> e : allEdges) {
            if (a == e.getTo()) {
                hasIncoming = true;
                break;
            }
        }

        return hasIncoming;
    }

    /**
     * Performs a topological ordering, using Kahn's algorithm.
     * @return A list containing the sorted elements.
     */
    public List<Vertex<T>> topologicalOrdering() {
        Dag<T> graph = new Dag<>(this);
        List<Vertex<T>> sortedList = new ArrayList<>();
        List<Vertex<T>> noIncomingEdge = new ArrayList<>();

        // Find all vertices with no incoming edges.
        for (Vertex<T> vert : vertices) {
            if (!hasIncomingEdge(vert)) {
                noIncomingEdge.add(vert);
            }
        }

        while (!noIncomingEdge.isEmpty()) {
            Vertex<T> node = noIncomingEdge.remove(0);
            sortedList.add(node);

            List<Edge<T>> edges = graph.edgeMap.get(node);
            if (edges != null) {
                // Loop through all edges that have an edge from `node`.
                for (Iterator<Edge<T>> iterator = edges.iterator(); iterator.hasNext();) {
                    Edge<T> edge = iterator.next();
                    iterator.remove();

                    if (!graph.hasIncomingEdge(edge.getTo())) {
                        noIncomingEdge.add(edge.getTo());
                    }
                }
            }
        }

        if (graph.allEdges.isEmpty()) {
            return sortedList;
        } else {
            return null;
        }
    }

    public T weightOfLongestPath(Vertex<T> a, Vertex<T> b, WeightMethods<T> weightMethods) {
        return null;
    }

    /**
     * Checks if the two vertices are connected or not, uses BFS
     * @param a
     * @param b
     * @return True if the vertices are connected; false otherwise.
     */
    public boolean connected(Vertex<T> a, Vertex<T> b) {
        Queue<Vertex<T>> queue = new LinkedList<>();

        queue.add(a);

        while (queue.size() > 0) {
            Vertex<T> next = queue.poll();

            if (next == b) {
                return true;
            }

            // Find all nodes we can reach from `next`.
            List<Edge<T>> edges = edgeMap.get(next);
            if (edges != null) {
                for (Edge<T> edge : edges) {
                    queue.add(edge.getTo());
                }
            }
        }

        return false;
    }

}