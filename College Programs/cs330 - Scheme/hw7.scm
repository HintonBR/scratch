#|
Bryan Hinton
HW-7
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

**************************************************************
Exercise 1.3.5
|#

(define compose-c (lambda (x) (lambda (y) (lambda (z) (x (y z))))))

(((compose-c cdr) cdr) '(1 2 3 4))
;Value: (3 4)
((compose-c cdr) cdr)
;Value: #[compound-procedure 6]

#|
They allow additional functions of the same type to be defined using various subsets of the
necessary parameters and they can be profitably used to improve program maintenance.

**************************************************************
HW-7 problem about sum and even-parity functions
|#

(define sum (lambda x (if (null? (cdr x)) (car x) (+ (car x) (sum-aux (cdr x))))))

(define sum-aux (lambda (y) (if (null? (cdr y)) (car y) (+ (sum-aux (cdr y)) (car y)))))

(sum 1 2)
;Value: 3
(sum 3 4 5)
;Value: 12


(define even-parity (lambda x (if (null? (cdr x)) (if (zero? (car x)) #t #f) 
(if (equal? 1 (apply + x)) #f (if (equal? 3 (apply + x)) #f #t)))))
(even-parity 1 0)
;Value: ()
(even-parity 1 0 1)
;Value: #t
