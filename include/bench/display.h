#pragma once

#include "bench/timing.h"

#include <string>

namespace bench::display {

std::string format_cps(double cps);
std::string format_spc(double spc);
void print_run_results(const std::string &description, int runtime_ms, const bench::timing::timed_run_t &results);
} // namespace bench::display