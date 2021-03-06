
 (load "/usr/local/lib/mit-scheme/records.scm")
;(load "dr-records.scm")



(define shift
  (lambda (next-action)
    (lambda (buffer char-seq)
      (next-action (cons (char-seq-head char-seq) buffer)
        (char-seq-tail char-seq)))))

(define drop
  (lambda (next-action)
    (lambda (buffer char-seq)
      (next-action buffer (char-seq-tail char-seq)))))

(define goto-scanner-state
  (lambda (state)
    (lambda (buffer char-seq)
      ((state (char-seq-head char-seq)) buffer char-seq))))

(define emit
  (lambda (cooker)
    (lambda (buffer char-seq)
      (make-scanner-answer (cooker (reverse buffer)) char-seq))))

(define-record token (class data))

(define-record scanner-answer (token unscanned))

(define char-seq-head car)

(define char-seq-tail cdr)

(define cook-punctuation
  (lambda (class)
    (lambda (buffer) (make-token class '*))))

(define cook-number
  (lambda (buffer)
    (make-token 'number 
                (list->string buffer))))
; (string->number (list->string buffer)))))

(define cook-string
   (lambda (buffer)
     (make-token 'string
		 (list->string buffer))))



(define cook-identifier
  (lambda (buffer)
    (let ((symbol (string->symbol (list->string buffer))))
      (if (memq symbol keywords-list)
          (make-token symbol '*)
          (make-token 'variable symbol)))))

(define scan-once
  (lambda (start-state char-seq)
    ((goto-scanner-state start-state) '() char-seq)))

(define scan-char-seq
  (lambda (start-state char-seq)
    (let ((next-answer (scan-once start-state char-seq)))
      (variant-case next-answer
        (scanner-answer (token unscanned)
          (make-token-seq token
            (lambda ()
              (if (eq? (token->class token) 'end-marker)
                  '()
                  (scan-char-seq start-state unscanned)))))))))

(define character-string-scanner
  (lambda (a-string)
    (scan-char-seq scanner-start-state
      (string->list (string-append a-string (string #\^))))))

(define make-token-seq
  (lambda (token thunk)
    (cons token (thunk))))

;;; Figure E.2 : page 491, 492


(define scanner-start-state
  (lambda (c)
    (cond
      ((char-whitespace? c)
       (drop (goto-scanner-state scanner-start-state)))
;      ((char-alphabetic? c)
;       (shift (goto-scanner-state scanner-identifier-state)))
      ((char=? c #\')
       (drop (goto-scanner-state scanner-string-state)))
      ((char-numeric? c)
       (shift (goto-scanner-state scanner-number-state)))
      ((char=? c #\%)
       (drop (goto-scanner-state scanner-comment-state)))
      ((char=? c #\()
       (drop (emit (cook-punctuation 'lparen))))
      ((char=? c #\))
       (drop (emit (cook-punctuation 'rparen))))
      ((char=? c #\[)
       (drop (emit (cook-punctuation 'lbracket))))
      ((char=? c #\])
       (drop (emit (cook-punctuation 'rbracket))))
      ((char=? c #\,)
       (drop (emit (cook-punctuation 'comma))))
      ((char=? c #\=)
       (drop (emit (cook-punctuation 'eqsign))))
      ((char=? c #\;)
       (drop (emit (cook-punctuation 'semicolon))))
      ((char=? c #\|)
       (drop (emit (cook-punctuation 'vert))))
      ((char=? c #\&)
       (drop (goto-scanner-state scanner-ampersand-state)))
      ((char=? c #\$)
       (drop (emit (cook-punctuation 'dollar-sign))))
      ((char=? c #\:)
       (drop (goto-scanner-state scanner-assign-state)))
      ((or (char=? c #\+) (char=? c #\-) (char=? c #\*))
       (shift (emit cook-identifier)))
      ((char=? c #\^)
       (emit (cook-punctuation 'end-marker)))
      (else (shift (goto-scanner-state scanner-identifier-state))))))

;      (else (error "Invalid character to scan:" c)))))

(define scanner-identifier-state
  (lambda (c)
    (cond
      ((or (char-alphabetic? c) (char=? c #\_))
       (shift (goto-scanner-state scanner-identifier-state)))
      ((char-numeric? c)
       (shift (goto-scanner-state scanner-identifier-state)))
      (else (emit cook-identifier)))))

(define scanner-number-state
  (lambda (c)
    (cond
      ((char-numeric? c)
       (shift (goto-scanner-state scanner-number-state)))
      (else (emit cook-number)))))

(define scanner-string-state
  (lambda (c)
    (cond
      ((char=? c #\')
       (drop (emit cook-string)))
      (else (shift (goto-scanner-state scanner-string-state))))))

(define scanner-comment-state
  (lambda (c)
    (cond
      ((char=? c #\newline)
       (drop (goto-scanner-state scanner-start-state)))
      (else (drop (goto-scanner-state scanner-comment-state))))))

(define scanner-assign-state
  (lambda (c)
    (cond
      ((char=? c #\:)
         (drop (drop (emit (cook-punctuation 'define-sym)))))
      ((char=? c #\=)
         (drop (emit (cook-punctuation 'assign-sym))))
      (else (emit (cook-punctuation 'colon))))))

(define scanner-define-state
  (lambda (c)
    (cond
      ((char=? c #\=)
       (drop (emit (cook-punctuation 'assign-sym))))
      (else (emit (cook-punctuation 'colon))))))

(define scanner-ampersand-state
  (lambda (c)
    (cond
      ((char=? c #\&)
       (drop (emit (cook-punctuation 'double-ampersand))))
      (else (emit (cook-punctuation 'ampersand))))))

(define keywords-list
  '(prog funcs decls func))




;;; End of figures for Appendix E

(define token-list '())

(define tokenize (lambda (string)
    (set! token-list (character-string-scanner string))
    'done))


(define advance (lambda () 
    (set! token-list (cdr token-list))
    'done))

(define next (lambda ()
    (car token-list)))

(define eat (lambda (class)
    (if (eq? (token->class (next)) class)
        (advance)
	(begin
          (display "Expected: ")
          (write  class)
          (display " found: ")
          (write (token->class (next)))
          (newline)
          (error "eat: no match")))))


