#lang racket

(define (ohodnot znak)
  "vrati +1 pro otevrenou zavorku, -1 pro zavrenou a 0 pro ostatni znaky"
  (if (char=? znak #\()
      1
      (if (char=? znak #\))
          -1
          0
      )
  )
)

(define (uvozovky znak faktor)
  "vrati faktor zavisly na tom zda jsme nebo nejsme uvnitr apostrofu, k tomu vyuziva predchozi stav"
  (if (char=? znak #\')
      (modulo (+ faktor 1) 2)
      faktor
  )
)



(define (zavorky text [debug #f] [soucet 0] [faktor 1])
  "vypise soucet nezavrenych zavorek v retezci, zaporna cisla udavaji zavorky zavrene navic. ignoruje znaky v apostrofech. lze zapnout debug mod."
  (if debug
      (begin
        (display soucet)
        (display "x")
        (display faktor)
        (display ":")
        (display text)
        (display "\n")
      )
      0
  )
  (if (non-empty-string? text)
      (zavorky (substring text 1) debug (+ soucet (* (ohodnot (string-ref text 0)) faktor)) (uvozovky (string-ref text 0) faktor))
      soucet
  )
)

;Ukazka pouziti:

(zavorky "pokus (')))')((")

;Ukazka vcetne debug modu

;(zavorky "pokus (')))')((" #t)
