#if defined (RASTER) 
#if defined (CRAY) 
*** S/P ACCOUNT - FAIT LA COMPTABILITE DE TRADUCT
*
      SUBROUTINE ACCOUNT(UN)
#include "impnone.cdk"
*
*AUTEURS       M. LEPINE  -  1985
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(ACCOUNT)
*     SERT A COMPTABILISER L'UTILISATION DU PAQUET GRAPHIQUE EN TERME
*   DE CPU, NOMBRE DE FRAMES, NOMBRE DE RASTERS ... ET CE PAR GROUPE
*
**
      CHARACTER(LEN=8) UN
#include "params.cdk"
#include "argum.cdk"
*
*  VARIABLE LOCALE TYPDEV (TYPE DE DEVICE POUR LEQUEL ON COMPTABILISE)
*
*                           TAPE
*                           DIFAX         TEKJET
*                           TANDEM        CGP220
*          V80  V80T  V100  DFI   LASER   CALCOMP ECRAN  RID  SUN
*                     V200  LINK
*  TYPDEV=  1     2     3    4       5       6       7    8    9
*
      INTEGER  BITCNT(9), WORDNB(9), Z(10,100), IER, I, J, N, TYPDEV,
     +         NWORDF(9), EXTRA(9),  TEMP, 
      EXTERNAL ACCESS, FRETOUR

      DATA BITCNT /0,  21, 42,  0, 21, 42, 0, 21,42/
      DATA NWORDF /6,   6,  6,  7,  7,  7, 8,  8, 8/
      DATA WORDNB /9,   9,  9, 10,  0,  0, 0,  0, 0/
      DATA EXTRA  /25, 25, 32,  0,  0,  0, 0,  0, 0/
*
*
      CALL ACCESS(IER, 'DN'L, 'XXXX001'L, 'PDN'L, 'ACTFILE'L, 'ID'L,
     +            'ARMN000'L, 'UQ'L, 'W'L, 'RMNP'L, 'OWN'L, 'ARMN000'L)
      GOTO(10, 10, 10, 20, 20, 10, 30, 60, 30, 30, 30, 50, 60, 30, 20, 70,
     +     70, 70, 20, 20, 60, 30, 40) DEVICE


 10   IF(UN .EQ. 'ARMNV80') THEN
         TYPDEV = 1
      ELSEIF (UN .EQ 'ASPOAAY') THEN
         TYPDEV = 2
      ELSE
         IF(UN .EQ 'CENTRAL') THEN
            TYPDEV = 3
         ELSE
           TYPDEV = 7
         ENDIF
      ENDIF
      GOTO 80

 20   TYPDEV = 4
      GOTO 80

 30   TYPDEV = 7
      GOTO 80

 40   TYPDEV = 5
      GOTO 80

 50   TYPDEV = 8
      GOTO 80

 60   TYPDEV = 6
      GOTO 80

 70   TYPDEV = 9

 80   CONTINUE
      N = 1
      IF(IER .EQ 0) THEN
         READ(LJUST(XXXX001)) Z
 90      IF((Z(1,N).NE.COMPTAB(1)) .AND. (N.LE.100) .AND.
     +      (Z(1,N).NE.0)) THEN
            N = N + 1
            GOTO 90
         ENDIF
      ELSE
         DO 100 J = 1,100
         DO 100 I = 1,10
            Z(I,J) = 0
 100        CONTINUE
      ENDIF
*
*  NUMERO D'USAGER
*
      Z(1,N) = COMPTAB(1)
*
*  COMPTABILISER LE NOMBRE DE FRAMES
*
      Z(NWORDF(TYPDEV),N) = Z(NWORDF(TYPDEV),N) + 
     +  ISHFT(COMPTAB(2), BITCNT(TYPDEV))
*
*  COMPTABILISER LE NOMBRE DE PAGES / POUCES DE PAPIER
*
      IF(TYPDEV .LT. 5) THEN
         TEMP = COMPTAB(3) + EXTRA(TYPDEV)
         IF((TYPDEV .EQ. 1) .OR. (TYPDEV .EQ. 2))
     +   TEMP = INT(TEMP / 8.6 + .499999)
         TEMP = ISHFT(TEMP, BITCNT(TYPDEV))
         Z(WORDNB(TYPDEV),N) = Z(WORDNB(TYPDEV),N) + TEMP
      ENDIF
*
*  COMPTABILISER LE TEMPS CPU ET LE NOBMRE D'APPEL
*
      DO 110I = 4,5
         Z(I,N) = Z(I,N) + COMPTAB(I)
 110     CONTINUE
      REWIND(LJUST(XXXX001))
      WRITE(LJUST(XXXX001)) Z
      CALL FRETOUR('XXXX001')

      RETURN
      END
#endif
#if defined (UNIX) 
*** S/P ACCOUNT - FAIT LA COMPTABILITE DE TRADUCT
#include "aaaamac.cdk"
*
      SUBROUTINE ACCOUNT( UN )
#include "impnone.cdk"
      CHARACTER(LEN=8)       UN
*
*AUTEUR       M. LEPINE   -  1985
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(ACCOUNT)
*           - COMPTABILISE L'UTILISATION DU PAQUET GRAPHIQUE EN TERME
*             DE CPU, NOMBRE DE FRAMES, NOMBRE DE RASTERS ...
*             ET CE PAR GROUPE
*
**
      RETURN
      END
#endif
#endif
