# pthread_extra

Library implementing extra features on top of POSIX threads.
Currently only tested on Linux. Mostly experimental code.

### Components

  * pthread_msgqueue - implements message queues, more features than POSIX mqueue (no known issues, but not really tested)
  * pthread_multi - lock multiple mutexes at once (might cause deadlocks in complex scenarios)
  * pthread_pause - implements suspend/resume functionality for pthreads (causes deadlocks under high load)
