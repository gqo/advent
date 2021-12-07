module Lib
    ( output
    ) where

import Data.Char (digitToInt)
import Data.List (foldl')
import Data.List.Split (splitOn)


toDec :: Int -> String -> Int
toDec base = foldl (\acc x -> acc * base + digitToInt x) 0

distanceAcc :: Int -> [Int] -> Int
distanceAcc origin = foldl (\acc x -> acc + sum [1 .. abs(origin - x)]) 0

distance :: Int -> [Int] -> Int
distance origin = foldl (\acc x -> acc + abs (origin - x)) 0

fuel :: String -> (Int -> [Int] -> Int) -> Int
fuel xs f = minimum distances
  where
    nums = toDec 10 <$> splitOn "," xs
    len = length nums
    distances = flip f nums <$> [0 .. maximum nums]

output :: IO ()
output = do
    input <- readFile "data/input.txt"
    
    putStr "Part1: "
    print $ fuel input distance

    putStr "Part2: "
    print $ fuel input distanceAcc