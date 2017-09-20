#|
Bryan Hinton
HW-22
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

;**************************************************************
;Exercise 4.2.1
;**************************************************************
1. ((lambda (x) (x (y x))) z)
	Answer = (z (y z))	
	Substitution using application and then variable references

2. ((lambda (x) (x y)) 
	(lambda (y) (x y)))
	Answer = (x y)
	
3. ((lambda (x) 
	(lambda (y) ((x y) z))) 
		(lambda (a) y))

	Answer = (lambda (w) (((lambda (a) y) y) z))
	On this one we had to rename the formal parameter y and it's associated value in the
	body because y occurs free in the parameter.  After this alpha reduction we were able to
	use two beta reductions to get to the final answer

4. ((lambda (x) 
	(lambda (y) 
		((lambda (x) (z x)) 
			(lambda (y) (z y))))) 
				(lambda (y) y))

		Answer = (lambda (y) ((lambda (x) (z x)) (lambda (y) (z y))))
		Because x doesn't occur free in the body - no substitutions are necessary
		and we return the expression.  From what Dr. Morse said in class if you can't
		reduce the outer lambda expression anymore than you are done even though we 
		could go inside the body of the lambda expression and simplify that a
		little bit more.
|#