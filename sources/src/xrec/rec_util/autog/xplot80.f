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
***S/P XPLOT80      TRACE DES COURBES
*
      SUBROUTINE XPLOT80 (FLDX,FLDY,NP,NC,IXY,JX,JY,IXGRD,
     1                   LLTIT,ILAB,NSET,IHGP,LISET,LLTYPE,LLFRAME)
      IMPLICIT NONE
*
*AUTEUR   SYSTEME NCAR
*
*REVISION 001  RPN  M.VALIN / C.THIBEAULT  1-7-78  REFAIT DOCUMENTATION
*         002  RPN  C.THIBEAULT  15-7-78  REMPLACE LES ASSIGN PAR DES
*                                         COMPUTED GO TO
*
*LANGAGE  FORTRAN
*
*OBJET(PLOT80)
*         PLOT80 PERMET DE TRACER DES COURBES (MAX. 26) AVEC DES TITRES
*         UN CHOIX D'IMAGES DE FOND OU ARRIERE PLAN
*
*DIRECTIVES(PLOT80)
*
*LIBRAIRIES
*         SOURCE   NEWAUTOGRAPHPL,ID=CMCULIB           DECK=PLOT80
*
*         OBJET     NCARSYSOB,ID=CMCULIB
*
*ARGUMENTS
*
*         FLDX    CHAMP DE VARIABLE INDEPENDANTE (X)
*
*         FLDY    CHAMP DE VARIABLE DEPENDANTE   (Y)
*
*         NP      NOMBRE DE POINTS A TRACER (NP.LE.IXY)
*
*         NC      NOMBRE DE COURBES A TRACER (NC.LE.JY)
*
*         IXY     PREMIERE DIMENSION DE FLDX ET FLDY
*
*         JX      DEUXIEME DIMENSION DE FLDX (UNE OU PLUSIEURS COURBES
*                                             A TRACER)
*         JY      DEUXIEME DIMENSION DE FLDY (EQUIVALENT AU NOMBRE DE C
*                                             A TRACER)
*
*         IXGRD   DECRIT DE QUELLE MANIERE FLDX ET FLDY SONT ENREGISTRE
*                 .EQ.0  GARDE LA MEME VALEUR QUE DANS LE DERNIER APPEL
*                 .GT.0  LE DATA EST ENREGISTRE DE LA MANIERE SUIVANTE:
*                        FLDY(I,J),I=1,2,....NPTS  ET
*                                  J=1,2,...MANY COURBES
*                 .LT.0  LE DATA EST ENREGISTRE DE LA MANIERE SUIVANTE:
*                        FLDY(I,J),J=1,2....NPTS ET
*                                  I=1,2....MANY COURBES
*         IABS(IXGRD)
*                 .EQ.1  LE CHAMP X A UNE DIMENSION POUR PLUSIEURS
*                        COURBES,  I.E. UNE DIMENSION DE X POUR
*                        PLUSIEURS Y
*                 .GT.1  POUR PLUSIEURS COURBES, X A LA MEME DIMENSION
*                        QUE Y
*                 DEFAUT:  IXGRD=1  (OPTION IROW)
*
*         LLTIT   TITRE DU GRAPHIQUE (UN MAXIMUM DE 40 CARACTERES EST
*                 PERMIS)  SI MOINS DE 40 CAR., N CARACTERES SUIVIT DU
*                 SIGNE $ TERMINE LA CHAINE.  LE SIGNE $ N'EST PAS
*                 POINTE ET LA CHAINE EST AUTOMATIQUEMENT CENTRE
*
*         ILAB    CONTROLE LE GENRE DE PATRON UTILISE POUR TRACER LES
*                 COURBES
*                     .EQ.0  NDASH A LA MEME VALEUR QUE DANS LE DERNIER
*                            APPEL
*                     .GE.1  LE PATRON DES LIGNES A TRACER EST DEFINI
*                            DANS LE CHAMP LDASH(I),I=1,NDASH
*                     .LT.0  LES COURBES SONT DES LIGNES PLEINES
*                            INTERROMPUES PAR DES LABELS,  A POUR LA
*                            PREMIERE, B POUR LA DEUXIEME, C POUR LA
*                            TROISIEME ETC.
*                            DEFAUT:  ILAB=1  (OPTION NDASH)
*
*         LISET   INDICATEUR D'ECHELLE
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
*         IHGP   .LT.0  LE FOND EST TRACE AVEC HALFAX
*                .EQ.0  LE FOND EST TRACE AVEC GRID
*                .GT.0  LE FOND EST TRACE AVEC PERIM
*
*         NSET    = -1  SEULS SET ET GRIDAL SONT APPELES
*                 = -2  SET ET GRIDAL SONT APPELES, LES LABELS SONT
*                       ECRIT PAR PLOT80 MAIS IL NE TRACE PAS DE COURBE
*                 =  0  SET ET GRIDAL SONT APPELES, LES LABELS SONT
*                       ECRIT ET LES COURBES SONT TRACEES PAR PLOT80
*                 =  1  SEULEMENT LES COURBES SONT TRACEES PAR PLOT80
*                 =  3  SET EST APPELE, LES LABELS SONT ECRIT ET LES
*                       COURBES SONT TRACEES PAR PLOT80.
*
*         LLTYPE  CONTROLE LA PRESENTATION DES COURBES
*                   = 0 GARDE LA MEME VALEUR QUE DANS LE DERNIER APPEL
*                   = 1 AXE-X  LINEAIRE, AXE-Y  LINEAIRE
*                   = 2 AXE-X  LINEAIRE, AXE-Y  LOGARITHMIQUE
*                   = 3 AXE-X  LOGARITHMIQUE,  AXE-Y  LINEAIRE
*                   = 4 AXE-X  LOGARITHMIQUE,  AXE-Y  LOGARITHMIQUE
*                   DEFAUT:  LTYPE =1
*
*         LLFRAME CONTROLE L'AVANCE DE L'IMAGE
*                   = 0 GARDE LA MEME VALEUR QUE DANS LE DERNIER APPEL
*                   = 1 L'IMAGE AVANCE APRES AVOIR TRACE
*                   = 2 L'IMAGE N'AVANCE PAS
*                   = 3 L'IMAGE AVANCE AVANT DE TRACER
*                   DEFAUT:  LFRAME=1
*
*IMPLICITES
*
      INTEGER ISZXAX,ISZYAX,ISZXLB,ISZYLB,ISZCRV,ISZTYT,NUMX,
     1        NUMY,IXOR,IYOR
      COMMON /XCHARSZ/ ISZXAX,ISZYAX,ISZXLB,ISZYLB,ISZCRV,ISZTYT,
     1                NUMX,NUMY,IXOR
      INTEGER IBAC,ISET,NDASH,LFRAME,IROW,LTYPE,LNDASH
      COMMON /XAUTOG1/ IBAC,ISET,NDASH,LFRAME,IROW,LTYPE,LNDASH(26)
      COMMON /XAUTOG2/ IIXYDEC
      CHARACTER *40 LABX,LABY,LTIT
      CHARACTER *(16) LDASHC(26)
      CHARACTER *10 LSYM(26)
      CHARACTER *8  IFMTX,IFMTY
      COMMON /XAUTOG3/ LABX,LABY,LTIT,LDASHC,LSYM,IFMTX,IFMTY
      LOGICAL CLIP,JOIN
      INTEGER ERRBAR,SSYM,SSIZ,EPAIS,FONC
      COMMON /XAUTOG6/ CLIP,ERRBAR,JOIN,SSYM,SSIZ,EPAIS,FONC
