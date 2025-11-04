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
*
***S/P  XEZXY      FAIT PARTIE D'UN ENSEMBLE DE ROUTINES POUR TRACER ET
*             ANNOTER DES COURBES
*
      SUBROUTINE XEZXY (X,Y,NPTS,LABGG)
      IMPLICIT NONE
*
*AUTEUR   SYSTEME NCAR
*
*REVISION 001  RPN  M.VALIN / C.THIBEAULT  1-7-78  REFAIT DOCUMENTATION
*
*LANGAGE  FORTRAN
*
*IMPLICITES
      INTEGER IBAC,ISET,NDASH,LFRAME,IROW,LTYPE,LNDASH
      COMMON /XAUTOG1/ IBAC,ISET,NDASH,LFRAME,IROW,LTYPE,LNDASH(26)
      CHARACTER(LEN=40) LABX,LABY,LTIT
      CHARACTER(LEN=16) LDASHC(26)
      CHARACTER(LEN=10) LSYM(26)
      CHARACTER(LEN=8)  IFMTX,IFMTY
      COMMON /XAUTOG3/ LABX,LABY,LTIT,LDASHC,LSYM,IFMTX,IFMTY
*
*NOTES    POUR PLUS DE DETAIL SUR LA DOCUMENTATION, LA DESCRIPTION DES
*         PARAMETRES ET LEUR UTILISATION VOIR LES ROUTINES EZY ET PLOT8
*
*----------------------------------------------------------------------
*
      INTEGER IHGP,IXGRD,L,NC,NPTS,NSET,NX,NY
      REAL            Y(1)       ,X(1)
      CHARACTER(LEN=*) LABGG
      EXTERNAL PLOT80
*
**---------------------------------------------------------------------
*
*     POUR TRACER UNE COURBE IROW DOIT ETRE POSITIF,  I.E. LE DATA EST
*      ENREGISTRE PAR COLONNE
*
      IF (IROW .LT. 0) IROW = -IROW
*
*     ABS(IXGRD) = 2  LE CHAMP X EXISTE
*
      IXGRD = 2
*
*     IXGRD EST NEGATIF SI LE DATA EST ENREGISTRE PAR RANGEE (IROW.LT.0
*
      IF (IROW .LT. 0) IXGRD = -IXGRD
*
*     PREPARATION POUR UNE COURBE
*
      L = NPTS
      NX = 1
      NY = 1
      NC = 1
*
*     LABG CONTIENT LE TITRE DU GRAPHIQUE INITIALIZER PAR DEFAUT.
*     LE TITRE DONNE PAR L'USAGER VA DANS LTIT SI LABGG .NE.0
*
      IF (LABGG .NE. ' ')  THEN
        LTIT = LABGG
      ELSE
        LTIT = '$'
      ENDIF
*
*     PREPARATION POUR LE FOND (GRIDAL ET LES TITRES)
*       ISET  CONTROLE L'APPEL A SET
*       NSET  CONTROLE L'ENSEMBLE DE LA LOGIQUE (FOND, TITRE ET COURBE)
*       IHGP  CONTROLE L'APPEL A GRIDAL
*
      IF (ISET .GT. 0) GO TO 101
      NSET = -2
      IHGP = 2-IBAC
      GO TO 102
  101 NSET = 3
      IF (IBAC .LT. 4) NSET = 0
      IHGP = 2-IBAC
      GO TO 102
  102 CONTINUE
*
      CALL XPLOT80 (X,Y,NPTS,NC,L,NX,NY,IXGRD,LTIT,NDASH,
     1             NSET,IHGP,ISET,LTYPE,LFRAME)
*
      RETURN
      END
