#include "bench/timing.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>

namespace {
const long NANOS_PER_SEC = 1'000'000'000;

}
namespace bench::timing {
double TimedRunResults::scaled_iterations() const { return static_cast<double>(iterations) / (elapsed / time_limit); }

double TimedRunResults::cps() const {
  return static_cast<double>(iterations) / (static_cast<double>(elapsed.count()) / NANOS_PER_SEC);
}

double TimedRunResults::spc() const { return static_cast<double>(elapsed.count()) / NANOS_PER_SEC / iterations; }

double TimedRunStats::variance() const {
  const auto mean = avg();
  double sse = std::accumulate(samples.begin(), samples.end(), 0.0, [&](const auto& sse, const auto& sample) {
    auto delta = sample.count() - mean;
    return sse + (delta * delta);
  });
  return sse / samples.size();
}

double TimedRunStats::stdev() const {
  return std::sqrt(variance());
}

double TimedRunStats::avg() const {
  auto total = std::accumulate(samples.begin(), samples.end(), std::chrono::nanoseconds{0});
  return static_cast<double>(total.count()) / samples.size();
}

double TimedRunStats::median() const {
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

double TimedRunStats::max() const { return std::max_element(samples.begin(), samples.end())->count(); }

double TimedRunStats::min() const { return std::min_element(samples.begin(), samples.end())->count(); }

} // namespace bench::timing