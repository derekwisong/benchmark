#pragma once

#include <chrono>
#include <iostream>
#include <vector>

namespace bench::timing {
namespace chrono = std::chrono;

class Timer {
  using clock_t = chrono::high_resolution_clock;
  clock_t::time_point start;

public:
  Timer() : start(now()) {}
  chrono::nanoseconds elapsed() const { return clock_t::now() - start; }
  void reset() { start = now(); }
  static clock_t::time_point now() { return clock_t::now(); }
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

class TimedRunStats : public TimedRunResults {
public:
  const std::vector<std::chrono::nanoseconds> samples;

  TimedRunStats(size_t iterations, chrono::nanoseconds elapsed, chrono::nanoseconds time_limit,
                std::vector<std::chrono::nanoseconds> samples)
      : TimedRunResults(iterations, elapsed, time_limit), samples(std::move(samples)) {}
  
  double avg() const;
  double max() const;
  double median() const;
  double min() const;
  double stdev() const;
  double variance() const;
};

template <typename Func, typename Duration>
TimedRunResults repeat_for(Func&& func, Duration duration) {
  const auto nanos_timelimit = chrono::duration_cast<chrono::nanoseconds>(duration);
  size_t iterations = 0;

  const Timer timer;
  while (timer.elapsed() < nanos_timelimit) {
    func();
    ++iterations;
  }

  return {iterations, timer.elapsed(), nanos_timelimit};
}

template <typename Func, typename Duration> TimedRunStats repeat_for_stats(Func&& func, Duration duration) {
  const auto nanos_timelimit = chrono::duration_cast<chrono::nanoseconds>(duration);
  size_t iterations = 0;
  std::vector<std::chrono::nanoseconds> runtimes;

  const Timer timer;
  while (timer.elapsed() < nanos_timelimit) {
    const Timer func_timer;
    func();
    runtimes.push_back(func_timer.elapsed());
    ++iterations;
  }

  return {iterations, timer.elapsed(), nanos_timelimit, std::move(runtimes)};
}

} // namespace bench::timing