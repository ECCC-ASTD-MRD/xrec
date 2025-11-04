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
***********************************************************************
*
*  objet:    c_wglpfton
*  fonction: Affichage en couleurs d'un champ par lignes de pixels
*  auteur:   Yves Chartier, fevrier 1992
*  Arguments:
*    fld:            champ bi-dimensionnel contenant les valeurs a contourer
*    ni, nj:         dimensions du champ
*    mdeb,ndeb:      coordonnees (gauche,bas)  de la sous-fenetre a afficher
*    mfin,nfin:      coordonnees (droite,haut) de la sous-fenetre a afficher
*    interv:         liste d'intervalles de contours
*    nbInterv:       nombre d'intervalles dans la liste
*    facteur:        ordre de grandeur du champ
*    rmin,rmax:      valeurs min et max du champ
*    ctable:         table d'indices de couleurs
*    ncol:           nombre de couleurs dans la table
*    flagint:        drapeau indiquant si la routine devra s'informer
*                    d'une demande eventuelle d'annulation
*    lissfac:        niveau de lissage demande
*
***********************************************************************
      subroutine wglrfton(z,ni,nj,mdeb,ndeb,mfin,nfin,interv,nbinterv,
     2                   facteur,rmin,rmax,ctable,ncol,flagint,lissfac)
      implicit none
      integer ni,nj,mdeb,ndeb,mfin,nfin
      real z(ni,nj)
      integer ncol, nbinterv
      real interv(nbinterv), facteur, rmin, rmax
      integer ctable(0:ncol-1)
      integer flagint, lissfac
      integer i,j,itmpfld,itmppos
      integer, dimension(:), allocatable :: tmpind
      real, dimension(:), allocatable :: tmpfld, tmppos
       real cmin, cmax, delta
      real x1,y1,x2,y2, dx, dy
      integer ix1, iy1
      real rx1,ry1,rx2,ry2
      integer oldindc, indc, rowl
      integer ideb, jdeb, ifin, jfin
      integer iptp, iptr
      integer wglanul
      external wglanul
      real zlin
      zlin(x1,x2,dx) = x1 + (x2 - x1)* dx
      if (rmin.eq.rmax)  then
         call wglcol(15)
         call wglclr
         return
      endif
      cmin = rmin
      cmax = rmax
      ideb = (mdeb-1)*lissfac+1
      ifin = (mfin-1)*lissfac+1
      jdeb = (ndeb-1)*lissfac+1
      jfin = (nfin-1)*lissfac+1
      rowl = lissfac * (mfin - mdeb) + 1
      allocate(tmpfld(ideb:ifin))
      allocate(tmppos(ideb:ifin))
      allocate(tmpind(ideb:ifin))
!       call memoirh(tmpfld, itmpfld, rowl)
!       call memoirh(tmppos, itmppos, rowl)
!       call memoirh(tmpind, itmp, rowl)
      if (interv(1).ne.0.0) then
         call ajusminmax(cmin,cmax,facteur,interv)
      endif
      delta = cmax - cmin
      if (lissfac.ne.1) then
         do i=ideb, ifin
            tmppos(i) = real(i-1)/lissfac+1
         enddo
      else
         do i=ideb, ifin
           tmppos(i) = real(i)
         enddo
      endif
      do j=jfin,jdeb,-1
         if ((0.eq.mod(j,32)).and.(1.eq.flagint)) then
            if (1.eq.wglanul()) return
         endif
         y1 = real(j-1)/lissfac+1
         iy1 = max(1, min(ifix(y1),nj-1))
         dy = y1 - real(iy1)
         if (lissfac.ne.1) then
            do  i=ideb,ifin
               x1 = i
               ix1 = max(1,min(ifix(x1),ni-1))
               dx = x1 - real(ix1)
               tmpfld(i) = zlin(zlin(z(ix1,iy1),z(ix1+1,iy1),dx),
     2              zlin(z(ix1,iy1+1),z(ix1+1,iy1+1), dx),
     3              dy)
            enddo
         else
            do i=ideb,ifin
               tmpfld(i) = z(i,j)
            enddo
         endif
         call wglcalcols(tmpind, tmpfld, rowl,
     2                   cmin, delta, interv, nbinterv, facteur,ncol)
         oldindc = tmpind(ideb)
         iptp = tmppos(ideb)
         x1 = tmppos(ideb) - 0.5/lissfac
         y1 = y1 - 0.5/lissfac
         y2 = y1 + 1.0/lissfac
         do 20 i=ideb, ifin
            x2 = tmppos(i) + 0.5/lissfac
            indc = tmpind(i)
            if (ctable(indc).ne.ctable(oldindc)) then
               call wglcol(ctable(oldindc))
               call xy2fxfy(rx1,ry1,x1,y1)
               call xy2fxfy(rx2,ry2,x2,y2)
               call wglrfx(rx1,ry1,rx2,ry2)
               oldindc = indc
               x1 = x2 - 1.0/lissfac
            endif
 20      continue
         call wglcol(ctable(oldindc))
          call xy2fxfy(rx1,ry1,x1,y1)
          call xy2fxfy(rx2,ry2,x2,y2)
         call wglrfx(rx1,ry1,rx2,ry2)
      enddo
      deallocate(tmpind)
      deallocate(tmppos)
      deallocate(tmpfld)
!       call memoirh(tmpind, itmp, 0)
!       call memoirh(tmppos, itmppos, 0)
!       call memoirh(tmpfld, itmpfld, 0)
      return
      end
