#if defined (RASTER) 
#if defined (CRAY) 
SUBROUTINE COMPRES2(Z,WRAST,NBYT,NBZERO,NLIGNES,DOUB)
implicit none
INTEGER WRAST,NBYT,NBZERO,NLIGNES
INTEGER Z(0:WRAST-1,NLIGNES)
LOGICAL DOUB
*IMPLICITES
#include "cpress.cdk"
*
IF (ASKI)
  CALL COMPRESB(Z,WRAST,NBYT,NBZERO,NLIGNES,DOUB)
ELSE
  CALL COMPRESA(Z,WRAST,NBYT,NBZERO,NLIGNES,DOUB)
RETURN
END
*** S/P COMPRESB - COMPRESSION DE RASTERS
*
SUBROUTINE COMPRESB(Z,WRAST,NBYT,NBZERO,NLIGNES,DOUB)
implicit none
INTEGER WRAST,NBYT,NBZERO,NLIGNES
INTEGER Z(0:WRAST-1,NLIGNES)
LOGICAL DOUB
*
*IMPLICITES
#include "cpress.cdk"
*
INTEGER TEMP(1000),CODE(0:255),DOUBLE4(0:15)
INTEGER COUNT,CC,NN(6),FF(8),XORON,XOROFF,C1,C2,NBYTES,MOT1,MOT2
*
DATA MOT1 /"1YZPLOT$"/
DATA MOT2 /"ZW"/
DATA NN /"O"R, "P"R, "Q"R, "R"R, "S"R, "T"R/
DATA FF /"G"R, "H"R, "I"R, "J"R, "K"R, "L"R, "M"R, "N"R/
DATA DOUBLE4 /"00"R, "03"R, "0C"R, "0F"R, "30"R, "33"R, "3C"R, "3F"R,
              "C0"R, "C3"R, "CC"R, "CF"R, "F0"R, "F3"R, "FC"R, "FC"R/
DATA XORON /"X"R/
DATA XOROFF /"Y"R/
DATA MOT /1/
DATA NBT /0/
NBYTES = NBYT - 1
IF (IDFLAG)
  $(
    WRITE("RAZZ3"L) MOT1,MOT2
    IDFLAG = .FALSE.
    PUTCAR(1H  )
  $)
IF (.NOT. DOUB)
  DO I = 0,255
    $(
    ENCODE(2,'(Z2)',CODE(I)) I
    CODE(I) = SHIFTR(CODE(I),48)
  $)
ELSE
  DO I = 0,15
    CODE(I) = DOUBLE4(I)
PRINT *,' *** DEBUG COMPRES2 WRAST , NBYTES, NLIGNES , NBZERO = ',
WRAST , NBYTES, NLIGNES , NBZERO, DOUB
NW = (NBYTES+7)/8
PRINT *,' *** DEBUG NW = ',NW


IF (NBZERO <> 0)
  $(
    PUTCAR(XOROFF)
    IF (DOUB)
      DO I = 1,NLIGNES * 2
        PUTCAR("Z"R)
    ELSE
      DO I = 1,NLIGNES
        PUTCAR("Z"R)
    RETURN
  $)
PUTCAR(XORON)
CALL SECOND(T0)
DO NL = NLIGNES,2,-1
  DO I = 0,MOD(NW-1,64)
    Z(I,NL) = Z(I,NL) .XOR. Z(I,NL-1)
CALL SECOND(T1)
PRINT *,' *** DEUBG TEMPS POUR .XOR. ',T1-T0


CALL SECOND(T2)
DO NL = 1,NLIGNES
  $(
    IF (DOUB)
      CALL UNPACK(Z(0,NL),4,TEMP,NW*16)
    ELSE
      CALL UNPACK(Z(0,NL),8,TEMP,NW*8)
    TEMP(1+NBYTES) = -1
    I = 1
    WHILE (I <= NBYTES)
      $(
        C1 = SHIFTR(CODE(TEMP(I)),8)
        C2 = IAND(CODE(TEMP(I)),255)
        PUTCAR(C1)
        PUTCAR(C2)
        COUNT = 0
        WHILE (TEMP(I) == TEMP(I+1))
          $(
            COUNT = COUNT + 1
            I = I + 1
          $)
        IF (COUNT <> 0)
          $(
            WHILE(COUNT > 111)
              $(
                PUTCAR("T"R)
                COUNT = COUNT - 96
              $)
            CC = SHIFTR(COUNT,4)
            IF (CC <> 0)
              PUTCAR("N"R+CC)
            COUNT = IAND(COUNT,15)
            CC = SHIFTR(COUNT,3)
            IF (CC <> 0)
              PUTCAR("N"R)
            CC = IAND(COUNT,7)
            IF (CC <> 0)
              PUTCAR("F"R+CC)
          $)
        I = I + 1
      $)  #END WHILE
    PUTCAR("Z"R)
  $)
