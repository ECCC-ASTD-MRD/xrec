*** S/P TTTSEGM  -  GESTION D'UN APPEL DE SEGMENT
#include "aaaamac.cdk"
*
      SUBROUTINE TTTSEGM
#include "impnone.cdk"
*
*AUTEUR       M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTSEGM)
*     REDIRIGER LA LECTURE DU METACODE LORS D'UN APPEL DE SEGMENT
*
*MODULES
      EXTERNAL WAOPEN, WAREAD, IIPAK, DFLERMS, PUSHPAR, TTTPWI1
*
**
      REAL     A1P, B1P, C1P, A1X, B1X, C1X, ALFA, THETA,
     +         A2P, B2P, C2P, A2X, B2X, C2X
      INTEGER  XT, YT, NUMER, SEGNAME, POS,  DIRECT(2,0:44), IR,
     +         X0, Y0, DENOM, MAXIND,  IADR, MBUF(MBUFSZ)
      LOGICAL  TROUVE
#include "segvar.cdk"
#include "mapvar.cdk"
#include "bpairp.cdk"
#include "params.cdk"

*     SEGNAME = ISHFT(BPAIRS(2),16) + BPAIRS(3)
      SEGNAME =ishft(iand(ishft(-1,-(16)),BPAIRS(2)),16) + BPAIRS(3)
      XT      = BPAIRS(4)
      YT      = BPAIRS(5)
      NUMER   = BPAIRS(6)
      DENOM   = BPAIRS(7)
      IR      = BPAIRS(8) - 32768
      X0      = BPAIRS(9)
      Y0      = BPAIRS(10)
      ALFA    = REAL(NUMER)/REAL(DENOM)
      IF(IAND(BPAIRS(1),255) .EQ. 12) THEN  ! ANCIEN APPEL A SEGMENT
         IR = 0
         X0 = 0
         Y0 = 0
      ENDIF
      THETA = IR * 3.1415927 / 180.
      IF( SEGCLOS ) THEN
         CALL WAOPEN( SEGUNIT )
         SEGCLOS = .FALSE.
      ENDIF
      CALL WAREAD(SEGUNIT, MBUF, 1, MBUFSZ)
#if !defined (UNIX) 
      CALL IIPAK(DIRECT(1,0), MBUF, 2, 45, -32, 0, 2)
#else
      CALL MOVLEV(MBUF, DIRECT(1,0), MBUFSZ)
#endif
      MAXIND = DIRECT(1,1)
      POS    = 0
      TROUVE = .FALSE.

 10   IF( ((POS.LT. MAXIND) .AND. (.NOT. TROUVE)))THEN
         IF(MOD(POS,MBUFSZ) .EQ. 0) THEN
            IADR = ((1 + POS/MBUFSZ) * MBUFSZ) + 1
            CALL WAREAD(SEGUNIT, MBUF, IADR, MBUFSZ)
#if !defined (UNIX)  
            CALL IIPAK(DIRECT(1,0), MBUF, 2, 45, -32, 0, 2)
#else
            CALL MOVLEV(MBUF, DIRECT(1,0), MBUFSZ)
#endif
         ENDIF
         IF(DIRECT(1,MOD(POS,MBUFSZ)) .EQ. SEGNAME) TROUVE = .TRUE.
         POS = POS + 1
         GO TO 10
      ENDIF

      IF( TROUVE ) THEN
         IF(PILE .GE. MAXPILE) THEN
            WRITE(6,*)' TTTSEGM  NIVEAU D"IMBRICATION TROP GRAND'
         ELSE
            MAPUSAV(1,PILE) = A1
            MAPUSAV(2,PILE) = B1
            MAPUSAV(3,PILE) = C1
            MAPVSAV(1,PILE) = A2
            MAPVSAV(2,PILE) = B2
            MAPVSAV(3,PILE) = C2
            PILE = PILE + 1
#if defined (RASTER) 
            A1P  = ALFA   * COS(THETA)
            B1P  = - ALFA * SIN(THETA)
            C1P  = (ALFA  * SIN(THETA)* Y0)-(ALFA*COS(THETA)*X0)+XT
            A2P  = ALFA   * SIN(THETA)
            B2P  = ALFA   * COS(THETA)
            C2P  = - (A2P * X0) - (B2P * Y0) + YT
            A1X  = (A1 * A1P) + (B1 * A2P)
            A2X  = (A2 * A1P) + (B2 * A2P)
            B1X  = (A1 * B1P) + (B1 * B2P)
            B2X  = (A2 * B1P) + (B2 * B2P)
            C1X  = (A1 * C1P) + (B1 * C2P) + C1
            C2X  = (A2 * C1P) + (B2 * C2P) + C2
            A1   = A1X
            B1   = B1X
            C1   = C1X
            A2   = A2X
            B2   = B2X
            C2   = C2X
#endif
#if defined (UNIVERS) 
            A1   = A1 + C1 * DELTAX
            B1   = B1 + C1 * DELTAY
            C1   = C1 * ALFA
#endif
#if defined (i386)
*     BUG fix pour forcer le compilateur a storer les variables du common
            call bugfix_comp(a1,b1,c1,a2,b2,c2)
#endif
            POS  = POS - 1
            ADRESS(PILE) = DIRECT(2,MOD(POS,MBUFSZ)) * MBUFSZ + 1
         ENDIF
      ENDIF

      CALL PUSHPAR
      CALL TTTPWI1

      RETURN
      END
#if defined (i386)
      subroutine bugfix_comp(a1,b1,c1,a2,b2,c2)
      return
      end
#endif
      
