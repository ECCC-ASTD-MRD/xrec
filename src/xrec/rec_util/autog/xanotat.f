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
*
***S/P  XANOTAT    CETTE ROUTINE PERMET A L'USAGER D'ANNOTER LES
*                 GRAPHIQUES SELON SES BESOINS
*
      SUBROUTINE XANOTAT (LABXX,LABYY,IBACC,ISETT,NDASHH,NDASHI,NDASHC)
      IMPLICIT NONE
      INTEGER NDASHI(26)
      INTEGER IBACC,ISETT,NDASHH
      CHARACTER *(*) LABXX,LABYY
      CHARACTER *(*) NDASHC(26)
*
*AUTEUR   SYSTEME NCAR
*
*REVISION 001  RPN  M.VALIN / C.THIBEAULT  1-7-78  REFAIT DOCUMENTATION
*
*LANGAGE  FORTRAN
*
*OBJET(ANOTAT)
*         ANOTAT DONNE LA POSSIBILITE DE CHANGER LES ETIQUETTES SUR LE
*         GRAPHIQUE (LABXX, LABYY) DE CHOISIR LE FOND DESIRE (IBACC,
*         ISETT) ET AUSSI DE CONTROLER LE GENRE DE PATRON UTILISE POUR
*         TRACER LES COURBES (NDASH, LDASHH)
*
*LIBRAIRIES
*         SOURCE  NEWAUTOGRAPHPL,ID=CMCULIB      DECK=ANOTAT
*
*         OBJET   NCARSYSOB,ID=CMCULIB
*
*ARGUMENTS
*
*         LABXX   TITRE POUR L'AXE X (UN MAXIMUM DE 40 CARACTERES EST
*                 PERMIS)  SI MOINS DE 40 CAR., N CARACTERES SUIVIT DU
*                 SIGNE $ TERMINE LA CHAINE.  LE SIGNE $ N'EST PAS
*                 POINTE ET LA CHAINE EST AUTOMATIQUEMENT CENTRE
*
*         LABYY   TITRE POUR L'AXE Y (UN MAXIMUM DE 40 CARACTERES EST
*                 PERMIS)  SI MOINS DE 40 CAR., N CARACTERES SUIVIT DU
*                 SIGNE $ TERMINE LA CHAINE.  LE SIGNE $ N'EST PAS
*                 POINTE ET LA CHAINE EST AUTOMATIQUEMENT CENTRE
*
*         IBACC   INDICATEUR QUI DETERMINE LE FOND A UTILISER
C                   IABS(IBACC)
*                 = 0  GARDE LA MEME VALEUR QUE DANS LE DERNIER APPEL
*                 = 1  FOND TRACE AVEC PERIM ET ETIQUETTE NUMERIQUE
*                 = 2  FOND TRACE AVEC GRID. LA GRILLE EST NUMEROTEE
*                 = 3  FOND TRACE AVEC HALFAX ET NUMEROTE
*                 = 4  PAS DE FOND
C                 SI IBACC > 0, ETIQUETTES EN BAS ET A GAUCHE
C                 SI IBACC < 0, ETIQUETTES EN HAUT ET A DROITE
*                 DEFAUT:  IBACC=1  (OPTION IBAC)
*
*         ISETT   INDICATEUR D'ECHELLE
*                     SI ISET EST NEGATIF LA COURBE NE SERA PAS TRACE
*                     = 0 GARDE LA MEME VALEUR QUE DANS LE DERNIER APPE
*                     = 1 AUTOGRAPH FAIT L'APPEL A SET.  LES LIMITES
*                         ARRONDIES SONT CALCULEES DE FLDX ET FLDY
*                     = 2 PLACE L'IMAGE DANS LA PARTIE DU CADRE QUI EST
*                         DETERMINE PAR LES QUATRE PREMIERS ARGUMENTS
*                         DANS LE DERNIER APPEL A SET.
*                     = 3 EMPLOIE LE MAXIMUM ET LE MINIMUM DE FLDX ET
*                         FLDY QUI EST DETERMINE PAR LES QUATRE DERNIER
*                         ARGUMENTS DANS LE DERNIER APPEL A SET.
*                     = 4 LES OPTIONS 2 ET 3 COMBINEES
*                     DEFAUT:  ISET=1
*
*         NDASHH  CONTROLE LE GENRE DE PATRON UTILISE POUR TRACER LES
*                 COURBES
*                     .EQ.0  NDASHH A LA MEME VALEUR QUE DANS LE DERNIE
*                            APPEL
*                     .GE.1  LE PATRON DES LIGNES A TRACER EST DEFINI
*                            DANS LE CHAMP NDASHI(I),I=1,NDASHH
*                     .LT.0  LES COURBES SONT DES LIGNES PLEINES
*                            INTERROMPUES PAR DES LABELS,  A POUR LA
*                            PREMIERE, B POUR LA DEUXIEME, C POUR LA
*                            TROISIEME ETC.
*
*         NDASHI  UN CHAMP DE PATRONS POUR LES COURBES A TRACER
*                 EX: UN PATRON DE 0 OU 1 EST INTERPRETE COMME UNE LIGNE
*                     PLEINE
*                     177777B EST UNE LIGNE PLEINE
*                     052525B EST UN TIRET, ESPACE, TIRET, ESPACE ETC.
*                     UN NOMBRE NEGATIF INDIQUE LA POSITION CORRESPONDANTE
*                     A UN PATRON EN CARACTERES DANS LE CHAMP NDASHC.
*                     DEFAUT:  LIGNES PLEINES AVEC ETIQUETTE A,B,C ETC.
*
*         NDASHC  UN CHAMP DE PATRONS EN CARACTERES POUR LES COURBES
*                 A TRACER
*
*IMPLICITES
      INTEGER IBAC,ISET,NDASH,LFRAME,IROW,LTYPE,LNDASH
      COMMON /XAUTOG1/ IBAC,ISET,NDASH,LFRAME,IROW,LTYPE,LNDASH(26)
      COMMON /XAUTOG2/ IIXYDEC
      CHARACTER *40 LABX,LABY,LTIT
      CHARACTER *(16) LDASHC(26)
      CHARACTER *10 LSYM(26)
      CHARACTER *8  IFMTX,IFMTY
      COMMON /XAUTOG3/ LABX,LABY,LTIT,LDASHC,LSYM,IFMTX,IFMTY
*
*NOTE     POUR PLUS DE DETAIL SUR LE BLOC /AUTOG1/ VOIR PLOT80
*
*----------------------------------------------------------------------
*
      INTEGER I,IIXYDEC
*
*----------------------------------------------------------------------
*
      IF (LABXX .NE. ' ') LABX = LABXX
      IF (LABYY .NE. ' ') LABY = LABYY
      IF (IBACC .NE. 0) IBAC = IABS(IBACC)
      IF (IBACC .GT. 0) IIXYDEC = 0
      IF (IBACC .LT. 0) IIXYDEC = 1
      IF (ISETT .NE. 0) ISET = ISETT
      IF(NDASHH.EQ.0)RETURN
      NDASH = MIN0(26,NDASHH)
*
      DO 101 I=1,NDASH
        LNDASH(I) = NDASHI(I)
        IF (NDASHI(I) .LT. 0)  THEN
          LDASHC(I) = NDASHC(I)
        ENDIF
  101 CONTINUE
*
  102 CONTINUE
      RETURN
      END
