#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  volatile char *p = sbrk(4096);
  int pid;
  if(p == (char*)-1){
    printf(2, "pagetest: sbrk failed\n");
    exit();
  }
  *p = 7; // Force the lazy page to be mapped while it is still writable.
  printf(1, "Before protection:\n");
  if(pageinfo((int)p) < 0 || setreadonly((int)p) < 0){
    printf(2, "pagetest: page permission update failed\n");
    exit();
  }
  printf(1, "After setreadonly:\n");
  pageinfo((int)p);
  pid = fork();
  if(pid == 0){
    printf(1, "Child attempting a write to the read-only page...\n");
    *p = 9;
    printf(1, "ERROR: protected write unexpectedly succeeded\n");
    exit();
  }
  if(pid < 0){
    printf(2, "pagetest: fork failed\n");
    exit();
  }
  wait();
  printf(1, "Parent survived the child's protection fault.\n");
  exit();
}
