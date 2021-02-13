import java.util.List;

public interface WeightMethods<T> {

    T getVertexWeight(Vertex<T> vertex);
    T getEdgeWeight(Edge<T> edge);

    /**
     * Sums the given weights and returns the sum.
     * @param a First weight
     * @param b Second weight
     * @return The sum of the weights a and b.
     */
    T addWeights(T a, T b);

    /**
     * Gets the weight that corresponds to 0. E.g. for integers it is
     * likely 0, and for strings the empty string, "".
     * @return The zero weight.
     */
    T getZeroWeight();

    /**
     * Compares the weights of the given vertices.
     * @param a
     * @param b
     * @return
     */
    WeightComparison compare(T a, T b);

    /**
     * Compares the weights of the given edges.
     * @param a
     * @param b
     * @return
     */
    WeightComparison compare(Edge<T> a, Edge<T> b);
}
