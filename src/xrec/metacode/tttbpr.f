*** S/P TTTBPR - LIRE LE METACODE
#include "aaaamac.cdk"
******
      FUNCTION TTTBPR( MCUNIT, BPAIRS, NPAIRS)
#include "impnone.cdk"
      INTEGER  TTTBPR, MCUNIT, NPAIRS, BPAIRS(NPAIRS)
*
*AUTEUR     M. VALIN / M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTBPR)
*     SERT A LIRE UNE PARTIE DU METACODE TOUT EN DETECTANT LES FINS
* DE SEGMENT ET LES FINS DE FRAME
*
*ARGUMENTS
*   IN   MCUNIT    NUMERO D'UNITE ASSOCIE AU FICHIER DE METACODE
*   OUT  BPAIRS    SERT A EMMAGASINER L'INSTRUCTION LUE
*   IN   NPAIRS    NOMBRE DE PAIRE DE BYTES DESIRE
**
#include "bpairp.cdk"
#include "segvar.cdk"
#include "mapvar.cdk"
*
*MODULES
#if defined (CRAY)
      EXTERNAL IIPAK
#endif
      EXTERNAL WAREAD, WACLOS, DFLERMS, POPPAR
*
**
      INTEGER  NPB, SERVI, DEMANDE, I, BUFFER( MBUFSZ )

      DATA     RESTE  /MAXPILE * 0/
      DATA     POSIT  /MAXPILE * 1/
      DATA     ADRESS /MAXPILE * 1/

      TTTBPR  = OK
      DEMANDE = NPAIRS
      SERVI   = 0

 10   NPB = MIN(RESTE(PILE),DEMANDE)
      DO 20 I = 1,NPB
*         BPAIRS(SERVI+I) = IAND(TWOBYT(POSIT(PILE)+I-1,PILE), D65535)
         BPAIRS(SERVI+I) = TWOBYT(POSIT(PILE)+I-1,PILE)
* on fait le mask de 16 bits
         BPAIRS(SERVI+I) = IAND(bpairs(servi+i), 65535)
 20      CONTINUE
      SERVI       = SERVI + NPB
      POSIT(PILE) = POSIT(PILE) + NPB
      DEMANDE     = DEMANDE - NPB
      RESTE(PILE) = RESTE(PILE) - NPB
      IF(DEMANDE .GT. 0) THEN
#if defined (SUN) 
         IF(SAVUNIT(PILE) .EQ. MCUNIT) THEN
            CALL WAREAD(SAVUNIT(PILE), BUFFER, 0,  MBUFSZ )
         ELSE
            CALL WAREAD(SAVUNIT(PILE), BUFFER, ADRESS(PILE), MBUFSZ )
         ENDIF
#else
         CALL WAREAD(SAVUNIT(PILE), BUFFER, ADRESS(PILE), MBUFSZ )
#endif
         ADRESS(PILE) = ADRESS(PILE) + MBUFSZ
#if defined (CRAY) 
         CALL IIPAK(TWOBYT(1,PILE), BUFFER, 180, 1, -16, 0, 2)
#else
         CALL MOVE3216(BUFFER,TWOBYT(1,PILE), MBUFSZ )
#endif
         POSIT(PILE) = 3
         RESTE(PILE) = TWOBYT(1,PILE)
         IF(TWOBYT(2,PILE) .EQ. MARKEOI) THEN
            IF(SAVUNIT(PILE) .NE. MCUNIT)
     +      WRITE( 6,*)' TTTBPR  MARKEOI TROUVE DANS SEGFILE'
            TTTBPR = MARKEOI
            RETURN
         ELSE
            IF(TWOBYT(2,PILE) .EQ. MARKEOF) THEN
               IF(SAVUNIT(PILE) .NE. MCUNIT)
     +            WRITE( 6,*)' TTTBPR  MARKEOF TROUVE DANS SEGFILE'
               TTTBPR = MARKEOF
               RETURN
            ELSE
               IF(TWOBYT(2,PILE) .EQ. MARKEOS) THEN
                  IF(SAVUNIT(PILE) .EQ. MCUNIT)
     +               WRITE( 6,*)' TTTBPR  MARKEOS TROUVE DANS METACOD'
                  PILE = PILE - 1
                  A1   = MAPUSAV(1,PILE)
                  B1   = MAPUSAV(2,PILE)
                  C1   = MAPUSAV(3,PILE)
                  A2   = MAPVSAV(1,PILE)
                  B2   = MAPVSAV(2,PILE)
                  C2   = MAPVSAV(3,PILE)
                  CALL POPPAR
               ELSE
                  IF(TWOBYT(2,PILE) .NE. MARKFRM) THEN
                     WRITE( 6,*)' TTTBPR  FICHIER METACODE INVALIDE'
                     STOP
                  ENDIF
               ENDIF
            ENDIF
         ENDIF
      ENDIF

      IF(.NOT.(DEMANDE .LE. 0))  GO TO 10

      RETURN
      END
