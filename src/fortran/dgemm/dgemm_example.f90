program dgemm_example
    use, intrinsic :: iso_fortran_env, only: dp => real64
    implicit none

    ! Matrix dimensions
    integer :: m, n, k, lda, ldb, ldc, i, j
    real(dp) :: alpha, beta
    real(dp), allocatable :: a(:,:), b(:,:), c(:,:)
    real(dp) :: start_time, end_time
    logical :: correct
    real(dp) :: expected, gflops

    ! Set dimensions
    m = 1000
    n = 1000
    k = 1000
    lda = m
    ldb = k
    ldc = m

    ! Parameters
    alpha = 1.0_dp
    beta = 0.0_dp

    ! Allocate matrices
    allocate(a(lda, k))
    allocate(b(ldb, n))
    allocate(c(ldc, n))

    ! Initialize matrices
    a = 1.0_dp
    b = 2.0_dp
    c = 0.0_dp

    ! Get start time
    call cpu_time(start_time)

    ! Call DGEMM: C = alpha*A*B + beta*C
    call dgemm('N', 'N', m, n, k, alpha, a, lda, b, ldb, beta, c, ldc)

    ! Get end time
    call cpu_time(end_time)

    ! Verify result (C should be filled with values = k*alpha*A*B)
    expected = k * alpha * 1.0_dp * 2.0_dp
    correct = .true.

    do j = 1, min(10, n)
        do i = 1, min(10, m)
            if (abs(c(i, j) - expected) > 1.0e-6_dp) then
                correct = .false.
                exit
            end if
        end do
    end do

    ! Calculate GFLOPS
    gflops = (2.0_dp * m * n * k) / ((end_time - start_time) * 1.0e9_dp)

    ! Print results
    print *, 'DGEMM test: ', merge('PASSED', 'FAILED', correct)
    print *, 'Performance: ', gflops, ' GFLOPS'
    print *, 'Time: ', (end_time - start_time) * 1000.0_dp, ' ms'

    ! Free memory
    deallocate(a, b, c)

end program dgemm_example
