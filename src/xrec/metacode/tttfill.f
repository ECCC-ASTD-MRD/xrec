#if defined (RASTER) 
***S/P TTTFILL REMPLISSAGE AVEC UN MOTIF
*
#if defined (CRAY) 
SUBROUTINE TTTFILL(JJ,KK,LL,MM,NA,DUMMY)
#endif
#if defined (UNIX)  
#include "aaaamac.cdk"
      SUBROUTINE TTTFILL(JJ, KK, LL, MM, NA, IVAR)
#endif
#include "impnone.cdk"
#include "params.cdk"
      REAL    JJ, KK, LL(2), MM(2)
      INTEGER NA, IVAR(LCP,NCP,0:*)
*
*AUTEUR   M.VALIN JAN 1984 RPN
*
*REVISION  001 M. LEPINE   - TRAITEMENT DES COULEURS
*          002 M. LEPINE   - VERSION 32 BITS DE DROITE (CDC)
*          001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTFILL)
*         FAIRE LE REMPLISSAGE D'UNE SECTION DE POLYGONE
*         TTTFILL EFFECTUE LA DEUXIEME PARTIE DE TRAVAIL DE
*         REMPLISSAGE, LA PREMIERE ETANT FAITE PAR TTTTONE
*         D'APRES DES ALGORITHMES DE VERSATEC.
*
*IMPLICITES
#include "patrns.cdk"
#include "argum.cdk"
#include "palpat.cdk"
*
*VARIABLES IMPORTANTES
*
*     MODPOL      = 1  MODE NORMAL
*                 = 0  MODE INVERSE
*                 = -1 MODE COMPLEMENT
*
*     HPALET(2,NPAT)   COULEUR DE PREMIER PLAN
*
*     HPALET(3,NPAT)   COULEUR DE SECOND PLAN
*
*                      UN CODE DE COULEUR EGAL A -1 INDIQUE UNE COULEUR
*                      TRANSPARENTE
*
*
*NOTES
**
      INTEGER INTJJ,  INTKK,  IXXX, J, K, M1, M2, IXT, CC0, C0, II,
     +        IBRN1,  IBRN2,  IBAS, I, L, N1, N2, NNA, CC1, C1,
     +        DUMMY,  IIPAT
      REAL    SLOPE1, SLOPE2, YY1,  YY2
#if defined (CRAY) 
*     CECI EST LA VERSION CRAY, 64 BITS PAR MOT

POINTER (P,IVAR(LCP,NCP,0:*))

IF(NPAT.EQ.0 .AND. NA.GT.0) RETURN  # RIEN A FAIRE

IIPAT = HPALET(1,NPAT)
IF(IIPAT.EQ.0 .AND. NA.GT.0) RETURN
INTJJ = JJ
INTKK = KK
J     = MAX(INTJJ, VMIN)
K     = MIN(INTKK, VMAX)
IF(J .GT. K) RETURN   ! POLYGONE HORS FENETRE
SLOPE1 = (MM(1)-LL(1))/MAX(KK-JJ,.0000001)
SLOPE2 = (MM(2)-LL(2))/MAX(KK-JJ,.0000001)
YY1    = LL(1) - SLOPE1*JJ + .99999
YY2    = LL(2) - SLOPE2*JJ
P      = BASE
IF(MODPOL .EQ. 1) THEN
C0 = TABCOL(HPALET(2,NPAT))
C1 = TABCOL(HPALET(3,NPAT))
ELSE
C0 = TABCOL(HPALET(3,NPAT))
C1 = TABCOL(HPALET(2,NPAT))
ENDIF
DO 30 IXT=J,K 
IBRN1 = MAX(UMIN,INT(SLOPE1*IXT + YY1))
IBRN2 = MIN(UMAX,INT(SLOPE2*IXT + YY2))

IF(IBRN1 .LE. IBRN2) THEN    ! QUELQUE CHOSE A TRACER
IF(NTP(IIPAT) .LT. 200) THEN
IXXX = IXT             ! PAS DE DEDOUBLEMENT
ELSE              
IXXX = RSHIFT(IXT, 1)  ! DEDOUBLEMENT
ENDIF
IF(NA .LT. 0) THEN
C0 = TABCOL(FOND)
C1 = TABCOL(FOND)
L  = 0
ELSE
L = ITP((IXXX.AND.31)+1,IIPAT)
ENDIF
M1 = .NOT.MASK(IBRN1.AND.63)
M2 = MASK(1+(63.AND.IBRN2))
N1 = RSHIFT(IBRN1,6)+ 1
N2 = RSHIFT(IBRN2,6)+ 1
*IBAS = IXT*WRAST
DO 20 I=N1,N2
IF(I .EQ. N2) M1 = M1.AND.M2
CC0 = C0
CC1 = C1
DO 10 II = 1,NCPOB
IF(MODPOL .EQ. -1)
IVAR(I,II,IXT) = IVAR(I,II,IXT) .OR.
     +                                (L.AND.M1)
