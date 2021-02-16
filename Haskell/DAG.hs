module DAG (
    Graph,
    initGraph,
    addVertex,
    addEdge', -- TODO: change to addEdge !!
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
    } deriving (Show, Read) -- also Eq and Ord? 


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
 

    -- Function: topologicalOrdering
    --
    -- Performs top. ord, based on Kahn's algorithm. 
    -- Returns list of sorted VertexID if ordering is found.
    -- If no topological ordering can be found, there is a cycle
    -- the the function returns Nothing. 
    topologicalOrdering :: Eq w => Graph w -> Maybe [VertexID]
    topologicalOrdering g = 
        let
            l = []
            s = noIncomingEdges g

            topOrd :: Eq w => Graph w -> [(Vertex w, Bool)] -> [VertexID] -> Maybe [VertexID] 
            topOrd (Graph _ es) [] l 
                | null es   = Just (reverse l)
                | otherwise = Nothing       

            topOrd (Graph vs es) ((v@(n,_),_):s) l = result
                where
                    l'     = n:l 
                    e'     = filter (\(n',_,_) -> n == n') es   
                    g'     = Graph (delete v vs) (removeEdges e' es) 
                    s'     = noIncomingEdges g' 
                    result = topOrd g' s' l' 
        in topOrd g s l 
    

    -- Function: noIncomingEdges
    --
    -- Filters out any vertices that has >=1 incoming edges
    noIncomingEdges :: Graph w -> [(Vertex w, Bool)]
    noIncomingEdges (Graph vs es) = result
        where
            findIn = map (\v -> (v, any (\(_,id,_) -> fst v == id) es)) vs
            result = filter (\(_,hasIn) -> not hasIn) findIn 



    --------------------------------------------------------
    -- Things to do ----------------------------------------
    --------------------------------------------------------


    weightOfLongestPath = undefined
