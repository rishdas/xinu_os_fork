#include <xinu.h>
#include <future.h>

syscall future_get(future *f, int *value)
{
    if (f == NULL) {
	return SYSERR;
    }
    if (f->state == FUTURE_WAITING) {
	return SYSERR;
    }
    if (f->state == FUTURE_EMPTY) {
	f->state = FUTURE_WAITING;
	f->state = currpid;
	return OK;
    }
    if (f->state == FUTURE_VALID) {
	value = f->value;
	f->state = FUTURE_EMPTY;
    }
    return OK;
}
