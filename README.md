# Blas testing
# BLAS Testing with Various Backends

This project demonstrates how to use BLAS libraries with different backends (MKL, OpenBLAS, etc.) in C++ and Fortran applications.

## Requirements

- pixi package manager
- CMake 3.10+
- C++ and Fortran compilers

## Structure

```
./
├── CMakeLists.txt        # Top-level CMake configuration
├── cmake/                # CMake modules
│   └── FindMKL.cmake     # MKL finder module
├── pixi.toml             # Pixi configuration
└── src/                  # Source code
    ├── cpp/              # C++ examples
    │   ├── dgemm/        # DGEMM example
    │   └── openblas/     # OpenBLAS example
    ├── fortran/          # Fortran examples
    │   ├── dgemm/        # DGEMM example
    │   └── openblas/     # OpenBLAS example
    └── mixed/            # Mixed C++/Fortran example
        ├── mixed_example.cpp      # C++ main code
        └── fortran_wrapper.f90    # Fortran DGEMM wrapper
```

## Setup

Create and activate the MKL environment:

```bash
pixi install
pixi shell mklenv
```

## Build

Build examples with MKL backend:

```bash
pixi run configure-mkl
pixi run build-mkl
```

Build examples with OpenBLAS backend:

```bash
pixi run configure-openblas
pixi run build-openblas
```

## Run

Run specific examples with MKL backend:

```bash
pixi run run-cpp-mkl      # Run C++ example with MKL
pixi run run-fortran-mkl  # Run Fortran example with MKL
pixi run run-mixed-mkl    # Run mixed C++/Fortran example with MKL
pixi run run-all-mkl      # Run all MKL examples
```

Run examples with OpenBLAS backend:

```bash
pixi run run-cpp-openblas      # Run C++ example with OpenBLAS
pixi run run-fortran-openblas  # Run Fortran example with OpenBLAS
pixi run run-mixed-openblas    # Run mixed C++/Fortran example with OpenBLAS
pixi run run-all-openblas      # Run all OpenBLAS examples
```

Or run all examples with all backends at once:

```bash
pixi run run-all
```

## Adding New Backends

To add a new backend (e.g., OpenBLAS):

1. Add a new feature in `pixi.toml`
2. Update CMake configuration if needed
3. Ensure proper linking in the build system

## Performance Comparison

You can compare the performance of different BLAS backends by running the examples with different environment configurations and comparing the GFLOPS output.

Typical results might look like:

```
# MKL with C++ (direct implementation)
DGEMM test: PASSED
Performance: 0.49 GFLOPS
Time: 4066 ms

# MKL with Fortran (native BLAS call)
DGEMM test: PASSED
Performance: 2.80 GFLOPS
Time: 714 ms

# OpenBLAS with C++ (direct implementation)
DGEMM test: PASSED
Performance: 0.47 GFLOPS
Time: 4102 ms

# OpenBLAS with Fortran (native BLAS call)
DGEMM test: PASSED
Performance: 2.74 GFLOPS
Time: 730 ms

# Mixed C++/Fortran with MKL
Fortran DGEMM:
  Result test: PASSED
  Performance: 2.78 GFLOPS
  Time: 719 ms

C++ DGEMM:
  Result test: PASSED
  Performance: 0.48 GFLOPS
  Time: 4080 ms

Results comparison:
  Fortran and C++ results match: YES
  Speedup (Fortran vs C++): 5.67x
```

Notice that Fortran implementation is typically much faster (5-6x) than the C++ version due to the way Fortran compilers optimize numeric operations and the direct binding to BLAS libraries.