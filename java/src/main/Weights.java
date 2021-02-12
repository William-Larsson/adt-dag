import java.util.List;

public interface Weights<T> {

    T VertexWeight(T a);
    T EdgeWeight(T a);

    /**
     * Sums the given weights and returns the sum.
     * TODO: The return value of this method should not be T, rather, it should
     *       be some other unkown value. Find a way to handle this.
     * @param vertexWeights
     * @param edgeWeights
     * @return
     */
    T sumWeights(List<T> vertexWeights, List<T> edgeWeights);

    /**
     * Compares the given weights.
     * @param a
     * @param b
     * @return
     */
    WeightComparison compare(T a, T b);
}
