#include "bench/cpu.h"

#include <cstdlib>

namespace bench::cpu {

void random_int() {
  auto val = std::rand();
  (void)val;
}

void random_double() {
  auto val = static_cast<double>(std::rand()) / RAND_MAX;
  (void)val;
}

} // namespace bench::cpu