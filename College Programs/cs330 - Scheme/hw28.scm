#|
Bryan Hinton
HW-28
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#
;**************************************************************
;Exercise 5.4.4
;**********************************************************

(load "/usr/local/lib/mit-scheme/parser.scm")
(load "/usr/local/lib/mit-scheme/ff.scm")
;(load "\\bryan'~1\\classes\\cs330\\parser.scm")
;(load "\\bryan'~1\\classes\\cs330\\ff.scm")
;(require-library "trace.ss")
;;;-----------------------------------------------------------
;;; SOME NEW RECORDS

(define-record lit (datum))
(define-record varref (var))
(define-record app (rator rands))
(define-record if (test-exp then-exp else-exp))
(define-record let (decls body))
(define-record proc (formals body))
(define-record varassign (var exp))
(define-record begin (exp1 exp2))
(define-record prim-proc (prim-op))
(define-record closure (formals body env))
(define-record decl (var exp))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR CELLS  [SEE CELL ADT, PAGE 125]

(define cell-tag "cell")

(define make-cell
  (lambda (x)
    (vector cell-tag x)))

(define cell?
  (lambda (x)
    (and (vector? x)
         (= (vector-length x) 2)
         (eq? (vector-ref x 0) cell-tag))))

(define cell-ref
  (lambda (x)
    (if (cell? x)
        (vector-ref x 1)
        (error "Invalid argument to cell-ref:" x))))

(define cell-set!
  (lambda (x value)
    (if (cell? x) 
        (vector-set! x 1 value)
        (error "Invalid argument to cell-set!:" x))))

(define cell-swap!
  (lambda (cell-1 cell-2)
    (let ((temp (cell-ref cell-1)))
      (cell-set! cell-1 (cell-ref cell-2))
      (cell-set! cell-2 temp))))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR THE ENVIRONMENTS

(define the-empty-env (create-empty-ff))
(define extend-env extend-ff*)
(define apply-env apply-ff)

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR EVAL-EXP

(define syntax-expand 
  (lambda (exp env)
    (variant-case exp
      (lit (datum) exp)
      (varref (var) exp)
      (app (rator rands)
        (make-app (syntax-expand rator env) (syntax-expand-aux rands env)))
      (if (test-exp then-exp else-exp)
          (make-if (syntax-expand test-exp env) (syntax-expand then-exp env) (syntax-expand else-exp env)))
      (let (decls body)
        (make-app (make-proc (map decl->var decls) (syntax-expand body env)) (syntax-expand-aux (map decl->exp decls) env)))
      (proc (formals body) 
        (make-proc formals (syntax-expand body env)))	  
      (else (error "Invalid abstract syntax: " exp)))))

(define syntax-expand-aux
  (lambda (rands env) 
    (if (null? (cdr rands)) (list (syntax-expand (car rands) env))
        (append (list (syntax-expand (car rands) env)) (syntax-expand-aux (cdr rands) env)))))
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
      (proc (formals body) 
        (make-closure formals body env))	  
      (else (error "Invalid abstract syntax: " exp)))))


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
      (closure (formals body env)
        (eval-exp body 
          (extend-env
            formals
            args
            env)))
      (else (error "Invalid procedure:" proc)))))

(define apply-prim-op
  (lambda (prim-op args)
    (case prim-op
      ((+) (+ (car args) (cadr args)))
      ((-) (- (car args) (cadr args)))
      ((*) (* (car args) (cadr args)))
      ((add1) (+ (car args) 1))
      ((sub1) (- (car args) 1))
      (else (error "Invalid prim-op name:" prim-op)))))

(define prim-op-names '(+ - * add1 sub1))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR INIT-ENV

(define init-env 
  (extend-env
    prim-op-names
    (map make-prim-proc prim-op-names)
    the-empty-env))

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
    (write (eval-exp (syntax-expand (parse (read)) init-env) init-env))
    (newline)
    (read-eval-print)))

  
  