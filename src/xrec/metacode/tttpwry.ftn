***S/PTTTPWRY - FAIT TRACER CARACTERES, TENANT COMPTE DES OPTIONS DEMANDEES
#include "aaaamac.cdk"
*
      SUBROUTINE TTTPWRY
#include "impnone.cdk"
*
*AUTEUR   - R. BOULANGER  -  M. VALIN
*
*REVISION 001   A.COTE - NOV 78 - ENHANCE,NOENHANCE,CLRPRNT,NOCLRPRNT
*         002   P.JOLICOEUR/M.VALIN  - NOV,83
*         003   M. LEPINE   - PRINTEMPS 85     TRAITEMENT DES COULEURS
*         004   Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTPWRY)
*         - TTTPWRY PREPARE LES OPTIONS DEMANDES, EXTRAIT LES CARACTERES
*           A TRACER ET LES FAIT TRACER EN SPECIFIANT CES OPTIONS.
*
*           CARACTERE    CODE      DEFINITION
*           J,A-I        152B,     LA FONTE EST FIXE POUR UN SEUL CARACTERE
*                        141B-151B
*           0-9          20B-31B   LA FONTE EST FIXE POUR TOUTE LA CHAINE
*                                  DE CARACTERE
*           X-Z          170B-172B INTENSITE DU CARACTERE A TRACER
*           [            173B      ENCADRER LA CHAINE DE CARACTERES SUIVANT
*           ]            175B      FIN DE LA CHAINE A ENCADRER
*           "             2B       ECRIRE BLANC SUR NOIR (MODE;EFFACER)
*           %             5B       ECRIRE NOIR SUR BLANC (MODE;NORMAL)
*           '             7B       SOUS-LIGNER LETEXTE SUIVANT
*           &             6B       FIN DE LA CHAINE A SOUS-LIGNER
*           ?            37B       CHANGER DE DEBUT DE LIGNE
*           <            34B       GROSSIR LES PROCHAINS CARACTERES
*           >            36B       RAPETISSER LES PROCHAINS CARACTERES
*           =            35B       CR + LF
*           \           174B       RECULER D'UN CARACTERE ("BACKSPACE")
*
*MESSAGES
*         - SUR IMPRIMANTE - "INTENSITE UNKNOWN  IGNORED"
*                            "OPTION OF TTTPWRY UNKNOWN  IGNORED"
*
*IMPLICITES
*         - /PWRYP/
*         - X1  - COORDONNEE X EN TENANT COMPTE DE L'ORIENTATION
*                 ET LA LARGEUR DU CARACTERE
*         - X2  - COORDONNEE X EN TENANT COMPTE DE L'ORIENTATION
*                 ET LA HAUTEUR DU CARACTERE
*         - Y1  - COORDONNEE Y EN TENANT COMPTE DE L'ORIENTATION
*                 ET LA LARGEUR DU CARACTERE
*         - Y2  - COORDONNEE Y EN TENANT COMPTE DE L'ORIENTATION
*                 ET LA HAUTEUR DU CARACTERE
*         - SPX - ESPACEMENT EN X
*         - SPY - ESPACEMENT EN Y
*
#include "pwryp.cdk"
#include "patrns.cdk"
#include "mapvar.cdk"
#include "params.cdk"
#include "carac.cdk"
#include "palpat.cdk"
*NOTES    - LE MAXIMUM DE CARACTERES ACCEPTES PAR APPEL ; 256
*
*MODULES
      EXTERNAL TTTDRAW, TTTUNDR, TTTDSBX, TTTTONE, TTTPCAR, TTTCNTR
*
**
      LOGICAL  IAVFONT, STKBOX, STKAVF, STKREV, STKCLR, STKUNDR,
     +         STKFILL
      INTEGER  STKCURY, STKINT, IUNL1X, AUGINT, LEN, ICOL2, ICONT,
     +         STKCURX, FONTNB, IUNL1Y, IFPREC, IC1, ICOL3, CHAR, 
     +         STKFONT, STKENH, SHFCNT, INBBYT, IC2, PASSE, LNG, I,
     +         IEPAIS,  IDBOXX, IDBOXY, NBCHAR, MOT, NPATP, IUV(2,4)  
      REAL     DEBLX,   STKX1,  STKY1,  STKSPX, RC1, GGG(2,4),
     +         DEBLY,   STKX2,  STKY2,  STKSPY, RC2

      DATA GGG/-1.5, -13.0, 21.5, -13.0, 21.5, 25.0, -1.5, 25.0/

