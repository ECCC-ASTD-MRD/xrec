#if defined (RASTER) 
*** S/P TTTCADR - FAIRE UN CADRE DE LA GRANDEUR DU FRAME
#include "aaaamac.cdk"
*
      SUBROUTINE TTTCADR
#include "impnone.cdk"
*
*AUTEURS      M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTCADR)
*    SERT A FAIRE UN CADRE D'INTENSITE 1 AUTOUR DU GRAPHIQUE
*    LE CADRE SERA DE GRANDEUR HU PAR HV
*
**
#include "params.cdk"
#include "argum.cdk"
#include "ivar.cdk"
      INTEGER  HVV
      EXTERNAL TTTV1IOR
*
      MODE = -1
      X1   = OFFSET
      Y1   = 0
      X2   = OFFSET
      Y2   = HV
      CALL TTTV1IOR( IVAR(PTIV) )
      X1   = OFFSET
      Y1   = HV
      X2   = MIN(OFFSET + HU,DEVWIDT)
      Y2   = HV
      CALL TTTV1IOR( IVAR(PTIV) )
      X1   = OFFSET
      Y1   = 0
      X2   = MIN(OFFSET + HU,DEVWIDT)
      Y2   = 0
      CALL TTTV1IOR( IVAR(PTIV) )
      X1   = MIN(OFFSET + HU,DEVWIDT)
      Y1   = 0
      X2   = MIN(OFFSET + HU,DEVWIDT)
      Y2   = HV
      CALL TTTV1IOR( IVAR(PTIV) )
      RETURN

      ENTRY TTTCDR2
      HVV  = MIN(HV,1605)
      MODE = -1
      X1   = OFFSET
      Y1   = 0
      X2   = OFFSET
      Y2   = HVV
      CALL TTTV1IOR( IVAR(PTIV) )
      X1   = OFFSET
      Y1   = HVV
      X2   = DEVWIDT - MODEP
      Y2   = HVV
      CALL TTTV1IOR( IVAR(PTIV) )
      X1   = OFFSET
      Y1   = 0
      X2   = DEVWIDT - MODEP
      Y2   = 0
      CALL TTTV1IOR( IVAR(PTIV) )
      X1   = DEVWIDT - MODEP
      Y1   = 0
      X2   = DEVWIDT - MODEP
      Y2   = HVV
      CALL TTTV1IOR( IVAR(PTIV) )
      RETURN
      END
#endif
