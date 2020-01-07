#lang scheme

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
    ((eq? (orientation? state) 'north) (eq? (at? (- (x? state) 1) (y? state) state) 'w))
    ((eq? (orientation? state) 'south) (eq? (at? (+ (x? state) 1) (y? state) state) 'w))
    ((eq? (orientation? state) 'east) (eq? (at? (x? state) (- (y? state) 1) state) 'w))
    ((eq? (orientation? state) 'west) (eq? (at? (x? state) (+ (y? state) 1) state) 'w))))

; get orientation
(define (north? state)
  (if (eq? (orientation? state) 'north) #t #f))

; get procedure by name
(define (get-procedure prg name)
  (if (null? prg) null
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
(define (change-val-1d maze y val)
  (if (= y 0) (cons val (cdr maze))
      (cons (car maze) (change-val-1d (cdr maze) (- y 1) val))))

; get all steps done until now
(define (steps? state)
  (if (null? (cddr (cdr state))) '()
  (caddr (cdr state))))

(define (broken? state) ;reverse array
  (if (null? state) #f
      (if (eq? (cdr state) 'broken-state) #t
          (broken? (cdr state)))))

; STEP
(define (step state expr prg limit)
  (if (wall? state) state
      (cond
        ((eq? (orientation? state) 'north) (recon (list (maze? state) (list (- (x? state) 1) (y? state)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit))
        ((eq? (orientation? state) 'south) (recon (list (maze? state) (list (+ (x? state) 1) (y? state)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit))
        ((eq? (orientation? state) 'east) (recon (list (maze? state) (list (x? state) (- (y? state) 1)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit))
        ((eq? (orientation? state) 'west) (recon (list (maze? state) (list (x? state) (+ (y? state) 1)) (orientation? state) (append (steps? state) '(step))) (if (atom? expr) '() (cdr expr)) prg limit)))))

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
  (let ((res (recon state expr prg limit)))
    (display (list (steps? res) (maze? res) (xy? res) (orientation? res)))))

(define get-maze
'(
(w w w w w w w w w)
(w 3 0 w 0 0 0 0 w)
(w w 0 w 0 w w 0 w)
(w 0 1 0 0 0 0 8 w)
(w w 0 0 0 w w 0 w)
(w 0 5 0 w 0 1 0 w)
(w 4 w 0 w 1 7 0 w)
(w w w w w w w w w)
)
)

(define program
  '(
    (procedure add-mark-to-maze
      (if mark?
       (get-mark
        (if mark?
          (put-mark)
          ( put-mark put-mark
            (if wall? () (step add-mark-to-maze step-back))
            turn-left
            (if wall? () (step add-mark-to-maze step-back))
            turn-left
            turn-left
            (if wall? () (step add-mark-to-maze step-back))
            turn-left get-mark
          ))
       ) (put-mark add-mark-to-maze get-mark)
     ))
    (procedure step-back
      (turn-left turn-left step turn-left turn-left)
    )
    )
)
(newline)
(simulate (list get-maze (list 4 1) 'west) 'add-mark-to-maze program 20)
