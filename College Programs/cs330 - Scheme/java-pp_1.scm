#|
   java-pp.scm : Unparser for java abstract syntax records
   November 12, 1999
   tavis.bennett@usa.net
|#

(load "/usr/local/lib/mit-scheme/records.scm")

(define-record jdecl (modifiers decl-type))
(define-record var (type name))
(define-record method (head body))
(define-record m-head (type name params))
(define-record class (head body))
(define-record c-head (name base))
(define-record assign (l-val r-val))
(define-record compound (name field))

(define space-out
  (lambda (output)
    (display output)
    (display #\Space)))

(define tab-over
  (lambda (tabs)
    (if (zero? tabs) 0
      (begin
        (display #\Tab)
        (tab-over (- tabs 1))))))

(define insert-commas
  (lambda (var-list)
    (if (null? var-list)
      'done
      (begin
        (java-pp (car var-list))
        (if (= (length var-list) 1) 0
          (begin
            (display ", ")
            (insert-commas (cdr var-list))))))))

(define java-pp
  (let ((tabs 0))
    (lambda (java-tree)
      (variant-case java-tree
        (jdecl (modifiers decl-type)
          (tab-over tabs)
          (for-each space-out modifiers)
          (set! tabs (+ tabs 1))
          (java-pp decl-type)
          (set! tabs (- tabs 1))
          (if (var? decl-type)
            (display ";") 0)
          (newline))
        (var (type name)
          (display type)
          (display #\Space)
          (display name))
        (method (head body)
          (java-pp head)
          (display " {")
          (newline)
          (for-each java-pp body)
          (tab-over (- tabs 1))
          (display "}"))
        (m-head (type name params)
          (if (null? type) 0
            (space-out type))
          (display name)
          (display "(")
          (insert-commas params)
          (display ")"))
        (class (head body)
          (java-pp head)
          (display " {")
          (newline)
          (for-each java-pp body)
          (tab-over (- tabs 1))
          (display "}"))
        (c-head (name base)
          (display "class ")
          (display name)
          (if (null? base) 0
            (begin
              (display " extends ")
              (display base))))
        (assign (l-val r-val)
          (tab-over tabs)
          (java-pp l-val)
          (display " = ")
          (java-pp r-val)
          (display ";")
          (newline))
        (compound (name field)
          (display name)
          (display ".")
          (display field))
        (else (display java-tree))))))
