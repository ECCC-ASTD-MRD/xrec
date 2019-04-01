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
      SUBROUTINE XBREAK(X,F,I,NSIG)
      IMPLICIT NONE
      INTEGER I,NSIG
      REAL F,X
*
***S/P BREAK - NORMALISATION D'UN NOMBRE
*
*AUTEUR   - M. VALIN
*
*LANGAGE  - FORTRAN
*
*OBJET(BREAK)
*           BREAK NORMALISE UN NOMBRE (X). LE RESULTAT (F) EST
*           ENTRE 1. ET 9.99999
*
*LIBRAIRIES
*         - SOURCE  AUTOSRC/ARMNSRC
*         - OBJET   RMNLIB5,ID=RMNP
*
*ARGUMENTS
*  IN     - X    NOMBRE A CONVERTIR
*  OUT    - F    RESULTAT NORMALISE ENTRE 1. ET 9.9999999
*  OUT    - I    PUISSANCE DE 10 DE RENORMALISATION
*  IN     - NSIG IABS(NSIG)=NOMBRE DE CHIFFRES SIGNIFICATIFS
*
*-------------------------------------------------------------
*
      INTEGER ND
*
      ND=MAX0(0,MIN0(12,IABS(NSIG))-1)
      I=ALOG10(ABS(X))
      F=X*.1**I
      IF(F.GT.1.0) GOTO 10
      I=I-1
      F=F*10.
10    F=AINT(F*10.**ND+.5)/(10.**ND)
      IF(F.LT.10.)GOTO 20
      I=I+1
      F=F*.1
20    RETURN
      END
