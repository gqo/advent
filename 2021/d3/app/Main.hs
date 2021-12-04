module Main where

import Lib
    ( gamma
    , flipbits
    , toDec
    , oxy
    , co2
    , tabulate
    )

main :: IO ()
main = do
  -- Read file and convert input to list of strings
  input <- readFile "input.txt"
  let nums = lines input

  -- Calculate gamma
  let g = gamma nums

  -- Multiply gamma and epsilon (gamma flipped)
  putStr "Part A: "
  print $ toDec g * toDec (flipbits g)

  -- Create a indexed list of strings
  let tNums = tabulate nums

  -- Find the product of oxy and co2 rates
  putStr "Part B: "
  print $ toDec (oxy tNums) * toDec (co2 tNums)