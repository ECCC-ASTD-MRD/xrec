#include "aaaamac.cdk"
*** PROGRAM TRAMES - PROGRAMME DE TRADUCTION POUR UNITES RASTER
      PROGRAM TRAMES
#include "impnone.cdk"
*
*REVISION
*    - V1.14 - M. LEPINE -  RECHARGEMENT COS 1.14 / CFT 1.14  (MAI 86)
*    - V1.15 - M. LEPINE -  BUG FIX DU TDIST POUR TEKJET
*    - V1.16 - M. LEPINE /  I. PITCHEN - VERSION CYBER A 32 BITS
*              M. LEPINE -  AJOUT DU PARAMETRE ST SUR LE ROUTE
*    - V1.17 - M. LEPINE -  AJOUT DES DEVICES "SUN"
*    - V1.18 - M. LEPINE -  AJOUT DES DEVICES TANDEM ET DIFAX
*    - V1.19 - M. LEPINE -  DEBUG DE TTTLINE POUR UN POINT DE LARG > 1
*    - V1.20 - M. LEPINE -  AJOUT DU PARAMETRE TNAM POUR DEVICE LINK
*    - V1.21 - M. LEPINE -  REORGANISATION DES PUSH, POP (OPTIONS,PARAM)
*    - V1.22 - M. LEPINE -  REORGANISATION DE LA COMPTABILITE (CRAY/CDC)
*                        -  REMPLACEMENT DU DEVICE TECMAR PAR IBMEGA
*                        -  AJOUT DU PARAMETRE NOROUTE
*    - V1.23 - M. LEPINE -  BUG FIX DE TTTLKID POUR PLUSIEURS FRAMES
*    - V1.24 - M. LEPINE -  CHANGEMENTS DANS HAFTPAL POUR FF ET BB
*    - V1.25 - M. LEPINE -  AJOUT DU PARAMETRE COLORS
*    - V1.26 - M. LEPINE -  REORGANISATION DU FRAMEID  (AVEC TNAME)
*    - V1.27 - M. LEPINE -  REORGANISATION DE TTTPRE2 ET TTTNDR2 (NOV 86)
*    - V1.28 - M. LEPINE -  BUG FIX DE TTTCNTR POUR FONTNB (CNTRG=12) (NOV 86)
*    - V1.29 - M. LEPINE -  BUG FIX DE COMP (CYBER) EN MODE DEDOUBLE (DEC 86)
*    - V1.30 - M. LEPINE -  BUG FIX DE TTTFILL EN MODE EFFACEMENT (DEC 86)
*    - V1.31 - M. LEPINE -  BUG FIX DE TTTFILL EN MODE EFFACEMENT (DEC 86)
*    - V1.32 - M. LEPINE -  AJUSTEMENT DES LIGNES BLANCHES DANS DRAW (DEC 86)
*    - V1.33 - M. LEPINE -  VERSION XMP (DEC 86)
*    - V1.34 - M. LEPINE -  BUG FIX DE TTTFINI POUR UN=CENTRAL (JAN 87)
*    - V1.35 - M. LEPINE -  MODIFICATION DU ACCESS DANS ACCOUNT (JAN 87)
*    - V1.36 - M. LEPINE -  AJOUT DU PARAMETRE REVERSE (JAN 87)
*    - V1.37 - M. LEPINE -  BUG FIX DE REVERSE AVEC TOPCLIP (JAN 87)
*    - V1.38 - M. LEPINE -  REMPLACEMENT DU DEVICE TECMAR2 PAR P3102
*    - V1.39 - M. LEPINE -  AJOUT DU PARAMETRE FONTES
*                        -  MODIFICATION A TTTPWI1 (CAS 0, MAX(1,L..))
*    - V1.40 - M. LEPINE -  : TRANSFORME EN BLANC (TTPUTID, ASC2DPC <->CYBER)
*    - V1.41 - M. LEPINE -  AJOUT DU DEVICE CALCOMP
*    - V1.42 - M. LEPINE -  CHANGEMENTS DANS LE FICHIER DE PATRONS
*    - V1.43 - M. LEPINE -  AJOUT DU DEVICE IBMVGA
*    - V1.44 - M. LEPINE -  AJOUT DU FORMAT RRBX (RAW RASTER BINARY EXCHANGE)
*                           ET DU FORMAT RRUX (RAW RASTER UNIVERSAL EXCHANGE)
*    - V1.45 - M. LEPINE -  AJOUT DU PARAMETRE TABPEN (POUR INTENSITE)
*    - V1.46 - M. LEPINE -  REORGANISATION SUR LE CYBER 930
*    - V1.47 - M. LEPINE -  BUGFIX DE IIPAT = 0 DANS TTTFILL
*    - V1.48 - M. LEPINE -  AJOUT DU DEVICE LASER ET ROUTAGE AUTOMATIQUE
*                           POUR LES DEVICES LASER ET CALCOMP
*    - V1.49 - M. LEPINE -  AJOUT DU PARAMETRE VOFFSET (OFFSET VERTICAL)
*    - V1.50 - M. LEPINE -  TRADUCTEUR CYBER 180 (NOS/VE)
*    - V1.51 - M. LEPINE -  DEPAQUETAGE DES DEFINITIONS DES CARACTERES
*    - V1.52 - M. LEPINE -  TRADUCTEUR SUN
*    - V2.00 - M. LEPINE -  GENERATION DE TRAMES (CRAY, C170, C180, SUN)
*    - V2.01 - M. LEPINE -  MODIFICATION DE WRITRAW ET AJOUT DU PARAM. FORMAT
*                           ET DU PARAMETRE SEGF
*    - V2.02 - M. LEPINE -  BUG FIX DE TTTSKFR POUR SUN ET C180
*    - V2.03 - M. LEPINE -  MODIFICATION A TTTHAFT POUR PRECISION SUR LE SUN
*    - V2.04 - M. LEPINE -  SAUVER LES OPTIONS CORRECTEMENT (TTTPWRY TTTHAFT)
*    - V2.05 - M. LEPINE -  REINITIALISER CORRECTEMENT (TTTBPR TTTSEGM)
*    - V2.06 - M. LEPINE -  CHANGEMENT DE FORMAT D'IMPRESSION
*    - V2.07 - M. LEPINE -  RESET DU WINDOW (TTTWNDW)
*    - V2.08 - M. LEPINE -  EMPECHER LE CHANGEMENT DE FONTE (TTTCNTR)
*    - V2.09 - M. LEPINE -  MODIFICATION A TTTSKFR (SUN, CAUSE WAIO SUN)
*                           BUGFIX TTTCADR (SUN)
*    - V2.10 - M. LEPINE -  NOUVEAU COMPRESSEUR (ASCII)
*    - V2.11 - M. LEPINE -  TRAITER FRAMEID (POUR C180)
*    - V2.12 - M. LEPINE -  VERSION POUR LES CYBERS 910
*    - V2.13 - M. LEPINE -  VERSION POUR LES CYBERS 910 (MODE NATIF)
*    - V2.14 - M. LEPINE -  TRAITER JUSQU'A 256 COULEURS
*                           AJOUT DU PARAMETRE PALETTE
*    - V2.15 - M. LEPINE -  AJOUT DU PARAMETRE ANIM POUR ANIMATION EN MODE
*                           NATIF CYBER 910
*    - V2.16 - M. LEPINE -  SELECTION DE FRAME EN MODE NATIF
*    - V2.17 - M. LEPINE -  INITIALISATION DE MESSAGE (BUG KMWA)
*    - V2.18 - M. LEPINE -  BUF FIX DE TTTDRAW (POINT) EN MODE NATIF 910
*    - V2.19 - M. LEPINE -  GARDER LE ZOOM SUR UN NEXT FRAME (MODE NATIF)
*                           ET AJOUT DU MODE RECUL (MODE NATIF)
*    - V2.20 - M. LEPINE -  UTILISATION DE LA VARIABLE D'ENVIRONNEMENT ARMNLIB
*                           POUR LES NOMS DES FICHIERS DE DONNEES (MODE NATIF)
*    - V2.21 - M. LEPINE -  CODE SOURCE SUR UNIX AVEC CPP ET RCS
*    - V2.22 - M. LEPINE -  BUG FIX POUR OUVERTURE UNF AVEC FORMAT RRBX
*    - V2.23 - M. LEPINE -  AJOUT TTTFMID MODE 910
*    - V2.24 - M. LEPINE -  MODIFICATIONS POUR L'UTILISATION DE WGL
*    - V2.25 - M. LEPINE -  BUG FIX LONGUEUR DES CLES POUR TRAMES
*    - V2.26 - M. LEPINE -  OUVERTURE DU FICHIER METACOD EN MODE OLD
*    - V2.27 - M. LEPINE -  MODIFICATION AU HEADER POUR RRBX
*    - V2.28 - M. LEPINE -  MODIFICATION A LCP ET DEVW POUR AMENER LE DEVICE
*                           V200 A 21 POUCES (DEC 91)
*    - V2.29 - M. LEPINE -  MODIFICATION POUR BUG AVEC DOUBLE PRECISION
*                           TTTTONE - TTTFILL - TTTHAFT
*    - V2.30 - M. LEPINE -  CHANGER GETLOG POUR GETENV (TTTBAN)
*    - V2.31 - M. LEPINE -  REMETTRE l'ACCES A PERMUT
*    - V2.32 - M. LEPINE -  OPTION REVERSE POUR RRUX RRBX
*    - V3.0  - Y. BOURASSA  Fev  95 RATFOR @ FTN77
*    - V3.01 - M. Lepine    Oct 1998 - Rechargement avec librmn32stack_LR.a
*    - V3.02 - M. Lepine    Jan 1999 - Bug fix common params + cmpres2
*    - V3.03 - M. Lepine    Fev 1999 - Bug fix writraw format sunrst
*    - V3.04 - M. Lepine    Fev 1999 - Mods little endian pour Linux
*    - V3.05 - M. Lepine    Mar 1999 - Mods fnom filename fichier scratch
*    - V3.07 - M. Lepine    Jan 2005 - Reload plus recent pour Debian
*    - V3.08 - Y. Chartier  Nov 2007 - Reload xmetaview, motif
*    -         M. Valin     Jul 2013 - Get rid of memoirh
*    - V3.10 - M. Lepine    Dec 2014 - Reload avec librmn_015.1
*    - V3.11 - M. Lepine    Fev 2015 - Reload avec librmn_015.2
*    - V3.12 - M. Valin     Avr 2015 - version.inc pour les versions

