module fortran_dgemm_wrapper
  use, intrinsic :: iso_fortran_env, only: dp => real64
  implicit none

  ! Interface for the C++ callable Fortran subroutine
  interface
    subroutine fortran_dgemm_wrapper_func(m, n, k, alpha, a, lda, b, ldb, beta, c, ldc, elapsed_time) &
               bind(c, name="fortran_dgemm_wrapper_func")
      use, intrinsic :: iso_c_binding
      integer(c_int), value :: m, n, k, lda, ldb, ldc
      real(c_double), value :: alpha, beta
      real(c_double), intent(in) :: a(lda, *), b(ldb, *)
      real(c_double), intent(inout) :: c(ldc, *)
      real(c_double), intent(out) :: elapsed_time
    end subroutine fortran_dgemm_wrapper_func
  end interface

contains

  ! Implementation of the wrapper function
  subroutine fortran_dgemm_wrapper_func(m, n, k, alpha, a, lda, b, ldb, beta, c, ldc, elapsed_time) &
             bind(c, name="fortran_dgemm_wrapper_func")
    use, intrinsic :: iso_c_binding
    integer(c_int), value :: m, n, k, lda, ldb, ldc
    real(c_double), value :: alpha, beta
    real(c_double), intent(in) :: a(lda, *), b(ldb, *)
    real(c_double), intent(inout) :: c(ldc, *)
    real(c_double), intent(out) :: elapsed_time
    real(dp) :: start_time, end_time

    ! Get start time
    call cpu_time(start_time)

    ! Call DGEMM: C = alpha*A*B + beta*C
    call dgemm('N', 'N', m, n, k, alpha, a, lda, b, ldb, beta, c, ldc)

    ! Get end time
    call cpu_time(end_time)
    elapsed_time = end_time - start_time
  end subroutine fortran_dgemm_wrapper_func

end module fortran_dgemm_wrapper
