*** S/P TTTFNTX - CHANGER LA PALETTE DES FONTES
#include "aaaamac.cdk"
*
      SUBROUTINE TTTFNTX
#include "impnone.cdk"
*
*AUTEUR       M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTFNTX)
*     CHANGER LA PALETTE DES FONTES DE CARACTERES
*
**
#include "params.cdk"
      INTEGER I

      IF(IAND(BPAIRS(1), 255) .eq. 20) THEN
         DO 10 I = 0,9
            TABFONT(I) = IAND(BPAIRS(I+2), 255)
 10         CONTINUE
         IFNTCAS = TABFONT(0)
      ELSE
         WRITE(IPUNIT,*) ' *** MAUVAISE LONGUEUR - TTTFNTX ***'
      ENDIF

      RETURN
      END
