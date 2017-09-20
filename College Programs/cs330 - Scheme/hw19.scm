#|
Bryan Hinton
HW-19
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"


(load "/usr/local/lib/mit-scheme/records.scm")
;**************************************************************
;Exercise 4.5.2
;**************************************************************
a = (3) (1)
b = (2 3) (2 1)
c = (3)  (3 2 1)
x = (4 1 2 3) (4 1)
y = (5 2 3) (5 2 1)
z = (6 3) (6 3 2 1)

With x, y, and z the reason they were wrong was because I got
a, b, and c wrong.  I got those wrong because I misunderstood what affect 
reverse! had on the lists.
;**************************************************************
;Exercise 4.5.3
;**************************************************************
p is now from a box and pointer diagram point of view a pair whose first 
value is 1 and whose second value is a pointer to the first value.  We don't return the expression resulting from
set-cdr! because if it returned the muted pair then it would print out infinitely.  

|#