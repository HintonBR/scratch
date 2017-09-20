#|
Bryan Hinton
HW-23
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"
|#
;**************************************************************
;Exercise 5.1.3
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

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR THE ENVIRONMENTS

(define the-empty-env (create-empty-ff))
(define extend-env extend-ff*)
(define apply-env apply-ff)

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR EVAL-EXP

(define eval-exp
  (lambda (exp)
    (variant-case exp
      (lit (datum) datum)
      (varref (var) (apply-env init-env var))
      (app (rator rands)
        (let ((proc (eval-exp rator))
              (args (eval-rands rands)))
          (apply-proc proc args)))
      (else (error "Invalid abstract syntax: " exp)))))

(define eval-rands
  (lambda (rands)
    (map eval-exp rands)))

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
      ((minus) (- (car args) (* 2 (car args))))
      (else (error "Invalid prim-op name:" prim-op)))))

(define prim-op-names '(+ - * add1 sub1 minus))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR INIT-ENV

(define init-env 
  (extend-env
   prim-op-names
   (map make-prim-proc prim-op-names)
   the-empty-env))

;;;-----------------------------------------------------------
;;; FUNCTIONS FOR READ-EVAL-PRINT

(define parse 
  (lambda (s)
    (if (string? s) 
	(character-string-parser s)
	(error "You need to enter a string!!!" s))))

(define run
  (lambda (x)
    (eval-exp (parse x))))

(define read-eval-print
  (lambda ()
    (display "--> ")
    (write (eval-exp (parse (read))))
    (newline)
    (read-eval-print)))

;(read-eval-print)
;test case
(eval-exp (parse "minus(+(minus(5), 9))"))
;value = 4

#| The reason why we do not have to modify the parser is because it doesn't care that
we had to extend the init-env to allow the symbol minus to be bound to something.  The parser
simply parsed minus like any other app it came across and then when we go to apply-proc is
where the actual look-up occurs as to what is the value of minus and as long as it has been
previously defined in the environment then the interpreter will work just fine
|#