*
*LANGUAGE   - FTN77
*
* CODE SOURCE RCS Revision # $Revision: 27 $
*
*LISTE DES FICHIERS INTERNES
*   UNITE      NOM              DESCRIPTION
*     8        METACOD    -->   FICHIER D'ENTREE
*     9        SEGFILE    -->   FICHIER DE SEGMENTS
*    11        PALETTE    -->   FICHIER DE PALETTE DES COULEURS
*    19        RASTDMP    -->   FICHIER DE SORTIE (RASTER DUMP ,TAPE)
*    22        LNKRAST    -->   FICHIER DE SORTIE (LINK,DFI,DIFAX,TANDEM)
*    33        IBM4RAS    -->   FICHIER DE SORTIE (POUR RID)
*    44        RRUX,RRBX  -->   FICHIER DE SORTIE (RAW RASTER)
*    55        CARFNTS    -->   FICHIER DE DEFINITION DES FONTES
*    66        PATRNS     -->   FICHIER DE DEFINITION DES PATRONS
*    77        STCKOOO    -->   FICHIER INTERNE (STOCKAGE DES OPTIONS)
*    88        STCKPPP    -->   FICHIER INTERNE (STOCKAGE DES OPTIONS)
*    99        RAZZ2      -->   FICHIER DE SORTIE (RASTER KMW)
*
*
*MODULES
      EXTERNAL TTTVOPN, TTTPATI, TTTCARI, TTTINIT, WACLOS, CCARD,
     +         TTTZOPN, TTTCADR, TTPUTID, TTTCOMP, TTTRIN, FCLOS,
     +         TTTFINI, TTTNDR3, TTTLKID, TTTMAPC, WAOPEN, QQEXIT,
     +         WRITRAW, ACCOUNT, FRETOUR, TTTSKFR, TTTBAN, FNOM,
     +         TTRRINI, TTTPRCS, SECOND,  EOF
