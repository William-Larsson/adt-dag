import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

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
        for (Vertex<Integer> vert : sorted) {
            System.out.println(vert.getWeight());
        }
    }
}
