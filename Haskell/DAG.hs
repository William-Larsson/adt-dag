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
    removeEdge,
    removeEdges,
    getAllPaths,
    topologicalOrdering,
    weightOfLongestPath,
    weightOfShortestPath
) where 

    import qualified Graph as G
    import qualified Weight as W
    import Data.List ( delete, find, maximumBy , minimumBy ) 
    import Data.Maybe ( isJust, fromJust )

    -- Function: addEdge
    --
    -- Adds edge to graph if no cycle is introduced and 
    -- the start of the edge exist as vertices in the graph. 
    addEdge :: W.Weight w => G.Graph w -> G.Edge w -> G.Graph w
    addEdge (G.Graph vs es) ed@(idStart,idEnd,_)
        | not startExists = error noStart
        | not endExists   = error noEnd
        | isJust topOrd   = newGraph
        | otherwise       = error hasCycle
        where
            (startExists, endExists) = foldl 
                (\(srt, end) (idv,_) -> 
                    (srt || idv == idStart, end || idv == idEnd)) 
                (False, False) vs
            newGraph = G.Graph vs (ed:es)
            topOrd   = topologicalOrdering newGraph 
            noStart  = "Cannot add edge - start vertex does not exist."
            noEnd    = "Cannot add edge - end vertex does not exist."
            hasCycle = "Cannot add edge - would result in cycle."
    
    -- Function: removeEdge
    --
    -- Removes a given edge from list of edges
    removeEdge :: W.Weight w => G.Edge w -> [G.Edge w] -> [G.Edge w]
    removeEdge = delete

    -- Function: removeEdges
    --
    -- Removes all given edges from a list if edges.
    -- Uses removeEdge as helper function.
    -- Input: [Edges that will be removed], [Edges to remove from]
    removeEdges :: W.Weight w => [G.Edge w] -> [G.Edge w] -> [G.Edge w]
    removeEdges _ []  = []
    removeEdges [] ed = ed
    removeEdges rm ed = removeEdges (tail rm) $ removeEdge (head rm) ed

    -- Function: topologicalOrdering
    --
    -- Performs top. ord, based on Kahn's algorithm. 
    -- Returns list of sorted VertexID if ordering is found.
    -- If no topological ordering can be found, there is a cycle
    -- the the function returns Nothing. 
    -- Uses filterEdgesFromVertex as helper function. 
    topologicalOrdering :: W.Weight w => G.Graph w -> Maybe [G.VertexID]
    topologicalOrdering graph = 
        let
            ordering   = []
            noIncoming = noIncomingEdges graph 

            topOrd :: W.Weight w => G.Graph w -> [(G.Vertex w, Bool)] 
                -> [G.VertexID] -> Maybe [G.VertexID] 
            topOrd (G.Graph _ es) [] ord 
                | null es   = Just (reverse ord)
                | otherwise = Nothing       

            topOrd (G.Graph vs es) ((v@(id,_),_):_) ord = result
                where
                    ord'   = id:ord 
                    edges  = filterEdgesFromVertex id es  
                    graph  = G.Graph (delete v vs) (removeEdges edges es) 
                    noIn   = noIncomingEdges graph 
                    result = topOrd graph noIn ord' 
        in topOrd graph noIncoming ordering 

    -- Function: weightOfLongestPath
    --
    -- Computes the longest path between two given vertices s and e
    -- representing the starting and ending vertices.
    -- Inputs includes two functions, f and g, which are used to 
    -- interpret the weight of the vertices and edges respectively.  
    -- Uses weightOfPathComb as helper function. 
    weightOfLongestPath :: W.Weight w => G.Graph w -> G.VertexID -> G.VertexID
        -> (w -> w) -> (w -> w) -> Maybe w
    weightOfLongestPath graph s e f g = weightOfPathComp graph s e f g maximumBy

    -- Function: weightOfShortestPath
    --
    -- Computes the shortest path between two given vertices s and e
    -- representing the starting and ending vertices. 
    -- Inputs includes two functions, f and g, which are used to 
    -- interpret the weight of the vertices and edges respectively. 
    -- Uses weightOfPathComb as helper function. 
    weightOfShortestPath :: W.Weight w => G.Graph w -> G.VertexID -> G.VertexID
        -> (w -> w) -> (w -> w) -> Maybe w
    weightOfShortestPath graph s e f g = weightOfPathComp graph s e f g minimumBy

    -- Function: weightOfPathComp 
    --
    -- Computes the weight of the longest path between the vertices 
    -- s (start) and e (end).
    -- Input function f is used to interpret the weight of a vertex.
    -- Input function g is used to interpret the weight on an edge. 
    -- Input function comp used to get longest/shortest path. 
    -- Uses getAllPaths, getPathInfo, calcPathWeight & W.compare 
    -- as helper functions. 
    weightOfPathComp :: W.Weight w => 
        G.Graph w -> Integer -> Integer -> (w -> w) -> (w -> w) -> 
        ((Maybe w -> Maybe w -> Ordering) -> [Maybe w] -> Maybe w) 
        -> Maybe w
    weightOfPathComp graph@(G.Graph _ es) start end f g compFunc = maxWeight
        where 
            allPaths    = getAllPaths graph start end
            pathsInfo   = map (getPathInfo graph) allPaths
            pathWeights = map (\l -> calcPathWeight l f g) pathsInfo
            maxWeight
                | null pathWeights = Nothing
                | otherwise        = compFunc W.compare pathWeights

    -- Function: noIncomingEdges
    --
    -- Filters out all vertices that has 0 incoming edges.
    noIncomingEdges :: G.Graph w -> [(G.Vertex w, Bool)]
    noIncomingEdges (G.Graph vs es) = result
        where
            findIn = map (\v -> (v, any (\(_,id,_) -> fst v == id) es)) vs
            result = filter (\(_,hasIn) -> not hasIn) findIn 

    -- Function: getAdjacentVertices
    --
    -- Get id of the vertices that are at the end of
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
    getAllPaths :: G.Graph w -> G.VertexID -> G.VertexID -> [[G.VertexID]] 
    getAllPaths g@(G.Graph _ es) start end = let

        -- find the first adjacent vertices to kick off the getAll recursion.
        startAdj = getAdjacentVertices $ filterEdgesFromVertex start es

        getAll :: G.Graph w -> G.VertexID -> G.VertexID -> [G.VertexID] ->
            [G.VertexID] -> [[G.VertexID]]
        getAll _ _ end [] path = filterEndingVertices end [path]

        getAll g@(G.Graph vs es) start end [adj] path 
            | start == end = [path]    
            | otherwise    =
            getAll g adj end 
                (getAdjacentVertices $ filterEdgesFromVertex adj es) (path ++ [adj]) 

        getAll g@(G.Graph vs es) start end (adj:as) path 
            | start == end = [path] 
            | otherwise    =
            getAll g adj end 
                (getAdjacentVertices $ filterEdgesFromVertex adj es) (path ++ [adj]) 
                ++ getAll g start end as path 

        in getAll g start end startAdj [start]


    -- Function: getPathInfo 
    --
    -- Retrieves all the information about a Vertex 
    -- (ID, weights, and Edge connecting to next vertex)
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
    -- Calculates the weight of the a Vertex and the next edge 
    -- along the given path, unless its the last vertex on the path.
    -- Then sums up all weights in a path and returns list of 
    -- all path weights.  
    -- Uses the f function to interpret a vertex weight.
    -- Uses the g function to interpret a edge weight. 
    -- Uses W.add as a helper function. 
    calcPathWeight :: W.Weight w => [(G.Vertex w, Maybe (G.Edge w))] -> 
        (w -> w) -> (w -> w) -> Maybe w
    calcPathWeight [] _ _ = Nothing

    calcPathWeight [((_,vertWeight), Nothing)] f _ = Just (f vertWeight)

    calcPathWeight (((_,vertWeight), Just (_,_,edgeWeight)):paths) f g = 
        Just $ W.add sum (fromJust recur)
        where 
            sum   = W.add (f vertWeight) (g edgeWeight)
            recur = calcPathWeight paths f g

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