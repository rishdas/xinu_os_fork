#include <xinu.h>
#include <future.h>

syscall future_set (
    fut32 fut, 
    int *value
    )
{
    intmask mask;
/*Saved interrupt mask*/
    struct futent *f;
    struct procent *prptr;
    pid32  p;
    mask = disable();
    if (isbadfut(fut)) {
	restore(mask);
	return SYSERR;
    }
    f = &futtab[fut];
    switch (f->flag) {
    case FUTURE_EXCLUSIVE:
    case FUTURE_SHARED:
    default:
	if (f->state == FUTURE_VALID) {
	    restore(mask);
	    return SYSERR;
	}
	break;
    }
    switch (f->flag) {
    case FUTURE_EXCLUSIVE:
    case FUTURE_SHARED:
    default:
	if (f->state == FUTURE_EMPTY) {
	    f->value = *value;
	    f->state = FUTURE_VALID;
	    return OK;
	}
	break;
    }
    switch (f->flag) {
    case FUTURE_EXCLUSIVE:
	if (f->state == FUTURE_WAITING) {
	    f->value = *value;
	    f->state = FUTURE_VALID;
	    f->get_count--;
	    ready(getfirst(f->get_queue));
	}
	break;
    case FUTURE_SHARED:
	if (f->state == FUTURE_WAITING) {
	    f->value = *value;
	    f->state = FUTURE_VALID;
	    resched_cntl(DEFER_START);
	    while(f->get_count-- > 0) {
		/*Signal all procs in get Queue*/
		p = getfirst(f->get_queue);
		prptr = &proctab[p];
		kprintf("Dequed: %s\n", prptr->prname);
		ready(p);
	    }
	    resched_cntl(DEFER_STOP);
	}
	break;
    default:
	if (f->state == FUTURE_WAITING) {
	    f->value = *value;
	    f->state = FUTURE_VALID;
	    f->get_count--;
	    ready(getfirst(f->get_queue));
	}
	break;
    }
    restore (mask);
    return OK;
}
