#include <prodcons.h>

void consumer(int count)
{
    //Code to consume values of global variable 'n' until the value
    // of n is less than or equal to count
    //print consumed value e.g. consumed : 8
    while (n <= count) {
	wait(producedsem);
	printf("consumed : %d\n", n);
	signal(consumedsem);
    }
    if (n == count) semdelete (producedsem);
}
