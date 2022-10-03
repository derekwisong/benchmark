#pragma once

#include <chrono>
#include <thread>

namespace bench::cpu {

template <int val> void square() {
  int result = val * val;
  (void)result;
}

template <typename Duration>

class BusyLoop {
  const Duration duration;

public:
  BusyLoop(Duration duration) : duration(duration){};
  void operator()() const {
    const auto end = std::chrono::steady_clock::now() + duration;
    while (std::chrono::steady_clock::now() < end) {
    };
  }
};

template <typename Duration> class Sleeper {
  const Duration duration;

public:
  Sleeper(Duration duration) : duration(duration){};
  void operator()() const { std::this_thread::sleep_for(duration); }
};

void random_int();
void random_double();
} // namespace bench::cpu