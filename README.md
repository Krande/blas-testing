# Blas testing
# BLAS Testing with Various Backends

This project demonstrates how to use BLAS libraries with different backends (MKL, OpenBLAS, etc.) in C++ and Fortran applications.

## Requirements

- pixi package manager
- CMake 3.10+
- C++ compiler (tested with MSVC 19.43 on Windows)
- Fortran compiler (tested with LLVMFlang 19.1.7 on Windows)

The project is configured to work with different compilers and platforms through the pixi environment system.

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

### Latest Results (Windows 10, MSVC 19.43 / LLVMFlang 19.1.7)

```
# MKL with C++ (direct implementation)
DGEMM test: PASSED
Performance: 0.486855 GFLOPS
Time: 4108 ms

# MKL with Fortran (native BLAS call)
DGEMM test: PASSED
Performance: 31.25 GFLOPS
Time: 64 ms

# OpenBLAS with C++ (direct implementation)
DGEMM test: PASSED
Performance: 0.492005 GFLOPS
Time: 4065 ms

# OpenBLAS with Fortran (native BLAS call)
DGEMM test: PASSED
Performance: 400.0 GFLOPS
Time: 5 ms

# Mixed C++/Fortran with MKL
Fortran DGEMM:
  Result test: PASSED
  Performance: 142.857143 GFLOPS
  Time: 14.000000 ms

C++ DGEMM:
  Result test: PASSED
  Performance: 3.389831 GFLOPS
  Time: 590.000000 ms

Results comparison:
  Fortran and C++ results match: YES
  Speedup (Fortran vs C++): 42.142857x

# Mixed C++/Fortran with OpenBLAS
Fortran DGEMM:
  Result test: PASSED
  Performance: 666.666667 GFLOPS
  Time: 3.000000 ms

C++ DGEMM:
  Result test: PASSED
  Performance: 3.454231 GFLOPS
  Time: 579.000000 ms

Results comparison:
  Fortran and C++ results match: YES
  Speedup (Fortran vs C++): 193.000000x
```

### Analysis

1. **Native BLAS vs Direct Implementation**: The results show a dramatic difference between using the native BLAS libraries through Fortran and our direct C++ implementation. The optimized BLAS routines are 42-193x faster.

2. **MKL vs OpenBLAS**: In these tests, OpenBLAS showed better performance than MKL for this specific DGEMM operation. The OpenBLAS Fortran implementation achieved up to 666 GFLOPS compared to MKL's 143 GFLOPS.

3. **Consistent C++ Performance**: Our direct C++ implementation performance is consistent regardless of the backend environment (~3.4 GFLOPS), as it doesn't actually use the backend libraries.

4. **Compiler Impact**: These results were obtained using Microsoft Visual C++ (MSVC 19.43) for C++ and LLVM Flang 19.1.7 for Fortran on Windows 10. Different compilers and platforms may yield different performance characteristics.