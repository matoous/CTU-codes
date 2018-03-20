#lang scheme

(define comparem (lambda (x y)
       (if (not (eq? (car (car x)) (car (car y)))) (< (car (car x)) (car (car y)))
           (if (not (eq? (car (cdr (car x))) (car (cdr (car y))))) (< (car (cdr (car x))) (car (cdr (car y))))
               (if (not (eq? (car (cddr (car x))) (car (cddr (car y))))) (< (car (cddr (car x))) (car (cddr (car y))))
                   (< (car (cddr (cdr (car x)))) (car (cddr (cdr (car y))))))))))

(define (insert x lst)
  (if (null? lst)
      (list x)
      (let ((y (car lst))
            (ys (cdr lst)))
        (if (comparem x y)
            (cons x lst)
            (cons y (insert x ys))))))
 
(define (insertion-sort lst)
  (if (null? lst)
      '()
      (insert (car lst)
              (insertion-sort (cdr lst)))))

; get maze from state
(define (maze? state)
  (car state))
 
; get x coordinate from state
(define (x? state)
  (car (cadr state)))
 
; get y coordinate from state
(define (y? state)
  (cadr (cadr state)))
 
; get orientation from state
(define (orientation? state)
  (cadr (cdr state)))
 
; get mark
(define (mark? state)
  (if (> (at? (x? state) (y? state) state) 0) #t #f))
 
; get what I am standing at
(define (at? x y state)
  (define return (lambda(x) (if (pair? x) (car x) x)))
  (if (= y 0)
    (if (= x 0)
        (return (car (car state)))
        (at? (- x 1) y (list (list (cdr (car (maze? state)))))))
    (at? x (- y 1) (list (cdr (maze? state))))))
 
; what is under me now?
(define (now-at? state)
  (at? (x? state) (y? state) state))
 
; get wall in front of me?
(define (wall? state)
  (cond
    ((eq? (orientation? state) 'north) (eq? (at? (x? state) (- (y? state) 1) state) 'w))
    ((eq? (orientation? state) 'south) (eq? (at? (x? state) (+ (y? state) 1) state) 'w))
    ((eq? (orientation? state) 'east) (eq? (at? (+ (x? state) 1) (y? state) state) 'w))
    ((eq? (orientation? state) 'west) (eq? (at? (- (x? state) 1) (y? state) state) 'w))))
 
; get orientation
(define (north? state)
  (if (eq? (orientation? state) 'north) #t #f))
 
; get procedure by name
(define (get-procedure prg name)
  (if (null? prg) '()
      (if (and (eq? (car (car prg)) 'procedure) (eq? (cadr (car prg)) name)) (cddr (car prg))
          (get-procedure (cdr prg) name))))
 
; is expresion atom
(define (atom? expr)
  (if (and (not (null? expr)) (not (pair? expr))) #t #f))
 
; next orientation when turning left
(define (next-orientation? state)
  (define (helper orientation olist)
    (if (eq? (car olist) orientation) (cadr olist)
        (helper orientation (cdr olist))))
  (helper (orientation? state) '(north west south east north)))
 
; pair (x , y) coordinates
(define (xy? state)
  (cadr state))
 
; change value at given index in maze
(define (change-val-2d maze x y val)
  (if (= y 0) (cons (change-val-1d (car maze) x val) (cdr maze))
      (cons (car maze) (change-val-2d (cdr maze) x (- y 1) val))))
 
; change value at given index in row
(define (change-val-1d maze x val)
  (if (= x 0) (cons val (cdr maze))
      (cons (car maze) (change-val-1d (cdr maze) (- x 1) val))))
 
; get all steps done until now
(define (steps? state)
  (if (null? (cddr (cdr state))) '()
      (if (eq? (cddr (cdr state)) 'broken-state) '()
          (caddr (cdr state)))))
 
(define (broken? state) ;reverse array
  (if (null? state) #f
      (if (eq? (cdr state) 'broken-state) #t
          (broken? (cdr state)))))
 
; STEP
(define (step state expr prg limit)
  (if (wall? state) (append state 'broken-state)
      (cond
        ((eq? (orientation? state) 'north) (recon (list (maze? state) (list (x? state) (- (y? state) 1)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit))
        ((eq? (orientation? state) 'south) (recon (list (maze? state) (list (x? state) (+ (y? state) 1)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit))
        ((eq? (orientation? state) 'east) (recon (list (maze? state) (list (+ (x? state) 1) (y? state)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit))
        ((eq? (orientation? state) 'west) (recon (list (maze? state) (list (- (x? state) 1) (y? state)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit)))))
 
; TURN LEFT
(define (turn-left state expr prg limit)
  (recon (list (maze? state) (xy? state) (next-orientation? state) (append (steps? state) '(turn-left))) (if (atom? expr) '() (cdr expr)) prg limit))
 
; PUT MARK
(define (put-mark state expr prg limit)
  (recon (list (change-val-2d (maze? state) (x? state) (y? state) (+ (now-at? state) 1)) (xy? state) (orientation? state) (append (steps? state) '(put-mark))) (if (atom? expr) '() (cdr expr)) prg limit))
 
; GET MARK
(define (get-mark state expr prg limit)
  (if (< 0 (at? (x? state) (y? state) state)) (recon (list (change-val-2d (maze? state) (x? state) (y? state) (- (now-at? state) 1)) (xy? state) (orientation? state) (append (steps? state) '(get-mark))) (if (atom? expr) '() (cdr expr)) prg limit)
      state))
 
; recognize command
(define (recon state expr prg limit)
  (cond
    ((broken? state) state) ; we breached the recursion limit, exit immidietly
    ((eq? expr 'step) (step state expr prg limit))
    ((eq? expr 'turn-left) (turn-left state expr prg limit))
    ((eq? expr 'put-mark) (put-mark state expr prg limit))
    ((eq? expr 'get-mark) (get-mark state expr prg limit))
    ((list? expr) (cond ; not a single expression but list of expressions
                    ((null? expr) state) ; empty list
                    ((not (eq? (car expr) 'if)) (recon (recon state (car expr) prg limit) (cdr expr) prg limit)) ; list of expressions
                    ((eq? (cadr expr) 'wall?) (if (wall? state) (recon state (cadr (cdr expr)) prg limit) (recon state (cadr (cddr expr)) prg limit))) ; if wall
                    ((eq? (cadr expr) 'north?) (if (north? state) (recon state (cadr (cdr expr)) prg limit) (recon state (cadr (cddr expr)) prg limit))) ; if north
                    ((eq? (cadr expr) 'mark?) (if (mark? state) (recon state (cadr (cdr expr)) prg limit) (recon state (cadr (cddr expr)) prg limit))))) ; if mark
    (else (if (= limit 0) (append state 'broken-state) ; calling recursion too deep
       (recon state (get-procedure prg expr) prg (- limit 1)))))) ; calling a function
 
; ENTRY POINT
(define (simulate state expr prg limit)
  (let ((res (recon (append state '()) expr prg limit)))
     (list (steps? res) (list (maze? res) (xy? res) (orientation? res)))))

; PROGRAM LENGTH
(define (prglen prg)
  (if (pair? prg) (+ (prglen (car prg)) (prglen (cdr prg)))
      (if (or (eq? prg '()) (eq? prg 'procedure) (eq? prg 'if)) 0 1)))

; ZIP SUM
(define (suml l1 l2)
  ;(display "sum: ")
  ;(display l1)
  ;(display " ")
  ;(display l2)
  ;(newline)
  (if (pair? l1) (cons (+ (car l1) (car l2)) (suml (cdr l1) (cdr l2))) '()))

(define (my-flatten lst)
  (cond ((null? lst) '())
        ((pair? (car lst))  ; it's more efficient if we use `pair?`
         (append (my-flatten (car lst)) (my-flatten (cdr lst))))
        (else (cons (car lst) (my-flatten (cdr lst))))))

;MANHATTEN DISTANCE
(define (manhatten-dist m1 m2)
  (define (sum-dif l1 l2)
    (if (pair? l1) (+ (if (eq? (car l1) 'w) 0 (abs (- (car l1) (car l2)))) (sum-dif (cdr l1) (cdr l2))) 0))
  (sum-dif (my-flatten m1) (my-flatten m2)))

; IN LIMIT
(define (in-limit score limit)
  (if (pair? score)
      (if (<= (car score) (car limit)) (in-limit (cdr score) (cdr limit)) #f)
      #t))

(define (runprg prg pair limits stack-size scores)
  ;(display "run program\n")
  ;(display pair)
  ;(newline)
  ;(display "out:\n")
  (let* ((res (simulate (car pair) 'start prg stack-size))
        (res-state (car (cdr res)))
        (want-state (car (cdr pair))))
    ;(display res)
    ;(newline)
    (list
     (manhatten-dist (maze? res-state) (maze? want-state))
     (+ (abs (- (x? res-state) (x? want-state))) (abs (- (y? res-state) (y? want-state))) (if (eq? (orientation? res-state) (orientation? want-state)) 0 1)) ;config distance
     0 ;length of program
     (length (car res)) ;num of steps
     )))

(define (my-eval prg pairs limits stack-size scores)
  ;(display "eval program\n")
  ;(display prg)
  ;(newline)
  ;(display "prg len\n")
  ;(display (prglen prg))
  ;(newline)
  (cond ((< (cadr (cdr limits)) (prglen prg)) '()) ; program too long
        ((not (in-limit scores limits)) '())
        ((null? pairs) (cons (change-val-1d scores 2 (prglen prg)) (list prg)))
        ((list? pairs) (my-eval prg (cdr pairs) limits stack-size (suml scores (runprg prg (car pairs) limits stack-size scores))))))

(define (evalall prgs pairs limits stack-size)
  (if (null? prgs) '()
       (let ((res (my-eval (car prgs) pairs limits stack-size '(0 0 0 0))))
         (if (null? res) (evalall (cdr prgs) pairs limits stack-size)
             (cons
              (my-eval (car prgs) pairs limits stack-size '(0 0 0 0))
              (evalall (cdr prgs) pairs limits stack-size))))))

(define (evaluate prgs pairs limits stack-size)
  ;(display "RUNNING:\n")
  ;(display prgs)
  ;(newline)
  ;(display pairs)
  ;(newline)
  ;(display limits)
  ;(newline)
  ;(display stack-size)
  ;(newline)
  ;(newline)
  ;(newline)
  ;(newline)
  ;(display (evalall prgs pairs limits stack-size))
  ;(newline)
  ;(newline)
  (let* ((res (evalall prgs pairs limits stack-size)))
    (insertion-sort
     res)))