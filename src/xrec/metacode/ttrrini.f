***S/P TTRRINI  -  ECRIRE LES 2 PREMIER RECORDS POUR (RRBX/RRUX)
#include "aaaamac.cdk"
*
      SUBROUTINE TTRRINI(NBR, NR, LCP, NCP, USER, JOBN, CDATE, CTIME,
     +                   PROTOCL)
#include "impnone.cdk"
      INTEGER            NBR ,NR ,LCP, NCP
      CHARACTER(LEN=8)       USER, JOBN, CDATE, CTIME, PROTOCL
*
*AUTEUR ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
**
      CHARACTER(LEN=80) LINE
*
      LINE = ' '
#if defined (CRAY)
LINE(1:32) = USER//JOBN//CDATE//CTIME
#else
      LINE(1:32) = USER//'RRBXRRUX'//CDATE//CTIME
#endif
      IF(PROTOCL .EQ. 'RRBX') THEN
         WRITE(44) NBR, NR, LCP, NCP
         WRITE(44) LINE
      ELSE
         WRITE(44,'(4I10)') NBR, NR, LCP, NCP
         WRITE(44,'(A80)')  LINE
      ENDIF

      RETURN
      END
