module Graph (
    VertexID, 
    Vertex,
    Edge,
    Graph (..),
    initGraph,
    addVertex,
    addEdge
) where

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


    -- Function:
    --
    -- Adds an edge into the graph. 
    addEdge :: Graph w -> Edge w -> Graph w
    addEdge (Graph vs es) ed = Graph vs (ed:es)