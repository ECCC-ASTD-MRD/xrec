#if defined (RASTER) 
***S/R TTPUTID
*
#include "aaaamac.cdk"
      SUBROUTINE TTPUTID( MSGID,    BUFID )
#include "impnone.cdk"
      INTEGER             MSGID(8), BUFID(7)
*
*AUTEUR       ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
#include "mesgid.cdk"
#if defined (CRAY) 
*MODULE
      EXTERNAL ASC2DSC, WRITE
*
**
      READ(MESSAGE,'(8A8)') MSGID
      CALL ASC2DSC(MSGID, BUFID)
      BUFID(7) = 0
      CALL WRITE(LJUST( RAZZ2 ), BUFID, 7, 28)
      WRITE(LJUST(RAZZ3)) MSGID
#endif
#if defined (UNIX) 
      WRITE(99,'(A64)') MESSAGE
#endif
      RETURN
      END
#endif
