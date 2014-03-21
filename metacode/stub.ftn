      subroutine dflerms
      print *,'Debug passe dans dflerms'
      return
      end

      SUBROUTINE TTTLNCP( JUNK )
#include "impnone.cdk"
      INTEGER JUNK

      PRINT *,' *** ERREUR: TTTLNCP NON PRESENT'

      RETURN
      END


*** S/P TTTPATR - PERMET LA REDEFINITION D'UN PATRON
*
      SUBROUTINE TTTPATR
#if defined (CRAY)
#include "impnone.cdk"
*
*AUTEUR       M. LEPINE
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(TTTPATR)
*     PERMET LA REDEFINITION D'UN PATRON
*
**
#include "params.cdk"
#include "patrns.cdk"
      INTEGER NP, IP, IP0, I, J

IF ( (BPAIRS(1) .AND. OCT(377)) == 130 )
  $(
    NP = BPAIRS(2)
    IF ((NP >= 100) .AND. (NP <= 128))
      IF (MODEP == 200)
        DO I = 1,32
          $(
IP= SHIFTL((BPAIRS(2*I+1)&OCT(177777)),16)!(BPAIRS(2*I+2) & OCT(177777))
             IP0 = SHIFT(IP,32)
             IP = 0
             DO J = 1,32
               $(
                 IP = SHIFT(IP,2)
                 IF (IP0 < 0) IP = IP .OR. 3
                 IP0 = SHIFT(IP0,1)
               $)
             ITP(I,NP) = IP
           $)
       ELSE
         DO I = 1,32
           $(
IP= SHIFTL((BPAIRS(2*I+1)&OCT(177777)),16)!(BPAIRS(2*I+2) & OCT(177777))
             ITP(I,NP) = IP .OR. SHIFT(IP,32)
           $)
  $)
ELSE
      WRITE(IPUNIT,*) ' *** MAUVAISE LONGUEUR - TTTPATR'
#endif
      RETURN
      END


#if defined (UNIX) 
      SUBROUTINE TTTSHUF( JUNK )
#include "impnone.cdk"
      INTEGER JUNK

      PRINT *,' *** ERREUR: TTTSHUF NON PRESENT'

      RETURN
      END


      INTEGER FUNCTION EOF( IUN )
#include "impnone.cdk"
      INTEGER IUN

      EOF = 0

      RETURN
      END


#endif

#if defined (RASTER)
*** F/N DPC2ASC   CONVERSION DISPLAY CODE A ASCII
#include "aaaamac.cdk"
*
      FUNCTION DPC2ASC( JJJ )
#include "impnone.cdk"
      INTEGER  DPC2ASC, JJJ
*
*AUTEUR       M. LEPINE   -  AOUT 86
*
*REVISION 001 Y. BOURASSA - MAI 94 RATFOR @ FTN77
*
*LANGUAGE   - FTN77
*
*OBJET(ASC)
*     FAIRE LA CONVERSION D'UN CARACTERE DISPLAY CODE A ASCII
*
**
#if defined (CRAY)
      DPC2ASC = JJJ
#endif
      RETURN
      END
#endif



