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
      SUBROUTINE XAUTOSEC(CREF,CVAL)
      implicit none
      CHARACTER(LEN=*) CREF, CVAL
      INTEGER IREF,IVAL
      READ(CREF,'(A4)') IREF
      READ(CVAL,'(A4)') IVAL
      CALL XAUTOSET(IREF,IVAL)
      RETURN
      END
      SUBROUTINE XAUTOSEI(CREF,IVAL)
      implicit none
      CHARACTER(LEN=*) CREF
      INTEGER IREF,IVAL
      READ(CREF,'(A4)') IREF
      CALL XAUTOSET(IREF,IVAL)
      RETURN
      END
      SUBROUTINE XAUTOSEL(CREF,LVAL)
      implicit none
      CHARACTER(LEN=*) CREF
      INTEGER IREF
      LOGICAL LVAL
      READ(CREF,'(A4)') IREF
      CALL XAUTOSET(IREF,LVAL)
      RETURN
      END
      SUBROUTINE XAUTOGEC(CREF,CVAL)
      implicit none
      CHARACTER(LEN=*) CREF, CVAL
      INTEGER IREF,IVAL
      READ(CREF,'(A4)') IREF
      CALL XAUTOGET(IREF,IVAL)
      WRITE(CVAL,'(A4)') IVAL
      RETURN
      END
      SUBROUTINE XAUTOGEI(CREF,IVAL)
      implicit none
      CHARACTER(LEN=*) CREF
      INTEGER IREF,IVAL
      READ(CREF,'(A4)') IREF
      CALL XAUTOGET(IREF,IVAL)
      RETURN
      END
      SUBROUTINE XAUTOGET(REF,IVAL)
      IMPLICIT NONE
      INTEGER IVAL,REF
*
***S/P  XAUTOGET  ROUTINE QUI RETOURNE DANS IVAL LA VALEUR TROUVEE A LA
*                POSITION CORRESPONDANTE A REF DANS UN CHAMP DE DONNEES.
*                AUTOSET INITIALISE AUSSI DES DICTIONNAIRES.
*ENTRY  XDASHSET  ROUTINE QUI INTRODUIT UNE VALEUR NOUVELLE A LA
*                POSITION CORRESPONDANTE A REF DANS UN CHAMP DE DONNEES
*
*AUTEUR   M. VALIN  -  C. THIBEAULT   MAI 1984
*
*LANGAGE  FORTRAN
*
*OBJET(AUTOGET)
*         AUTOGET CHERCHE DANS 3 DICTIONNAIRES LA POSITION D'UNE CHAINE
*         DE CARACTERES QUI CORRESPOND A REF, AFIN DE RETOURNER DANS
*         IVAL LA VALEUR TROUVE DANS LE CHAMP IAR A LA POSITION DE REF
*         DANS LE DICTIONNAIRE APPROPRIE.
*         INITIALISE AUSSI DES DICTIONNAIRES UTILISES PAR PLOT80.
*OBJET(AUTOSET)
*         DASHSET CHERCHE DANS 3 DICTIONNAIRES LA POSITION D'UNE CHAINE
*         DE CARACTERES QUI CORRESPOND A REF, AFIN DE CHANGER PAR IVAL
*         A LA MEME POSITION LA VALEUR QUI EXISTE DANS LE CHAMP DE
*         DONNEES DANS LE DICTIONNAIRE CORRESPONDANT.
*
*LIBRAIRIES
*         SOURCE  AUTOGPL/ARMNSRC.
*         OBJET   RMNLIB5,ID=RMNP
*
*APPEL    CALL XAUTOGET(REF,IVAL)
*         CALL XAUTOSET(REF,IVAL)
*
*ARGUMENTS
*         REF    CHAINE DE CARACTERES QUE L'ON VEUT RETROUVER DANS UN
*                DES DICTIONNAIRES
*
*         IVAL   VALEUR RETOURNEE (AUTOGET)
*                VALEUR QUE L'ON VEUT REMPLACER DANS LE CHAMP DE DONNEES
*                APPROPRIE. (AUTOSET)
*
*
*-----------------------------------------------------------------------
*
*
*  DICTIONNAIRE POUR AJUSTER LA TAILLE DES CARACTERES POUR ANNOTER
*  LES GRAPHIQUES
*
      INTEGER ISZXAX,ISZYAX,ISZXLB,ISZYLB,ISZCRV,ISZTYT,NUMX,
     1        NUMY,IXOR,IYOR
      COMMON /XCHARSZ/ ISZXAX,ISZYAX,ISZXLB,ISZYLB,ISZCRV,ISZTYT,
     1                NUMX,NUMY,IXOR
      INTEGER DICSZ4
      COMMON /XAUTOG4/ DICSZ4(6)
*
*  DICTIONNAIRE QUI CONTROLE LES GRAPHIQUES AVEC DES BARRES D'ERREURS
*
      INTEGER DICBA
      COMMON /XAUTOG5/ DICBA(7)
      LOGICAL CLIP,JOIN
      INTEGER ERRBAR,SSYM,SSIZ,EPAIS,FONC
      COMMON /XAUTOG6/ CLIP,ERRBAR,JOIN,SSYM,SSIZ,EPAIS,FONC
      EXTERNAL XOPGET, XOPSET
*
*  INITIALISATION DES DICTIONNAIRES
*
*     DATA DICSZ4 /"SZXA", "SZYA", "SZXL", "SZYL", "SZCR", "SZTY" /
      DATA DICSZ4 /4HSZXA, 4HSZYA, 4HSZXL, 4HSZYL, 4HSZCR, 4HSZTY /
*
*      DATA  ISZXAX, ISZYAX, ISZXLB, ISZYLB, ISZCRV, ISZTYT
*     1    /    0  ,    0  ,    0  ,    0  ,    0  ,    0    /
*
*     DATA DICBA /"CLIP", "ERRB", "JOIN" , "SSYM" , "SSIZ" , "EPAI" ,
*    1            "FONC" /
      DATA DICBA /4HCLIP, 4HERRB, 4HJOIN , 4HSSYM , 4HSSIZ , 4HEPAI ,
     1            4HFONC /
      DATA         CLIP  , ERRBAR,   JOIN  ,  SSYM  ,  SSIZ  ,  EPAIS  ,
     1             FONC
     2           /.FALSE.,   0   ,  .TRUE. , 4H     ,    0   ,    1    ,
     3            4HYVSX /
*
      CALL XOPGET(DICSZ4,REF,ISZXAX,IVAL,6)
      CALL XOPGET(DICBA,REF,CLIP,IVAL,7)
      RETURN
*
      ENTRY XAUTOSET(REF,IVAL)
      CALL XOPSET(DICSZ4,REF,ISZXAX,IVAL,6)
      CALL XOPSET(DICBA,REF,CLIP,IVAL,7)
      RETURN
      END
