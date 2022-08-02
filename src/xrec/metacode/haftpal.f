#if defined (RASTER) 
*** S/P HAFTPAL - ETABLIR LA PALETTE DE PATRONS
#include "aaaamac.cdk"
*
      SUBROUTINE HAFTPAL
#include "impnone.cdk"
*
*AUTEUR       M. LEPINE   -  JUIN 85
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(HAFTPAL)
*     SERT A DEFINIR LA NOUVELLE TABLE DE PATRONS AINSI QUE LEURS
*     COULEURS DE "FOREGROUND"/"BACKGROUND" CORRESPONDANTES
*
**
#include "params.cdk"
#include "palpat.cdk"
      INTEGER IND, I, LNG
*
*     IND   =  1   CORRESPOND AU NUMERO DE PATRON
*           =  2   CORRESPOND A LA COULEUR FOREGROUND
*           =  3   CORRESPOND A LA COULEUR BACKGROUND
*

      LNG = (IAND(BPAIRS(1) , 255)) - 2
      IND =  IAND(BPAIRS(2) , 255)

      DO 10 I = 1,LNG
         HPALET(IND,I) = IAND(ISHFT(BPAIRS(3+(ISHFT(I-1,-1))),
     +                        -8*(IAND(1, I))), 255)
         IF((IND.EQ.2) .OR. (IND.EQ.3)) 
     +      HPALET(IND,I) = MAX(0,MIN(HPALET(IND,I),16))
         IF((IND.EQ.3) .AND. (HPALET(2,I).EQ.0) .AND.
     +      (HPALET(3,I).EQ.0)) THEN
            HPALET(2,I) = 1
            HPALET(3,I) = 16
         ENDIF
 10      CONTINUE

      RETURN
      END
#else
*     SUBROUTINE HAFTPAL
*     RETURN
*     END
#endif