*
*         LABX    TITRE POUR L'AXE X (UN MAXIMUM DE 40 CARACTERES EST
*                 PERMIS)  SI MOINS DE 40 CAR., N CARACTERES SUIVIT DU
*                 SIGNE $ TERMINE LA CHAINE.  LE SIGNE $ N'EST PAS
*                 POINTE ET LA CHAINE EST AUTOMATIQUEMENT CENTRE
*
*         LABY    TITRE POUR L'AXE Y (UN MAXIMUM DE 40 CARACTERES EST
*                 PERMIS)  SI MOINS DE 40 CAR., N CARACTERES SUIVIT DU
*                 SIGNE $ TERMINE LA CHAINE.  LE SIGNE $ N'EST PAS
*                 POINTE ET LA CHAINE EST AUTOMATIQUEMENT CENTRE
*
*         IBAC    INDICATEUR QUI DETERMINE LE FOND A UTILISE
*                 = 0  GARDE LA MEME VALEUR QUE DANS LE DERNIER APPEL
*                 = 1  FOND TRACE AVEC PERIM ET ETIQUETTE NUMERIQUE
*                 = 2  FOND TRACE AVEC GRID. LA GRILLE EST NUMEROTEE
*                 = 3  FOND TRACE AVEC HALFAX ET NUMEROTE
*                 = 4  PAS DE FOND
*                 DEFAUT:  LIBAC=1  (OPTION IBAC)
*
*         ISET    CONTROLE L'ECHELLE   (VOIR ISET)
*
*         NDASH   CONTROLE LE PATRON DES LIGNES A TRACER   (VOIR ILAB)
*
*         LNDASH  UN CHAMP DE PATRONS POUR LES COURBES A TRACER
*                 EX: UN PATRON DE 0 OU 1 EST INTERPRETE COMME UNE LIGN
*                     PLEINE
*                     177777B EST UNE LIGNE PLEINE
*                     052525B EST UN TIRET, ESPACE, TIRET, ESPACE ETC.
*                     DEFAUT:  LIGNES PLEINES AVEC ETIQUETTE A,B,C ETC.
*
*
*         LFRAME  CONTROLE L'AVANCE DE L'IMAGE  (VOIR LFRAME)
*
*         IROW    INDIQUE LA FACON D'ENREGISTRER LE DATA DANS LES
*                 CHAMPS X ET Y        (VOIR IXGRD)
*
*         LTYPE   CONTROLE LA PRESENTATION DE LA COURBE    (VOIR LTYPE)
*
*         LTIT    TITRE DU GRAPHIQUE   (VOIR LTIT)
*
*MODULES  FRAME,GETSET,LINRD,LGRD,SET,LABMOD,GRIDAL,DASHD,LNTH,PWRY,
*         FRSTD,VECTD,LASTD,ULIBER
*
*PRECISION
*         SIMPLE
*
*TABLES   LSYM - CETTE TABLE EST UTILISE POUR IDENTIFIER LES COURBES DE
*                A  A  Z. POSSIBILITE DE 26 COURBES.
*
*NOTES    POUR OBTENIR PLUS DE 26 COURBES SUR LE MEME GRAPHIQUE ON
*         PEUT FAIRE UN APPEL DIRECTEMENT A PLOT80.  ON DOIT ALORS
*         DONNER LE PATRON DES COURBES A TRACER DANS IPATRN(N) N =NOMBR
*         DE COURBES  DESIREES
*
*
*----------------------------------------------------------------------
*
      INTEGER X1O,Y1O,X2O,Y2O
      INTEGER I,IDUM,IER,IHGP,IIXYDEC,INRX,INRY,IOR,ITY,IX1,IX2
      INTEGER IXC,IXD,IXDM,IXGRD,IY1,IY2,IYC,IYDM,J,JCRT,JJ
      INTEGER JSIZE,JX,K,LDASH,LHGP,LLDASH,LT,MGRX,MGRY,MM,NC,NK,NP,NPT
      INTEGER NSET,LISET,LLFRAME,LLTYPE,IXY,ILAB,JY,LLTIT
      INTEGER IISZXAX,IISZYAX,IISZXLB,IISZYLB,IISZCRV,IISZTYT,
     1        NNUMX,NNUMY,IIXOR,INUMX,INUMY
      INTEGER IS1,IS2,IS3,IS4,INCER,XLT
      INTEGER LLTYP,LLVXY(4),NCC,SSIZZ
      character*4 cval
      integer iblank
      integer largf, hautf
      REAL       FLDX(IXY,JX)           ,FLDY(IXY,JY)
      REAL X1,X2,XAX,XB,XBO,XMAX,XXMAX,XMIN,XXMIN,XP,XR,XT,XTO,XX,XY
      REAL YBO,YMAX,YYMAX,YMIN,YYMIN,YP,YR,YT,YTO,YY,Y1,Y2,YAX,YB
      REAL SDY,XS1,YS1,XS2,YS2
      CHARACTER *40 LLABX,LLABY
      CHARACTER *8  IIFMTX,IIFMTY,FMTX,FMTY
      CHARACTER *3  PSYM
