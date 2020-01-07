main = do
  n <- getLine
  let result = fib (read n :: Int)
  print result
  n <- getLine
  let a = read n :: Float
  n <- getLine
  let b = read n :: Float
  n <- getLine
  let c = read n :: Float
  let x = root a b c
  print x

fib :: Int -> Int
fib n =
  let fibh = 0:1:zipWith (+) fibh (tail fibh)
  in fibh!!n

fib3 n
  | n <= 0 = 0
  | n == 1 = 1
  | otherwise = fib3 (n-1) + fib3 (n-2)

root :: Float -> Float -> Float -> (Float, Float)
root a b c = if d < 0 then error "0" else (x, y)
                        where
                          x = e + sqrt d / (2 * a)
                          y = e - sqrt d / (2 * a)
                          d = b * b - 4 * a * c
                          e = - b / (2 * a)

root2 :: Float -> Float -> Float -> (Float, Float)
root2 a b c =
  let e = - b / (2 * a)
      d = b * b - 4 * a * c
      x1 = e + sqrt d / (2 * a)
      x2 = e - sqrt d / (2 * a)
  in if d < 0 then error "0" else (x1, x2)





