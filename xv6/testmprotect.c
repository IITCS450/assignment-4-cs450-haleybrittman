#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(void) {
  printf(1, "mprotect test: allocate one page\n");
  char *p = sbrk(PGSIZE);
  p[0] = 42;
  printf(1, "before mprotect: p[0]=%d\n", p[0]);

  if (mprotect(p, 1) < 0) {
    printf(1, "mprotect failed\n");
    exit();
  }
  printf(1, "mprotect succeeded, now forking\n");

  int pid = fork();
  if (pid < 0) {
    printf(1, "fork failed\n");
    exit();
  } else if (pid == 0) {
    printf(1, "child: attempting to write protected page (should fault)\n");
    p[0] = 7; // This should cause a page fault
    printf(1, "child: write succeeded (this should not happen)\n");
    exit();
  } else {
    int status = wait();
    if (status == -1) {
      printf(1, "parent: wait failed\n");
    } else {
      printf(1, "parent: child returned with status %d\n", status);
    }
  }
}
