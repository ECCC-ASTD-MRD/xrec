*** S/P TTTCNTR - CENTRER UNE CHAINE DE CARACTERES
#include "aaaamac.cdk"
*
      SUBROUTINE TTTCNTR( FONTNB )
#include "impnone.cdk"
      INTEGER             FONTNB
*
*AUTEUR     M. VALIN / M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTCNTR)
*    CENTRER PAR RAPPORT A UN POINT X,Y UNE CHAINE DE CARACTERES
*
#include "params.cdk"
#include "pwryp.cdk"
#include "carac.cdk"
*
*MODULE
      EXTERNAL TTTDRAW
*
**
      LOGICAL  IAVFONT, FIRSTC
      REAL     LENGTH, FACTEUR, SPXY, PX, PY
      INTEGER  IDX, LEN, INBBYT, CHAR, BIGGEST, ICONT, L, NBCHAR,
     +         IDY, MOT, IFPREC, CORX, ISAVCAS, VAL(0:5), L0
      DATA VAL /0,1,2,3,4,5/
*
*     LE CENTRAGE S'EFFECTUERA EN FONCTION DE
*
*            CENTRAGE EN X
*            -------------
*     SI CNTRX = 0     CENTRE DU PREMIER CARACTERE
*              = 1     CENTRE DE LA CHAINE COMPLETE
*              = 2     BORD GAUCHE DU PREMIER CARACTERE
*              = 3     CENTRE DU DERNIER CARACTERE
*              = 4     BORD DROIT DU DERNIER CARACTERE
*
*            CENTRAGE EN Y
*            -------------
*     SI CNTRY = 0     CENTRE DU CARACTERE
*              = 1     BAS DU CARACTERE
*              = 2     HAUT DU CARACTERE
*              = 3     BAS DE LA BOITE
*              = 4     HAUT DE LA BOITE
*
*
      ISAVCAS = IFNTCAS
      FONTNB  = IFNTCAS
      IF((CNTRY.EQ.1) .AND. (CNTRX.EQ.2)) RETURN

      LEN     = IAND(255, BPAIRS(1))
      ICONT   = 2
      INBBYT  = -1
      NBCHAR  = 0
      LENGTH  = 0.
      FACTEUR = 1.0
      IAVFONT = .FALSE.
      FIRSTC  = .TRUE.
      BIGGEST = 0


 10   IF(NBCHAR .LT. LEN) THEN
         NBCHAR = NBCHAR+1
         INBBYT = INBBYT+1
*        EXTRACTION DU CARACTERE ET DES BITS DE CONTROLE.
         CHAR  = IAND(255, ISHFT(BPAIRS(ICONT),-8*(1-(IAND(INBBYT,1)))))
         ICONT = ICONT + (IAND(INBBYT , 1))
 
*        OPCODE ENTRE 0 ET 32
         GOTO(10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,20,
     +        20,20,20,20,20,20,20,20,20,20,10,10,30,10,40,10) CHAR+1
        
*        CHAR PLUS HAUT QUE 95: SINON C'EST UN CARACTERE
         GOTO(10,50,50,50,50,50,50,50,50,50,50,10,10,10,10,10,
     +        10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10) CHAR-95
         GOTO 60
 
 20      IFNTCAS = TABFONT(CHAR-16)          !  DE 20B A 31B
         IAVFONT = .FALSE.
         GOTO 10
 30      FACTEUR = FACTEUR * 0.7071068
         GOTO 10
 40      FACTEUR = FACTEUR * 1.4142136
         GOTO 10
 50      IFPREC  = IFNTCAS                   ! DE 140B A 152B
         IFNTCAS = IAND(CHAR , 15)
         IFNTCAS = TABFONT(MOD(IFNTCAS,10))
         IAVFONT = .TRUE.
         GOTO 10

 60      IF((.NOT.FIXFLG) .AND. (IORN.EQ.0)) THEN
            MOT = IFNTCAS
            L   = IVL(MOT,CHAR) * FACTEUR
         ELSE
            L   = 20 * FACTEUR
         ENDIF
         LENGTH = LENGTH + L
         IF( FIRSTC ) THEN
            L0     = L
            FIRSTC = .FALSE.
         ENDIF
         IF(IVH(IFNTCAS,2) .LT. BIGGEST) THEN
            BIGGEST = IVH(IFNTCAS,2)
            FONTNB  = IFNTCAS
         ENDIF
         IF( IAVFONT ) THEN
            IFNTCAS = IFPREC
            IAVFONT = .FALSE.
         ENDIF
         IF((.NOT.FIXFLG) .AND. (IORN.EQ.0) .AND. (CNTRX.EQ.0)) GO TO 70
         GO TO 10
      ENDIF

 70   IF( UPPERC ) THEN
         VAL(3) = 3
      ELSE
         VAL(3) = 5
      ENDIF

      IDY = IVH(FONTNB, VAL(CNTRY))

      IF((CNTRY.EQ.3) .OR. (CNTRY.EQ.4)) THEN
         CORX = 4
      ELSE
         CORX = 0
      ENDIF

      L = CNTRX+1
      IF(L.LT.1 .OR. L.GT.5) GO TO 80
      IF(IORN .NE. 0) THEN
         LENGTH = LENGTH / 20.0
         IF(L .EQ. 1) THEN
            IDX  = -8
            SPXY = 0.
         ELSEIF(L .EQ. 2) THEN
            IDX  = -8
            SPXY = -LENGTH/2. + 0.5
         ELSEIF(L .EQ. 3) THEN
            IDX  = CORX
            SPXY = 0.
         ELSEIF(L .EQ. 4) THEN
            IDX  = -8
            SPXY = -LENGTH + 1.
         ELSE
            IDX  = -16-CORX
            SPXY = -LENGTH + 1.
         ENDIF
      ELSE
         IF(L .EQ. 1) THEN
            IDX  = -L0/2.0
            SPXY = 0.
         ELSEIF(L .EQ. 2) THEN
            IDX  = -LENGTH/2.0
            SPXY = 0.
         ELSEIF(L .EQ. 3) THEN
            IDX  = CORX
            SPXY = 0.
         ELSEIF(L .EQ. 4) THEN
            IDX  = -(LENGTH - L/2.0)
            SPXY = 0.
         ELSE
            IDX  = -LENGTH-CORX
            SPXY = 0.
         ENDIF
      ENDIF

      PX = CURX + IDX*X1 + IDY*X2 + .5 + SPXY*SPX
      PY = CURY + IDX*Y1 + IDY*Y2 + .5 + SPXY*SPY
      CALL TTTDRAW(PX, PY, 0, 0, 0)
      
 80   IFNTCAS = ISAVCAS

      RETURN
      END
