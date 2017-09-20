#|
Bryan Hinton
HW-9
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#

;**************************************************************
;Exercise 2.2.7

;Problem 1
;**************************************************************

(define duple (lambda (n x) (if (zero? n) (list) (if (equal? n 1) (list x) (cons x (duple (- n 1) x))))))

;test cases
(duple 2 3)
;Value: (3 3)

(duple 4 '(ho ho))
;Value: ((ho ho) (ho ho) (ho ho) (ho ho))


;**************************************************************
;Exercise 2.2.7

;Problem 2
;**************************************************************

(define invert (lambda (lst) (if (null? (cdr lst)) 
	(list (list (cadr (car lst)) (car (car lst)))) 
	(cons (list (cadr (car lst)) (car (car lst))) (invert (cdr lst)))))
	
;test cases
(invert '((a 1) (a 2) (b 1) (b 2)))
;value: ((1 a) (2 a) (1 b) (2 b))	

;**************************************************************
;Exercise 2.2.7

;Problem 3
;**************************************************************

(define list-index (lambda (x los) (if (null? (cdr los)) (if (equal? x (car los)) 0 -1) (if (equal? x (car los)) 0 (list-aux x (cdr los) 1)))))
(define list-aux (lambda (x los y) (if (null? (cdr los)) (if (equal? x (car los)) y -1) (if (equal? x (car los)) y (list-aux x (cdr los) (+ y 1))))))

;test cases
(list-index 'c '(a b c d))
;value: 2
(list-ref '(a b c) (list-index 'b '(a b c)))
;value: b

;**************************************************************
;Exercise 2.2.7

;Problem 4
;**************************************************************

(define vector-index (lambda (x vos) (if (equal? (vector-length vos) 1) (if (equal? (vector-ref vos 0) x) 0 -1) (if (equal? (vector-ref vos 0) x) 0 (vector-aux x vos 1))))) 
(define vector-aux (lambda (x vos y) (if (equal? (vector-length vos) (+ y 1)) 
(if (equal? (vector-ref vos y) x) y -1) 
(if (equal? (vector-ref vos y) x) y (vector-aux x vos (+ y 1)))))) 

;test cases
(vector-index 'c '#(a b c d))
;value: 2
(vector-ref '#(a b c) (vector-index 'b '#(a b c)))
;value: b

;**************************************************************
;Exercise 2.2.7

;Problem 5
;**************************************************************


(define ribassoc (lambda (s los v fail-value) (if (equal? -1 (list-index s los)) fail-value (vector-ref v (list-index s los)))))

;test cases
(ribassoc 'b '(a b c) '#(1 2 3) 'fail)
;value: 2
(ribassoc 'c '(a b foo) '#(3 squiggle bar) 'fail)
;value: fail
(ribassoc 'i '(a i o i) '#(fx (fz) () (fm fe)) 'fail)
;value: (fz)
