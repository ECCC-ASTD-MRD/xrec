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
      SUBROUTINE XAUTOLAB(IFMTXX,IFMTYY,NUMXX,NUMYY,IXORR)
      IMPLICIT NONE
      INTEGER IXORR,NUMXX,NUMYY
      CHARACTER(LEN=*) IFMTXX,IFMTYY
*
***S/P AUTOLAB - CETTE ROUTINE DONNE A L'USAGER LA POSSIBILITE DE
*                CHOISIR L'ORIENTATION ET LE FORMAT
*                UTILISE POUR ANNOTER LES GRAPHIQUES
*
*AUTEUR   - C. THIBEAULT  AVR 84
*
*LANGAGE  - FORTRAN
*
*OBJET(AUTOLAB)
*         - AUTOLAB PERMET DE CHOISIR L'ORIENTATION (AXE X) ET LE
*           FORMAT UTILISE POUR ANNOTER LES GRAPHIQUES
*
*LIBRAIRIES
*         - SOURCE  AUTOSRC
*         - OBJET   RMNLIB5,ID=RMNP
*
*ARGUMENTS
*         - IFMTXX  - FORMAT POUR NUMEROTER L'AXE X
*         - IFMTYY  - FORMAT POUR NUMEROTER L'AXE Y
*         - NUMXX   - NOMBRE DE CARACTERES UTILISE PAR LE FORMAT
*                     POUR NUMEROTER L'AXE X
*         - NUMYY   - NOMBRE DE CARACTERES UTILISE PAR LE FORMAT
*                     POUR NUMEROTER L'AXE Y
*         - IXORR   - ORIENTATION DES CARACTERES SUR L'AXE X
*
*IMPLICITES
*
*
*---------------------------------------------------------------------
*
      INTEGER ISZXAX,ISZYAX,ISZXLB,ISZYLB,ISZCRV,ISZTYT,NUMX,
     1        NUMY,IXOR,IYOR
      COMMON /XCHARSZ/ ISZXAX,ISZYAX,ISZXLB,ISZYLB,ISZCRV,ISZTYT,
     1                NUMX,NUMY,IXOR
      CHARACTER(LEN=40) LABX,LABY,LTIT
      CHARACTER(LEN=16) LDASHC(26)
      CHARACTER(LEN=10) LSYM(26)
      CHARACTER(LEN=8)  IFMTX,IFMTY
      COMMON /XAUTOG3/ LABX,LABY,LTIT,LDASHC,LSYM,IFMTX,IFMTY
*
      IFMTX  = IFMTXX
      IFMTY  = IFMTYY
      NUMX   = NUMXX
      NUMY   = NUMYY
      IXOR   = IXORR
*
      RETURN
      END
