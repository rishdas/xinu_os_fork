#include <prodcons.h>

void producer(int count)
{
    //Code to produce values less than equal to count, 
    //produced value should get assigned to global variable 'n'.
    //print produced value e.g. produced : 8

    while (n < count) {
	wait(consumedsem);
	n++;
	printf ("produced: %d\n",n);
	signal(producedsem);
    }

}
