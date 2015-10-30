#include <prodcons.h>

int future_prod(fut32 fut) {
  int j;
  j = (uint32)&fut%10;
  future_set(fut, &j);
  return OK;
}
