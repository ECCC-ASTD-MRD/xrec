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
#include <xinit.h>
#include <rpnmacros.h>
#include <expat.h>
   
int nbChampsDict = 0;
_InfoChamps *infoChamps = NULL;

int LireDictionnaireXML(FILE *fichierEntree);

/**
*****************************************************************
*****************************************************************
**/

int GetNbChampsDict()
{
   return nbChampsDict;
   }

/**
*****************************************************************
*****************************************************************
**/

int LireDictionnaireRMNLIB(infoChamps)
_InfoChamps infoChamps[];
{
   FILE *fichierEntree;
   int i, j, k;
   char *tmp, *home;
   char nomFichierDictionnaire[128];
   int lng;

   tmp = (char *) getenv("ARMNLIB");
   if (tmp == NULL)
      {
      NoRMNLIB();
      exit(-1);
      }
   
   lng = c_getulng();
   strcpy(nomFichierDictionnaire, tmp);
   
   if (lng == 0)
      strcat(nomFichierDictionnaire, "/data/dict_rec.f");
   else
      strcat(nomFichierDictionnaire, "/data/dict_rec.e");
   
   fichierEntree = fopen(nomFichierDictionnaire, "r");
   if (fichierEntree == NULL)
      {
      if (lng == 0)
	 printf("Impossible d'ouvrir le fichier $ARMNLIB/data/dict_rec.f... \nImpossible de continuer.\n");
      else
	 printf("Can't open file $ARMNLIB/data/dict_rec.e... \nCan't continue.\n");
      exit(-1);
      }
   
   fclose(fichierEntree);

   f77name(rlx)(nomFichierDictionnaire, strlen(nomFichierDictionnaire));
   
   return nbChampsDict;
   }

/**
*****************************************************************
*****************************************************************
**/

int LireDictionnaireRMNLIB_XML(infoChamps)
_InfoChamps infoChamps[];
{
   FILE *fichierEntree;
   int i, j, k;
   char *tmp, *home;
   char nomFichierDictionnaire[128];
   int lng;

   tmp = (char *) getenv("ARMNLIB");
   if (tmp == NULL)
      {
      NoRMNLIB();
      exit(-1);
      }
   
   lng = c_getulng();
   strcpy(nomFichierDictionnaire, tmp);
   strcat(nomFichierDictionnaire, "/data/rmndict.xml");
   
   fichierEntree = fopen(nomFichierDictionnaire, "r");
   if (fichierEntree == NULL)
      {
      switch (lng)
	{
	case FRANCAIS:
	  printf("Impossible d'ouvrir le fichier $ARMNLIB/data/dict_rec.f... \nImpossible de continuer.\n");
	  exit(-1);	 
	  break;
	  
	case ENGLISH:
	  printf("Can't open file $ARMNLIB/data/dict_rec.e... \nCan't continue.\n");
	  exit(-1);
	  break;
	}
      
      }
   
   LireDictionnaireXML(fichierEntree);
   
   fprintf(stderr,"Nombre de variables lues du dict. XML : %d\n", nbChampsDict);

   fclose(fichierEntree);
   
   }

/**
*****************************************************************
*****************************************************************
**/

int LireDictionnaireUsager(infoChamps)
_InfoChamps infoChamps[];
{
   FILE *fichierEntree;
   char *tmp, *home;
   static char *messageDictPersonnel[] = {"Lecture du dictionnaire personnalise pour %s\n", 
				      "Reading user dictionnary of %s\n"};
   char nomFichierDictionnaire[128];
   int lng;

   lng = c_getulng();
   home = (char *) getenv("HOME");
   strcpy(nomFichierDictionnaire, home);
   strcat(nomFichierDictionnaire, "/.recrc");
   fichierEntree = fopen(nomFichierDictionnaire, "r");
   if (fichierEntree)
      {
      fclose(fichierEntree);
      printf(messageDictPersonnel[lng], (char *)getenv("USER"));
      f77name(rlx)(nomFichierDictionnaire, strlen(nomFichierDictionnaire));
      }

   return nbChampsDict;

   }


/**
*****************************************************************
*****************************************************************
**/

