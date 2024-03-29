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
      subroutine put2to3(fld2d,fld3d,ni,nj,nk,index)
      implicit none
      real fld2d(ni,nj), fld3d(ni,nj,nk)
      integer ni,nj,nk,index
      integer i,j
      do  i=1,ni
         do  j=1,nj
            fld3d(i,j,index) = fld2d(i,j)
         enddo
      enddo
      return
      end
***********************************************************************
      subroutine put3to2(fld2d,fld3d,ni,nj,nk,index)
      implicit none
      real fld2d(ni,nj), fld3d(ni,nj,nk)
      integer ni,nj,nk,index
      integer i,j
      do i=1,ni
         do j=1,nj
            fld2d(i,j) = fld3d(i,j, index)
         enddo
        enddo
        return
        end
***********************************************************************
      subroutine swpgrid(outfld, infld, ni, nj)
      implicit none
      real outfld(ni,nj), infld(ni, nj)
      integer ni, nj
      integer i,j
      do  j=1,nj
         do  i=1, ni
            outfld(i,nj-j+1)=infld(i,j)
         enddo
      enddo
      do  j=1,nj
         do  i=1, ni
            infld(i,j)=outfld(i,j)
         enddo
      enddo
      return
      end
***********************************************************************
      subroutine chkgrid(fld, ni, nj, grtyp, orient)
      implicit none
      real fld(ni,nj),fld2(ni,nj)
      integer ni, nj
      character*1 grtyp
      integer orient
      if (grtyp.eq.'A'.or.grtyp.eq.'B'.or.grtyp.eq.'G') then
         if (orient.eq.1) then
            call swpgrid(fld2, fld, ni, nj)
            orient = 0
         endif
      endif
      return
      end
***********************************************************************
      subroutine aminmax(rmin, rmax, fld, ni, nj)
      implicit none
      real rmin, rmax, fld(ni,nj)
      integer i, j, ni, nj
      real armin, armax
      rmin = fld(1, 1)
      rmax = fld(1, 1)
      do i=1, ni
         do j=1,nj
            if (fld(i,j).lt.rmin) then
               rmin = fld(i,j)
            endif
            if (fld(i,j).gt.rmax) then
               rmax = fld(i,j)
            endif
         enddo
      enddo
      return
      end
***********************************************************************
      SUBROUTINE VMINMAX(aMIN, aMAX, uu,vv, NI, NJ)
      implicit none
      REAL amIN, aMAX, uu(ni,nj),vv(ni,nj)
      INTEGER i,j,NI, NJ
      REAL temp
      amin = uu(1,1)*uu(1,1)+vv(1,1)*vv(1,1)
      amax = amin
      do j=1,nj
         do i=1,ni
            temp = uu(i,j)*uu(i,j)+vv(i,j)*vv(i,j)
            if (temp.lt.amin) then
               amin = temp
            endif
            if (temp.gt.amax) then
               amax = temp
            endif
         enddo
      enddo
      amin = sqrt(amin)
      amax = sqrt(amax)
      RETURN
      END
***********************************************************************
      SUBROUTINE SMINMAX(MIN, MAX, FLD, NI, NJ, IDEBUT,
     * JDEBUT, IFIN, JFIN)
      implicit none
      REAL MIN, MAX, FLD(NI,NJ)
      INTEGER I, J, NI, NJ, IDEBUT, JDEBUT, IFIN, JFIN
      MIN = FLD(IDEBUT, JDEBUT)
      MAX = FLD(IDEBUT, JDEBUT)
      DO  J=JDEBUT,JFIN
         DO  I=IDEBUT, IFIN
            IF (FLD(I,J).LT.MIN) THEN
               MIN = FLD(I,J)
            ENDIF
            IF (FLD(I,J).GT.MAX) THEN
               MAX = FLD(I,J)
            ENDIF
         enddo
      enddo
      RETURN
      END
