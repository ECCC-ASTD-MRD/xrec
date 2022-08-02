***S/P TTTINIT - INITIALISE LE BLOCK COMMON PARAMS
*
#include "aaaamac.cdk"
      SUBROUTINE TTTINIT
#include "impnone.cdk"
*
*AUTEUR   - M. VALIN  -  FEV 78
*
*REVISION 001 Y. BOURASSA - JUIN 94 RATFOR @ FTN77
*
*LANGUAGE - FTN77
*
*OBJET(INIT)
*         - INITIALISER LES VARIABLES DU BLOCK COMMON /PARAMS/
*           ENTRE CHAQUE GRAPHIQUE
*
*LIBRAIRIES
*         - SOURCE    METACODETRANSLATORPL,ID=RMN.  DECK=INIT
*         - OBJET     METACODETRANSLATOROB,ID=RMN.
*
*PARAMETRES
*         - PARAMETER - TECTRON, LOW, STANDARD, NO
*         - CALL      - PARAMS
*
*APPEL    - CALL TTTINIT
*
*IMPLICITES
*         - VARIABLES DU BLOC PARAMS
#include "params.cdk"
#include "patrns.cdk"
#include "mapvar.cdk"
#include "segvar.cdk"
#include "palpat.cdk"
*
*MODULES
      EXTERNAL TTTWNDW, TTTPWI1
*
**
      INTEGER MXPM1
      PARAMETER (MXPM1 = MAXPILE - 1) 
      DATA IPUNIT,MCUNIT/6,8/
      DATA SEGUNIT /9/
      DATA PILE /1/
      DATA SAVUNIT /8, MXPM1 * 9/
      DATA SEGCLOS /.TRUE./
      DATA TABFONT /0,1,2,3,4,5,6,7,8,9/
      DATA COLCOMP /-1,0,7,3,4,1,2,5,6,0,7,3,4,1,2,5,6,-1/
      DATA CONSTCK /0/
      DATA OPTSTCK /0/
      DATA BASLOPT /0/

      CURX       = 0           ! NOW INITIALIZE PEN POSITION AT ORIGIN
      CURY       = 0
      KURX       = 0 
      KURY       = 0
      COLLIN(-1) = -1
      COLLIN(0)  = FOND
      COLLIN(1)  = 1
      COLCAR(-1) = -1
      COLCAR(0)  = FOND
      COLCAR(1)  = 1
      MODPOL     = 1
      MODLIN     = 1
      INTLIN     = 1
      MODCAR     = 1
      INTCAR     = 1
      ICSIZ      = 16/ALPHA    ! SET CHARACTR SIZE TO FUNDAMENTAL
      LCW        = ICSIZ
      LCH        = ICSIZ
      LHS        = ICSIZ
      LVS        = ICSIZ
      ICLRFLG    =.FALSE.      ! CLEAR BEFORE PRINT OFF
      IENHANC    = 0           ! ENHANCE PRINT OFF
      CFILL      = .FALSE.     ! CHARACTER BOX FILL OFF
      UPPERC     = .TRUE.
      NPAT       = 0
      IORN       = 0           ! SET CHARACTR ORIENTATION TO HORIZONTAL
      ISLANT     = 0
      IANOT      = 0
      CALL TTTPWI1


      IFNTCAS    = TABFONT(0)  ! SET UPPERCASE, ALL OPTIONS OFF
      IBXFLG     = .FALSE.
      IREVFLG    = .FALSE.
      IUNDFLG    = .FALSE.
      FIXFLG     = .FALSE.
      CNTRY      = 1
      CNTRX      = 2
      BPAIRS(1)  = 58376
      BPAIRS(2)  = 0            ! WINDOW PREND LES VALEURS DE
      BPAIRS(3)  = 0            ! XMIN, YMIN, XMAX, YMAX DANS
      BPAIRS(4)  = 32767        ! BPAIRS(2) ... (3) ... (4) ... (5)
      BPAIRS(5)  = 32767        ! FENETRE PLEINE

      CALL TTTWNDW

      RETURN
      END