*
**
      character(len=8) :: TRAMES_VERSION, XMETAVIEW_VERSION
#include "params.cdk"
#include "docpar.cdk"
#include "mapvar.cdk"
#include "segvar.cdk"
#include "argum.cdk"
#include "palpat.cdk"
#include "mesgid.cdk"
#include "inscont.cdk"
#include "ivar.cdk"
#include "cpress.cdk"
#include "endian.cdk"
      DATA ENDIAN /1/
      INTEGER TLCP(24), TSIZE(24), TVSPACE(24), STRFRM, SIZEY, ICGR,
     +        TNCP(24), TDEVW(24), TOFFSET(24), ESPACE, SIZEX, DIST,
     +        MSGID(8), ERRCODE,   NFRAMES,  I, FRAMES, ICONT, FNOM,
     +        BUFID(7), VOFFSET,   INITSPA, GR, STATUS, EOF,
     +        FRSTOFF,  INITVSP
      REAL    TDIST(24), CC1, T1, T2, TA1, TA2, UUNIT, VUNIT, SECOND,
     +        TAJUSTX(24)
      integer hpier, hpabt
      CHARACTER * 128 CLE(64) , DEF1(64), VAL(64), DN
      CHARACTER * 8   TABDEV(24), TFORMSC(24), CDATE, TABDN(24), USAGER,
     +                TABTID(24), TPROTOC(24), CTIME, PROTOCL, UN, JOBN    
      CHARACTER * 2   FC, MF
      LOGICAL         FOUND, TSEPARE(24), NOROUTE, VERTOFF, TOPCLIP
