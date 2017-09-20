



(load "/u1/ugrad/bhinton/cs330/SLAT-pp.scm")

(load "/usr/local/lib/mit-scheme/records.scm")

(load "/u1/ugrad/bhinton/cs330/token.scm")
;(load "token.scm")

(define-record decl (name prods))

(define-record prod (name fields))

(define parse-decl (lambda ()

    (let ((tname (token->data (next))))

         (advance)

         (eat 'define-sym)

         (make-decl tname (parse-prods)))))



(define parse-prods (lambda ()

    (let ((prodrec (parse-prod)))

         (if (eq? (token->class (next)) 'vert)

             (begin

	       (eat 'vert)

	       (cons prodrec (parse-prods)))

             (list prodrec)))))



(define parse-prod (lambda ()

                     (let ((name (token->data (next)))) (advance) (eat 'lparen) (make-prod name (parse-fields)))))

(define parse-fields (lambda ()

	(if (equal? (token->class (next)) 'rparen)

	(begin (advance) '())

	(let ((name (token->data (next)))) (advance) (cons name (parse-fields)))

        )))



