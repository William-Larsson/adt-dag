module DAG (
    addVertex,
    addEdge,
    topologicalOrdering,
    weightOfLongestPath
) where 
    import Data.List ( delete ) 

    -- Type aliases. 
    type VertexID = Integer
    type Vertex w = (VertexID, w) 
    type Edge w   = (VertexID, VertexID, w) -- start, end, w


    -- Graph data structure.
    data Graph w = Graph {
        vertices :: [Vertex w],
        edges :: [Edge w]
        -- TODO: map of Vertex(ID) and [Edge] like Java impl.?
    } deriving (Show, Read) -- also Eq and Ord?

 
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
    g10 = addEdge' g9 (1,2,1)
    g11 = addEdge' g10 (2,3,1)
    g12 = addEdge' g11 (5,2,1)
    g13 = addEdge' g12 (6,2,1)
    g14 = addEdge' g13 (6,7,1)
    g15 = addEdge' g14 (7,8,1)
    g16 = addEdge' g15 (4,7,1)
    g = addEdge' g16 (4,1,1) 


    -- Function: initGraph
    --
    -- Creates empty Graph. 
    initGraph :: Graph w 
    initGraph = Graph [] []


    -- Function: addVertex
    --
    -- Inserts Vertex to Graph.
    addVertex :: Graph w -> w -> (VertexID, Graph w)
    addVertex (Graph vs es) w = (id, Graph (vx:vs) es) 
        where 
            id = fromIntegral (length vs + 1)
            vx = (id, w)  


    -- Function: addEdge
    -- 
    -- Adds edge to graph if no cycle is introduced.
    addEdge :: Graph w -> Edge w -> Graph w
    addEdge (Graph vs es) ed
        | hasCycle  = error "Cannot add edge - would result in cycle!"
        | otherwise = newGraph  
        where
            newGraph = Graph vs (ed:es) -- Add edge to graph
            hasCycle = undefined        -- TODO: call topologicalOrdering.  


    -- TODO:  simple edge insert for testing, remove later
    addEdge' (Graph vs es) ed = Graph vs (ed:es)


    -- Function: removeEdge
    --
    -- Removes a given edge from list of edges
    removeEdge :: Eq w => Edge w -> [Edge w] -> [Edge w]
    removeEdge = delete


    -- Function: removeEdges
    --
    -- Removes all given edges from a list if edges.
    -- Uses removeEdge as helper function.
    removeEdges :: Eq w => [Edge w] -> [Edge w] -> [Edge w]
    removeEdges _ []      = []
    removeEdges [] ex     = ex
    removeEdges (e:es) ex = removeEdges es (removeEdge e ex)
 

    --------------------------------------------------------
    -- Things to do ----------------------------------------
    --------------------------------------------------------


    -- If no topological ordering can be found, there is a cycle.
    topologicalOrdering :: Graph w -> Maybe [VertexID]
    topologicalOrdering g@(Graph vs es) = result 
        where 
            l = []
            s = noIncomingEdges g
            result = undefined 

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
    --  👉  3.1 else return Just L (the topological sort)


    -- Function: topOrd
    --
    -- Helper func. for topologicalOrdering
    -- Performs top. ord, based loosely on Kahn's algorithm. 
    -- Returns list of sorted VertexID. 
    topOrd :: Eq w => Graph w -> [(Vertex w, Bool)] -> [VertexID] -> [VertexID] -- TODO: make this a Maybe func?
    topOrd (Graph _ es) [] l 
        | null es   = reverse l                             -- TODO: replace with Just (reverse l)?
        | otherwise = error "There is a cycle in the graph" -- TODO: replace with Nothing?

    topOrd (Graph vs es) ((v@(n,_),_):s) l = result
        where
            l'     = n:l                                     -- Add "n" to "L"
            e'     = filter (\(n',_,_) -> n == n') es        -- Filter all edges "e" that start at "n" and goes to "m"
            g'     = Graph (delete v vs) (removeEdges e' es) -- Remove "n" from "S" && Remove "e" from "G"
            s'     = noIncomingEdges g'                      -- if "m" has no incoming edges -> insert "m" to "S" 
            result = topOrd g' s' l'                         -- Return to 1.1


    -- Function: noIncomingEdges
    --
    -- Filters out any vertices that has >=1 incoming edges
    noIncomingEdges :: Graph w -> [(Vertex w, Bool)]
    noIncomingEdges (Graph vs es) = result
        where
            findIn = map (\v -> (v, any (\(_,id,_) -> fst v == id) es)) vs
            result = filter (\(_,hasIn) -> not hasIn) findIn 




    weightOfLongestPath = undefined


    -- Shortest path --> is it possible to create a higher order func that take function of 
    -- (< or >) so that longest and shorted path can be the same basic func?
