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
      SUBROUTINE XEBAR (X,Y,SDY,IGO,SSYM,NSYM,NSIZ)
      IMPLICIT NONE
      INTEGER IGO,NSIZ,NSYM
      REAL    SDY,X,Y
      CHARACTER(LEN=*) SSYM
*
***S/P EBAR  TRACE DES BARRES D'ERREUR
*
*AUTEUR   - SYSTEME NCAR
*
*LANGAGE  - FORTRAN
*
*OBJET(EBAR)
*         - TRACE DES BARRES D'ERREUR ARITHMETIQUE, GEOMETRIQUE
*           OU RELATIVE
*
*LIBRAIRIES
*         - SOURCE  AUTOSRC/ARMNSRC
*         - OBJET   RMNLIB5,ID=RMNP
*
*ARGUMENTS
*         - X   - COORDONNEE X DU POINT DE BASE
*         - Y   - COORDONNEE Y DU POINT DE BASE
*         - SDY - ERREUR SUR Y
*         - IGO - 1 = BARRES D'ERREUR ARITHMETIQUE
*                 2 = BARRES D'ERREUR GEOMETRIQUE
*                 3 = BARRES D'ERREUR RELATIVE
*
*---------------------------------------------------------------------
*
      INTEGER ILBAR,IRBAR,K,MANGLE,MBOT,MDIFF,MTOP,MY,MY1,MY2
      REAL    BOT,TOP
*
      EXTERNAL XFRSTPT
      EXTERNAL XMXMY
      EXTERNAL XPWRIT
      EXTERNAL XVECTOR
*
      DATA K /5/
*
   10 CALL XFRSTPT(X,Y)
      CALL XMXMY (MANGLE,MY)
      IF(SSYM(1:1) .NE. ' ') THEN
      CALL XPWRIT(X,Y,SSYM,NSYM,NSIZ,0,0)
      ENDIF
      GO TO ( 20, 30, 40),IGO
      RETURN
*
* ARITHMETIQUE
*
   20 TOP = Y+SDY
      GO TO  50
*
* GEOMETRIQUE
*
   30 TOP = Y*SDY
      GO TO  50
*
* RELATIVE
*
   40 TOP = Y*(1.+SDY)
   50 CALL XFRSTPT(X,TOP)
      CALL XMXMY (MANGLE,MTOP)
      MDIFF = MTOP-MY
      IF (MDIFF .LT. K) RETURN
      MY1 = MY+K
      CALL XFRSTPT(MANGLE,MY1)
      CALL XVECTOR(MANGLE,MTOP)
      ILBAR = MANGLE-K
      IRBAR = MANGLE+K
      CALL XFRSTPT(ILBAR,MTOP)
      CALL XVECTOR(IRBAR,MTOP)
      GO TO ( 60, 70, 80),IGO
   60 BOT = Y-SDY
      GO TO  90
   70 BOT = Y/SDY
      GO TO  90
   80 BOT = Y * (1.-SDY)
   90 CALL XFRSTPT(X,BOT)
      CALL XMXMY (MANGLE,MBOT)
      IF(IABS(MY-MBOT-MDIFF) .LE. 2) MBOT = MY-MDIFF
      MY2 = MY-K
      CALL XFRSTPT(MANGLE,MY2)
      CALL XVECTOR(MANGLE,MBOT)
      CALL XFRSTPT(ILBAR,MBOT)
      CALL XVECTOR(IRBAR,MBOT)
*
      RETURN
C
      END
