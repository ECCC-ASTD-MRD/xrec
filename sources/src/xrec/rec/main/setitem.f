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
      subroutine setitem(panneau,item,valeur)
      IMPLICIT NONE
      integer panneau(8),item(8),valeur(8)
      integer i
      integer argdims
      external argdims
      character*32 cpanneau,citem,cvaleur
      write (cpanneau, '(8a4)') (panneau(i),i=1,argdims(1))
      write (citem,    '(8a4)') (item(i),   i=1,argdims(2))
      write (cvaleur , '(8a4)') (valeur(i),i=1,argdims(3))
      if (cpanneau.eq.'tableau_de_bord'.or.
     $    cpanneau.eq.'control_panel') then
         call c_sctlatr(citem,cvaleur)
      endif
      if (cpanneau.eq.'geographie'.or.
     $     cpanneau.eq.'geography') then
         call c_sgeoatr(citem,cvaleur)
      endif
      if (cpanneau.eq.'animation') then
         call c_sanmatr(citem,cvaleur)
      endif
      if (cpanneau.eq.'contours') then
         call c_sconatr(citem,cvaleur)
      endif
      if (cpanneau.eq.'attributs_divers'.or.
     $    cpanneau.eq.'various_attributes') then
         call c_satratr(citem,cvaleur)
      endif
      if (cpanneau.eq.'coupe'.or.
     $    cpanneau.eq.'xsection') then
         call c_scpeatr(citem,cvaleur)
      endif
      if (cpanneau.eq.'palette') then
         call c_spalatr(citem,cvaleur)
      endif
      return
      end
