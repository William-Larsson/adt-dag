module Tests where
    import DAG
    import Data.Maybe

    -- Vertices for testing... 
    (id1, g1) = addVertex initGraph 1
    (id2, g2) = addVertex g1 1
    (id3, g3) = addVertex g2 1
    (id4, g4) = addVertex g3 1
    (id5, g5) = addVertex g4 1
    (id6, g6) = addVertex g5 1
    (id7, g7) = addVertex g6 1
    (id8, g8) = addVertex g7 1
    -- Edges for testing...
    g9 = addEdge' g8 (2,1,1)
    g10 = addEdge' g9 (2,8,1)
    g11 = addEdge' g10 (2,3,1)
    g12 = addEdge' g11 (5,2,1)
    g13 = addEdge' g12 (6,2,1)
    g14 = addEdge' g13 (6,7,1)
    g15 = addEdge' g14 (7,8,1)
    g16 = addEdge' g15 (4,7,1)
    g = addEdge' g16 (4,1,1)
    gCycle = addEdge' g (1,2,1)


    --------------------------------------------------------
    -- Tests -----------------------------------------------
    --------------------------------------------------------

    -- Returns True if there is no cycle, otherwise False
    testNoCycle :: Eq w => Graph w -> Bool
    testNoCycle g = isJust (topologicalOrdering g)


    -- Returns True if there is a cycle, otherwise False
    testHasCycle :: Eq w => Graph w -> Bool
    testHasCycle g = isNothing (topologicalOrdering g)



    --------------------------------------------------------
    -- Notes -----------------------------------------------
    --------------------------------------------------------


-- Create a few DAGs, and test that the output is correct with what's 
-- expected. 



-- For longest path:
-- same type: weight for Vertices and Edges must be same
-- must be able to sum two weights somehow
-- must be able to compare weights (Eq, Ord?)



-- Shortest path --> is it possible to create a higher order func that take function of 
-- (< or >) so that longest and shorted path can be the same basic func?



-- Following Kahn's algorithm:
--
-- Input: 
--      The Graph, G

-- Algorithm:
--  ✅  0. Create empty list "L" that will contain sorted elements
--  ✅     Create Set/stack/queue "S" of all nodes with no incoming edges (noIncomingEdges)
--      
--  ✅  1.  Recursively take first elem "n" from "S"
--  ✅  1.1 Remove "n" from "S"
--  ✅  1.2 Add "n" to "L"
--
--  ✅  2.  Filter all edges "e" that start at "n" and goes to "m"
--      2.1 Recursively, do:
--  ✅  2.1.1 Remove "e" from "G"
--  ✅  2.1.2 if "m" has no incoming edges -> insert "m" to "S"
--  ✅  2.1.3 Return to 1.1
--
--  ✅  3.  if "G" has edge --> error (there is a cycle) return Nothing
--  ✅  3.1 else return Just L (the topological sort)