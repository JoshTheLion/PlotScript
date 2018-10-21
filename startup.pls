(begin
  (define make-point (lambda (x y) (begin
									  (define point{x,y} (list x y))
									  (define point{x,y} (set-property "object-name" "point" point{x,y}))
									  (define point{x,y} (set-property "size" 0 point{x,y}))
									)))
)