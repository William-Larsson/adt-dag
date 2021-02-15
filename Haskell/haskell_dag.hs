module DAG (
    addVertex,
    addEdge,
    topologicalOrdering,
    weightOfLongestPath
) where 

    -- Type aliases. 
    type VertexID = Integer
    type Vertex w = (VertexID, w) 
    type Edge w   = (VertexID, VertexID, w) 


    -- Graph data structure.
    data Graph w = Graph {
        vertices :: [Vertex w],
        edges :: [Edge w]
        -- TODO: map of Vertex(ID) and [Edge] like Java impl.?
    } deriving (Show, Read) -- also Eq and Ord?


    -- Function: addVertex
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
            hasCycle = undefined  -- TODO: call topologicalOrdering.  


    -- TODO:  simple edge insert for testing, remove later
    addEdge' (Graph vs es) ed = Graph vs (ed:es)


    --------------------------------------------------------
    -- Things to do ----------------------------------------
    --------------------------------------------------------


    -- If no topological ordering can be found, there is a cycle.
    topologicalOrdering :: Graph w -> Maybe [VertexID]
    topologicalOrdering g@(Graph vs es) = let 
        l = []
        s = noIncomingEdges g
        in undefined 

    -- Following Kahn's algorithm:
    --
    -- Input: 
    --      The Graph, G

    -- Algorithm:
    --  ✅  0. Create empty list "L" that will contain sorted elements
    --  ✅    Create Set/stack/queue "S" of all nodes with no incoming edges (noIncomingEdges)
    --      
    --  👉  1. Recursively take first elem "n" from "S"
    --      1.1 Remove "n" from "S"
    --      1.2 Add "n" to "L"
    --
    --      2. Filter all edges "e" that start at "n" and goes to "m"
    --      2.1 Recursively, do:
    --      2.1.1 Remove "e" from "G"
    --      2.1.2 if "m" has no incoming edges -> insert "m" to "S"
    --      2.1.3 Return to 1.1
    --
    --      3.  if "G" has edge --> error (there is a cycle) return Nothing
    --      3.1 else return Just L (the topological sort)




    -- Function: noIncomingEdges
    --
    -- Filters out any vertices that has >=1 incoming edges
    noIncomingEdges :: Graph w -> [(VertexID, Bool)]
    noIncomingEdges (Graph vs es) = result
        where
            findIn = map (\(id1,_) -> (id1, any (\(_,id2,_) -> id1 == id2) es)) vs
            result = filter (\(_,hasIn) -> not hasIn) findIn 



    weightOfLongestPath = undefined