CALL SECOND(T3)
PRINT *,' *** DEUBG TEMPS POUR UNPACK ET CODE',T3-T2
RETURN
END
#endif

#if defined (UNIX) 
*** S/P CMPRES2 - COMPRESSION DE RASTERS
*
#include "aaaamac.cdk"
*
      SUBROUTINE CMPRES2(Z,      NLIGNE, DOUBLE, ZEROS)
#include "impnone.cdk"
      INTEGER            Z(0:*), NLIGNE
      LOGICAL            DOUBLE, ZEROS
*
*AUTEUR     I.C. PITCHEN  - VERSION CDC, JUIL 86
*
*REVISION 001 M. LEPINE   - VERSION SUN
*         002 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*         003 M. Lepine   - JAN 99 Bug fix .and. au lieu de .or. dans comp
*
*LANGUAGE   - FTN77
*
*OBJET(CMPRES2)
*  COMPRESSION DE RASTERS
*
*ARGUMENTS : Z       -  ARRAY DE RASTERS A ETRE COMPRESSES
*            NLIGNE  -  NOMBRE DE LIGNES (RASTERS) A COMPRESSER
*            DOUBLE  -  INDIQUE S'IL DOIT Y AVOIR DEDOUBLEMENT DES RASTERS
*            ZEROS   -  INDIQUE S'IL SAGIT DE LIGNES VIDES SEULEMENT
#include "argum.cdk"
#include "pact.cdk"

      INTEGER I, XORON, PREV( 0:1000 ), MODH128, IND0, KW, XOROFF,
     +        K, TEMP1, RAST1(MAXWR*8), DOUBLE4(0:15), MOLD

      DATA PTR      / 1 /
      DATA RESULT(1)/1H  /
      DATA XORON    /1HX /
      DATA XOROFF   /1HY /
      DATA DOUBLE4  / 0,   3,  12,  15,  48, 51, 60, 63, 192, 195, 204,
     +              207, 240, 243, 252, 255 /

      MODH128 = 128

 100  CONTINUE
      IF( ZEROS ) THEN
#define Arg1 XOROFF
#include "putcar.cdk"
         IF( DOUBLE ) THEN
            DO 10 I = 1,NLIGNE * 2
#define Arg1 TRANSFER('Z    ',MOLD)
#include "putcar.cdk"
 10            CONTINUE
         ELSE
            DO 20 I = 1,NLIGNE
#define Arg1 TRANSFER('Z    ',MOLD)
#include "putcar.cdk"
 20            CONTINUE
         ENDIF
         RETURN
      ENDIF
   
      DO 30 I = 0,WRAST-1
         PREV( I ) = 0
 30      CONTINUE
#define Arg1 XOROFF
#include "putcar.cdk"
      DO 40 I = 1, WRAST*4
         RAST1(I) = 0
 40      CONTINUE
      DO 60 K = 0, NLIGNE-1
         KW   = K*WRAST
         IND0 = 1
*        ATTENTION PASSE CROCHE POUR LE CA\LCOMP
         DO 50 I = 0, MOD(WRAST-1,MODH128)
            TEMP1   = IEOR(Z( I+KW ) , PREV(I))
            PREV(I) = Z( I + KW )
            IF( DOUBLE ) THEN
               RAST1(IND0)   = DOUBLE4(IAND(ISHFT(TEMP1,-28), 15))
               RAST1(IND0+1) = DOUBLE4(IAND(ISHFT(TEMP1,-24), 15))
               RAST1(IND0+2) = DOUBLE4(IAND(ISHFT(TEMP1,-20), 15))
               RAST1(IND0+3) = DOUBLE4(IAND(ISHFT(TEMP1,-16), 15))
               RAST1(IND0+4) = DOUBLE4(IAND(ISHFT(TEMP1,-12), 15))
               RAST1(IND0+5) = DOUBLE4(IAND(ISHFT(TEMP1, -8), 15))
               RAST1(IND0+6) = DOUBLE4(IAND(ISHFT(TEMP1, -4), 15))
               RAST1(IND0+7) = DOUBLE4(IAND(TEMP1, 15) )
               IND0          = IND0 + 8
            ELSE
               RAST1(IND0)   = IAND(ISHFT(TEMP1,-24) , 255)
               RAST1(IND0+1) = IAND(ISHFT(TEMP1,-16) , 255)
               RAST1(IND0+2) = IAND(ISHFT(TEMP1, -8) , 255)
               RAST1(IND0+3) = IAND(TEMP1, 255)
               IND0          = IND0 + 4
            ENDIF
 50         CONTINUE
         CALL COMP( RAST1, DOUBLE )
#define Arg1 TRANSFER('Z    ',MOLD)
#include "putcar.cdk"
#define Arg1 XORON
#include "putcar.cdk"
         IF( DOUBLE ) THEN
#define Arg1 TRANSFER('Z    ',MOLD)
#include "putcar.cdk"
         ENDIF
 60      CONTINUE

      RETURN

      ENTRY CMPRES2A( Z, NLIGNE, DOUBLE, ZEROS )
