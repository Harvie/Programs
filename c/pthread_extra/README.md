# pthread_extra

Library implementing extra features on top of POSIX threads.
Currently will probably only work on Linux. Mostly experimental code.

### Components

  * pthread_msgqueue - implements message queues, more features than POSIX mqueue (no known issues, but not really tested)
  * pthread_user_data - allows user to store and retreive custom data using thread handle as a key (suboptimal, used internaly, not tested)
  * pthread_multi - lock multiple mutexes at once (might cause deadlocks in complex scenarios)
  * pthread_pause - implements suspend/resume functionality for pthreads (causes deadlocks under high load)
