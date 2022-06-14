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
#include <string.h>

int  strfind();

int strfind(SousChaine, Chaine)
char *SousChaine, *Chaine;
{
int i,j, LongueurChaine, LongueurSousChaine, PositionTrouvee;

PositionTrouvee = -1;
LongueurChaine = strlen (Chaine);
LongueurSousChaine = strlen (SousChaine);

if (LongueurChaine < LongueurSousChaine)
   return(PositionTrouvee);

i=0;
while (i < LongueurChaine && PositionTrouvee == -1)
      {
      if (SousChaine[0] == Chaine[i])
	 {
	 j = i + LongueurSousChaine - 1;

	 if (j > LongueurChaine)
	    return(PositionTrouvee);

	 while (j > i && SousChaine[j-i] == Chaine[j])
	       j--;

         if (j==i)
	    PositionTrouvee = i;
         }
      i++;
      }
return(PositionTrouvee); 
}



/**
void LireLigne(FichierEntree, Ligne)
FILE *FichierEntree;
char Ligne[];
{
   int i=0;

   strcpy(Ligne, "");
   if (feof(FichierEntree) != 0)
          return;

   Ligne[0] = fgetc(FichierEntree);
   while (feof(FichierEntree) == 0 && Ligne[i] != '\n')
      {
          i++;
	  Ligne[i] = fgetc(FichierEntree);
	  }
   Ligne[i] = '\0';
   }
**/
