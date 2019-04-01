*/* RMNLIB - Library of useful routines for C and FORTRAN programming
* * Copyright (C) 1975-2001  Division de Recherche en Prevision Numerique
* *                          Environnement Canada
* *
* * This library is free software; you can redistribute it and/or
* * modify it under the terms of the GNU Lesser General Public
* * License as published by the Free Software Foundation,
* * version 2.1 of the License.
* *
* * This library is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* * Lesser General Public License for more details.
* *
* * You should have received a copy of the GNU Lesser General Public
* * License along with this library; if not, write to the
* * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* * Boston, MA 02111-1307, USA.
* */
      SUBROUTINE XDASHGEC(CREF,IVAL)
*     implicit none
      CHARACTER * (*) CREF
      INTEGER IVAL
      INTEGER IREF
      READ(CREF,'(A4)') IREF
      CALL XDASHGET(IREF,IVAL)
      RETURN
      END
***S/P  XDASHGET  ROUTINE QUI RETOURNE DANS IVAL LA VALEUR TROUVEE A LA
*               POSITION CORRESPONDANTE A REF DANS UN CHAMP DE DONNEES.
      SUBROUTINE XDASHGET(REF,IVAL)
*
*AUTEUR   M. VALIN  -  C. THIBEAULT   OCTOBRE 1980
*
*FORTRAN
*
*OBJET(DASHGET)
*         DASHGET CHERCHE DANS 3 DICTIONNAIRES LA POSITION D'UNE CHAINE
*         DE CARACTERES QUI CORRESPOND A REF, AFIN DE RETOURNER DANS
*         IVAL LA VALEUR TROUVE DANS LE CHAMP IAR A LA POSITION DE REF
*         DANS LE DICTIONNAIRE APPROPRIE.
*
*LIBRAIRIES
*         SOURCE  NEWDASHPL,ID=CMCULIB.
*         OBJET   NCARSYSOB,ID=CMCULIB
*
*APPEL    CALL DASHGET(REF,IVAL)
*
*ARGUMENTS
*         REF    CHAINE DE CARACTERES QUE L'ON VEUT RETROUVER DANS UN
*                DES DICTIONNAIRES
*
*         IVAL   VALEUR RETOURNEE
*
*
*-----------------------------------------------------------------------
*
*     implicit none
      COMMON /XDASHD3/ DICIN(8)
      COMMON /XDASHD2/ FPART,IPAU,IGP,IOFFS,TENSN,NP,SMALL,L1
*
      CHARACTER * 2 OPNAM
*
      WRITE(OPNAM,'(A2)') REF
      READ(OPNAM,'(A2)') IREF
      CALL XOPGET(DICIN,IREF,FPART,IVAL,8)
*
      RETURN
      END