*     
***********************************************************************
      subroutine vsminmax(rmin, rmax, uu, vv, ni, nj, idebut, jdebut, if
     %in, jfin)
      implicit none
      real rmin, rmax, rmodule, uu(ni,nj),vv(ni,nj)
      integer ni, nj, idebut, jdebut, ifin, jfin, i,j
      rmin = uu(idebut, jdebut)*uu(idebut,jdebut)+vv(idebut,jdebut)*vv(i
     %debut,jdebut)
      rmax = rmin
      do  j=jdebut,jfin
         do  i=idebut, ifin
            rmodule = uu(i,j)*uu(i,j)+vv(i,j)*vv(i,j)
            if (rmodule.lt.rmin) then
               rmin = rmodule
            endif
            if (rmodule.gt.rmax) then
               rmax = rmodule
            endif
         enddo
      enddo
      rmin = sqrt(rmin)
      rmax = sqrt(rmax)
      return
      end
*     
***********************************************************************
      subroutine sminmax2(min, max, min2, max2, fld, ni, nj, idebut, jde
     %but, ifin, jfin)
      implicit none
      real min, max, min2, max2, fld(ni,nj)
      integer i,j, ni, nj, idebut, jdebut, ifin, jfin
      call sminmax(min, max, fld, ni, nj, idebut, jdebut, ifin, jfin)
      min = fld(idebut, jdebut)
      max = fld(idebut, jdebut)
      min2 = min
      max2 = max
      do  j=jdebut,jfin
         do i=idebut, ifin
            if (fld(i,j).lt.min) then
               min2 = min
               min = fld(i,j)
            endif
            if (fld(i,j).gt.max) then
               max2 = max
               max = fld(i,j)
            endif
         enddo
      enddo
      max2 = min
      min2 = max
      do  j=jdebut,jfin
         do i=idebut, ifin
            if (fld(i,j).gt.max2.and.fld(i,j).lt.max) then
               max2 = fld(i,j)
            endif
            if (fld(i,j).lt.min2.and.fld(i,j).gt.min) then
               min2 = fld(i,j)
            endif
         enddo
      enddo
      return
      end
*     
*     
* 
* 
***********************************************************************
      subroutine sminmax_ij(rmin, rmax, imin, jmin, imax, jmax, fld, ni,
     % nj, idebut, jdebut, ifin, jfin)
      implicit none
      integer i,j,ni, nj, idebut, jdebut, ifin, jfin, imin, jmin, imax, 
     %jmax
      real rmin, rmax, fld(ni,nj)
      imin = max(1,idebut)
      jmin = max(1,jdebut)
      imax = imin
      jmax = jmin
      rmin = fld(imin, jmin)
      rmax = fld(imax, jmax)
      print *, rmin, rmax, imin, imax, jmin, jmax, ni, nj
      do  j=max(1,jdebut),min(nj,jfin)
         do  i=max(1,idebut), min(ni,ifin)
            if (fld(i,j).lt.rmin) then
               rmin = fld(i,j)
               imin = i
               jmin = j
            endif
            if (fld(i,j).gt.rmax) then
               rmax = fld(i,j)
               imax = i
               jmax = j
            endif
         enddo
      enddo
      print *, rmin, rmax, imin, imax, jmin, jmax, ni, nj
      print *, '!!!---@@@----------------------------'
      return
      end
*     
*     
* 
************************************************************************
      subroutine smeanvar(mean, stddev, fld, ni, nj, idebut, jdebut, ifi
     %n, jfin)
      implicit none
      integer i,j,npts,ni, nj, idebut, jdebut, ifin, jfin, imin, jmin, i
     %max, jmax
      real mean, stddev, fld(ni,nj)
* Utiliser une variable 'sum' en real*8 afin d'avoir une meilleure
* précision (mantisse plus grande) lors de l'addition de toutes les
* valeurs du champ. Ceci évite que les valeurs additionnées ne soient
* plus significatives lorsque la somme devient grande.
      real*8 sum
* Calculer la moyenne
      sum = 0.0d0
      npts = 0
      do  j=max(1,jdebut),min(nj,jfin)
         do  i=max(1,idebut), min(ni,ifin)
            sum = sum + fld(i,j)
            npts = npts + 1
         enddo
      enddo
      mean = sum / float(npts)
* Calculer l'écart-type
      sum = 0.0d0
      do  j=max(1,jdebut),min(nj,jfin)
         do  i=max(1,idebut), min(ni,ifin)
            sum = sum + (fld(i,j)-mean)*(fld(i,j)-mean)
         enddo
      enddo
      stddev = dsqrt(sum / float(npts))
      return
      end
