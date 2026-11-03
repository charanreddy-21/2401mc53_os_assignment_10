#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int bytes = 3 * 4096;
  char *p = sbrk(bytes);
  int i;
  if(p == (char*)-1){
    printf(2, "memtest: sbrk failed\n");
    exit();
  }
  // Touch one byte per page so lazy sbrk allocation obtains real frames.
  for(i = 0; i < bytes; i += 4096)
    p[i] = 1;
  printf(1, "After allocating and touching 3 pages:\n");
  memstats();
  if(sbrk(-bytes) == (char*)-1){
    printf(2, "memtest: release failed\n");
    exit();
  }
  printf(1, "After releasing the pages:\n");
  memstats();
  exit();
}
