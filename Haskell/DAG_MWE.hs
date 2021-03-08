import DAG ( addVertex, initGraph, addEdge, weightOfLongestPath )

main :: IO ()
main = let 
    d0 = initGraph       
    (a, d1) = addVertex d0 (1::Int)
    (b, d2) = addVertex d1 2
    (c, d3) = addVertex d2 3
    d4  = addEdge d3 (a, b, 2)
    d5  = addEdge d4 (b, c, 3)
    dag = addEdge d5 (a, c, 10)
    in
        print $ weightOfLongestPath dag a c id id