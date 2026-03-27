#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  char *p = sbrk(4096); 
  if (p == (char *)-1) {
    printf(1, "sbrk failed\n");
    exit();
  }

  p[0] = 42;
  printf(1, "Before mprotect: p[0] = %d\n", p[0]);

  if (mprotect(p, 1) < 0) {
    printf(1, "mprotect failed\n");
    exit();
  }

  printf(1, "Attempting to write to protected page (should fault)...\n");
  if (fork() == 0) {
    p[0] = 7; 
    printf(1, "Child: Write succeeded (this should not happen)\n");
    exit();
  } else {
    wait();
  }

  if (munprotect(p, 1) < 0) {
    printf(1, "munprotect failed\n");
    exit();
  }

  p[0] = 7;
  printf(1, "After munprotect: p[0] = %d\n", p[0]);

  if (p[0] == 7) {
    printf(1, "munprotect test passed\n");
  } else {
    printf(1, "munprotect test failed\n");
  }

  exit();
}