int LireStartRec()
{
   FILE *fichierEntree;
   char *tmp, *home;
   static char *messageDictPersonnel[] = {"Lecture du fichier de demarrage pour %s\n", 
				      "Reading startup file of %s\n"};
   char nomFichierDictionnaire[128];
   int lng;

   lng = c_getulng();
   home = (char *) getenv("HOME");
   strcpy(nomFichierDictionnaire, home);
   strcat(nomFichierDictionnaire, "/.startrec");
   fichierEntree = fopen(nomFichierDictionnaire, "r");
   if (fichierEntree)
      {
      fclose(fichierEntree);
      printf(messageDictPersonnel[lng], (char *)getenv("USER"));
      f77name(rlx)(nomFichierDictionnaire, strlen(nomFichierDictionnaire));
      }

   return nbChampsDict;

   }


/**
*****************************************************************
*****************************************************************
**/

int ChercherNomVar(nomVar)
char *nomVar;
{
   int i;
   
   i = 0;
   while (i < nbChampsDict && 0 != strcmp(nomVar, infoChamps[i].nomVar))
      {
      i++;
      }
   
   if (i == nbChampsDict)
      return -1;
   else
      return i;
   
   }


/**
*****************************************************************
*****************************************************************
**/

int AjouterNomVar(nomVar)
char *nomVar;
{
   int i;

   if (0 == (nbChampsDict % 16))
      infoChamps = (_InfoChamps *)realloc(infoChamps, sizeof(_InfoChamps)*(nbChampsDict+16));
   
   infoChamps[nbChampsDict].facteurDeConversion = 1.0;
   infoChamps[nbChampsDict].indIntervalleDeDefaut =  10;

   strcpy(infoChamps[nbChampsDict].nomVar, nomVar);
   strcpy(infoChamps[nbChampsDict].identifVar, nomVar);
   strcpy(infoChamps[nbChampsDict].unitesVar, " (???)");

   infoChamps[nbChampsDict].nbMenuItems = 16;
   infoChamps[nbChampsDict].nbIntervalles = (int *)calloc(16, sizeof(int));
   infoChamps[nbChampsDict].intervallesDeContour = (float **)calloc(16, sizeof(float *));

   for (i=0; i < 16; i++)
      {
      infoChamps[nbChampsDict].nbIntervalles[i] = 1;
      infoChamps[nbChampsDict].intervallesDeContour[i] = (float *)calloc(1, sizeof(float));
      infoChamps[nbChampsDict].intervallesDeContour[i][0] = 1.0;
      }
   
   DictMgrSetMinMaxMode(nomVar,nbChampsDict,AUTO);
   nbChampsDict += 1;

   return (nbChampsDict-1);   
   }

/**
*****************************************************************
*****************************************************************
**/


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

   nomVar[nomVarLen-1] = '\0';
   idVar[idVarLen-1] = '\0';
   unitesVar[unitesVarLen-1] = '\0';
   paletteVar[paletteVarLen-1] = '\0';

   nettoyer(nomVar);
   nettoyer(idVar);
   nettoyer(unitesVar);
   nettoyer(paletteVar);

   j = 0;
   while (nbIntVar[j] > 0)
      {
      j++;
      }

   nbMenuItems = j;

   indDict = ChercherNomVar(nomVar);
   if (indDict < 0)
      {
      indDict = nbChampsDict;
      }
   else
      {
      for (i=0; i < infoChamps[indDict].nbIntervalles[i]; i++)
	 {
	 free(infoChamps[indDict].intervallesDeContour[i]);
	 }
      free(infoChamps[indDict].intervallesDeContour);
      free(infoChamps[indDict].nbIntervalles);
      
      }
   
   
   if (0 == (nbChampsDict % 16) && indDict == nbChampsDict)
      {
      if (nbChampsDict == 0)
	 infoChamps = (_InfoChamps *)calloc(16, sizeof(_InfoChamps));
      else
	 infoChamps = (_InfoChamps *)realloc(infoChamps, sizeof(_InfoChamps)*(nbChampsDict+16));
      }
      
   infoChamps[indDict].facteurDeConversion = *echelleVar;
   infoChamps[indDict].indIntervalleDeDefaut =  *indDef - 1;

   strcpy(infoChamps[indDict].nomVar, nomVar);
   strcpy(infoChamps[indDict].identifVar, idVar);
   strcpy(infoChamps[indDict].unitesVar, unitesVar);
   strcpy(infoChamps[indDict].paletteVar, paletteVar);

   infoChamps[indDict].nbIntervalles = (int *)calloc(nbMenuItems, sizeof(int));
   memcpy(infoChamps[indDict].nbIntervalles, nbIntVar, sizeof(int) * nbMenuItems);

   infoChamps[indDict].intervallesDeContour = (float **)calloc(nbMenuItems, sizeof(float *));

   for (i=0; i < nbMenuItems; i++)
      {
      infoChamps[indDict].intervallesDeContour[i] = (float *)calloc(nbIntVar[i], sizeof(float));
      }
   
   for (i=0; i < nbMenuItems; i++)
      {
      for (j=0; j < nbIntVar[i]; j++)
	 {
	 infoChamps[indDict].intervallesDeContour[i][j] =  intVar[j][i];
	 }
      }
   infoChamps[indDict].nbMenuItems = nbMenuItems;

   if (indDict == nbChampsDict)
      nbChampsDict += 1;



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

