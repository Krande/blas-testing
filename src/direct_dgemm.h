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

    // First apply beta to C
    if (beta != 1.0) {
        for (int i = 0; i < m * n; i++) {
            C[i] *= beta;
        }
    }

    // Then compute alpha*A*B and add to C
    // Column-major layout: C(i,j) = C[i + j*ldc]
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            double sum = 0.0;
            for (int p = 0; p < k; p++) {
                sum += A[i + p*lda] * B[p + j*ldb];
            }
            C[i + j*ldc] += alpha * sum;
        }
    }
}

#endif // DIRECT_DGEMM_H
