#if defined (RASTER) 
#if defined (CRAY) 
***S/P TTTPRE2   REMETTRE INDICE A 1, OUVRIR FICHIER RAZZ2 OU RAZZ3
*
SUBROUTINE TTTPRE2
implicit none
#include "cpress.cdk"
DATA IDFLAG /.FALSE./
*
IF (ASKI)
 $(
    MOT = 1
    NBT = 0
    IDFLAG = .TRUE.
 $)
ELSE
  CALL TTTPREA
RETURN
END
#endif
#if defined (C910) 
***S/P TTTPRE2   REMETTRE INDICE A 1, OUVRIR FICHIER RAZZ2
#include "aaaamac.cdk"
*
      SUBROUTINE TTTPRE2
#include "impnone.cdk"
*
*AUTEUR M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
#include "pact.cdk"
*
      PTR = 1

      RETURN
      END
#endif
#endif
