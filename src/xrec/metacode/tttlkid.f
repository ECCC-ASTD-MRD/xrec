#if defined (RASTER) 
*** S/P TTTLKID - PREPARER UN IDENTIFICATEUR DE FRAME (LINK...)
#include "aaaamac.cdk"
*
      SUBROUTINE TTTLKID( TNAME )
#include "impnone.cdk"
      CHARACTER(LEN=8) TNAME
*
*AUTEUR       M. LEPINE   -  AOUT 86
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTLKID)
*     PREPARER UN IDENTIFICATEUR DE FRAME POUR LES DEVICES DE TYPE
*     LINK
*MODULE
      EXTERNAL DPC2ASC
      INTEGER  DPC2ASC
*
**
#include "mesgid.cdk"
#include "lnkpak.cdk"

      INTEGER  I, JJJ
      LOGICAL  DEJAVU
      DATA DEJAVU /.FALSE./
*
      IF( DEJAVU ) RETURN
      DEJAVU = .TRUE.
      IF(TNAME .NE. '        ') MESSAGE(1:8) = TNAME
      BUF(1) = 0
      BUF(2) = 34
      LAST   = 34
      DO 10 I = 1,32
#if !defined (UNIX) 
         READ(MESSAGE(I:I),'(R1)') JJJ
         BUF(2+I) = DPC2ASC(JJJ)
#else
         BUF(2+I) = ICHAR(MESSAGE(I:I))
#endif
 10      CONTINUE

         RETURN
         END
#endif
