#include <xinu.h>
#include <stdlib.h>

/*Global variable for producer consumer*/
extern int n; /*this is just declaration*/

/*function Prototype*/
void consumer(int count);
void producer(int count);

extern sid32 producedsem, consumedsem;
