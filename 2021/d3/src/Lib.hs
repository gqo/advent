module Lib
    ( gamma
    , flipbits
    , toDec
    , oxy
    , co2
    , tabulate
    ) where

import Data.Char (digitToInt)
import Data.List (foldl')

-- Calculates the gamma by counting the number of ones/zeros in each column, determining the max between the columsn of two lists, and concatenating those values into one answer
gamma :: [String] -> String
gamma xs = zipWith (\o z -> if o < z then '1' else '0') ones zeros
  where
    len = length $ head xs
    nth n = map (!! n) xs
    nthes = map nth [0 .. len - 1]
    ones = length . filter (== '1') <$> nthes
    zeros = length . filter (== '0') <$> nthes

flipbits :: String -> String
flipbits = map (\b -> if b == '1' then '0' else '1')

-- Converts from a binary string to a decimal int
toDec :: String -> Int
toDec = foldl' (\acc x -> acc * 2 + digitToInt x) 0

-- Determines the index of the element within the table that matches the rate search criteria by recursively filtering the lists
part2 :: (Int -> Int -> Char) -> [(Int,String)] -> Int
part2 _ [(i,_)] = i
part2 f xs = part2 f $ (tail <$>) <$> criteria
  where
    xs' = map (head .snd) xs
    ones = (length . filter (== '1')) xs'
    zeros = (length . filter (== '0')) xs'
    bit = f ones zeros
    criteria = filter ((== bit) . head . snd) xs

oxy :: [(Int, String)] -> String
oxy xs = snd $ (!!) xs $ part2 (\o z -> if o >= z then '1' else '0') xs

co2 :: [(Int, String)] -> String
co2 xs = snd $ (!!) xs $ part2 (\o z -> if o < z then '1' else '0') xs

-- Creates a list of tuples where the first value in each tuple is its index within the list
tabulate :: [a] -> [(Int,a)]
tabulate = zip [0 ..]
