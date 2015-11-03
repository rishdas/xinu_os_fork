/* freenomem.c - freenomem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  freenomem  -  Do not free a memory block
 *------------------------------------------------------------------------
 */
syscall	freenomem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	return OK;
}
