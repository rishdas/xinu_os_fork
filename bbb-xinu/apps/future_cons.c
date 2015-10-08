#include <prodcons.h>
#include <future.h>

int future_cons(future *fut) {

  int i;
  int status;
  int is_cons = 0;
  while (is_cons == 0) {
      status = future_get(fut, &i);
      if (status == SYSERR) {
	  continue;
      }
      if (status == WAIT) {
	  continue;
      }
      printf("it produced 0x%x\n", i);
      is_cons = 1;
  }
  return OK;
}
