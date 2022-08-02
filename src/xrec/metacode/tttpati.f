#if defined (RASTER) 
***S/P TTTPATI LIRE LES TABLES  DE MOTIFS DU DISQUE
#include "aaaamac.cdk"
*
      SUBROUTINE TTTPATI
#include "impnone.cdk"
*
*AUTEUR     M. VALIN
*
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTPATI)
*     INITIALISER LES TABLES DE DEFINITION DES DIFFERENTS PATRONS
*
**

      INTEGER JUNK(32), IER, I, J, IPAT
#include "params.cdk"
#include "patrns.cdk"
#if defined (C910) && defined (NATIF) 
      EXTERNAL DEFPAT
      INTEGER  IPATT(32), IPATN(32)
#endif
#if defined (C910)
      EXTERNAL GETENVC
      INTEGER  L
      CHARACTER * 128 DATAREP
#endif
#if defined (CRAY) 
      EXTERNAL ACCESS,RELEASE
      DATA NTP /MAXPATS * 0/

      CALL ACCESS(IER, 'DN'L, 'AAATMPP'L, 'PDN'L, 'PATRNS'L, 'ID'L,
     +            'ARMN000'L, 'OWN'L,'ARMN000'L)
      DO I=1,MAXPATS  $(
        IF (MODEP < 200) # NON DEDOUBLE
          READ('AAATMPP'L,END=777) IPAT,NTP(IPAT),(ITP(J,IPAT),J=1,32)
        ELSE            # DEDOUBLE
          READ('AAATMPP'L,END=777) IPAT,NTP(IPAT),JUNK,(ITP(J,I),J=1,32)
        NTP(IPAT) = NTP(IPAT) * MODEP
        $)

  777 CALL RELEASE(IER,'DN'L,'AAATMPP'L)
#endif
#if defined (SUN) 
      DATA NTP /MAXPATS * 0/

      OPEN(66,FILE='/usr/local/lib/rmndata/patrns',FORM='UNFORMATTED')
      DO 10 I =1,MAXPATS
         IF(MODEP .LT. 200) THEN
            READ(66, END=20) IPAT, NTP(IPAT), (ITP(J,I),J=1,32)
          ELSE
            READ(66, END=20) IPAT, NTP(IPAT), JUNK, (ITP(J,I),J=1,32)
          ENDIF
          NTP(IPAT) = NTP(IPAT) * MODEP
 10       CONTINUE
 20    CLOSE(66)
#endif
#if defined (C910) 
      DATA NTP /MAXPATS * 0/

      CALL GETENVC('ARMNLIB',DATAREP)
      L = len_trim(DATAREP)
      OPEN(66,FILE=DATAREP(1:L)//'/data/patrns',FORM='UNFORMATTED')
      DO 40 I =1,MAXPATS
         IF(MODEP .LT. 200) THEN
            READ(66, END=50) IPAT, NTP(IPAT), (ITP(J,I),J=1,32)
         ELSE
            READ(66, END=50) IPAT, NTP(IPAT), JUNK, (ITP(J,I),J=1,32)
         ENDIF
         NTP(IPAT) = NTP(IPAT) * MODEP
#if defined (NATIF) 
         DO 30 J = 1,32
            IPATT(J) = ITP(33-J,I)
            IPATN(J) = NOT(IPATT(J))
 30         CONTINUE
         CALL DEFPAT(I,  32, IPATT)
         CALL DEFPAT(-I, 32, IPATN)
#endif
 40      CONTINUE
 50   CLOSE(66)
#endif
      RETURN
      END
#endif
