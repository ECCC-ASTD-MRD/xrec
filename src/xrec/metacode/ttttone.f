#if defined (RASTER) 
#include "aaaamac.cdk"
#if !defined (NATIF) 
***S/R TTTTONE - POLYGONAL TONER PREMIERE PASSE
      SUBROUTINE TTTTONE(UV,       NE,    NA)
#include "impnone.cdk"
      INTEGER            UV(2,64), NE(1), NA
*
*AUTEUR     - M. VALIN    - SEP 79 BORROWED FROM VERSATEC ALGORITHMS
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTTONE)
*         - POLYGONAL TONER PREMIERE PASSE
*         TTTTONE PROCEDE A LA SUBDIVISION DU POLYGONE EN FIGURES
*         ELEMENTAIRES QUI SERONT ENSUITE "REMPLIES" PAR TTTFILL.
*
*ARGUMENTS
*         -UV  - TABLE (2,N) CONTENANT LES COORDONNEES
*                DES SOMMETS DU POLYGONE A REMPLIR
*                UV(1,X) = COORDONNEE X
*                UV(2,X) = COORDONNEE Y
*         - NE - NOMBRE DE POINTS DE CHAQUE POLYGONE
*         - NA - NOMBRE DE POLYGONES
*
*MODULES 
      EXTERNAL TTTFILL
*
**
#include "ivar.cdk"
      INTEGER MA, NET, I, J, K, L, N
      REAL(KIND=8)  YL(20), XRI, X1, X2, Y1, Y2, DLT, SLOPE, XMAX, U, V,
     +        YR(20), XLF
      REAL    SYL(20),SYR(20), SXRI, SXLF

*     
*     "STATEMENT FUNCTIONS" PERMETTANT DE RECUPERER LES COORDONNEES
*     U EQUIVAUT A Y, V EQUIVAUT A X
      U(I) = FLOAT(UV(1,I))
      V(I) = FLOAT(UV(2,I))
      MA   = IABS(NA)

*     DETERMINER LE NOMBRE TOTAL DE VARIABLES DANS V ET U.
      NET = 0
      DO 10 I=1,MA
         NET = NET+NE(I)
 10      CONTINUE

      XMAX = V(1)       ! TROUVER XMIN ET XMAX
      XLF  = V(1)
      DO 20 I=2,NET 
        XMAX = AMAX1(XMAX,V(I))
        XLF  = AMIN1(XLF ,V(I))
 20     CONTINUE

      XMAX = XMAX - 0.0001

*     TROUVER LE PLUS PETIT V > XLF
 30   XRI = XMAX
      DO 40 I=1,NET
         X1 = V(I)
         IF(X1.GT.XLF .AND. X1.LT.XRI) XRI = X1
 40   CONTINUE

*     TROUVER LES LIGNES QUI COUPENT LA SURFACE DE RECHERCHE
 50   N = 0
      L = 0
      DO 80 I=1,MA
         K  = L+1
         L  = K+NE(I)-1
         X1 = V(L)
         Y1 = U(L)
         DO 70 J=K,L
            X2  = V(J)
            Y2  = U(J)
            DLT = XLF-X1
            IF((DLT.GE.0..AND.XLF.GE.X2).OR.(DLT.LT.0..AND.XRI.LE.X2))
     +         GO TO 60
            SLOPE = (Y2-Y1)/(X2-X1)
            N     = N+1
            YL(N) = SLOPE*DLT+Y1
            YR(N) = SLOPE*(XRI-X1)+Y1
 60         X1    = X2
            Y1    = Y2
 70         CONTINUE
 80      CONTINUE
      IF(N. LE. 0) GOTO 150     ! TROUVE QUELQUE CHOSE?

*     Y A-T-IL DES SEGMENTS QUI SE COUPENT DANS NOTRE FENETRE
      DO 100 I=1,N
         DO 90 J=I,N
            Y1 = YL(I)-YL(J)
            Y2 = YR(I)-YR(J)
            IF(Y1*Y2 .GE. -0.5) GOTO 90
