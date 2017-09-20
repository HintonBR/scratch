#|
Bryan Hinton
HW-15
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"


;**************************************************************
;Exercise 3.1.1
;**************************************************************

Expression 1
(let ((x 5) (y 6) (z 7)) 
(let ((x 13) (y (+ x y)) (z x))
(- (+ x z) y)))
 
(let ((x 13) (y (+ 13 6)) (z 13))
(- (+ x z) y))

(let ((x 13) (y 19) (z 13))
(- (+ x z) y))

(- (+ 13 13) 19)

(- 26 19)

The value is 7

Expression 2
(let ((x 5) (y 6) (z 7)) 
(+ (let ((z (+ x z))) (* z (+ z x)))
(let ((z (* x y))) (+ z (* z (- z y))))))

(+ (let ((z (+ 5 7))) (* z (+ z 5)))
(let ((z (* 5 6))) (+ z (* z (- z 6)))))

(+ (let ((z 12)) (* z (+ z 5)))
(let ((z (* 5 6))) (+ z (* z (- z 6)))))

(+ (* 12 (+ 12 5))
(let ((z (* 5 6))) (+ z (* z (- z 6)))))

(+ 204
(let ((z (* 5 6))) (+ z (* z (- z 6)))))

(+ 204
(let ((z 30)) (+ z (* z (- z 6)))))

(+ 204
(+ 30 (* 30 (- 30 6))))

(+ 204
(+ 30 (* 30 24)))

(+ 204
(+ 30 720))

(+ 204
750)

The value is 954
|#
;**************************************************************
;Exercise 3.1.3
;**************************************************************

(define subst (lambda (new old slst)
		(letrec ((subcdr (lambda (plst) (if (null? plst)
		'()
		(if (symbol? (car plst))
			(if (eq? (car plst) old)
				(cons new (subcdr (cdr plst)))
				(cons (car plst) (subcdr (cdr plst)))
				)
				(cons (subst new old (car plst))
				(subcdr (cdr plst)))))))) (subcdr slst))))

;test case
(subst 'x 'y '(y y y))
;value (x x x)