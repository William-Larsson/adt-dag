import dag.*;
import exceptions.CyclicGraphException;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class DagTest {

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

    static class StrWeightI implements WeightMethods<String> {
        @Override
        public String add(String a, String b) {
            return a.concat(b);
        }

        /**
         * Comparison is defined alphabetical order. a < b < c ...
         * aa < ab < ac
         * aaa > aa
         * @param b dag.Weight to compare to.
         * @return
         */
        @Override
        public WeightComparison compare(String a, String b) {
            if (a.length() > b.length()) return WeightComparison.GREATER_THAN;
            if (a.length() < b.length()) return WeightComparison.LESS_THAN;

            for (int i = 0; i < a.length(); i++) {
                if ((int) a.charAt(i) > (int)  b.charAt(i)) {
                    return WeightComparison.GREATER_THAN;
                } else if ((int)  a.charAt(i) < (int) b.charAt(i)) {
                    return WeightComparison.LESS_THAN;
                }
            }

            return WeightComparison.EQUAL;
        }
    }

    @Test
    public void connectedTest() {
        Dag<Integer> dag = new Dag<Integer>(new IntWeightI());

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
        Dag<Integer> dag = new Dag<Integer>(new IntWeightI());

        Vertex<Integer> b = dag.addVertex(2);
        Vertex<Integer> a = dag.addVertex(1);
        Vertex<Integer> c = dag.addVertex(3);
        Vertex<Integer> d = dag.addVertex(4);

        dag.addEdge(a, b, 0);
        dag.addEdge(a, c, 0);
        dag.addEdge(b, d, 0);

        List<Vertex<Integer>> sorted = dag.topologicalOrdering();
        for (int i = 0; i < sorted.size(); i++)
            assertEquals(new IntWeight(i+1).getIntWeight(), sorted.get(i).getWeight());
    }

    @Test
    public void topologicalOrderingWithLargeGraph() {
        Dag<Integer> dag = new Dag<Integer>(new IntWeightI());
        Vertex<Integer> a = dag.addVertex((1));
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((3));
        Vertex<Integer> d = dag.addVertex((4));
        Vertex<Integer> e = dag.addVertex((5));
        Vertex<Integer> f = dag.addVertex((6));
        Vertex<Integer> g = dag.addVertex((7));

        dag.addEdge(a, b, (0));
        dag.addEdge(b, e, (0));
        dag.addEdge(b, c, (0));
        dag.addEdge(b, d, (0));
        dag.addEdge(e, f, (0));
        dag.addEdge(f, g, (0));

        int[] correct = {1,2,5,3,4,6,7};
        List<Vertex<Integer>> sorted = dag.topologicalOrdering();
        for (int i = 0; i < correct.length; i++) {
            assertEquals(correct[i], sorted.get(i).getWeight());
        }
    }

    @Test
    public void topologicalOrderingMultiplePaths() {
        Dag<Integer> dag = new Dag<>(new IntWeightI());

        Vertex<Integer> a = dag.addVertex((0));
        Vertex<Integer> b = dag.addVertex((1));
        Vertex<Integer> c = dag.addVertex((2));
        Vertex<Integer> d = dag.addVertex((3));
        Vertex<Integer> e = dag.addVertex((4));
        Vertex<Integer> f = dag.addVertex((5));
        Vertex<Integer> g = dag.addVertex((6));
        Vertex<Integer> h = dag.addVertex((7));

        dag.addEdge(a, b, (1));
        dag.addEdge(a, d, (2));
        dag.addEdge(b, c, (2));
        dag.addEdge(b, d, (5));
        dag.addEdge(b, e, (6));
        dag.addEdge(c, e, (3));
        dag.addEdge(c, h, (2));
        dag.addEdge(d, e, (7));
        dag.addEdge(e, f, (8));
        dag.addEdge(e, g, (4));

        int[] correct = {0, 1, 2, 3, 7, 4, 5, 6};
        List<Vertex<Integer>> sorted = dag.topologicalOrdering();
        assertEquals(8, sorted.size());
        for (int i = 0; i < correct.length; i++) {
            assertEquals(correct[i], sorted.get(i).getWeight());
        }
    }

    @Test
    public void longestPathTest() {
        Dag<Integer> dag = new Dag<>(new IntWeightI());

        Vertex<Integer> a = dag.addVertex((1));
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((3));

        dag.addEdge(a, b, (2));
        dag.addEdge(b, c, (3));
        dag.addEdge(a, c, (10));

        Integer pathWeight = dag.weightOfLongestPath(a, c, (i) -> i, (i) -> i);

        assertEquals(14, pathWeight);
    }

    @Test
    public void longestPathLargeTest() {
        Dag<Integer> dag = new Dag<>(new IntWeightI());

        Vertex<Integer> a = dag.addVertex((1));
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((2));
        Vertex<Integer> d = dag.addVertex((6));
        Vertex<Integer> e = dag.addVertex((5));
        Vertex<Integer> f = dag.addVertex((15));
        Vertex<Integer> g = dag.addVertex((20));
        Vertex<Integer> h = dag.addVertex((25));

        dag.addEdge(a, b, (1));
        dag.addEdge(a, d, (2));
        dag.addEdge(b, c, (2));
        dag.addEdge(b, d, (5));
        dag.addEdge(b, e, (6));
        dag.addEdge(c, e, (3));
        dag.addEdge(c, h, (2));
        dag.addEdge(d, e, (7));
        dag.addEdge(e, f, (8));
        dag.addEdge(e, g, (4));

//      These are all the paths from a to g
//        a -> d -> e -> g (weight 1 + 2 + 6 + 7 + 5 + 4 + 20 = 45)
//        a -> b -> d -> e -> g (weight 1 + 1 + 2 + 5 + 6 + 7 + 5 + 4 + 20 = 51)
//        a -> b -> e -> g (weight 1 + 1 + 2 + 6 + 5 + 4 + 20 = 39)
//        a -> b -> c -> e -> g (weight 1 + 1 + 2 + 1 + 2 + 3 + 5 + 4 + 20 = 39)

        List<List<Vertex<Integer>>> paths = dag.getAllPaths(a, g);
        Integer longPath = dag.weightOfLongestPath(a, g, (i) -> i, (i) -> i);

        Integer shortPath = dag.weightOfPathComp(a, g, i -> i, i -> i,
                WeightComparison.LESS_THAN);

        assertEquals(51, longPath);
        assertEquals(39, shortPath);
    }


    @Test
    public void cyclicGraphThrowsException() {
        Dag<Integer> dag = new Dag<Integer>(new IntWeightI());

        Vertex<Integer> a = dag.addVertex((1));
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((2));
        Vertex<Integer> d = dag.addVertex((6));

        dag.addEdge(a, b, (1));
        dag.addEdge(a, d, (2));
        dag.addEdge(b, c, (2));
        dag.addEdge(b, d, (5));

        assertThrows(CyclicGraphException.class,
                () ->  dag.addEdge(d, a, (2)));

    }

    @Test
    public void strWeightPathTest() {
        Dag<String> dag = new Dag<>(new StrWeightI());

        Vertex<String> a = dag.addVertex("a");
        Vertex<String> bb = dag.addVertex("bb");
        Vertex<String> ca = dag.addVertex("ca");

        dag.addEdge(a, bb, "");
        dag.addEdge(bb, ca, "");

        dag.addEdge(a, ca, "");

        String w = dag.weightOfPathComp(a, ca, i -> i, i -> i,
                WeightComparison.LESS_THAN);
        String longest = dag.weightOfLongestPath(a, ca, i -> i, i -> i);
        String lPipe = dag.weightOfLongestPath(a, ca, i -> i, i -> "|");

        assertEquals("aca", w);
        assertEquals("abbca", longest);
        assertEquals("a|bb|ca", lPipe);
    }

    @Test
    public void copyConstructorTest() {
        Dag<Integer> dag = new Dag<Integer>(new IntWeightI());

        Vertex<Integer> a = dag.addVertex((1));
        Vertex<Integer> b = dag.addVertex((2));
        Vertex<Integer> c = dag.addVertex((2));
        Vertex<Integer> d = dag.addVertex((6));

        dag.addEdge(a, b, (1));
        dag.addEdge(a, d, (2));
        dag.addEdge(b, c, (2));
        dag.addEdge(b, d, (5));

        Dag<Integer> copy = new Dag<>(dag);
        copy.removeEdge(a, d);
        copy.removeEdge(b, d);

        assertNotEquals(dag.getInCount(d), copy.getInCount(d));
    }
}


