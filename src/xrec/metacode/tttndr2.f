#if defined (RASTER) 
#if defined (CRAY) 
***S/P TTTNDR2   FERMER FICHIER RASTER, FLUSH RASTER
*
SUBROUTINE TTTNDR2
implicit none
INTEGER EGAL,BLANK
#include "cpress.cdk"
*
DATA EGAL /"="R/
DATA BLANK /" "R/
IF (ASKI)
  $(
    PUTCAR(EGAL)
    DO I = 1,128
      PUTCAR(BLANK)
  $)
ELSE
  CALL TTTNDRA
RETURN
END
#endif
#if defined (UNIX)
***S/P TTTNDR2   FERMER FICHIER RASTER, FLUSH RASTER
*
#include "aaaamac.cdk"
      SUBROUTINE TTTNDR2
#include "impnone.cdk"
*
*AUTEUR I.C.PITCHEN
*
*REVISION 001  - Y. BOURASSA -  MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*
*OBJET(TTTNDR2)
*
#include "ivar.cdk"
#include "pact.cdk"
*
      INTEGER EGAL
#define Arg1 transfer('=   ',EGAL)
#include "putcar.cdk"

      CALL  FLUSHIT

      RETURN
      END
#endif
#endif
*
