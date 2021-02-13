import java.util.List;

public interface WeightMethods<T> {

    T getVertexWeight(T a);
    T getEdgeWeight(T a);

    /**
     * Sums the given weights and returns the sum.
     * @param vertexWeights
     * @param edgeWeights
     * @return
     */
    T sumWeights(List<T> vertexWeights, List<T> edgeWeights);

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
