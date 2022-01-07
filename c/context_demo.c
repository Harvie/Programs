#include <ucontext.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

/* ucontext sample program

   Downloaded from https://gist.github.com/DanGe42/7148946
   A non-trivial example of how to use user contexts for trivial scheduling. 

   by Jon Kaplan and Robert Spier (October 24th, 1999)
   Updated for 2000 and poll, Robert Spier
   sigprocmask gaff fixed by Ben Slusky
   ported to Linux by Eric Cronin
   $Id: context_demo.c 37 2006-10-12 22:16:59Z ecronin $

   Demonstrates swapping between multiple processor contexts (in a
   _stable_ way).  n-1 contexts do nothing.  1 context accepts input
   and outputs it.
*/


#define NUMCONTEXTS 10              /* how many contexts to make */
#define STACKSIZE 4096              /* stack size */
#define INTERVAL 100                /* timer interval in nanoseconds */

sigset_t set;                       /* process wide signal mask */
ucontext_t signal_context;          /* the interrupt context */
void *signal_stack;                 /* global interrupt stack */

ucontext_t contexts[NUMCONTEXTS];   /* store our context info */
int curcontext = 0;                 /* whats the current context? */
ucontext_t *cur_context;            /* a pointer to the current_context */

/* The scheduling algorithm; selects the next context to run, then starts it. */
void
scheduler()
{
    printf("scheduling out thread %d\n", curcontext);

    curcontext = (curcontext + 1) % NUMCONTEXTS; /* round robin */
    cur_context = &contexts[curcontext];

    printf("scheduling in thread %d\n", curcontext);

    setcontext(cur_context); /* go */
}

/*
  Timer interrupt handler.
  Creates a new context to run the scheduler in, masks signals, then swaps
  contexts saving the previously executing thread and jumping to the
  scheduler.
*/
void
timer_interrupt(int j, siginfo_t *si, void *old_context)
{
    /* Create new scheduler context */
    getcontext(&signal_context);
    signal_context.uc_stack.ss_sp = signal_stack;
    signal_context.uc_stack.ss_size = STACKSIZE;
    signal_context.uc_stack.ss_flags = 0;
    sigemptyset(&signal_context.uc_sigmask);
    makecontext(&signal_context, scheduler, 1);

    /* save running thread, jump to scheduler */
    swapcontext(cur_context,&signal_context);
}

/* Set up SIGALRM signal handler */
void
setup_signals(void)
{
    struct sigaction act;

    act.sa_sigaction = timer_interrupt;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART | SA_SIGINFO;

    sigemptyset(&set);
    sigaddset(&set, SIGALRM);

    if(sigaction(SIGALRM, &act, NULL) != 0) {
        perror("Signal handler");
    }
}


/* Thread bodies */
void
thread1()
{
    while(1) {
        poll(NULL,0,100);
    };     /* do nothing nicely */
}

void
thread2()
{
    char buf[1024];
    /* get a string.. print a string.. ad infinitum */
    while(1) {
        fgets(buf, 1024, stdin);
        printf("[[[[[[%s]]]]]]\n",buf);
    }
}

/* helper function to create a context.
   initialize the context from the current context, setup the new
   stack, signal mask, and tell it which function to call.
*/
void
mkcontext(ucontext_t *uc,  void *function)
{
    void * stack;

    getcontext(uc);

    stack = malloc(STACKSIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(1);
    }
    /* we need to initialize the ucontext structure, give it a stack,
        flags, and a sigmask */
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = STACKSIZE;
    uc->uc_stack.ss_flags = 0;
    if (sigemptyset(&uc->uc_sigmask) < 0){
      perror("sigemptyset");
      exit(1);
    }

    /* setup the function we're going to, and n-1 arguments. */
    makecontext(uc, function, 1);

    printf("context is %p\n", uc);
}


int
main()
{
    int i;
    struct itimerval it;

    fprintf(stderr,"Process Id: %d\n", (int)getpid());

    /* allocate the global signal/interrupt stack */
    signal_stack = malloc(STACKSIZE);
    if (signal_stack == NULL) {
        perror("malloc");
        exit(1);
    }

    /* make all our contexts */
    mkcontext(&contexts[0], thread2);
    for(i=1; i < NUMCONTEXTS; i++)
        mkcontext(&contexts[i], thread1);


    /* initialize the signal handlers */
    setup_signals();

    /* setup our timer */
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = INTERVAL * 1000;
    it.it_value = it.it_interval;
    if (setitimer(ITIMER_REAL, &it, NULL) ) perror("setitiimer");

    /* force a swap to the first context */
    cur_context = &contexts[0];
    setcontext(&contexts[0]);

    return 0; /* make gcc happy */
}