*     
*     
* 
************************************************************************
      subroutine vsminmax_ij(rmin, rmax, imin, jmin, imax, jmax, uu,vv, 
     %ni, nj, idebut, jdebut, ifin, jfin)
      implicit none
      real rmin, rmax, rmodule, uu(ni,nj), vv(ni,nj)
      integer i,j,ni, nj, idebut, jdebut, ifin, jfin, imin, jmin, imax, 
     %jmax
      imin = max(1,idebut)
      jmin = max(1,jdebut)
      imax = imin
      jmax = jmin
      rmodule = uu(imin, jmin)*uu(imin, jmin)+vv(imin, jmin)*vv(imin, jm
     %in)
      rmin = rmodule
      rmax = rmin
      do  j=max(1,jdebut),min(nj,jfin)
         do  i=max(1,idebut), min(ni,ifin)
            rmodule = uu(i,j)*uu(i,j)+vv(i,j)*vv(i,j)
            if (rmodule.lt.rmin) then
               rmin = rmodule
               imin = i
               jmin = j
            endif
            if (rmodule.gt.rmax) then
               rmax = rmodule
               imax = i
               jmax = j
            endif
         enddo
      enddo
      rmin = sqrt(rmin)
      rmax = sqrt(rmax)
      return
      end
*     
************************************************************************
      subroutine uvmeanvar(mean, stddev, uu,vv, ni, nj, idebut, jdebut, 
     %ifin, jfin)
      implicit none
      integer i,j,npts,ni, nj, idebut, jdebut, ifin, jfin
      real mean, stddev, uu(ni,nj),vv(ni,nj),rmodule
* Utiliser une variable 'sum' en real*8 afin d'avoir une meilleure
* précision (mantisse plus grande) lors de l'addition de toutes les
* valeurs du champ. Ceci évite que les valeurs additionnées ne soient
* plus significatives lorsque la somme devient grande.
      real*8 sum
* Calculer la moyenne
      sum = 0.0d0
      npts = 0
      do  j=max(1,jdebut),min(nj,jfin)
         do  i=max(1,idebut), min(ni,ifin)
            rmodule = uu(i,j)*uu(i,j)+vv(i,j)*vv(i,j)
            sum = sum + rmodule
            npts = npts + 1
         enddo
      enddo
      mean = sum / float(npts)
* Calculer l'écart-type
      sum = 0.0d0
      do  j=max(1,jdebut),min(nj,jfin)
         do  i=max(1,idebut), min(ni,ifin)
            rmodule = uu(i,j)*uu(i,j)+vv(i,j)*vv(i,j)
            sum = sum + (rmodule-mean)*(rmodule-mean)
         enddo
      enddo
      stddev = dsqrt(sum / float(npts))
      return
      end
*     
*     min, jmin, imax, jmax
* 
************************************************************************
*     
* 
**************************************************************
* 
* 
* S/R SMOOTH
* 
* 
* 
* 
***********************************************************************
      SUBROUTINE SMOOTH(MATIJ,MATMN,MDIM,NDIM,IDIM,JDIM,IFAC,JFAC,
     *                  id, jd, if, jf)
      IMPLICIT NONE
* 
* 
*      CETTE SOUS ROUTINE FAIT L'EXPANSION D'UN 
*      CHAMP (MATIJ) A UN CHAMP (MATMN) VIA UNE 
*      INTERPOLATION BI-LINEAIRE. LES FACTEURS
*      D'EXPANSION SONT NOMMES (IFAC ET JFAC) 
* 
* 
* 
* 
* -ARGUMENTS     -OUT 
*                        -MATIJ        :MATRICE INTERPOLEE
* 
*                -IN
*                        -MATMN        :MATRICE INITIALE
*                        -MDIM,NDIM    :DIMENSIONS DE MATMN 
* 
* 
*                        -IDIM,JDIM    :DIMENSIONS DE MATIJ 
* 
*                        -IFAC,JFAC    :FACTEURS D EXPANSION
*                                        EN X ET Y
* 
* 
*     N.B.              IDIM= 1 + IFAC*(MDIM-1) 
*                       JDIM= 1 + JFAC*(NDIM-1) 
* 
* 
**************************************************************
* 
      REAL I1V(1000),I2V(1000)
      REAL SLOPIV(1000)
      REAL SLOPJ,FIJ1,FIJ2
