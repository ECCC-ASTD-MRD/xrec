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
      program xrec
      implicit none
!
! Revisions: see Changelog
!
      integer maxrecs,newfld,ouvrir,fermer,quitter
      parameter (maxrecs= 128)
      parameter (newfld = 4)
      parameter (ouvrir = 1)
      parameter (fermer = 2)
      parameter (quitter = -1)
      integer xfslfer97
      integer fstouv, fclos, fstfrm, indfich, lenfich, fstopl
      integer nf, oldnf
      integer xconouv, xinit
      integer xfslouv97, xfslact97
      integer xconact, xpandact, xpancact, xpaneact, xpangact, xpancbiac
     %t,
     $     xpanmact ,xpanpaact, xpanpcsac, xpanpact, xpanpavact,
     $     xpanpcpact ,xpanpcsact, xpventact, xpattact, xpanstatact
      integer ier, nrecs
      integer i, inf, lnguser, getulng
      integer fnom, fstopi, ezsetopt
      external fnom, fstopi, ezsetopt
      character*512   nomfich
      character*512     vide
      integer        recs(maxrecs)
      integer        nbrecs
      integer, parameter :: maxfich = 1000
      integer, dimension(maxfich) :: lnkdiun
      common /firstiun/ lnkdiun
      integer nbfich
      integer ipos
      integer typesel
      integer indmin, indmax, level
      character(len=20), dimension(1008) ::   cle
      character(len=256),dimension(1008) :: def, val
      character(len=256),dimension(maxfich) :: nfich
      character(len=256) :: tmpdir,sortiexrec,temporaire
      integer cle_ar, cle_ozsrt, cle_colmin, cle_colmax, cle_v, cle_debu
     %g
      integer cle_nologo, cle_roc
      integer stack_size
      character(len=16) :: ezopt,ezval
      common /ccardprm/ cle, def, val
      logical flag
*
      data vide/' '/
      data cle /1000 * 'IMFLDS:', 'AR',     'OZSRT.',
     *          'COLMIN', 'COLMAX', 'V', 'DEBUG', 'NOLOGO', 'ROC' /
      data def /1000 * 'SCRAP', 'SQUARE', 'CHAMP_EDITE',
     *          '64',     '248', 'OUI', 'OUI','OUI','OUI'         /
      data val /1000 * 'SCRAP', 'SQUARE', 'CHAMP_EDITE',
     *          '64',     '248', 'NON', 'NON','NON','NON' /
      lnkdiun = 0
      cle_ar     = 1001
      cle_ozsrt  = 1002
      cle_colmin = 1003
      cle_colmax = 1004
      cle_v      = 1005
      cle_debug  = 1006
      cle_nologo = 1007
      cle_roc    = 1008
      call qqqnocore
      call wglscon('x')
      stack_size=128
      call setstacksize(stack_size)
      ezopt = 'EXTRAP_DEGREE'
      ezval = 'MAXIMUM'
      ier = ezsetopt(ezopt,ezval)
      ier = xinit('xrec')
      lnguser = getulng()
      if (lnguser.eq.0) then
         nomfich = 'Selecteur d''enregistrements'
         def(cle_ozsrt) = 'champ_edite'
         val(cle_ozsrt) = 'champ_edite'
      else
         nomfich = 'Record Selector'
         def(cle_ozsrt) = 'modified_field'
         val(cle_ozsrt) = 'modified_field'
      endif
      call ccard(cle,def,val, 1008, ipos)
!       do i=1,1000
!          def(i) = trim(def(i))
!          val(i) = trim(val(i))
!          print *, i, len(def(i)),len(val(i))
!       enddo
      nf = 1
      if (val(1).eq.def(1)) then
         call selfic(val, 1000, 512, nf)
      endif
      if (nf.eq.0.or.val(1).eq.vide) then
         print *, 'Aucun fichier demande - No files selected'
         print *, 'Bonne chance - Good Luck'
         stop
      endif
      ier = fstopl('REDUCTION32',.true.,.false.)
      call recopnf(nbrecs, val, def, lnkdiun, nf)
      if (val(cle_v).eq.'NON') then
         call getenv('TMPDIR',tmpdir)
         if (len(tmpdir) == 0) then
            tmpdir='/tmp'
         endif
         sortiexrec = '/sortie-xrec.txt'
         temporaire = tmpdir(1:len_trim(tmpdir)) // sortiexrec
         call rec_redirect(temporaire)
