#ifndef BLAS_TESTING_CBLAS_HELPER_H
#define BLAS_TESTING_CBLAS_HELPER_H

// This header provides CBLAS compatibility for different backends

#ifdef HAVE_MKL
  #include <mkl.h>

  // If MKL is used but dsecnd isn't found, provide our own implementation
  #ifndef MKL_HAVE_DSECND
  #include <ctime>
  inline double dsecnd() {
    static clock_t start = clock();
    return static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
  }
  #endif

#elif defined(HAVE_CBLAS)
  #include <cblas.h>
#else
  // Basic CBLAS definitions for when using standard BLAS
  #define CblasRowMajor 101
  #define CblasColMajor 102
  #define CblasNoTrans 111
  #define CblasTrans 112
  #define CblasConjTrans 113

  extern "C" {
    // DGEMM prototype
    void dgemm_(char* transa, char* transb, int* m, int* n, int* k,
               double* alpha, double* a, int* lda, double* b, int* ldb,
               double* beta, double* c, int* ldc);
  }

  // CBLAS-style wrapper for standard BLAS
  inline void cblas_dgemm(int Order, int TransA, int TransB,
                         int M, int N, int K,
                         double alpha, const double *A, int lda,
                         const double *B, int ldb, double beta,
                         double *C, int ldc) {
    char transA = (TransA == CblasNoTrans) ? 'N' : 'T';
    char transB = (TransB == CblasNoTrans) ? 'N' : 'T';

    if (Order == CblasColMajor) {
      dgemm_(&transA, &transB, &M, &N, &K, &alpha,
             const_cast<double*>(A), &lda, const_cast<double*>(B), &ldb, &beta, C, &ldc);
    } else {
      // For row major, swap the order of matrices and transpose operations
      dgemm_(&transB, &transA, &N, &M, &K, &alpha,
             const_cast<double*>(B), &ldb, const_cast<double*>(A), &lda, &beta, C, &ldc);
    }
  }
#endif

#endif // BLAS_TESTING_CBLAS_HELPER_H
