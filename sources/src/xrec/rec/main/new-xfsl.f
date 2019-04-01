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
      integer function xfslouv(nomfich, iun, ttlrecs, winind, typesel)
      implicit none
      integer ttlrecs,ntmrecs
      character*(*) nomfich
      integer iun, winind, typesel
      integer ligne,maxrecs,maxdes
      parameter (ligne  = 51)
      parameter (maxrecs= 64)
      parameter (maxdes = 8)
      character*12 idents
      character*51 tableau
      common /xfscinf/ idents(maxdes), tableau(0:maxrecs)
      integer, pointer, dimension(:) :: liste
      integer table, nrecs, nbrecs, nbdes
      common /xfsiinf/ table(3,maxdes),
     *                 liste, nrecs, nbrecs, nbdes
      character*2 nomvar
      character*1 typvar, grtyp
      character*8 etiket
      character*160  titre
      integer key, date0, deet, npas, ni, nj, nk, nbits, datyp
      integer ip1, ip2, ip3, swa, lng, dltf, ubc
      integer ig1, ig2, ig3, ig4, extra1, extra2, extra3
      integer fstinf, fstprm, fstsui
      integer fnom, fstfrm, res
      integer xselouv, xseloup, xselins, xselouf
      integer i, j, inf
      integer m, jour, an, heure, r
      character*4 mois(0:12), month(0:12), mm
      integer ier, ulng, getulng
      external getulng
      data (mois(i),i=0,12) /'*** ', 'Jan ','Fev ','Mar ','Avr ',
     *                       'Mai ','Juin','Juil','Aout',
     *                       'Sep ','Oct ','Nov ','Dec ' /
      data (month(i),i=0,12) /'***', 'Jan ','Feb ','Mar ','Apr ',
     *                        'May ','Jun ','Jul ','Aug ',
     *                        'Sep ','Oct ','Nov ','Dec ' /
      ulng = getulng()
*     
      nbdes = 7
      call initid(idents, maxdes)
      call inittab(tableau, table, ligne)
      write(titre, 5) nomfich
*    
*    
      res = xseloup(titre, ttlrecs, idents, nbdes, winind, typesel)
      if (ttlrecs.eq.0) then
         res = xselins(table,ttlrecs)
         goto 100
      else
         allocate(liste(ttlrecs))
      endif
*     
      i = 0
      key = fstinf(iun, ni, nj, nk,  -1, ' ', -1, -1, -1, ' ', ' ')
      if (key.lt.0) goto 100
      i = i+1
*     
*     
      inf = fstprm(key, date0, deet, npas, ni, nj, nk, nbits,
     *             datyp, ip1, ip2, ip3, typvar, nomvar, etiket, grtyp,
     *             ig1, ig2, ig3, ig4, swa, lng, dltf, ubc,
     *             extra1, extra2, extra3)
*     
*     
      m = date0 / 10000000
      if (ulng.eq.0) then
         mm = mois(m)
      else
         mm = month(m)
      endif
      jour = mod((date0 / 100000), 100)
      an   = mod((date0 / 1000), 100)
      heure = mod((date0 / 10), 100)
      r     = mod(date0, 10)
      if (ulng.eq.0.and.m.ge.6.and.m.le.8) then
         write(tableau(mod(i-1,64)), 11) nomvar, typvar, ip1, ip2, ip3,
     *         etiket, jour, mm, an, heure
      else
         write(tableau(mod(i-1,64)), 12) nomvar, typvar, ip1, ip2, ip3,
     *         etiket, jour, mm, an, heure
      endif
      liste(i-1) = key
      if (ttlrecs.le.1) then
         res = xselins(table,ttlrecs)
      endif
 50   if (key.lt.0) goto 100
      i = i + 1
*    
*    
      key = fstsui(iun, ni, nj, nk)
      if (key.lt.0) goto 100
      inf = fstprm(key, date0, deet, npas, ni, nj, nk, nbits,
     *     datyp, ip1, ip2, ip3, typvar, nomvar, etiket, grtyp,
     *     ig1, ig2, ig3, ig4, swa, lng, dltf, ubc,
     *     extra1, extra2, extra3)
      m     = date0 / 10000000
      if (ulng.eq.0) then
         mm = mois(m)
      else
         mm = month(m)
      endif
      jour  = mod((date0 / 100000), 100)
      an    = mod((date0 / 1000), 100)
      heure = mod((date0 / 10), 100)
      r     = mod(date0, 10)
      if (ulng.eq.0.and.m.ge.6.and.m.le.8) then
         write(tableau(mod(i-1,64)), 11) nomvar, typvar, ip1, ip2, ip3,
     *         etiket, jour, mm, an, heure
      else
         write(tableau(mod(i-1,64)), 12) nomvar, typvar, ip1, ip2, ip3,
     *         etiket, jour, mm, an, heure
      endif
      liste(i-1) = key
      ntmrecs = mod(i,64)
      if (ntmrecs.eq.0) then
         ntmrecs = 64
      endif
      if (0.eq.mod(i,64).or.i.eq.ttlrecs) then
         res = xselins(table,ntmrecs)
      endif
      goto 50
 100  continue
      res = xselouf(table, ntmrecs)
      xfslouv = winind
 2    format(40a)
 4    format(3i16)
 5    format(128a)
 6    format(40a)
 10   format(a2,4x, a1, 4x, i5, 2x, i5, x, i4,
     *       x, a8, 2x, i9.9)
 11   format(a2,4x, a1, 4x, i5, 2x, i5, x, i4,
     *       x, a8, 2x, i2.2,a4,i2.2,'-',i2.2,'Z')
 12   format(a2,4x, a1, 4x, i5, 2x, i5, x, i4,
     *       x, a8, 2x,i2.2,a3,i2.2,'-',i2.2,'Z')
      return
      end
