;;; LECTURE 24 INTERPRETER WITH IF, LET, PROC, SUPPORT FOR CELLS
;;; 09 MARCH 1999, SDK

(load "/usr/local/lib/mit-scheme/parser.scm")
(load "/usr/local/lib/mit-scheme/ff.scm")

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
    (write (eval-exp (parse (read)) init-env))
    (newline)
    (read-eval-print)))



