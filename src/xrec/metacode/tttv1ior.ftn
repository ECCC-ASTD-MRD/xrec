#if defined (RASTER) 
#if defined (UNIX)
*** S/P TTTV1IOR TRACER LIGNE EPAISSEUR 1
#include "aaaamac.cdk"
*
      SUBROUTINE TTTV1IOR( IVAR )
#include "impnone.cdk"
      INTEGER  IVAR(0:*)
*
*AUTEURS    I.C. PITCHEN
*
*REVISION 001 M. LEPINE   - VERSION SUN (AOUT 88)
*         002 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTV1IOR)
*  TRACER UNE LIGNE D'EPAISSEUR 1 ALLANT DE X1,Y1 A X2,Y2
*
#include "argum.cdk"
      INTEGER INDEX, DX, DY, DLTADY, MASK0, NPTS, RESIDU, I, MASKR31

      DATA MASKR31 /O'17777777777'/

      INDEX  = WRAST * Y1 + X1/32
*     MASK0  = ISHFT(1 ,31-IAND(X1,31))
      MASK0 =ishft(iand(ISHFT(-1,-(31-IAND(X1,31))),1 ),31-IAND(X1,31))
      DX     = X2 - X1
      DY     = Y2 - Y1
      DLTADY = ISIGN(WRAST, DY)
      NPTS   = MAX(DX, ABS(DY)) + 1
      DY     = ABS( DY )

      IF(DY .GE. DX) THEN
         RESIDU = DY/2
         DO 10 I = 1,NPTS
            IF(MODE .EQ. -1) THEN
               IVAR(INDEX) = IEOR(IVAR(INDEX) , MASK0)
            ELSE
               IF(MODE .EQ. 0) THEN
                  IVAR(INDEX) = IAND(NOT(MASK0), IVAR(INDEX))
               ELSE
                  IVAR(INDEX) = IOR(IVAR(INDEX) , MASK0)
               ENDIF
            ENDIF
            RESIDU = RESIDU - DX
            INDEX  = INDEX + DLTADY
            IF(RESIDU .LT. 0) THEN
               RESIDU = RESIDU + DY
               MASK0  = IAND(ISHFT(MASK0,-1) , MASKR31)
               IF(MASK0 .EQ. 0 ) THEN
                  INDEX = INDEX + 1
                  MASK0 = MASK(1)
               ENDIF
            ENDIF
 10         CONTINUE
      ELSE
         RESIDU = DX/2
         DO 20 I =1,NPTS
            IF(MODE .EQ. -1) THEN
               IVAR(INDEX) = IEOR(IVAR(INDEX) , MASK0)
            ELSE
               IF(MODE .EQ. 0) THEN
                  IVAR(INDEX) = IAND(NOT(MASK0), IVAR(INDEX))
               ELSE
                  IVAR(INDEX) = IOR(IVAR(INDEX) , MASK0)
               ENDIF
            ENDIF
            RESIDU = RESIDU - DY
            MASK0  = IAND(ISHFT(MASK0,-1) , MASKR31)
            IF(MASK0 .EQ. 0 ) THEN
               INDEX = INDEX + 1
               MASK0 = MASK(1)
            ENDIF
            IF(RESIDU .LT. 0) THEN
               RESIDU = RESIDU + DX
               INDEX  = INDEX + DLTADY
            ENDIF
 20       CONTINUE
       ENDIF

       RETURN
       END
#endif
#endif
