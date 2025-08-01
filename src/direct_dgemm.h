#ifndef DIRECT_DGEMM_H
#define DIRECT_DGEMM_H

#include <vector>
#include <iostream>

// A direct implementation of matrix multiplication
// that doesn't rely on external BLAS libraries
inline void direct_dgemm(int m, int n, int k, 
                  double alpha, const std::vector<double>& A, int lda,
                  const std::vector<double>& B, int ldb,
                  double beta, std::vector<double>& C, int ldc) {
    // C = alpha*A*B + beta*C
    // A is m x k, B is k x n, C is m x n
    // In column-major format:
    // A(i,j) = A[i + j*lda] for 0 <= i < m, 0 <= j < k
    // B(i,j) = B[i + j*ldb] for 0 <= i < k, 0 <= j < n
    // C(i,j) = C[i + j*ldc] for 0 <= i < m, 0 <= j < n

    // First apply beta to C
    if (beta != 1.0) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < m; i++) {
                C[i + j*ldc] *= beta;
            }
        }
    }

    // Then compute alpha*A*B and add to C
    for (int j = 0; j < n; j++) {        // For each column of C
        for (int i = 0; i < m; i++) {    // For each row of C
            double sum = 0.0;
            for (int p = 0; p < k; p++) {  // Dot product of A row and B column
                // A(i,p) * B(p,j)
                sum += A[i + p*lda] * B[p + j*ldb];
            }
            C[i + j*ldc] += alpha * sum;
        }
    }
    // Debug: Print first few elements of result
    #ifdef DEBUG_DGEMM
    std::cout << "First few elements of C after direct_dgemm:" << std::endl;
    for (int i = 0; i < std::min(5, m); i++) {
        for (int j = 0; j < std::min(5, n); j++) {
            std::cout << C[i + j*ldc] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Expected value: " << k * alpha * 1.0 * 2.0 << std::endl;
    #endif
}

#endif // DIRECT_DGEMM_H
