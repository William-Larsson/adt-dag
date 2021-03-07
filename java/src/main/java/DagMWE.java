import dag.Dag;
import dag.Vertex;
import dag.WeightComparison;
import dag.WeightMethods;

public class DagMWE {
    static class IntWeightI implements WeightMethods<Integer> {
        @Override
        public Integer add(Integer a, Integer b) {
            return a + b;
        }

        @Override
        public WeightComparison compare(Integer a, Integer b) {
            if (a > b) return WeightComparison.GREATER_THAN;
            if (a < b) return WeightComparison.LESS_THAN;

            return WeightComparison.EQUAL;
        }
    }

    public static void main(String[] args) {
        Dag<Integer> dag = new Dag<>(new IntWeightI());

        Vertex<Integer> a = dag.addVertex((1));
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((3));

        dag.addEdge(a, b, (2));
        dag.addEdge(b, c, (3));
        dag.addEdge(a, c, (10));

        Integer pathWeight = dag.weightOfLongestPath(a, c,
                (i) -> i, (i) -> i);
        System.out.println(pathWeight);
    }
}
