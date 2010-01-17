;;  Copyright 2009 Rubén Berenguel

;;  ruben /at/ maia /dot/ ub /dot/ es

;;  This program is free software: you can redistribute it and/or
;;  modify it under the terms of the GNU General Public License as
;;  published by the Free Software Foundation, either version 3 of the
;;  License, or (at your option) any later version.

;;  This program is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
;;  General Public License for more details.

;;  You should have received a copy of the GNU General Public License
;;  along with this program. If not, see
;;  <http://www.gnu.org/licenses/>.

;;  This program is called (LavaursChords N), where N is a period
;;  depth, ranging from 2 to any positive number. Due to optimization
;;  issues, do not go deeper than 12 or 13, as it would take a really
;;  long time.


(defvar PERIODS
  "A global variable, needed to avoid messiness in the code.")

(setq PERIODS '())

(defun LavaursChords (depth)
  "To generate the image, call (LavaursChords N) Do not go further
  than 10, 11 as it would take quite a while to draw, this program is
  not optimized."
  (CircleTrace (GenerateListOfCircles depth)))

(defun GenerateListOfCircles (Depth)
  "This generates the final list of chords. "
  (Arcs Depth 1 100 '(((0 0) 0))))


(defun Arc (R alpha balpha)
  "Find the ortogonal circle to the main circle, given the angles in
  it. R: radius of the main circle alpha, balpha angles of the first
  and second point (a, b) first point (ba, bb) second point"
  (let* (
	 (gamma (+ alpha (/ (- balpha alpha) 2)))
	 (ca (cos alpha))
	 (cg (cos gamma))
	 (sa (sin alpha))
	 (sg (sin gamma))
	 (denom (+ (* ca cg) (* sa sg)))
	 (a (* R ca))
	 (b (* R sa))
	 (x (/ (* R cg) denom))
	 (y (/ (* R sg) denom))
	 (xma (- x a))
	 (ymb (- y b))
	 )
    (list (list x y)	
	  (sqrt (+ (* xma xma) (* ymb ymb))))))
	
(defun DrawArc (alpha balpha R)
  "Generate the postscript arcs"
  (format t "newpath ~A ~A moveto 300 300 ~A ~A ~A arct" 
	  (+ 300 (* 100 (cos balpha))) 
	  (+ 300 (* 100 (sin balpha)))
	  (+ 300 (* 100 (cos alpha))) 
	  (+ 300 (* 100 (sin alpha)))
	  R))
       
(defun PeriodK (k)
  "Returns the list of period k (and below), rational points"
  (PeriodKAux k (FullList k)))

(defun PeriodKAux (k Previous)
  "Auxiliary function to remove duplicates in PeriodK"
  (if (= k 0)
      Previous
      (PeriodKAux (- k 1) (RemDouble Previous (FullList (- k 1))))))


(defun FullList (k)
  "Returns a list of angles of period K. This does the dirty work for
  PeriodK, creating all points. Then PeriodKAux trims the resulting
  list off duplicates"
  (let ((Periods '()))
    (do ((n 1 (+ n 1))) ((>= n (- (expt 2 k) 1)) Periods)
      
	  (setq Periods (append Periods (list (/ n (- (expt 2 k) 1)))))

	  )))
  
(defun RemDouble (list1 list2)
  "This function removes duplicates in the way I wanted. List1 is the
  longer one."
  (if (>(length list2) 0)
      (RemDouble (remove (car List2) List1) (cdr List2))
      list1
      ))



(defun SomeArc (R List1 List2 PreviousArcs )
  "Decide whether we can draw a new arc or not. Recurse"
  (if (and List1 List2)
      (if (CanDraw (car List1) (car List2) R PreviousArcs)
	  (let ((ArcXY (list (Arc R (* 2
			    pi (car List1)) (* 2 pi (car List2))) 
			     )))
	    (delete (car List1) List1)
	    (delete (car List2) List1)
	    (delete (car List2) List2)
	    ArcXY)
	    (SomeArc R List1 (cdr List2) PreviousArcs))
      NIL
      ))

(defun GetArcs (R List1 List2 PreviousArcs)
  "List1 contains (PM PM+1... PN) List2 contains the second element,
  i.e. we try to get an arc from (car List1) to (car List2) if we can,
  recurse with the trimmed lists List1' List2' if we cannot, recurse
  with the trimmed lists List1 List2'"
  (if List2
      (append (SomeArc R List1 List2 PreviousArcs) 
	      (GetArcs R (cdr List1) (cdr List2) PreviousArcs ))))

      

(defun Arcs (MaxPeriod CurrentPeriod R Previous)
  "R is the radius of the main circle MaxPeriod is the maximum period
  CurrentPeriod is the current period Previous contains the previous
  arcs as center, radius"
  (if (< CurrentPeriod MaxPeriod)
      (progn 
	(setq PERIODS (PeriodK CurrentPeriod))
      (Arcs MaxPeriod (+ 1 CurrentPeriod) R 
	    (append Previous 
		    (GetArcs R  PERIODS (cdr PERIODS) Previous))))
      
  Previous))

(defun CanDraw (X Y R Previous)
  "The most important function. Decide whether we can draw an arc (it
   not intersects previous arcs) or not"
  (if Y
      (let* (
	     (X2 (* 2 pi X))
	     (Y2 (* 2 pi Y))
	     (ArcXY (Arc R X2 Y2)))
	(andF (mapcar #'(lambda (A) 
			  (and (or (> (dist (car A) (car ArcXY)) (+ (cadr
							    A) (cadr
							    ArcXY)))
			  (> (abs (- (cadr A) (cadr ArcXY))) 
			     (dist (car A) (car ArcXY)))
			  )(> (cadr ArcXY) 0.00000001)
			  
			  ) ) Previous))) ))

(defun andF (List1)
  "A recursive 'and' for a list of T, NILs"
  (if (not List1)
      t
      (and (car List1) (andF (cdr List1)))))

 (defun dist (L1 L2)
   "Standard euclidean distance for two lists of 3 elements"
   (let ((M1 (- (car L1) (car L2)))
	 (M2 (- (cadr L1) (cadr L2))))
   (sqrt (+ (* M1 M1) (* M2 M2)))))


(defun Remaining (element elist)
  (if (not elist) nil 
      (if (= element (car elist)) (cdr elist)
	  (Remaining element (cdr elist)))))

(defun CircleTraceColor (ListOfCircles)
  "A random color assigner for the list of circles to be drawn. Just
   for the artistic effect."
  (with-open-file (PostScriptFile "LavaursColor.eps" :direction :output
		       :if-exists :supersede) 
    
    (format PostScriptFile " 
%!PS-Adobe-3.0 EPSF-3.0 
%%BoundingBox: 198 198 402 402 
gsave newpath

newpath
    198 198 moveto
    198 402 lineto
    402 402 lineto
    402 198 lineto
    198 198 lineto
closepath
0.08235 0.0352 0.17842 setrgbcolor
fill

 300.00000000 300.00000000 100.00000000 0
360 arc gsave 0 setlinewidth stroke grestore clip~%")
    (format PostScriptFile "newpath ~%")
    (dolist (Circle ListOfCircles)
      (format PostScriptFile " ~,8F ~,8F ~,8F 0 360 arc  0 setlinewidth
  ~A ~A ~A setrgbcolor stroke ~%"
	      (+ 300 (caar Circle))
	      (+ 300 (cadar Circle))
	      (cadr Circle)
	      (random 1.0)
	      (random 1.0)
	      (random 1.0)))
    (format PostScriptfile "~% showpage")))
  
(defun CircleTrace (ListOfCircles)
  "A postscript tracer for the list of circles"
  (with-open-file (PostScriptFile "Lavaurs.eps" :direction :output 
				  :if-exists :supersede) 
    (format PostScriptFile " 
%!PS-Adobe-3.0 EPSF-3.0 
%%BoundingBox: 198 198 402 402 

 300.00000000 300.00000000 100.00000000 0
360 arc gsave 0 setlinewidth stroke grestore clip~%")
    (format PostScriptFile "newpath ~%")
    (dolist (Circle ListOfCircles)
      (format PostScriptFile " ~,8F ~,8F ~,8F 0 360 arc  0 setlinewidth
   stroke ~%"
	      (+ 300 (caar Circle))
	      (+ 300 (cadar Circle))
	      (cadr Circle)))
    (format PostScriptfile "~% showpage")))


  
