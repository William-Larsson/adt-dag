public interface WeightMethods<T> {

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
     * Gets the weight that corresponds to 0. E.g. for integers it is
     * likely 0, and for strings the empty string, "".
     * @return The zero weight.
     */
    T getZeroWeight();

    /**
     * Compares the given weights, and returns information about their relative
     * sizes.
     * @param a First weight
     * @param b Second weight
     * @return Enum representing the comparison results.
     */
    WeightComparison compare(T a, T b);
}