*
*   TDIST(DEVICE) = (NOMBRE DE POINTS EN Y / HAUTEUR EN Y (EN POUCE)) 
*                   (NOMBRE DE POINTS EN X / LARGEUR EN X (EN POUCE))
*
*   EXEMPLE : TDIST(IBMVGA) = (480 /7) / (640 / 10) = 1.0714
*
*
      DATA CLE/
     % 'DX',       'DY',     'HX',      'HY',       'SIZE',   'SPACE',
     % 'VSPACE',   'OFFSET', 'DEVICE',  'FRAMES',   'GR',     'MC',
     % 'ORN',      'CADRE',  'NCP',     'DIST',     'FOND',   'DN',
     % 'I',        'SIZEY',  'FC',      'UN',       'MF',     'TOPCLIP',
     % 'TNAME',    'NOROUTE', 17*'COLORS','REVERSE', 10*'FONTES',
     % 5*'TABPEN', 'VOFFSET','FORMAT', 'SEGF',     'PALETTE', 'ANIM'/
*
      DATA DEF1/
     % '0',        '0',      '0',       '0',        '-1',     '-1',
     % '-1',       '-1',     'V80',     '000999',   '0',      'METACOD',
     % 'STD',      'OUI',    '-1',      '-1',       '0',      ' ',
     % '$IN',      '-1',     'XX',      'DEFAUT',   'MC',     'OUI',
     % ' ',        'OUI',
     % '0','1','2','3','4','5','6','7','8','9','10','11','12','13','14',
     % '15','16',
     % 'OUI',
     % '0','1','2','3','4','5','6','7','8','9',
     % '1','2','3','4','5',
     % '0',        ' ',      'SEGFILE', ' ', 'OUI'/
*
      DATA VAL/
     % '0',        '0',      '0',       '0',        '-1',     '-1',
     % '-1',       '-1',     'V80',     '000999',   '0',      'METACOD',
     % 'STD',      'NON',    '-1',      '-1',       '0',      ' ',
     % '-1',       '-1',     'XX',      'DEFAUT',   'MC',     'NON',
     % ' ',        'NON',
     % '0','1','2','3','4','5','6','7','8','9','10','11','12','13','14',
     % '15','16',
     % 'NON',
     % '0','1','2','3','4','5','6','7','8','9',
     % '1','2','3','4','5',
     % '0',        ' ',      'SEGFILE', ' ', 'NON'/
*
*            ------------------------------------------------------
      DATA TABDEV  /
     +        'V80',     'V100',   'V200',   'DFI',  'LINK',  'V80T',
     +        'P3102',   'CGP220', 'IBMEGA', 'IBM2', 'IBM4',  'IBMK',
     +        'TEKJET',  'P3100',  'TAPE',   'SUN6', 'SUN12', 'SUN24',
     +        'TANDEM',  'DIFAX',  'CALCOMP','IBMVGA','LASER',' '   /
*            ------------------------------------------------------
      DATA TLCP    /
     +         33,      33,       66,       33,     33,      33,
     +         10,      10,       10,       10,     5,       5,
     +         16,      10,       33,       8,      16,      32,
     +         33,      33,       32,       10,     38,      0    /
*            ------------------------------------------------------
      DATA TNCP    /
     +         1,       1,        1,        1,      1,       1,
     +         1,       3,        4,        1,      2,       2,
     +         3,       1,        1,        3,      3,       3,
     +         1,       1,        3,        4,      1,       0    /
*            ------------------------------------------------------
      DATA TDEVW   /
     +         2039,    2103,     4223,     2112,   2112,    2039,
     +         639,     639,      639,      639,    319,     319,
     +         1023,    639,      2103,     511,    1023,    2047,
     +         1199,    1759,     1999,     639,    2399,    0    /
*            ------------------------------------------------------
      DATA TDIST   /
     +         1.019,    1.0,      1.0,   1.0435,    1.0,     1.0,
     +         .8928,    1.0,    .7813,    .4464,  .8928,   .8928,
     +          1.0,   .7254,     1.0,     1.0,      1.0,     1.0,
     +         1.4118,   1.0,     1.0,    1.0714,    1.0,     0    /
*            ------------------------------------------------------
      DATA TSIZE   /
     +         800,     800,      800,      800,    800,     800,
     +         700,     753,      700,      700,    700,     700,
     +         1024,    700,      800,      600,    1200,    2400,
     +         1764,    1833,     800,      700,    800,     0    /
*            ------------------------------------------------------
      DATA TOFFSET /
     +         100,     100,      100,      100,    100,     100,
     +         150,     0,        150,      150,    150,     150,
     +         0,       150,      100,      0,      0,       0,
     +         0,       0,        100,      150,    0,       0    /
*            ------------------------------------------------------
      DATA TVSPACE /
     +         55,      0,        0,        0,      0,       55,
     +         0,       2,        0,        0,      0,       0,
     +         0,       0,        0,        0,      0,       0,
     +         0,       0,        0,        0,      200,     0    /
