module DAG (
    G.Graph (..),
    G.VertexID,
    G.Vertex,
    G.Edge,
    G.initGraph,
    G.addVertex,
    addEdge,
    topologicalOrdering,
    weightOfLongestPath
) where 

    import qualified Graph as G
    import Data.List ( delete ) 
    import Data.Maybe ( isJust )
    import Data.Char ( ord, chr )


    -- Type class: AddWeights
    --
    -- Interface for defining addition operation of
    -- Vertex and Edge weights. 
    class AddWeights w where 
        add :: w -> w -> w 
    
    -- AddWeights instances
    --
    -- TODO: Start by making sure that regular numbers work!!!
    instance AddWeights Integer where
        add w1 w2 = w1 + w2
    instance AddWeights Float where
        add w1 w2 = w1 + w2
    instance AddWeights Double where
        add w1 w2 = w1 + w2

    -- TODO: These should work too? 
    --instance AddWeights Bool where
    --   add w1 w2 = w1 < w2
    --instance AddWeights String where
    --    add w1 w2 = w1 ++ w2
    --
    --
    -- TODO: Can I do this? This might be solution to avoid 
    -- "Use TypeSynonymInstances" needed for String..
    -- maxBound :: Char  == 1114111 so that is a limiting factor.
    -- instance AddWeights Char where
    --     add w1 w2 = chr $ ord w1 + ord w2


    
    -- Function: addEdge
    --
    -- Adds edge to graph if no cycle is introduced and 
    -- the start of the edge exist as a vertex in graph. 
    addEdge :: Eq w => G.Graph w -> G.Edge w -> G.Graph w
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
    removeEdge :: Eq w => G.Edge w -> [G.Edge w] -> [G.Edge w]
    removeEdge = delete


    -- Function: removeEdges
    --
    -- Removes all given edges from a list if edges.
    -- Uses removeEdge as helper function.
    removeEdges :: Eq w => [G.Edge w] -> [G.Edge w] -> [G.Edge w]
    removeEdges _ []      = []
    removeEdges [] ex     = ex
    removeEdges (e:es) ex = removeEdges es (removeEdge e ex)
 

    -- Function: topologicalOrdering
    --
    -- Performs top. ord, based on Kahn's algorithm. 
    -- Returns list of sorted VertexID if ordering is found.
    -- If no topological ordering can be found, there is a cycle
    -- the the function returns Nothing. 
    topologicalOrdering :: Eq w => G.Graph w -> Maybe [G.VertexID]
    topologicalOrdering g = 
        let
            l = []
            s = noIncomingEdges g

            topOrd :: Eq w => 
                G.Graph w -> [(G.Vertex w, Bool)] -> [G.VertexID]
                -> Maybe [G.VertexID] 
            topOrd (G.Graph _ es) [] l 
                | null es   = Just (reverse l)
                | otherwise = Nothing       

            topOrd (G.Graph vs es) ((v@(n,_),_):s) l = result
                where
                    l'     = n:l 
                    e'     = filter (\(n',_,_) -> n == n') es   
                    g'     = G.Graph (delete v vs) (removeEdges e' es) 
                    s'     = noIncomingEdges g' 
                    result = topOrd g' s' l' 
        in topOrd g s l 
    

    -- Function: noIncomingEdges
    --
    -- Filters out any vertices that has >=1 incoming edges
    noIncomingEdges :: G.Graph w -> [(G.Vertex w, Bool)]
    noIncomingEdges (G.Graph vs es) = result
        where
            findIn = map (\v -> (v, any (\(_,id,_) -> fst v == id) es)) vs
            result = filter (\(_,hasIn) -> not hasIn) findIn 



    --------------------------------------------------------
    -- Things to do ----------------------------------------
    --------------------------------------------------------



    -- TODO: Wait with this until I figure out the class/instance things above ☝
    weightOfLongestPath :: 
        Eq w => G.Graph w -> 
        G.VertexID -> G.VertexID -> 
        (w -> w) -> (w -> w) -> w
    weightOfLongestPath gph start end f g = let 
        minInt = -2147483648 -- 32-bit int
        topOrd = topologicalOrdering gph
        dist   = take (length topOrd) [minInt, minInt ..]
        -- TODO: Find index of start and make that pos = 0 in dist
        in undefined 

    -- 1.  ✅ Init dist[] = [minBound ..]
    -- 1.1 👉 dist[start] = 0G.
    -- 
    -- 2.     topOrd = topological ordering. 
    --
    -- 3.     For each vertex v in topOrd
    -- 3.1    For each adjacent vertex av of v
    -- 3.2    if    dist[av] < dist[v] + weight(av, v) -- weight (av is weight of an edge, right?)
    --        then  dist[v] = dist[u] + weight(av, v)

    -- OBS! this algorithm only counts weights from edges
    -- needs to be modified to count vertex weight too!


    -- TODO: could this modified version work?
    -- 3.2 if dist[av] + weight(av) < dist[v] + weight(a) + weightEdge(av, v) ?? 

    -- TODO: 
    -- This algorithm also needs to take two functions, f & g.
    -- f & g are both used to determine the weight of paths. 


    -- Both f & G: functions of type W -> W. (W is the type parameter of the DAG)
    -- f: used to interpret weight of a vertex
    -- g: used to interpret weight of an edge. 

    -- Example - the weight of a path from a to c in the path [a,b,c]:
    -- tot = f(a.weight) + g((a,b).weight) + f(b.weight) + g((b,c).weight) + f(c.weight)
    -- Pattern (?): f1 + g1 + ... + gn-1 + fn

    -- At least begin with:
    -- f: (w -> w)
    -- g: (w -> w)
    -- this should work with numbers? 