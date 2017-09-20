#|
Bryan Hinton
HW-13
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#

;**************************************************************
;Exercise 2.1.10
;**************************************************************

;define lambda expressions
(define is-lambda? (lambda (lst) (eq? (car lst) 'lambda)))
(define lambda-formals  (lambda (lst) (cadr lst)))
(define lambda-body  (lambda (lst) (caddr lst)))

;define if functions
(define is-if? (lambda (lst) (eq? (car lst) 'if)))
(define if-test (lambda (lst) (cadr lst)))
(define if-true (lambda (lst) (caddr lst)))
(define if-else (lambda (lst) (cadddr lst)))

;redefine list-index functions
(define list-index (lambda (x los) (if (null? (cdr los)) (if (equal? x (car los)) 0 -1) (if (equal? x (car los)) 0 (list-aux x (cdr los) 1)))))
(define list-aux (lambda (x los y) (if (null? (cdr los)) (if (equal? x (car los)) y -1) (if (equal? x (car los)) y (list-aux x (cdr los) (+ y 1))))))

;redefine union functions from previous assignment
(define member? (lambda (s los) (cond ((null? los) #f) ((equal? (car los) s) #t) (else (set! los (cdr los)) (member? s los)))))
(define union (lambda (lst1 lst2) (if (null? lst1) (union-aux lst2 lst1 '()) (union-aux lst1 lst2 '()))))
(define union-aux (lambda (lst1 lst2 lst3) (if (null? lst1) (if (null? lst2) lst3 (if (member? (car lst2) lst3) (union-aux (cdr lst2) lst1 lst3) (union-aux (cdr lst2) lst1 (append lst3 (list (car lst2))))))
(if (member? (car lst1) lst3) (union-aux (cdr lst1) lst2 lst3) (union-aux (cdr lst1) lst2 (append lst3 (list (car lst1)))))
)))

;define free-vars functions
    (define free-vars (lambda (expr) (free-vars-aux expr '())))
    (define free-vars-aux (lambda (expr b-lst)  (if (null? expr) '() 
	(if (symbol? expr)  (if (member expr b-lst) '() (list expr))
        (if (is-if? expr) (free-vars-aux (cdr expr) b-lst)
	(if (is-lambda? expr) (free-vars-aux (cddr expr) (append b-lst (lambda-formals expr)))
	(union  (free-vars-app (car expr) b-lst) (free-vars-aux (cdr expr) b-lst))))))))
	
    (define free-vars-app (lambda (expr b-lst) (if (list? expr) 
       (if (is-lambda? expr) 
       (free-vars-aux (cddr expr) (append b-lst (lambda-formals expr)))
       (if (is-if? expr) (free-vars-aux (cdr expr) b-lst)
	(union  (free-vars-app (car expr) b-lst) (free-vars-aux (cdr expr) b-lst))))
    (if (symbol? expr) 
       (if (member expr b-lst) '() (list expr))
	'()))))

;define mk-var-ref
(define mk-var-ref (lambda (x lst) (mk-var-ref-aux x lst 0)))
(define mk-var-ref-aux (lambda (x lst ind) (if (null? lst) x
	(if (member x (car lst)) (list x ': ind (list-index x (car lst)))
	(mk-var-ref-aux x (cdr lst) (+ ind 1))))))

;define lexical address

;define lexical address
(define lexical-address (lambda (x) (lexical-address-aux x (list (free-vars x)))))
(define lexical-address-aux (lambda (expr b-lst)
	(cond ((null? expr) '())
	      ((symbol? expr) (mk-var-ref expr b-lst))
              ((is-if? expr) (list 'if (lexical-address-app (cadr expr) b-lst) (lexical-address-aux (if-true expr) b-lst) (lexical-address-aux (if-else expr) b-lst)))
              ((is-lambda? expr) (list 'lambda (lambda-formals expr) (lexical-address-aux (lambda-body expr) (cons (lambda-formals expr) b-lst))))
	      (else (cons (lexical-address-app (car expr) b-lst) (lexical-address-aux (cdr expr) b-lst))) 	
	)
))

(define lexical-address-app (lambda (expr b-lst) (if (list? expr)
	(if (is-lambda? expr)
	(list 'lambda (lambda-formals expr) (lexical-address-aux (lambda-body expr) (cons (lambda-formals expr) b-lst)))
	(if (is-if? expr) (list 'if (lexical-address-app (cadr expr) b-lst) (lexical-address-aux (if-true expr) b-lst) (lexical-address-aux (if-else expr)))
		(cons (lexical-address-app (car expr) b-lst) (lexical-address-aux (cdr expr) b-lst))))
	(if (symbol? expr) (mk-var-ref expr b-lst) (list expr)))))


;define test expression
(define testexp '(lambda (a b c) (if (eq? b c) ((lambda (c) (cons a c)) a) b)))
(lexical-address testexp)
;value = 





