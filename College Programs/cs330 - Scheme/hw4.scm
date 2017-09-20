#|
Bryan Hinton
HW-4
bhinton@cs.byu.edu
"I am submitting my own work at not anyone elses"

________________________________________________
exercise 1.2.1
		My guess		Actual	
(car (cdr x)) =    b 			  b 

(caddr x) =        ((3) c)		  ((3) c)
	
(cdaddr x) =       c			  c	

(char? (car '(#\a #\b))) = #t		  #t

(cons 'x x) =      (x . x)  		  (x a b ((3) c) d)

The difference in this one was that I forgot what x was defined as a list which would make it equivalent to what Scheme returned

(cons (list 1 2) (cons 3 '(4))) = ((1 2) 3 4)  ((1 2) 3 4)

(cons (list) (list 1 (cons 2 '()))) = (() 1 (2))  (() 1 (2))	

___________________________________________________
exercise 1.2.2

x3          	((a b) a)		   ((a b) a)		

(eq? x3 (cons x1 x2)) = #f                    () or #f

(eq? (cdr x3) x2) = #t			      #t

(eq? (car x1) (car x2)) = #t		      #t

(cons (cons 'a 'b) (cons 'c '())) = ((a.b) c)	((a.b) c)

(cons 1 (cons 2 3))  = (1 (2 . 3))            (1 2 . 3)

My guess was incorrect because I forgot that the second argument to cons was supposed to be a list and thus would have been read and understood as such	

|#
