#if defined (RASTER)
*** S/P TTTHAFT - REMPLIR UNE SERIE DE POLYGONES AVEC UN OU PLUSIEURS PATRONS
#include "aaaamac.cdk"
*
      SUBROUTINE TTTHAFT
#include "impnone.cdk"
*
*AUTEUR       M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTHAFT)
*     SERT A REMPLIR AVEC UN OU DIFFERENTS MOTIFS UNE SERIE DE
*     POLYGONES ET CE SUIVANT UNE DIRECTION  X  OU  Y.
*
*MODULES
#if !defined (NATIF) 
      EXTERNAL TTTFILL
**
#endif
#if defined (NATIF) || defined (C910)
      EXTERNAL TTTTONE
**
      INTEGER  UV(2,4)
#endif
      INTEGER X0, Y0, X1, Y1, NX, NBPOL, NY, LONG, LNG, TOTPOL, SAVPAT,
     +        U0, V0, U1, V1, NP, COUNT
      REAL    DX01, DY01, DDX, DDY, LL(2), JJ, KK
*
#include "params.cdk"
#include "patrns.cdk"
#include "mapvar.cdk"
#include "ivar.cdk"
#include "mapping.cdk"
*
*     NOTE: 255=OCT(377) 32767=OCT(77777)
      SAVPAT = NPAT
      LONG   = IAND(BPAIRS(1), 255)
      X0     = IAND(BPAIRS(2), 32767)
      Y0     = IAND(BPAIRS(3), 32767)
      X1     = IAND(BPAIRS(4), 32767)
      Y1     = IAND(BPAIRS(5), 32767)
      NX     = IAND(ISHFT(BPAIRS(6),-8), 255)
      NY     = IAND(BPAIRS(6) , 255)
      DX01   = (X1 - X0) / REAL(NX)
      DY01   = (Y1 - Y0) / REAL(NY)
      IF(NX .EQ. 1) THEN
         DDX  = DX01
         DDY  = Y0
         DX01 = 0.
      ELSE
         IF(NY .EQ. 1) THEN
            DDY  = DY01
            DDX  = X0
            DY01 = 0.
         ELSE
            WRITE(IPUNIT,*) '*** MAUVAISE DIRECTION - TTTHAFT'
            RETURN
         ENDIF
      ENDIF
      NBPOL  = MAX(NX,NY)
      LNG    = LONG - 10
      TOTPOL = 0
      NP     = 1

 10   IF(TOTPOL .LT. NBPOL) THEN
         IF(NP .GT. LNG) THEN
            WRITE(IPUNIT,*) '*** MAUVAISE LONGUEUR - TTTHAFT'
            RETURN
         ENDIF
         NPAT = IAND(ISHFT(BPAIRS(7+(ISHFT(NP-1, -1))),
     +               -8*(IAND(1,NP))), 255)
         NP = NP + 1
         IF(NPAT .GE. 128) THEN
            COUNT = NPAT - 127
            NPAT  = IAND(ISHFT(BPAIRS(7+(ISHFT(NP-1,-1))),
     +                   -8*(IAND(1,NP))), 255)
            NP = NP + 1
         ELSE
            COUNT = 1
         ENDIF
         IF(NX .EQ. 1) THEN
            Y1 = DDY + (TOTPOL+COUNT) * DY01
         ELSE
            X1 = DDX + (TOTPOL+COUNT) * DX01
         ENDIF
#if !defined (NATIF) 
         U0      = MAPU(X0,Y0)
         V0      = MAPV(X0,Y0)
         U1      = MAPU(X1,Y1)
         V1      = MAPV(X1,Y1)
         LL(1)   = MIN(U0,U1)
         LL(2)   = MAX(U0,U1)
         JJ      = MIN(V0,V1)
         KK      = MAX(V0,V1)
         IF(NPAT .NE. 0) CALL TTTFILL(JJ, KK, LL, LL, 1, IVAR(PTIV))
#endif
#if defined (NATIF) || defined (C910)
         UV(1,1) = MAPU(X0,Y0)
         UV(2,1) = MAPV(X0,Y0)
         UV(1,2) = MAPU(X1,Y0)
         UV(2,2) = MAPV(X1,Y0)
         UV(1,3) = MAPU(X1,Y1)
         UV(2,3) = MAPV(X1,Y1)
         UV(1,4) = MAPU(X0,Y1)
         UV(2,4) = MAPV(X0,Y1)
         CALL TTTTONE(UV, 4, 1)
#endif
         IF(NX .EQ. 1) THEN
            Y0 = Y1
         ELSE
            X0 = X1
         ENDIF
         TOTPOL = TOTPOL + COUNT
         GO TO 10
      ENDIF

      NPAT = SAVPAT

      RETURN
      END
#endif
