#include <prodcons.h>

int n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons(int nargs, char *args[])
{
      int count = 2000;             //local varible to hold count

      //Argument verifications and validations
      if (nargs > 2)
	{
	  fprintf (stderr,"%s: invalid number of arguments\n", args[0]);
	  return 1;
	}
      
      if ((nargs == 2) && (strncmp(args[1], "--help", 7)) == 0) {
	printf("Use: %s  name\n\n", args[0]);
	printf("Description:\n");
	printf("\tDisplay a friendly message\n");
	printf("Options:\n");
	printf("\t--help\t display this help and exit\n");
	printf("\tname\t the message addressed person's name\n");
	return 0;
      }

      //check args[1] if present assign value to count
      if (nargs == 2)
	count = atoi (args[1]);
      

      //create the process producer and consumer and put them in ready queue.
      //Look at the definations of function create and resume in exinu/system folder for reference.      
      resume( create(producer, 1024, 20, "producer", 1, count) );
      resume( create(consumer, 1024, 20, "consumer", 1, count) );

      return 0;
}
