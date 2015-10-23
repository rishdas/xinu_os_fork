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
    case FUTURE_QUEUE:
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
	if (f->state == FUTURE_EMPTY) {
	    f->value = *value;
	    f->state = FUTURE_VALID;
	    return OK;
	}
	break;
    case FUTURE_QUEUE:
      if (f->state == FUTURE_EMPTY) {
	f->set_count++;
	prptr = &proctab[currpid];
	prptr->prstate = PR_WAIT;
	prptr->prfut = fut;
	enqueue(currpid, f->set_queue);
	restore (mask);
	resched();
      }
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
		ready(p);
	    }
	    resched_cntl(DEFER_STOP);
	}
	break;
    case FUTURE_QUEUE:
      if (f->state == FUTURE_WAITING) {
	f->value = *value;
	f->state = FUTURE_VALID;
	if (f->get_count-- > 0){
	  ready(getfirst(f->get_queue));
	}
      }
      break;
    default:
	if (f->state == FUTURE_WAITING) {
	    f->value = *value;
	    f->state = FUTURE_VALID;
	    if (f->get_count-- > 0){
	    ready(getfirst(f->get_queue));
	    }
	}
	break;
    }
    restore (mask);
    return OK;
}
