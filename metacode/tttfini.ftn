#if defined (RASTER) 
#if defined (CRAY) 
*** S/P TTTFINI - TERMINER ET FERMER LE FICHIER COMPRIME
*
SUBROUTINE TTTFINI(FC,UN,MF,NOROUTE,JOBN,PROTOCL)
implicit none
IMPNONE
*
*AUTEUR     M. VALIN
*
*OBJET(TTTFINI)
*    VIDER LES TAMPONS ET FERMER LE FICHIER COMPRESSE
*
**
#include "cpress.cdk"
CHARACTER * 2 FC, MF
CHARACTER * 8 UN, JOBN,PROTOCL
CHARACTER * 3 ST
LOGICAL NOROUTE
INTEGER RBN, TEXTE(10), IER, MAINFRM
EXTERNAL TTTNDR3,WAWRIT,WACLOS,PACK,DISPOSE,RBN
INTEGER I
#include "params.cdk"
#include "lnkpak.cdk"
*
*
*
IF ((PROTOCL == 'KMW') .AND. (.NOT. ASKI))
  $(
     CALL TTTNDR3
     IF (UN == 'NOROUTE') RETURN
     DO I = 1,7
       TEXTE(I) = 0
     IF (UN == 'FTPV80')
       $(
         ENCODE(72,85,TEXTE) JOBN
85       FORMAT('ATTACH,A=UTILLIB/UN=ASPHLIB.LIBRARY,A.CTASK.FTPV80,',_
                 'AAARAZZ,',A8)
         TEXTE(9) = RBN(TEXTE(9))
       $)
     ELSE
     IF (UN == 'SUNV80')
       $(
         ENCODE(72,95,TEXTE) JOBN
95       FORMAT('ATTACH,A=UTILLIB/UN=ASPHLIB.LIBRARY,A.CTASK.SUNV80,',_
                 'AAARAZZ,',A8)
         TEXTE(9) = RBN(TEXTE(9))
       $)
     ELSE
       IF (UN == 'CENTRAL')
        $(
         ST = 'PLT'
         ENCODE(40,96,TEXTE) FC,ST
96       FORMAT('CTASK.ROUTE,AAARAZZ,DC=PR,FC=',A2,',ST=',A3,'.')
         TEXTE(5) = RBN(TEXTE(5))
        $)
       ELSE
         IF (UN == 'ARMNV80')
           $(
             ENCODE(72,97,TEXTE) JOBN
97           FORMAT('ATTACH,A=UTILLIB/UN=ASPHLIB.LIBRARY,A.CTASK.KMWV80,',_
                    'AAARAZZ,',A8)
             TEXTE(9) = RBN(TEXTE(9))
           $)
       ELSE
         $(
           ENCODE(48,99,TEXTE) FC,UN
99         FORMAT('CTASK.ROUTE,AAARAZZ,DC=PR,FC=',A2,',UN=',A7,'.')
           TEXTE(6) = RBN(TEXTE(6))
         $)
     IF ((DEVICE <> IBMK) .AND. (DEVICE <> TAPE) .AND. (.NOT. NOROUTE)_
         .AND. (DEVICE <> V80T))
       IF (MF <> 'MC')
         $(
           READ(MF,'(R2)') MAINFRM
           MAINFRM = LSHIFT(MAINFRM,48)
           CALL DISPOSE(IER,LJUST(DN),LJUST(RAZZ2),LJUST(SDN),LJUST(AAARAZZ),
                   LJUST(DC),LJUST(ST),LJUST(DF),LJUST(BB),LJUST(MF),
                   MAINFRM,LJUST(TEXT),TEXTE,LJUST(WAIT))
         $)
       ELSE
         CALL DISPOSE(IER,LJUST(DN),LJUST(RAZZ2),LJUST(SDN),
               LJUST(AAARAZZ),LJUST(DC),LJUST(ST),LJUST(DF),
               LJUST(BB),LJUST(TEXT),TEXTE,LJUST(WAIT))
  $)
ELSE IF (PROTOCL == 'LINK')
  $(
    DO I = LAST + 1,1920
      BUF(I) = 0
    CALL PACK(PKBF,8,BUF,1920)
    CALL WAWRIT( 22,PKBF,CURADR,BYT1920)
    CURADR = CURADR + BYT1920
    DO I = 1,5
      PKBF(I) = SHIFT(I,4*I)
    CALL WAWRIT( 22, PKBF,CURADR,BYT1920 )
    CALL WACLOS( 22 )
  $)
RETURN
END
#endif
#if defined (UNIX)  
*** S/P TTTFINI - TERMINER ET FERMER LE FICHIER COMPRIME
#include "aaaamac.cdk"
*
      SUBROUTINE TTTFINI(FC, UN, MF, NOROUTE, JOBN, PROTOCL)
#include "impnone.cdk"
      CHARACTER* 2 FC, MF
      CHARACTER* 8 UN, JOBN, PROTOCL
      LOGICAL      NOROUTE
*
*AUTEUR       M. VALIN
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTFINI)
*    VIDER LES TAMPONS ET FERMER LE FICHIER COMPRESSE
*
*MODULES
      EXTERNAL TTTNDR3, WAWRIT, WACLOS
*
**
      INTEGER  I
#include "params.cdk"
#include "lnkpak.cdk"

      IF(PROTOCL .EQ. 'KMW') THEN
         CALL TTTNDR3
      ELSE
         IF(PROTOCL .EQ. 'LINK') THEN
            DO 10 I = LAST + 1,1920
               BUF(I) = 0
 10            CONTINUE
            CALL WACLOS( 22 )
         ENDIF
      ENDIF
      RETURN
      END
#endif
#endif
