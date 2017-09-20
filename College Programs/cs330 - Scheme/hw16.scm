#|
Bryan Hinton
HW-16
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#
(load "/usr/local/lib/mit-scheme/records.scm")
#|
;**************************************************************
;Exercise 3.4.3
;**************************************************************
Syntatic Derivation
<exp>
(<exp> <exp>)
((lambda (<var>) <exp>) <exp>)
((lambda (a) <exp>) <exp>)
((lambda (a) (<exp> <exp>)) <exp>)
((lambda (a) (<exp> <exp>)) <varref>)
((lambda (a) (<exp> <exp>)) c)
((lambda (a) (<varref> <exp>)) c)
((lambda (a) (a <exp>)) c)
((lambda (a) (a <varref>)) c)
((lambda (a) (a b)) c)


Abstract Syntax Tree

       [app]
      |     |
rator|       | rand
  [lambda]  [varref]
   |     |        |
formal    |body    |var 
  |     [app]      c
  a     |   | 
 rator |     | rand
   [varref]  [varref]
    |         |
var|      var |
   a          b 


|#
;**************************************************************
;Exercise 3.4.1
;**************************************************************


(define-record interior (symbol left-tree right-tree))
(define-record leaf (number))

(define leaf-sum (lambda (tree)
   (cond
     ((leaf? tree) (leaf->number tree))
     ((interior? tree) (+ (leaf-sum (interior->left-tree tree)) (leaf-sum (interior->right-tree tree))))
     (else (error "Thanks for playing - Try again"))
   )
))

;test cases
(define tree1 (make-interior '4 (make-leaf 4) (make-leaf 4)))
(leaf-sum tree1)
;value = 8
