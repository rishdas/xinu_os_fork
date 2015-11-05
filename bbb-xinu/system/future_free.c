#include <xinu.h>
#include <future.h>

syscall future_free(fut32 fut
		    /*ID of the future*/)
{
    intmask mask;
    /*Saved interupt mask*/
    struct futent *futptr; /*ptr to future table*/
    
    mask = disable();
    
    if (isbadfut(fut)) {
	restore(mask);
	return SYSERR;
    }
    futptr = &futtab[fut];
    if (futptr->state == FUTURE_UNUSED) {
	restore(mask);
	return SYSERR;
    }
    futptr->state = FUTURE_UNUSED;
    resched_cntl(DEFER_START);
    while (futptr->set_count-- > 0) {
	/* Free all waiting set process */
	ready(getfirst(futptr->set_queue));
    }
    while (futptr->get_count-- > 0) {
	/* Free all waiting get process */
	ready(getfirst(futptr->get_queue));
    }
    resched_cntl(DEFER_STOP);
    restore(mask);
    resched();
    return OK;
}
