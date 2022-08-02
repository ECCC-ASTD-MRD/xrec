***S/P TTTSKFR  SAUTER UNE OU PLUSIEURS IMAGES
#include "aaaamac.cdk"
*
      SUBROUTINE TTTSKFR(N, STATUS)
#include "impnone.cdk"

      INTEGER            N, STATUS
*
*AUTEUR       M. VALIN RPN 1984
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTSKFR)
*         SAUTER UNE OU PLUSIEURS IMAGES DANS LE FICHIER
*         DE METACODE. IL Y A DES LIENS VIA LE COMMON
*         BPAIRP AVER LA FONCTION TTTBPR.
*
*ARGUMENTS
*  IN     N         NOMBRE D'IMAGES A SAUTER
*  OUT    STATUS    ETAT APRES LE SAUT (OK OU MARKEOI)
*
*IMPLICITES
#include "params.cdk"
#include "bpairp.cdk"
#include "segvar.cdk"
#include "endian.cdk"
*
*NOTES
*     SI LE FICHIER SE TERMINE PREMATUREMENT (MARKEOI OU ENREGISTREMENT
*     N'ETANT PAS DU METACODE, LE CODE MARKEOI EST RENVOYE.
*
*MODULE 
      EXTERNAL WAREAD
*
**
      INTEGER I, TEMP

      STATUS = OK

      IF (LITTLE_END(1) .EQ. 1) THEN
         TEMP=1
      ELSE
         TEMP=2
      ENDIF
      DO 20 I=1,N 
 10      CALL WAREAD(MCUNIT, TWOBYT(1,PILE), ADRESS(PILE), 1)
         ADRESS(PILE) = ADRESS(PILE) + MBUFSZ
#if defined (CRAY) 
         IF(IAND(OCT(177777),RSHIFT(TWOBYT(1,PILE),BITMOT-32))
     +      .EQ. OCT(52525)) GP TP 10
         IF(RSHIFT(TWOBYT(1,PILE),BITMOT-32).NE.MARKEOF) THEN
#else
         IF(.NOT.TWOBYT(TEMP,PILE) .NE. 21845) GO TO 10
         IF(TWOBYT(TEMP,PILE) .NE. MARKEOF) THEN
#endif
            STATUS = MARKEOI
            GO TO 30
         ENDIF
 20      CONTINUE
 30      CONTINUE

#if defined (UNIX) 
*     REPOSITIONNEMENT DU FICHIER AU DEBUT DU BLOC
      IF(N .GT. 0) CALL WAREAD(MCUNIT, TWOBYT(1,PILE),
     +                         ADRESS(PILE)-MBUFSZ-1,1)
      IF (LITTLE_END(1) .EQ. 1) THEN
         TEMP = TWOBYT(1,PILE)
         TWOBYT(1,PILE) = TWOBYT(2,PILE)
         TWOBYT(2,PILE) = TEMP
      ENDIF
#endif

      RESTE(PILE) = 0

      RETURN
      END