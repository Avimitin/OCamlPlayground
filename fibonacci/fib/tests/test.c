#include <stdio.h>
#include <fib.h>

int main() {
  uint32_t val = fib(13);
  printf("Fibonacci of 13 is %d\n", val);
  return 0;
}