#include "mapping.cdk"

      AUGINT  = MIN(1, MODEP/100)
      CALL TTTCNTR( FONTNB )
      IAVFONT = .FALSE.

*     ON CONSERVE DANS DEBLX,DEBLY LES COORDONNEES DU DEBUT
*     DE LIGNE POUR UTILISATION AVEC CR ET CRLF
*     ICOINX,ICOINY DONNE LE POINT DE DEPART DU PROCHAIN CARACTERE A TRACER.
      DEBLX   = CURX
      DEBLY   = CURY
      STKCURX = CURX
      STKCURY = CURY
      STKFONT = IFNTCAS
      STKINT  = INTCAR
      STKX1   = X1
      STKY1   = Y1
      STKX2   = X2
      STKY2   = Y2
      STKSPX  = SPX
      STKSPY  = SPY
      STKBOX  = IBXFLG
      STKUNDR = IUNDFLG
      STKREV  = IREVFLG
      STKCLR  = ICLRFLG
      STKFILL = CFILL
      STKENH  = IENHANC
      STKAVF  = IAVFONT
      DO 310 PASSE = 1,3
         CURX    = STKCURX
         CURY    = STKCURY
         CALL TTTDRAW(FLOAT(CURX), FLOAT(CURY), 0, 0, 1)
         IFNTCAS = STKFONT
         INTCAR  = STKINT
         X1      = STKX1
         Y1      = STKY1
         X2      = STKX2
         Y2      = STKY2
         SPX     = STKSPX
         SPY     = STKSPY
         IBXFLG  = STKBOX
         IUNDFLG = STKUNDR
         IREVFLG = STKREV
         ICLRFLG = STKCLR
         CFILL   = STKFILL
         IENHANC = STKENH
         IAVFONT = STKAVF
         IF( IBXFLG ) THEN
            IDBOXX = CURX        ! OPTION "ENCADREMENT" DANS LE CALL
            IDBOXY = CURY
         ENDIF

         IF( IUNDFLG ) THEN      ! SOUS-LIGNAGE
            IUNL1X = CURX
            IUNL1Y = CURY
         ENDIF

*       ON EXTRAIT ET ON TRAITE SOIT LE CARACTERE , SOIT L'OPTION DEMANDE.
*             INBBYT ; INDIQUE A QUEL BYTE ON EST RENDU.
*              ICONT ; INDIQUE A QUEL MOT ON EST RENDU.
*             ICHARS EST CONSTRUIT DE LA FACON SUIVANTE;
*                       2 BYTES (16 BITS) PAR MOT ALIGNES A GAUCHE
*                       ET DONT CHAQUE BYTE REPRESENTE UNE INSTRUCTION.
*        LES CARACTERES A TRACER ONT DES VALEURS ENTRE 32 ET 95.
*        LES OPTIONS ONT DES VALEURS ENTRE 0 ET 32 ET AU DESSUS DE 95.

         LEN    = IAND(255, BPAIRS(1))
         ICONT  = 2
         INBBYT = -1
         NBCHAR = 0

 70      IF(NBCHAR .LT. LEN) THEN
            NBCHAR = NBCHAR+1
*           EXTRACTION DU CARACTERE ET DES BITS DE CONTROLE.
            INBBYT = INBBYT+1 
            CHAR   = IAND(255,
     +                ISHFT(BPAIRS(ICONT),-(8*(1-(IAND(INBBYT,1))))))
            ICONT  = ICONT + IAND(INBBYT , 1)
            
*           OPCODE ENTRE 0 ET 32

            GOTO(270, 270,  80, 270, 270,  90, 100, 110, 270, 270, 270,
     +           270, 270, 270, 270, 120, 120, 120, 120, 120, 120, 120,
     +           120, 120, 120, 120, 270, 270, 130, 140, 150, 160)
     +           CHAR+1

*           CHAR PLUS HAUT QUE 95
            GOTO(270, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170,
     +           270, 270, 270, 270, 270, 180, 190, 200, 210, 270, 270,
     +           270, 270, 220, 230, 240, 250, 270, 260, 270, 270)
     +           CHAR-95
            GOTO 280

