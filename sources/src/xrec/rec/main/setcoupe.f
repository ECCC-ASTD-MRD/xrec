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
      subroutine setprof1(nivmin,nivmax,echelle,titrex,titrey)
      implicit none
      real nivmin, nivmax
      integer echelle
      character*(*) titrex, titrey
      call xsysdat
      CALL XDISPLA(0,2,echelle)
      CALL XANOTAT(titrex,titrey,1,3,0,0,' ')
      if (nivmax.le.1.0) then
         CALL XAUTOLAB('(F8.2)','(F5.3)',8,8,0)
      else
         CALL XAUTOLAB('(F8.2)','(F8.2)',8,8,0)
      endif
      CALL XAUTOSET("SZTY", 15)
      CALL XAUTOSET("SZXA", 8)
      CALL XAUTOSET("SZYA", 8)
      CALL XAUTOSET("SZXL", 10)
      CALL XAUTOSET("SZYL", 10)
      CALL XAUTOSET("SZCR", 10)
      return
      end
      subroutine setprof2()
      implicit none
      CALL XDISPLA(0,2,0)
      CALL XANOTAT(' ',' ',4,0,0,0,' ')
      return
      end
