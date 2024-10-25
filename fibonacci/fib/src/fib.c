#include <fib.h>

uint32_t fib(uint32_t n) {
  if (n < 2) {
    return n;
  } else {
    return fib(n - 2) + fib(n - 1);
  }
}
