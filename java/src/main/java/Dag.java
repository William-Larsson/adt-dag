import exceptions.CyclicGraphException;

import java.util.*;
import java.util.function.Function;

/**
 * A general directed acyclic data (DAG) data structure. This data structure
 * is parameterized and can take any type that implements the Weight interface.
 * @param <T> The type of the weights.
 */
public class Dag<T extends Weight<T>> {

    private final List<Vertex<T>> vertices = new ArrayList<>();

    // Maps a vertex to a list of all edges that exist from that vertex.
    private final Map<Vertex<T>, List<Edge<T>>> edgeMap = new HashMap<>();
    private final List<Edge<T>> allEdges = new ArrayList<>();

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
            throw new CyclicGraphException();
        }

        if (!edgeMap.containsKey(a)) {
            List<Edge<T>> edgeList = new ArrayList<>();
            edgeMap.put(a, edgeList);
        }
        Edge<T> e = new Edge<>(a, b, w);
        allEdges.add(e);
        edgeMap.get(a).add(e);
    }

    /**
     * Removes the edge from Vertex a to Vertex b, if such an edge exists.
     * @param a From
     * @param b To
     */
    public void removeEdge(Vertex<T> a, Vertex<T> b) {
        List<Edge<T>> edges = edgeMap.get(a);
        edges.removeIf(e -> e.getFrom() == a && e.getTo() == b);
        allEdges.removeIf(e -> e.getFrom() == a && e.getTo() == b);
    }

    public boolean noIncomingEdge(Vertex<T> a) {
        boolean hasIncoming = false;
        for (Edge<T> e : allEdges) {
            if (a == e.getTo()) {
                hasIncoming = true;
                break;
            }
        }

        return !hasIncoming;
    }

    /**
     * Performs a topological ordering, using Kahn's algorithm.
     * @return A list containing the sorted elements.
     */
    public List<Vertex<T>> topologicalOrdering() {
        Dag<T> graph = new Dag<>(this);
        List<Vertex<T>> sortedList = new ArrayList<>();
        List<Vertex<T>> noIncomingEdgeList = new ArrayList<>();

        // Find all vertices with no incoming edges.
        for (Vertex<T> vert : vertices) {
            if (noIncomingEdge(vert)) {
                noIncomingEdgeList.add(vert);
            }
        }

        while (!noIncomingEdgeList.isEmpty()) {
            Vertex<T> node = noIncomingEdgeList.remove(0);
            sortedList.add(node);

            List<Edge<T>> edges = graph.edgeMap.get(node);
            if (edges != null) {
                // Loop through all edges that have an edge from `node`.
                for (Iterator<Edge<T>> iterator = edges.iterator(); iterator.hasNext();) {
                    Edge<T> edge = iterator.next();
                    iterator.remove();

                    if (graph.noIncomingEdge(edge.getTo())) {
                        noIncomingEdgeList.add(edge.getTo());
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

    /**
     * Traverses the graph and creates a list of paths between the given vertices.
     * @param a Starting vertex
     * @param b Goal vertex
     * @return The path between a and b.
     */
    public List<List<Vertex<T>>> getAllPaths(Vertex<T> a, Vertex<T> b) {
        List<List<Vertex<T>>> allPaths = new LinkedList<>();

        Queue<List<Vertex<T>>> queue = new LinkedList<>();
        List<Vertex<T>> firstPath = new LinkedList<>();
        firstPath.add(a);

        queue.add(firstPath);

        while (queue.size() > 0) {
            List<Vertex<T>> path = queue.poll();
            Vertex<T> next = path.get(path.size()-1);
            if (next == b) {
                // This is the end of a path.
                allPaths.add(path);
            }

            // Find all nodes we can reach from `next`.
            List<Edge<T>> edges = edgeMap.get(next);
            if (edges != null) {
                for (Edge<T> edge : edges) {
                    List<Vertex<T>> newPath = new LinkedList<>(List.copyOf(path));
                    newPath.add(edge.getTo());
                    queue.add(newPath);
                }
            }
        }
        return allPaths;
    }

    /**
     * Computes the weight of the longest path between the vertices a and b.
     * @param a Path start
     * @param b Path end
     * @param f function for interpreting the weight of the vertices
     * @param g function for interpreting the weight of the edges.
     * @return the weight of the longest path between a and b.
     */
    public T weightOfLongestPath(Vertex<T> a, Vertex<T> b, Function<T,T> f, Function<T, T> g) {
        return weightOfPathComp(a, b, f, g, WeightComparison.GREATER_THAN);
    }

    /**
     * Computes the weights of the path between vertices a and b, a custom
     * comparison operator is used to determine how the new weight is chosen
     * LESS_THAN yields the shortest path, GREATER_THAN yields the longest path.
     * @param a Path start
     * @param b Path end
     * @param f function for interpreting the weight of the vertices
     * @param g function for interpreting the weight of the edges.
     * @return the weight of the path between a and b, using some comparison.
     */
    public T weightOfPathComp(Vertex<T> a, Vertex<T> b, Function<T,T> f, Function<T, T> g, WeightComparison comp) {
        // TODO: This method needs more testing.
        //       Also, is it general enough?
        List<List<Vertex<T>>> allPaths = getAllPaths(a, b);

        T currWeight = null;
        for (List<Vertex<T>> path : allPaths) {

            T weight = f.apply(path.get(0).getWeight());
            weight = weight.add(g.apply(findEdge(path.get(0), path.get(1)).getWeight()));

            for (int i = 1; i < path.size(); i++) {
                Vertex<T> v = path.get(i);
                weight = weight.add(f.apply(v.getWeight()));
                // Find weight from current node to next node.
                if (i < path.size() - 1) {
                    Edge<T> edge = findEdge(v, path.get(i+1));
                    weight = weight.add(g.apply(edge.getWeight()));
                }
            }

            // If weight `comp` largestWeight, we have found a new weight.
            if (currWeight == null || weight.compare(currWeight) == comp) {
                currWeight = weight;
            }
        }

        return currWeight;
    }

    /**
     * Checks if the two vertices are connected or not, uses BFS
     * @param a From vertex
     * @param b To vertex
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

    /**
     * Finds the edge that connects the vertices a and b.
     * @param a Edge from
     * @param b Edge to
     * @return an edge connecting a and b, or null if such an edge does not exist.
     */
    public Edge<T> findEdge(Vertex<T> a, Vertex<T> b) {
        List<Edge<T>> edges = edgeMap.get(a);
        if (edges == null || edges.size() == 0) return null;

        for (Edge<T> e : edges) {
            if (e.getTo() == b) {
                return e;
            }
        }

        return null;
    }

}