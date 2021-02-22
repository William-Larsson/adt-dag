module Tests where
    import DAG
    import Data.Maybe ( isJust, isNothing )

    -- Vertices for testing... 
    g1 :: Graph Int
    (id1, g1) = addVertex initGraph 1
    (id2, g2) = addVertex g1 1
    (id3, g3) = addVertex g2 1
    (id4, g4) = addVertex g3 1
    (id5, g5) = addVertex g4 1
    (id6, g6) = addVertex g5 1
    (id7, g7) = addVertex g6 1
    (id8, g8) = addVertex g7 1
    -- Edges for testing...
    g9 = addEdge g8 (2,1,1)
    g10 = addEdge g9 (2,8,1)
    g11 = addEdge g10 (2,3,1)
    g12 = addEdge g11 (5,2,1)
    g13 = addEdge g12 (6,2,1)
    g14 = addEdge g13 (6,7,2)
    g15 = addEdge g14 (7,8,1)
    g16 = addEdge g15 (4,7,1)
    g = addEdge g16 (4,1,1)


    (idc1, gc1) = addVertex initGraph 'a'
    (idc2, gc2) = addVertex gc1 'a'
    (idc3, gc3) = addVertex gc2 'a'
    (idc4, gc4) = addVertex gc3 'a'
    (idc5, gc5) = addVertex gc4 'a'
    (idc6, gc6) = addVertex gc5 'a'
    (idc7, gc7) = addVertex gc6 'a'
    (idc8, gc8) = addVertex gc7 'a'
    -- Edges for testing...
    gc9 = addEdge gc8 (2,1,'a')
    gc10 = addEdge gc9 (2,8,'a')
    gc11 = addEdge gc10 (2,3,'a')
    gc12 = addEdge gc11 (5,2,'a')
    gc13 = addEdge gc12 (6,2,'a')
    gc14 = addEdge gc13 (6,7,'a')
    gc15 = addEdge gc14 (7,8,'a')
    gc16 = addEdge gc15 (4,7,'a')
    gc = addEdge gc16 (4,1,'a')


    --------------------------------------------------------
    -- Tests -----------------------------------------------
    --------------------------------------------------------


    -- Returns True if there is a cycle, otherwise False
    testHasCycle :: Weight w => Graph w -> Bool
    testHasCycle g = isNothing (topologicalOrdering g)


    -- Returns true if Edge results in no cycles. 
    -- Ex: returns false when Edge (1,2,1) in inserted into
    -- g, because if results in a cycle.
    testCanAddEdge :: Weight w => Graph w -> Edge w -> Bool
    testCanAddEdge (Graph vs es) e
        | isJust result = True 
        | otherwise     = False
        where
            result = topologicalOrdering $ Graph vs (e:es)




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