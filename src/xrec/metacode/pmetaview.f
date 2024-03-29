#include "aaaamac.cdk"
***   PROGRAM METAVIEW - PROGRAMME DE VIZUALISATION POUR LES 910
      PROGRAM METAVIEW
#include "impnone.cdk"
*
*REVISION
*
*    - V2.13 - M. LEPINE -  VERSION POUR LES CYBERS 910 (MODE NATIF)
*    - V2.14 - M. LEPINE -  TRAITER JUSQU'A 256 COULEURS
*                           AJOUT DU PARAMETRE PALETTE
*    - V2.15 - M. LEPINE -  AJOUT DU PARAMETRE ANIM POUR ANIMATION EN MODE
*                           NATIF CYBER 910
*    - V2.16 - M. LEPINE -  SELECTION DE FRAME EN MODE NATIF
*    - V2.17 - M. LEPINE -  INITIALISATION DE MESSAGE (BUG KMWA)
*    - V2.18 - M. LEPINE -  BUF FIX DE TTTDRAW (POINT) EN MODE NATIF (C910)
*    - V2.19 - M. LEPINE -  GARDER LE ZOOM SUR UN NEXT FRAME (MODE NATIF)
*                           ET AJOUT DU MODE RECUL (MODE NATIF)
*    - V2.20 - M. LEPINE -  UTILISATION DE LA VARIABLE D'ENVIRONNEMENT ARMNLIB
*                           POUR LES NOMS DES FICHIERS DE DONNEES (MODE NATIF)
*    - V2.21 - M. LEPINE -  CODE SOURCE SUR UNIX AVEC CPP ET RCS
*    - V2.22 - M. LEPINE -  BUG FIX POUR OUVERTURE UNF AVEC FORMAT RRBX
*    - V2.23 - M. LEPINE -  AJOUT TTTFMID MODE C910
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
*    - v3.0  - Y. BOURASSA  FEV 95 RATFOR @ FTN77
*    - V3.04 - M. Lepine    Fev 1999 - Mods little endian pour Linux
*    - V3.05 - M. Lepine    Jan 2001 - Rechargement pour display 16 bits
*    - V3.06 - M. Lepine    Fev 2002 - Augmentation du tableau menu
*    - V3.07 - M. Lepine    Jan 2005 - Reload plus recent pour Debian
*    - V3.08 - Y. Chartier  Nov 2007 - Reload xmetaview, motif
*    - V3.10 - M. Lepine    Dec 2014 - Reload avec librmn_015.1
*    - V3.11 - M. Lepine    Fev 2015 - Reload avec librmn_015.2
*    - V3.12 - M. Valin     Avr 2015 - version.inc pour les versions
*
*LANGUAGE   - FTN77
*
* CODE SOURCE RCS Revision # $Revision: 27 $
*
*
*LISTE DES FICHIERS INTERNES
*   UNITE      NOM              DESCRIPTION
*     8        METACOD    -->   FICHIER D'ENTREE
*     9        SEGFILE    -->   FICHIER DE SEGMENTS
*    11        PALETTE    -->   FICHIER DE PALETTE DES COULEURS
*    55        CARFNTS    -->   FICHIER DE DEFINITION DES FONTES
*    66        PATRNS     -->   FICHIER DE DEFINITION DES PATRONS
*    77        STCKOOO    -->   FICHIER INTERNE (STOCKAGE DES OPTIONS)
*    88        STCKPPP    -->   FICHIER INTERNE (STOCKAGE DES OPTIONS)
*
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
#include "bpairp.cdk"
#include "com910.cdk"
#include "endian.cdk"
      DATA ENDIAN /1/
