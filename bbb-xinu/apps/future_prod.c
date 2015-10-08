#include <prodcons.h>

int future_prod(future *fut) {
  int i, j;
  j = (uint32)fut;
    for (i=0; i<1000; i++) {
    j += i;
  }

  future_set(fut, &j);
  return OK;
}
