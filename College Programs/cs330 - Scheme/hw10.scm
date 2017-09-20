#|
Bryan Hinton
HW-10
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#

;**************************************************************
;Exercise 2.2.7

;Problem 6
;**************************************************************

(define filter-in (lambda (p lst) (if (null? (cdr lst)) (if (p (car lst)) (cons (car lst) '()) '()) (if (p (car lst)) 
(cons (car lst) (filter-in p (cdr lst)))
	(filter-in p (cdr lst))))))

;test cases
(filter-in number? '(a 2 (1 3) b 7))
;Value: (2 7)

(filter-in symbol? '(a (b c) 17 foo))
;Value: (a foo)


;**************************************************************
;Exercise 2.2.7

;Problem 7
;**************************************************************

(define product (lambda (lst1 lst2) (if (null? lst1) '() (append (prodaux (car lst1) lst2) (product (cdr lst1) lst2)))))
(define prodaux (lambda (a alst2) (if (null? alst2) '() (cons (list a (car alst2)) (prodaux a (cdr alst2))))))
	
;test cases
(product '(a b c) '(x y))
;value: ((a x) (a y) (b x) (b y) (c x) (c y))

;**************************************************************
;Exercise 2.2.7

;Problem 8
;**************************************************************

(define swapper (lambda (x y lst) (if (null? lst) '() (if (equal? x (car lst)) (cons y (swapper x y (cdr lst)))
(if (equal? y (car lst)) (cons x (swapper x y (cdr lst)))
(if (list? (car lst)) (cons (swapper x y (car lst)) (swapper x y (cdr lst))) (cons (car lst) (swapper x y (cdr lst)))
))))))

;test cases
(swapper 'a 'd '(a b c d))
;value: (d b c a)
(swapper 'x 'y '((x) y (z (x))))
;value: ((y) x (z (y)))

;**************************************************************
;Exercise 2.2.7

;Problem 9
;**************************************************************

(define rotate (lambda (lst) (if (null? lst) '() (cons (rotatelast lst) (rotateend lst)))))
(define rotatelast (lambda (lst) (if (null? (cdr lst)) (car lst) (rotatelast (cdr lst)))))
(define rotateend (lambda (lst) (if (null? (cdr lst)) '() (cons (car lst) (rotateend (cdr lst))))))

;test cases
(rotate '(a b c d))
;value: (d a b c)
(rotate '(notmuch))
;value: (notmuch)
(rotate '())
;value: ()