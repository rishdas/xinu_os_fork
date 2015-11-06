/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*curr;
	struct	memblk	*fits;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use mblock multiples	*/

	curr = memlist.mnext;
	fits = NULL;
	
	if (curr->mlength >= nbytes) {	
	    fits = curr;
	} else {
	    return (char *)SYSERR;
	}
		
	fits->mlength -= nbytes;
	fits = (struct memblk *)((uint32)fits + fits->mlength);

	memlist.mlength -= nbytes;
	restore(mask);
	return (char *)((uint32) fits + sizeof(uint32));
}
