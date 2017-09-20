
(define-record empty-ff ())
(define-record extended-ff (sym val ff))

(define create-empty-ff make-empty-ff)
(define extend-ff make-extended-ff)
(define-record extended-ff* (sym-list val-vector ff))

(define extend-ff*
  (lambda (sym-list val-list ff)
    (make-extended-ff* sym-list (list->vector val-list) ff)))

(define ribassoc (lambda (s los v fail-value)
   (define ribassoc-aux (lambda (los vl)
       (if (null? los) 
	   fail-value
	   (if (eq? s (car los))
	       (car vl)
	       (ribassoc-aux (cdr los) (cdr vl))))))
   (ribassoc-aux los (vector->list v))))

(define apply-ff
  (lambda (ff symbol)
    (variant-case ff
      (empty-ff ()
        (error "empty-ff: no association for symbol" symbol))
      (extended-ff (sym val ff)
        (if (eq? symbol sym) 
            val 
            (apply-ff ff symbol)))
      (extended-ff* (sym-list val-vector ff)
        (let ((val (ribassoc symbol sym-list val-vector '*fail*)))
          (if (eq? val '*fail*)
              (apply-ff ff symbol)
              val)))
      (else (error "apply-ff: Invalid finite function" ff)))))


(define defined-ff?
  (lambda (symbol ff)
    (variant-case ff
      (empty-ff () #f)
      (extended-ff (sym val ff)
        (if (eq? symbol sym)
            #t
            (defined-ff? symbol ff)))
      (extended-ff* (sym-list val-vector ff)
        (let ((val (ribassoc symbol sym-list val-vector '*fail*)))
          (if (eq? val '*fail*)
              (defined-ff? symbol ff)
              #t)))
      (else (error "defined-ff?: Invalid finite function" ff)))))
