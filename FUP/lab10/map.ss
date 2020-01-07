#lang racket

(define (init map)
  map)

(define (add map key val)
  (cond ((eq? map '()) (list (cons key val)))
        ((eq? (car (car map)) key) map)
        (#t (cons (car map) (add (cdr map) key val) ))))

(define (lookup map key default)
  (cond ((eq? map '()) default)
        ((eq? (car (car map)) key) (cdr (car map)))
        (#t (lookup (cdr map) key default))))

(define (delete map key)
  (cond ((eq? map '()) '())
        ((eq? (car (car map)) key) (cdr map))
        (#t (cons (car map) (delete (cdr map) key)))))
