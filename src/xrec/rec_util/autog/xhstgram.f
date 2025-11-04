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
***S/P  XHSTGRAM     FAIT PARTIE D'UN ENSEMBLE DE ROUTINES POUR TRACER ET
*             ANNOTER DES COURBES
*
      SUBROUTINE XHSTGRAM (X,Y,L,MANY,NPTS,LABGG)
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
*         LES DONNEES EN Y DOIVENT ETRE DANS UN ORDRE ASCENDANT
*         C'EST-A-DIRE  Y(I,K) > Y(I,K-1)
*
*----------------------------------------------------------------------
*
      INTEGER I,IHGP,IXGRD,J,L,MANY,NC,NDSHSV,NPTS,NSET,NX,NY
      INTEGER IX1,IX2,IY1,IY2,LT
      REAL X(NPTS+1), Y(L,1), XX(4), YY(4)
      REAL XMIN,XMAX,YMIN,YMAX
      CHARACTER(LEN=*) LABGG
      EXTERNAL XFRAME
      EXTERNAL XGETSET
      EXTERNAL XPLOT80
      EXTERNAL XPOLYGON
*
**---------------------------------------------------------------------
*
*     IXGRD = 2 QUAND LE CHAMP X A PLUS D'UNE DIMENSION, I.E. QUAND IL
*               YA PLUSIEURS COURBES ET QUE LE CHAMP X A LA MEME
*               DIMENSION QUE LE CHAMP Y
*
      IXGRD = 2
*
*     IXGRD EST NEGATIF SI LE DATA EST ENREGISTRE PAR RANGEE (IROW.LT.0
*
      IF (IROW .LT. 0) IXGRD = -IXGRD
*
*     PREPARATION POUR PLUSIEURS COURBES
*
      NC = MANY
      NY = MANY
      NX = 1
*
*     VERIFIE SI LE CHAMP X A SEULEMENT 1 COLONNE (IROW.EQ.1)
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
      NSET = -2
      IHGP = 2-IBAC
      CALL XPLOT80 (X,Y,NPTS+1,NC,L,NX,NY,IXGRD,LTIT,NDASH,
     1             NSET,IHGP,ISET,LTYPE,LFRAME)
*
      CALL XGETSET(IX1,IX2,IY1,IY2,XMIN,XMAX,YMIN,YMAX,LT)
*
      DO 50 J=1,MANY
        DO 40 I=1,NPTS
          XX(1) = X(I)
          XX(2) = X(I)
          XX(3) = X(I+1)
          XX(4) = X(I+1)
          IF (J .EQ. 1) THEN
            YY(1) = YMIN
          ELSE
            YY(1) = Y(I,J-1)
          ENDIF
*
          YY(2) = Y(I,J)
          YY(3) = Y(I,J)
          YY(4) = YY(1)
*
          IF (XX(2).LT.XX(3) .AND. YY(2).GT.YY(1))
     1      CALL XPOLYGON(XX,YY,4,IAND(LNDASH(J),127),.TRUE.)
   40   CONTINUE
   50 CONTINUE
*
      IF (LFRAME .EQ. 1) CALL XFRAME
*
      RETURN
      END
