*** S/P TTTMAPC
#include "aaaamac.cdk"
*
      SUBROUTINE TTTMAPC(IUN, FIC)
#include "impnone.cdk"
      INTEGER       IUN
      CHARACTER*(*) FIC
*
*AUTEUR ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*IMPLICITES
#include "params.cdk"
*
#if defined (C910) 
#if defined (NATIF) 

      EXTERNAL MAPCOL
      INTEGER  I, R(0:255), G(0:255), B(0:255), NUMERO, RED, GREEN, BLUE

*     BLANC
      R(0) = 255
      G(0) = 255   
      B(0) = 255
*     NOIR
      R(1) = 000
      G(1) = 000
      B(1) = 000
*     ROUGE
      R(2) = 230
      G(2) = 000
      B(2) = 000
*     CYAN
      R(3) = 000
      G(3) = 150
      B(3) = 210
*     JAUNE
      R(4) = 255
      G(4) = 250
      B(4) = 000
*     MAGENTA
      R(5) = 255
      G(5) = 000
      B(5) = 151
*     VERT
      R(6) = 000
      G(6) = 140
      B(6) = 000
*     BLEU
      R(7) = 000
      G(7) = 000
      B(7) = 150

      DO 10 I = 8,255
         R(I) = R(MOD(I,8))
         G(I) = G(MOD(I,8))
         B(I) = B(MOD(I,8))
 10      CONTINUE

      DO 20 I = 0,15
         TABCOL(I) = TABCOL(I)+64
         CALL MAPCOL(I+64, R(I), G(I), B(I))
 20      CONTINUE

      DO 30 I = 17,255
         TABCOL(I) = TABCOL(MOD(I,16))
*        CALL MAPCOL(I+256, R(I), G(I), B(I))
 30      CONTINUE

      IF(FIC .NE. ' ') THEN
 40      READ(IUN,*,END=50) NUMERO, RED, GREEN, BLUE
         CALL MAPCOL(NUMERO+64, RED, GREEN, BLUE)
         GOTO 40
      ENDIF
#endif
#endif

 50   RETURN
      END
