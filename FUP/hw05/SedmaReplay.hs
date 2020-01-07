module SedmaReplay where

import Prelude
import SedmaBase
import Data.List ( nub )

type Score = Maybe Int
type Scores = (Score, Score)
data State = State Cards Player Scores deriving Show

team :: Player -> Team
team A = AC
team B = BD
team C = AC
team D = BD

value :: Card -> Int
value (Card _ RA)  = 10
value (Card _ R10) = 10
value _            = 0

score :: Trick -> Int
score = sum . map value

update :: Int -> Score -> Score
update x (Just y) = Just (y+x)
update x Nothing  = Just x

updates :: Team -> Int -> Scores -> Scores
updates AC x (s1,s2) = (update x s1,s2)
updates BD x (s1,s2) = (s1, update x s2)

check :: Cards -> Maybe Cards
check cards | length cards == 32 &&
              length (nub cards) == 32 = Just cards
check _                                = Nothing

start :: Cards -> State
start cards = State cards A (Nothing,Nothing)

play :: State -> Scores
play (State [] leader scores) = updates (team leader) 10 scores
play (State (a:b:c:d:cards) leader scores) = 
   play $ State cards leader' scores'
   where trick   = [a,b,c,d]
         leader' = winner trick leader
         scores' = updates (team leader') (score trick) scores

finish :: Scores -> Maybe Winner
finish (Just x, Nothing)            = Just (AC,Three)
finish (Just x, Just 0)             = Just (AC,Two)
finish (Just x, Just y) | (x > y)   = Just (AC,One)
finish (x,y) = Just (BD,s) where (Just (_,s)) = finish (y,x)

replay :: Cards -> Maybe Winner
replay cards = check cards >>= (finish . play . start)

count :: Maybe Winner -> Int
count (Just (AC, One)) = 1
count (Just (AC, Two)) = 2
count (Just (AC, Three)) = 3
count (Just (BD, points)) = - count (Just (AC, points))

