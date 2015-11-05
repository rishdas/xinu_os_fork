#include <prodcons.h>
#include <future.h>

int future_cons(fut32 fut) {

  int i;
  int status;
  status = future_get(fut, &i);
  if (status == SYSERR) {
      return status;
  }
  printf("it produced 0x%x\n", i);
  return OK;
}
