#|
Bryan Hinton
HW-5
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

**************************************************************
Exercise 1.3.2

(define cell-tag "cell")

(define make-cell (lambda (x) (vector cell-tag x)))

(define cell? (lambda (x) (if (vector? x) (if (* (vector-length x) 2) (eq? (vector-ref x 0) cell-tag) #f) #f)))

(define cell-ref (lambda (x) (if (cell? x) (vector-ref x 1) (error "Invalid argument to cell-ref:" x))))

(define c (make-cell 5))
c
;Value 13: #("cell" 5)

(cell? c)
;Value: #t

(cell-ref c)
;Value: 5

**************************************************************
Exercise 1.3.3

In Visual Basic it is not possible to use functions as parameters to other functions nor is it possible
in anyway to store a function in a data type.  They cannot be represented as anonymous simply because
they must always be pre declared and the BNF requires that they have a name for the function or method to
exist and compile as valid syntax.  Hence Visual Basic functions and methods are not first-class

C++ and Java are similar as far as answers go.  Neither one allows functions directly to be passed
as parameters or stored as direct data types, but both C++ and Java allow functions to be passed through objects
and C++ also allows access to functions through function pointers, nevertheless neither of these two languages represent
functions as first-class

|#