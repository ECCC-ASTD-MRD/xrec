#if defined (RASTER) 
*** S/R ASC2DSC
#include "aaaamac.cdk"
      SUBROUTINE ASC2DSC(ASCII, DISPLAY)
#include "impnone.cdk"
      INTEGER    ASCII(8),      DISPLAY(6)
*AUTEUR ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*MODULES
#if defined (CRAY) 
      EXTERNAL UNPACK
#endif
      EXTERNAL IIPAK
*
**
      INTEGER BUF(64), I, DPCT(32:95)

*     ASCII-32 A DISPLAY CODE
      DATA  DPCT /
     + 45, 54, 52, 48, 43, 51, 55, 56, 41, 42, 39, 37, 46, 38, 47, 40,
     + 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 45, 63, 58, 44, 59, 57,
     + 60,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
     + 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 49, 61, 50, 62, 53/

#if defined (CRAY) 
      CALL UNPACK(ASCII,8,BUF,64)
#endif
      DO 10 I = 1,64
         IF (BUF(I) .GT. 95) BUF(I) = BUF(I) - 32
         BUF(I) = DPCT(MAX(32, BUF(I)))
 10      CONTINUE
      CALL IIPAK(BUF, DISPLAY, 64, 1, -6, 0, 1)

      RETURN
      END
#endif