*            ------------------------------------------------------
      DATA TAJUSTX /
     +         2.0,     1.0,      2.0,      0.92,   1.0,     2.0,
     +         0.64,    0.85,     0.64,     0.64,   0.32,    0.32,
     +         1.2,     0.64,     1.0,      .8533,  .8533,   .8533,
     +         0.68,    0.96,     2.0,      0.64,   3.0,     0    /
*            ------------------------------------------------------
      DATA TFORMSC /
     +        'AA',     'AA',     'AA',     'AA',   'AA',    'AA',
     +        'AA',     'AC',     'AA',     'AA',   'AA',    'AA',
     +        'AA',     'AA',     'AA',     'AC',   'AC',    'AC',
     +        'AA',     'AA',     'AA',     'AA',   'AA',    ' '    /
*            ------------------------------------------------------
      DATA TABTID /
     +        'ARMNV80','CENTRAL','CENTRAL','USER', 'USER', 'ASPOAAY',
     +        'USER',   'ARMNV80','USER',   'USER', 'USER',  'USER',
     +        'USER',  'USER',  'USER', 'NOROUTE', 'NOROUTE', 'NOROUTE',
     +        'USER',   'USER', 'FTPV80', 'USER',   'FTPV80',' '    /
*            ------------------------------------------------------
      DATA TSEPARE /
     +        .FALSE., .FALSE., .FALSE., .FALSE., .FALSE., .FALSE.,
     +        .TRUE.,  .FALSE., .TRUE.,  .TRUE.,  .TRUE.,  .FALSE.,
     +        .FALSE., .TRUE.,  .FALSE., .TRUE.,  .TRUE.,  .TRUE.,
     +        .FALSE., .FALSE., .FALSE., .TRUE.,  .FALSE., .FALSE./
*            ------------------------------------------------------
      DATA TABDN  /
     +      'RAZZ2',  'RAZZ2',  'RAZZ2',  'LNKRAST','LNKRAST','RAZZ2',
     +      'RAZZ2',  'RAZZ2',  'RAZZ2',  'RAZZ2',  'RAZZ2',  'IBM4RAS',
     +      'RAZZ2',  'RAZZ2',  'RASTDMP','RAZZ2',  'RAZZ2',  'RAZZ2',
     +      'LNKRAST','LNKRAST','RAZZ2',  'RAZZ2',  'RAZZ2',  ' '/
*            ------------------------------------------------------
      DATA TPROTOC /
     +        'KMW',   'KMW',    'KMW',   'LINK',    'LINK',   'KMW',
     +        'KMW',   'KMW',    'KMW',   'KMW',     'KMW',    'OLDRID',
     +        'KMW',   'KMW',    'RAW',   'KMW',     'KMW',    'KMW',
     +        'LINK',  'LINK',   'KMW',   'KMW',     'KMW',     ' '/
*            ------------------------------------------------------
*
      DATA MESSAGE /' '/
