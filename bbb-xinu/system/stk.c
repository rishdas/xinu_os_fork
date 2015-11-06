/* stk.c - Stamp and count unused stamps */


#include <xinu.h>

void stkstamp (
            char *stkbase, 
            uint32 stklen
              )
{
    char *ptr;

    ptr = stkbase;
    while (stklen)
    {
	*ptr-- = STACKSTAMP;
	stklen--;
    }
    return;
};

uint32 stkusedsize (
            char *stkbase,
	    uint32 stklen
                     )
{
    uint32 c,i;			/* count the number of unmodified stamps */
    char *ptr;
    
    c = 0;
    ptr = stkbase;
    i = stklen;
    while (i)
    {
	if (*ptr-- == STACKSTAMP)
	    c++;
	i--;
    }
    return (stklen - c);
}
