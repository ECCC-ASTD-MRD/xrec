*/* RMNLIB - Library of useful routines for C and FORTRAN programming
* * Copyright (C) 1975-2001  Division de Recherche en Prevision Numerique
* *                          Environnement Canada
* *
* * This library is free software; you can redistribute it and/or
* * modify it under the terms of the GNU Lesser General Public
* * License as published by the Free Software Foundation,
* * version 2.1 of the License.
* *
* * This library is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* * Lesser General Public License for more details.
* *
* * You should have received a copy of the GNU Lesser General Public
* * License along with this library; if not, write to the
* * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* * Boston, MA 02111-1307, USA.
* */
      subroutine initrlx
      IMPLICIT NONE
      external defvar
      external setitem
      external xrecgrille
      external vecteur
      real dum
      integer i
	integer npar
	common /cdefvar/ npar
       character*8 qlxcon(12)
       integer qlxval(12)
       common /rg/ qlxval,qlxcon
       data qlxcon(1)  / 'NORD'/
       data qlxval(1)   / 1    /
       data qlxcon(2)  / 'SUD'/
       data qlxval(2)   / 2    /
       data qlxcon(3)  / 'GLOBAL'/
       data qlxval(3)   / 0    /
       data qlxcon(4)  / 'STD'/
       data qlxval(4)   / 1    /
       data qlxcon(5)  / 'LATLON'/
       data qlxval(5)   / 2   /
       data qlxcon(6)  / 'PS'/
       data qlxval(6)   / 3    /
       data qlxcon(7)  / 'GAUSS'/
       data qlxval(7)   / 5    /
       data qlxcon(8)  / 'STDB'/
       data qlxval(8)   / 6    /
       data qlxcon(9)  / 'TAPE1'/
       data qlxval(9)   / 7    /
       data qlxcon(10)  / 'GEF'/
       data qlxval(10)  / 9    /
       data qlxcon(11)  / 'STEREO'/
       data qlxval(11)  / 8    /
       data qlxcon(12)  / 'GRIB'/
       data qlxval(12)  / 10    /
      call qlxopt('CARMOT', 4)
      do i=1,12
        call qlxins(qlxval(i),qlxcon(i), dum, 1, 0)
      enddo
      call qlxinx(defvar,     'DEFVAR', npar, 0730, 2)
      call qlxinx(setitem,    'SETITEM',npar, 0303, 2)
      call qlxinx(xrecgrille, 'GRILLE', npar, 0509, 2)
      call qlxinx(vecteur,    'VECTEUR',npar, 0303, 2)
      return
      end
      subroutine rlx(nomFich)
      implicit none
      character*128 nomFich
      integer fnom, fclos, ier
      external fnom, fclos
      integer ipose,kend, iun
      iun = 133
      iun = 0
      ier = fnom(iun, nomFich, 'SEQ+OLD+R/O+FMT', 0)
      kend = 0
      ipose = 0
      call readlx(iun, kend, ipose)
      ier = fclos(iun)
      return
      end
