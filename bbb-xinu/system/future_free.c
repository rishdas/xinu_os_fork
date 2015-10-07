#include <xinu.h>
#include <future.h>

syscall future_free(future *f)
{

    syscall ret_val;
    ret_val = freemem((char *)f, sizeof(*f));
    if(ret_val != OK) {
	return SYSERR;
    }
    return OK;
}
