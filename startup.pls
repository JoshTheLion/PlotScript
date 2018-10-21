(begin
  (define make-point (lambda (x y) (begin
									  (define point{x,y} (list x y))
									  (define point{x,y} (set-property "object-name" "point" point{x,y}))
									  (define point{x,y} (set-property "size" 0 point{x,y}))
									)))
  
  (define make-line (lambda (p1 p2)(begin
									  (define line{p1,p2} (list p1 p2))
									  (define line{p1,p2} (set-property "object-name" "line" line{p1,p2}))
									  (define line{p1,p2} (set-property "thickness" 1 line{p1,p2}))
									)))
)