# Vizionix
Experimental VJ software for GNU/Linux

## Contents

* mkxsocket
  * Creates empty window of specified properties with GTK socket and returns window ID
  * This window will be used for displaying video output
* getxsock
  * singleton wrapper around mkxsocket allows you to use single common window per user
* filters
  * mplaywrap
     * mplayer wrapper allowing you to play videos
  * xtermwrap
     * xterm wrapper allowing you to display asciiart, ansiart, etc...
* vizionix-cli
     * first experimental queue manager (without GUI)
     * reads specified queue of filters and loops it forever (you can edit the queue while running)
