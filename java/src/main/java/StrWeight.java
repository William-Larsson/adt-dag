public class StrWeight implements Weight<StrWeight> {

    private String weight;

    public StrWeight(String weight) {
        this.weight = weight;
    }

    @Override
    public StrWeight add(StrWeight w) {
        return new StrWeight(weight.concat(w.weight));
    }

    /**
     * Comparison is defined alphabetical order. a < b < c ...
     * aa < ab < ac
     * aaa > aa
     * @param b Weight to compare to.
     * @return
     */
    @Override
    public WeightComparison compare(StrWeight b) {
        if (weight.length() > b.weight.length()) return WeightComparison.GREATER_THAN;
        if (weight.length() < b.weight.length()) return WeightComparison.LESS_THAN;

        for (int i = 0; i < weight.length(); i++) {
            if ((int) weight.charAt(i) > (int)  b.weight.charAt(i)) {
                return WeightComparison.GREATER_THAN;
            } else if ((int)  weight.charAt(i) < (int) b.weight.charAt(i)) {
                return WeightComparison.LESS_THAN;
            }
        }

        return WeightComparison.EQUAL;
    }
}
