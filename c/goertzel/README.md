# Goertzel
Scriptable tone detection and audio analysis

## Contents

* **goertzel.c**
  * Does all the dirty work using [Goertzel algorithm](http://en.wikipedia.org/wiki/Goertzel_algorithm)
  * Detect specified tones and print their levels
  * Filter tones using tresholds
     * Eg.: Print levels only when treshold is crossed (=event detection)
  * TODO
     * treshold hysteresis

* Sample scripts using goertzel.c
  * **dtmf.sh**: decodes DTMF signals in audio
  * **histogram.pl**: shows goertzel output in ASCII graphs
  * **histogram.sh**: example using histogram.pl to draw realtime equalizer
  * **guitar-tuner.sh**: example using histogram.pl to tune guitar (currently only to low E)
  * **sleepmon.sh**: Generates frequency on soundcard output and detects it on input
     * Can be used to detect and log motion using soundcard and PIR sensor or NC switch
  * **sleepplot.sh**: Generates gnuplot graph from sleeplog
  * TODO
     * Send me more...
