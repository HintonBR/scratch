#|
Bryan Hinton
HW-6
bhinton@cs.byu.edu
"I did this by myself without person or computer help!!!"

**************************************************************
Exercise 2.1.1

<list-of-numbers>
(<number) . <list-of-numbers>)
(-7 . <list-of-numbers>)
(-7 . (<number> . <list-of-numbers>))
(-7 . (3 . <list-of-numbers>))
(-7 . (3 . (<number> . <list-of-numbers>)))
(-7 . (3 . (14 . <list-of-numbers>)))
(-7 . (3 . (14 . ())))

**************************************************************
Exercise 2.1.2

New BNF for Datum grammer

<list> ::= () | (<list-of-datums>)
<list-of-datums> ::= <datum><list-of-datums> | <datum>
<vector> ::= #() | #(<list-of-datums>)
<dotted-datum> ::= (<datum><list-of-datums> . <datum>) | (<datum> . <datum>)
<datum> ::= <number> | <symbol> | <boolean> | <string> | <list> | <dotted-datum> | <vector>

Changes in derivation 
<list>
(<list-of-datums>)
(<datum><list-of-datums>)
(<datum><datum><list-of-datums>)
(<datum><datum><datum>)
(<boolean><datum><datum>)
(#t <datum><datum>)
(#t <dotted-datum><datum>)
(#t (<datum> . <datum>) <datum>)
(#t (<symbol> . <datum>) <datum>)
(#t (foo . <list>) <datum>)
(#t (foo . ()) <datum>)
(#t (foo . ()) <number>)
(#t (foo . ()) 3)

**************************************************************
Exercise 2.2.4
	The recursion involved is actually indirect recursion meaning the function
doesn't call itself, but instead calls another function that calls subst-symbol-expression.
It is that function call where the recursive call is made on a smaller substructure.
So while the indirect recursive call to subst uses the same size in a variable in subst
the actual recursive call to subst-symbol-expression is made using a smaller size parameter
	

|#