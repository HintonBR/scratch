#|
Bryan Hinton
HW-18
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

|#
(load "/usr/local/lib/mit-scheme/records.scm")
;**************************************************************
;Exercise 3.4.7
;**************************************************************

(define make-interior
(lambda (sym lt rt) (vector 'interior sym lt rt))
)
(define interior?
(lambda (tree) (and (equal? (vector-length tree) 4) (equal? (vector-ref tree 0) 'interior)))
)
(define interior->symbol
(lambda (tree) (if (interior? tree) (vector-ref tree 1) (error "Invalid record")))
)
(define interior->left-tree
(lambda (tree) (if (interior? tree) (vector-ref tree 2) (error "Invalid record")))
)
(define interior->right-tree
	(lambda (tree) (if (interior? tree) (vector-ref tree 3) (error "Invalid record")))
)


;**************************************************************
;Exercise 3.5.1
;**************************************************************


(define make-interior2
(lambda (sym lt rt) (list 'interior sym lt rt))
)
(define interior2?
(lambda (tree) (and (equal? (length tree) 4) (equal? (car tree) 'interior)))
)
(define interior2->symbol
(lambda (tree) (if (interior2? tree) (cadr tree) (error "Invalid record")))
)
(define interior2->left-tree
(lambda (tree) (if (interior2? tree) (caddr tree) (error "Invalid record")))
)
(define interior2->right-tree
	(lambda (tree) (if (interior2? tree) (cadddr tree) (error "Invalid record")))
)


(define make-leaf2
(lambda (number) (list 'leaf number))
)
(define leaf2?
(lambda (tree) (and (equal? (length tree) 2) (equal? (car tree) 'leaf)))
)
(define leaf2->number
(lambda (tree) (if (leaf2? tree) (cadr tree) (error "Invalid record")))
)