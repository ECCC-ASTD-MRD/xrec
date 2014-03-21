#if defined (RASTER) 
#if defined (UNIX) 
*** S/P TTTLINE - TRACER UNE LIGNE, EPAISSEUR = LARG
#include "aaaamac.cdk"
*
      SUBROUTINE TTTLINE( IVAR )
#include "impnone.cdk"
      INTEGER IVAR(0:*)
*AUTEUR      I.C. PITCHEN    JUIL. 86
*REVISION 001  M. LEPINE   - VERSION SUN (AOUT 88)
*REVISION 002  Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTLINE)
*   TRACER UNE LIGNE D'EPAISSEUR "LARG" ALLANT DE X1,Y1 A X2,Y2
*
**
#include "argum.cdk"
      INTEGER I, MASKD, DLTADY, BASY, X0, XS, XX0, DX, MINX, INDEXD,
     +        N, MASKS, RESIDA, MAXX, Y0, YS, YY0, DY, MINY, INDEXS,
     +        J, MASK0, RESIDB, MAXY, XD, YD

      N    = LARG
      DX   = X2 - X1
      DY   = ABS(Y2 - Y1)
      MINX = MAX(UMIN,X1 - N/2)
      MAXX = MIN(UMAX,X2 - N/2 + N-1)
      IF(Y2 .GT. Y1) THEN
         MINY = MAX(VMIN,Y1 - N/2)
         MAXY = MIN(VMAX,Y2 - N/2 + N-1)
      ELSE
         MINY = MAX(VMIN,Y2 - N/2)
         MAXY = MIN(VMAX,Y1 - N/2 + N-1)
      ENDIF
      DLTADY = ISIGN(WRAST,Y2 - Y1)
      IF(DY .GE. DX) THEN
         X0     = X1 - N/2
         XX0    = X0 + N-1
         RESIDB = DY/2
         RESIDA = RESIDB + DX*(N-1)
         IF(Y2 .GE. Y1) THEN
            Y0 = Y1 - N/2
         ELSE
            Y0 = Y1 - N/2 + N-1
         ENDIF
         BASY = Y0 * WRAST

         DO 20 I = 1,DY + N
            IF((Y0.GE.MINY) .AND. (Y0.LE.MAXY)) THEN
               XS = MAX(X0,MINX)
               XD = MIN(XX0,MAXX)
               IF(XD .GE. XS) THEN
                  MASKS  = NOT (MASK(IAND(XS,31)))
                  MASKD  = MASK(IAND(31,XD)+1)
                  INDEXS = ISHFT(XS, -5)
                  INDEXD = ISHFT(XD, -5)
                  MASK0  = MASKS
                  DO 10 J = INDEXS,INDEXD
                     IF (J .EQ. INDEXD) MASK0 = IAND(MASK0 ,MASKD)
                     IF(MODE .EQ. -1) THEN
                        IVAR(BASY+J) = IEOR(IVAR(BASY+J) , MASK0)
                     ELSE
                        IF(MODE .EQ. 0) THEN
                           IVAR(BASY+J) = IAND(NOT(MASK0), IVAR(BASY+J))
                        ELSE
                           IVAR(BASY+J) = IOR(IVAR(BASY+J) , MASK0)
                        ENDIF
                     ENDIF
                     MASK0 = MASK(32)
 10               CONTINUE
               ENDIF
            ENDIF
            BASY = BASY + DLTADY
            Y0 = Y0 + ISIGN(1,DLTADY)
            RESIDB = RESIDB - DX
            RESIDA = RESIDA - DX
            IF(RESIDB .LT. 0) THEN
               RESIDB = RESIDB + DY
               XX0    = XX0 + 1
            ENDIF
            IF(RESIDA .LT. 0) THEN
               RESIDA = RESIDA + DY
               X0     = X0 + 1
            ENDIF
 20         CONTINUE

      ELSE
         X0 = X1 - N/2
         IF(Y2 .GT. Y1) THEN
            Y0  = Y1 - N/2
            YY0 = Y0 + N-1
         ELSE
            Y0  = Y1 - N/2 + N-1
            YY0 = Y0  - (N-1)
         ENDIF
         RESIDB = DX/2
         RESIDA = RESIDB + DY*(N-1)
         INDEXS = X0/32
         IF(X0 .LT. 0 ) THEN
            MASK0  = ishft(iand(ishft(-1,-(IAND((-X0-1) , 31))), 1),
     %                     IAND((-X0-1) , 31))
            INDEXS = INDEXS - 1
         ELSE
             MASK0  = ishft(iand(ishft(-1,-( 31-IAND(X0 , 31))), 1), 31
     %                     -IAND(X0 , 31))
         ENDIF
         DO 40 I = 1,DX+N
            IF((X0.GE.MINX) .AND. (X0.LE.MAXX)) THEN
               IF(Y2 .GT. Y1) THEN
                  YS = MAX(Y0,  MINY)
                  YD = MIN(YY0, MAXY)
               ELSE
                  YS = MAX(YY0, MINY)
                  YD = MIN(Y0,  MAXY)
               ENDIF
               DO 30 J = YS,YD
                  IF(MODE .EQ. -1) THEN
                     IVAR(INDEXS+WRAST*J) = 
     +               IEOR(IVAR(INDEXS+WRAST*J), MASK0)
                  ELSE
                     IF(MODE .EQ. 0) THEN
                        IVAR(INDEXS+WRAST*J) = 
     +                  IAND(NOT(MASK0), IVAR(INDEXS+WRAST*J))
                     ELSE
                        IVAR(INDEXS+WRAST*J) = 
     +                  IOR(IVAR(INDEXS+WRAST*J), MASK0)
                     ENDIF
                  ENDIF
 30               CONTINUE
            ENDIF
            MASK0 = IAND(ISHFT(MASK0, -1), RMASK(31))
            X0    = X0 + 1
            IF(MASK0 .EQ. 0) THEN
               INDEXS = INDEXS + 1
               MASK0  = MASK(1)
            ENDIF
            RESIDB = RESIDB - DY
            RESIDA = RESIDA - DY
            IF(RESIDB .LT. 0) THEN
               RESIDB = RESIDB + DX
               YY0    = YY0 + ISIGN(1,Y2 - Y1)
            ENDIF
            IF(RESIDA .LT. 0) THEN
               RESIDA = RESIDA + DX
               Y0     = Y0 + ISIGN(1,Y2 -Y1)
            ENDIF
 40         CONTINUE
      ENDIF

      RETURN
      END
#endif
#endif
