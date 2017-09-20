


;(load "/usr/local/lib/mit-scheme/records.scm")
(load "\\bryan'~1\\classes\\cs330\\dr-records.scm")


;;; Beginning of figures for Appendix B

;;; Figure B.1 : page 465

(define-record define (var exp))
(define-record varref (var))
(define-record lit (datum))
(define-record app (rator rands))
(define-record if (test-exp then-exp else-exp))
(define-record let (decls body))
(define-record decl (var exp))
(define-record proc (formals body))
(define-record varassign (var exp))
(define-record letmutable (decls body))
(define-record begin (exp1 exp2))
(define-record letrecproc (procdecls body))
(define-record procdecl (var formals body))
(define-record letrec (decls body))
(define-record dynassign (var exp body))
(define-record letdynamic (decls body))

;;; Figure B.2 : page 466

(define-record definearray (var dim-exp)) 
(define-record letarray (arraydecls body))
(define-record arrayref (array index))
(define-record arrayassign (array index exp))
(define-record letproc (procdecls body))
(define-record local (decls body))
(define-record keydecl (var exp))

;;; Figure B.2 : page 466 %%%%% This is for 3rd printing %%%%%%

(define-record definearray (var lex-exp))
(define-record letarray (arraydecls body))
(define-record arrayref (array index))
(define-record arrayassign (array index exp))
(define-record letproc (procdecls body))
(define-record local (decls body))
(define-record keydecl (var exp))

;;; Figure B.3 : page 466

(define-record super-meth-app (name rands))
(define-record meth-app (name rands))
(define-record i-varref (var))
(define-record c-varref (var))
(define-record i-varassign (var exp))
(define-record c-varassign (var exp))
(define-record method (formals body))
(define-record new-simpleinst (class-exp)) ; corrected for second printing
(define-record new-simpleclass (c-vars i-vars methdecls init-exp))
(define-record new-class (parent-exp c-vars i-vars methdecls init-exp))
(define-record new-instance (class-exp parent-exp i-vars methdecls))

;;; Figure B.4 : page 466

(define-record abort (exp))
(define-record letcont (var body))
(define-record callcc (exp))
(define-record coroutine (exp))
(define-record wind (pre body post))

;;; Figure B.5 : page 466

(define-record sum (rands))

;;; End of figures for Appendix B



;;; Beginning of figures for Appendix D

;;; Anonymous code : page 471

(define error
  (lambda args
    (for-each (lambda (x) (display x) (display " ")) args)
    (newline)
    (reset)))

;;; Figure D.1 : page 472, 473, 474

(define check/drop
  (lambda (class next-action)
    (lambda (buffer token-seq)
      (let ((token (token-seq-head token-seq)))
        (if (eq? (token->class token) class)
            (next-action buffer (token-seq-tail token-seq))
            (error "Syntax error: expecting a" class "not a" token))))))

(define check/shift
  (lambda (class next-action)
    (lambda (buffer token-seq)
      (let ((token (token-seq-head token-seq)))
        (if (eq? (token->class token) class)
            (next-action (cons (token->data token) buffer)
                           (token-seq-tail token-seq))
            (error "Syntax error: expecting a" class "not a" token))))))

(define goto-parser-state
  (lambda (state)
    (lambda (buffer token-seq)
      (let ((next-action (state (token-seq-head token-seq))))
        (next-action buffer token-seq)))))

(define reduce
  (lambda (prod-name)
    (lambda (buffer token-seq)
      (make-parser-answer
        (apply (get-constructor-from-name prod-name) (reverse buffer))
        token-seq))))

