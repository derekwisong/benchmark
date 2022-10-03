#pragma once

#include <chrono>
namespace bench::cpu {

template <int val> void square() {
  int result = val * val;
  (void)result;
}

template <typename Duration> class BusyLoop {
  const Duration duration;

public:
  BusyLoop(Duration duration) : duration(duration){};
  void operator()() const {
    const auto end = std::chrono::steady_clock::now() + duration;
    while (std::chrono::steady_clock::now() < end) {
    };
  }
};

void random_int();
void random_double();
} // namespace bench::cpu