*
      EXTERNAL XAUTOGET
      EXTERNAL XDASHD
      EXTERNAL XDASHI
      EXTERNAL XEBAR
      EXTERNAL XFRAME
      EXTERNAL XFRSTD
      EXTERNAL XGETSET
      EXTERNAL XGRIDAL
      EXTERNAL XLABMOD
      EXTERNAL XLASTD
      EXTERNAL XLGRD
      EXTERNAL XLINRD
      EXTERNAL XLNTH
      EXTERNAL XOPTN
      EXTERNAL XPWRIT
      EXTERNAL XSET
      EXTERNAL XULIBER
      EXTERNAL XVECTD
      EXTERNAL XWINDOW
*
*
*     LES PARAMETRES DE TRACAGE ET LES IDENTIFICATEURS DE COURBES SONT
*         INITIALIZES ICI
*
      DATA LABX, LABY /'X$', 'Y$'/
      DATA IBAC, ISET, LFRAME, IROW, LTYPE /1,1,1,1,1/
      DATA NDASH / 1 /
      DATA LNDASH / 26*65535/
C     DATA LNDASH / 26*177777B /
      DATA IIXYDEC /0/
*
      DATA
     1 LSYM /'$$$$A$$$$$', '$$$$$$$$B$', '$C$$$$$$$$', '$$$$$D$$$$'
     2     , '$$$$$$$$$E', '$$F$$$$$$$', '$$$$$$G$$$', 'H$$$$$$$$$'
     3     , '$$$I$$$$$$', '$$$$$$$J$$', '$$$$K$$$$$', '$$$$$$$$L$'
     4     , '$M$$$$$$$$', '$$$$$N$$$$', '$$$$$$$$$O', '$$P$$$$$$$'
     5     , '$$$$$$Q$$$', 'R$$$$$$$$$', '$$$S$$$$$$', '$$$$$$$T$$'
     6     , '$$$$U$$$$$', '$$$$$$$$V$', '$W$$$$$$$$', '$$$$$X$$$$'
     7     , '$$$$$$$$$Y', '$$Z$$$$$$$'/
      DATA   IFMTX   ,   IFMTY
     1    /   '  '   ,    '  ' /
      DATA  ISZXAX, ISZYAX, ISZXLB, ISZYLB, ISZCRV, ISZTYT
     1    /    0  ,    0  ,    0  ,    0  ,    0  ,    0    /
      DATA  NUMX, NUMY, IXOR, IYOR
     1    /   0 ,   0 ,   0 ,   0  /
      DATA LLVXY / 1,3,2,4/