*           SINON C'EST UN CARACTERE
 80         IREVFLG = .TRUE.
            GOTO 300

 90         IREVFLG = .FALSE.
            GOTO 300

 100        IF(PASSE .EQ. 3) THEN
               CALL TTTUNDR(IUNL1X, IUNL1Y, CURX, CURY, INTCAR,
     +                      COLCAR(MODCAR), FONTNB)
               IUNDFLG = .FALSE.
            ENDIF
            GOTO 300

 110        IUNL1X  = CURX
            IUNL1Y  = CURY
            IUNDFLG = .TRUE.
            GOTO 300
            
 120        IFNTCAS = TABFONT(CHAR-16) !  DE 20B A 31B
            IAVFONT = .FALSE.
            GOTO 300
            
 130        X1  = X1  *0.7071068
            Y1  = Y1  *0.7071068
            X2  = X2  *0.7071068
            Y2  = Y2  *0.7071068
            SPX = SPX *0.7071068
            SPY = SPY *0.7071068
            GOTO 300
            
 140        DEBLX = DEBLX-(16*X2+.5)
            DEBLY = DEBLY-(16*Y2+.5)
            CALL TTTDRAW(DEBLX, DEBLY, 0, 0, 0)
            GOTO 300
            
 150        X1  = X1  *1.4142136
            Y1  = Y1  *1.4142136
            X2  = X2  *1.4142136
            Y2  = Y2  *1.4142136
            SPX = SPX *1.4142136
            SPY = SPY *1.4142136
            GOTO 300

 160        DEBLX = CURX
            DEBLY = CURY
            GOTO 300

 170        IFPREC  = IFNTCAS   ! DE 140B A 152B
            IFNTCAS = IAND(CHAR , 15)
            IFNTCAS = TABFONT(MOD(IFNTCAS,10))
            IAVFONT = .TRUE.
            GOTO 300

 180        IENHANC = 1
            GOTO 300
            
 190        IENHANC = 0
            GOTO 300

 200        ICLRFLG = .TRUE.
            GOTO 300

 210        ICLRFLG = .FALSE.
            GOTO 300

 220        INTCAR  = 1         !  INTENSITE FAIBLE
            GOTO 300
            
 230        INTCAR  = MIN(INTCAR+1,255) !  INTENSITE NORMALE
            GOTO 300

 240        INTCAR  = MAX(1,INTCAR-1)
            GOTO 300

 250        IDBOXX  = CURX      !  DEBUT DE L'ENCADREMENT
            IDBOXY  = CURY
            IBXFLG  = .TRUE.
            GOTO 300
 260        IF(PASSE .EQ. 3) THEN
               CALL TTTDSBX(IDBOXX, IDBOXY, CURX, CURY, INTCAR,
     +                      COLCAR(MODCAR), FONTNB)
               IBXFLG = .FALSE.
            ENDIF
            GOTO 300

 270        WRITE(6,320) CHAR
            GOTO 300

*           ON TRACE LE CARACTERE
 280        IC1 = CURX 
            IC2 = CURY
            RC1 = REAL(IC1)
            RC2 = REAL(IC2)
#if defined (RASTER)
            IF(IREVFLG .OR. ICLRFLG .OR. CFILL) THEN
               IF(.NOT. FIXFLG) THEN
                  MOT      = IFNTCAS
                  LNG      = IVL(MOT, CHAR)
                  GGG(1,2) = LNG + 4.0
                  GGG(1,3) = GGG(1, 2)
               ELSE
                  GGG(1,2) = 22.0
                  GGG(1,3) = 22.0
               ENDIF
               IF( UPPERC ) THEN
                  GGG(2,1) = - IVH(IFNTCAS, 3)
                  GGG(2,2) = - IVH(IFNTCAS, 3)
               ELSE
                  GGG(2,1) = - IVH(IFNTCAS, 5)
                  GGG(2,2) = - IVH(IFNTCAS, 5)
               ENDIF
               GGG(2,3)    = - IVH(IFNTCAS, 4)
               GGG(2,4)    = - IVH(IFNTCAS, 4)
               DO 290 I=1,4
                  IUV(1,I) = MAPU(CURX+IFIX(X1*GGG(1,I)+X2*GGG(2,I)),
     +                            CURY+IFIX(Y1*GGG(1,I)+Y2*GGG(2,I)))
                  IUV(2,I) = MAPV(CURX+IFIX(X1*GGG(1,I)+X2*GGG(2,I)),
     +                            CURY+IFIX(Y1*GGG(1,I)+Y2*GGG(2,I)))
 290              CONTINUE
           ENDIF

            IF( IREVFLG ) THEN
*              ECRIT EN BLANC SUR FOND NOIR
               NPATP         = NPAT
               NPAT          = 129
               HPALET(1,129) = 1
               HPALET(2,129) = COLCAR(1)
               HPALET(3,129) = COLCAR(1)
