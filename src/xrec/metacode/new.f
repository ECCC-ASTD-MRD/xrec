***F/N BLACKLST
#include "aaaamac.cdk"
      Function BLACKLST( USER )
#include "impnone.cdk"
      LOGICAL       BLACKLST
      CHARACTER * 8 USER
*
*AUTEUR ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*

#if defined (C910) 
*MODULES
      EXTERNAL        LONGUEUR, GETENVC
      INTEGER         LONGUEUR
*
**
      INTEGER         FNOM, L, IER
      CHARACTER * 8   USRLIST
      CHARACTER * 128 DATAREP

      CALL GETENVC('ARMNLIB', DATAREP)
      L        = LONGUEUR( DATAREP )
      IER      = FNOM(666, DATAREP(1:L) / / '/usr/blacklist', 'SEQ+OLD', 0)
      BLACKLST = .FALSE.
      IF (IER .NE. 0) RETURN

 10   READ(666, '(A8)', END=100, ERR=100) USRLIST
      IF (USER .EQ. USRLIST) THEN
         BLACKLST = .TRUE.
         GOTO 100
      ELSE
         GOTO 10
      ENDIF

 100  CONTINUE
#else
**
      BLACKLST = .FALSE.
#endif
      RETURN
      END
