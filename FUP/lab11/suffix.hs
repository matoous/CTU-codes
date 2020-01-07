-- Haskell suffix calculator (Polish notation)
import Data.List

suffixCalc :: (Num a, Read a) => String -> a
-- other option is to write this:
-- suffixCalc x = head $ foldl calcFun [] $ words x
suffixCalc = head . foldl calcFun [] . words
  where
    calcFun (a:b:bs) "+" = (a + b):bs
    calcFun (a:b:bs) "-" = (b - a):bs
    calcFun (a:b:bs) "*" = (a * b):bs
    calcFun numbers n = read n:numbers

main = do
  content <- getContents
  sequence $ map (print . show . suffixCalc) (lines content)