#include "mapping.cdk"
#include "version.inc"
      T1 = SECOND()
      CALL CCARD(CLE, DEF1, VAL, 64, -1)
      ERRCODE = FNOM(MCUNIT, VAL(12), 'RND,OLD', 0)
      IF(ERRCODE .NE. 0) STOP
      CALL WAOPEN( MCUNIT )
      ERRCODE = FNOM(SEGUNIT, VAL(62), 'RND', 0)
      ERRCODE = FNOM(77, 'STCKOOO', 'D77+UNF', 100)
      ERRCODE = FNOM(88, 'STCKPPP', 'D77+UNF', 150)
      DN      = VAL(18)
      PROTOCL = VAL(61)
      READ(VAL(1),  9) DX
      READ(VAL(2),  9) DY
      READ(VAL(3),  9) HX
      READ(VAL(4),  9) HY
      READ(VAL(5),  9) SIZEX
      READ(VAL(6),  9) SPACE
      READ(VAL(7),  9) VSPACE
      READ(VAL(8),  9) OFFSET
      READ(VAL(10), 9) FRAMES
      READ(VAL(11), 9) GR
      READ(VAL(15), 9) NCP
      READ(VAL(16), 9) DIST
      READ(VAL(17), 9) FOND
      READ(VAL(20), 9) SIZEY
      READ(VAL(60), 9) VOFFSET
      FOND = MAX(0,MIN(FOND,15))
 9    FORMAT(I8)

      IF(VAL(24) .EQ. 'OUI') THEN
         TOPCLIP = .TRUE.
      ELSE
         TOPCLIP = .FALSE.
      ENDIF

      IF(VAL(26) .EQ. 'OUI') THEN
         NOROUTE  = .TRUE.
      ELSE
         NOROUTE = .FALSE.
      ENDIF

      DEVICE = 1
      FOUND  = .FALSE.
 10   IF((DEVICE .LE. 24) .AND. (.NOT. FOUND)) THEN
         IF (VAL(9) .EQ. TABDEV(DEVICE)) THEN
            FOUND = .TRUE.
         ELSE
            DEVICE = DEVICE + 1
         ENDIF
         GOTO 10
      ENDIF
      IF(.NOT. FOUND) THEN
         WRITE( 6,*) ' TRAMES   DEVICE NON RECONNU'
         CALL QQEXIT(1)
      ENDIF
      IF(DN      .EQ. ' ') DN      = TABDN  (DEVICE)
      IF(PROTOCL .EQ. ' ') PROTOCL = TPROTOC(DEVICE)
      ASKI = .FALSE.
      IF(PROTOCL .EQ. 'KMWA') THEN
          ASKI    = .TRUE.
          PROTOCL = 'KMW'
      ENDIF
      IF(PROTOCL .EQ. 'KMW')    ERRCODE = FNOM(99,DN,'SEQ',0)
      IF(PROTOCL .EQ. 'RAW')    ERRCODE = FNOM(19,DN,'RND',0)
      IF(PROTOCL .EQ. 'OLDRID') ERRCODE = FNOM(33,DN,'SEQ',0)
      IF(PROTOCL .EQ. 'LINK')   ERRCODE = FNOM(22,DN,'SEQ',0)
      IF(PROTOCL .EQ. 'RRUX')   ERRCODE = FNOM(44,DN,'SEQ+FMT',0)
      IF(PROTOCL .EQ. 'RRBX')   ERRCODE = FNOM(44,DN,'SEQ+UNF',0)
      IF(PROTOCL .EQ. 'SUNRST') ERRCODE = FNOM(44,DN,'RND',0)
      STRFRM  = FRAMES/1000
      NFRAMES = MOD(FRAMES,1000)
      IF(VAL(13) .EQ. 'STD') THEN
         IROT = 0
      ELSE
         IROT = 90
      ENDIF

      MODEP   = 100 * TAJUSTX(DEVICE)
      LCP     = TLCP(DEVICE)
      LCP     = 64/BITUSED * LCP
      DEVWIDT = TDEVW(DEVICE)
      IF(NCP .EQ. -1) NCP = TNCP(DEVICE)
      WRAST   = LCP * NCP
      IF(SIZEX .EQ. -1) SIZEX = TSIZE(DEVICE)
      IF(SIZEY .EQ. -1) SIZEY = SIZEX
      IF(DIST  .EQ. -1) THEN
        DISTOR = TDIST(DEVICE)
      ELSE
        DISTOR = DIST/1000.
      ENDIF
      IF(OFFSET .EQ. -1) OFFSET = TOFFSET(DEVICE)
      IF(VSPACE .EQ. -1) VSPACE = TVSPACE(DEVICE)
      IF(HX .EQ. 0)      HX = SIZEX - DX
      IF(HY .EQ. 0)      HY = SIZEY - DY
      IF(SPACE .EQ. -1)  SPACE = HX + 100
      FRSTOFF = OFFSET
      INITSPA = SPACE
      INITVSP = VSPACE
      SPACE   = NINT(SPACE * TAJUSTX(DEVICE))
      VSPACE  = NINT(VSPACE * DISTOR * TAJUSTX(DEVICE))
      IF(GR .EQ. 0) GR = MAX(1,(DEVWIDT-OFFSET)/SPACE)
      ALPHA  = SIZEX / 32767.0
      ALPHAY = SIZEY / 32767.0
      UUNIT  = TAJUSTX(DEVICE)
      VUNIT  = TAJUSTX(DEVICE) * DISTOR

      IF(IROT .EQ. 0) THEN
         HU  = NINT(HX * UUNIT) - 1
         HV  = NINT(HY * VUNIT) - 1
         A1  = ALPHA * HU / HX
         B1  = 0
         CC1 = OFFSET * UUNIT - FLOAT(DX * HU) / HX
         A2  = 0
         B2  = - ALPHAY * HV / HY
         C2  = (HY+DY) * HV / HY
      ELSE
         HU  = NINT(HY * UUNIT) - 1
         HV  = NINT(HX * VUNIT) - 1
         A1  = 0
         B1  = ALPHAY * HU / HY
         CC1 = OFFSET * UUNIT - FLOAT(DY * HU) / HY
         A2  = ALPHA * HV / HX
         B2  = 0
         C2  = -DX * HV / HX
      ENDIF

      DO 20 I = 0,15
         IF(VAL(27+I) .NE. '16') THEN
            READ(VAL(27+I),'(I2)') TABCOL(I)
         ELSE
            TABCOL(I) = -1
         ENDIF
         TABCOL(I) = MOD(TABCOL(I),2**NCP)
 20      CONTINUE

      TABCOL(16) = -1
      IF(DEVICE .EQ. CALCOMP) THEN
         DO 40 I = 0,16
            TABCOL(I) = COLCOMP(TABCOL(I))
 40         CONTINUE
      ENDIF
      DO 50 I = 0,9
         READ(VAL(45+I),'(I2)') TABFONT(I)
 50      CONTINUE
      DO 60 I = 1,5
         READ(VAL(54+I),'(I2)') TABPEN(I)
 60      CONTINUE
      DO 70 I = 1,128
         HPALET(1,I) = I
         HPALET(2,I) = 1
         HPALET(3,I) = 16
 70      CONTINUE

