#|
Bryan Hinton
HW-3
bhinton@cs.byu.edu
"I am submitting my own work at not anyone elses"

________________________________________________
Part 1

"http://students.cs.byu.edu/~bhinton/"
___________________________________________________
Part 2

(define cube (lambda (n) (* (* n n) n)))

(define new-sin (lambda (n d) (if (equal? d 'degrees) (sin (* (/ n 180) 3.14)) (if (equal? d 'radians) (sin n) "Error invalid type"))))

|#