*
**---------------------------------------------------------------------
*
      call wglgwz(largf, hautf)
      call wglcmi(0, 0, largf-1, hautf-1)
      read(cval,'(A4)') fonc
      IF (cval .EQ. 'XVSY') THEN
        LLABX = LABY
        LLABY = LABX
        INUMX = NUMY
        INUMY = NUMX
        FMTX  = IFMTY
        FMTY  = IFMTX
        IISZXAX = ISZYAX
        IISZYAX = ISZXAX
        IISZYLB = ISZXLB
        IISZXLB = ISZYLB
      ELSE
        LLABX = LABX
        LLABY = LABY
        INUMX = NUMX
        INUMY = NUMY
        FMTX  = IFMTX
        FMTY  = IFMTY
        IISZXAX = ISZXAX
        IISZYAX = ISZYAX
        IISZYLB = ISZYLB
        IISZXLB = ISZXLB
      ENDIF
*
      IF (IISZXAX .EQ. 0) THEN
        IISZXAX = 12
      ENDIF
*
      IF (IISZYAX .EQ. 0) THEN
        IISZYAX = 12
      ENDIF
*
      IF (IISZXLB .EQ. 0) THEN
        IISZXLB = 15
      ENDIF
*
      IF (IISZYLB .EQ. 0) THEN
        IISZYLB = 15
      ENDIF
*
      IF (ISZCRV .EQ. 0) THEN
        IISZCRV = 10
      ELSE
        IISZCRV = ISZCRV
      ENDIF
