***FONCTION TTTCLIP - TROUVE LA PORTION D'UN SEGMENT DE DROITE SITUE
*                   DANS UNE FENETRE DONNEE.
#include "aaaamac.cdk"
*
      FUNCTION TTTCLIP (XMIN, XMAX, YMIN, YMAX, X1, Y1, X2, Y2)
#include "impnone.cdk"
      INTEGER  TTTCLIP, XMIN, XMAX, YMIN, YMAX, X1, X2, Y1, Y2
*
*  ENTREE TTTCLIP
*
*AUTEUR   - M. VALIN  -  FEV 78
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTCLIP)
*         - CALCULE LA PORTION D'UN SEGMENT DE DROITE SITUE DANS UNE
*           FENETRE; RENVOIE UN CODE INDIQUANT SI LE SEGMENT EST
*           VISIBLE,INVISIBLE, OU PARTIELLEMENT VISIBLE.
*
*ALGORITHME
*         - CHAQUE BOUT DU SEGMENT PEUT SE TROUVER DANS L'UNE DE NEUF
*           REGIONS. LA REGION 5 CONSTITUTE LA FENETRE. IL Y A 81
*           POSSIBILITES DE COMBINER LES POSITIONS DES DEUX BOUTS DU
*           SEGMENT. L'ALGORITHME UTILISE ICI EST LE PLUS SIMPLE, SOIT
*           UN BRANCHEMENT CALCULE (COMPUTED GOTO) A 81 VOIES
*
*                  I     I
*                1 I  2  I  3
*                  I     I
*              ---------------
*                  I*****I
*                4 I* 5 *I  6
*                  I*****I
*              ---------------
*                  I     I
*                7 I  8  I  9
*                  I     I
*
*              TABLE DES REGIONS
*
*PRECISION
*         - INTEGER
*
*LIBRAIRIES
*         - SOURCE   METACODETRANSLATORPL,ID=RMN     DECK=TTTCLIP
*         - OBJET    METACODETRANSLATOROB,ID=RMN
*
*APPEL    - IPS=TTTCLIP(XMIN,XMAX,YMIN,YMAX,X1,Y1,X2,Y2)
*
*ARGUMENTS
*    E    - XMIN  - COORDONNEES X ET Y DES QUATRE
*    E    - XMAX  - COINS DE LA FENETRE QUI NOUS
*    E    - YMIN  - INTERESSE  (>=0, <=32767)
*    E    - YMAX  -
*         - X1    - COORDONNEES DU PREMIER POINT
*         - Y1    - SERVENT AUSSI D'ARGUMENT DE SORTIE
*         - X2    - COORDONNEES DU DEUXIEME POINT,
*         - Y2    - SERVENT D'ARGUMENT DE SORTIE
*         - TTTCLIP - =-2, INVISIBLE
*                   = 0, TOUT VISIBLE
*                   = 1, PARTIALLEMENT VISIBLE
*                   = 2, PARTIALLEMENT VISIBLE
*
*IMPLICITES
*         - AUCUN
*
*NOTES    - TTTCLIP S'INSPIRE D'UN SOUS PROGRAMME ANALOGUE DE LA SECTION
*           "WINDOW" DU SYSTEME DE GRAPHISME DE NCAR
*
*--------------------------------------------------------------------
*
      INTEGER LX, LOLD, I1, MXJ(4), MXI, MYI, MXOLD, MX, KK, INDX(4), 
     +        LY, JUMP, I2, MYJ(4), MMX, MMY, MYOLD, MY, L
      REAL    FM, RFM,  CON
*
      MXOLD = X1
      MYOLD = Y1
      MX    = X2
      MY    = Y2
      LX    = 1
      LY    = 1
      IF(MXOLD .LT. XMIN) LX = 0
      IF(MXOLD .GT. XMAX) LX = 2
      IF(MYOLD .LT. YMIN) LY = 0
      IF(MYOLD .GT. YMAX) LY = 2
      LOLD = 1+LX+3*LY
      LX   = 1
      LY   = 1
      IF(MX    .LT. XMIN) LX = 0
      IF(MX    .GT. XMAX) LX = 2
      IF(MY    .LT. YMIN) LY = 0
      IF(MY    .GT. YMAX) LY = 2
      L    = 1+LX+LY*3
      JUMP = LOLD+(L-1)*9
*
* NEW     1   2   3   4   5   6   7   8   9.           !  OLD
*
      GOTO (141,141,141,141,132,114,141,114,114,                   !  1
     +      141,141,141,114,102,114,114,114,114,                   !  2
     +      141,141,141,114,136,141,114,114,141,                   !  3
     +      141,114,114,141,109,114,141,114,114,                   !  4
     +      133,107,137,112,101,113,125,108,129,                   !  5
     +      114,114,141,114,111,141,114,114,141,                   !  6
     +      141,114,114,141,124,114,141,141,141,                   !  7
     +      114,114,114,114,106,114,141,141,141,                   !  8
     +      114,114,141,114,128,141,141,141,141),JUMP              !  9
*
*   NOTHING VISIBLE
*
 141  TTTCLIP = -2
      RETURN
*
*   ALL VISIBLE
*
 101  TTTCLIP = 0
      RETURN
