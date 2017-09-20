#|
Bryan Hinton
HW-12
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#

;**************************************************************
;Exercise 2.1.3
;**************************************************************
#|
<list>
(<datum> <datum> <datum>)
(<symbol> <datum> <datum>)
(a <string> <datum>)
(a "mixed" <vector>)
(a "mixed" #(<datum> <datum>))
(a "mixed" #(<symbol> <datum>))
(a "mixed" #(bag <datum>))
(a "mixed" #(bag <dotted-datum>))
(a "mixed" #(bag (<datum> . <datum>)))
(a "mixed" #(bag (<symbol> . <datum>)))
(a "mixed" #(bag (of . <datum>)))
(a "mixed" #(bag (of . <symbol>)))
(a "mixed" #(bag (of . data)))

We know that (a . b . c) is not a member of the language because it can't be
derived from the rules defined for the grammer
|#

;**************************************************************
;Exercise 2.3.1
;**************************************************************

;define test expression
(define mexp '(lambda (x) ((lambda (z) (x y)) (lambda (y) (y)))))

;define lambda expressions
(define is-lambda? (lambda (lst) (eq? (car lst) 'lambda)))

    (define lambda-formals  (lambda (lst) (cadr lst)))

    (define lambda-body  (lambda (lst) (caddr lst)))

    ;;; test lambda syntax functions

    (is-lambda? '(lambda (vars) body))

    (lambda-formals '(lambda  (vars) body))

    (lambda-body '(lambda (vars) body))

;redefine union functions from previous assignment
(define member? (lambda (s los) (cond ((null? los) #f) ((equal? (car los) s) #t) (else (set! los (cdr los)) (member? s los)))))

(define union (lambda (lst1 lst2) (if (null? lst1) (union-aux lst2 lst1 '()) (union-aux lst1 lst2 '()))))

(define union-aux (lambda (lst1 lst2 lst3) (if (null? lst1) (if (null? lst2) lst3 (if (member? (car lst2) lst3) (union-aux (cdr lst2) lst1 lst3) (union-aux (cdr lst2) lst1 (append lst3 (list (car lst2))))))
(if (member? (car lst1) lst3) (union-aux (cdr lst1) lst2 lst3) (union-aux (cdr lst1) lst2 (append lst3 (list (car lst1)))))
)))

;define bound-vars functions
    (define bound-vars (lambda (expr)	(if (is-lambda? expr) 
	(bound-vars-aux (cddr expr) '() (cons (car (lambda-formals expr)) '()))
	(if (symbol? (car expr)) 
	      (bound-vars-aux (cdr expr) '() '()) 
	 (union (bound-vars-aux (car expr) '() '()) (bound-vars-aux (cdr expr) '() '()))))))

    (define bound-vars-aux (lambda (expr var-lst b-lst) (if (null? expr) var-lst 
        (if (is-lambda? expr) 
	(bound-vars-aux (cddr expr) var-lst (append b-lst (cons (car (lambda-formals expr)) '())))
	 (if (symbol? (car expr))
	(bound-vars-aux (cdr expr) (if (member? (car expr) b-lst) (union var-lst (list (car expr))) var-lst) b-lst) 
	 (union var-lst (union (bound-vars-aux (car expr) '() b-lst) (bound-vars-aux (cdr expr) '() b-lst))))))))

(bound-vars mexp)
;value = (x y)

;define free-vars functions
    (define free-vars (lambda (expr)	(if (is-lambda? expr) 
	(free-vars-aux (cddr expr) '() (cons (car (lambda-formals expr)) '()))
	(if (symbol? (car expr)) 
	      (free-vars-aux (cdr expr) (list (car expr)) '()) 
	 (union (free-vars-aux (car expr) '() '()) (free-vars-aux (cdr expr) '() '()))))))

    (define free-vars-aux (lambda (expr var-lst b-lst) (if (null? expr) var-lst 
        (if (is-lambda? expr) 
	(free-vars-aux (cddr expr) var-lst (append b-lst (cons (car (lambda-formals expr)) '())))
	 (if (symbol? (car expr))
	(free-vars-aux (cdr expr) (if (member? (car expr) b-lst) var-lst (union var-lst (list (car expr)))) b-lst) 
	 (union var-lst (union (free-vars-aux (car expr) '() b-lst) (free-vars-aux (cdr expr) '() b-lst))))))))

(free-vars mexp)
;value = (y)
;**************************************************************
;Exercise 2.3.8
;**************************************************************
#|
(lambda (x.1) (lambda (y.1) ((lambda (x.2) (x.2 y.1)) x.1)))

(lambda (z.1) ((lambda (a.1 b.1 c.1) (a.1 (lambda (a.2) (+ a.2 c.1)) b.1)) (lambda (f.1 x.1) (f.1 (z.1 x.1)))))

|#




