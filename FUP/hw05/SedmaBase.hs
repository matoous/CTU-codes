module SedmaBase where

data Suit = Heart | Diamond | Spade | Club deriving (Eq, Show, Enum)
data Rank = R7 | R8 | R9 | R10 | RJ | RQ | RK | RA deriving (Eq, Show, Enum)
data Card = Card Suit Rank deriving (Eq, Show)
type Cards = [Card]

data Team = AC | BD deriving Show
data Points = One | Two | Three deriving Show
type Winner = (Team, Points)

data Player = A | B | C | D deriving (Eq, Show)
type Trick = [Card]
type Hand = [Card]

class PlayerState s where
   initState   :: Player -> Hand -> s
   updateState :: Trick -> Player -> Card -> Maybe Card -> s -> s
   
type AIPlayer s = Trick -> s -> Card

players = A:B:C:D:players

next :: Player -> Player
next p = head $ tail $ dropWhile (/= p) players

beats :: Card -> Card -> Bool
beats (Card _ r1) (Card _ r2) = (r1 == r2) || (r1 == R7)

iter 0 _ a = a
iter n f a = f $ iter (n-1) f a

winner :: Trick -> Player -> Player
winner [a,b,c,d] l
   | beats d a = iter 3 next l
   | beats c a = iter 2 next l
   | beats b a = next $ l
   | True      = l

