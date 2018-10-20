(begin
	(define point0 (lambda (x y) (list x y)))
	(define point1 (set-property "object-name" "point" point0))
	(define make-point (set-property "size" 0 point1))
)