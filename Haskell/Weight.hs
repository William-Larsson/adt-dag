-- Module: Weight
--
-- Type class that acts as an interface for all objects that 
-- are used weights in the DAG data structure. 
-- Implements and exports instances the adhere to the interface.
module Weight ( Weight (..) ) where
    
    import Data.Char ( ord, chr )

    -- Type class: Weight
    --
    -- Interface for defining addition operation of
    -- Vertex and Edge weight. 
    class Ord w => Weight w where 
        add :: w -> w -> w 
        compare :: w -> w -> Ordering
    
    -- Weight instances
    --
    -- Defines addition for different types
    -- in accordance with Weight interface.
    -- Example usage: add 1 2
    instance Weight Integer where
        add w1 w2 = w1 + w2
        compare   = weightCompare 
    instance Weight Int where
        add w1 w2 = w1 + w2
        compare   = weightCompare
    instance Weight Float where
        add w1 w2 = w1 + w2
        compare   = weightCompare 
    instance Weight Double where
        add w1 w2 = w1 + w2
        compare   = weightCompare 
    instance Weight Char where
        add w1 w2 = chr $ ord w1 + ord w2
        compare   = weightCompare

    -- Function: weightCompare
    --
    -- Helper function for comparing instances
    -- of type class Weight. 
    weightCompare :: Weight w => w -> w -> Ordering 
    weightCompare w1 w2 
            | w1 == w1  = EQ 
            | w1 > w2   = GT 
            | otherwise = LT 