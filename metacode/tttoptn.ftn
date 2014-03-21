***S/P  TTTOPTN      SPECIFIER L'OPTION DEMANDE DANS LE BLOCK COMMON /PARAMS/
#include "aaaamac.cdk"
*
      SUBROUTINE TTTOPTN
#include "impnone.cdk"
*
*AUTEUR        - R. BOULANGER - M. VALIN
*
*REVISION 001  - A.COTE NOV,78 ENHANCE,NOENHANC,CLPRNT,NOCLPRNT
*         002  - NOV,83
*         003  -  M. LEPINE   -  TRAITEMENT DES COULEURS ET AUTRE
*         004  -  Y. BOURASSA -  MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTOPTN)
*         - EXTRAIRE DE LA MULTIBYTE L'OPTION DEMANDE ET METTRE LA
*           VALEUR EXTRAITE DANS LE BLOCK COMMON APPROPRIE
*
*MESSAGES
*         - SUR IMPRIMANTE    "OPTION UNKNOWN=  IGNORED"
*
*IMPLICITES
*         - BLOCK    - /PARAMS/
*         VOIR LA DEFINITION DANS LE MODULE TRADUCT
*
#include "params.cdk"
#include "patrns.cdk"
#include "pwryp.cdk"
*MODULES
      EXTERNAL TTTPWI1, TTTPWI2, TTTPWI3
*
**
      INTEGER OPCODE, OPVAL, MINVAL, VALEUR, MAXVAL, ENTRE

      ENTRE(MINVAL, VALEUR, MAXVAL) = MAX(MINVAL, MIN(VALEUR, MAXVAL))

      IF(IAND(BPAIRS(1) , 255) .NE. 4) THEN
         PRINT *,' *** OPTION CAN"T HAPPEN'
         RETURN
      ENDIF

      OPCODE = IAND(ISHFT(BPAIRS(2),-8), 255) ! EXTRACTION DE L'OPTION
      OPVAL  = BPAIRS(3) - 32768


*     QUEL EST L'OPTION DEMANDEE?
      IF(OPCODE.GT.0 .AND. OPCODE.LT.31) THEN
         GOTO( 1,  2,  3,  4,  5,  6,  7, 10, 11, 12, 13, 14, 15, 16, 
     +        17, 20, 21, 22, 23, 24, 25, 26, 27, 30, 31, 32, 33, 34,
     +        35, 36) OPCODE
 1       IREVFLG = OPVAL .EQ. 1
         GOTO 100

 2       INTCAR  = ABS(OPVAL)   !   POUR LES CARACTERES
         GOTO 100

 3       IANOT   = OPVAL        !   ANGLE D'ANNOTATION
         CALL TTTPWI1
         GOTO 100

 4       ICSIZ   = ABS(OPVAL)
         LCW     = ICSIZ
         LCH     = ICSIZ
         LVS     = ICSIZ
         LHS     = ICSIZ
         CALL TTTPWI2
         GOTO 100

 5       IFNTCAS = TABFONT(ABS(OPVAL))
         GOTO 100

 6       IUNDFLG = OPVAL .EQ. 1
         GOTO 100

 7       IBXFLG  = OPVAL .EQ. 1
         GOTO 100

 10      MODLIN  = ENTRE(-1, OPVAL, 1)
         GOTO 100

 11      CONTINUE
#if defined (UNIVERS) 
         CALL UNIOPT("COLOR", OPVAL)
#endif
         INTLIN  = ABS(OPVAL)
         GOTO 100

 12      ICLRFLG = OPVAL .EQ. 1
         GOTO 100

 13      IENHANC = OPVAL
         GOTO 100

 14      LHS     = ABS( OPVAL )
         CALL TTTPWI3
         GOTO 100

 15      LVS     = ABS( OPVAL )
         CALL TTTPWI3
         GOTO 100

 16      IORN    = OPVAL
         CALL TTTPWI1
         GOTO 100

 17      ISLANT  = OPVAL
         CALL TTTPWI1
         GOTO 100

 20      CONTINUE
#if defined (UNIVERS) 
         CALL UNIOPT("COLOR", OPVAL)
#endif
#if defined (RASTER) 
         COLLIN(1) = ENTRE(0, OPVAL, 16)
#endif
         GOTO 100

 21      CONTINUE
#if defined (UNIVERS) 
         CALL UNIOPT("BLNK", OPVAL)
#endif
         GOTO 100
 
 22      CONTINUE
#if defined (UNIVERS) 
         CALL UNIOPT("DASH", OPVAL)
#endif
         GOTO 100

 23      LCW     = ABS(OPVAL)
         CALL TTTPWI2
         GOTO 100

 24      LCH     = ABS(OPVAL)
         CALL TTTPWI2
         GOTO 100

 25      NPAT    = ENTRE(0, OPVAL, MAXPATS)
         GOTO 100

 26      MODCAR  = ENTRE(-1,OPVAL,1)
         GOTO 100

 27      CNTRY   = ENTRE(0, OPVAL/10, 4)
         CNTRX   = ENTRE(0, MOD(OPVAL,10), 4)
         GOTO 100

 30      FIXFLG  = OPVAL .EQ. 1
         GOTO 100

 31      UPPERC  = OPVAL .EQ. 1
         GOTO 100

 32      CONTINUE
#if defined (RASTER) 
         COLCAR(1) = ENTRE(0, OPVAL, 16)
#endif
         GOTO 100

 33      MODPOL  = ENTRE(-1, OPVAL, 1)
         GOTO 100

 34      COLCAR(0) = ENTRE(0, OPVAL, 16)
         GOTO 100

 35      CFILL = OPVAL .EQ. 1
 36      GOTO 100
      ELSE
         WRITE(IPUNIT,200)OPCODE    ! OPTION NON DEFINIE
      ENDIF
 100  CONTINUE

      RETURN
 200  FORMAT(20X,'****OPTION INCONNUE =',I5,' IGNOREE')
      END
