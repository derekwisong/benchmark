#pragma once

namespace bench::cpu {

template <int val> void square() {
  int result = val * val;
  (void)result;
}

void random_int();
void random_double();
} // namespace bench::cpu