#|
Bryan Hinton
HW-14
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"


;**************************************************************
;Exercise 2.3.4
;**************************************************************

((lambda (z) 3) (lambda (y) x))

;**************************************************************
;Exercise 2.3.11
;**************************************************************
The outer a is shadowed by the inner a and so the reference to a in the inner function 
should contain the lexical addres a : 0 0 instead of a : 1 0.

|#