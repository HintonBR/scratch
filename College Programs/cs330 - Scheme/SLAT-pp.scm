; SLAT-pp assumes you already have records.scm loaded.  If it isn't,
; then uncomment this load statement:
; (load "/usr/local/lib/mit-scheme/records.scm")

; SLAT-pp also assumes that you have named your record-types decl and prod,
; like in the code given to us for the parser.

; If you want to pretty-print only one decl record, run SLAT-pp-aux on it.
; If you want to pretty-print a whole list of decl records, run SLAT-pp on it.

(define SLAT-pp
  (lambda (lst)
    (if (null? lst)
	'()
	(begin
	  (SLAT-pp-aux (car lst))
	  (SLAT-pp (cdr lst))))))

(define SLAT-pp-aux
  (lambda (record)
    (if (null? record)
	'()
	(if (list? record)
	    (begin
	      (SLAT-pp-aux (car record))
	      (if (null? (cdr record))
		  '()
		  (display "   | " ))
	      (SLAT-pp-aux (cdr record))
	      )
	    (if (prod? record)
		(begin
		  (display (prod->name record))
		  (display " ")
		  (display (prod->fields record))
		  (newline))
		(begin
		  (newline)
		  (display (decl->name record))
		  (display " ::= ")
		  (SLAT-pp-aux (decl->prods record))
		  ))))))










