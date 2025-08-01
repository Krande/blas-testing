module fortran_dgemm_wrapper
  use, intrinsic :: iso_fortran_env, only: dp => real64
  implicit none

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

    ! Debug printouts
    print *, 'Fortran wrapper received m =', m, 'n =', n, 'k =', k
    print *, 'alpha =', alpha, 'beta =', beta

    ! Get start time
    call cpu_time(start_time)

    ! Print debug info before DGEMM call
    print *, 'Calling DGEMM with', m, 'x', k, ' * ', k, 'x', n, ' matrices'

    ! Print some values from input matrices
    print *, 'A(1,1) =', a(1,1), 'B(1,1) =', b(1,1)
    print *, 'Expected result should be around:', k * alpha * a(1,1) * b(1,1)

    ! Call DGEMM: C = alpha*A*B + beta*C
    call dgemm('N', 'N', m, n, k, alpha, a, lda, b, ldb, beta, c, ldc)

    ! Print debug info after DGEMM call
    print *, 'DGEMM call completed successfully'

    ! Print some values from result matrix
    print *, 'C(1,1) =', c(1,1), 'C(2,2) =', c(2,2)
    print *, 'C(1,2) =', c(1,2), 'C(2,1) =', c(2,1)

    ! Get end time
    call cpu_time(end_time)
    elapsed_time = end_time - start_time
  end subroutine fortran_dgemm_wrapper_func

end module fortran_dgemm_wrapper
