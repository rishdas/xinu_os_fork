#include <xinu.h>

shellcmd xsh_freemem(int nargs, char *args[])
{

    //Argument verifications and validations
    if ((nargs < 1) || ((nargs == 2) && (strncmp(args[1], "--help", 7))) == 0) {
	printf("Use: %s  [option]\n\n", args[0]);
	printf("Description:\n");
	printf("\tEnables/disables memory unallocation\n");
	printf("Options:\n");
	printf("\t--help\t display this help and exit\n");
	printf("\t--enable\t enables memory unallocation\n");
	printf("\t--disable\t disables memory unallocation\n");
	return OK;
    }
    if (strncmp (args[1], "--enable",8) == 0) {
	fmptr = &freemem;
    }
    else if (strncmp (args[1], "--disable",9) == 0 ) {
	fmptr = &freenomem;
    }
    else
	return SYSERR;

    return OK;
}