*
*MODULES
      EXTERNAL TTTVOPN, TTTZOPN, ACCOUNT, TTTRIN, FRONTB, CLEAR, LOC,
     +         TTTPATI, TTTSKFR, ASC2DSC, TTTBAN, GETSIZ, CCARD, FNOM,
     +         TTTCARI, TTTFINI, TTTLKID, SETPAT, FCLOS, EOF,
     +         TTTINIT, TTTPRE3, FRETOUR, SOURIS, GETBUT, 
     +         TTTPRCS, TTTNDR3, DFLERMS, RECTFI, KEEPAS, COLOR, 
     +         TTTCOMP, WRITRAW, WGLDBF, BACKBU,  
     +         TTRRINI, TTTCFRM, INITDEV, WAOPEN, WACLOS, BLACKLST,
     +         TTTCADR, TTPUTID, WINOPE,  SWAPBU
*
**
      CHARACTER*128 DEF1(64), VAL(64)
      CHARACTER*8   TABDEV(24), TFORMSC(24), CTIME, USAGER,  JOBN,   UN,
     +              TABTID(24), TPROTOC(24), CDATE, PROTOCL, CLE(64), C,
     +              TABDN(24)
      CHARACTER*2   FC, MF
      CHARACTER*7   CJUNK
      CHARACTER*30  MENU(64)
      INTEGER  TLCP(24), TDEVW(24), TOFFSET(24), ISZX, III, ESPACE, LF, 
     +         TNCP(24), TSIZE(24), TVSPACE(24), ISZY, EOF, WINOPE, FF,
     +         MSGID(8), INITSPA, SIZEY0, SIZEX, SGX0, LOC, MINSGY, GR, 
     +         BUFID(7), VOFFSET, BUTTON, SIZEY, SGY0, DX0, MAXSGX, I, 
     +         NFRAMES,  INITVSP, STRFRM, EVENT, SGLX, DY0, MINSGX, 
     +         NBFRAME,  FRSTOFF, SIZEX0, ICONT, SGX1, FNB, ADRDEB, 
     +         NBENTRY,  MAXSGY,  STATUS, SGX2,  FNOM, SGY2, ICGR,   
     +         ERRCODE,  FRAMES,  DIST,   SGY1,  SGLY, POSFRM(999)  
      REAL     TA1, RATIOX, UUNIT, TAJUSTX(24), CC1, 
     +         TA2, RATIOY, VUNIT, TDIST(24)
      LOGICAL  TOPCLIP, SOURIS, TSEPARE(24), VERTOFF, ANIM, FOUND,
     +         NOROUTE, GETBUT, BLACKLST
*
*   TDIST(DEVICE) = (NOMBRE DE POINTS EN Y / HAUTEUR EN Y (EN POUCE)) /
*                   (NOMBRE DE POINTS EN X / LARGEUR EN X (EN POUCE))
*
*   EXEMPLE : TDIST(IBMVGA) = (480 /7) / (640 / 10) = 1.0714
*
      DATA CLE/
     % 'DX',       'DY',     'HX',      'HY',       'SIZE',   'SPACE',
     % 'VSPACE',   'OFFSET', 'DEVICE',  'FRAMES',   'GR',     'MC.',
     % 'ORN',      'CADRE',  'NCP',     'DIST',     'FOND',   'DN.',
     % 'I',        'SIZEY',  'FC',      'UN',       'MF',     'TOPCLIP',
     % 'TNAME',    'NOROUTE',
     % 17*'COLORS',
     % 'REVERSE',
     % 10*'FONTES',
     % 5*'TABPEN',
     % 'VOFFSET' , 'FORMAT', 'SEGF.', 'PALETTE', 'ANIM' /

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
     % '0',        ' ',      'SEGFILE', ' ', 'OUI' /

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
     % '0',        ' ',      'SEGFILE', ' ', 'NON' /

*           -------------------------------------------------------
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
      DATA MENU(1) /'Selection d''un frame'/
      DATA MENU(2) /'Revoir le frame courant'/
      DATA MENU(3) /'Frame #  1'/
