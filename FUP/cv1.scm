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

(define (D a b c)
  (- (* b b) (* 4 a c)))

(define (Q a b c)
  (if (< (D a b c) 0)
      null
      (list
       (/ (+ (- 0 b) (sqrt (D a b c))) (* 2 a))
       (/ (- (- 0 b) (sqrt (D a b c))) (* 2 a)))))

(define (even? x)
  (if (= x 0) #t
     (if (< x 0) #f
         (even? (- x 2)))))

(define (pow2? x)
  (if (= x 2) #t
      (if (< (- x 2) 0) #f
          (pow2? (/ x 2)))))