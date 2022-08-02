***S/R TTTPWI1 - CALCULE LES FACTEURS D'ORIENTATION POUR TTTPWRY
#include "aaaamac.cdk"
*
      SUBROUTINE TTTPWI1
#include "impnone.cdk"
*
*AUTEUR     - M. VALIN    -  AVR 80
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTPWI1)
*           - CALCULE LES FACTEURS D'ORIENTATION QUI SERVIRONT A TRACER
*             LES CARACTERES DE TTTPWRY.
*
*APPEL      - CALL TTTPWI1   # INITIALISATION COMPLETE
*             CALL TTTPWI2   # CHANGEMENT DE TAILLE DES CARACTERES
*             CALL TTTPWI3   # CHANGEMENT D'ESPACEMENT ENTRE LES CARACTERES
*
*IMPLICITES
#include "pwryp.cdk"
#include "params.cdk"
#include "mapvar.cdk"
**
      INTEGER ICC
      REAL    S1, D1, AA, SAA, CAA, RLCW, RLCH, RHOH, CORW, XXX, YYY, T,
     +        S2, D2, PI, SOA, COA, RLHS, RLVS, RHOW, CORH, ZZZ, RHO,
     +        ZZ, WW, OA, SQR, SA,  RHOE, CORE, FUDGE
      SAVE    AA, OA, S1, SAA, S2,  CAA,  SOA,  COA,  T, D1, D2
      DATA    PI /3.1415926535/


      FUDGE(ZZ,ICC) = REAL(NINT(0.2 * ABS(ZZ) * ICC)) / (0.2 * ABS(ZZ))
      SQR(WW)       = WW * WW
      RHO(XXX,YYY)  = 4 * SQRT(SQR(A1*XXX+B1*YYY) + SQR(A2*XXX+B2*YYY))

      AA = IANOT*PI/180.     ! ANGLE D'ORIENTATION
      OA = IORN*PI/180.      ! ANGLE D'ANNOTATION
      SA = ISLANT*PI/180.    ! ANGLE SLANT


      SAA = SIN(AA)
      CAA = COS(AA)
      SOA = SIN(OA)
      COA = COS(OA)
      T   = AA+OA
      S1  = SIN(T)
      D1  = COS(T)
      T   = T+SA
      S2  = SIN(T)
      D2  = COS(T)

      ENTRY TTTPWI2

      LCW  = MAX(1,LCW)
      LCH  = MAX(1,LCH)
      X1   = LCW*D1*.05     ! LCW = CHARACTR WIDTH
      X2   =-LCH*S2*.05     ! LCH = CHARACTR HEIGHT
      Y1   = LCW*S1*.05
      Y2   = LCH*D2*.05
      RHOH = RHO(X2,Y2)
      RHOW = RHO(X1,Y1)
      CORH = MAX(1,NINT(RHOH)) / RHOH
      CORW = MAX(1,NINT(RHOW)) / RHOW
      X1   = X1 * CORW
      Y1   = Y1 * CORW
      X2   = X2 * CORH
      Y2   = Y2 * CORH

      ENTRY TTTPWI3

      LHS  = MAX(1,LHS)
      LVS  = MAX(1,LVS)
      T    = ABS(COA*LHS+SOA*LVS)   ! LHS=HORIZONTAL SPACING
      SPX  = T*CAA                  ! LVS=VERTICAL SPACING
      SPY  = T*SAA
      RHOE = RHO(SPX,SPY) * .2
      CORE = MAX(1,NINT(RHOE)) / RHOE
      SPX  = SPX * CORE
      SPY  = SPY * CORE

      RETURN
      END

