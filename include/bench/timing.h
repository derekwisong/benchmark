#pragma once

#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <tuple>

namespace bench::timing {
namespace chrono = std::chrono;

class Timer {
  using clock_t = chrono::high_resolution_clock;
  const clock_t::time_point start{clock_t::now()};

public:
  Timer() : start(clock_t::now()) {}
  chrono::nanoseconds elapsed() const { return clock_t::now() - start; }
};

class TimedRunResults {
public:
  const size_t iterations;
  const chrono::nanoseconds elapsed;
  const chrono::nanoseconds time_limit;

  TimedRunResults(size_t iterations, chrono::nanoseconds elapsed, chrono::nanoseconds time_limit)
      : iterations(iterations), elapsed(elapsed), time_limit(time_limit) {}

  double scaled_iterations() const;
  double cps() const;
  double spc() const;
};

template <typename Func, typename Duration> TimedRunResults repeat_for(Func&& func, Duration duration) {
  const auto nanos_timelimit = chrono::duration_cast<chrono::nanoseconds>(duration);
  size_t iterations = 0;

  const Timer timer;
  while (timer.elapsed() < nanos_timelimit) {
    func();
    ++iterations;
  }

  return {iterations, timer.elapsed(), nanos_timelimit};
}

} // namespace bench::timing