*
      IF (ISZTYT .EQ. 0) THEN
        IISZTYT = 20
      ELSE
        IISZTYT = ISZTYT
      ENDIF
*
*     AVANCE L'IMAGE AVANT DE TRACER SI LFRAME = 3
*
      IF(LLFRAME.EQ.3)CALL XFRAME
*
*     CONSTANTES POUR APPEL A DASHD
*
      JCRT = 24
      JSIZE = 10
*
*     CONSTANTES POUR APPEL A SET
*
*      X1 = .1+.1*(1-IIXYDEC)
*      Y1 = .1+.1*(1-IIXYDEC)
*****
*     modif pour avoir une plus grande surface de tracage
*
      X1 = .1*(1-IIXYDEC)
      Y1 = .1*(1-IIXYDEC)
      X2 = .9-.1*IIXYDEC
      Y2 = .9-.1*IIXYDEC
**---------------------------------------------------------------------
*
      IF (IABS(LISET) .NE. 4) GO TO 51
*
*     ISET = 4  LES OPTIONS 2 ET 3 COMBINEES
*
      CALL XGETSET(IX1,IX2,IY1,IY2,XMIN,XMAX,YMIN,YMAX,LT)
      X1 = (IX1-1) / 1023.
      X2 = (IX2-1) / 1023.
      Y1 = (IY1-1) / 1023.
      Y2 = (IY2-1) / 1023.
      GO TO 1101
*
   51 CONTINUE
      IF(IABS(LISET).EQ.1)GO TO 12
*
*     APPEL A GETSET SEULEMENT SI ISET = 2 OU 3
*
      CALL XGETSET(X1O,X2O,Y1O,Y2O,XBO,XTO,YBO,YTO,LT)
      IF(IABS(LISET).EQ.3)GO TO 5
*
*     ISET = 2
*
      X1 = (X1O-1) / 1023.
      X2 = (X2O-1) / 1023.
      Y1 = (Y1O-1) / 1023.
      Y2 = (Y2O-1) / 1023.
      GOTO12
*
*     ISET = 3
*
    5 XMIN=XBO
      XMAX=XTO
      YMIN=YBO
      YMAX=YTO
      GOTO1101
**---------------------------------------------------------------------
*
*     ISET = 1  TROUVE MAXIMUM ET MINIMUM DE FLDX ET FLDY
*
   12 CONTINUE
      IXDM = 1
      IF (IXGRD.LT.0 .AND. JX.NE.1) IXDM = 2
      IYDM = 1
      IF (IXGRD.LT.0) IYDM = 2
      IF (IABS(IXGRD) .GT. 1) GO TO 101
      XXMIN = 1
      XXMAX = NP
      GO TO 106
  101 XXMIN = FLDX(1,1)
      XXMAX = FLDX(1,1)
*
      DO 105 J=1,JX
*
         DO 104 I=1,NP
            GO TO (102,103),IXDM
  102       XXMIN = AMIN1(XXMIN,FLDX(I,J))
            XXMAX = AMAX1(XXMAX,FLDX(I,J))
            GO TO 104
  103       XXMIN = AMIN1(XXMIN,FLDX(J,I))
            XXMAX = AMAX1(XXMAX,FLDX(J,I))
  104    CONTINUE
  105 CONTINUE
*
  106 CONTINUE
      YYMIN = FLDY(1,1)
      YYMAX = FLDY(1,1)
*
      DO 110 J=1,NC
*
         DO 109 I=1,NP
            GO TO (107,108),IYDM
  107       YYMIN = AMIN1(YYMIN,FLDY(I,J))
            YYMAX = AMAX1(YYMAX,FLDY(I,J))
            GO TO 109
  108       YYMIN = AMIN1(YYMIN,FLDY(J,I))
            YYMAX = AMAX1(YYMAX,FLDY(J,I))
  109    CONTINUE
  110 CONTINUE
*
      IF (cval .EQ. 'XVSY') THEN
        XMIN = YYMIN
        XMAX = YYMAX
        YMIN = XXMIN
        YMAX = XXMAX
      ELSE
        XMIN = XXMIN
        XMAX = XXMAX
        YMIN = YYMIN
        YMAX = YYMAX
      ENDIF
