#include <xinu.h>
#include <future.h>


fut32 future_alloc(int future_flag)
{
    static fut32 nextfut = 0;/*Next future index to try*/
    fut32 fut; /*Future ID*/
    int32 i;   /*Iterate through #entries*/
    for (i = 0; i< NFUTURES ; i++) {
	fut = nextfut++;
	if (nextfut >= NFUTURES)
	    nextfut = 0;
	if (futtab[fut].state == FUTURE_UNUSED) {
	    futtab[fut].state = FUTURE_EMPTY;
	    futtab[fut].flag = future_flag;
	    return fut;
	}
    }
    return SYSERR;
}
