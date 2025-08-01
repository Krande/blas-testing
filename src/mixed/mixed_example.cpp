#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <iomanip>

// Include direct matrix multiplication implementation
#include "../direct_dgemm.h"

// Ensure cout flushes immediately
#define FLUSH std::cout.flush()

// Declare the Fortran function
extern "C" {
    void fortran_dgemm_wrapper_func(int m, int n, int k, double alpha, 
                                  const double* a, int lda, 
                                  const double* b, int ldb,
                                  double beta, double* c, int ldc,
                                  double* elapsed_time);
}

// Simple timing function for C++ part
double get_time() {
    static clock_t start = clock();
    return static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
}

int main() {
    // Disable buffering on cout
    std::cout.setf(std::ios::unitbuf);

    std::cout << "Mixed C++/Fortran DGEMM Example" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    FLUSH;

    // Matrix dimensions
    int m = 1000, n = 1000, k = 1000;
    int lda = m, ldb = k, ldc = m;

    // Parameters
    double alpha = 1.0;
    double beta = 0.0;

    // Allocate matrices in C++
    std::vector<double> A(m * k, 1.0);
    std::vector<double> B(k * n, 2.0);
    std::vector<double> C_fortran(m * n, 0.0);
    std::vector<double> C_cpp(m * n, 0.0);

    // ===== Run Fortran DGEMM =====
    double fortran_time = 0.0;

    // Call the Fortran wrapper function
    fortran_dgemm_wrapper_func(m, n, k, alpha, A.data(), lda, B.data(), ldb, 
                             beta, C_fortran.data(), ldc, &fortran_time);

    // ===== Run C++ DGEMM =====
    double cpp_start_time = get_time();

    // Use our direct implementation in C++
    direct_dgemm(m, n, k, alpha, A, lda, B, ldb, beta, C_cpp, ldc);

    double cpp_end_time = get_time();
    double cpp_time = cpp_end_time - cpp_start_time;

    // ===== Verify results =====
    double expected = k * alpha * 1.0 * 2.0;
    bool fortran_correct = true;
    bool cpp_correct = true;

    for (int i = 0; i < 10; ++i) {
        if (std::abs(C_fortran[i] - expected) > 1e-6) {
            fortran_correct = false;
            break;
        }
        if (std::abs(C_cpp[i] - expected) > 1e-6) {
            cpp_correct = false;
            break;
        }
    }

    // Also check if Fortran and C++ results match each other
    bool results_match = true;
    for (int i = 0; i < 10; ++i) {
        if (std::abs(C_fortran[i] - C_cpp[i]) > 1e-6) {
            results_match = false;
            break;
        }
    }

    // ===== Print results =====
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n\nReady to display results..." << std::endl;
    FLUSH;

    // Fortran results
    std::cout << "\nFortran DGEMM:" << std::endl;
    FLUSH;
    std::cout << "  Result test: " << (fortran_correct ? "PASSED" : "FAILED") << std::endl;
    std::cout << "  Performance: " << (2.0 * m * n * k) / (fortran_time * 1e9) << " GFLOPS" << std::endl;
    std::cout << "  Time: " << fortran_time * 1000.0 << " ms" << std::endl;
    FLUSH;

    // C++ results
    std::cout << "\nC++ DGEMM:" << std::endl;
    std::cout << "  Result test: " << (cpp_correct ? "PASSED" : "FAILED") << std::endl;
    std::cout << "  Performance: " << (2.0 * m * n * k) / (cpp_time * 1e9) << " GFLOPS" << std::endl;
    std::cout << "  Time: " << cpp_time * 1000.0 << " ms" << std::endl;
    FLUSH;

    // Comparison
    std::cout << "\nResults comparison:" << std::endl;
    std::cout << "  Fortran and C++ results match: " << (results_match ? "YES" : "NO") << std::endl;
    std::cout << "  Speedup (Fortran vs C++): " << (cpp_time / fortran_time) << "x" << std::endl;
    FLUSH;

    std::cout << "\nMixed C++/Fortran DGEMM Example completed successfully." << std::endl;
    FLUSH;

    return 0;
}
