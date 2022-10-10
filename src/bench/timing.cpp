#include "bench/timing.h"

namespace {
const long NANOS_PER_SEC = 1'000'000'000;

}
namespace bench::timing {
double TimedRunResults::scaled_iterations() const { return static_cast<double>(iterations) / (elapsed / time_limit); }

double TimedRunResults::cps() const {
  return static_cast<double>(iterations) / (static_cast<double>(elapsed.count()) / NANOS_PER_SEC);
}

double TimedRunResults::spc() const { return static_cast<double>(elapsed.count()) / NANOS_PER_SEC / iterations; }

} // namespace bench::timing