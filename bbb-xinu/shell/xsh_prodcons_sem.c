#include <xinu.h>
#include <stdlib.h>

void producer_sem(int );
void consumer_sem(int );

sid32 sem0, sem1;
int buf[15];
int n;

shellcmd xsh_prodcons_sem(int nargs, char *args[])
{
      int count = 2000;             //local varible to hold count
      
      //Argument verifications and validations
      if (nargs > 2)
	{
	  fprintf (stderr,"%s: invalid number of arguments\n", args[0]);
	  return 1;
	}
      
      if ((nargs == 2) && (strncmp(args[1], "--help", 7)) == 0) {
	printf("Use: %s  [Integer]\n\n", args[0]);
	printf("Description:\n");
	printf("\tprodcons semaphore implementation\n");
	printf("Options:\n");
	printf("\t--help\t display this help and exit\n");
	printf("\t[Integer]\t count value (default 2000)\n");
	return 0;
      }

      //check args[1] if present assign value to count
      if (nargs == 2)
	count = atoi (args[1]);
      
      n = 0;

      sem0 =  semcreate(0);
      sem1 = semcreate(1);

      resume( create(producer_sem, 1024, 20, "producer_sem", 1, count) );
      resume( create(consumer_sem, 1024, 20, "consumer_sem", 1, count) );

      return 0;
}

void producer_sem(int count)
{
  //Code to produce values less than equal to count, 
  //produced value should get assigned to global variable 'n'.
  //print produced value e.g. produced : 8
  int i;
  while(n < count) {
    wait(sem1);
    i = 0;
    while (n < count && i < 15){
      buf[i]=n;
      printf("Produced Value :%d\n", buf[i]);
      n++;
      i++;
    }
    signal(sem0);
  } 
};

void consumer_sem(int count)
{
  //Code to consume values of global variable 'n' until the value
  // of n is less than or equal to count
  //print consumed value e.g. consumed : 8
  int j;
  wait(sem0);
  j = 0;
  while (j < 15 && n <= count) {
    printf("consumed : %d\n", buf[j]);
    j++;
  }
  signal(sem1);
}
