  subroutine f_convip( ip, p, sorte, mode, chaine, flag,longueur)
  implicit none
  integer ip,sorte,mode,longueur
  real p
  character*15 chaine
  character*15 local_string
  logical flag
  integer i
  do i=1,15
     chaine(i:i) = ' '
  enddo
  call convip_plus(ip, p, sorte, mode, local_string, flag)
  if (flag) then
    chaine(1:15) = local_string(1:15)
  endif
  return
  end
