#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *p = sbrk(2 * 4096);
  int pa;
  if(p == (char*)-1){
    printf(2, "vttest: sbrk failed\n");
    exit();
  }
  p[0] = 1; // Resolve the first lazy page before translation.
  pa = vtop((int)p);
  printf(1, "va 0x%x -> pa 0x%x\n", (uint)p, pa);
  pa = vtop((int)p + 123);
  printf(1, "va 0x%x -> pa 0x%x (same frame, nonzero offset)\n",
         (uint)p + 123, pa);
  printf(1, "unmapped lazy page 0x%x -> %d\n", (uint)p + 4096,
         vtop((int)p + 4096));
  printf(1, "out-of-range address -> %d\n", vtop(0x7ffff000));
  exit();
}
