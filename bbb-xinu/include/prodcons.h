#include <xinu.h>
#include <stdlib.h>
#include <future.h>

/*Global variable for producer consumer*/
extern int n; /*this is just declaration*/

/*function Prototype*/
void consumer(int count);
void producer(int count);
int future_prod(future *fut);
int future_cons(future *fut);

extern sid32 producedsem, consumedsem;
