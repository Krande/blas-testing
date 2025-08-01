#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

#ifdef HAVE_MKL
  #include <mkl.h>
#else
  // Use standard BLAS interface
  extern "C" {
    void dgemm_(char* transa, char* transb, int* m, int* n, int* k,
               double* alpha, double* a, int* lda, double* b, int* ldb,
               double* beta, double* c, int* ldc);
  }

  // Replacement for MKL's dsecnd function
  double dsecnd() {
    static clock_t start = clock();
    return static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
  }
#endif

int main() {
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
    double start_time = dsecnd();

    // Call DGEMM: C = alpha*A*B + beta*C
    dgemm_(&transa, &transb, &m, &n, &k, &alpha,
           A.data(), &lda, B.data(), &ldb, &beta, C.data(), &ldc);

    double end_time = dsecnd();

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
