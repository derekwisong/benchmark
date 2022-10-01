#pragma once

#include <cstddef>
#include <cstdlib>

namespace bench::memory {

template <size_t size> void malloc_free() {
  auto ptr = std::malloc(size);
  std::free(ptr);
}

template <typename Type> void new_delete() {
  auto obj = new Type();
  delete obj;
}

template <typename Type, size_t count> void new_delete_a() {
  auto obj = new Type[count];
  delete[] obj;
}

template <typename Ptr> void random_access(const Ptr *array, size_t size) {
  auto val = array[std::rand() % size];
  (void)val;
}
} // namespace bench::memory