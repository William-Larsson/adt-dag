import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class DagConnectedTest {

    @Test
    public void connectedTest() {
        Dag<IntWeight> dag = new Dag<>();

        Vertex<IntWeight> a = dag.addVertex(new IntWeight(5));
        Vertex<IntWeight> b = dag.addVertex(new IntWeight(10));
        Vertex<IntWeight> c = dag.addVertex(new IntWeight(15));
        Vertex<IntWeight> d = dag.addVertex(new IntWeight(20));

        dag.addEdge(a, b, new IntWeight(5));
        dag.addEdge(a, c, new IntWeight(10));
        dag.addEdge(b, d, new IntWeight(10));

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
        Dag<IntWeight> dag = new Dag<>();

        Vertex<IntWeight> a = dag.addVertex(new IntWeight(1));
        Vertex<IntWeight> b = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> c = dag.addVertex(new IntWeight(3));
        Vertex<IntWeight> d = dag.addVertex(new IntWeight(4));

        dag.addEdge(a, b, new IntWeight(0));
        dag.addEdge(a, c, new IntWeight(0));
        dag.addEdge(b, d, new IntWeight(0));

        List<Vertex<IntWeight>> sorted = dag.topologicalOrdering();
        for (int i = 0; i < sorted.size(); i++)
            assertEquals(new IntWeight(i+1).getIntWeight(), sorted.get(i).getWeight().getIntWeight());
    }

    @Test
    public void topologicalOrderingWithLargeGraph() {
        Dag<IntWeight> dag = new Dag<>();
        Vertex<IntWeight> a = dag.addVertex(new IntWeight(1));
        Vertex<IntWeight> b = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> c = dag.addVertex(new IntWeight(3));
        Vertex<IntWeight> d = dag.addVertex(new IntWeight(4));
        Vertex<IntWeight> e = dag.addVertex(new IntWeight(5));
        Vertex<IntWeight> f = dag.addVertex(new IntWeight(6));
        Vertex<IntWeight> g = dag.addVertex(new IntWeight(7));

        dag.addEdge(a, b, new IntWeight(0));
        dag.addEdge(b, e, new IntWeight(0));
        dag.addEdge(b, c, new IntWeight(0));
        dag.addEdge(b, d, new IntWeight(0));
        dag.addEdge(e, f, new IntWeight(0));
        dag.addEdge(f, g, new IntWeight(0));

        int[] correct = {1,2,5,3,4,6,7};
        List<Vertex<IntWeight>> sorted = dag.topologicalOrdering();
        for (int i = 0; i < correct.length; i++) {
            assertEquals(correct[i], sorted.get(i).getWeight().getIntWeight());
        }
    }

    @Test
    public void longestPathTest() {
        Dag<IntWeight> dag = new Dag<>();

        Vertex<IntWeight> a = dag.addVertex(new IntWeight(1));
        Vertex<IntWeight> b = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> c = dag.addVertex(new IntWeight(3));
        Vertex<IntWeight> d = dag.addVertex(new IntWeight(3));

        dag.addEdge(a, b, new IntWeight(2));
        dag.addEdge(b, c, new IntWeight(3));
        dag.addEdge(a, c, new IntWeight(10));

        IntWeight pathWeight = dag.weightOfLongestPath(a, c, (i) -> i, (i) -> i);

        assertEquals(14, pathWeight.getIntWeight());
    }
}
