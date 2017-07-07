(do 
	(def b (* 5 5))
	(def foo (fn (a) (+ a b)))
	(def bar (fn () (foo 4)))
	(bar))