#include "mapping.cdk"
      CALL CCARD(CLE, DEF1, VAL, 64, -1)
      ERRCODE = FNOM(MCUNIT, VAL(12), 'RND,OLD' ,0)
      IF(ERRCODE .NE. 0) GOTO 160
      CALL WAOPEN( MCUNIT )
      ERRCODE = FNOM(SEGUNIT, VAL(62), 'RND', 0)
      ERRCODE = FNOM(77, 'STCKOOO', 'D77+UNF+SCRATCH', 100)
      ERRCODE = FNOM(88, 'STCKPPP', 'D77+UNF+SCRATCH', 150)
      IF(VAL(63) .NE. ' ') ERRCODE = FNOM(11, VAL(63), 'SEQ', 0)
      PROTOCL = 'SUNSCR'
      READ(VAL(1), 9) DX
      READ(VAL(2), 9) DY
      READ(VAL(3), 9) HX
      READ(VAL(4), 9) HY
      READ(VAL(5), 9) SIZEX
      READ(VAL(6), 9) SPACE
      READ(VAL(7), 9) VSPACE
      READ(VAL(8), 9) OFFSET
      READ(VAL(10),9) FRAMES
      READ(VAL(11),9) GR
      READ(VAL(15),9) NCP
      READ(VAL(16),9) DIST
      READ(VAL(17),9) FOND
      READ(VAL(20),9) SIZEY
      READ(VAL(60),9) VOFFSET
      FOND = MAX(0,MIN(FOND,15))
 9    FORMAT(I8)
      IF(VAL(26) .EQ. 'OUI') THEN
         NOROUTE = .TRUE.
      ELSE
         NOROUTE = .FALSE.
      ENDIF
      IF(VAL(64) .EQ. 'OUI') THEN
         ANIM    = .TRUE.
         ERRCODE = FNOM(6,'/dev/null','SEQ',0)
      ELSE
         ANIM    = .FALSE.
      ENDIF
      DEVICE  = SUN12
      ASKI    = .FALSE.
      STRFRM  = FRAMES/1000
      NFRAMES = MOD(FRAMES,1000)
      IF(VAL(13) .EQ. 'STD') THEN
         IROT = 0
      ELSE
         IROT = 90
      ENDIF
      MODEP = 100 * TAJUSTX(DEVICE)
      LCP   = 64/BITUSED * TLCP(DEVICE)

      DEVWIDT = 1280
      IF(NCP .EQ. -1) NCP = 8
      WRAST = LCP * NCP
      IF(SIZEX .EQ. -1) THEN
         CALL WGLSCON('x')
         CALL KEEPAS(1,1)
         III = WINOPE('?Metaview '//trim(XMETAVIEW_VERSION))
         CALL GETSIZ(ISZX,ISZY)
         SIZEX = MIN(ISZX,ISZY)
      ELSE
         CALL WGLSCON('x')
         III = WINOPE('?Metaview '//trim(XMETAVIEW_VERSION))
         CALL GETSIZ(ISZX,ISZY)
         HX = ISZX
         HY = ISZY
      ENDIF
      IF( ANIM ) THEN
         CALL WGLDBF
         CALL BACKBU
*        CALL FRONTB(.FALSE.)
      ENDIF
      IF(SIZEY .EQ. -1) SIZEY = SIZEX
      IF(DIST  .EQ. -1) THEN
         DISTOR = TDIST(DEVICE)
      ELSE
         DISTOR = DIST/1000.
      ENDIF

      IF(OFFSET .EQ. -1) OFFSET = TOFFSET(DEVICE)
      IF(VSPACE .EQ. -1) VSPACE = TVSPACE(DEVICE)
      IF(HX     .EQ.  0) HX     = SIZEX - DX
      IF(HY     .EQ.  0) HY     = SIZEY - DY
      IF(SPACE  .EQ. -1) SPACE  = HX + 100
      FRSTOFF = OFFSET
      INITSPA = SPACE
      INITVSP = VSPACE
      SPACE   = NINT(SPACE  * TAJUSTX(DEVICE))
      VSPACE  = NINT(VSPACE * DISTOR * TAJUSTX(DEVICE))
      GR      = 1
      ALPHA   = SIZEX / 32767.0
      ALPHAY  = SIZEY / 32767.0
      UUNIT   = 1.0
      VUNIT   = 1.0
      IF(IROT .EQ. 0) THEN
         HU  = NINT(HX * UUNIT) - 1
         HV  = NINT(HY * VUNIT) - 1
         A1  = ALPHA * HU / HX
         B1  = 0
         CC1 = OFFSET * UUNIT - FLOAT(DX * HU) / HX
         A2  = 0
         B2  =  ALPHAY * HV / HY
         C2  = (-DY) * HV / HY
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
 20      CONTINUE
      DO 30 I = 0,15
         TABCOL(I) = MOD(TABCOL(I),2**NCP)
 30      CONTINUE      
      TABCOL(16) = -1
      DO 40 I = 0,9
         READ(VAL(45+I),'(I2)') TABFONT(I)
 40      CONTINUE
      DO 50 I = 1,5
         READ(VAL(54+I),'(I2)') TABPEN(I)
 50      CONTINUE
      DO 60 I = 1,128
         HPALET(1,I) = I
         HPALET(2,I) = 1
         HPALET(3,I) = 16
 60      CONTINUE
      VOFFSET = NINT(VOFFSET * DISTOR * TAJUSTX(DEVICE))
      VERTOFF = .TRUE.
      OFFSET  = NINT(OFFSET * TAJUSTX(DEVICE))
      INITOFF = OFFSET
      WRITE(IPUNIT,610) trim(XMETAVIEW_VERSION),'Linux'
      CALL TTTPATI
      CALL TTTCARI
      WRITE(IPUNIT,620) REAL(SIZEX)/100.,   REAL(INITSPA)/100., 
     +                  REAL(INITVSP)/100., REAL(FRSTOFF)/100., 
     +                  REAL(SIZEY)/100. ,  IROT
      WRITE(IPUNIT,630) REAL(HX)/100.,      REAL(HY)/100., 
     +                  REAL(DX)/100.,      REAL(DY)/100.
      CALL TTTRIN
      ICGR   = 0
      STATUS = OK
      CALL TTTSKFR(STRFRM, STATUS)
      CALL TTTCFRM(POSFRM, NBFRAME)
      CALL TTTBAN(USAGER, JOBN, CDATE, CTIME, VAL(25), PROTOCL)
*      IF(BLACKLST( USAGER ) ) THEN
*         CALL WACLOS( MCUNIT )
*         CALL FCLOS( MCUNIT )
*         ERRCODE = 
*     +   FNOM(MCUNIT,'/home/cidsv02/armn/lib/usr/sec.met','RND',0)
*         CALL WAOPEN( MCUNIT )
*         IF(.NOT. ANIM) THEN
*            ANIM    = .TRUE.
*            ERRCODE = FNOM(6,'/dev/null','SEQ',0)
*            CALL WGLDBF
*            CALL BACKBU
*         ENDIF
*      ENDIF
      CALL INITDEV
      CALL TTTMAPC(11, VAL(63))
      SIZEX0 = SIZEX
      SIZEY0 = SIZEY
      DX0    = DX
      DY0    = DY
      PRINT *,' *** INFO: ***'
      PRINT *,' *** HX, HY, DX, DY = ',HX,HY,DX,DY

*     WHILE ((STATUS.<>.MARKEOI) .AND. (ICGR < NFRAMES))
 70   IF(STATUS.NE.MARKEOI .AND. ICGR.LT.NFRAMES) THEN
         ADRDEB = ADRESS(PILE)
         ALPHA  = SIZEX / 32767.0
         ALPHAY = SIZEY / 32767.0
         IF(IROT .EQ. 0) THEN
            HU  = NINT(HX * UUNIT) - 1
            HV  = NINT(HY * VUNIT) - 1
            A1  = ALPHA * HU / HX
            B1  = 0
            CC1 = OFFSET * UUNIT - FLOAT(DX * HU) / HX
            A2  = 0
            B2  =  ALPHAY * HV / HY
            C2  = (-DY) * HV / HY
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
         ICONT = 0
         C1    = CC1
         CALL COLOR(TABCOL(FOND))
         CALL SETPAT(1)
         CALL RECTFI(1,1,ISZX,ISZY)
         LASTCOL = -1
         LASTTIK = -1

*        WHILE ((ICONT < GR) .AND. (ICGR < NFRAMES))
 80      IF(ICONT.LT.GR .AND. ICGR.LT.NFRAMES) THEN
            ICONT   = ICONT + 1
            STATUS  = 0
            CALL TTTINIT
            COMPT4B = 0
            COMPTMB = 0
            CALL CPU_TIME(TA1)
            CALL TTTPRCS( STATUS )
            CALL CPU_TIME(TA2)
            IF(STATUS .EQ. MARKEOI) GOTO 90
            ICGR    = ICGR + 1
            WRITE(IPUNIT,600) ICGR, TA2-TA1
            WRITE(IPUNIT,605) COMPT4B, COMPTMB
            GOTO 80
         ENDIF
   
 90      IF((STATUS.NE.MARKEOI) .AND. (.NOT. ANIM)) THEN
            FF = MIN(2,NBFRAME)
 100        IF(ABS(ICGR-FF) .GT. 10) THEN
               FF = FF + 1
               GOTO 100
            ENDIF

            LF = NBFRAME - 1
 110        IF(ABS(LF-ICGR) .GT. 10) THEN
               LF = LF - 1
               GOTO 110
            ENDIF

            NBENTRY = 4
            DO 120 FNB = FF,LF
               WRITE(MENU(NBENTRY),'(A7,I3)') 'Frame #',FNB
               NBENTRY = NBENTRY + 1
 120           CONTINUE
            IF(NBFRAME .GT. 1) THEN
               WRITE(MENU(NBENTRY),'(A7,I3)') 'Frame #',NBFRAME
               NBENTRY = NBENTRY + 1
            ENDIF
            WRITE(MENU(NBENTRY),'(A)') 'Quitter Metaview'
            NBENTRY = NBENTRY + 1
            WRITE(MENU(NBENTRY),'(A,I3,A,I3)') 'Frame ', ICGR,
     +                          ' de ',NBFRAME

*           ATTENDRE UN EVENNEMENT
 130        IF(.NOT. SOURIS(BUTTON, EVENT, SGX0, SGY0, SGLX, SGLY, SGX1,
     +         SGY1, SGX2, SGY2, MENU, NBENTRY)) GOTO 130

            IF(EVENT .EQ. 1) THEN
               PRINT *,' *** INFO: NEXT FRAME ***'
               GOTO 70

            ELSEIF(EVENT .EQ. 2) THEN
               ADRESS(PILE) = ADRDEB
               RESTE(PILE)  = 0
               MINSGX       = MIN(SGX1,SGX2)
               MINSGY       = MIN(SGY1,SGY2)
               MAXSGX       = MAX(SGX1,SGX2)
               MAXSGY       = MAX(SGY1,SGY2)
               SGX1         = MINSGX
               SGY1         = MINSGY
               SGX2         = MAXSGX
               SGY2         = MAXSGY
               RATIOX       = REAL(SGLX) / REAL((SGX2-SGX1))
               RATIOY       = REAL(SGLY) / REAL((SGY2-SGY1))
               SIZEX        = SIZEX * RATIOX
               SIZEY        = SIZEY * RATIOY
               DX           = (DX + (SGX1 - SGX0)) * RATIOX
               DY           = (DY + (SGY1 - SGY0)) * RATIOY
               ICGR         = ICGR - 1
               STATUS       = OK
               PRINT *,' *** INFO: ZOOM ***'
               PRINT *,' ***       SIZE, DX, DY = ',SIZEX,DX,DY
               GOTO 70

            ELSEIF(EVENT .EQ. 3) THEN
               ADRESS(PILE) = ADRDEB
               RESTE(PILE)  = 0
               RATIOX       = REAL(SGLX) / REAL((SGX2-SGX1))
               RATIOY       = REAL(SGLY) / REAL((SGY2-SGY1))
               DX           = DX - (SGX2-SGX1)
               DY           = DY - (SGY2-SGY1)
               ICGR         = ICGR - 1
               STATUS       = OK
               PRINT *,' *** INFO: MOVE ***'
               PRINT *,' ***       DX, DY = ',DX,DY
               GOTO 70
         
            ELSEIF(EVENT .EQ. 4) THEN
               IF(SGX0 .EQ. NBENTRY-1) GOTO 150
               IF(SGX0 .GT. 2) READ(MENU(SGX0),'(A7,I3)') CJUNK, ICGR
               ADRESS(PILE) = POSFRM(ICGR)
               RESTE(PILE)  = 0
               ICGR         = ICGR - 1
               STATUS       = OK
               SIZEX        = SIZEX0
               SIZEY        = SIZEY0
               DX           = DX0
               DY           = DY0
               PRINT *,' *** INFO: SELECT FRAME ***'
               GOTO 70

            ELSEIF(EVENT .EQ. 5) THEN
               IF(ICGR .GT. 1) THEN
                  ICGR = ICGR - 1
               ELSE
                  ICGR = 1
               ENDIF
               ADRESS(PILE) = POSFRM(ICGR)
               RESTE(PILE)  = 0
               ICGR         = ICGR - 1
               STATUS       = OK
               PRINT *,' *** INFO: FRAME BEFORE ***'
               GOTO 70
            ENDIF
         ENDIF

         IF( ANIM ) THEN
*           FREEZE
 140        IF( GETBUT(1) ) GOTO 140
*           TERMINE
            IF( GETBUT(3) ) GOTO 150    
*           BOUCLE
            IF(STATUS .EQ. MARKEOI) THEN
               ICGR         = ICGR - 1
               STATUS       = OK
               ADRESS(PILE) = 1
               RESTE(PILE)  = 0
            ELSE
               CALL SWAPBU
            ENDIF
         ENDIF
         GOTO 70
      ENDIF   

 150  CONTINUE
      CALL WGLCLW( III )
      IF (.NOT. SEGCLOS) CALL WACLOS( SEGUNIT )
      CALL WACLOS( MCUNIT )
      CALL FRETOUR('STCKPPP')
      CALL FRETOUR('STCKOOO')
*      CALL QQQRMSC
      WRITE(IPUNIT, 660) ICGR
 160  STOP

 600  FORMAT(//T25,'***  R A S T E R -- GRAPHE NUMERO: ',I3,'   ',F8.3,
     +       ' S.')
 605  FORMAT(/T30,"--- NOMBRE D'INSTRUCTIONS DE 4 BYTES: ",I8,
     +       /T30,"--- NOMBRE D'INSTRUCTIONS MULTIBYTES: ",I8)
 610  FORMAT(/T10,'METAVIEW - ',A,' -', '  2002/02/07',
     +       ' TRADUCTEUR POUR ',A8/T10,71('=')//)
 620  FORMAT(//T10,'SIZE = ',F6.2,T35,'SPACE = ',F6.2,T55,'VSPACE = ',
     +        F6.2//T10,'OFFSET = ',F6.2,T35,'SIZEY = ',F6.2,
     +        T55,'ORIENTATION = ',I2,' DEGRE(S)')
 630  FORMAT(//T10,'HX = ',F6.2,T45,'HY = ',F6.2//T10,
     +       'DX = ',F6.2,T45,'DY = ',F6.2)
 660  FORMAT(//T40,'FIN D''INFORMATION APRES ',I3,' GRAPHE(S)')
 670  FORMAT(/T35,'C O M P R E S S I O N    ',F8.3,'  SECONDE(S)')

      END
