import SedmaDatatypes

data Player = A | B | C | D deriving (Show, Eq)

players_team :: Player -> Team
players_team A = AC
players_team C = AC
players_team B = BD
players_team D = BD

sameRank :: Card -> Card -> Bool
sameRank (Card _ RA) (Card _ RA) = True
sameRank (Card _ RK) (Card _ RK) = True
sameRank (Card _ RQ) (Card _ RQ) = True
sameRank (Card _ RJ) (Card _ RJ) = True
sameRank (Card _ R10) (Card _ R10) = True
sameRank (Card _ R9) (Card _ R9) = True
sameRank (Card _ R8) (Card _ R8) = True
sameRank (Card _ R7) _ = True
sameRank _ _ = False

fromJust :: Maybe Player -> Player
fromJust Nothing = error "Maybe.fromJust: Nothing"
fromJust (Just x) = x

score :: Card -> Int
score (Card _ RA) = 10
score (Card _ R10) = 10
score (Card _ _) = 0

nextPlayer :: Player -> Player
nextPlayer A = B
nextPlayer B = C
nextPlayer C = D
nextPlayer D = A

handScore :: Cards -> Int
handScore (x:xs) = score x + handScore xs
handScore [] = 0

handsScore :: [Cards] -> Maybe Player -> (Int, Int)
handsScore [acards, bcards, ccards, dcards] (Just A) = (handScore acards + handScore ccards + 10, handScore bcards + handScore dcards)
handsScore [acards, bcards, ccards, dcards] (Just C) = (handScore acards + handScore ccards + 10, handScore bcards + handScore dcards)
handsScore [acards, bcards, ccards, dcards] (Just B) = (handScore acards + handScore ccards, handScore bcards + handScore dcards + 10)
handsScore [acards, bcards, ccards, dcards] (Just D) = (handScore acards + handScore ccards, handScore bcards + handScore dcards + 10)
handsScore [acards, bcards, ccards, dcards] Nothing = (handScore acards + handScore ccards, handScore bcards + handScore dcards)
handsScore _ _ = (0, 0)

gotNoTrick :: Team -> [Cards] -> Bool
gotNoTrick AC [acards, bcards, ccards, dcards] = null acards && null ccards
gotNoTrick BD [acards, bcards, ccards, dcards] = null bcards && null dcards

lastWinner :: Cards -> Player -> Maybe Player
lastWinner [] _ = Nothing
lastWinner cards player =
  if nextWinner == Nothing then winner else nextWinner
  where
    winner = findWinner (head cards) (take 4 cards) player
    nextWinner = lastWinner (drop 4 cards) (if winner == Nothing then player else (fromJust winner))

handle :: Cards -> Player -> [Cards]
handle [] _ = [[], [], [], []]
handle cards player
  | winner == A = zipWith (++) [take 4 cards, [], [], []] hands
  | winner == B = zipWith (++) [[], take 4 cards, [], []] hands
  | winner == C = zipWith (++) [[], [], take 4 cards, []] hands
  | winner == D = zipWith (++) [[], [], [], take 4 cards] hands
  | player == A = zipWith (++) [take 4 cards, [], [], []] hands
  | player == B = zipWith (++) [[], take 4 cards, [], []] hands
  | player == C = zipWith (++) [[], [], take 4 cards, []] hands
  | otherwise = zipWith (++) [[], [], [], take 4 cards] hands
  where
    Just winner = findWinner (head cards) (take 4 cards) player
    hands = handle (drop 4 cards) winner

findWinner :: Card -> Cards -> Player -> Maybe Player
findWinner firstCard cards player
  | null cards = Nothing
  | nextWinner == Nothing = if sameRank (head cards) firstCard then Just player else Nothing
  | otherwise = nextWinner
  where
    nextWinner = (findWinner firstCard (tail cards) (nextPlayer player))

replay :: Cards -> Maybe Winner
replay [] = Nothing
replay cards
  | firstTeamScore > secondTeamScore && gotNoTrick BD handCards = Just (AC, Three)
  | firstTeamScore > secondTeamScore && secondTeamScore == 0 = Just (AC, Two)
  | firstTeamScore > secondTeamScore = Just (AC, One)
  | secondTeamScore > firstTeamScore && gotNoTrick AC handCards = Just (BD, Three)
  | secondTeamScore > firstTeamScore && firstTeamScore == 0 = Just (BD, Two)
  | secondTeamScore > firstTeamScore = Just (BD, One)
  | otherwise = Nothing
  where
    handCards = handle cards A
    lastHandWinner = lastWinner cards A
    (firstTeamScore, secondTeamScore) = (handsScore handCards lastHandWinner)





    -- main = do
    --   -- Test got no trick function
    --   print (gotNoTrick BD [[Card Club RA], [], [Card Club R10], []]) -- true
    --   print (gotNoTrick AC [[Card Club RA], [], [Card Club R10], []]) -- false
    --   (testNextPlayer)
    --   (testHandScore)
    --   (testFindWinner)
    --   (testLastWinner)












