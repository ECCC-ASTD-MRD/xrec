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
      subroutine xrecgrille(gridname,gridtype,ni,nj,ig1,ig2,ig3,ig4,ig5)
      IMPLICIT NONE
      integer gridname(8)
      integer i, gridtype, ni, nj, ig1, ig2, ig3, ig4, ig5
      integer argdims
      external argdims
      character(len=32) cgridname
      write (cgridname, '(8a4)') (gridname(i),i=1,argdims(1))
      if (ig5.eq.2) then
         call c_xrecgrille(cgridname, 4,ni,nj,ig1,ig2,ig3,ig4)
      else
         call c_xrecgrille(cgridname, gridtype,ni,nj,ig1,ig2,ig3,ig4)
      endif
      return
      end