*              NOIRCIR LE CARREAU DE LA LETTRE
               IF(PASSE .EQ. 1) CALL TTTTONE(IUV, 4, 1)

*              REMETTRE LE PATRON COURANT
               NPAT = NPATP
               CALL TTTPCAR(CHAR, IFNTCAS, INTCAR, COLCAR(0), IC1, IC2,
     +                      PASSE)
               ELSE
                  IF( CFILL ) THEN
                  NPATP         = NPAT
                  NPAT          = 129
                  HPALET(1,129) = 1
                  HPALET(2,129) = COLCAR(0)
                  HPALET(3,129) = COLCAR(0)
*                 NOIRCIR LE CARREAU DE LA LETTRE
                  IF(PASSE .EQ. 1) CALL TTTTONE(IUV, 4, 1)
  
*                 REMETTRE LE PATRON COURANT
                  NPAT          = NPATP
                  CALL TTTPCAR(CHAR, IFNTCAS, INTCAR+AUGINT, COLCAR(1),
     +                         IC1, IC2,PASSE)
               ELSE
                  IF( ICLRFLG ) THEN
                  NPATP         = NPAT
                  NPAT          = 129
                  HPALET(1,129) = 1
                  HPALET(2,129) = COLCAR(0)
                  HPALET(3,129) = FOND
                  IF(PASSE .EQ. 1) CALL TTTTONE(IUV, 4, -1)
                  NPAT          = NPATP
                  CALL TTTPCAR(CHAR, IFNTCAS, INTCAR, COLCAR(MODCAR),
     +                         IC1, IC2, PASSE)
               ELSE
#endif
                  CALL TTTPCAR(CHAR, IFNTCAS, INTCAR, COLCAR(MODCAR),
     +                         IC1, IC2, PASSE)
#if defined (RASTER)
                  ENDIF
               ENDIF
            ENDIF
#endif
            IF(IENHANC .NE. 0) THEN
               IF(IENHANC .EQ. 1) THEN
                  IEPAIS = MAX(INTCAR+6*AUGINT,3*INTCAR)
               ELSE
                  IEPAIS = INTCAR + (2*IABS(IENHANC))
               ENDIF
               IF(IENHANC .LT. 0) THEN
                  ICOL2  = COLCAR( MODCAR )
                  ICOL3  = COLLIN( 0 )
               ELSE
                  ICOL2  = COLLIN( 0 )
                  ICOL3  = COLCAR( MODCAR )
               ENDIF
               IF(PASSE .EQ. 2) THEN
                  CALL TTTPCAR(CHAR, IFNTCAS, IEPAIS, ICOL2, IC1, IC2,3)
               ELSE
                  CALL TTTPCAR(CHAR, IFNTCAS, INTCAR, ICOL3, IC1, IC2,
     +                         PASSE)
               ENDIF
            ENDIF

            IF((IORN.NE.0) .OR. FIXFLG) CALL TTTDRAW(RC1+(SPX+.5),
     +                                            RC2+(SPY+.5), 0, 0, 0)

            IF( IAVFONT ) THEN
*              ON REMET LA FONTE FIXE
               IFNTCAS = IFPREC
               IAVFONT = .FALSE.
            ENDIF
 300        GOTO 70
         ENDIF
 310     CONTINUE

*     SI L'ENCADREMENT N'A PAS ETE FERME, ON FORCE LA FERMETURE.
      IF( IBXFLG ) CALL TTTDSBX(IDBOXX, IDBOXY, CURX, CURY, INTCAR,
     +                          COLCAR(MODCAR), FONTNB)

*     SOUS-LIGNAGE
      IF( IUNDFLG ) CALL TTTUNDR(IUNL1X, IUNL1Y, CURX, CURY, INTCAR,
     +                           COLCAR(MODCAR), FONTNB)
      IFNTCAS = STKFONT
      INTCAR  = STKINT
      X1      = STKX1
      Y1      = STKY1
      X2      = STKX2
      Y2      = STKY2
      SPX     = STKSPX
      SPY     = STKSPY
      IBXFLG  = STKBOX
      IUNDFLG = STKUNDR
      IREVFLG = STKREV
      ICLRFLG = STKCLR
      CFILL   = STKFILL
      IENHANC = STKENH
      IAVFONT = STKAVF

      RETURN
 320  FORMAT(20X,'****OPTION OF TTTPWRY UNKNOWN=',I6,'IGNORED')

      END