*
*   HALF VISIBLE
*
 102  MYI = YMIN
      MXI = MXOLD+FLOAT(MYI-MYOLD)/(MY-MYOLD)*(MX-MXOLD)
      GO TO 104
 106  MYI = YMAX
      MXI = MXOLD+FLOAT(MYI-MYOLD)/(MY-MYOLD)*(MX-MXOLD)
      GOTO 104
 107  MYI = YMIN
      MXI = MXOLD+FLOAT(MYI-MYOLD)/(MY-MYOLD)*(MX-MXOLD)
      GOTO 105
 108  MYI = YMAX
      MXI = MXOLD+FLOAT(MYI-MYOLD)/(MY-MYOLD)*(MX-MXOLD)
      GOTO 105
 104  X2      = MXI                        ! POINT 2 LIES OUTSIDE
      Y2      = MYI
      TTTCLIP = 1
      RETURN
 105  X1      = MXI                        ! POINT 1 LIES OUTSIDE
      Y1      = MYI
      TTTCLIP = 1
      RETURN
 109  MXI = XMIN
      MYI = MYOLD+FLOAT(MXI-MXOLD)/(MX-MXOLD)*(MY-MYOLD)
      GOTO 104
 111  MXI = XMAX
      MYI = MYOLD+FLOAT(MXI-MXOLD)/(MX-MXOLD)*(MY-MYOLD)
      GOTO 104
 112  MXI = XMIN
      MYI = MYOLD+FLOAT(MXI-MXOLD)/(MX-MXOLD)*(MY-MYOLD)
      GOTO 105
 113  MXI = XMAX
      MYI = MYOLD+FLOAT(MXI-MXOLD)/(MX-MXOLD)*(MY-MYOLD)
      GOTO 105
*
*   LINE GOES THROUGH WINDOW,STARTS AND ENDS OUTSIDE
*   TEST FIRST FOR HORIZONTAL OR VERTICAL
*
 114  IF(MX .EQ. MXOLD) THEN   ! VERTICAL
         Y1      = YMIN
         Y2      = YMAX
         TTTCLIP = 2
         RETURN
      ENDIF
      IF(MY .EQ. MYOLD) THEN   ! HORIZONTAL
         X1      = XMIN
         X2      = XMAX
         TTTCLIP = 2
         RETURN
      ENDIF
*
*  NEITHER VERTICAL NOR HORIZONTAL
*
      FM     = FLOAT(MY-MYOLD)/FLOAT(MX-MXOLD)
      RFM    = 1./FM
      CON    = MY-FM*MX
      MYJ(1) = YMAX
      MXJ(1) = RFM*(MYJ(1)-CON)
      MXJ(2) = XMIN
      MYJ(2) = CON+FM*MXJ(2)
      MXJ(3) = XMAX
      MYJ(3) = CON+FM*MXJ(3)
      MYJ(4) = YMIN
      MXJ(4) = RFM*(MYJ(4)-CON)
      KK     = 0
      IF(MXJ(1).GE.XMIN .AND. MXJ(1).LE.XMAX) THEN
         KK       = KK+1
         INDX(KK) = 1
      ENDIF
      IF(MYJ(2).GE.YMIN .AND. MYJ(2).LE.YMAX) THEN
         KK       = KK+1
         INDX(KK) = 2
      ENDIF
      IF(MYJ(3).GE.YMIN .AND. MYJ(3).LE.YMAX) THEN
         KK       = KK+1
         INDX(KK) = 3
      ENDIF
      IF(MXJ(4).GE.XMIN .AND. MXJ(4).LE.XMAX) THEN
         KK       = KK+1
         INDX(KK) = 4
      ENDIF
      IF(KK .NE. 2) THEN
         TTTCLIP = -2                       ! NOT VISIBLE
         RETURN
      ELSE 
         I1      = INDX(1)
         I2      = INDX(2)
         X1      = MXJ(I1)
         Y1      = MYJ(I1)
         X2      = MXJ(I2)
         Y2      = MYJ(I2)
         TTTCLIP = 2
         RETURN
      ENDIF
 124  MMX = MXOLD
      MMY = MYOLD
      GOTO 126
 125  MMX = MX
      MMY = MY
 126  FM  = FLOAT(MY-MYOLD)/FLOAT(MX-MXOLD)
      CON = MY-FM*MX
      MXI = XMIN
      MYI = CON+FM*MXI
 127  IF(MYI.LT.YMIN  .OR. MYI.GT.YMAX) THEN
         MYI = YMAX
         MXI = (MYI-CON)/FM
      ENDIF
      GOTO 140

 128  MMX = MXOLD
      MMY = MYOLD
      GOTO 130
 129  MMX = MX
      MMY = MY
 130  FM  = FLOAT(MY-MYOLD)/FLOAT(MX-MXOLD)
      CON = MY-FM*MX
      MXI = XMAX
      MYI = CON+FM*MXI
      GOTO 127
 132  MMX = MXOLD
      MMY = MYOLD
      GOTO 134
 133  MMX = MX
      MMY = MY
 134  FM  = FLOAT(MY-MYOLD)/FLOAT(MX-MXOLD)
      CON = MY-FM*MX
      MXI = XMIN
      MYI = CON+FM*MXI
 135  IF(MYI.LT.YMIN .OR. MYI.GT.YMAX) THEN
         MYI = YMIN
         MXI = (MYI-CON)/FM
      ENDIF
      GOTO 140

 136  MMX = MXOLD
      MMY = MYOLD
      GOTO 138

 137  MMX = MX
      MMY = MY

 138  FM  = FLOAT(MY-MYOLD)/FLOAT(MX-MXOLD)
      CON = MY-FM*MX
      MXI = XMAX
      MYI = CON+FM*MXI
      GOTO 135

 140  X1      = MMX
      X2      = MXI
      Y1      = MMY
      Y2      = MYI
      TTTCLIP = 1

      RETURN
      END
