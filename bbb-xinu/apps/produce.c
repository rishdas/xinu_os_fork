<<<<<<< .merge_file_1YbzEK
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
    if (n == count) semdelete (consumedsem);
}
=======
#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
}



>>>>>>> .merge_file_0lELCL