*     autrement dit desactive
      MODH128 = 1024  
      GOTO 100

      END
*
      SUBROUTINE COMP( RAST, DOUBLE )
      implicit none
#include "argum.cdk"
#include "pact.cdk"
      INTEGER RAST( WRAST*4 )
      LOGICAL DOUBLE

      INTEGER OO(0:15), NN(6), FF(8), COUNT, PAT1, PAT2, I, J, K, X, W4
      INTEGER MOLD

      DATA OO /1H0, 1H1, 1H2, 1H3, 1H4, 1H5, 1H6, 1H7, 1H8, 1H9,
     +         1HA, 1HB, 1HC, 1HD, 1HE, 1HF /
*     DATA OO /"0","1","2","3","4","5","6","7","8","9",
*    +         "A","B","C","D","E","F"/
      DATA NN /1HO, 1HP, 1HQ, 1HR, 1HS, 1HT / 
      DATA FF /1HG, 1HH, 1HI, 1HJ, 1HK, 1HL, 1HM, 1HN /
*     DATA NN /"O","P","Q","R","S","T"/
*     DATA FF /"G","H","I","J","K","L","M","N"/

      W4 = WRAST*4
      IF( DOUBLE ) W4 = W4 + W4
      I = 2
      J = 1
 10   IF(I .LE. W4) THEN
         COUNT = 0
 20      IF(I.LE.W4 .and. (RAST(I).EQ.RAST(J))) THEN
            COUNT = COUNT + 1
            I     = I + 1
            GO TO 20
         ENDIF
*        RAST(J) = ISHFT(COUNT, 8 ) + RAST(J)
         RAST(J) = ISHFT(IAND(ISHFT(-1,-8), COUNT), 8) + RAST(J)
         J       = J + 1
         RAST(J) = RAST(I)
         I       = I + 1
!         IF(I .EQ. W8+1) J = J + 1
         GO TO 10
         ENDIF

      RAST(J) = -1
      K       = 1
      I       = 1
 30   IF(RAST(I) .NE. -1 ) THEN
         PAT1 = IAND(ISHFT(RAST(I),- 4 ) , 15)
         PAT2 = IAND(RAST(I), 15)
#define Arg1 OO( PAT1 )
#include "putcar.cdk"
         K    = K+1
#define Arg1 OO( PAT2 )
#include "putcar.cdk"
         K     = K+1
         COUNT = ISHFT( RAST(I), -8)
         IF(COUNT .NE. 0) THEN
 40         IF(COUNT .GT. 111) THEN
#define Arg1  TRANSFER('T    ',MOLD)
#include "putcar.cdk"
               K     = K + 1
               COUNT = COUNT - 96
               GO TO 40
            ENDIF
            X = ISHFT(COUNT, -4)
            IF(X .NE. 0) THEN
#define Arg1  NN( X ) 
#include "putcar.cdk"
               K = K + 1
            ENDIF
            COUNT = IAND(COUNT , 15)
            X     = ISHFT(COUNT, -3)
            IF(X .NE. 0) THEN
#define Arg1 TRANSFER('N    ',MOLD)
#include "putcar.cdk"
               K = K + 1
            ENDIF
            X = IAND(COUNT , 7)
            IF(X .NE. 0) THEN
#define Arg1 FF( X ) 
#include "putcar.cdk"
               K = K + 1
            ENDIF
         ENDIF
         I = I + 1
         GO TO 30
      ENDIF

      RETURN
      END

***S/R FLUSHIT
*
      SUBROUTINE FLUSHIT( )
#include "impnone.cdk"
#include "pact.cdk"
      INTEGER K,MOLD

      DO 10 K = 1, 129
#define Arg1 TRANSFER('     ',MOLD)
#include "putcar.cdk"
 10      CONTINUE

      RETURN
      END

***S/R OUT130
      SUBROUTINE OUT130( BUF )
*
#include "impnone.cdk"
      INTEGER BUF(130)


      INTEGER I, STATUS

      COMMON /PLOTZ/ STATUS

      IF(STATUS .EQ. 1) THEN
          STATUS = 2
          WRITE(99, 55)
      ENDIF

      WRITE(99,'(130A1)') (BUF(I),I=1,130)

 55   FORMAT('1YZPLOT$ZW')

      RETURN
      END
#endif
#else
*     SUBROUTINE OUT130( BUF )
*     RETURN
*     END
*     SUBROUTINE tttban( )
*     RETURN
*     END
*     SUBROUTINE FLUSHIT( )
*     RETURN
*     END
*     SUBROUTINE CMPRES2A( Z, NLIGNE, DOUBLE, ZEROS )
*     RETURN
*     END
*     SUBROUTINE CMPRES2(Z,      NLIGNE, DOUBLE, ZEROS)
*     RETURN
*     END
#endif
