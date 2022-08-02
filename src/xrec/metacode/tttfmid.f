***S/R TTTFMID
#include "aaaamac.cdk"
*
      SUBROUTINE TTTFMID
#include "impnone.cdk"
*
*AUTEUR  ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
#if defined (RASTER) 
#include "params.cdk"
#include "mesgid.cdk"

      INTEGER LNG, I

      LNG = (IAND(BPAIRS(1) , 255))/ 2 + 1
#if defined (CRAY) 
      WRITE(MESSAGE,'(40R2)') (BPAIRS(I),I=3,LNG)
#endif
#if defined (UNIX)
      WRITE(MESSAGE,'(40A2)') (ishft(iand(ishft(-1,-(16)),BPAIRS(I))
     %,16),I=3,LNG)
#endif
#endif
      RETURN
      END
