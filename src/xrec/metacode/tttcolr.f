*** S/P TTTCOLR  -  CHANGER LA PALETTE DE COULEURS
#include "aaaamac.cdk"
*
      SUBROUTINE TTTCOLR
#include "impnone.cdk"
*
*AUTEUR       M. LEPINE   -  PRINTEMPS 85
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTCOLR)
*     DEFINIR LA NOUVELLE PALETTE DES COULEURS
*
**
*NOTE : UNE COULEUR = A -1 INDIQUE UNE COULEUR TRANSPARENTE
*
#include "params.cdk"
*
**
      INTEGER I

      IF(IAND(BPAIRS(1), 255) .EQ. 34) THEN
         DO 10 I = 0,16
            IF((BPAIRS(I+2).LT.32768) .OR. (BPAIRS(I+2).GT.32783)) THEN 
               TABCOL(I) = -1
            ELSE
               TABCOL(I) = IAND(MOD((BPAIRS(I+2)-32768),2** NCP) ,15)
            ENDIF
 10         CONTINUE

         IF(DEVICE .EQ. CALCOMP) THEN
            DO 20 I = 0,16
               TABCOL(I) = COLCOMP(TABCOL(I))
 20            CONTINUE
         ENDIF
      ELSE
         WRITE(IPUNIT,*) ' *** MAUVAISE LONGUEUR - TTTCOLR'
      ENDIF

      RETURN
      END
