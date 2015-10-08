#include <xinu.h>
#include <future.h>

syscall future_set (
		    future *f, 
		    int *value
		    )
{
  intmask mask;			/* Saved interrupt mask		*/

  mask = disable();
    if (f == NULL) {
      restore(mask);
	return SYSERR;
    }
    if (f->state == FUTURE_VALID) {
	restore(mask);
	return SYSERR;
    }
    if (f->state == FUTURE_EMPTY) {
	f->value = *value;
	f->state = FUTURE_VALID;
    }
    if (f->state == FUTURE_WAITING) {
      f->value = *value;
      f->state = FUTURE_VALID;
      /* ready (f->pid); */
    }
    restore (mask);
    return OK;
}
