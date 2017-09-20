#|
Bryan Hinton
HW-6
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

**************************************************************
Exercise 1.3.6
|#

(define curry2 (lambda (x) (lambda (y) (lambda (z) (x y z)))))

(((curry2 +) 1) 2)
;Value: 3
(define consa ((curry2 cons) 'a))
(consa '(b))
;Value: (a b)

#|
**************************************************************
Exercise 1.3.6

The advantage of automatic currying is that you can start to build a function call as soon
as the variables become available - bind them and then throw them away because they are now
represented by the new curried function.  Thus variables passed as parameters could be 
immediately applied to the functions that they pertain to.  A disadvantage is now you have 
to keep track of what variable is holding what curried version of the function and it might
cause the program to be difficult to understand.


|#