;; -*- lexical-binding: t; -*-

(TeX-add-style-hook
 "plateau_awale"
 (lambda ()
   (TeX-run-style-hooks
    "latex2e"
    "standalone"
    "standalone10"
    "tikz"))
 :latex)

