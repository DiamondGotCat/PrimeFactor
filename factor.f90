subroutine factorize(n, factors, num_factors)
  implicit none
  integer(8), intent(in) :: n
  integer(8), intent(out) :: factors(100)
  integer, intent(out) :: num_factors
  integer(8) :: i, value
  integer :: j

  ! 値のコピーを作成して n を変更しないようにする
  value = n
  num_factors = 0

  if (value <= 1) then
     print *, 'Input must be greater than 1.'
     return
  end if

  i = 2
  do while (value > 1)
    if (mod(value, i) == 0) then
      value = value / i
      num_factors = num_factors + 1
      factors(num_factors) = i
    else
      i = i + 1
    end if
  end do
end subroutine factorize
