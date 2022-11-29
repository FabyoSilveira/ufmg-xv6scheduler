#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) 
{ 
  int a = 1;
  int b = 2;
  int c = 3;

  int *p = &a;
  int *x = &b;
  int *z = &c;
  
  printf(1, "\nIf return 10 is working: %d\n", wait2(p, x, z));

  exit();
}