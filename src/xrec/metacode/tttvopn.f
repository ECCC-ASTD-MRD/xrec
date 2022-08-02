#if defined (RASTER) 
#if defined (CRAY) 
***S/P VOPN RESERVER ET INITIALISER LA MEMOIRE
*
      SUBROUTINE TTTVOPN(LCP,NCP,HV,FOND)
      implicit none
      IMPNONE
*
*AUTEUR     M. VALIN
*
*REVISION 001     M. LEPINE   -   TRAITEMENT DES COULEURS
*
*OBJET(TTTVOPN)
*     INITIALISER ET RESERVER LA MEMOIRE
*
**
      INTEGER N,IIVAR,LOC,LCP,NCP,HV,FOND
      EXTERNAL MEMOIR,LOC


*
*AUTEUR   M.VALIN RPN 1984
*
*LANGAGE  RATFOR
*
*OBJET(TTTVOPN)
*         RESERVER LA MEMOIRE NECESSAIRE AU TRADUCTEUR DE METACODE
*         DANS LE CAS D'UN TRADUCTEUR AVEC TRAMES (RASTER).
*         INITIALISER LE POINTEUR A L'IMAGE DES TRAMES EN MEMOIRE,
*         (BASE, COMMON /ARGUM/ ).
*         LE TOUT EST SUIVI DE LA MISE A ZERO.
*
*ARGUMENTS
*  IN     N         NOMBRE DE MOTS A RESERVER ET A METTRE A ZERO
*                 (NCP * LCP * HV)
*
*IMPLICITES
#include "argum.cdk"
#include "ivar.cdk"
*
**
      INTEGER DUM(10),IP,I,J,K,CC0,MSK
      POINTER(P,IIVAR(LCP,NCP,HV))


      CALL MEMOIR(DUM,IP,LCP * NCP * HV)
      BASE = LOC(DUM) + IP - 1
      PIVAR = BASE
      GOTO 10


***S/P TTTZOPN  METTRE LA MEMOIRE A ZERO
      ENTRY TTTZOPN(LCP,NCP,HV,FOND)
*
*OBJET(TTTZOPN)
*         PROCEDER A LA MISE A ZERO DE L'IMAGE MEMOIRE DES TRAMES.
*
*ARGUMENTS
*  IN     N         NOMBRE DE MOTS DE L'IMAGE MEMOIRE
*
**


10    P = BASE
      CC0 = FOND
      DO J = 1,NCP
        $(
          IF ((CC0 .AND. 1) <> 0)
            MSK = MASK(64)
          ELSE
            MSK = 0
          DO I = 1,LCP
            DO K = 1,HV
              IIVAR(I,J,K) = MSK
          CC0 = SHIFTR(CC0,1)
        $)
      RETURN
      END
#endif
#if defined (UNIX)  
***S/P TTTVOPN INITIALISER LA MEMOIRE RESERVEE
#include "aaaamac.cdk"
*
      SUBROUTINE TTTVOPN(LCP, NCP, HV, FOND, IVAR2)
#include "impnone.cdk"
      INTEGER            LCP, NCP, HV, FOND, IVAR2(LCP,NCP,HV)
*
*AUTEUR  ?
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTVOPN)
*     DANS LE CAS D'UN TRADUCTEUR AVEC TRAMES (RASTER).
*     INITIALISER LA MEMOIRE RESERVEE AU TRADUCTEUR DE METACODE
*     A ZERO.
*
*ARGUMENTS
*  IN  (NCP * LCP * HV) NOMBRE DE MOTS A RESERVER ET A METTRE A ZERO
*
*IMPLICITES
#include "argum.cdk"
#include "ivar.cdk"
*
***S/P TTTZOPN  METTRE LA MEMOIRE A ZERO
      ENTRY TTTZOPN(LCP, NCP, HV, FOND, IVAR2)
*
*OBJET(TTTZOPN)
*     PROCEDER A LA MISE A ZERO DE L'IMAGE MEMOIRE DES TRAMES.
**

      INTEGER N, I, J, K, CC0, MSK

      CC0 = FOND
      DO 20 J = 1,NCP
         IF(IAND(CC0,1) .NE. 0) THEN
            MSK = MASK(32)
         ELSE
            MSK = 0
         ENDIF
         DO 10 I = 1,LCP
         DO 10 K = 1,HV
            IVAR2(I, J, K) = MSK
 10         CONTINUE
         CC0 = ISHFT(CC0, -1)
 20      CONTINUE

      RETURN
      END
#endif
#endif