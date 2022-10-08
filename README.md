# Benchmark Library

## Compiling

The Benchmark Library project is a CMake project.

To add this to your own CMake project use `add_subdirectory(<bench subdirectory>)` to your own `CMakeLists.txt` and
link against the library `bench`. 

### Build Instructions

```bash
$ mkdir build
$ cd build
$ cmake ../
$ cmake build
$ make
```

The following files will be produced:

- `benchmark`: An executable which will run a set of sample benchmarks.
- `libbench.a`: Static library (use with headers in the repo `includes/` directory).
