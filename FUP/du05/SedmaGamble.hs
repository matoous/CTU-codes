module SedmaGamble where

import Prelude
import SedmaBase
import SedmaReplay

data GlobalState s t = GlobalState {
   deck :: Cards,
   leader :: Player,
   hands :: [Hand],
   statesAC :: [s],
   playerAC :: AIPlayer s,
   statesBD :: [t],
   playerBD :: AIPlayer t
}

order :: Player -> Cards -> Cards
order A [c1,c2,c3,c4] = [c1,c2,c3,c4]
order B [c1,c2,c3,c4] = [c4,c1,c2,c3]
order C [c1,c2,c3,c4] = [c3,c4,c1,c2]
order D [c1,c2,c3,c4] = [c2,c3,c4,c1]

refill :: Cards -> [Hand] -> Player -> (Cards, [Hand], [Maybe Card])
refill []    hands _      = ([], hands, [Nothing,Nothing,Nothing,Nothing])
refill cards hands leader = (cards', hands', map Just ordered) where
   (dealed, cards') = splitAt 4 cards
   ordered = order leader dealed
   hands' = map (uncurry (:)) (zip ordered hands)

verify :: Cards -> [Hand] -> Bool
verify trick hands = and $ map (uncurry elem) (zip trick hands)

playRound :: (PlayerState s, PlayerState t) => GlobalState s t -> Trick
playRound (GlobalState deck leader hands statesAC playerAC statesBD playerBD) = trick' where
   orders = take 4 $ dropWhile (/= leader) players
   playCard A t = playerAC t (statesAC !! 0)
   playCard B t = playerBD t (statesBD !! 0)
   playCard C t = playerAC t (statesAC !! 1)
   playCard D t = playerBD t (statesBD !! 1)
   trick = foldl (\ t p -> t++[playCard p t]) [] orders
   trick' = if (verify (order leader trick) hands) then trick else (error "Sedma: Cheating player")

playGame :: (PlayerState s, PlayerState t) => GlobalState s t -> Cards -> Cards
playGame (GlobalState [] _ [[],[],[],[]] _ _ _ _) cards = cards
playGame gs cards = playGame gs' (cards++trick) where
   trick = playRound gs 
   ordered = order (leader gs) trick
   leader' = winner trick (leader gs)
   hands' = map (filter ((flip notElem) trick)) (hands gs)
   (deck', hands'', new) = refill (deck gs) hands' leader'
   stateA = updateState trick (leader gs) (ordered !! 0) (new !! 0) (statesAC gs !! 0)
   stateB = updateState trick (leader gs) (ordered !! 1) (new !! 1) (statesBD gs !! 0)
   stateC = updateState trick (leader gs) (ordered !! 2) (new !! 2) (statesAC gs !! 1)
   stateD = updateState trick (leader gs) (ordered !! 3) (new !! 3) (statesBD gs !! 1)
   statesAC' = [stateA, stateC]
   statesBD' = [stateB, stateD]
   gs' = GlobalState deck' leader' hands'' statesAC' (playerAC gs) statesBD' (playerBD gs)

gamble :: (PlayerState s, PlayerState t) => AIPlayer s -> AIPlayer t -> Cards -> Cards
gamble ac bd cards = playGame gs [] where
   hands = [[],[],[],[]]
   (cards',hands',_) = iter 4 (\ (cs,hs,_) -> refill cs hs A) (cards,hands,[])
   stateA = initState A (hands' !! 0)
   stateB = initState B (hands' !! 1)
   stateC = initState C (hands' !! 2)
   stateD = initState D (hands' !! 3)
   statesAC' = [stateA, stateC]
   statesBD' = [stateB, stateD]
   gs = GlobalState cards' A hands' statesAC' ac statesBD' bd

