
(setq DoF (/ 1 3)) 
; Depth of field. Leave it as 1/3
(setq Eye 6.35)
; Eye separation in centimetres
(setq E (* Eye (/ 72 2.54)))
; Eye separation in pixels (with a factor conversion of 72 Dpi, if
; your screen/print has another resolution, change this parameter
(setq HSR T)


(defun Separation (3dMat i j)
; Given a z-map, i.e. a grayscale image with white as nearest and
; black as farthest, this function (which, just in case, averages the
; red, green and blue components to yield a mean color value, and then
; divides by the maximal color number) returns the separation of the
; "left eye" and "right eye" pixels in the random dot stereogram.
; These two points should have the same color, thus when properly
; viewed will yield the deepness feeling.

  (let ((element (/ (/ (+ 
			(svref (aref 3dMat i j) 0) 
			(svref (aref 3dMat i j) 1) 
			(svref (aref 3dMat i j) 2)) 
		       3.0)
		     255.0)))
    (DistanceFormula element)))

(defun DistanceFormula (element)
; Given a matrix element, between 0 and 1, returns the separation

  (round (/ (* (- 1.0 (* DoF element)) E) (- 2.0 (* DoF element)))))

(defun Shifting (Same left right k)
; This function will change the "same vector" to fit the depth image

  (if (and (not (eq left k)) (not (eq k right)))
      (if (< k right)
	  (Shifting Same k right (aref Same k))
	  (Shifting Same right k (aref Same right))))
  (values left right))

(defun Z (3dMat x y)
  (/ (/ (+ 
	 (svref (aref 3dMat x y) 0) 
	 (svref (aref 3dMat x y) 1) 
	 (svref (aref 3dMat x y) 2)) 
	3.0)
     255.0))

(defun visible-p (3dMat x y tee)
  (let* (
	 (element (Z 3dMat x y))
	 (elementpt (Z 3dMat (+ x tee) y))
	 (elementmt (Z 3dMat (- x tee) y))
	 (zt (+ element (* 2.0 (/ (* (- 2.0 (* DoF element) tee)) (* DoF E)))))
	 (visible (and (< elementmt zt) (< elementpt zt)))
	 )
    (if (and (< zt 1) visible)
	(visible-p 3dMat x y (+ 1 tee))
	visible)))

(defun Stereo (3dMat res texture)
; Given the grayscale image in 3dMat, this function generates the
; random dot steoreogram. Texture is a list (matrix textwidth
; textheight)

  (let ((Same (make-array (* res 100)))
	(SIRDS (make-array (list (* res 100) (* res 100))))
	)

    ; Same is a vector which will contain the points which should have
    ; the same colour. SIRDS will contain the final image

    (dotimes (y (* res 100)) 
      (dotimes (x (* res 100))
	(setf (aref Same x) x))
      ; We start by setting it at the identity

      (dotimes (x (* res 100))
	(let* ((separation (Separation 3dMat x y))
	       (left  (round (- x (/ separation 2.0))))
	       (right (round (+ left separation))))
	  ; Calculate the separation of the corresponding depth point,
	  ; as a left eye-right eye pixels.
	  
	  (if (and (>= left 0) (< right (* res 100)))
	      ; If the left-right pixels are in the image, check for
	      ; hidden surfaces. Then change the same vector to
	      ; adequately map to the random dot stereogram
	      (if (or (visible-p 3dMat x y 1) HSR)
		  (multiple-value-bind (left right) 
		      (Shifting Same left right (aref Same left))
		    (setf (aref Same left) right))
		  nil
		  ))))

     (if (not texture) 
	 ; If we don't have a texture field, use a random dot generator
	 (let ((Colors (make-array (* res 100)))
	       (x 0)
	       )
	   (dotimes (sss (* res 100))
	     (setq x (- (- (* res 100) 1) sss))
	     (if (eq (aref Same x) x)
		 (progn
		   (setf (aref Colors x) (vector 
					  (random 255.0)
					  (random 255.0)
					  (random 255.0))))
		 (progn
		   (setf (aref Colors x) (aref Colors (aref Same x)))
		   ))
	     (setf (aref SIRDS x y)  (aref Colors x))))

	 ; If we are using the texture, colours should be taken from the texture
	 (let ((Colors (make-array (* res 100))) 
	       (x 0))
	   (dotimes (sss (* res 100))
	     (setq x (- (- (* res 100) 1) sss))
	     (if (eq (aref Same x) x)
		 (setf (aref Colors x)  
		        (aref (car texture) 
				     (mod x (second texture)) 
				     (mod y (third texture))))
		 (setf (aref Colors x) (aref Colors (aref Same x))))
	     (setf (aref SIRDS x y)  (aref Colors x))
	     ))))
    
    (marking SIRDS res)
    SIRDS
    ))
	

(defun marking (SIRDS res)
  (dotimes (i 3)
    (dotimes (j 3)
    (setf (aref SIRDS (round (- (/ (* 100 res) 2.0) (+ i (/ (distanceformula 0) 2.0)))) (- (* 100 res) (+ 1 j))) (vector 0 0 0))
    (setf (aref SIRDS (round (+ (/ (* 100 res) 2.0) (+ i (/ (distanceformula 0) 2.0)))) (- (* 100 res) (+ 1 j))) (vector 0 0 0)))))
    
; (list (GetImage 50 500 "texture.ppm") 50 500)

(defun Main (res)
  (tracer (Stereo (GetImage (* 100 res) (* 100 res) "Entrada.ppm") res 
		  nil
		  ) 
	  (make-pathname :name "SIRDS.ppm") res))
;

(defun tracer (matriu pathname res)
  (with-open-file (p pathname :direction :output :if-exists :supersede)
    (format p "P3 ~A ~A~% 255~%" (* res 100) (* res 100))
    (dotimes (j (* res 100)) 
					; For every column
      (dotimes (i (* res 100))
	(let ((color (aref matriu i j)))
	  (format p "~3d ~3d ~3d~%" 
		  (round (float (svref color 0))) 
		  (round (float (svref color 1)))
		  (round (float (svref color 2))))
	  )
	))))

(defun GetImage (width height filename)
  ; 
  (with-open-file (p (make-pathname :name filename) :direction :input)
  (let ((imatge (make-array (list width height)))
	)
    (read p) ; P3
    (format t "Resolution ~A x ~A~%" (read p) (read p)) ; Res
    (format t "Maxcolor ~A~%" (read p)) ; Maxcolor
    (dotimes (j height)
          (dotimes (i width)
	    (setf (aref imatge i j) (vector (read p) (read p) (read p)))))
    
  imatge
  )))