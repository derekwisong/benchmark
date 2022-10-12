#include "bench/benchmarks/memory.h"
#include <cstddef>

namespace bench::memory {

size_t random_index(size_t size) {
  return std::rand() % size;
}

} // namespace bench::memory