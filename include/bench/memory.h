#pragma once

#include <cstddef>
#include <cstdlib>
#include <span>

namespace bench::memory {

template <size_t size> void malloc_free() {
  auto* ptr = std::malloc(size);
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

size_t random_index(size_t size) {
  return std::rand() % size;
}

template <typename Ptr> void random_access(const Ptr* array, size_t size) {
  auto val = array[random_index(size)];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  (void)val;
}

template <typename Ptr> const Ptr& random_draw(const Ptr* array, size_t size) {
  return array[random_index(size)];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}

template <typename Ptr> const Ptr& random_span_draw(const Ptr* array, size_t size) {
  auto span = std::span(array, size);
  return span[random_index(size)];
}

} // namespace bench::memory