ELSE
IF(C0 .NE. -1) THEN
IF(((CC0.AND.1).NE.0) .AND. L.NE.0) THEN
IVAR(I,II,IXT) = IVAR(I,II,IXT) .OR.
     +                                      (L.AND.M1)
ELSE
IVAR(I,II,IXT) = IVAR(I,II,IXT) .AND.
     +                                      .NOT.(L.AND.M1)
ENDIF
ENDIF 
IF(C1 .NE. -1) THEN
IF(((CC1.AND.1).NE.0) .AND.
     +                     L.NE.MASK(64)) THEN
IVAR(I,II,IXT) = IVAR(I,II,IXT) .OR.
     +                     ((.NOT.L).AND.M1)
ELSE
IVAR(I,II,IXT) = IVAR(I,II,IXT) .AND.
     +                     .NOT.(((.NOT.L).AND.M1))
ENDIF
ENDIF
ENDIF
CC0 = RSHIFT(CC0,1)
CC1 = RSHIFT(CC1,1)
  !               P   = P + LCP
 10               CONTINUE
M1 = MASK(64)
 20            CONTINUE
            ENDIF
 30         CONTINUE
#endif
#if defined (UNIX) 
*     CECI EST LA VERSION SUN ,32 BITS PAR MOT

      IF(NPAT.EQ.0 .AND. NA.GT.0) RETURN    ! RIEN A FAIRE
      IIPAT = HPALET(1,NPAT)
      IF(IIPAT.EQ.0 .AND. NA.GT.0)  RETURN
      INTJJ = JJ
      INTKK = KK
      J     = MAX(INTJJ, VMIN)
      K     = MIN(INTKK, VMAX)
      IF(J .GT. K) RETURN   ! POLYGONE HORS FENETRE
      SLOPE1 = (MM(1)-LL(1))/MAX(KK-JJ,.0000001)
      SLOPE2 = (MM(2)-LL(2))/MAX(KK-JJ,.0000001)
      YY1    = LL(1) - SLOPE1*JJ + .99999
      YY2    = LL(2) - SLOPE2*JJ
      IF(MODPOL .EQ. 1) THEN
          C0 = TABCOL(HPALET(2, NPAT))
          C1 = TABCOL(HPALET(3, NPAT))
      ELSE
          C0 = TABCOL(HPALET(3, NPAT))
          C1 = TABCOL(HPALET(2, NPAT))
      ENDIF
      DO 30 IXT=J,K 
         IBRN1 = MAX(UMIN,INT(SLOPE1*IXT + YY1))
         IBRN2 = MIN(UMAX,INT(SLOPE2*IXT + YY2))

         IF(IBRN1 .LE. IBRN2) THEN     ! QUELQUE CHOSE A TRACER
            IF(NTP(IIPAT) .LT. 200) THEN
               IXXX = IXT              ! PAS DE DEDOUBLEMENT
            ELSE 
               IXXX = ISHFT(IXT, -1)   ! DEDOUBLEMENT
            ENDIF

            IF(NA .LT. 0) THEN
               C0 = TABCOL( FOND )
               C1 = TABCOL( FOND )
               L  = 0
            ELSE
              L = ITP(IAND(IXXX,31)+1, IIPAT)
            ENDIF

*           MASQUE A GAUCHE DE %1 BITS
            M1 = NOT(MASK(IAND(IBRN1,31)))
            M2 = MASK(1+IAND(31,IBRN2))
            N1 = ISHFT(IBRN1, -5)+ 1
            N2 = ISHFT(IBRN2, -5)+ 1

            DO 20 I=N1,N2 
               IF(I .EQ. N2) M1 = IAND(M1,M2)
               CC0 = C0
               CC1 = C1
               DO 10 II = 1,NCP
                  IF(MODPOL .EQ. -1) THEN
                     IVAR(I,II,IXT) = IEOR(IVAR(I,II,IXT), IAND(L,M1))
                  ELSE
                     IF(C0 .NE. -1) THEN
                        IF((IAND(CC0 , 1).NE.0) .AND. L.NE.0) THEN
                           IVAR(I,II,IXT) = IOR(IVAR(I,II,IXT),
     +                                          IAND(L,M1))
                        ELSE
                           IVAR(I,II,IXT) = IAND(IVAR(I,II,IXT) , 
     +                                           NOT(IAND(L,M1)))
                        ENDIF
                     ENDIF
                     IF(C1 .NE. -1) THEN
                        IF((IAND(CC1 , 1).NE.0) .AND. 
     +                     (L.NE.MASK(32))) THEN
                           IVAR(I,II,IXT) = IOR(IVAR(I,II,IXT) ,
     +                     IAND(NOT(L),M1))
                        ELSE
                           IVAR(I,II,IXT) = IAND(IVAR(I,II,IXT) , 
     +                     NOT(IAND(NOT(L),M1)))
                        ENDIF
                     ENDIF
                  ENDIF
                  CC0 = ISHFT(CC0, -1)
                  CC1 = ISHFT(CC1, -1)
 10               CONTINUE
               M1 = MASK(32)
 20            CONTINUE
         ENDIF
 30      CONTINUE
#endif
      RETURN
      END
#endif
