-- Module: DAG_Tests
--
-- A set of tests to ensure that the DAG implementations 
-- is working as intended. 
module DAG_Tests where
    import DAG ( 
        Weight (..),
        VertexID,
        Edge,
        addVertex,
        initGraph,
        Graph(..),
        addEdge,
        topologicalOrdering,
        weightOfLongestPath,
        weightOfShortestPath)
    import Data.Maybe ( isJust, isNothing )


    --------------------------------------------------------
    -- Test Graphs -----------------------------------------
    --------------------------------------------------------


    -- Vertices for testing int
    g1 :: Graph Int
    (id1, g1) = addVertex initGraph 1
    (id2, g2) = addVertex g1 1
    (id3, g3) = addVertex g2 1
    (id4, g4) = addVertex g3 1
    (id5, g5) = addVertex g4 1
    (id6, g6) = addVertex g5 1
    (id7, g7) = addVertex g6 1
    (id8, g8) = addVertex g7 1

    -- Edges for testing int 
    g9 = addEdge g8 (2,1,1)
    g10 = addEdge g9 (2,8,1)
    g11 = addEdge g10 (2,3,1)
    g12 = addEdge g11 (5,2,1)
    g13 = addEdge g12 (6,2,1)
    g14 = addEdge g13 (6,7,1)
    g15 = addEdge g14 (7,8,1)
    g16 = addEdge g15 (4,7,1)


    -- Test Graph g. 
    -- Weights are int, no cycles
    g = addEdge g16 (4,1,1)


    -- Same as g, but with an added cycle through manual 
    -- manipulation of edge array. 
    gWithCycle = Graph (vertices g) ((2,5,1):edges g)

    -- Vertices for testing Char
    (idc1, gc1) = addVertex initGraph 'a'
    (idc2, gc2) = addVertex gc1 'a'
    (idc3, gc3) = addVertex gc2 'a'
    (idc4, gc4) = addVertex gc3 'a'
    (idc5, gc5) = addVertex gc4 'a'
    (idc6, gc6) = addVertex gc5 'a'
    (idc7, gc7) = addVertex gc6 'a'
    (idc8, gc8) = addVertex gc7 'a'
    
    -- Edges for testing Char
    gc9 = addEdge gc8 (2,1,'a')
    gc10 = addEdge gc9 (2,8,'a')
    gc11 = addEdge gc10 (2,3,'a')
    gc12 = addEdge gc11 (5,2,'a')
    gc13 = addEdge gc12 (6,2,'a')
    gc14 = addEdge gc13 (6,7,'a')
    gc15 = addEdge gc14 (7,8,'a')
    gc16 = addEdge gc15 (4,7,'a')


    -- Test Graph gc. 
    -- Same as g, but with Char weights.
    gc = addEdge gc16 (4,1,'a')


    --------------------------------------------------------
    -- Tests -----------------------------------------------
    --------------------------------------------------------


    -- Tests for asserting proper addEdge functionality
    addEdgeTest        = addEdge g (6, 8, 1)
    addEdgeCharTest    = addEdge gc (6, 8, 'p')
    addEdgeNoStartTest = addEdge g (100, 1, 1)
    addEdgeNoEndTest   = addEdge g (1, 100, 1)
    addEdgeNoCycleTest = addEdge g (1, 2, 1)

    -- Tests for asserting proper topological ordering
    hasNoCycleTest = isJust (topologicalOrdering g)
    hasCycleTest   = isNothing (topologicalOrdering gWithCycle)

    topologicalOrdTest    = topologicalOrdering g
    noTopologicalOrdTest  = topologicalOrdering gWithCycle
    
    -- Tests for asserting proper longest/shortest path functionality
    longestOfPathIntTest   = weightOfLongestPath (addEdge g (6,8,1)) 6 5 id id 
    shortestOfPathIntTest  = weightOfShortestPath (addEdge g (6,8,1)) 6 8 id id 
    longestOfPathCharTest  = weightOfLongestPath gc 6 8 id id 
    shortestOfPathCharTest = weightOfShortestPath gc 6 8 id id 
    noPathExistsCharTest   = weightOfShortestPath gc 6 5 id id 

    -- Tests for asserting Weight type class addition
    addIntTest :: Int
    addIntTest     = DAG.add 1 2

    addIntegerTest :: Integer
    addIntegerTest = DAG.add 1 2

    addFloatTest :: Float
    addFloatTest   = DAG.add 0.9 2.1

    addDoubleTest :: Double
    addDoubleTest  = DAG.add 1.0 2.0

    addCharTest :: Char
    addCharTest    = DAG.add '0' '1'

    addStringTest :: String
    addStringTest    = DAG.add "String" "Other string"
    
    -- Tests for asserting Weight type class comparison
    compareIntTest :: Ordering
    compareIntTest     = DAG.compare (Just 1::Maybe Int) (Just 2::Maybe Int)

    compareIntegerTest :: Ordering
    compareIntegerTest = DAG.compare (Just 2::Maybe Integer) (Just 2::Maybe Integer)

    compareFloatTest :: Ordering
    compareFloatTest   = DAG.compare (Just 2::Maybe Float) (Just 1::Maybe Float)

    compareDoubleTest :: Ordering
    compareDoubleTest  = DAG.compare (Just 1::Maybe Double) (Just 2::Maybe Double)

    compareCharTest :: Ordering
    compareCharTest    = DAG.compare (Just 'a'::Maybe Char) (Just 'b'::Maybe Char)

    compareStringTest :: Ordering
    compareStringTest   = DAG.compare (Just "abc"::Maybe String) (Just "aaa"::Maybe String)

    compareStringEqTest :: Ordering 
    compareStringEqTest = DAG.compare (Just "abc"::Maybe String) (Just "cba"::Maybe String)