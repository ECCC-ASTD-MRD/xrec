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
      subroutine defvar(nom, id, unites, palette, scale, inddef,
     *                  int1,  int2,  int3,  int4,  int5,  int6,
     *                  int7,  int8,  int9,  int10, int11, int12,
     *                  int13, int14, int15, int16, int17, int18,
     *                  int19, int20, int21, int22, int23, int24)
      implicit none
      integer npar
      common /cdefvar/ npar
      integer nom, id(24), unites(8), palette(8)
      real  scale
      integer inddef
      real  int1(32), int2(32), int3(32), int4(32), int5(32), int6(32)
      real  int7(32), int8(32), int9(32),int10(32),int11(32),int12(32)
      real int13(32),int14(32),int15(32),int16(32),int17(32),int18(32)
      real int19(32),int20(32),int21(32),int22(32),int23(32),int24(32)
      real tabint(24,32)
      integer ntabint(24)
      character(len=4) cnom
      character(len=64) cid
      character(len=32) cunites
      character(len=32) cpalette
      integer argdims
      external argdims
      integer i,j
      integer ntab, ioffset
      write (cnom, '(a4)') nom
      write (cid, '(24a4)') (id(i),i=1,argdims(2))
      write (cunites, '(8a4)') (unites(i),i=1,argdims(3))
      write (cpalette, '(8a4)') (palette(i),i=1,argdims(4))
      do 10 i=1,24
         ntabint(i) = 0
         do 20 j=1,32
            tabint(i,j) = 0.0
 20      continue
 10   continue
      ntab = npar - 6
      ioffset = 6
      i = 1
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int1(j)
         enddo
      endif
      i = 2
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int2(j)
         enddo
      endif
      i = 3
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int3(j)
         enddo
      endif
      i = 4
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int4(j)
         enddo
      endif
      i = 5
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int5(j)
         enddo
      endif
      i = 6
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int6(j)
         enddo
      endif
      i = 7
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int7(j)
         enddo
      endif
      i = 8
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int8(j)
         enddo
      endif
      i = 9
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int9(j)
         enddo
      endif
      i = 10
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int10(j)
         enddo
      endif
      i = 11
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int11(j)
         enddo
      endif
      i = 12
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int12(j)
         enddo
      endif
      i = 13
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int13(j)
         enddo
      endif
      i = 14
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int14(j)
         enddo
      endif
      i = 15
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int15(j)
         enddo
      endif
      i = 16
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int16(j)
         enddo
      endif
      i = 17
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int17(j)
         enddo
      endif
      i = 18
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int18(j)
         enddo
      endif
      i = 19
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int19(j)
         enddo
      endif
      i = 20
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int20(j)
         enddo
      endif
      i = 21
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int21(j)
         enddo
      endif
      i = 22
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int22(j)
         enddo
      endif
      i = 23
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int23(j)
         enddo
      endif
      i = 24
      if ((i + ioffset).le.npar) then
         ntabint(i) = argdims(i+ioffset)
         do j=1,ntabint(i)
            tabint(i,j) = int24(j)
         enddo
      endif
      call initvar(cnom, cid, cunites, cpalette,
     *             scale, inddef, tabint, ntabint)
      return
      end
