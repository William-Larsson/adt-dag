package dag;

/**
 * Implements the dag.Weight interface such that this class can be used as  a weight
 * in the dag.Dag data structure.
 */
public class IntWeight implements Weight<IntWeight> {

    private final Integer intWeight;

    /**
     * Constructs a new dag.IntWeight and initializes the weight to the given value.
     * @param val Value to initialize to.
     */
    public IntWeight(int val) {
        this.intWeight = val;
    }

    /**
     * Returns this weight.
     * @return This objects weight.
     */
    public Integer getIntWeight() {
        return intWeight;
    }

    /**
     * Adds this object's weight with the given weight.
     * @param w weight to add to the current weight.
     * @return the sum of the weights
     */
    @Override
    public IntWeight add(IntWeight w) {
        return new IntWeight(intWeight + w.intWeight);
    }

    /**
     * Compares this weight with the given weight.
     * @param b dag.Weight to compare to.
     * @return The results of the comparison.
     */
    @Override
    public WeightComparison compare(IntWeight b) {
        if (intWeight > b.intWeight) return WeightComparison.GREATER_THAN;
        if (intWeight < b.intWeight) return WeightComparison.LESS_THAN;

        return WeightComparison.EQUAL;
    }
}
