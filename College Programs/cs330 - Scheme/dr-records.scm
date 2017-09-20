
;;; MzScheme macros implementing records for the book:
;;;
;;; "Essentials of Programming Languages", Daniel P. Friedman,
;;;   Mitchell Wand and Christopher T. Haynes, MIT Press, 1992.
;;;
;;; Code modified January 15, 1998 by Matthias Felleisen
;;; 1) every? replaced by andmap to avoid namespace problems
;;;    andmap is a standard Scheme routine
;;; 2) define-record simplified with h/o function and in-lining
;;; 3) variant-case simplified  by in-lining and renaming
;;; 4) added test code (see end of file)
;;; 
;;; (variant-case code Based on code of David McCusker, Copyrighted in 1993)
;;; Code created October 21, 1997 by Dan Friedman.

;;; For behavioral specification see tests at end of file. 

(define-macro define-record 
  (lambda (rec-name rec-fields)
    (let ((translate
	    (lambda (token)
	      (lambda (rec-name f)
		(string->symbol
		  (string-append
		    (symbol->string rec-name) token (symbol->string f)))))))
      `(begin
	 ,@(append
	     (list (list 'define-struct rec-name rec-fields))
	     (map (lambda (f)
		    (list 'define
		      ((translate "->") rec-name f)
		      ((translate "-")  rec-name f)))
  	          rec-fields)
	     (list (list 'quote rec-name)))))))

(define-macro variant-case
  (lambda (record-exp . clauses)
    (let*
      (;; -- silly abbreviations
       (sym string->symbol)
       (str symbol->string)
       (cat string-append)
       ;; -- real stuff
       (exp (gensym))
       (make-clause
	 (lambda (c)
	   (let* ((name (str (car c)))
		  (n-f (lambda (f) (list f (list (sym (cat name "-" (str f))) exp)))))
	     (if (eq? 'else (car c))
		 c
		 (list (list (sym (cat name "?")) exp)
		       (cons 'let (cons (map n-f (cadr c)) (cddr c)))))))))
      (for-each
	(lambda (c)
	  (unless (and (pair? c)
		    (or (eq? 'else (car c))
			(and (symbol? (car c))
			  (pair? (cdr c))
			  (list? (cadr c))
			  (andmap symbol? (cadr c)))))
	    (error "variant-case: expected (name fields* ...); given: ~s" c)))
	clauses)
      `(let ((,exp ,record-exp))
	 (cond
	   ,@(map make-clause clauses))))))

