(call-process-shell-command  "~/Documents/Codi/C/1.x"  nil "*Fractals*" nil "-resolucio" "500" "-Julia" "-0.745" "0.11301" "-x" "-1.1" "-y" "1.1" "-w" "2.2" "-maxIter" "200" "-bailout" "8" "-epsilon" "0.01")


(defun Carregador (res ul w)  
  (let ((Resolucio (number-to-string res))
	(ulRe (number-to-string (car ul)))
	(ulIm (number-to-string (cadr ul)))
	(width (number-to-string w))
	
	)
    (call-process-shell-command  "~/Documents/Codi/C/1.x"  nil "*Fractals*" nil "-resolucio" Resolucio "-Julia" "-0.745" "0.11301" "-x" ulRe "-y" ulIm "-w" width "-maxIter" "500" "-bailout" "8" "-epsilon" "0.1"))
  (set-buffer "Fractals")
  (erase-buffer)
  (clear-image-cache)
  (insert-image (create-image "/Users/ruben/Bloc0i0.ppm" 'pbm nil :heuristic-mask t))
  )

(defun MainLoop (res ul w)
  (get-buffer-create "Fractals")
  (Carregador res ul w)
  (switch-to-buffer "Fractals")
  (dotimes (iter 10) 
    (let (
	  (event-mouse (read-event))
	  )
      (if (listp event-mouse)
	  (if (eq (car event-mouse) 'down-mouse-1)
	      (let* (
		     (P1 (event-start event-mouse))
		     (P2 (event-end (read-event)) ))
		
		(progn 
		  (setq ul (list 
			    (+ (* 1.0 (car (posn-object-x-y  P1)) (/ w res)) (car ul))
			    (- (cadr ul) (* 1.0 (cdr (posn-object-x-y P1)) (/ w res)))
			    ))
		  (setq w (* 1.0 
			     (abs (- (car (posn-object-x-y P2)) (car (posn-object-x-y P1)))) 
			     (/ w (* 1.0 res))))
	  

	    (Carregador res ul w))))))))

	      

(modify-frame-parameters (selected-frame)
                              '((menu-bar-lines . 5)))
(define-key global-map [menu-bar functions]
  (cons "Functions" (make-sparse-keymap "Functions")))

;; Aqui haurà d'anar una macro, que generi la llista a partir d'un
;; conjunt de paràmetres donat

(define-key global-map
  [menu-bar functions forward]
  '("z^2+c, DEM" . set-Quadratic-DEM))
