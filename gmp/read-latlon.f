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
      subroutine opllfl(unite, nomfich, code)
      integer unite, code
      character*128 nomfich
      OPEN(UNIT=unite,FILE=nomfich,
     %     FORM='UNFORMATTED',IOSTAT=IER,STATUS='OLD')
      REWIND(unite)
      code = ier
      return
      end
      subroutine rdllfl(unite, npts, maxlat, minlat, maxlon,
     %                  minlon, pts, nptsmx)
      integer unite, npts, nptsmx
      real maxlat, minlat, maxlon, minlon, pts(nptsmx)
      READ(unite,end=999) NPTS,MAXLAT,MINLAT,MAXLON,MINLON,
     $     (PTS(I),I=1,NPTS)
      return
 999  print *,'READ ERROR'
      npts = 0
      call clllfl(unite)
      return
      end
      subroutine clllfl(unite)
      integer unite
 100  CLOSE(unite,IOSTAT=IER)
      return
      END
