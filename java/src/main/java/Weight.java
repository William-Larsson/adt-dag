/**
 * This interface must be implemented by all objects that are to act as weights
 * in the Dag data structure.
 * @param <T> Type of the weight
 */
public interface Weight<T> {

    /**
     * It must be possible to add two weights together.
     * @param w weight to add to the current weight.
     * @return The sum of the weights.
     */
    T add(T w);

    /**
     * The weights must be comparable in some way.
     * @param b Weight to compare to.
     * @return How the relative size of this weight compares to b.
     */
    WeightComparison compare(T b);
}
