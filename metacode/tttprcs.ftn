*** S/P TTTPRCS - DETERMINE LE TYPE D'INSTRUCTION METACODE
*                 ET L'ACTION A ENTREPRENDRE
#include "aaaamac.cdk"
*
      SUBROUTINE TTTPRCS( STATUS )
#include "impnone.cdk"
      INTEGER  STATUS
*
*AUTEUR       M. VALIN / M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTPRCS)
*  DETERMINE LE TYPE D'INSTRUCTION METACODE TEL QUE MULTIBYTE OU
*  SIMPLE INSTRUCTION DE TRACAGE D'UNE LIGNE ET FAIT APPEL AU MODULE
*  MIS EN CAUSE
*
*MODULES
      EXTERNAL TTTSEGM, TTTFNTX, TTTPATR, TTTCOLR, HAFTPAL, PUSHOPT,
     +         TTTFMID, POPOPT
*
**
#include "params.cdk"
#include "inscont.cdk"
*
      REAL     RX, RY
      INTEGER  NY, NX, NPAIRS, ICP,TTTBPR, OPCODE
#if defined (CRAY) 
      EXTERNAL DFLERMS
#endif
*
      STATUS = TTTBPR(MCUNIT, BPAIRS, 2)

 10   IF(STATUS.NE.MARKEOF .AND. STATUS.NE.MARKEOI) THEN
         IF(ISHFT(BPAIRS(1), -15) .EQ. 0) THEN
            NX  = BPAIRS(1)
            NY  = IAND(BPAIRS(2), 32767)
            RX  = REAL(NX)
            RY  = REAL(NY)
            ICP = ISHFT(BPAIRS(2), -15)
            CALL TTTDRAW(RX, RY, ICP, INTLIN, COLLIN(MODLIN))
            COMPT4B = COMPT4B + 1
         ELSE
            IF(ISHFT(BPAIRS(1), -14) .EQ. 2) THEN
               WRITE( 6,*)' TTTPRCS  INSTRUCTION IMPOSSIBLE'
               RETURN
            ELSE
               IF(ISHFT(BPAIRS(1), -14) .EQ. 3) THEN
                  NPAIRS = ISHFT(IAND(255 , BPAIRS(1)+1),-1) -1
                  STATUS = TTTBPR(MCUNIT, BPAIRS(3), NPAIRS)
                  IF(STATUS .EQ. MARKEOF .OR. STATUS .EQ. MARKEOI) THEN
                     WRITE( 6,*)' TTTPRCS  FIN D"IMAGE INTEMPESTIVE'
                     RETURN
                  ENDIF
                  COMPTMB = COMPTMB + 1
*                 NOTE 63 = OCT(77)
                  OPCODE  = IAND(ISHFT(BPAIRS(1),-8), 63)
                  GOTO(33, 99, 35, 36, 37, 38, 99, 40, 41, 42, 43, 99,
     +                 45, 46, 47, 48, 49, 50) OPCODE-32
                  GO TO 99
            
 33               CALL TTTPWRY
                  GOTO 100

 35               CALL TTTOPTN
                  GOTO 100
            
 36               CALL TTTWNDW
                  GOTO 100

 37               CALL TTTERFI(1)
                  GOTO 100

 38               CALL TTTERFI(-1)
                  GOTO 100

 40               CALL TTTPWRY
                  GOTO 100

 41               CALL TTTHAFT
                  GOTO 100

 42               CALL TTTSEGM
                  GOTO 100

 43               CALL TTTFNTX
                  GOTO 100

 45               CALL TTTPATR
                  GOTO 100

 46               CALL TTTCOLR 
                  GOTO 100

 47               CALL HAFTPAL
                  GOTO 100

 48               CALL PUSHOPT
                  GOTO 100

 49               CALL POPOPT
                  GOTO 100

 50               CALL TTTFMID
                  GOTO 100

 99               WRITE(IPUNIT,200) OPCODE
 100              CONTINUE
               ELSE 
                  PRINT *,' *** TTTPRCS CAN T HAPPEN'
               ENDIF
            ENDIF
         ENDIF
         STATUS = TTTBPR(MCUNIT, BPAIRS, 2)
         GOTO 10
      ENDIF

      RETURN
 200  FORMAT(T20,'INVALID OPTION ',I4,' IGNORED')
      END


