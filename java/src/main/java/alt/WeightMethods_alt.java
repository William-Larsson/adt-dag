package alt;

import dag.WeightComparison;

public interface WeightMethods_alt<T> {

    T getVertexWeight(T w);
    T getEdgeWeight(T w);

    /**
     * Sums the given weights and returns the sum.
     * @param a First weight
     * @param b Second weight
     * @return The sum of the weights a and b.
     */
    T addWeights(T a, T b);

    /**
     * Compares the given weights, and returns information about their relative
     * sizes.
     * @param a First weight
     * @param b Second weight
     * @return Enum representing the comparison results.
     */
    WeightComparison compare(T a, T b);
}
