#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

// Include direct matrix multiplication implementation
#include "../../direct_dgemm.h"

// Simple timing function
double get_time() {
  static clock_t start = clock();
  return static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
}

int main() {
    std::cout << "OpenBLAS DGEMM Example" << std::endl;

    // Matrix dimensions
    int m = 1000, n = 1000, k = 1000;
    int lda = m, ldb = k, ldc = m;

    // Parameters
    char transa = 'N';
    char transb = 'N';
    double alpha = 1.0;
    double beta = 0.0;

    // Allocate matrices
    std::vector<double> A(m * k, 1.0);
    std::vector<double> B(k * n, 2.0);
    std::vector<double> C(m * n, 0.0);

    // Measure performance
    double start_time = get_time();

    // Call DGEMM: C = alpha*A*B + beta*C using direct computation
    std::cout << "Using direct matrix multiplication (OpenBLAS version)" << std::endl;
    direct_dgemm(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    double end_time = get_time();

    // Verify result (C should be filled with values = k*alpha*A[0]*B[0])
    double expected = k * alpha * 1.0 * 2.0;
    bool correct = true;
    for (int i = 0; i < 10 && correct; ++i) {
        if (std::abs(C[i] - expected) > 1e-6) {
            correct = false;
        }
    }

    std::cout << "DGEMM test: " << (correct ? "PASSED" : "FAILED") << std::endl;
    std::cout << "Performance: " << (2.0 * m * n * k) / ((end_time - start_time) * 1e9) << " GFLOPS" << std::endl;
    std::cout << "Time: " << (end_time - start_time) * 1000 << " ms" << std::endl;

    return 0;
}
