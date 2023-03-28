#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

namespace bench::timing {
namespace chrono = std::chrono;

template <typename Duration> class Timer {
  using clock_t = chrono::high_resolution_clock;
  clock_t::time_point start;

public:
  Timer() : start(now()) {}

  Duration elapsed() const {
    return chrono::duration_cast<Duration>(clock_t::now() - start);
  }

  void reset() {
    start = now();
  }

  static clock_t::time_point now() {
    return clock_t::now();
  }
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

template <typename Duration> class TimedRunStats {
  std::vector<Duration> samples;


  Duration minimum{0};
  Duration maximum{0};
  double average{0};
  Duration sum_squares{0};
  Duration sum{0};
  size_t count{0};

  void update_median(const Duration& sample) {
    
  }

  void update_sum_squares(const Duration& sample) {
    sum_squares += Duration{(sample.count() * sample.count())};
  }

  void update_sum(const Duration& sample) {
    sum += sample;
  }

  void update_average(const Duration& sample) {
    average += (static_cast<double>(sample.count()) - average) / count;
  }

  void update_min(const Duration& sample) {
    if (sample < minimum) {
      minimum = sample;
    }
  }

    void update_max(const Duration& sample) {
    if (sample > maximum) {
      maximum = sample;
    }
  }

public:
  void add_sample(const Duration& sample) {
    samples.push_back(sample);
    ++count;
    update_average(sample);
    update_median(sample);
    update_min(sample);
    update_max(sample);
    update_sum(sample);
    update_sum_squares(sample);
  }

  double avg() const {
    return average;
  }

  Duration max() const {
    return maximum;
  }

  double median2() const {
    return 0;
  }

  double median() const {
    // sorted copy of the samples
    auto sorted = [&](auto vec) {
      std::sort(vec.begin(), vec.end());
      return std::move(vec);
    }(samples);

    if (sorted.size() % 2 == 0) {
      // average of the middle two
      size_t mid = sorted.size() / 2;
      return static_cast<double>((sorted[mid - 1] + sorted[mid]).count()) / 2;
    }

    return sorted[(sorted.size() - 1) / 2].count();
  }

  Duration min() const {
    return minimum;
  };

  double stdev() const {
    return std::sqrt(variance());
  };

  double variance() const {
    return ((static_cast<double>(sum_squares.count())) / count) - (avg() * avg());
  }
};

template <typename Func, typename Duration> TimedRunResults repeat_for(Func&& func, Duration duration) {
  const auto nanos_timelimit = chrono::duration_cast<chrono::nanoseconds>(duration);
  size_t iterations = 0;

  const Timer<chrono::nanoseconds> timer;
  while (timer.elapsed() < nanos_timelimit) {
    func();
    ++iterations;
  }

  return {iterations, timer.elapsed(), nanos_timelimit};
}

template <typename Func, typename Duration> TimedRunStats<Duration> repeat_for_stats(Func&& func, Duration duration) {
  TimedRunStats<Duration> stats;
  const Timer<Duration> timer;

  while (timer.elapsed() < duration) {
    const Timer<Duration> func_timer;
    func();
    stats.add_sample(func_timer.elapsed());
  }

  return stats;
}

} // namespace bench::timing