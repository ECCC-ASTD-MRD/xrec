***S/P WRITRAW
#include "aaaamac.cdk"
*
      SUBROUTINE WRITRAW(NLIGNE, PROTOCL, IVAR2, REVERSE)
#include "impnone.cdk"
      INTEGER     NLIGNE
      LOGICAL     REVERSE
      CHARACTER(LEN=8) PROTOCL
*
#include "params.cdk"
      INTEGER     IVAR2(LCP,NCP,0:*)
*
*IMPLICITES
*
#include "mesgid.cdk"
#include "argum.cdk"
#include "ivar.cdk"
*
*MODULES
      EXTERNAL WAOPEN, WAWRIT, WACLOS, PERMUT
*
**

      CHARACTER(LEN=1) HEXDA(0:15)
      INTEGER     TEMP(512), I, J, K, L, POS, MOT, NWORD0, BUF(8)
#if defined (UNIX)  
      INTEGER     RED(0:7), GREEN(0:7), BLUE(0:7)
      DATA RED   / 255, 000, 255, 000, 255, 255, 000, 000 /
      DATA GREEN / 255, 000, 000, 255, 255, 000, 255, 000 /
      DATA BLUE  / 255, 000, 000, 255, 000, 255, 000, 255 /
#endif

      DATA NWORD0 /1/
      DATA HEXDA  /'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     +             'A', 'B', 'C', 'D', 'E', 'F'/

      IF( REVERSE ) CALL PERMUT(IVAR2, WRAST, NLIGNE)

      IF(PROTOCL .EQ. 'RRBX') THEN
         WRITE(44) MESSAGE
         DO 10 K = 0,NLIGNE-1
            WRITE(44) ((IVAR2(I,J,K),I=1,LCP),J=1,NCP)
 10         CONTINUE

      ELSE
         IF(PROTOCL .EQ. 'RRUX') THEN
            WRITE(44,'(A80)') MESSAGE
            DO 50 K = 0,NLIGNE-1
            DO 50 J = 1,NCP
               POS = 1
               I   = 1
 20            IF(I .LE. (DEVWIDT+1)/4) THEN
                  MOT = IVAR2(POS,J,K)
                  DO 30 L = BITUSED-4,0,-4
                     TEMP(I) = IAND(15,ISHFT(MOT,-L))
                     I = I + 1
 30                  CONTINUE
                  POS = POS + 1
                  GOTO 20
               ENDIF
 40            WRITE(44,'(80A1)') (HEXDA(TEMP(I)),I=1,(DEVWIDT+1)/4)
 50            CONTINUE
         ELSE
            IF(PROTOCL .NE. 'SUNSCR') THEN
               IF(PROTOCL .EQ. 'SUNRST') THEN
                  BUF(1) = 1504078485 ! 59A66A95 Hex
                  BUF(2) = LCP*NCP*BITUSED
                  BUF(3) = NLIGNE
                  BUF(4) = 1
                  BUF(5) = (LCP*NCP*BITUSED) * NLIGNE / 8
                  BUF(6) = 1
                  BUF(7) = 0
                  BUF(8) = 0
                  CALL WAOPEN(44)
                  CALL WAWRIT(44,BUF,1,8)
                  CALL WAWRIT(44,IVAR2,9,LCP*NCP*NLIGNE)
                  CALL WACLOS(44)
               ELSEIF(PROTOCL .EQ. 'RAW') THEN
                  CALL WAOPEN(19)  
                  CALL WAWRIT(19, IVAR, NWORD0, LCP*NCP*NLIGNE)
                  NWORD0 = NWORD0 + LCP*NCP*NLIGNE
                  CALL WACLOS(19)
               ENDIF
            ENDIF
         ENDIF
      ENDIF

      RETURN
      END