* 
      INTEGER I,J,N
      INTEGER IDIM,JDIM,MDIM,NDIM
      integer id, jd, if, jf
      INTEGER II1,JJ1,II,JJ
      INTEGER IFAC,JFAC
      INTEGER IIM1,II1P1,JJ1P1
      REAL MATMN(MDIM,NDIM),MATIJ(IDIM,JDIM)
* 
* 
      DO 33 I=id, if
        II1=1+(I-1)/IFAC
        II=I-IFAC*(II1-1)
        IIM1=II-1
        II1P1=II1+1
        IF(II1P1.GT.MDIM) II1P1=MDIM
* OCCURS WHEN I=IDIM AND FORCES SLOPIV(N) = 0 
      IF(II.NE.1) GOTO 42
        DO 38 N=1,NDIM
          I1V(N)=MATMN(II1,N)
          I2V(N)=MATMN(II1P1,N)
* GET NORTH-SOUTH SLOPES
          SLOPIV(N)=(I2V(N)-I1V(N))/FLOAT(IFAC)
 38     CONTINUE
* 
* THE FOLLOWING STATEMENT SAVES THE NEED TO SAY 
* IF(JJ1P1.GT.NDIM) JJ1P1=NDIM INSIDE THE MUCH LARGER DO 22 LOOP
* 
      SLOPIV(NDIM+1)=SLOPIV(NDIM)
      I1V(NDIM+1)=I1V(NDIM)
* 
 42   CONTINUE
      DO 22 J=jd, jf
        JJ1=1+(J-1)/JFAC
        JJ=J-JFAC*(JJ1-1)
        IF(JJ.NE.1) GOTO 43
        FIJ1=I1V(JJ1)+SLOPIV(JJ1)*IIM1
        JJ1P1=JJ1+1
        FIJ2=I1V(JJ1P1)+SLOPIV(JJ1P1)*IIM1
* GET EAST-WEST SLOPE 
        SLOPJ=(FIJ2-FIJ1)/JFAC
 43     CONTINUE
* 
        MATIJ(I,J)=FIJ1+SLOPJ*(JJ-1)
* 
 22   CONTINUE
 33   CONTINUE
      RETURN
      END
***********************************************************************
      subroutine chkdims(fld, buffer, ni, nj, nk, ninjnk)
      implicit none
      real fld(ni, nj, nk), buffer(ninjnk)
      integer ni, nj, nk, ninjnk
      if (nk.eq.1) then
         return
      endif
      if (ni.eq.1) then
         call rdgrid(fld, buffer, ni, nj, nk, nj, nk)
      else
         call rdgrid(fld, buffer, ni, nj, nk, ni, nk)
      endif
      return
      end
***********************************************************************
      subroutine rdgrid(fld, buffer, ni, nj, nk, newni, newnj)
      implicit none
      real fld(ni, nj, nk), buffer(newni, newnj)
      integer ni, nj, nk, newni, newnj
      integer i, j, k
      if (ni.eq.1) then
         do 100 j=1,nj
            do 100 k=1, nk
 100           buffer(j,k) = fld(1,j,k)
      endif
      if (nj.eq.1) then
         do 200 i=1,ni
            do 200 k=1, nk
 200           buffer(i,k) = fld(i,1,k)
      endif
      return
      end
***********************************************************************
      subroutine grdval(val, fld, i, j, ni, nj)
      implicit none
      integer i, j, ni, nj
      real val, fld(ni, nj)
      val = fld(i, j)
      return
      end
***********************************************************************
      subroutine modulus(module,uu,vv,npts)
      implicit none
      integer npts
      real module(npts),uu(npts),vv(npts)
      integer i
      do i=1,npts
         module(i)=sqrt(uu(i)*uu(i)+vv(i)*vv(i))
      enddo
      return
      end
***********************************************************************
      subroutine modulus3d(module,uu,vv,ww,npts)
      implicit none
      integer npts
      real module(npts),uu(npts),vv(npts),ww(npts)
      integer i
      do i=1,npts
         module(i)=sqrt(uu(i)*uu(i)+vv(i)*vv(i)+ww(i)*ww(i))
      enddo
      return
      end
