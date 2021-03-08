{-# LANGUAGE FlexibleInstances #-}
-- Module: Weight
--
-- Type class that acts as an interface for all objects that 
-- are used weights in the DAG data structure. 
-- Implements and exports instances the adhere to the interface.
module Weight ( Weight (..) ) where
    
    import Data.Char ( ord, chr )
    import Data.Maybe ( fromJust ) 

    -- Type class: Weight
    --
    -- Interface for defining addition operation of
    -- Vertex and Edge weight. 
    class Ord w => Weight w where 
        add :: w -> w -> w 
        compare :: Maybe w -> Maybe w -> Ordering
    
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
    instance Weight [Char] where
        add w1 w2 = w1 ++ w2
        compare   = stringWeightCompare

    -- Function: weightCompare
    --
    -- Helper function for comparing instances
    -- of type class Weight. 
    weightCompare :: Weight w => Maybe w -> Maybe w -> Ordering 
    weightCompare w1 w2 
        | w1 == w2  = EQ 
        | w1 > w2   = GT 
        | otherwise = LT 

    -- Function: stringWeightCompare
    --
    -- Compares the weights of two strings in 
    -- by converting each char to an int and adding
    -- them together. The int weights are then compared.
    -- Uses weightCompare as a helper function.
    stringWeightCompare :: Maybe String -> Maybe String -> Ordering 
    stringWeightCompare str1 str2 = weightCompare w1 w2
        where
            w1 = Just $ stringToIntWeight $ fromJust str1
            w2 = Just $ stringToIntWeight $ fromJust str2

    -- Function: stringToIntWeight
    --
    -- Takes each char in a string and converts to 
    -- its integer (ordinal) representation. Then
    -- adds the int values together. 
    stringToIntWeight :: String -> Int
    stringToIntWeight = foldr ((+) . ord) 0