*           DES CROISEMENTS, ON DOIT SUBDIVISER NOTRE FENETRE
            XRI = XLF+(XRI-XLF)*(Y1/(Y1-Y2))
            GOTO 50
 90         CONTINUE
 100     CONTINUE

*     METTRE NOS SEGMENTS EN ORDRE CROISSANT PAR RAPPORT A U(Y)
      L = N-1
      DO 120 I=1,L
         K=I+1
         DO 110 J=K,N
            Y1 = YL(J)
            Y2 = YR(J)
            IF(YL(I).GT.Y1 .OR. YR(I).GT.Y2) THEN
               YL(J) = YL(I)
               YR(J) = YR(I)
               YL(I) = Y1
               YR(I) = Y2
            ENDIF
 110        CONTINUE
 120     CONTINUE

      SXLF = XLF
      SXRI = XRI
      DO 130 I=1,N
         SYL(I) = YL(I)
         SYR(I) = YR(I)
 130     CONTINUE

      DO 140 I=1,N,2  ! ON TRACE LES FIGURES ELEMENTAIRES TROUVEES
         CALL TTTFILL(SXLF, SXRI, SYL(I), SYR(I), NA, IVAR(PTIV))
 140     CONTINUE

 150  XLF = XRI       !  LA LIMITE DE DROITE DEVIENT CELLE DE GAUCHE
      IF(.NOT.(XLF .GE. XMAX)) GO TO 30   ! TRAVAIL TERMINE ?

      RETURN
      END
#endif
#endif
#if defined (NATIF) 
#if defined (C910)
***S/R TTTTONE - POLYGONAL TONER PREMIERE PASSE
*
      SUBROUTINE TTTTONE(UV,       NE,    NA)
#include "impnone.cdk"
      INTEGER            UV(2,64), NE(1), NA
*
*AUTEUR     - M. VALIN    - SEP 79 BORROWED FROM VERSATEC ALGORITHMS
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTTONE)
*         - POLYGONAL TONER PREMIERE PASSE
*         TTTTONE PROCEDE A LA SUBDIVISION DU POLYGONE EN FIGURES
*         ELEMENTAIRES QUI SERONT ENSUITE "REMPLIES" PAR TTTFILL.
*
*ARGUMENTS
*         -UV  - TABLE (2,N) CONTENANT LES COORDONNEES
*                DES SOMMETS DU POLYGONE A REMPLIR
*                UV(1,X) = COORDONNEE X
*                UV(2,X) = COORDONNEE Y
*         - NE - NOMBRE DE POINTS DE CHAQUE POLYGONE
*         - NA - NOMBRE DE POLYGONES
*
*MODULES
      EXTERNAL COLOR, POLF2I, SETPAT
**
#include "ivar.cdk"
#include "palpat.cdk"
#include "patrns.cdk"
#include "params.cdk"
#include "com910.cdk"

      INTEGER  IIPAT, C0, C1

*
*     "STATEMENT FUNCTIONS" PERMETTANT DE RECUPERER LES COORDONNEES
*     U EQUIVAUT A Y, V EQUIVAUT A X
*

      IF(NPAT.EQ.0 .AND. NA.GT.0) RETURN

      IIPAT = HPALET(1,NPAT)
      IF(IIPAT .EQ. 0) RETURN

      IF(NA .LT. 0) THEN
         C0    = TABCOL(FOND)
         C1    = -1
         IIPAT = 1
      ELSE
         IF(MODPOL .EQ. 1) THEN
            C0 = TABCOL(HPALET(2,NPAT))
            C1 = TABCOL(HPALET(3,NPAT))
         ELSE
            C1 = TABCOL(HPALET(2,NPAT))
            C0 = TABCOL(HPALET(3,NPAT))
         ENDIF
      ENDIF

      IF(C0 .NE. -1) THEN
         CALL COLOR( C0 )
         LASTCOL = C0
         CALL SETPAT( IIPAT )
         CALL POLF2I( NE, UV)
      ENDIF

      IF(C1 .NE. -1) THEN
         CALL COLOR( C1 )
         LASTCOL = C1
         CALL SETPAT( -IIPAT )
         CALL POLF2I( NE, UV )
      ENDIF

      RETURN
      END
#endif
#endif
