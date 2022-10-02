#pragma once

namespace bench::fileio {

void open_close_fstream(const char *path);
void open_close_fopen(const char *path);
void stat_file(const char *path);

} // namespace bench::fileio