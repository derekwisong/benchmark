#include "bench/fileio.h"

#include <cstdio>
#include <fstream>

#include <sys/stat.h>

namespace bench::fileio {

void open_close_fstream(const char *path) {
  std::fstream file;
  file.open(path, std::ios::in);
  file.close();
}

void open_close_fopen(const char *path) {
  FILE *file = fopen(path, "rb");
  fclose(file);
}

void stat_file(const char *path) {
  struct stat st;
  stat(path, &st);
}

} // namespace bench::fileio