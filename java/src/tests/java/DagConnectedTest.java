import exceptions.CyclicGraphException;
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

    @Test
    public void longestPathLargeTest() {
        Dag<IntWeight> dag = new Dag<>();

        Vertex<IntWeight> a = dag.addVertex(new IntWeight(1));
        Vertex<IntWeight> b = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> c = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> d = dag.addVertex(new IntWeight(6));
        Vertex<IntWeight> e = dag.addVertex(new IntWeight(5));
        Vertex<IntWeight> f = dag.addVertex(new IntWeight(15));
        Vertex<IntWeight> g = dag.addVertex(new IntWeight(20));
        Vertex<IntWeight> h = dag.addVertex(new IntWeight(25));

        dag.addEdge(a, b, new IntWeight(1));
        dag.addEdge(a, d, new IntWeight(2));
        dag.addEdge(b, c, new IntWeight(2));
        dag.addEdge(b, d, new IntWeight(5));
        dag.addEdge(b, e, new IntWeight(6));
        dag.addEdge(c, e, new IntWeight(3));
        dag.addEdge(c, h, new IntWeight(2));
        dag.addEdge(d, e, new IntWeight(7));
        dag.addEdge(e, f, new IntWeight(8));
        dag.addEdge(e, g, new IntWeight(4));

//      These are all the paths from a to g
//        a -> d -> e -> g (weight 1 + 2 + 6 + 7 + 5 + 4 + 20 = 45)
//        a -> b -> d -> e -> g (weight 1 + 1 + 2 + 5 + 6 + 7 + 5 + 4 + 20 = 51)
//        a -> b -> e -> g (weight 1 + 1 + 2 + 6 + 5 + 4 + 20 = 39)
//        a -> b -> c -> e -> g (weight 1 + 1 + 2 + 1 + 2 + 3 + 5 + 4 + 20 = 39)

        List<List<Vertex<IntWeight>>> paths = dag.getAllPaths(a, g);
        IntWeight longPath = dag.weightOfLongestPath(a, g, (i) -> i, (i) -> i);

        IntWeight shortPath = dag.weightOfPathComp(a, g, i -> i, i -> i, WeightComparison.LESS_THAN);

        assertEquals(51, longPath.getIntWeight());
        assertEquals(39, shortPath.getIntWeight());
    }


    @Test
    public void cyclicGraphThrowsException() {
        Dag<IntWeight> dag = new Dag<>();

        Vertex<IntWeight> a = dag.addVertex(new IntWeight(1));
        Vertex<IntWeight> b = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> c = dag.addVertex(new IntWeight(2));
        Vertex<IntWeight> d = dag.addVertex(new IntWeight(6));

        dag.addEdge(a, b, new IntWeight(1));
        dag.addEdge(a, d, new IntWeight(2));
        dag.addEdge(b, c, new IntWeight(2));
        dag.addEdge(b, d, new IntWeight(5));

        assertThrows(CyclicGraphException.class,
                () ->  dag.addEdge(d, a, new IntWeight(2)));

    }

    @Test
    public void strWeightShortestPath() {
        Dag<StrWeight> dag = new Dag<>();

        Vertex<StrWeight> a = dag.addVertex(new StrWeight("a"));
        Vertex<StrWeight> bb = dag.addVertex(new StrWeight("bb"));
        Vertex<StrWeight> ca = dag.addVertex(new StrWeight("ca"));

        dag.addEdge(a, bb, new StrWeight(""));
        dag.addEdge(bb, ca, new StrWeight(""));

        dag.addEdge(a, ca, new StrWeight(""));

        StrWeight w = dag.weightOfPathComp(a, ca, i -> i, i -> i, WeightComparison.LESS_THAN);

        System.out.println("Weight: " + w);
    }
}


