#|
Bryan Hinton
HW-25
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#
;**************************************************************
;Exercise 5.1.4
;**************************************************************
(load "/usr/local/lib/mit-scheme/parser.scm")
(load "/usr/local/lib/mit-scheme/ff.scm")
;(load "c:\\parser.scm")
;(load "c:\\ff.scm")


;;;-----------------------------------------------------------
;;; SOME NEW RECORDS

(define-record lit (datum))
(define-record varref (var))
(define-record app (rator rands))
(define-record prim-proc (prim-op))
(define-record let (decls body))
(define-record decl (var exp))
(define-record  if (test-exp then-exp else-exp))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR THE ENVIRONMENTS

(define the-empty-env (create-empty-ff))
(define extend-env extend-ff*)
(define apply-env apply-ff)

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR EVAL-EXP

(define eval-exp
  (lambda (exp env)
    (variant-case exp
      (lit (datum) datum)
      (varref (var) (apply-env env var))
      (app (rator rands)
	   (let ((proc (eval-exp rator env))
		 (args (eval-rands rands env)))
	     (apply-proc proc args)))
      (if (test-exp then-exp else-exp)
	  (if (true-value? (eval-exp test-exp env))
	      (eval-exp then-exp env)
	      (eval-exp else-exp env)))
      (let (decls body)
        (let ((vars (map decl->var decls))
              (exps (map decl->exp decls)))
          (let ((new-env (extend-env vars
				     (eval-rands exps env) 
				     env)))
            (eval-exp body new-env))))
      (else (error "Invalid abstract syntax:" exp)))))

(define eval-rands
  (lambda (rands env)
    (map (eval-rand env) rands)))

(define eval-rand
  (lambda (env)
   (lambda (exp)
    (eval-exp exp env))))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR PROCEDURES

(define apply-proc
  (lambda (proc args)
    (variant-case proc
      (prim-proc (prim-op) (apply-prim-op prim-op args))
      (else (error "Invalid procedure:" proc)))))

(define apply-prim-op
  (lambda (prim-op args)
    (case prim-op
      ((+) (+ (car args) (cadr args)))
      ((-) (- (car args) (cadr args)))
      ((*) (* (car args) (cadr args)))
      ((add1) (+ (car args) 1))
      ((sub1) (- (car args) 1))
      ((cons) (cons (car args) (cadr args)))
      ((car) (caar args))
      ((cdr) (cdar args))
      ((list) args)			
      (else (error "Invalid prim-op name:" prim-op)))))

(define prim-op-names '(+ - * add1 sub1 cons car cdr list))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR INIT-ENV

(define init-env 
  (extend-env '(emptylist) (list '())
  (extend-env
    prim-op-names
    (map make-prim-proc prim-op-names)
    the-empty-env)))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR MISC AUXILLARY SERVICES

(define true-value?
  (lambda (x)
    (not (zero? x))))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR READ-EVAL-PRINT

(define parse 
  (lambda (s)
    (if (string? s) 
	(character-string-parser s)
	(error "You need to enter a string!!!" s))))

(define run
  (lambda (x)
    (eval-exp (parse x) init-env)))

(define read-eval-print
  (lambda ()
    (display "--> ")
    (write (eval-exp (parse (read)) init-env))
    (newline)
    (read-eval-print)))


;(read-eval-print)
;test case
(run "list(1, 2, 3)")
;value = (1, 2, 3)
(run "car(cons(4, emptylist))")
;value 4

;**************************************************************
;Exercise 5.2.2
;**************************************************************
(load "/usr/local/lib/mit-scheme/parser.scm")
(load "/usr/local/lib/mit-scheme/ff.scm")
;(load "c:\\parser.scm")
;(load "c:\\ff.scm")


;;;-----------------------------------------------------------
;;; SOME NEW RECORDS

(define-record lit (datum))
(define-record varref (var))
(define-record app (rator rands))
(define-record prim-proc (prim-op))
(define-record let (decls body))
(define-record decl (var exp))
(define-record  if (test-exp then-exp else-exp))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR THE ENVIRONMENTS

(define the-empty-env (create-empty-ff))
(define extend-env extend-ff*)
(define apply-env apply-ff)

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR EVAL-EXP

(define eval-exp
  (lambda (exp env)
    (variant-case exp
      (lit (datum) datum)
      (varref (var) (apply-env env var))
      (app (rator rands)
	   (let ((proc (eval-exp rator env))
		 (args (eval-rands rands env)))
	     (apply-proc proc args)))
      (if (test-exp then-exp else-exp)
	  (if (true-value? (eval-exp test-exp env))
	      (eval-exp then-exp env)
	      (eval-exp else-exp env)))
      (let (decls body)
        (let ((vars (map decl->var decls))
              (exps (map decl->exp decls)))
          (let ((new-env (extend-env vars
				     (eval-rands exps env) 
				     env)))
            (eval-exp body new-env))))
      (else (error "Invalid abstract syntax:" exp)))))

(define eval-rands
  (lambda (rands env)
    (map (eval-rand env) rands)))

(define eval-rand
  (lambda (env)
   (lambda (exp)
    (eval-exp exp env))))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR PROCEDURES

(define apply-proc
  (lambda (proc args)
    (variant-case proc
      (prim-proc (prim-op) (apply-prim-op prim-op args))
      (else (error "Invalid procedure:" proc)))))

(define apply-prim-op
  (lambda (prim-op args)
    (case prim-op
      ((+) (+ (car args) (cadr args)))
      ((-) (- (car args) (cadr args)))
      ((*) (* (car args) (cadr args)))
      ((add1) (+ (car args) 1))
      ((sub1) (- (car args) 1))
      ((cons) (cons (car args) (cadr args)))
      ((car) (caar args))
      ((cdr) (cdar args))
      ((list) args)			
      ((equal) (if (equal? (car args) (cadr args)) 1 0))
      ((zero) (if (zero? (car args)) 1 0))
      ((greater) (if (> (car args) (cadr args)) 1 0))
      ((less) (if (< (car args) (cadr args)) 1 0))	
      (else (error "Invalid prim-op name:" prim-op)))))

(define prim-op-names '(+ - * add1 sub1 cons car cdr list equal zero greater less))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR INIT-ENV

(define init-env 
  (extend-env '(emptylist) (list '())
  (extend-env
    prim-op-names
    (map make-prim-proc prim-op-names)
    the-empty-env)))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR MISC AUXILLARY SERVICES

(define true-value?
  (lambda (x)
    (not (zero? x))))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR READ-EVAL-PRINT

(define parse 
  (lambda (s)
    (if (string? s) 
	(character-string-parser s)
	(error "You need to enter a string!!!" s))))

(define run
  (lambda (x)
    (eval-exp (parse x) init-env)))

(define read-eval-print
  (lambda ()
    (display "--> ")
    (write (eval-exp (parse (read)) init-env))
    (newline)
    (read-eval-print)))

(run "equal(3, 3)")
;value is true
(run "zero(sub1(5))")
;value is false
(run "if greater(2, 3) then 5 else 6")
;value 6