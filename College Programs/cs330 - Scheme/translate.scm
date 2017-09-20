(load "parse.scm")
(load "java-pp.scm")


(define translate (lambda (x) 	(variant-case x
			(decl (name prods) (cons
				(make-jdecl '(public abstract) (make-class (make-c-head (symbol->string name) '()) '()))
				(translate-prod-list prods name))))))

(define translate-prod-list (lambda (y base)       
       	(if (null? y) '()
       	(cons (translate-prod (car y) base) (translate-prod-list (cdr y) base))
	)))

(define translate-prod (lambda (y base) (variant-case y
		(prod (name fields)		
			(make-jdecl '() (make-class (make-c-head (symbol->string name) base) 
				(translate-field-list fields fields name 1)))))))

(define translate-field-list (lambda (lst rlst mname counter) 
	(if (null? lst) 
		(list (translate-method rlst mname))
		(cons (translate-field (car lst) counter) (translate-field-list (cdr lst) rlst mname (+ counter 1)))
	)
))

(define translate-field (lambda (field count)
	(make-jdecl '(public) (make-var (symbol->string field) (string-append "field" (number->string count))))))

(define translate-method (lambda (flst mname) 
	(make-jdecl '(public) (make-method (make-m-head '() (symbol->string mname) (translate-param-list flst 1)) (translate-assign-list flst 1)))))

(define translate-param-list (lambda (lst count) 
	(if (null? lst) '()
		(cons (make-var (symbol->string (car lst)) (string-append "param" (number->string count))) (translate-param-list (cdr lst) (+ count 1)))	
	)
))

(define translate-assign-list (lambda (lst count) 
	(if (null? lst) '()
		(cons (make-assign (string-append "this.field" (number->string count)) (string-append "param" (number->string count))) (translate-assign-list (cdr lst) (+ count 1)))	
	)
))

(define pretty-print (lambda (lst) 
                       (if (null? (cdr lst))
                        (java-pp (car lst))
                        (begin
                         (java-pp (car lst))
                         (pretty-print (cdr lst))))))
                         
(define testcase "tree ::= leaf (integer) | node (String tree tree)")
(tokenize testcase)

(pretty-print (translate (parse-decl))) 
;(trace translate-prod-list)
;(trace translate-param-list)
;(trace translate-assign-list)

