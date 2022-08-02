#if !defined (NATIF) 
***S/P TTTDRAW - EXECUTE UN MOUVEMENT DE PLUME EN TENANT COMPTE
*                DE LA FENETRE DE TRAVAIL.
#include "aaaamac.cdk"
*
      SUBROUTINE TTTDRAW(RX, RY, ICP, INTEN, MODIMP)
#include "impnone.cdk"
*
*AUTEURS       - M. VALIN
*
*REVISION 001  - I.C. PITCHEN - JUILLET 86 - POUR VERSION CDC
*         002  - P. JOLICOEUR - HIVERS 83    - POUR TELIDON
*         003  - M. LEPINE    - CORRECTION D'INTENSITE POUR DIFFERENTS DEVICES
*         004  - Y. BOURASSA  - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(VDRAW)
*         - ETANT DONNE LES COORDONNEES X ET Y D'UNE INSTRUCTION
*           ET DU CONTROLE DE LA PLUME, ON EXECUTE UN  MOUVEMENT
*           EN  TENANT  COMPTE  DE LA FENETRE DE TRAVAIL.    LES
*           COORDONNEES  SONT TRANSFORMEES EN COORDONNEES  (U,V)
*           POUR LE VARIAN EN TENANT COMPTE DE L'ORIENTATION  DU
*           GRAPHIQUE, POUR LE TEKTRONIC ET TELIDON, ELLES  SONT
*           LAISSEES TELLE QUELLE.
*
*LIBRAIRIES
*         - SOURCE   METACODETRANSLATORPL,ID=RMN.     DECK=VDRAW
*         - OBJET    METACODETRANSLATOROB,ID=RMN.
*
*PARAMETRES
*         - PARAMETER - TEKTRONIC,PLOT,TELIDON
*
*APPEL    - CALL TTTDRAW(NX,NY,ICP)
*
*ARGUMENTS
*         - NX  - COORDONNEE X (16 BITS A DROITE CONTENUES DANS INS(1)
*         - NY  - COORDONNEE Y (15 DERNIERES BITS DE INS(2)
*         - ICP - CONTROLE DE LA PLUME (16E BIT DE INS(2)
*         - INTEN - INTENSITE DU TRAIT
*         - MODIMP - MODE D'IMPRESSION (NORMAL REVERSE)
*
*IMPLICITES
*         - BLOC      - /PARAMS/
*         - MODEP     - INDIQUE L'INSTRUMENT QUE L'ON EMPLOIE
*         - CURX,CURY - POSITION DE LA PLUME AU DEPART EN (X,Y)
*         - IROT      - ROTATION DU GRAPHIQUE
*         - RSCALE    - FACTEUR DE NORMALISATION
*         - COFFSET   - PREMIER POINT DU GRAPHIQUE EN X
*         - CURU,CURV - POSITION DE LA PLUME AUDEPART EN (U,V) (VARIAN)
*         - XMIN,YMIN - COORDONNEE DU POINT MINIMUM DELA FENETRE DE TRAVAIL
*         - XMAX,YMAX - COORDONNEE DU POINT MAXIMUM DELA FENETRE DE TRAVAIL
*                                    1;TRACER NOIR SUR BLANC (NORMAL)
*         - MODE      - INDIQUE LE NB DE BYTES POUR  UNE PAIRE DE COORDONNEES
*
*
*VARIABLES
*         - WINTES -   INDICATEUR QUI SPECIFIE SI ON EST DANS LA FENETRE DE
*                      TRAVAIL OU NON ET SI ON PEUTTRACER
*                         -2; 2 POINT HORS CHAMP,ON NE PEUT PAS TRACER
*                          0; 2 POINT DANS LE CHAMP, ON PEUT TRACER
*                          1; 1 POINT HORS CHAMP,ON PEUT TRACER
*                          2; 2 POINT HORS CHAMP,ON PEUT TRACER
*
*
*MODULES  - 
#if defined (UNIVERS) 
      EXTERNAL UNIDRWA, UNIMOVA
#endif
      EXTERNAL TTTCLIP, TTTV1IOR, TTTLINE
      INTEGER  TTTCLIP 
*
*NOTES    - COMME VDRAW EMPLOIE DES ROUTINES SPECIFIQUES AU TEKTRONIC
*           AVEC MODE GRAPHIQUE,BIEN VERIFIER QUE CES ROUTINES SONT UTILISABLES.
*
**
#include "params.cdk"
#include "teldon.cdk"
#include "argum.cdk"
#include "mapvar.cdk"
#if defined (UNIX)
#include "ivar.cdk"
#endif
      INTEGER  NX, NY, BPAIR, OPCODE, JTEC, IV1, IV2, ICP, II, WINTES,  
     +         IX, IY, INTEN, MODIMP, ITEC, IU1, IU2, CC0, BS0
      REAL     RX, RY
*
#include "mapping.cdk"

      NX = RX
      NY = RY
#if defined (UNIVERS) 
      IX = NX
      IY = NY
      IX = NINT(IX * C1 + A1)
      IY = NINT(IY * C1 + B1)
      IF(ICP .EQ. 0) THEN
         CALL UNIMOVA(IX, IY)
      ELSE 
*        ON DETERMINE SI LES COORDONNEES SONT DANS LA
*        FENETRE DE TRAVAIL AVEC TTTCLIP
         WINTES = TTTCLIP(XMIN, XMAX, YMIN, YMAX, CURX, CURY, IX, IY)
         IF(WINTES .NE. -2) THEN
            IF((KURX.NE.CURU) .OR. (KURY .NE. CURV))
     +         CALL UNIMOVA(KURX,KURY)
            CALL UNIDRWA(IX, IY, INTEN, MODIMP)
            CURU = IX
            CURV = IY
         ENDIF
      ENDIF
      CURX = NX            ! ON GARDE LE POINT OU L'ON EST RENDU,
      CURY = NY            ! CURX,CURY (TEKTRONIC)
      KURX = IX
      KURY = IY
#endif
      IU2  = RMAPU(RX, RY)
      IV2  = RMAPV(RX, RY)
      IU1  = CURU
      IV1  = CURV
      CURU = IU2
      CURV = IV2
      CURX = NX
      CURY = NY
      IF(ICP .EQ. 1) THEN
         IF(IU1 .GT. IU2) THEN
            X1 = IU2
            X2 = IU1
            Y1 = IV2
            Y2 = IV1
         ELSE 
            X1 = IU1
            X2 = IU2
            Y1 = IV1
            Y2 = IV2
         ENDIF
         IF(TTTCLIP(UMIN, UMAX, VMIN, VMAX, X1, Y1, X2, Y2) .EQ. -2)
     +      RETURN
         LARG = INTEN
         IF((LARG.GE.1) .AND. (LARG.LE.5)) LARG = MAX(1, TABPEN(LARG))
#if defined (CRAY) 
         BS0 = BASE
#endif
#if defined (UNIX) 
         BS0 = PTIV
#endif
         IF(MODIMP .EQ. -1) THEN
            MODE = -1
         ELSE
            MODE = TABCOL( MODIMP )
         ENDIF
         CC0 = MODE
         IF((CC0.EQ.0) .AND. ((DEVICE.EQ.V80) .OR. (DEVICE.EQ.V80T)
     +     .OR. (DEVICE.EQ.V200) .OR. (DEVICE.EQ.DIFAX)
     +     .OR. (DEVICE.EQ.DFI)  .OR. (DEVICE.EQ.TANDEM))) 
     +     LARG = MAX(2,LARG)
         DO 10 II = 1,NCP
            IF(MODE .NE. -1) MODE = IAND(CC0 , 1)
#if defined (CRAY) 
            IF(LARG .EQ. 1) THEN
               CALL TTTV1IOR
            ELSE
               CALL TTTLINE
            ENDIF 
            BASE = BASE + LCP
            CC0  = RSHIFT(CC0,1)
 10         CONTINUE

         BASE = BS0
#endif
#if defined (UNIX) 
             IF(LARG .EQ. 1) THEN
                CALL TTTV1IOR(IVAR(BS0))
             ELSE
                CALL TTTLINE( IVAR(BS0) )
             ENDIF
             BS0 = BS0 + LCP
             CC0 = ISHFT(CC0, -1)
 10          CONTINUE
#endif
       ENDIF
       RETURN
       END
#endif
#if defined (NATIF) && defined (C910)
***S/P TTTDRAW - EXECUTE UN MOUVEMENT DE PLUME EN TENANT COMPTE
*                DE LA FENETRE DE TRAVAIL.
*
#include "aaaamac.cdk"
      SUBROUTINE TTTDRAW(RX, RY, ICP, INTEN, MODIMP)
#include "impnone.cdk"
      REAL               RX, RY
      INTEGER                    ICP, INTEN, MODIMP
*
*AUTEURS       - M. VALIN
*
*REVISION 001  - I.C. PITCHEN - JUILLET 86 - POUR VERSION CDC
*         002  - P. JOLICOEUR - HIVERS 83    - POUR TELIDON
*         003  - M. LEPINE    - CORRECTION D'INTENSITE POUR DIFFERENTS DEVICES
*         004  - Y. BOURASSA  - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*
*OBJET(VDRAW)
*         - ETANT DONNE LES COORDONNEES X ET Y D'UNE INSTRUCTION
*           ET DU CONTROLE DE LA PLUME, ON EXECUTE UN  MOUVEMENT
*           EN  TENANT  COMPTE  DE LA FENETRE DE TRAVAIL.    LES
*           COORDONNEES  SONT TRANSFORMEES EN COORDONNEES  (U,V)
*           POUR LE VARIAN EN TENANT COMPTE DE L'ORIENTATION  DU
*           GRAPHIQUE, POUR LE TEKTRONIC ET TELIDON, ELLES  SONT
*
*LIBRAIRIES
*         - SOURCE   METACODETRANSLATORPL,ID=RMN.     DECK=VDRAW
*         - OBJET    METACODETRANSLATOROB,ID=RMN.
*
*PARAMETRES
*         - PARAMETER - TEKTRONIC,PLOT,TELIDON
*
*APPEL    - CALL TTTDRAW(NX,NY,ICP)
*
*ARGUMENTS
*         - NX     - COORDONNEE X (16 BITS A DROITE CONTENUES DANS INS(1)
*         - NY     - COORDONNEE Y (15 DERNIERES BITS DE INS(2)
*         - ICP    - CONTROLE DE LA PLUME (16E BIT DE INS(2)
*         - INTEN  - INTENSITE DU TRAIT
*         - MODIMP - MODE D'IMPRESSION (NORMAL REVERSE)
*
*IMPLICITES
*         - BLOC      - /PARAMS/
*         - MODEP     - INDIQUE L'INSTRUMENT QUE L'ON EMPLOIE
*         - CURX,CURY - POSITION DE LA PLUME AU DEPART EN (X,Y)
*         - IROT      - ROTATION DU GRAPHIQUE
*         - RSCALE    - FACTEUR DE NORMALISATION
*         - COFFSET   - PREMIER POINT DU GRAPHIQUE EN X
*         - CURU,CURV - POSITION DE LA PLUME AUDEPART EN (U,V) (VARIAN)
*         - XMIN,YMIN - COORDONNEE DU POINT MINIMUM DELA FENETRE DE TRAVAIL
*         - XMAX,YMAX - COORDONNEE DU POINT MAXIMUM DELA FENETRE DE TRAVAIL
*
*VARIABLES
*         - WINTES -   INDICATEUR QUI SPECIFIE SI ON EST DANS LA FENETRE DE
*                      TRAVAIL OU NON ET SI ON PEUTTRACER
*                         -2; 2 POINT HORS CHAMP,ON NE PEUT PAS TRACER
*                          0; 2 POINT DANS LE CHAMP, ON PEUT TRACER
*                          1; 1 POINT HORS CHAMP,ON PEUT TRACER
*                          2; 2 POINT HORS CHAMP,ON PEUT TRACER
*
*
*MODULES  
      EXTERNAL DRAW2I, MOVE2I, RECTFI, LINEWI, COLOR
*
*NOTES    - COMME VDRAW EMPLOIE DES ROUTINES SPECIFIQUES AU TEKTRONIC
*           AVEC MODE GRAPHIQUE,BIEN VERIFIER QUE CES ROUTINES SONT UTILISABLES.
*
#include "params.cdk"
#include "argum.cdk"
#include "mapvar.cdk"
#include "com910.cdk"
*
      INTEGER IU1, IU2, PI1, PI2, BS0, NX, NY, WINTES, JTEC, BPAIR, I,  
     +        IV1, IV2, PJ1, PJ2, CC0, IX, IY, OPCODE, ITEC, II
*
#include "mapping.cdk"
*
      NX   = RX
      NY   = RY
      IU2  = RMAPU(RX, RY)
      IV2  = RMAPV(RX, RY)
      IU1  = CURU
      IV1  = CURV
      CURU = IU2
      CURV = IV2
      CURX = NX
      CURY = NY
      IF(ICP .EQ. 1 ) THEN
         LARG = INTEN
         IF((LARG.GE.1) .AND. (LARG.LE.5)) LARG = MAX(1,TABPEN(LARG))

         IF(MODIMP .EQ. -1) THEN
            MODE = -1
         ELSE
            MODE = TABCOL( MODIMP )
         ENDIF
         CC0 = MODE
         IF(CC0 .NE. LASTCOL) THEN
            CALL COLOR( CC0 )
            LASTCOL = CC0
         ENDIF
         IF(LARG .NE. LASTTIK) THEN
             CALL LINEWI( LARG )
             LASTTIK = LARG
         ENDIF
         IF((IU1.EQ.IU2) .AND. (IV1.EQ.IV2) .AND. (LARG.GT.1)) THEN
            PI1 = IU1 - LARG/2
            PJ1 = IV1 - LARG/2
            PI2 = PI1 + LARG-1
            PJ2 = PJ1 + LARG-1
            CALL RECTFI(PI1, PJ1, PI2, PJ2)
         ELSE
            CALL MOVE2I(IU1, IV1)
            CALL DRAW2I(IU2, IV2)
         ENDIF
      ENDIF

      RETURN
      END
#endif
