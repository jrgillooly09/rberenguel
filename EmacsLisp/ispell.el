(setq ispell-dictionary-alist-meva
(quote (("catala" "[A-Z\300\301\310\311\314\315\322\323\331\332a-z\340\341\350\351\354\355\363\371\372]" "[^A-Z\300\301\310\311\314\315\322\323\331\332a-z\340\341\350\351\354\355\363\371\372]" "[-.]" nil ("-B" "-d" "ca") "~tex" iso-8859-1
))))

(setq ispell-dictionary-alist (append 	ispell-dictionary-alist
					ispell-dictionary-alist-meva))
(setq ispell-menu-map nil)
(load-library "ispell")