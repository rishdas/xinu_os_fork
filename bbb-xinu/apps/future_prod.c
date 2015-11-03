#include <prodcons.h>

int future_prod(fut32 fut) {
  int j;
  j = fut;
  future_set(fut, &j);
  return OK;
}
