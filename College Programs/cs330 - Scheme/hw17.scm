#|
Bryan Hinton
HW-17
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

|#
(load "/usr/local/lib/mit-scheme/records.scm")
;**************************************************************
;Exercise 3.4.5
;**************************************************************
(define-record lit (datum))
(define-record varref (var))
(define-record lambda (formal body))
(define-record app (rator rand))

(define parse
  (lambda (datum)
    (cond
      ((number? datum) (make-lit datum))
      ((symbol? datum) (make-varref datum))
      ((pair? datum)
       (if (eq? (car datum) 'lambda)
           (make-lambda (caadr datum) (parse (caddr datum)))
           (make-app (parse (car datum)) (parse (cadr datum)))))
      (else (error "parse: Invalid concrete syntax" datum)))))

(define unparse
  (lambda (exp)
    (variant-case exp
      (lit (datum) datum)
      (varref (var) var)
      (lambda (formal body) 
        (list 'lambda (list formal) (unparse body)))
      (app (rator rand) (list (unparse rator) (unparse rand)))
      (else (error "unparse: Invalid abstract syntax" exp)))))

(define free-vars
  (lambda (exp)
    (variant-case exp
      (lit (datum) '())
      (varref (var) (list var))
      (lambda (formal body) (remove formal (free-vars body)))
      (app (rator rand) 
        (union (free-vars rator) (free-vars rand))))))

;redefine union functions from previous assignment
(define member? (lambda (s los) (cond ((null? los) #f) ((equal? (car los) s) #t) (else (set! los (cdr los)) (member? s los)))))

(define union (lambda (lst1 lst2) (if (null? lst1) (union-aux lst2 lst1 '()) (union-aux lst1 lst2 '()))))

(define union-aux (lambda (lst1 lst2 lst3) (if (null? lst1) (if (null? lst2) lst3 (if (member? (car lst2) lst3) (union-aux (cdr lst2) lst1 lst3) (union-aux (cdr lst2) lst1 (append lst3 (list (car lst2))))))
(if (member? (car lst1) lst3) (union-aux (cdr lst1) lst2 lst3) (union-aux (cdr lst1) lst2 (append lst3 (list (car lst1)))))
)))


(define bound-vars
	(lambda (expr)
	(bound-vars-aux (parse expr) '())))

(define bound-vars-aux 
	(lambda (expr blst)
	(variant-case expr
	(lit (datum) blst)
	(varref (var) blst)
	(lambda (formal body) (bound-vars-aux body (union blst (cons formal '()))))
	(app (rator rand) (union (bound-vars-aux rator blst) (bound-vars-aux rand blst)))
	))	
)

;test cases for bound-vars

(define yeah '(lambda (x) ((lambda (z) (x y))
	   (lambda (y) y))))

(bound-vars yeah)


(define free? (lambda (var expr) (free-aux? var expr '())))
(define free-aux? 	
	(lambda (s expr blst)
	(variant-case expr
	(lit (datum) #t)
	(varref (var) (if (equal? s var) (if (member? var blst) #f #t) #t))
	(lambda (formal body) (free-aux? s body (union blst (cons formal '()))))
	(app (rator rand) (and (free-aux? s rator blst) (free-aux? s rand blst)))
	))	
)

;test cases for free?
(free? 'x (parse '(lambda (y) (x (x y)))))
;value = #t
(free? 'y (parse '(lambda (y) (x (x y)))))
;value = #f
