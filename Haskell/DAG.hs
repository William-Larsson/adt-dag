-- Module: DAG 
--
-- A general directed acyclic data (DAG) data structure. 
-- This data structure is parameterized and can take any 
-- type that implements the Weight type class.
module DAG (
    W.Weight (..), 
    G.Graph (..),
    G.VertexID,
    G.Vertex,
    G.Edge,
    G.initGraph,
    G.addVertex,
    addEdge,
    topologicalOrdering,
    weightOfLongestPath,
    weightOfShortestPath
) where 

    import qualified Graph as G
    import qualified Weight as W
    import Data.List ( delete, find ) 
    import Data.Maybe ( isJust, fromJust )

    -- Function: addEdge
    --
    -- Adds edge to graph if no cycle is introduced and 
    -- the start of the edge exist as a vertex in graph. 
    addEdge :: W.Weight w => G.Graph w -> G.Edge w -> G.Graph w
    addEdge (G.Graph vs es) ed@(eid,_,_)
        | not startExists = error noStart
        | isJust topOrd   = newGraph
        | otherwise       = error hasCycle
        where
            startExists = any (\(vid,_) -> vid == eid) vs 
            newGraph    = G.Graph vs (ed:es)
            topOrd      = topologicalOrdering newGraph 
            noStart     = "Cannot add edge - start vertex does not exist"
            hasCycle    = "Cannot add edge - would result in cycle!"
    
    -- Function: removeEdge
    --
    -- Removes a given edge from list of edges
    removeEdge :: W.Weight w => G.Edge w -> [G.Edge w] -> [G.Edge w]
    removeEdge = delete

    -- Function: removeEdges
    --
    -- Removes all given edges from a list if edges.
    -- Uses removeEdge as helper function.
    removeEdges :: W.Weight w => [G.Edge w] -> [G.Edge w] -> [G.Edge w]
    removeEdges _ []      = []
    removeEdges [] ex     = ex
    removeEdges (e:es) ex = removeEdges es (removeEdge e ex)
 
    -- Function: topologicalOrdering
    --
    -- Performs top. ord, based on Kahn's algorithm. 
    -- Returns list of sorted VertexID if ordering is found.
    -- If no topological ordering can be found, there is a cycle
    -- the the function returns Nothing. 
    -- Uses filterEdgesFromVertex as helper function. 
    topologicalOrdering :: W.Weight w => G.Graph w -> Maybe [G.VertexID]
    topologicalOrdering g = 
        let
            l = []
            s = noIncomingEdges g

            topOrd :: W.Weight w => G.Graph w -> [(G.Vertex w, Bool)] 
                -> [G.VertexID] -> Maybe [G.VertexID] 
            topOrd (G.Graph _ es) [] l 
                | null es   = Just (reverse l)
                | otherwise = Nothing       

            topOrd (G.Graph vs es) ((v@(id,_),_):s) l = result
                where
                    l'     = id:l 
                    e'     = filterEdgesFromVertex id es  
                    g'     = G.Graph (delete v vs) (removeEdges e' es) 
                    s'     = noIncomingEdges g' 
                    result = topOrd g' s' l' 
        in topOrd g s l 

    -- Function: weightOfLongestPath
    --
    -- Computes the longest path between two given vertices s and e. 
    -- Uses weightOfPathComb as helper function. 
    weightOfLongestPath :: W.Weight w => G.Graph w -> G.VertexID -> G.VertexID
        -> (w -> w) -> (w -> w) -> Maybe w
    weightOfLongestPath graph s e f g = weightOfPathComp graph s e f g maximum

    -- Function: weightOfShortestPath
    --
    -- Computes the shortest path between two given vertices s and e. 
    -- Inputs includes two functions, f and g, which are used to 
    -- interpret the weight of the vertices and edges respectively. 
    -- Uses weightOfPathComb as helper function. 
    weightOfShortestPath :: W.Weight w => G.Graph w -> G.VertexID -> G.VertexID
        -> (w -> w) -> (w -> w) -> Maybe w
    weightOfShortestPath graph s e f g = weightOfPathComp graph s e f g minimum

    -- Function: weightOfLongestPath 
    --
    -- Computes the weight of the longest path between the vertices 
    -- s (start) and e (end).
    -- Input function f is used to interpret the weight of a vertex.
    -- Input function g is used to interpret the weight on an edge. 
    -- Input function comp used to get longest/shortest path. 
    -- Uses getAdjacentVertices, filterEdgesFromVertex, getAllPaths
    -- getPathInfo and calcPathWeight as helper functions. 
    weightOfPathComp :: W.Weight w => G.Graph w -> Integer -> Integer 
        -> (w -> w) -> (w -> w) -> ([Maybe w] -> Maybe a) -> Maybe a
    weightOfPathComp graph@(G.Graph _ es) start end f g comp = maxWeight
        where 
            startAdj    = getAdjacentVertices $ filterEdgesFromVertex start es  
            allPaths    = getAllPaths graph start end startAdj [start]
            pathsInfo   = map (getPathInfo graph) allPaths
            pathWeights = map (\l -> calcPathWeight l f g) pathsInfo
            maxWeight
                | null pathWeights = Nothing
                | otherwise        = comp pathWeights

    -- Function: noIncomingEdges
    --
    -- Filters out all vertices that has 0 incoming edges
    noIncomingEdges :: G.Graph w -> [(G.Vertex w, Bool)]
    noIncomingEdges (G.Graph vs es) = result
        where
            findIn = map (\v -> (v, any (\(_,id,_) -> fst v == id) es)) vs
            result = filter (\(_,hasIn) -> not hasIn) findIn 

    -- Function: getAdjacentVertices
    --
    -- Get id of vertices that are at the end of
    -- each given edge. 
    getAdjacentVertices :: [G.Edge w] -> [G.VertexID]
    getAdjacentVertices []             = []
    getAdjacentVertices ((_,end,_):es) = end : getAdjacentVertices es 
    
    -- Function: getAllPaths
    --
    -- Works by finding all paths leading out from the starting vertex 
    -- and filters out every path that does not end in ending vertex.
    -- Uses getAdjacentVertices and filterEdgesFromVertex as 
    -- helper functions. 
    -- Input: Graph , start, end, adjacent vertices, current path
    getAllPaths :: G.Graph a -> G.VertexID -> G.VertexID -> [G.VertexID] ->
        [G.VertexID] -> [[G.VertexID]]
    getAllPaths _ _ end [] path = filterEndingVertices end [path]

    getAllPaths g@(G.Graph vs es) start end [adj] path 
        | start == end = filterEndingVertices end [path]    
        | otherwise    =
        getAllPaths g adj end 
            (getAdjacentVertices $ filterEdgesFromVertex adj es) (path ++ [adj]) 

    getAllPaths g@(G.Graph vs es) start end (adj:as) path 
        | start == end = filterEndingVertices end [path] 
        | otherwise    =
        getAllPaths g adj end 
            (getAdjacentVertices $ filterEdgesFromVertex adj es) (path ++ [adj]) 
            ++ getAllPaths g start end as path 

    -- Function: getPathInfo 
    --
    -- Retrieves all the information about a Vertex 
    -- (ID, weights, and Edge connecting to next vertex.
    getPathInfo :: W.Weight w => G.Graph w -> [G.VertexID] 
        -> [(G.Vertex w, Maybe (G.Edge w))]
    getPathInfo _ []  = []

    getPathInfo (G.Graph vs _) [id] = [(fromJust vertex, Nothing)] 
        where 
            vertex = find (\(id',_) -> id == id') vs

    getPathInfo g@(G.Graph vs es) (id1:id2:ids) = 
        (fromJust vertex, edge) : getPathInfo g (id2:ids)
        where 
            vertex = find (\(id,_) -> id1 == id) vs
            edge   = find (\(id1', id2',_) -> id1 == id1' && id2 == id2') es

    -- Function: calcPathWeight
    --
    -- Takes tuples of a Vertex and an Edge that leads
    -- to the next Vertex on a given path through tge DAG.  
    calcPathWeight :: W.Weight w => [(G.Vertex w, Maybe (G.Edge w))] -> 
        (w -> w) -> (w -> w) -> Maybe w
    calcPathWeight [] _ _                  = Nothing
    calcPathWeight [((_,vw), Nothing)] f _ = Just (f vw)

    calcPathWeight (((_,vw), Just (_,_,ew)):ps) f g 
        | isJust recur = Just $ W.add sum (fromJust recur)
        | otherwise    = Just sum
        where 
            sum   = W.add (f vw) (g ew)
            recur = calcPathWeight ps f g

    -- Function: filterEdgesFromVertex
    --
    -- Filters out all edges that starts at given VertexID
    filterEdgesFromVertex :: G.VertexID -> [G.Edge w] -> [G.Edge w]
    filterEdgesFromVertex start = filter (\(start',_,_) -> start == start') 

    -- Function: filterEndingVertices
    --
    -- Filters all lists of VertexIDs that end at given VertexID.
    filterEndingVertices :: G.VertexID -> [[G.VertexID]] -> [[G.VertexID]]
    filterEndingVertices end = filter (\path -> last path == end)