-- I changed the list, so change it back when testing
testGame = [Card Heart R7,Card Heart R8,Card Heart R9,Card Heart R10,Card Heart RJ,Card Diamond R7,Card Diamond R8,Card Diamond R9,Card Diamond R10,Card Diamond RJ,Card Spade R7,Card Spade R8,Card Spade R9,Card Spade R10,Card Spade RJ,Card Club R7,Card Club R8,Card Club R9,Card Club R10,Card Club RJ]
testGame2 = []
testGame3 = [Card Club RA,Card Club RK,Card Club RQ,Card Club RJ,Card Club R10,Card Club R9,Card Club R7,Card Club R8,Card Spade RA,Card Spade RK,Card Spade RQ,Card Spade RJ,Card Spade R10,Card Spade R9,Card Spade R7,Card Spade R8,Card Diamond RA,Card Diamond RK,Card Diamond RQ,Card Diamond RJ,Card Diamond R7,Card Diamond R9,Card Diamond R8,Card Diamond R10,Card Heart RJ,Card Heart RK,Card Heart RQ,Card Heart RA,Card Heart R7,Card Heart R9,Card Heart R8,Card Heart R10]
testGame4 = [[Card Club RA], [Card Heart RA], [], [Card Club RK, Card Heart RK]]

testNextPlayer = do
  print("Test nextPlayer function")
  if nextPlayer A == B then print("OK") else error("Test Failed")
  if nextPlayer B == C then print("OK") else error("Test Failed")
  if nextPlayer C == D then print("OK") else error("Test Failed")
  if nextPlayer D == A then print("OK") else error("Test Failed")

testHandScore = do
  print("Test handScore function")
  if handScore [Card Club RA] == 10 then print("OK") else error("Test Failed")
  if handScore [] == 0 then print("OK") else error("Test Failed")
  if handScore [Card Club RA, Card Club R10, Card Heart R9] == 20 then print("OK") else error("Test Failed")
  if handScore [Card Heart R9] == 0 then print("OK") else error("Test Failed")


testFindWinner = do
  print("Test FindWinner function")
  if findWinner (Card Club RA) [Card Club RA, Card Club R10, Card Heart R9, Card Heart RA] A == Just D then print("OK") else error("Test Failed")
  if findWinner (Card Club RA) [Card Club RA, Card Club R10, Card Heart R9, Card Heart R7] A == Just D then print("OK") else error("Test Failed")
  if findWinner (Card Club RA) [Card Club RA, Card Club R10, Card Heart R9, Card Heart R8] A == Just A then print("OK") else error("Test Failed")
  if findWinner (Card Club R7) [Card Club R7, Card Club R10, Card Heart R9, Card Heart R8] A == Just A then print("OK") else error("Test Failed")
  if findWinner (Card Club R7) [Card Club R7, Card Club R10, Card Heart R7, Card Heart R8] A == Just C then print("OK") else error("Test Failed")
  if findWinner (Card Club RA) [Card Club RA, Card Club R10, Card Heart R9, Card Heart R9] C == Just C then print("OK") else error("Test Failed")
  if findWinner (Card Club R10) [Card Club R10, Card Club R9, Card Club R8, Card Heart R8] D == Just D then print("OK") else error("Test Failed")

testLastWinner = do
  print("Test lastWinner function")
  if lastWinner [Card Club RA, Card Club R10, Card Heart R9, Card Heart RA] A == Just D then print("OK") else error("Test Failed")
  if lastWinner [Card Club RA, Card Club R10, Card Heart R9, Card Heart RA, Card Club R10, Card Club R9, Card Club R8, Card Heart R8] A == Just D then print("OK") else error("Test Failed")
  if lastWinner [Card Club RA, Card Club R10, Card Heart R9, Card Heart RA, Card Club R10, Card Club R9, Card Club R8, Card Heart R8, Card Club RA, Card Club R10, Card Heart R9, Card Heart RA] A == Just C then print("OK") else error("Test Failed")
