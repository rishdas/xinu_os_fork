#include <xinu.h>
#include <future.h>

syscall future_get(
    fut32 fut, 
    int *value
    )
{
    intmask mask;   /*Saved interrupt mask*/
    struct futent *f; /*Ptr to future table entry*/
    struct procent *prptr;


    mask = disable();
    if (isbadfut(fut)) {
	restore(mask);
	return SYSERR;
    }
    f = &futtab[fut];
    switch(f->flag)
    {
    case FUTURE_EXCLUSIVE:
	if (f->state == FUTURE_WAITING) {
	    restore (mask);
	    return SYSERR;
	}
	if (f->state == FUTURE_EMPTY) {
	    f->state = FUTURE_WAITING;
	    f->get_count++;
	    prptr = &proctab[currpid];
	    prptr->prstate = PR_WAIT;
	    prptr->prfut = fut;
	    enqueue(currpid, f->get_queue);
	    restore (mask);
	    resched();
	}
	break;
    case FUTURE_SHARED:
	if (f->state == FUTURE_WAITING) {
	    prptr = &proctab[currpid];
	    prptr->prstate = PR_WAIT;
	    prptr->prfut = fut;
	    f->get_count++;
	    enqueue(currpid, f->get_queue);
	    restore(mask);
	    resched();
	}
	if (f->state == FUTURE_EMPTY) {
	    f->state = FUTURE_WAITING;
	    f->get_count++;
	    prptr = &proctab[currpid];
	    prptr->prstate = PR_WAIT;
	    prptr->prfut = fut;
	    enqueue(currpid, f->get_queue);
	    restore (mask);
	    resched();
	}
	break;
    case FUTURE_QUEUE:
      if (f->state == FUTURE_WAITING ||
	  f->state == FUTURE_EMPTY) {
	f->state = FUTURE_WAITING;
        f->get_count++;
	prptr = &proctab[currpid];
	prptr->prstate = PR_WAIT;
	prptr->prfut = fut;
	enqueue (currpid, f->get_queue);
	if (f->set_count-- > 0) {
	  ready (getfirst (f->set_queue));
	}
	else
	  resched();
      }
      break;
    default:
	if (f->state == FUTURE_WAITING) {
	    restore (mask);
	    return SYSERR;
	}
	if (f->state == FUTURE_EMPTY) {
	    f->state = FUTURE_WAITING;
	    f->get_count++;
	    prptr = &proctab[currpid];
	    prptr->prstate = PR_WAIT;
	    prptr->prfut = fut;
	    enqueue(currpid, f->get_queue);
	    restore (mask);
	    resched();
	}
	break;
    }

    if (f->state == FUTURE_VALID) {
	*value = f->value;
	if (f->set_count-- > 0) {
	  f->state = FUTURE_WAITING;
	  ready(getfirst(f->set_queue));
	} else {
	  if (f->get_count > 0) {
	    f->state = FUTURE_WAITING;
	    resched();
	  }
	}
    }
    restore (mask);
    return OK;
}
