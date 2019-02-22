  subroutine f_convip( ip, p, sorte, mode, chaine, flag,longueur)
  implicit none
  integer ip,sorte,mode,longueur
  real p
  character*16 chaine
  character*16 local_string
  logical flag
  integer i
  do i=1,16
     chaine = '                '
  enddo
  call convip_plus(ip, p, sorte, mode, local_string, flag)
  if (flag) then
    chaine(1:16) = local_string(1:16)
  endif
  return
  end
