#include <xinu.h>
#include <future.h>

syscall future_get(
		   future *f, 
		   int *value
		   )
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();
    if (f == NULL) {
      restore (mask);
	return SYSERR;
    }
    if (f->state == FUTURE_WAITING) {
      restore (mask);
	return SYSERR;
    }
    if (f->state == FUTURE_EMPTY) {
	f->state = FUTURE_WAITING;
	f->pid = currpid;
	prptr = &proctab[currpid];
	prptr->prstate = PR_WAIT;
	resched();
    }
    if (f->state == FUTURE_VALID) {
	*value = f->value;
	f->state = FUTURE_EMPTY;
    }
    restore (mask);
    return OK;
}
