module Map where

data KeyVal a b = KeyVal a b deriving Show
type Map a b = [KeyVal a b]

minit :: [a] -> [b] -> (Map a b)
minit [] [] = []
minit (x:xs) (y:ys) = (KeyVal x y):(minit xs ys)

madd :: Eq a => (Map a b) -> a -> b -> (Map a b)
madd [] a b = [KeyVal a b]
madd ((KeyVal a b):rest) key val =
  if a == key then ((KeyVal key val):rest) else (KeyVal a b):(madd rest key val)

mlookup :: Eq a => (Map a b) -> a -> Maybe b
mlookup [] _ = Nothing
mlookup ((KeyVal a b):rest) key
  | key == a = Just b
  | otherwise = (mlookup rest key)

mdelete :: Eq a => (Map a b) -> a -> (Map a b)
mdelete [] _ = []
mdelete ((KeyVal a b):rest) key
  | key == a = rest
  | otherwise = ((KeyVal a b):(mdelete rest key))
