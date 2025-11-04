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
*
***s/p    xlgrd     construit le format hollerith pour numeroter les
*                  axes x ou y d'un graphique.  routine utilisee par
*                  plot80  pour des axes logarithmiques.
*
      subroutine xlgrd (xmin,xmax,xb,xt,mgrx,inrx,ifmt,num,ior)
      IMPLICIT NONE
*
*auteur   systeme ncar
*
*revision 001  rpn  m.valin / c.thibeault  1-4-79  refait documentation
*
*----------------------------------------------------------------------
*
      integer icrt,inrx,ior,ixb,ixt,mgrx,num
      real xb,xmax,xmin,xt
      character(len=8) ifmt
      logical reverse
      real temp
*
      ior = 0
      reverse = .false.
      if (xmin.gt.xmax) then
         reverse = .true.
         temp = xmin
         xmin  = xmax
         xmax = temp
      endif
      ixb = alog10(xmin)+.001
      ixt = alog10(xmax)-.001
      if (xmin .lt. 1.) ixb = ixb-1
      if (xmax .ge. 1.) ixt = ixt+1
      if (ixb .eq. ixt) ixt = ixb+1
      xb = 10.**ixb
      xt = 10.**ixt
      mgrx = (ixt-ixb)/20+1
      inrx = (ixt-ixb)/12*10
      inrx = 9
      mgrx = (ixt-ixb)
      num = 8
      ifmt = '(e8.1)'
      icrt = (ixt-ixb)*96
      if (icrt .gt. 790) ior = 90
*
*----------------------------------------------------------------------
*
10    if (reverse) then
         temp = xb
	 xb = xt
	 xt = temp
      endif
      return
      end
