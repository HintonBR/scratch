#|
Bryan Hinton
HW-20
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

;**************************************************************
;Exercise 4.6.3
;**************************************************************
(eq? (next-symbol) 'g1) = #t
(eq? (next-symbol) 'g1) = #f
(eq? (next-symbol) (next-symbol)) = #f

When the function is initially defined c is initialized to 0 and bound to the function
next-symbol and so on every function call c is incremented instead of being reset to 0.
That is why in the first function call - c is still 0 on the call so the function returns g1 while the
next function call brings g2 and the next succesive calls bring g3 and g4 which are not equal.  

From an environmental model point of view when the let expression is used inside of the lambda
function an extended environment is set up which is referenced by the closure that stores the lambda 
values and this extended environment holds the value of c as it changes over subsequent invocations of
the functions.  So in that sense you could say that c is a static variable whose scope belongs to the 
function next-symbol.
|#