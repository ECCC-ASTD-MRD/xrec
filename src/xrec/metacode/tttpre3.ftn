#if defined (RASTER) 
*** S/P TTTPRE3/TTTNDR3 
#include "aaaamac.cdk"
*
      SUBROUTINE TTTPRE3
#include "impnone.cdk"
*
*AUTEUR ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*OBJET  APPELER PRE2 ET NDR2 SANS DOUBLE APPEL
*
*LANGUAGE   - FTN77
*
*MODULES
      EXTERNAL TTTPRE2, TTTNDR2
*
**

      INTEGER  STATUS
      COMMON /PLOTZ/ STATUS
      DATA STATUS /0/

      IF(STATUS .EQ. 0) THEN
         CALL TTTPRE2
         STATUS = 1
      ENDIF
      RETURN

      ENTRY TTTNDR3
      IF(STATUS .GE. 1) THEN
         CALL TTTNDR2
         STATUS = 0
      ENDIF

      RETURN
      END
#endif
