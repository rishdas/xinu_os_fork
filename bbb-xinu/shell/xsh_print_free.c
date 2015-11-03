#include <xinu.h>

shellcmd xsh_print_free(int nargs, char *args[])
{
    //Argument verifications and validations
    struct memblk *memptr; 
    if ((nargs == 2) && (strncmp(args[1], "--help", 7)) == 0) {
	printf("Use: %s  [Integer]\n\n", args[0]);
	printf("Description:\n");
	printf("\tPrint list of free block addresses and size\n");
	printf("Options:\n");
	printf("\t--help\t display this help and exit\n");
	return 0;
    }
    memptr = memlist.mnext;
    while (memptr != NULL)
    {
	printf("Address:%x\tLength:%d\n", memptr, 
	       memptr->mlength);
	memptr = memptr->mnext;
    }
    return 0;
}