*
 1101 CONTINUE
*----------------------------------------------------------------------
*
*     DEPENDANT DU FOND DESIRE, LES VALEURS MAXIMUMS ET MINIMUM SONT
*      ARRONDIES
*
      IF (LLTYPE.LE.0 .OR. LLTYPE.GT.4) GO TO 144
*
*     SI LES AXES SONT INVERSES, ON INVERSE LLTYPE
*
      IF (cval .EQ. 'XVSY') THEN
        LLTYP = LLVXY(LLTYPE)
      ELSE
        LLTYP = LLTYPE
      ENDIF
*
      IF (LLTYP.EQ.1.OR.LLTYP.EQ.2)
     1   CALL XLINRD(XMIN,XMAX,XB,XT,MGRX,INRX,IIFMTX,NNUMX,IOR)
      IF (LLTYP.EQ.3.OR.LLTYP.EQ.4)
     1   CALL XLGRD(XMIN,XMAX,XB,XT,MGRX,INRX,IIFMTX,NNUMX,IOR)
*
      IF (LLTYP.EQ.1.OR.LLTYP.EQ.3)
     1   CALL XLINRD(YMIN,YMAX,YB,YT,MGRY,INRY,IIFMTY,NNUMY,IDUM)
      IF (LLTYP.EQ.2.OR.LLTYP.EQ.4)
     1   CALL XLGRD (YMIN,YMAX,YB,YT,MGRY,INRY,IIFMTY,NNUMY,IDUM)
*
      IF (INUMX .EQ. 0) THEN
        INUMX = NNUMX
      ENDIF
*
      IF (INUMY .EQ. 0) THEN
        INUMY = NNUMY
      ENDIF
*
      IF (IXOR .EQ. 0) THEN
        IIXOR = IOR
      ELSE
        IIXOR = IXOR
      ENDIF
      IF(IIXOR.NE.0) IIXOR = 1
*
      IF (FMTX .EQ. ' ') THEN
        FMTX = IIFMTX
      ENDIF
*
      IF (FMTY .EQ. ' ') THEN
        FMTY = IIFMTY
      ENDIF
*----------------------------------------------------------------------
*
*     CHANGE LES UNITES DES AXES X ET Y POUR QUE L'UN SOIT LE MULTIPLE
*      DE L'AUTRE.  CENTRE L'AXE LA PLUS COURTE
*
  116 IF (IABS(LISET) .EQ. 4) GO TO 124
      IF (NSET .EQ. 1) GO TO 129
      IF(IABS(LISET) .EQ. 2)GO TO 123
  117 GO TO (118,121,121,119),LLTYP
  118 XR = XT-XB
      YR = YT-YB
      GO TO 120
  119 XR = ALOG10(XT)-ALOG10(XB)
      YR = ALOG10(YT)-ALOG10(YB)
  120 XY = XR/YR
      IF (XY.LT..5 .OR. XY.GT.2.) GO TO 121
      XP = .7
      YP = .7
      IF (XY .LT. 1) XP = XY*(X2-X1)
      IF (XY .GT. 1) YP = (Y2-Y1) /XY
      X1 = .5*(1.-XP)
      X2 = X1+XP
      Y1 = .5*(1.-YP)
      Y2 = Y1+YP
  121 CONTINUE
  123 CALL XSET (X1,X2,Y1,Y2,XB,XT,YB,YT,LLTYP)
  124 CONTINUE
      IF(NSET.NE.3)  THEN
        IF (cval .EQ. 'XVSY') THEN
          CALL XLABMOD (FMTX,FMTY,INUMX,INUMY,IISZYAX,IISZXAX,IIXYDEC,
     1                 IIXYDEC,IIXOR)
        ELSE
          CALL XLABMOD (FMTX,FMTY,INUMX,INUMY,IISZXAX,IISZYAX,IIXYDEC,
     1                 IIXYDEC,IIXOR)
        ENDIF
      ENDIF
 1241 CONTINUE
**---------------------------------------------------------------------
*
*     TRACER LE PLAN DESIRE EN UTILISANT GRID, PERIM OU HALFAX
*
      IF (IHGP) 125,126,127
*
*     HALFAX
*
  125 LHGP = 10
      GO TO 128
