#|
Bryan Hinton
HW-11
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#

;**************************************************************
;Write a union function
;**************************************************************

(define member? (lambda (s los) (cond ((null? los) #f) ((equal? (car los) s) #t) (else (set! los (cdr los)) (member? s los)))))
(define union (lambda (lst1 lst2) (if (null? lst1) (union-aux lst2 lst1 '()) (union-aux lst1 lst2 '()))))
(define union-aux (lambda (lst1 lst2 lst3) (if (null? lst1) (if (null? lst2) lst3 (if (member? (car lst2) lst3) (union-aux (cdr lst2) lst1 lst3) (union-aux (cdr lst2) lst1 (list (append lst3 (car lst2))))))
(if (member? (car lst1) lst3) (union-aux (cdr lst1) lst2 lst3) (union-aux (cdr lst1) lst2 (append lst3 (list (car lst1)))))
)))
;test cases
(union () '(a b a))
;Value: (a b)

(union '(x y 7) '(7 z x w)) 
;Value: (x y 7 z w)

;**************************************************************
;Write prod and fold function
;**************************************************************

(define prod (lambda (lst) (apply * lst)))

;test cases
(prod '(1 2 3 4 5 6))
;value: 720

(define fold (lambda (op base lst) (op base (apply op lst))))

;test cases
(fold * 1 '(1 2 3 4 5))
;value: 120
(fold + 0 '(1 2 3 4 5))
;value: 15
(fold + 0 (fold append '() '((3 1 2) (3 4 5) (3 5 6))))
;We were supposed to evaluate this function without the compiler first
;I came up with 32 and that answer was then confirmed by the compiler