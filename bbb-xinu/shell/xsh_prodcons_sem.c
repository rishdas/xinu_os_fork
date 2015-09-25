#include <prodcons.h>

int n; 
int buf[15];
int S=1;  
 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/
void wait_pc(int S)
{
    while(S>0){
	;
    }
    S++;
}
void signal_pc(int S)
{
    S--;
}
void producer_sem(int count)
{
  //Code to produce values less than equal to count, 
  //produced value should get assigned to global variable 'n'.
  //print produced value e.g. produced : 8
    int i = 0;
    while(n <= count) {
	wait(S);
	i = 0;
	while (n <= count && i<15) {
	    n++;
	    buf[i]=n;
	    printf("Produced Value :%d\n", buf[i]);
	    i++;
	}
	signal(S);
    }

};

void consumer_sem(int count)
{
  //Code to consume values of global variable 'n' until the value
  // of n is less than or equal to count
  //print consumed value e.g. consumed : 8
    int j = 0;
    wait(S);
    while (j < 15 && n <= count) {
	printf("consumed : %d\n", buf[j]);
	j++;
    }
    signal(S);
}

shellcmd xsh_prodcons_sem(int nargs, char *args[])
{
      int count = 20;             //local varible to hold count

      //Argument verifications and validations
      if (nargs > 2)
	{
	  fprintf (stderr,"%s: invalid number of arguments\n", args[0]);
	  return 1;
	}
      
      if ((nargs == 2) && (strncmp(args[1], "--help", 7)) == 0) {
	printf("Use: %s  name\n\n", args[0]);
	printf("Description:\n");
	printf("\tprodcon sem impl\n");
	printf("Options:\n");
	printf("\t--help\t display this help and exit\n");
	printf("\tname\t the message addressed person's name\n");
	return 0;
      }

      //check args[1] if present assign value to count
      if (nargs == 2)
	count = atoi (args[1]);
      
      n = 0;
      //create the process producer and consumer and put them in ready queue.
      //Look at the definations of function create and resume in exinu/system folder for reference.      
      resume( create(producer_sem, 1024, 20, "producer_sem", 1, count) );
      resume( create(consumer_sem, 1024, 20, "consumer_sem", 1, count) );

      return 0;
}
