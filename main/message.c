/* RMNLIB - Library of useful routines for C and FORTRAN programming
 * Copyright (C) 1975-2001  Division de Recherche en Prevision Numerique
 *                          Environnement Canada
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation,
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <wgl.h>
/**
   static char *messNouveauProfil[] = {"\
Dans la fenetre d'affichage...\n\n\
clic bouton gauche --> profil\n\
drag bouton gauche + clic  --> coupe\n\
clic bouton droit --> Terminer","\
In the drawing window...\n\n\
clic left button --> profile\n\
drag left button + clic --> cross section\n\
clic right button --> exit"};

   static char *messScanProfil[] = {"\
Dans la fenetre d'affichage...\n\n\
bouton gauche enfonce --> profil\n\
lever le bouton gauche pour terminer", "\
In the drawing window...\n\n\
left button pressed --> profile\n\
release left button to exit"};

   static char *messScanCoupe[] = {"\
Dans la fenetre d'affichage...\n\n\
bouton gauche enfonce --> coupe\n\
lever le bouton gauche pour terminer", "\
In the drawing window...\n\n\
left button pressed --> cross section\n\
release left button to exit"};

main()
{
   c_wglpsz(500, 500);
   c_wglopw("message");
   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);

   MessageAide(messNouveauProfil[0]);
   while (!c_wglbtn(BGAUCH));

   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);

   MessageAide(messNouveauProfil[1]);
   while (!c_wglbtn(BGAUCH));
   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);

   MessageAide(messScanProfil[0]);
   while (!c_wglbtn(BGAUCH));
   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);

   MessageAide(messScanProfil[1]);
   while (!c_wglbtn(BGAUCH));
   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);

   MessageAide(messScanCoupe[0]);
   while (!c_wglbtn(BGAUCH));

   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);

   MessageAide(messScanCoupe[1]);
   while (!c_wglbtn(BGAUCH));
   }
**/
MessageAide(str)
char *str;
{
   int i;
   int lenstr;
   int hauteurFenetre, largeurFenetre;
   int hauteurRect, largeurRect;
   int hauteurTexte, largeurTexte;
   int largeurMax, largeurCourante;
   int ncr, ind;
   char tmpStr[128];

   lenstr = strlen(str);
   c_wglfsz(17);
   c_wglgwz(&largeurFenetre, &hauteurFenetre);

   ncr = 0;
   for (i=0; i < lenstr; i++)
      {
      if (str[i] == '\n')
	 ncr++;
      }
      
   largeurCourante = 0;
   largeurMax = -1;
   for (i=0; i < lenstr; i++)
      {
      if (str[i] == '\n' || i == (lenstr-1))
	 {
	 if (largeurMax < largeurCourante)
	    {
	    largeurMax = largeurCourante;
	    }
	 largeurCourante = 0;
	 }
      else
	 {
	 tmpStr[largeurCourante] = str[i];
	 largeurCourante++;
	 }
      }
     
   tmpStr[largeurMax+1] =  '\0';
   hauteurTexte = c_wglhsi(tmpStr, strlen(tmpStr));
   largeurTexte = c_wglwsi(tmpStr, strlen(tmpStr));

   hauteurRect = (ncr+2)*hauteurTexte + 40;
   largeurRect = 40 + largeurTexte;

   c_wgllwi(3);
   c_wglrli((largeurFenetre-largeurRect)/2, (hauteurFenetre-hauteurRect)/2, 
	    (largeurFenetre+largeurRect)/2, (hauteurFenetre+hauteurRect)/2);

   c_wglrli((largeurFenetre-largeurRect)/2+5, (hauteurFenetre-hauteurRect)/2+5, 
	    (largeurFenetre+largeurRect)/2-5, (hauteurFenetre+hauteurRect)/2-5);

   ind = 0;
   ncr = 1;
   tmpStr[0] = '\0';
   for (i=0; i < lenstr; i++)
      {
      if (str[i] == '\n' || i == (lenstr-1))
	 {
	 if (i == (lenstr-1))
	    {
	    tmpStr[ind] = str[i];
	    ind++;
	    }

	 tmpStr[ind] = '\0';
	 largeurTexte = c_wglwsi(tmpStr, strlen(tmpStr));
	 c_wglpsi((largeurFenetre-largeurTexte)/2,
		  (hauteurFenetre+hauteurRect-40)/2-(ncr*hauteurTexte), 
		  tmpStr, strlen(tmpStr), 17, 0, 0);
	 ind = 0;
	 ncr++;
	 }
      else
	 {
	 tmpStr[ind] = str[i];
	 ind++;
	 }
      }
   }