*
*     GRID
*
  126 LHGP = IHGP
      GO TO 128
*
*     PERIM
*
  127 LHGP = 5
128   IF(IIXYDEC.EQ.0) CALL XGETSET(IDUM,IDUM,IDUM,IDUM,XAX,IDUM,
     $     YAX,IDUM,IDUM)
      IF(IIXYDEC.EQ.1) CALL XGETSET(IDUM,IDUM,IDUM,IDUM,IDUM,XAX,
     $     IDUM,YAX,IDUM)
      IF(NSET.NE.3) CALL XGRIDAL (MGRX,INRX,MGRY,INRY,1,1,LHGP,XAX,YAX)
*
*     NSET = -1  SEUL LE FOND EST TRACE
*
      IF (NSET .EQ. -1) RETURN
**---------------------------------------------------------------------
*
*     TROUVE LE CENTRE DES AXES X ET Y ET TRACE LES TITRES
*
      IXC = 512.*(X1+X2)
      IYC = 512.*(Y1+Y2)
      IXD = IIXOR*((INUMX+1)*IISZXAX-IISZXLB/2)
      IF (IIXOR .EQ. 0) IXD = 3*IISZXAX+IISZXLB
      MM = 1024.*Y1-IXD
      IF(IIXYDEC.EQ.1) MM = 1024.*Y2+IXD
      CALL XDASHI(65535)
      CALL XLNTH (LLABX,NK)
*
      CALL XPWRIT (IXC*largf/1024,MM*hautf/1024,LLABX,NK,IISZXLB,0,1)
*
      MM = 1024.*X1-(INUMY+3)*IISZYAX-IISZYLB
      IF(IIXYDEC.EQ.1) MM = 1024.*X2+(INUMY+3)*12
      CALL XLNTH (LLABY,NK)
*
      CALL XPWRIT (MAX(1,MM)*largf/1024,IYC*hautf/1024,
     $     LLABY,NK,IISZYLB,90,21)
      ITY = MIN(Y2*1024+16,FLOAT(1023-IISZTYT))
      IF(IIXYDEC.EQ.1) ITY = ITY + IXD
      CALL XLNTH (LTIT,NK)
      CALL XPWRIT (IXC*largf/1024,ITY*hautf/1024,LTIT,NK,IISZTYT,0,11)
*----------------------------------------------------------------------
*
*     NSET = -2  ON NE TRACE PAS DE COURBES
*
      IF (NSET .EQ. -2) RETURN
  129 JJ = 1
**---------------------------------------------------------------------
*
*     SECTION DE TRACAGE
*
*
*     QUAND IXGRD EST NEGATIF LE DATA EST ENREGISTRE PAR RANGEES
*
      IYDM = 1
      IF (IXGRD.LT.0) IYDM = 2
*
*     IXDM .EQ. 1    SI FLDX N'EXISTE PAS
*     IXDM .EQ. 2    SI LE DATA EST ENREGISTRE PAR RANGEES OU SI LE
*                    DATA EST ENREGISTRE PAR COLONNES MAIS IL Y A
*                    SEULEMENT UNE COURBE
*     IXDM .EQ. 3    SI LE DATA EST ENREGISTRE PAR COLONNES ET IL Y A
*                    PLUSIEURS COURBES
*
      IXDM = 1
      IF (IXGRD.GT.1 .OR. (IXGRD.LT.(-1) .AND. JX.EQ.1)) IXDM = 2
      IF (IXGRD.LT.-1 .AND. JX.GT.1) IXDM = 3
**---------------------------------------------------------------------
*
      iblank = transfer('    ',iblank)
      IF (SSYM .EQ. 0 .OR. SSYM.EQ.iblank .AND. ERRBAR.GT.0) THEN
        PSYM = '@A$'
      ELSE
        WRITE(PSYM,'(A3)') SSYM
      ENDIF
*
      IF (PSYM(1:1) .EQ. '@') THEN
        NCC = 3
      ELSE
        NCC = 1
      ENDIF
*
      IF (SSIZ .EQ. 0) THEN
        SSIZZ = 20
      ELSE
        SSIZZ = SSIZ
      ENDIF