DictMgrGetIdentifVar(identifVar, indDict)
char identifVar[];
int indDict;
{
   strcpy(identifVar, infoChamps[indDict].identifVar);
   }

DictMgrGetUnitesVar(unitesVar, indDict)
char unitesVar[];
int indDict;
{
   strcpy(unitesVar, infoChamps[indDict].unitesVar);
   }

DictMgrGetPaletteVar(paletteVar, indDict)
char paletteVar[];
int indDict;
{
   strcpy(paletteVar, infoChamps[indDict].paletteVar);
   }

DictMgrGetIndIntervalleDeDefaut(indIntervalle, indDict)
int *indIntervalle;
int indDict;
{
   *indIntervalle = infoChamps[indDict].indIntervalleDeDefaut;
   }

DictMgrSetIndIntervalleDeDefaut(indIntervalle, indDict)
int indIntervalle;
int indDict;
{
   infoChamps[indDict].indIntervalleDeDefaut = indIntervalle;
   }

DictMgrGetNbMenuItems(nbMenuItems, indDict)
int *nbMenuItems;
int indDict;
{
   *nbMenuItems = infoChamps[indDict].nbMenuItems;
   }

DictMgrGetFacteurDeConversion(facteur, indDict)
float *facteur;
int indDict;
{
   *facteur = infoChamps[indDict].facteurDeConversion;
   }

DictMgrSetFacteurDeConversion(facteur, indDict)
float facteur;
int indDict;
{
   infoChamps[indDict].facteurDeConversion = facteur;
   }

DictMgrGetIntervallesDeContours(intervalles, nbIntervalles, indDict, indIntervalle)
float *intervalles;
int *nbIntervalles;
int indDict;
int indIntervalle;
{
   int i;
   *nbIntervalles = infoChamps[indDict].nbIntervalles[indIntervalle];

   for (i=0; i < *nbIntervalles; i++)
      {
      intervalles[i] = infoChamps[indDict].intervallesDeContour[indIntervalle][i];
      }

   }

DictMgrSetIntervallesDeContour(intervalles, nbIntervalles, indDict, indIntervalle)
float *intervalles;
int nbIntervalles;
int indDict;
int indIntervalle;
{
   int i;

   infoChamps[indDict].nbIntervalles[indIntervalle] = nbIntervalles;

   for (i=0; i < nbIntervalles; i++)
      {
      infoChamps[indDict].intervallesDeContour[indIntervalle][i] = intervalles[i];
      }

   }

/**
********
********
**/


DictMgrGetVariableList(varlist,n,nmax)
char varlist[][5];
int *n,nmax;
{
   int i;

   if (nbChampsDict > nmax)
      {
      *n = nmax;
      }
   else
      {
      *n = nbChampsDict;
      }
  
   for (i=0; i < *n; i++)
      {
      strcpy(varlist[i], infoChamps[i].nomVar);
      }

   }

DictMgrSetMinMaxMode(nomvar,pos,mode)
char nomvar[];
int pos,mode;
{
   if (0 == strcmp(nomvar, infoChamps[pos].nomVar))
      {
      infoChamps[pos].minmaxSource = mode;
      }
   
   }

DictMgrSetMinMaxValues(nomvar,pos,min,max)
char nomvar[];
int pos;
float min,max;
{
   if (0 == strcmp(nomvar, infoChamps[pos].nomVar))
      {
      infoChamps[pos].userMin = min * infoChamps[pos].facteurDeConversion;
      infoChamps[pos].userMax = max * infoChamps[pos].facteurDeConversion;
      }
   
   }

DictMgrGetMinMaxValues(nomvar,min,max)
char nomvar[];
float *min,*max;
{
   int pos;
   pos = ChercherNomVar(nomvar);
   *min = infoChamps[pos].userMin;
   *max = infoChamps[pos].userMax;
   
   }


DictMgrGetMinMaxMode(nomvar)
char nomvar[];
{
   int pos;
   pos = ChercherNomVar(nomvar);
   return infoChamps[pos].minmaxSource;
   }