(define process-nt
  (lambda (state next-action)
    (lambda (buffer token-seq)
      (let ((next-answer ((goto-parser-state state) '() token-seq)))
        (next-action 
          (cons (parser-answer->tree next-answer) buffer)
          (parser-answer->unparsed next-answer))))))

(define emit-list
  (lambda ()
    (lambda (buffer token-seq)
      (make-parser-answer (reverse buffer) token-seq))))

(define-record parser-answer (tree unparsed))
(define token-seq-head car)
(define token-seq-tail cdr)

(define character-string-parser
  (lambda (a-string)
    (parse-token-seq parse-form
      (character-string-scanner a-string))))

(define parse-token-seq
  (lambda (start-state token-seq)
    (let ((answer (parse-once start-state token-seq)))
      (variant-case answer
        (parser-answer (tree unparsed)
          (if (eq? (token->class (token-seq-head unparsed)) 'end-marker)
              tree
              (error "Tokens left over:" unparsed)))))))

(define parse-once
  (lambda (start-state token-seq)
    ((goto-parser-state start-state) '() token-seq)))

(define parse-loop*
  (lambda (terminator separator action)
    (letrec
      ((more (lambda (token)
               (let ((class (token->class token)))
                 (cond
                   ((eq? class terminator) (emit-list))
                   ((eq? separator class)
                    (check/drop separator
                      (action (goto-parser-state more))))
                   (else (error "Invalid separator token:" token)))))))
      (lambda (token)
        (let ((class (token->class token)))
          (if (eq? class terminator)
              (emit-list)
              (action (goto-parser-state more))))))))

(define parse-loop+
  (lambda (terminator separator action)
    (letrec
      ((more (lambda (token)
               (let ((class (token->class token)))
                 (cond
                   ((eq? class terminator) (emit-list))
                   ((eq? separator class)
                    (check/drop separator
                      (action (goto-parser-state more))))
                   (else (error "Invalid separator token:" token)))))))
      (lambda (token)
        (action (goto-parser-state more))))))

;;; Figure D.2 : page 474, 475

(define parse-form
  (lambda (token)
    ((case (token->class token)
       ((define) seen-define)
       ((definearray) seen-definearray)
       (else parse-exp))
     token)))

(define parse-exp
  (lambda (token)
    ((case (token->class token)
      ((number) seen-number)
      ((variable) seen-variable)
      ((lparen) seen-lparen)
      ((if) seen-if)
      ((let) seen-let)
      ((proc) seen-proc)
      ((begin) seen-begin)
      ((letmutable) seen-letmutable)
      ((letrecproc) seen-letrecproc)
      ((letrec) seen-letrec)
      ((letdynamic) seen-letdynamic)
      ((letarray) seen-letarray)
      ((letproc) seen-letproc)
      ((local) seen-local)
      ((dollar-sign) seen-dollar-sign)
      ((ampersand) seen-ampersand)
      ((double-ampersand) seen-double-ampersand)
      ((method) seen-method)
      ((simpleclass) seen-simpleclass)      
      ((simpleinstance) seen-simpleinstance)
      ((class) seen-class) 
      ((instance) seen-instance)
      ((abort) seen-abort)
      ((letcont) seen-letcont)
      ((callcc) seen-callcc)
      ((coroutine) seen-coroutine)
      ((wind) seen-wind)
      ((sum) seen-sum)
      (else (error "Invalid parse-exp token:" token)))
     token)))

(define parse/var
  (lambda (token)
    (case (token->class token)
      ((lparen) (seen-var&lparen token))
      ((assign-sym) (seen-var&assign-sym token))
      ((lbracket) (seen-var&lbracket token))
      (else (reduce 'varref)))))

;;; Figure D.3 : page 476, 477

(define get-constructor-from-name
  (lambda (prod-name)
    (case prod-name
      ((chain) (lambda (x) x))
      ((lit) make-lit)      
      ((app) make-app)
      ((app/var) (lambda (var rands) (make-app (make-varref var) rands)))
      ((begin) (letrec ((loop (lambda (exps)
                                (if (null? (cdr exps))
                                    (car exps)
                                    (make-begin (car exps) (loop (cdr exps)))))))
                 loop))
      ((define) make-define)
      ((varref) make-varref)
      ((if) make-if)
      ((proc) make-proc)
      ((let) make-let)
      ((letmutable) make-letmutable)
      ((letrecproc) make-letrecproc)
      ((letrec) make-letrec)
      ((decl) make-decl)
      ((procdecl) make-procdecl)
      ((varassign) make-varassign)
      ((dynassign) make-dynassign)
      ((letdynamic) make-letdynamic)
      ((definearray) make-definearray)      
      ((arrayref) make-arrayref)      
      ((arrayref/var) (lambda (var index)
                        (make-arrayref (make-varref var) index)))
      ((arrayassign) make-arrayassign)
      ((arrayassign/var) (lambda (var index val)
                           (make-arrayassign (make-varref var) index val)))
      ((letarray) make-letarray)
      ((letproc) make-letproc)
      ((local) make-local)
      ((stuff-and-keydecls) append)      
      ((keydecl) make-keydecl)
      ((i-var) make-i-varref)
      ((c-var) make-c-varref)
      ((method) make-method)
      ((letcont) make-letcont)
      ((i-varassign) make-i-varassign)
      ((c-varassign) make-c-varassign)
      ((new-simpleinst) make-new-simpleinst)
      ((new-simpleclass) make-new-simpleclass)
      ((new-class) make-new-class)
      ((instance) make-new-instance)
      ((meth-app) (lambda (var rands)
                    (if (null? rands)
                        (error "Invalid method rands:" rands)
                        (let ((rand (car rands)))
                          (if (and (varref? rand)
                                   (eq? (varref->var rand) 'super))
                              (make-super-meth-app var (cdr rands))
                              (make-meth-app var rands))))))
      ((abort) make-abort)
      ((letcont) make-letcont)
      ((callcc) make-callcc)
      ((coroutine) make-coroutine)
      ((wind) make-wind)
      ((sum) make-sum)
      (else (error "Bad production name:" prod-name)))))

;;; Figure D.4 : page 478, 479, 480, 481

(define seen-define
  (lambda (token)
    (check/drop 'define
      (check/shift 'variable
        (check/drop 'eqsign
          (process-nt parse-exp (reduce 'define)))))))

(define seen-number
  (lambda (token)
    (check/shift 'number (reduce 'lit))))

(define seen-variable
  (lambda (token)
    (check/shift 'variable (goto-parser-state parse/var))))

(define seen-lparen
  (lambda (token)
    (check/drop 'lparen
      (process-nt parse-exp
        (check/drop 'rparen
          (goto-parser-state parse-proc))))))

(define seen-if
  (lambda (token)
    (check/drop 'if
      (process-nt parse-exp
        (check/drop 'then
          (process-nt parse-exp
            (check/drop 'else
              (process-nt parse-exp (reduce 'if)))))))))

(define seen-proc
  (lambda (token)
    (check/drop 'proc
      (check/drop 'lparen
        (process-nt parse-varlist
          (check/drop 'rparen
            (process-nt parse-exp (reduce 'proc))))))))

(define seen-begin
  (lambda (token)
    (check/drop 'begin
      (process-nt parse-compound
        (check/drop 'end (reduce 'begin))))))

(define seen-let
  (lambda (token)
    (check/drop 'let
      (process-nt parse-decls
        (check/drop 'in (process-nt parse-exp (reduce 'let)))))))

(define seen-letmutable
  (lambda (token)
    (check/drop 'letmutable
      (process-nt parse-decls
        (check/drop 'in
          (process-nt parse-exp (reduce 'letmutable)))))))

(define seen-letrec
  (lambda (token)
    (check/drop 'letrec
      (process-nt parse-decls
        (check/drop 'in (process-nt parse-exp (reduce 'letrec)))))))

(define seen-letrecproc
  (lambda (token)
    (check/drop 'letrecproc
      (process-nt parse-procdecls
        (check/drop 'in (process-nt parse-exp (reduce 'letrecproc)))))))

(define seen-letdynamic
  (lambda (token)
    (check/drop 'letdynamic
      (process-nt parse-decls
        (check/drop 'in (process-nt parse-exp (reduce 'letdynamic)))))))

(define seen-var&lparen
  (lambda (token)
    (check/drop 'lparen
      (process-nt parse-rands
        (check/drop 'rparen (reduce 'app/var))))))

(define seen-var&assign-sym
  (lambda (token)
    (check/drop 'assign-sym
      (process-nt parse-exp
        (goto-parser-state parse-assign-or-dynassign)))))

(define parse-proc
  (lambda (token)
    (case (token->class token)
      ((lparen)
       (check/drop 'lparen
         (process-nt parse-rands (check/drop 'rparen (reduce 'app)))))
      (else (reduce 'chain)))))

(define parse-assign-or-dynassign
  (lambda (token)
    (case (token->class token)
      ((during)
       (check/drop 'during (process-nt parse-exp (reduce 'dynassign))))
      (else (reduce 'varassign)))))

(define parse-varlist
  (parse-loop* 'rparen 'comma
    (lambda (action) (check/shift 'variable action))))

(define parse-compound   ; corrected for second printing
  (parse-loop* 'end 'semicolon
    (lambda (action) (process-nt parse-exp action))))

(define parse-rands
  (parse-loop* 'rparen 'comma
    (lambda (action)
      (process-nt parse-exp action))))

(define parse-decls
  (parse-loop+ 'in 'semicolon
    (lambda (action) (process-nt parse-decl action))))

(define parse-procdecls
  (parse-loop+ 'in 'semicolon
    (lambda (action) (process-nt parse-procdecl action))))

(define parse-decl
  (lambda (token)
    (case (token->class token)
      ((variable)
       (check/shift 'variable
         (check/drop 'eqsign
           (process-nt parse-exp (reduce 'decl)))))
      (else (error "Invalid parse-decl token:" token)))))

(define parse-procdecl
  (lambda (token)
    (case (token->class token)
      ((variable)
       (check/shift 'variable
         (check/drop 'lparen
           (process-nt parse-varlist
             (check/drop 'rparen
               (check/drop 'eqsign
                 (process-nt parse-exp (reduce 'procdecl))))))))
      (else (error "Invalid parse-procdecl token:" token)))))

;;; Figure D.5 : page 482, 483

(define parse-proc
  (lambda (token)
    (case (token->class token)
      ((lparen)
       (check/drop 'lparen
         (process-nt parse-rands (check/drop 'rparen (reduce 'app)))))
      ((lbracket)
       (check/drop 'lbracket
 	 (process-nt parse-exp
 	   (check/drop 'rbracket
 	     (goto-parser-state parse-assign-or-dynassign)))))
      (else (reduce 'chain)))))

(define seen-var&lbracket
  (lambda (token)
    (check/drop 'lbracket
      (process-nt parse-exp
        (check/drop 'rbracket
          (goto-parser-state
            parse-arrayassign-or-arrayref/var))))))

(define seen-definearray
  (lambda (token)
    (check/drop 'definearray
      (check/shift 'variable
        (check/drop 'lbracket
          (process-nt parse-exp
            (check/drop 'rbracket (reduce 'definearray))))))))

(define seen-letarray
  (lambda (token)
    (check/drop 'letarray
      (process-nt parse-arraydecls
        (check/drop 'in (process-nt parse-exp (reduce 'letarray)))))))

(define seen-letproc
  (lambda (token)
    (check/drop 'letproc
      (process-nt parse-procdecls
        (check/drop 'in (process-nt parse-exp (reduce 'letproc)))))))

(define seen-local
  (lambda (token)
    (check/drop 'local
      (process-nt parse-decls
        (check/drop 'in (process-nt parse-exp (reduce 'local)))))))

(define parse-arrayassign-or-arrayref
  (lambda (token)
    (case (token->class token)
      ((assign-sym)
       (check/drop 'assign-sym
         (process-nt parse-exp (reduce 'arrayassign))))
      (else (reduce 'arrayref)))))

(define parse-arrayassign-or-arrayref/var
  (lambda (token)
    (case (token->class token)
      ((assign-sym)
       (check/drop 'assign-sym
         (process-nt parse-exp (reduce 'arrayassign/var))))
      (else (reduce 'arrayref/var)))))

(define parse-arraydecls
  (parse-loop+ 'in 'semicolon
    (lambda (action) (process-nt parse-arraydecl action))))

(define parse-arraydecl
  (lambda (token)
    (case (token->class token)
      ((variable)
       (check/shift 'variable
         (check/drop 'lbracket
           (process-nt parse-exp
             (check/drop 'rbracket (reduce 'decl))))))
      (else (error "Invalid parse-arraydecl token:" token)))))

;;; Figure D.6 : page 483, 484, 485

(define parse-vars-until-optdecls
  (parse-loop* 'rparen 'comma
    (lambda (action)
      (goto-parser-state
        (lambda (token)
          (case (token->class token)
            ((colon) (emit-list))
            ((variable) (check/shift 'variable action))
            (else (error "Invalid parse-vars-until-optdecls:" token))))))))

(define parse-optdecls
  (parse-loop* 'rparen 'comma
    (lambda (action) (process-nt parse-optdecl action))))

(define parse-optdecl-var
  (lambda (token)
    (case (token->class token)
      ((variable)
       (check/shift 'variable
         (check/drop 'eqsign
           (process-nt parse-exp (reduce 'keydecl)))))
      (else (error "Invalid parse-optdecl-var token:" token)))))

(define parse-optdecl
  (lambda (token)
    (case (token->class token)
      ((colon) (check/drop 'colon
                 (goto-parser-state parse-optdecl-var)))
      (else (error "Invalid parse-optdecl token:" token)))))

(define parse-exps-until-keydecls
  (parse-loop* 'rparen 'comma
    (lambda (action)
      (goto-parser-state
        (lambda (token)
          (case (token->class token)
            ((colon) (emit-list))
            (else (process-nt parse-exp action))))))))

(define parse-keydecls
  (parse-loop* 'rparen 'comma
    (lambda (action) (process-nt parse-keydecl action))))

(define parse-keydecl
  (lambda (token)
    (case (token->class token)
      ((colon)
       (check/drop 'colon
         (check/shift 'variable
           (check/drop 'eqsign
             (process-nt parse-exp (reduce 'keydecl))))))
      (else (error "Invalid parse-opts token:" token)))))

(define parse-varlist
  (lambda (token)
    (process-nt parse-vars-until-optdecls
      (process-nt parse-optdecls (reduce 'stuff-and-keydecls)))))

(define parse-rands
  (lambda (token)
    (process-nt parse-exps-until-keydecls
      (process-nt parse-keydecls (reduce 'stuff-and-keydecls)))))

;;; Figure D.7 : page 485, 486, 487

(define seen-dollar-sign
  (lambda (token)
    (check/drop 'dollar-sign
      (check/shift 'variable
        (check/drop 'lparen
          (process-nt parse-rands
            (check/drop 'rparen (reduce 'meth-app))))))))

(define seen-ampersand
  (lambda (token)
    (check/drop 'ampersand
      (check/shift 'variable (goto-parser-state parse/ivar)))))

(define seen-double-ampersand
  (lambda (token)
    (check/drop 'double-ampersand
      (check/shift 'variable (goto-parser-state parse/cvar)))))

(define seen-method
  (lambda (token)
    (check/drop 'method
      (check/drop 'lparen
        (process-nt parse-varlist
          (check/drop 'rparen
            (process-nt parse-exp (reduce 'method))))))))

(define seen-simpleinstance
  (lambda (token)
    (check/drop 'simpleinstance
      (process-nt parse-exp (reduce 'new-simpleinst)))))

(define seen-simpleclass
  (lambda (token)
    (check/drop 'simpleclass
      (check/drop 'lparen
        (process-nt parse-varlist
          (check/drop 'rparen
            (check/drop 'lparen
              (process-nt parse-varlist
                (check/drop 'rparen
                  (check/drop 'lparen
                    (process-nt parse-methdecls
                      (check/drop 'rparen
                        (process-nt parse-exp
                          (reduce 'new-simpleclass))))))))))))))

(define seen-class
  (lambda (token)
    (check/drop 'class
      (process-nt parse-exp
        (check/drop 'comma
          (check/drop 'lparen
            (process-nt parse-varlist
              (check/drop 'rparen
                (check/drop 'lparen
                  (process-nt parse-varlist
                    (check/drop 'rparen
                      (check/drop 'lparen
                        (process-nt parse-methdecls
                          (check/drop 'rparen
                            (process-nt parse-exp
                              (reduce 'new-class))))))))))))))))

(define seen-instance
  (lambda (token)
    (check/drop 'instance
      (process-nt parse-exp
        (check/drop 'comma
          (process-nt parse-exp
            (check/drop 'comma
              (check/drop 'lparen
                (process-nt parse-varlist
                  (check/drop 'rparen
                    (check/drop 'lparen
                      (process-nt parse-methdecls
                        (check/drop 'rparen (reduce 'instance))))))))))))))

(define parse/ivar
  (lambda (token)
    (case (token->class token)
      ((assign-sym)
       (check/drop 'assign-sym
         (process-nt parse-exp (reduce 'i-varassign))))
      (else (reduce 'i-var)))))

(define parse/cvar
  (lambda (token)
    (case (token->class token)
      ((assign-sym)
       (check/drop 'assign-sym
         (process-nt parse-exp (reduce 'c-varassign))))
      (else (reduce 'c-var)))))

(define parse-methdecls
  (parse-loop* 'rparen 'semicolon
    (lambda (action) (process-nt parse-decl action))))

;;; Figure D.8 : page 488

(define seen-abort
  (lambda (token)
    (check/drop 'abort
      (process-nt parse-exp (reduce 'abort)))))

(define seen-letcont
  (lambda (token)
    (check/drop 'letcont
      (check/shift 'variable
        (check/drop 'in (process-nt parse-exp (reduce 'letcont)))))))

(define seen-callcc
  (lambda (token)
    (check/drop 'callcc
      (process-nt parse-exp (reduce 'callcc)))))

(define seen-coroutine
  (lambda (token)
    (check/drop 'coroutine
      (process-nt parse-exp (reduce 'coroutine)))))

(define seen-wind
  (lambda (token)
    (check/drop 'wind
      (process-nt parse-exp
        (check/drop 'within
          (process-nt parse-exp
            (check/drop 'unwind 
              (process-nt parse-exp (reduce 'wind)))))))))

;;; Figure D.9 : page 488

(define seen-sum
  (lambda (token)
    (check/drop 'sum
      (check/drop 'lparen
        (process-nt parse-rands (check/drop 'rparen (reduce 'sum)))))))

;;; End of figures for Appendix D


;;; Beginning of figures for Appendix E

;;; Figure E.1 : page 489, 490

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
    (make-token 'number (string->number (list->string buffer)))))

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
      ((char-alphabetic? c)
       (shift (goto-scanner-state scanner-identifier-state)))
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
      (else (error "Invalid character to scan:" c)))))

(define scanner-identifier-state
  (lambda (c)
    (cond
      ((char-alphabetic? c)
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

(define scanner-comment-state
  (lambda (c)
    (cond
      ((char=? c #\newline)
       (drop (goto-scanner-state scanner-start-state)))
      (else (drop (goto-scanner-state scanner-comment-state))))))

(define scanner-assign-state
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
  '(if then else let in proc begin end letmutable letrecproc letrec
     during letdynamic letarray letproc local method define definearray
     simpleclass simpleinstance class instance abort letcont callcc
     coroutine wind unwind within sum))

;;; End of figures for Appendix E
