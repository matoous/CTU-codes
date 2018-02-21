#lang racket

(define (quicksort arr)
    (if (null? arr)
        arr
        (append
         (quicksort (filter (lambda(x) (< x (car arr))) (cdr arr)))
         (list (car arr))
         (quicksort (filter (lambda(x) (>= x (car arr))) (cdr arr))))))

(define (collatz x)
  (define (helper x i)
    (if (= x 1) i
        (if (even? x) (helper (/ x 2) (+ i 1))
            (helper (+ (* x 3) 1) (+ i 1)))))
  (helper x 0))

(define (D a b c) ; diskriminant
  (- (* b b) (* 4 a c)))

(define (Q a b c) ; reseni kvadraticke rovnice
  (if (< (D a b c) 0)
      null
      (list
       (/ (+ (- 0 b) (sqrt (D a b c))) (* 2 a))
       (/ (- (- 0 b) (sqrt (D a b c))) (* 2 a)))))

(define (even? x)
  (if (< x 0) (even? (- 0 x))
      (if (= x 0) #t
          (if (= x 1) #f
              (even? (- x 2))))))

(define (pow2? x)
  (if (= x 2) #t
      (if (= (- x (floor x)) 0) (pow2? (/ x 2))
          #f)))

(define (:randarr l)
  (if (= l 0) null
      (append (list (random 100000)) (:randarr (- l 1)))))

(define (fib n)
  (cond
    ((= n 0) 0)
    ((= n 1) 1)
    (else (+ (fib (- n 1)) (fib (- n 2))))))

(define (fibs n)
  (if (null? n) null
      (append (list (fib (car n))) (fibs (cdr n)))))

(define (range n) ;range of numbers 1 ... n
  (if (= n 0) null
      (append (range (- n 1)) (list n))))

(define (reverse n) ;reverse array
  (if (null? n) null
      (append (reverse (cdr n)) (list (car n)))))

(define (fib? n) ;is it fib number?
  (define (helper num curr prev)
    (cond
      ((= (- num curr) 0) #t)
      ((< (- num curr) 0) #f)
      (else (helper num (+ curr prev) curr))))
  (helper n 1 0))

(define (ffib n) ;linear fib
  (define (helper n prev curr)
    (if (= n 0) prev
        (helper (- n 1) curr (+ curr prev))))
  (helper n 0 1))

(define (reduce arr con) ;implementation of own reduce -> con = condition, lambda function to apply
  (if (null? arr) null
      (append (if (con (car arr)) (list (car arr)) null) (reduce (cdr arr) con))))

(define (sum n) ;sum from 1..n
  (* (/ (+ n 1) 2) n))

(define (ssum n) ;ssum as slow sum
  (define (helper n acc)
    (if (= n 0) acc
        (helper (- n 1) (+ acc n))))
  (helper n 0))
