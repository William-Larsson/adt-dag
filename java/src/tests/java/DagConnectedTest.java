import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class DagConnectedTest {

    @Test
    public void connectedTest() {
        Dag<Integer> dag = new Dag<>();

        Vertex<Integer> a = dag.addVertex(5);
        Vertex<Integer> b = dag.addVertex(10);
        Vertex<Integer> c = dag.addVertex(15);
        Vertex<Integer> d = dag.addVertex(20);

        dag.addEdge(a, b, 5);
        dag.addEdge(a, c, 10);
        dag.addEdge(b, d, 10);

        assertTrue(dag.connected(a, b));
        assertTrue(dag.connected(a, c));
        assertTrue(dag.connected(a, d));
        assertTrue(dag.connected(b, d));

        assertFalse(dag.connected(b, a));
        assertFalse(dag.connected(c, a));
        assertFalse(dag.connected(d, b));
        assertFalse(dag.connected(b, c));
    }

    @Test
    public void topologicalOrderingTest() {
        Dag<Integer> dag = new Dag<>();

        Vertex<Integer> a = dag.addVertex(1);
        Vertex<Integer> b = dag.addVertex(2);
        Vertex<Integer> c = dag.addVertex(3);
        Vertex<Integer> d = dag.addVertex(4);

        dag.addEdge(a, b, 0);
        dag.addEdge(a, c, 0);
        dag.addEdge(b, d, 0);

        List<Vertex<Integer>> sorted = dag.topologicalOrdering();
        for (int i = 0; i < sorted.size(); i++)
            assertEquals(i+1, sorted.get(i).getWeight());
    }

    @Test
    public void topologicalOrderingWithLargeGraph() {
        Dag<Integer> dag = new Dag<>();
        Vertex<Integer> a = dag.addVertex(1);
        Vertex<Integer> b = dag.addVertex(2);
        Vertex<Integer> c = dag.addVertex(3);
        Vertex<Integer> d = dag.addVertex(4);
        Vertex<Integer> e = dag.addVertex(5);
        Vertex<Integer> f = dag.addVertex(6);
        Vertex<Integer> g = dag.addVertex(7);

        dag.addEdge(a, b,0);
        dag.addEdge(b, e,0);
        dag.addEdge(b, c,0);
        dag.addEdge(b, d,0);
        dag.addEdge(e, f,0);
        dag.addEdge(f, g,0);

        int[] correct = {1,2,5,3,4,6,7};
        List<Vertex<Integer>> sorted = dag.topologicalOrdering();
        for (int i = 0; i < correct.length; i++) {
            assertEquals(correct[i], sorted.get(i).getWeight());
        }
    }

    @Test
    public void longestPathTest() {
        Dag<Integer> dag = new Dag<>();

        Vertex<Integer> a = dag.addVertex(1);
        Vertex<Integer> b = dag.addVertex(2);
        Vertex<Integer> c = dag.addVertex(3);
        Vertex<Integer> d = dag.addVertex(3);

        dag.addEdge(a, b, 2);
        dag.addEdge(b, c, 3);
        dag.addEdge(a, c, 10);

        Integer pathWeight = dag.weightOfLongestPath(a, c, new WeightMethods<Integer>() {
            @Override
            public Integer getVertexWeight(Vertex<Integer> vertex) {
                return vertex.getWeight();
            }

            @Override
            public Integer getEdgeWeight(Edge<Integer> edge) {
                return edge.getWeight();
            }

            @Override
            public Integer getZeroWeight() {
                return 0;
            }

            @Override
            public Integer addWeights(Integer a, Integer b) {
                return a + b;
            }

            @Override
            public WeightComparison compare(Integer a, Integer b) {
                if (a < b) return WeightComparison.LESS_THAN;
                if (a > b) return WeightComparison.GREATER_THAN;

                return WeightComparison.EQUAL;
            }

            @Override
            public WeightComparison compare(Edge<Integer> a, Edge<Integer> b) {
                if (a.getWeight() < b.getWeight()) return WeightComparison.LESS_THAN;
                if (a.getWeight() > b.getWeight()) return WeightComparison.GREATER_THAN;

                return WeightComparison.EQUAL;
            }
        });

        assertEquals(14, pathWeight);
    }
}