*
      IF (CLIP) THEN
        CALL XGETSET(IS1,IS2,IS3,IS4,XS1,XS2,YS1,YS2,XLT)
        CALL XWINDOW(XS1,YS1,XS2,YS2)
      ENDIF
      IF (ERRBAR.GT.0 .AND. ERRBAR.LT.4) THEN
        INCER = 2
      ELSE
        INCER = 1
      ENDIF
      CALL XOPTN("THI",EPAIS)
*
*     BOUCLE SUR LE NOMBRE DE COURBES A TRACER
*
      DO 143 K=1,NC,INCER
*
*     IL EST POSSIBLE D'AVOIR SEULEMENT UN AXE X POUR TOUTES LES COURBE
*
         IF (K .LE. JX) JJ = K
*
*     ILAB .GT.0  COURBES GENEREES PAR L'USAGER
*     ILAB .LE.0  COURBES GENEREES INTERNEMENT DANS LSYM
*
      IF (NDASH .GT. 0) THEN
        IF (LNDASH(K) .GE. 0) THEN
          CALL XDASHI(LNDASH(K))
        ELSE
          CALL XDASHD(LDASHC(K),-LNDASH(K),JCRT,IISZCRV)
        ENDIF
      ELSE
        CALL XDASHD(LSYM(K),10,JCRT,IISZCRV)
      ENDIF
**---------------------------------------------------------------------
*
      IF (JOIN) THEN
*     BOUCLE SUR LE NOMBRE DE POINTS PAR COURBE
*
         DO 142 NPT=1,NP
*
*     CHOISI ENTRE:  PAS DE CHAMP X, RANGEES OU COLONNES
*
      GO TO (134,135,136),IXDM
  134       XX = NPT
            GO TO 137
  135       XX = FLDX(NPT,JJ)
            GO TO 137
  136       XX = FLDX(JJ,NPT)
*
  137 GO TO (138,139),IYDM
  138       YY = FLDY(NPT,K)
            GO TO 140
  139       YY = FLDY(K,NPT)
  140       IF (NPT .EQ. 1) THEN
              IF (cval .EQ. 'XVSY') THEN
                CALL XFRSTD(YY,XX)
              ELSE
                CALL XFRSTD(XX,YY)
              ENDIF
*
            ELSE
              IF (cval .EQ. 'XVSY') THEN
                CALL XVECTD(YY,XX)
              ELSE
                CALL XVECTD(XX,YY)
              ENDIF
            ENDIF
  142    CONTINUE
*
         CALL XLASTD
      ENDIF
*
*     BOUCLE SUR LE NOMBRE DE POINTS PAR COURBE
*
  233    DO 242 NPT=1,NP
*
*     CHOISI ENTRE:  PAS DE CHAMP X, RANGEES OU COLONNES
*
      GO TO (234,235,236),IXDM
  234       XX = NPT
            GO TO 237
  235       XX = FLDX(NPT,JJ)
            GO TO 237
  236       XX = FLDX(JJ,NPT)
*
  237 GO TO (238,239),IYDM
  238       YY = FLDY(NPT,K)
            SDY = FLDY(NPT,K+1)
            GO TO 240
  239       YY = FLDY(K,NPT)
            SDY = FLDY(K+1,NPT)
*
240         IF (cval .EQ. 'XVSY') THEN
              CALL XEBAR(YY,XX,SDY,ERRBAR,PSYM,NCC,SSIZZ)
            ELSE
              CALL XEBAR(XX,YY,SDY,ERRBAR,PSYM,NCC,SSIZZ)
            ENDIF
  242    CONTINUE
*
         CALL XLASTD
  143 CONTINUE
*----------------------------------------------------------------------
*
*     AVANCE L'IMAGE APRES AVOIR TRACE SI LFRAME = 1
*
      CALL XWINDOW(1,1,1,1)
      IF (LLFRAME .EQ. 1) CALL XFRAME
      CALL XDASHI(65535)
      CALL XOPTN("THI",1)
      RETURN
*----------------------------------------------------------------------
*
*     ERREUR FATALE - LTYPE EST .LT. 1 OU .GT. 4
*
  144 IER = 33
      CALL XULIBER (IER,'LTYPE IS ILL-DEFINED',20)
*
      RETURN
      CALL XAUTOGET
      END
