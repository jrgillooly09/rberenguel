(defvar SearchResults)

(defconst Google-result-markup '("<em>" "</em>" "<b>" "</b>" "</a>" "<strong>" "</strong>")) 

(defun trim-markup-Google-Bing (str)
  (let ((intermediate str))
    (dolist (item Google-result-markup)
      (setq intermediate (replace-regexp-in-string item "" intermediate)))
    intermediate
    ))



(defun get-only-url (str)
  (cadr (split-string str "\"")))

(defun get-url-buffer(url)
  (let ((results-buffer (url-retrieve-synchronously url)))
    (switch-to-buffer results-buffer)
    ))

(require 'url)



(defun parse-Google-buffer(url query)
  "Parse the Google results buffer for search results"
  (interactive "surl \nsquery:")
  (message "Query: %S" (replace-regexp-in-string " " "+" query))
  (get-url-buffer (format "http://www.google.com/search?hl=en&q=%S&num=100" (replace-regexp-in-string " " "+" query)))
  (goto-line 1)
  (let ((llista '()))
    (while (re-search-forward "class=\"r\">" nil t)

      (let ((start (point)))
	(re-search-forward "class=l>" nil t)
	(let ((firststring (buffer-substring
			    start (point))))
	  (setq start (point))
	  (re-search-forward "/a>")
	  (add-to-list 'llista (cons firststring (buffer-substring
						  start (point))))
      
	  )))
    (with-output-to-temp-buffer "*my output*"
      (dolist (item (reverse llista)) 
	(princ  (get-only-url (car item)))
	(princ "\n")
	(princ  (trim-markup-Google-Bing (cdr item)))
	(princ "\n")
	))
    (switch-to-buffer "*my output*")
    (let ((found
	   (re-search-forward url nil t)))
      (with-output-to-temp-buffer "*Results*"
	(insert-parse-date)
	(princ "\nSearch string: ")
	(princ query)
	(princ "\nTarget URL: ")
	(princ url)
	(princ "\n: ")
	(if found
	    (progn (princ "Result: ")
		   (princ (+ 1 (/ (- (line-number-at-pos) 1) 2))))
	  (princ "Result not found in 100 first results"))
	(kill-buffer "*my output*")
	)
      )))

(defun parse-Bing-buffer(url query)
  "Parse the Google results buffer for search results"
  (interactive "surl \nsquery:")
  (message "Query: %S" (replace-regexp-in-string " " "+" query))
  (with-output-to-temp-buffer "*my output*"
    (let ((llista '()))
      (dotimes (i 10)
	(get-url-buffer (format "http://www.bing.com/search?q=%S&mkt=en-US&first=%S&adlt=demote" (replace-regexp-in-string " " "%20" query) (+ (* i 10) 1)))
	(goto-line 1)
	
	(while (re-search-forward "class=\"sb_tlst\">" nil t)
	  (re-search-forward "href=\"" nil t) 
	  (let ((start (point)))
	    (re-search-forward "\"" nil t) 
	    (let ((firststring
		   (buffer-substring start (point))))
	      ;(message "first: %S" firststring)
	      (re-search-forward ">" nil t) 
	      (setq start (point))
	      (re-search-forward "/a>" nil t)
	      (add-to-list 'llista (cons firststring (buffer-substring
						      start (point))))
	      ))))
	(message "Asdf: %S" llista)
	

      (dolist (item (reverse llista)) 
	(princ (car item))
	(princ "\n")
	(princ  (trim-markup-Google-Bing (cdr item)))
	(princ "\n")
	)
      )
    )
					;    (switch-to-buffer "*my output*")
    
  (switch-to-buffer "*my output*")
  (let ((found
	 (re-search-forward url nil t)))
  (with-output-to-temp-buffer "*Results*"
    (insert-parse-date)
    (princ "\nSearch string: ")
    (princ query)
    (princ "\nTarget URL: ")
    (princ url)
    (princ "\n: ")
    (if found
	(progn (princ "Result: ")
	       (princ (+ 1 (/ (- (line-number-at-pos) 1) 2))))
      (princ "Result not found in 100 first results"))
    ;(kill-buffer "*my output*")
    )
  ))

;;     (with-output-to-temp-buffer "*my output*"
;;       (dolist (item (reverse llista)) 
;; 	(princ  (get-only-url (car item)))
;; 	(princ "\n")
;; 	(princ  (trim-markup-Google (cdr item)))
;; 	(princ "\n")
;; 	))


(defun insert-parse-date ()
  (let ((format "%Y%m%d@%H:%M")
	(system-time-locale ""))
    (princ (format-time-string format))))

