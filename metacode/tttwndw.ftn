*** S/P TTTWNDW - DETERMINER LES BORNES DE LA FENETRE DE TRAVAIL
#include "aaaamac.cdk"
*
      SUBROUTINE TTTWNDW
#include "impnone.cdk"
*
*AUTEUR       M. VALIN
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTWNDW)
*     DETERMINER LES BORNES DE LA FENETRE DE TRAVAIL
*
#if defined (NATIF) && defined (C910) 
*MODULE
      EXTERNAL WGLCMI
#endif
*
**
      INTEGER IU1, IU2, IV1, IV2
#include "params.cdk"
#include "argum.cdk"
#include "mapvar.cdk"
#include "mapping.cdk"
      XMIN = IAND(BPAIRS(2), 32767)
      YMIN = IAND(BPAIRS(3), 32767)
      XMAX = IAND(BPAIRS(4), 32767)
      YMAX = IAND(BPAIRS(5), 32767)
#if defined (UNIVERS) 
      XMIN = NINT(XMIN * C1 + A1)
      YMIN = NINT(YMIN * C1 + B1)
      XMAX = NINT(XMAX * C1 + A1)
      YMAX = NINT(YMAX * C1 + B1)
#endif
#if defined (RASTER) 
      IF ((XMIN.EQ.0 ) .AND. (YMIN.EQ.0) .AND. (XMAX.EQ.0) .AND.
     +    (YMAX.EQ.0)) THEN
         UMIN = OFFSET
         UMAX = OFFSET + HU
         VMIN = 0
         VMAX = HV
      ELSE
         IU1  = MAPU(XMIN,YMIN)
         IV1  = MAPV(XMIN,YMIN)
         IU2  = MAPU(XMAX,YMAX)
         IV2  = MAPV(XMAX,YMAX)
         UMIN = MAX(OFFSET,MIN(IU1,IU2))
         VMIN = MAX(0,MIN(IV1,IV2))
         UMAX = MIN(OFFSET + HU,MAX(IU1,IU2),DEVWIDT)
         VMAX = MIN(HV,MAX(IV1,IV2))
      ENDIF
      CURU = MAPU(CURX,CURY)
      CURV = MAPV(CURX,CURY)
#endif
#if defined (NATIF) 
      CALL WGLCMI(UMIN, VMIN, UMAX, VMAX)
#endif
      RETURN
      END
