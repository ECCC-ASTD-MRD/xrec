***S/P TTTDSBX - TRACE L'ENCADREMENT D'UN TEXTE
#include "aaaamac.cdk"
*
      SUBROUTINE TTTDSBX(IDBOXX, IDBOXY, IFBOXX, IFBOXY, INT, MODE,
     +                   FONTNB)
#include "impnone.cdk"
      INTEGER  IDBOXX, IDBOXY, IFBOXX, IFBOXY, INT, MODE, DISTY, FONTNB
#include "params.cdk"
*
*AUTEUR   - M. VALIN
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTDSBX)
*         - CALCULE LES 4 COINS D'UN RECTANGLE QUI ENCADRERA UNE SERIE
*           DE CARACTERES
*         - VARIATION EN X ET EN Y POUR TRACER LE RECTANGLE,
*           4           3                   X   Y
*           -------------               1) -1  -5
*           I           I               2)  1  -5
*           I           I               3)  1   10
*           I           I               4) -1   10
*           -------------
*           1           2
*
*ARGUMENTS
*         - IDBOXX - COORDONNEE X DU DEBUT DE LA CHAINE A ENCADRER
*         - IDBOXY - COORDONNEE Y DU DEBUT DE LA CHAINE A ENCADRER
*         - IFBOXX - COORDONNEE X DE LA FIN DE LA CHAINE A ENCADRER
*         - IFBOXY - COORDONNEE Y DE LA FIN DE LA CHAINE A ENCADRER
*         - INT    - INTENSITE DU TRAIT
*         - MODE  - MODE D'ECRITURE (VOIR MODLIN/MODCAR DANS/PARAMS/)
*
*IMPLICITES
#include "pwryp.cdk"
#include "carac.cdk"
*
*MODULES
      EXTERNAL TTTDRAW
*
**
      REAL     RX0, RX1, RX2, RX3, RX4, RY0, RY1, RY2, RY3, RY4

      RX0 = IFBOXX
      RY0 = IFBOXY
      IF( UPPERC ) THEN
         DISTY = IVH(FONTNB,3)
      ELSE
         DISTY = IVH(FONTNB,5)
      ENDIF
      RX1 = IDBOXX-4*X1-DISTY*X2+.5     ! CALCULE LES COORDONNEES
      RY1 = IDBOXY-4*Y1-DISTY*Y2+.5     ! DU RECTANGLE A ENCADRER
      RX2 = IFBOXX+4*X1-DISTY*X2+.5
      RY2 = IFBOXY+4*Y1-DISTY*Y2+.5
      RX3 = IFBOXX+4*X1-IVH(FONTNB,4)*X2+.5
      RY3 = IFBOXY+4*Y1-IVH(FONTNB,4)*Y2+.5
      RX4 = IDBOXX-4*X1-IVH(FONTNB,4)*X2+.5
      RY4 = IDBOXY-4*Y1-IVH(FONTNB,4)*Y2+.5

      CALL TTTDRAW(RX1,RY1,0,INT,MODE) ! TRACE
      CALL TTTDRAW(RX2,RY2,1,INT,MODE)
      CALL TTTDRAW(RX3,RY3,1,INT,MODE)
      CALL TTTDRAW(RX4,RY4,1,INT,MODE)
      CALL TTTDRAW(RX1,RY1,1,INT,MODE)
      CALL TTTDRAW(RX0,RY0,0,INT,MODE)


      RETURN
      END
