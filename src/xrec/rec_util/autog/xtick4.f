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
***S/P XTICK4      TRACE LES DIVISIONS DANS LES ROUTINE PERIM-PERIML-
*                 GRIDAL ET HALFAX
*
      SUBROUTINE XTICK4 (MGRX,MNRX,MGRY,MNRY)
*
*AUTEUR   SYSTEME NCAR
*
*REVISION 001  RPN  M.VALIN / C.THIBEAULT  1-9-78  REFAIT DOCUMENTATION
*
*LANGAGE  FORTRAN
*
*OBJET(TICK4)
*         TICK4 PERMET DE CONTROLLER LA LONGUEUR DES LIGNES DE DIVISION
*         POUR LES ROUTINES PERIM-PERIML-GRIDAL ET HALFAX.  LES DIVISIO
*         MAJEURES POUR L'AXE X ET Y SONT INITIALISEES A 12 ET LES
*         MINEURES A 8.
*
*DIRECTIVES(TICK4)
*
*LIBRAIRIES
*         SOURCE  NEWPLOT106PL,ID=CMCULIB        DECK=TICK4
*         OBJET   NCARSYSOB,ID=CMCULIB
*
*
*ARGUMENTS
*         MGRX ET MGRY
*                 LA LONGUEUR EN UNITES DE TRACEUR DES DIVISIONS
*                 MAJEURES SUR L'AXE X ET Y.
*         MNRX ET MNTY
*                 LA LONGUEUR EN UNITES DE TRACEUR DES DIVISIONS
*                 MINEURES SUR L'AXE X ET Y.
*
*IMPLICITES
*         BLOC /SYSPL1/  ***** POUR UNE EXPLICATION DES ARGUMENTS DANS
*                              LE BLOC COMMON SYSPL1 VOIR LA ROUTINE
*                              SYSDAT *****
*
*PRECISION
*         SIMPLE
*
*----------------------------------------------------------------------
*
*     implicit none
      INTEGER SAVUNIT,SEGUNIT,SEGSAVU,PIPEFLG
      LOGICAL MTCLOS,FLCLOS,SAVCLOS,SEGCLOS,SSCLOS,PASDEF
*
      COMMON /XSYSPL1/ MMAJX   ,MMAJY   ,MMINX   ,MMINY
      COMMON /XSYSPL1/ MXLAB   ,MYLAB   ,MFLG   ,MTYPE
      COMMON /XSYSPL1/ MXA   ,MYA   ,MXB   ,MYB
      COMMON /XSYSPL1/ MX   ,MY   ,MTYPEX   ,MTYPEY
      COMMON /XSYSPL1/ XXA   ,YYA   ,XXB   ,YYB
      COMMON /XSYSPL1/ XXC   ,YYC   ,XXD   ,YYD
      COMMON /XSYSPL1/ XFACTR   ,YFACTR   ,XADD   ,YADD
      COMMON /XSYSPL1/ MUMX   ,MUMY   ,MSIZX   ,MSIZY
      COMMON /XSYSPL1/ MXDEC   ,MYDEC   ,MXOR   ,MOP(30,-1:32)
      COMMON /XSYSPL1/ MXOLD   ,MYOLD   ,MXMAX   ,PASDEF(30)
      COMMON /XSYSPL1/ MYMAX   ,MXFAC   ,MYFAC   ,MODEF
      COMMON /XSYSPL1/ MF2ER   ,MSHFTX   ,MSHFTY
      COMMON /XSYSPL1/ MMGRX   ,MMGRY   ,MMNRX   ,MMNRY
      COMMON /XSYSPL1/ MFRCNT   ,MJXMIN   ,MJYMIN   ,MJXMAX
      COMMON /XSYSPL1/ MJYMAX   ,SMALL   ,XX   ,YY
      COMMON /XSYSPL1/ IVAR   ,IMUNIT   ,IPUNIT
      COMMON /XSYSPL1/ NBITS   ,INDEX   ,MBUF(90)
      COMMON /XSYSPL1/ IWIDE(0:3) ,IWCUR   ,PIPEFLG
      COMMON /XSYSPL1/ IFLSHNT   ,IWCURF   ,IWCURS   ,SAVUNIT
      COMMON /XSYSPL1/ MTCLOS   ,FLCLOS   ,SAVCLOS   ,SEGCLOS
      COMMON /XSYSPL1/ IWCSEG   ,SEGUNIT   ,SSCLOS   ,SEGSAVU
      INTEGER MOPRIN(30)
      EQUIVALENCE (MOPRIN(1), MOP(1,-1))
*
*----------------------------------------------------------------------
*
*         MMGRX(Y) EST LA LONGUEUR DES DIVISIONS MAJEURES DANS LA
*         DIRECTION X(Y) ET EST UTILISE SUR L'AXE Y(X). (CECI POUR ETRE
*         CONSISTANT AVEC MX(Y) DEC. DE LABMOD).
*
*         MGRX(Y) EST LA LONGUEUR DES DIVISIONS MAJEURES DE L'AXE X(Y)
*         LA MEME CHOSE S'APPLIQUE POUR MMRX(Y) ET MNRX(Y).
*
      MMGRX=IABS(MGRY)
      MMGRX=ishft(iand(ishft(-1,-(MSHFTX)),MMGRX),MSHFTX)
      MMGRX=ISIGN(MMGRX,MGRY)
*
      MMGRY=IABS(MGRX)
      MMGRY=ishft(iand(ishft(-1,-(MSHFTY)),MMGRY),MSHFTY)
      MMGRY=ISIGN(MMGRY,MGRX)
*
      MMNRX=IABS(MNRY)
      MMNRX=ishft(iand(ishft(-1,-(MSHFTX)),MMNRX),MSHFTX)
      MMNRX=ISIGN(MMNRX,MNRY)
*
      MMNRY=IABS(MNRX)
      MMNRY=ishft(iand(ishft(-1,-(MSHFTY)),MMNRY),MSHFTY)
      MMNRY=ISIGN(MMNRY,MNRX)
*
*
      RETURN
      END
