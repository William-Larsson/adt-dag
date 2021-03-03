package alt;

import dag.Vertex;
import dag.WeightComparison;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class Dag_altConnectedTest {
    @Test
    public void longestPathTest() {
        Dag_alt<Integer> dag = new Dag_alt<>(new WeightMethods_alt<>() {
            @Override
            public Integer getVertexWeight(Integer w) {
                return w;
            }

            @Override
            public Integer getEdgeWeight(Integer w) {
                return w;
            }

            @Override
            public Integer addWeights(Integer a, Integer b) {
                return a + b;
            }

            @Override
            public WeightComparison compare(Integer a, Integer b) {
                if (a > b) return WeightComparison.GREATER_THAN;
                if (a < b) return WeightComparison.LESS_THAN;
                return WeightComparison.EQUAL;
            }
        });

        Vertex<Integer> a = dag.addVertex(1);
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((3));
        Vertex<Integer> d = dag.addVertex((3));

        dag.addEdge(a, b, (2));
        dag.addEdge(b, c, (3));
        dag.addEdge(a, c, (10));

        Integer pathWeight = dag.weightOfLongestPath(a, c);

        assertEquals(14, pathWeight);
    }
}
