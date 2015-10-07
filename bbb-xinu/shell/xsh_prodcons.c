#include <prodcons.h>

local void usefuture (void);

/*Definition for global variable 'n'*/
int n;
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 producedsem, consumedsem;
future *f1, *f2, *f3;

local void usefuture (void)
{

  f1 = future_alloc(FUTURE_EXCLUSIVE);
  f2 = future_alloc(FUTURE_EXCLUSIVE);
  f3 = future_alloc(FUTURE_EXCLUSIVE);

  resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
  resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
  resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
  resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
  resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
  resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );
};

shellcmd xsh_prodcons(int nargs, char *args[])
{
    int count = 2000;             //local varible to hold count

    //Argument verifications and validations
    if (nargs > 2) {
	fprintf (stderr,"%s: invalid number of arguments\n", args[0]);
	return 1;
    }
      
    if ((nargs == 2) && (strncmp(args[1], "--help", 7)) == 0) {
	printf("Use: %s  [Integer]\n\n", args[0]);
	printf("Description:\n");
	printf("\tProduces and consumes numbers\n");
	printf("Options:\n");
	printf("\t--help\t display this help and exit\n");
	printf("\t[Integer]\t count (default:2000)\n");
	return 0;
    }

    if ((nargs == 2) && (strncmp (args[1], "-f",1)) == 0) {
      usefuture ();
      return 0;
    }

    //check args[1] if present assign value to count
    if (nargs == 2)
	count = atoi (args[1]);
      
    n = 0;
    producedsem = semcreate(0);
    consumedsem = semcreate(1);

    /* create the process producer and consumer  */
    /* and put them in ready queue. */
    /* Look at the definations of function create and resume in  */
    /* xinu/system folder for reference. */
      
    resume( create(producer, 1024, 20, "producer", 1, count) );
    resume( create(consumer, 1024, 20, "consumer", 1, count) );

    return 0;
}