!        temporaire = '/dev/null'
!         ier = fnom(6, temporaire,'SEQ+FTN+FMT',0)
!         if (ier.ne.0) then
!            print *, 'Cannot write into temporary file : ', temporaire
!         endif
         level = 10
         flag = .false.
         ier = fstopi('MSGLVL', level, flag)
      else
         ier = ezsetopt('VERBOSE', 'YES')
      endif
      if (val(cle_debug).eq.'OUI') then
         call wgldbg
      endif
      if (val(cle_nologo).eq.'OUI') then
         call recnologo
      endif
      if (val(cle_roc).eq.'OUI') then
         call wglroc
      endif
      read(val(cle_colmin),*) indmin
      read(val(cle_colmax),*) indmax
*     call rgopti('INTERP', 1, .true.)
      ezopt = 'INTERP_DEGREE'
      ezval = 'LINEAR'
      ier = ezsetopt(ezopt, ezval)
      call setcolors(indmin,indmax)
      call recckar(lnkdiun(1), val(1))
      inf = xconouv(lnkdiun(1))
      typesel = 2
      ier = xfslouv97(nomfich, lnkdiun(1), nbrecs, 1, typesel)
      nrecs = 0
 1000 inf = xconact(recs, nrecs, lnkdiun(1))
      if (inf.eq.ouvrir) then
         oldnf = nf
         call allpdown
         call selfic(val(oldnf+1), 1000-oldnf, 512, nf)
         call allpup
         if (nf.eq.0)  then
            nf=oldnf
         else
            nf = oldnf+nf
            call recclsf(lnkdiun, oldnf)
            if (nf.gt.maxfich) then
               print *, 'Trop de fichiers ouverts'
               stop
            endif
            call recopnf(nbrecs, val(1), def, lnkdiun, nf)
            call xfslupd97(val(1), lnkdiun(1), nbrecs, 1, typesel)
         endif
      endif
      if (inf.eq.fermer) then
         oldnf = nf
         call allpdown
         call xfcl(val(1), nf)
         call allpup
         if (nf.ne.oldnf) then
            call recclsf(lnkdiun, oldnf)
            do 200 i=nf+1,oldnf
 200           val(i) = def(i)
         call recopnf(nbrecs, val, def, lnkdiun, nf)
         call xfslupd97(val(1), lnkdiun(1), nbrecs, 1, typesel)
         endif
      endif
      if (inf.eq.quitter) goto 3000
      ier = xfslact97(recs, nrecs, 1)
      ier = xpanstatact()
      ier = xpandact()
      ier = xpancact()
      ier = xpaneact()
      ier = xpangact()
      ier = xpanmact()
      ier = xpanpaact()
      ier = xpanpcsact()
      ier = xpanpact()
      ier = xpanpavact()
      ier = xpanpcpact()
      ier = xpanpcsact()
      ier = xpventact()
      ier = xpattact()
      ier = xpancbiact()
      goto 1000
 2    format(72a)
 4    format(3i16)
 6    format(72a)
 3000 print *, 'fin d execution normale'
      stop
      end
c     ****************************************************************
c     **                                                            **
c     ****************************************************************
      subroutine recopnf(nbrecs, val, def, lnkdiun, nf)
      implicit none
      integer nbrecs
      character(len=256),dimension(nf) :: val, def
      character(len=256) :: str
      integer tol
      integer lnkdiun(*)
      integer nf
      integer i, ier, niun, nrecs, nrecs2
      integer, dimension(:), allocatable :: liste
      integer fnom, fstouv, fstinl, fstnbr
      integer ni, nj, nk, fstopi
      external fnom, fstouv, fstinl, fstopi, fstnbr
      logical flag
      if (nf.eq.0) then
         nbrecs = 0
         return
      endif
      nf = 1
 33   if (val(nf).ne.def(nf)) then
         nf = nf +1
         goto 33
      endif
      nf = nf -1
      do 34 i=1, nf
         str = trim(val(i))