c     ****************************************************************
c     **                                                            **
c     ****************************************************************
      integer function xfslfer(winind)
      implicit none
      integer winind
      integer xselfer
      integer i, inf, res
      xfslfer = xselfer(winind)
      return
      end
c     ****************************************************************
c     **                                                            **
c     ****************************************************************
      integer function xfslact(slkeys, nslkeys, winind)
      implicit none
      integer nslkeys
      integer slkeys(nslkeys), winind
      integer ligne,maxrecs,maxdes
      parameter (ligne  = 51)
      parameter (maxrecs= 64)
      parameter (maxdes = 8)
      character*12 idents
      character*51 tableau
      common /xfscinf/ idents(maxdes), tableau(0:maxrecs)
      integer table, listbuf, nrecs, nbrecs, nbdes
      integer, pointer, dimension(:) :: liste
      common /xfsiinf/ table(3,maxdes),
     *                 liste, nrecs, nbrecs, nbdes
      integer xselact
      integer i, inf, res
*     
*     
      xfslact = xselact(slkeys, nslkeys, winind)
      do 200 i=1, nslkeys
         slkeys(i) = liste(slkeys(i)-1)
 200  continue
      return
      end
c     ****************************************************************
c     **                                                            **
c     ****************************************************************
      subroutine initid(idents)
      implicit none
      character*12 idents(*)
      integer i, j, ulng
      integer  getulng
      external getulng
      ulng = getulng()
      if (ulng.eq.0) then
         idents(1) =  'Champ'
         idents(2)  = 'Type'
         idents(3)  = 'Niveau'
         idents(4)  = 'Heure'
         idents(5)  = 'IP3'
         idents(6)  = 'Etiquette'
         idents(7) =  'Date'
      else
         idents(1) =  'Field'
         idents(2)  = 'Type'
         idents(3)  = 'Level'
         idents(4)  = 'Time'
         idents(5)  = 'IP3'
         idents(6)  = 'Stamp    '
         idents(7) =  'Date'
      endif
      return
      end
*****************************************************************
      subroutine inittab(tableau, table, len)
      implicit none
      character*51 tableau(*)
      integer table(3, *)
      integer len
      table(1,1)   = 6
      table(2,1)   = len
      table(3,1)   = loc(tableau(1))
      table(1,2)   = 5
      table(2,2)   = len
      table(3,2)    = loc(tableau(1))+6
      table(1,3)   = 7
      table(2,3)   = len
      table(3,3)   = loc(tableau(1))+11
      table(1,4)   = 6
      table(2,4)   = len
      table(3,4)   = loc(tableau(1))+18
      table(1,5)   = 5
      table(2,5)   = len
      table(3,5)   = loc(tableau(1))+24
      table(1,6)   = 10
      table(2,6)   = len
      table(3,6)   = loc(tableau(1))+29
      table(1,7)  = 11
      table(2,7)  = len
      table(3,7)  = loc(tableau(1))+39
      return
      end
c     ****************************************************************
      integer function xfslupd(nomfich, iun, ttlrecs, winind, typesel)
      implicit none
      integer ttlrecs,ntmrecs,typesel
      character*(*) nomfich
      integer iun, winind, ier, xfslouv
      integer ligne,maxrecs,maxdes
      parameter (ligne  = 51)
      parameter (maxrecs= 64)
      parameter (maxdes = 8)
      character*12 idents
      character*51 tableau
      common /xfscinf/ idents(maxdes), tableau(0:maxrecs)
      integer table, listbuf, nrecs, nbrecs, nbdes
      integer, pointer, dimension(:) :: liste
      common /xfsiinf/ table(3,maxdes),
     *                 liste, nrecs, nbrecs, nbdes
      if (associated(liste)) then
         deallocate(liste)
      endif
      call xselupd(winind)
      ier = xfslouv(nomfich, iun, ttlrecs, winind, typesel)
      xfslupd = 0
      return
      end
c     ****************************************************************