*     VSPACE  = MIN(VSPACE, HV)
      VOFFSET = NINT(VOFFSET * DISTOR * TAJUSTX(DEVICE))
*     VOFFSET = MIN(VOFFSET, HV)
      VERTOFF = .TRUE.
      OFFSET  = NINT(OFFSET * TAJUSTX(DEVICE))
      INITOFF = OFFSET
      ESPACE  = MAX(HV+1, VSPACE, VOFFSET)
      IF(DEVICE .EQ. CALCOMP) ESPACE = MAX(ESPACE, 1601)
!!if !defined (UNIX) 
      PTIV    = 1
!!endif
!!if !defined (Alpha) 
!      CALL MEMOIRH(IVAR, PTIV, ESPACE*LCP*NCP)
!      CALL TTTVOPN(LCP, NCP, ESPACE, TABCOL(FOND), IVAR(PTIV))
!!else
!      CALL hpalloc(PIVAR, ESPACE*LCP*NCP, hpier, hpabt)
      allocate(ivar(ESPACE*LCP*NCP))
      CALL TTTVOPN(LCP, NCP, ESPACE, TABCOL(FOND), IVAR)
!!endif
      WRITE(IPUNIT,610) TRIM(TRAMES_VERSION),TABDEV( DEVICE )
      CALL TTTPATI
      CALL TTTCARI
      WRITE(IPUNIT,620) REAL(SIZEX)/100.,   REAL(INITSPA)/100., 
     +                  REAL(INITVSP)/100., REAL(FRSTOFF)/100.,
     +                  REAL(SIZEY)/100. ,  IROT
      WRITE(IPUNIT,630) REAL(HX)/100.,      REAL(HY)/100.,
     +                  REAL(DX)/100.,      REAL(DY)/100.
      CALL TTTRIN
      CALL TTTMAPC(11, VAL(63))
      ICGR   = 0
      STATUS = OK
      CALL TTTSKFR(STRFRM, STATUS)
      CALL TTTBAN(USAGER, JOBN, CDATE, CTIME, VAL(25), PROTOCL)
      IF((PROTOCL.EQ.'RRUX') .OR. (PROTOCL.EQ.'RRBX'))
     +   CALL TTRRINI(LCP*BITMOT, HV+1, LCP, NCP, USAGER, JOBN, CDATE,
     +                CTIME, PROTOCL)
      IF((DEVICE.EQ.IBMK) .AND. (VAL(19).NE.'-1'))
     +   ERRCODE = FNOM(5, VAL(19), 'SEQ', 0)
 80   IF((STATUS.NE.MARKEOI) .AND. (ICGR.LT.NFRAMES)) THEN
         ICONT  = 0
         OFFSET = INITOFF
         C1     = CC1
         CALL TTTZOPN(LCP, NCP, ESPACE, TABCOL(FOND), IVAR(PTIV))
         IF( VERTOFF ) THEN
            CALL TTTCOMP(IVAR(PTIV), VOFFSET, TABCOL(FOND) .EQ. 0,
     +                   TOPCLIP, VAL(44).EQ.'OUI')
            VERTOFF = .FALSE.
         ENDIF

 90      IF((ICONT.LT.GR) .AND. (ICGR.LT.NFRAMES)) THEN
            ICONT  = ICONT + 1
            STATUS = 0
            CALL TTTINIT
            COMPT4B = 0
            COMPTMB = 0
            IF(VAL(14) .EQ. 'OUI') CALL TTTCADR
            IF((DEVICE.EQ.IBMK) .AND. (VAL(19).NE.'-1')) THEN
               MESSAGE = ' '
               READ(5, '(A80)', END=100, ERR=100) MESSAGE
            ENDIF
            GOTO 110
 100        I   = EOF(5)

 110        TA1 = SECOND()
            CALL TTTPRCS(STATUS)
            TA2 = SECOND()
            IF(STATUS .EQ. MARKEOI) GOTO 120
            OFFSET = OFFSET + SPACE
            ICGR   = ICGR + 1
            C1     = C1 + SPACE
            WRITE(IPUNIT, 600) ICGR,    TA2-TA1
            WRITE(IPUNIT, 605) COMPT4B, COMPTMB
            GOTO 90
         ENDIF
 120     IF((STATUS.NE.MARKEOI) .OR. (ICONT.GT.1)) THEN
            TA1 = SECOND()
            IF(TSEPARE(DEVICE) .AND. (PROTOCL .EQ. 'KMW'))
     +         CALL TTTPRE3
            IF((DEVICE.EQ.LINK) .OR. (DEVICE.EQ.TANDEM) .OR.
     +         (DEVICE.EQ.DFI)  .OR. (DEVICE.EQ.DIFAX))
     +         CALL TTTLKID(VAL(25))
            IF(TSEPARE(DEVICE) .AND. (PROTOCL.EQ.'KMW')) THEN
               WRITE(MESSAGE(56:64),'(2(I4.4), I1)')
     +               LCP*BITUSED,HV+1,NCP
               CALL TTPUTID(MSGID, BUFID)
            ENDIF
            IF((PROTOCL.EQ.'KMW') .OR. (PROTOCL.EQ.'LINK') .OR.
     +         (PROTOCL.EQ.'OLDRID')) THEN
               CALL TTTCOMP(IVAR(PTIV), HV+1, .FALSE., TOPCLIP, 
     +                      VAL(44).EQ.'OUI')
               CALL TTTZOPN(LCP, NCP, VSPACE, TABCOL(FOND), IVAR(PTIV))
               CALL TTTCOMP(IVAR(PTIV), VSPACE, TABCOL(FOND).EQ.0, 
     +                      TOPCLIP, VAL(44).EQ.'OUI')
               TA2 = SECOND()
               WRITE(IPUNIT, 670) TA2-TA1
               IF(TSEPARE(DEVICE) .AND. (PROTOCL.EQ.'KMW')) THEN
                  CALL TTTNDR3
                  VERTOFF = .TRUE.
               ENDIF
            ELSE
               CALL WRITRAW(HV+1, PROTOCL, IVAR(PTIV), VAL(44).EQ.'OUI')
            ENDIF
         ENDIF
         GO TO 80
      ENDIF

      IF((DEVICE.EQ.IBMK) .AND. (VAL(19).NE.'-1')) CALL FCLOS(5)
      FC = VAL(21)
      IF(FC .EQ. 'XX')     FC = TFORMSC(DEVICE)
      UN = VAL(22)
      IF(UN .EQ. 'DEFAUT') UN = TABTID(DEVICE)
      IF(UN .EQ. 'USER')   UN = USAGER
      MF = VAL(23)
      CALL TTTFINI(FC, UN, MF, NOROUTE, JOBN, PROTOCL)
      T2 = SECOND()
      COMPTAB(2) = ICGR
      COMPTAB(3) = ((ICGR + GR-1) / GR*(HV + VSPACE)) / MODEP
      IF(DEVICE .EQ. V100) COMPTAB(3) = COMPTAB(3)*2
      COMPTAB(4) = ((T2 - T1)*1000) / 1
      COMPTAB(5) = 1
      CALL ACCOUNT( UN )
      IF(.NOT. SEGCLOS) CALL WACLOS( SEGUNIT )
      CALL WACLOS( MCUNIT )
      CALL FRETOUR( 'STCKPPP' )
      CALL FRETOUR( 'STCKOOO' )
      WRITE(IPUNIT, 660) ICGR
      STOP

 600  FORMAT(//T25, '***  R A S T E R -- GRAPHE NUMERO: ', I3, '   ',
     +       F8.3, ' S.')
 605  FORMAT(/T30, "--- NOMBRE D'INSTRUCTIONS DE 4 BYTES: ", I8,
     +       /T30, "--- NOMBRE D'INSTRUCTIONS MULTIBYTES: ", I8)
 610  FORMAT('1', T10, 'TRAMES - ',A,' -',
     +       '  ', ' TRADUCTION',
     +       ' POUR UNITE ', A8/T10, 71('=')//)
 620  FORMAT(//T10, 'SIZE = ', F6.2, T35, 'SPACE = ', F6.2,T55,
     +       'VSPACE = ', F6.2//T10, 'OFFSET = ', F6.2,T35,
     +       'SIZEY = ', F6.2, T55, 'ORIENTATION = ', I2, ' DEGRE(S)')
 630  FORMAT(//T10, 'HX = ', F6.2, T45, 'HY = ', F6.2//T10, 'DX = ',
     +       F6.2, T45, 'DY = ', F6.2)
 660  FORMAT(//T40, 'FIN D''INFORMATION APRES ', I3, ' GRAPHE(S)')
 670  FORMAT(/T35, 'C O M P R E S S I O N    ', F8.3, '  SECONDE(S)')
      END
      
      character *128 function product_id_tag()
      product_id_tag='$Id: praster.ftn 27 2007-05-15 20:36:47Z armnlib $'
      return
      end
