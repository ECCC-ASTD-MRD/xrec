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
      SUBROUTINE XULIBER(ISTD,ICHAI)
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
      CHARACTER *(*) ICHAI
*
      WRITE(IPUNIT,100)MOD(ABS(ISTD),1000),ICHAI
100   FORMAT('GRF',I3.3,'-',A80)
*
      RETURN
      END
