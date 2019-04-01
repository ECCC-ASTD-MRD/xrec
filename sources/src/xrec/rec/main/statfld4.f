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
***s/r statfld4 - calcule la moyenne, la variance, le minimum et 
*                le maximum d'un champs et imprime le resultat.
*
      subroutine statfld4 (f,champ,no,from,nis,njs,nks,ni,nj,nk,
     $     is,js,ks)
      IMPLICIT NONE
* 
      integer nis,njs,nks,ni,nj,nk,no,is,js,ks
      character*(*) champ , from
      real f(nis,njs,nks)
*
*OBJECT
*     calcule et imprime: la moyenne    (moy)
*                         la variance   (var)
*                         le minimum et le maximum
*     du champ f   
* 
*     arguments:
*         - f       - champ sur lequel on veut faire des statistiques
*         - n       - dimensions du champ f
*         - champ   - identification du champ
*         - no      - compteur 
*         - from    - identification du module d'ou on fait l'appel 
*
*METHOD
*
*EXTERNALS
*
*AUTHOR   Michel Desgagne                   Nov   1992
*
*HISTORY
*
**
      integer i,j,k
      real*8 sum,moy,var,min,max
      integer imin,jmin,kmin,imax,jmax,kmax
c--------------------------------------------------------------------
c
c ** On calcule la moyenne.
c
      sum = 0.0
      do 1 k=1,nk-ks
         do 1 j=1,nj-js
            do 1 i=1,ni-is
         sum = sum + f(i,j,k)
 1    continue
      moy = sum / float(ni*nj*nk)
c
c ** On calcule la variance
c
      sum = 0.0
      do 2 k=1,nk-ks
         do 2 j=1,nj-js
            do 2 i=1,ni-is
               sum = sum + ((f(i,j,k) - moy)*(f(i,j,k) - moy))
 2    continue
      var = sqrt (sum / float(ni*nj*nk))
c
c ** On identifie le minimum et le maximum.
c
      imin = 1
      jmin = 1
      kmin = 1
      imax = 1
      jmax = 1
      kmax = 1
      max = f(1,1,1)
      min = f(1,1,1)
c
      do 3 k=1,nk-ks
         do 3 j=1,nj-js
            do 3 i=1,ni-is
               if (f(i,j,k) .gt. max) then
                  max  = f(i,j,k)
                  imax = i
                  jmax = j
                  kmax = k
               endif
               if (f(i,j,k) .lt. min) then
                  min  = f(i,j,k)
                  imin = i
                  jmin = j
                  kmin = k
               endif
 3    continue
c       
c ** On imprime
c 
      write(6,10) champ,moy,var,imin,jmin,kmin,min,
     $            imax,jmax,kmax,max,from
 10   format (a7,' Mean:',e14.7,'  Var:',e14.7,
     $        '  Min:[(',i3,',',i3,',',i3,') ',
     $        e14.7,']',' Max:[(',i3,',',i3,',',i3,') ',
     $        e14.7,']',a7)
c
c----------------------------------------------------------------
      return
      end
