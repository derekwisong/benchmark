#include "bench.h"

#include <iostream>

int main(int, char**) {
  static int i = 0;
  int* ptr = &i;
  
  bench::Benchmark b{"Name", [ptr] { ++*ptr; }};
  b.run();

  if (i == 1) {
    return 0;
  }
  return 1;
}