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
#include <fcntl.h>
#include <ctype.h>
#include <malloc.h>
#include <rec.h>
#include <rpnmacros.h>
   
static int nbChampsDict = 0;
_InfoChamps *infoChamps = NULL;
FILE *fdict;

void f77name(initxmldict)(char nomfich[], F2Cl flennomfich)
{
  char nomDict[128];
  int lennomfich=flennomfich;

  nomfich[lennomfich] = '\0';
  nettoyer(nomfich);
  strcpy(nomDict, nomfich);
  if (0 == strcmp(nomfich, "stdout"))
    {
    fdict = stdout;
    }
  else
    {
    fdict = fopen(nomDict, "w");
    if (fdict == NULL)
      {
      fprintf(stderr, "Pas capable d'ouvrir le fichier!\n");
      exit(13);
      }
    
    fprintf(fdict, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n\n");
    fprintf(fdict, "<xrec_DataDictionnary>\n");
    
    }
  
}


void f77name(closexmldict)()
{
  fprintf(fdict, "</xrec_DataDictionnary>\n");
  fclose(fdict);
}

void f77name(initvar)(nomVar, idVar, unitesVar, paletteVar, echelleVar, indDef, intVar, nbIntVar, 
		      nomVarLen, idVarLen, unitesVarLen, paletteVarLen)
char nomVar[];
char idVar[];
char unitesVar[];
char paletteVar[];
float *echelleVar;
int   *indDef;
float  intVar[][24];
int   *nbIntVar;
int   nomVarLen, idVarLen, unitesVarLen, paletteVarLen;
{
   int i,j, indDict;
   int nbMenuItems;
   char tab[4];
   
   
   nomVar[nomVarLen-1] = '\0';
   idVar[idVarLen-1] = '\0';
   unitesVar[unitesVarLen-1] = '\0';
   paletteVar[paletteVarLen-1] = '\0';

   nettoyer(nomVar);
   nettoyer(idVar);
   nettoyer(unitesVar);
   nettoyer(paletteVar);
   strcpy(tab, "   ");

   fprintf(fdict, "<metvar>\n");
   fprintf(fdict, "%s <nomvar>%s</nomvar>\n", tab, nomVar);
   fprintf(fdict, "%s <description>\n", tab);
   fprintf(fdict, "%s %s <francais>%s</francais>\n", tab, tab, idVar);
   fprintf(fdict, "%s %s <english>%s</english>\n", tab, tab, idVar);
   fprintf(fdict, "%s </description>\n", tab);
   fprintf(fdict, "%s <units>%s</units>\n", tab,unitesVar);
   fprintf(fdict, "%s <color_palette>\n");
   fprintf(fdict, "%s %s <palette>default</palette>\n", tab, tab);
   fprintf(fdict, "%s %s <palette_min>0.0</palette_min>\n", tab, tab);
   fprintf(fdict, "%s %s <palette_max>1.0</palette_max>\n", tab, tab);
   fprintf(fdict, "%s %s <palette_variation>linear</palette_variation>\n", tab, tab);
   fprintf(fdict, "%s </color_palette>\n", tab);
   fprintf(fdict, "%s <min>auto</min>\n", tab);
   fprintf(fdict, "%s <max>auto</max>\n", tab);
   fprintf(fdict, "%s <scaling_factor>%g</scaling_factor>\n", tab, *echelleVar);

   j = 0;
   while (nbIntVar[j] > 0)
      {
      j++;
      }

   nbMenuItems = j;
   
   fprintf(fdict, "%s <default_contour>%g</default_contour>\n", tab, intVar[0][*indDef]);

   for (j=0; j < nbMenuItems; j++)
     {
     if (j != *indDef)
       {
       fprintf(fdict, "%s <contour>", tab);
       for (i=0; i < nbIntVar[j]; i++)
	 {
	 fprintf(fdict, " %g", intVar[i][j]);
	 }
       fprintf(fdict, "</contour>\n");    
       }
     }

   fprintf(fdict, "</metvar>\n\n");

   }


nettoyer(chaine)
char chaine[];
{
   int longueur;

   longueur = strlen(chaine)-1;
   while (chaine[longueur] == ' ' || chaine[longueur] == '\0')
      {
      chaine[longueur] = '\0';
      longueur--;
      }
   }