!         print *, len(str),len_trim(str)
         ier = fnom(lnkdiun(i),str,'STD+RND+OLD+R/O+REMOTE',0)
         if (ier.lt. 0) then
            call messerr(val(i))
            print *, '***********************************************'
            print *, 'rec - fichier ',val(1),' inexistant - au revoir'
            print *, '************************************************'
            stop
         endif
 34   continue
      tol = 10
      flag = .false.
      ier = fstopi('TOLRNC', tol, flag)
      nrecs = 0
      nbrecs = 0
      do i=1,nf
         nrecs = fstouv(lnkdiun(i), 'RND')
         call get_nbrecs_actifs(nrecs2, lnkdiun(i))
         if (nrecs.ne.nrecs2) then
          print *, nrecs, nrecs2
         endif
         if (nrecs.lt.0) then
            call messerr(val(i))
            print *, '**********************************************'
            print *, '* le fichier #',val(i),
     *               'n''est pas standard random'
            print *, '**********************************************'
            stop
         endif
      nbrecs = nbrecs + nrecs2
      enddo
      tol = 6
      ier = fstopi('TOLRNC', tol, flag)
      call fstlnk(lnkdiun, nf)
      allocate (liste(nbrecs))
      ier = fstinl(lnkdiun(1), ni, nj, nk, -1, ' ',-1,-1,-1,' ',' ',
     *             liste, nrecs, nbrecs)
      return
      end
c     ****************************************************************
      subroutine recckar(iun, aspect)
      implicit none
      integer iun
      character*(*) aspect
      integer fstinf
      external fstinf
      external wglkas, wglfsc
      integer ni, nj, nk
      integer key
      integer vrai
      vrai = 1
      key = fstinf(iun, ni, nj, nk,  -1, ' ', -1, -1, -1, ' ', ' ')
*     if (aspect.eq.'SQUARE') then
*        call wglkas(1, 1)
*     endif
      print *, 'recckar', aspect
      if (aspect.eq.'NONE') then
         call wglias(vrai)
*        call wglkas(1, 1)
      endif
      if (aspect.eq.'GRID') then
         if (ni.eq.1) then
            call wglkas(nj, nk)
         endif
         if (nj.eq.1) then
            call wglkas(ni, nk)
         endif
         if (nk.eq.1) then
            call wglkas(ni, nj)
         endif
      endif
      if (aspect.eq.'FULL') then
         call wglfsc
      endif
      return
      end
c     ****************************************************************
      subroutine recclsf(lnkdiun, nf)
      implicit none
      integer lnkdiun(124)
      integer nf
      integer fclos, fstfrm, fstunl
      external fclos, fstfrm, fstunl
      integer i, ier
      if (nf.eq.0) then
         return
      endif
      ier = fstunl()
      do 10 i=1, nf
         ier = fstfrm(lnkdiun(i))
         ier = fclos(lnkdiun(i))
 10   continue
      return
      end
c     ****************************************************************
      subroutine rtnozsrt(filename)
      implicit none
      character *(*) filename
      character(len=20), dimension(1008) ::   cle
      character(len=256),dimension(1008) :: def, val
      common /ccardprm/ cle, def, val
!      print *, val(126)
      filename = val(1002)
      return
      end
c     ****************************************************************
      subroutine allpup
      implicit none
      call xconup
      call xselundim(1)
      return
      end
c     ****************************************************************
      subroutine allpdown
      implicit none
      call xcondown
      call xseldim(1)
      return
      end
c     ****************************************************************
      subroutine get_nbrecs_actifs(nbrecs, iun)
      implicit none
      integer nbrecs, iun, key, fstinf, fstsui, ni,nj,nk
      nbrecs = 0
      key = fstinf(iun, ni, nj, nk,  -1, ' ', -1, -1, -1, ' ', ' ')
      do while (key >= 0)
        nbrecs = nbrecs+1
        key = fstsui(iun,ni,nj,nk)
      end do
      return
      end subroutine get_nbrecs_actifs
      character *128 function product_id_tag()
      implicit none
      product_id_tag='$Id$'
      return
      end
      subroutine get_firstiun(first_iun)
      implicit none
      integer :: first_iun
      integer, parameter :: maxfich = 1000
      integer, dimension(maxfich) :: lnkdiun
      common /firstiun/ lnkdiun
      print *, 'lnkdiun(1)', lnkdiun(1)
      first_iun = lnkdiun(1)
      print *, 'first_iun', first_iun
      return
      end
