#include "bench.h"

#include <cstddef>
#include <cstdlib>

namespace {

size_t num_new = 0;
size_t num_delete = 0;
} // namespace

void* operator new(size_t size) {
  void* memory = malloc(size);
  ++num_new;
  return memory;
}

namespace bench {

size_t new_calls() {
  return num_new;
}

size_t delete_calls() {
  return num_delete;
}

void reset_new() {
  num_new = 0;
}

void reset_delete() {
  num_delete = 0;
}

void reset_new_delete() {
  reset_new();
  reset_delete();
}

} // namespace bench