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

#include <Xm/Xm.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>

#include <xinit.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl.h>
#include <rpnmacros.h>

#define OUI 1
#define NON 0

extern SuperWidgetStruct SuperWidget;
extern _XContour   xc;

static XtCallbackProc PgCouleur();
static XtCallbackProc PgResolution();
static XtCallbackProc PgOk();
static XtCallbackProc PgAfficher();
static XtCallbackProc PgMeridiens();
static XtCallbackProc PgPolitique();
static XtCallbackProc PgEspaceMeridiens();
static XtCallbackProc PgEpaisseur();

Widget pgTopLevel = NULL;
Widget pgForme, pgFrame, pgRC, pgAfficher, pgOk;
Widget pgResolution, pgCouleur, pgEpaisseur, pgMeridiens, pgPolitique, pgEspaceMeridiens;
Widget pgOptionsResolution, pgOptionsCouleur, pgOptionsEpaisseur, pgOptionsMeridiens, pgOptionsPolitique, pgOptionsEspaceMeridiens;
Widget pgOptionsResolutionItems[9], pgOptionsCouleurItems[8], pgOptionsEpaisseurItems[5], pgOptionsMeridiensItems[2];
Widget pgOptionsPolitiqueItems[2], pgOptionsEspaceMeridiensItems[7];
int pgSelectionTerminee = False;

static int resolution = 50;
static int espacementMeridiens = 10;
int epaisseur = 1;
int couleurGeo = GRIS;
int frontieresPolitiques = OUI;
int meridiens = OUI;

char panneauGeoGeometrie[32];

   static char *nomPanneauGeo[] = {"PanneauGeo", "GeoPanel"};
   static char *labelTopLevel[] = {"Geographie", "Geography"};
   static char *labelOk[] = {"Fermer", "Close"};
   static char *labelAfficher[] = {"Redessiner", "Redraw"};

   static char *labelResolution[] = {"Resolution\n(degres)  ", "Resolution\n(degrees) "};
   static char *labelCouleur[] = {"Couleur   ", "Color     "};
   static char *labelEpaisseur[] = {"Epaisseur \ndes lignes", "Line      \nEpaisseur "};
   static char *labelMeridiens[] = {"Meridiens ", "Meridiens "};
   static char *labelPolitique[] = {"Frontieres\npolitiques", "Political \nBoundaries"};
   static char *labelEspaceMeridiens[] = {"Espacement\nmeridiens", "Meridian  \ngrid      "};

   static char *labelOptionsResolution[][10] = {{"Maximum","0.01   ", "0.05", "0.1", "0.2", "0.4", "0.5", "0.75", "1.0", "2.0"}, 
						  {"Maximum","0.01   ", "0.05", "0.1", "0.2", "0.4", "0.5", "0.75", "1.0", "2.0"}};
   static char *labelOptionsCouleur[][9] = {{"blanc", "noir", "rouge", "cyan", "jaune", "magenta", "vert", "bleu", "gris"},
						{"white", "black", "red", "cyan", "yellow", "magenta", "green", "blue", "gray"}};
   static char *labelOptionsEpaisseur[][5] = {{"1      ", "2", "3", "4", "5"}, {"1      ", "2", "3", "4", "5"}};
   static char *labelOptionsMeridiens[][2] = {{"Oui    ", "Non"},{"Yes    ", "No"}};
   static char *labelOptionsPolitique[][2] = {{"Oui    ", "Non"},{"Yes    ", "No"}};
   static char *labelOptionsEspaceMeridiens[][8] = {{"5.0    ", "10.0", "15.0", "20.0", "30.0", "45.0", "60.0", "90.0"},
						       {"5.0    ", "10.0", "15.0", "20.0", "30.0", "45.0", "60.0","90.0"}};


void InitPanneauGeo()
{
   int i,n;
   Arg args[6];
   XmString string;
   Pixel indCouleurs[16];
   char nomShell[128];

   float ftemp;
   int itemp;
   int indItem;
 
   int lng;
   Colormap cmap;

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauGeo[lng]);
   pgTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);

   pgForme = (Widget) XmCreateForm(pgTopLevel, "form", NULL, 0);
   XtManageChild(pgForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pgOk = (Widget)XmCreatePushButton(pgForme, labelOk[lng], args, i);
   XtAddCallback(pgOk, XmNactivateCallback, PgOk, NULL);
   XtManageChild(pgOk);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pgOk); i++;
   pgAfficher = (Widget)XmCreatePushButton(pgForme, labelAfficher[lng], args, i);
   XtAddCallback(pgAfficher, XmNactivateCallback, PgAfficher, NULL);
   XtManageChild(pgAfficher);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pgOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pgFrame = (Widget) XmCreateFrame(pgForme, "form", args, i);
   XtManageChild(pgFrame);

/* Create RowColumn in pgTopLevel */

   i = 0;
   XtSetArg(args[i], XmNnumColumns, 3); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pgRC = XmCreateRowColumn(pgFrame, "pgRC", args, i);
   XtManageChild(pgRC);

   pgOptionsResolution = (Widget)XmCreatePulldownMenu(pgRC, labelResolution[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsResolution[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(labelOptionsResolution[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pgOptionsResolutionItems[n] = XmCreatePushButtonGadget(pgOptionsResolution, labelOptionsResolution[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(pgOptionsResolutionItems[n], XmNactivateCallback, PgResolution, labelOptionsResolution[lng][n]);
	}

   XtManageChildren(pgOptionsResolutionItems, XtNumber(labelOptionsResolution[lng]));

   indItem = 5;
   for (n=0; n < 9; n++)
      {
      sscanf(labelOptionsResolution[0][n],"%f",&ftemp);
      if (resolution == (int)(ftemp*100+0.5))
         {
         indItem = n;
         }
      }

   i = 0;
   string = XmStringCreateLtoR(labelResolution[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsResolution); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsResolutionItems[indItem]); i++;
   pgResolution = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pgResolution);

/**
****
**/

   pgOptionsCouleur = (Widget)XmCreatePulldownMenu(pgRC, labelCouleur[lng], NULL, 0);

   InitPixelsCouleursDeBase(indCouleurs);

   for (n=0; n < XtNumber(labelOptionsCouleur[lng]); n++)
	{
	i = 0;
	XtSetArg(args[i], XmNbackground, indCouleurs[n]); i++;
	switch(n)
	   {
	   case JAUNE:
	   case BLANC:
	   case GRIS:
	   case CYAN:
	   XtSetArg(args[i], XmNforeground, indCouleurs[NOIR]); i++;
	   break;

	   default:
	   XtSetArg(args[i], XmNforeground, indCouleurs[BLANC]); i++;
	   break;
	   }
	   
	pgOptionsCouleurItems[n] = (Widget)XmCreatePushButton(pgOptionsCouleur, labelOptionsCouleur[lng][n], args, i);
	XtAddCallback(pgOptionsCouleurItems[n], XmNactivateCallback, PgCouleur, labelOptionsCouleur[lng][n]);
	}

   XtManageChildren(pgOptionsCouleurItems, XtNumber(labelOptionsCouleur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelCouleur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsCouleur); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsCouleurItems[couleurGeo]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgCouleur = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   

   i = 0;
   XtSetArg(args[i], XmNbackground, indCouleurs[8]); i++;
   XtSetArg(args[i], XmNforeground, indCouleurs[NOIR]); i++;
   XtSetValues(pgCouleur, args, i);


   XtManageChild(pgCouleur);

/**
****
**/

   pgOptionsEpaisseur = (Widget)XmCreatePulldownMenu(pgRC, labelEpaisseur[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsEpaisseur[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(labelOptionsEpaisseur[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pgOptionsEpaisseurItems[n] = XmCreatePushButtonGadget(pgOptionsEpaisseur, labelOptionsEpaisseur[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(pgOptionsEpaisseurItems[n], XmNactivateCallback, PgEpaisseur, labelOptionsEpaisseur[lng][n]);
	}

   XtManageChildren(pgOptionsEpaisseurItems, XtNumber(labelOptionsEpaisseur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelEpaisseur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsEpaisseur); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsEpaisseurItems[epaisseur-1]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgEpaisseur = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pgEpaisseur);

/**
****
**/

   pgOptionsMeridiens = (Widget)XmCreatePulldownMenu(pgRC, labelMeridiens[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsMeridiens[lng]); n++)
	{
	i = 0;
	pgOptionsMeridiensItems[n] = XmCreatePushButtonGadget(pgOptionsMeridiens, labelOptionsMeridiens[lng][n], args, i);
	XtAddCallback(pgOptionsMeridiensItems[n], XmNactivateCallback, PgMeridiens, (XtPointer) n);
	}

   XtManageChildren(pgOptionsMeridiensItems, XtNumber(labelOptionsMeridiens[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelMeridiens[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsMeridiens); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsMeridiensItems[!meridiens]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgMeridiens = XmCreateOptionMenu(pgRC, labelMeridiens[lng], args, i);
   XmStringFree(string);   

   XtManageChild(pgMeridiens);

/**
****
**/

   pgOptionsPolitique = (Widget)XmCreatePulldownMenu(pgRC, labelPolitique[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsPolitique[lng]); n++)
	{
	i = 0;
	pgOptionsPolitiqueItems[n] = XmCreatePushButtonGadget(pgOptionsPolitique, labelOptionsPolitique[lng][n], args, i);
	XtAddCallback(pgOptionsPolitiqueItems[n], XmNactivateCallback, PgPolitique, (XtPointer) n);
	}

   XtManageChildren(pgOptionsPolitiqueItems, XtNumber(labelOptionsPolitique[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelPolitique[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsPolitique); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsPolitiqueItems[!frontieresPolitiques]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgPolitique = XmCreateOptionMenu(pgRC, labelPolitique[lng], args, i);
   XmStringFree(string);   

   XtManageChild(pgPolitique);

/**
****
**/

   pgOptionsEspaceMeridiens = (Widget)XmCreatePulldownMenu(pgRC, labelEspaceMeridiens[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsEspaceMeridiens[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(labelOptionsEspaceMeridiens[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pgOptionsEspaceMeridiensItems[n] = XmCreatePushButtonGadget(pgOptionsEspaceMeridiens, labelOptionsEspaceMeridiens[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(pgOptionsEspaceMeridiensItems[n], XmNactivateCallback, PgEspaceMeridiens, labelOptionsEspaceMeridiens[lng][n]);
	}

   XtManageChildren(pgOptionsEspaceMeridiensItems, XtNumber(labelOptionsEspaceMeridiens[lng]));

   indItem = 1;
   for (n=0; n < 7; n++)
      {
      sscanf(labelOptionsEspaceMeridiens[0][n],"%f",&ftemp);
      if (espacementMeridiens == (int)(ftemp+0.5))
         {
         indItem = n;
         }
      }


   i = 0;
   string = XmStringCreateLtoR(labelEspaceMeridiens[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsEspaceMeridiens); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsEspaceMeridiensItems[indItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgEspaceMeridiens = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pgEspaceMeridiens);
   
   
   
   }


ActiverPanneauGeo()
{
   XEvent pgEvent;
   Widget pgWidgetParent;
   

   Colormap cmap;
   Arg args[2];
   int i;


   if (!pgTopLevel)
      InitPanneauGeo();
   
   if (!XtIsRealized(pgTopLevel))
      {
      XtRealizeWidget(pgTopLevel);
      CheckColormap(pgTopLevel);
      }
   
   f77name(xpangact)();
   
   }

f77name(xpangact)()
{
   LocalEventLoop(pgTopLevel);
   }


DesactiverPanneauGeo()
{
   XtUnrealizeWidget(pgTopLevel);
   }


static XtCallbackProc PgOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   pgSelectionTerminee = TRUE;
   DesactiverPanneauGeo();
   }

static XtCallbackProc PgAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   RedessinerFenetreAffichage();
   }

static XtCallbackProc PgResolution(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   float res;
   
   if (0 == strcmp("Maximum", XtName(w)))
      {
      res = 0.0;
      }
   else
      {
      sscanf(XtName(w), "%f", &res);
      }
   PgSetResolution((int)(100.0*res+0.5));   
   }

static XtCallbackProc PgCouleur(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
  int couleur, r, g, b;
  Pixel back, fore; 
  Arg args[2];

  int indmin, indmax;

  c_wglgetcolrange(&indmin, &indmax);

  XtSetArg(args[0], XmNbackground, &back);
  XtSetArg(args[1], XmNforeground, &fore);
  XtGetValues(w, args, 2);
  
  XtSetArg(args[0], XmNbackground, back);
  XtSetArg(args[1], XmNforeground, fore);
  XtSetValues(pgCouleur, args, 2);
  
  if (0 == strcmp(XtName(w),"gris") || 0 == strcmp(XtName(w), "gray"))
     {
     if (8 <= c_wglgpl())
        {
	c_wglmco(indmax-OFFSET_GEO, 100, 100, 100);
        couleurGeo = GRIS;
        }
     else
	c_wglmco(15, 128, 128, 128);
     return;
     }
  
  
  couleur = 0;
  while (0 != strcmp(XtName(w), XtName(pgOptionsCouleurItems[couleur])))
     couleur++;
  
  couleurGeo = couleur;
#ifdef GL_WGL
  if (8 <= c_wglgpl())
     {
     c_wglgco(couleur, &r, &g, &b);
     c_wglmco(indmax-OFFSET_GEO, r, g, b);
     }
  else
     {
     c_gmpopti("GRID_COLOR", couleur);
     c_gmpopti("OUTLINE_COLOR", couleur);
     }
#else
  PgSetCouleurGeographie(couleur);
#endif
  }

static XtCallbackProc PgMeridiens(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   switch ((int)unused1)
      {
      case 0:
      PgSetMeridiens(OUI);
      break;
      
      case 1:
      PgSetMeridiens(NON);
      break;
      }
   }

static XtCallbackProc PgPolitique(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   
   switch ((int)unused1)
      {
      case 0:
      PgSetFrontieresPolitiques(OUI);
      break;
      
      case 1:
      PgSetFrontieresPolitiques(NON);
      break;
      }
   }

static XtCallbackProc PgEspaceMeridiens(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   float grid;

   sscanf(XtName(w), "%f", &grid);
   PgSetEspacementMeridiens((int)grid);
   }

static XtCallbackProc PgEpaisseur(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int epaisseur;

   sscanf(XtName(w), "%d", &epaisseur);
   PgSetEpaisseur(epaisseur);
   }


f77name(c_sgeoatr)(item,valeur,lenItem,lenValeur)
char item[],valeur[];
int lenItem,lenValeur;
{
   Arg args[10];
   int i,j;
   int indItem;
   int found;
   float res;
   char tmpItem1[32],tmpItem2[32];


   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);

   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauGeoGeometrie,valeur);
      return;
      }

   if (0 == strcmp(item, "resolution"))
      {
      sscanf(valeur,"%f",&res);
      PgSetResolution( (int) (100.0 * res));
      return;
      }
      
   if (0 == strcmp(item, "epaisseur"))
      {
      PgSetEpaisseur(atoi(valeur));
      return;
      }
      
   if (0 == strcmp(item, "frontieres_politiques"))
      {
      if (0 == strcmp(valeur,"on"))
         {
         PgSetFrontieresPolitiques(OUI);
         }
      else
         {
         PgSetFrontieresPolitiques(NON);
         }
      return;
      }
      
   if (0 == strcmp(item, "meridiens"))
      {
      if (0 == strcmp(valeur,"on"))
         {
         PgSetMeridiens(OUI);
         }
      else
         {
         PgSetMeridiens(NON);
         }
      return;
      }
      
   if (0 == strcmp(item, "espacement_meridiens"))
      {
      PgSetEspacementMeridiens(atoi(valeur));
      return;
      }
      
   if (0 == strcmp(item, "couleur"))
      {
      for (j=0; j < 9; j++)
         {
         if (0 == strcmp(valeur,labelOptionsCouleur[0][j]) || 0 == strcmp(valeur,labelOptionsCouleur[1][j]))
            {
            PgSetCouleurGeographie(j);
            }
         }
      }
   }
      
         

EcrGeoAtr(fichierDemarrage)
FILE *fichierDemarrage;
{
   char tableau[32];
   char ligne[80];
   char item[32],valeur[32];
   int i;

   Arg  args[10];
   char *geom;
   Window root;
   Position x,y;
   Display *disp;
   Window win;
   strcpy(tableau, "geographie");
   
   strcpy(item,"geometrie");
   if (pgTopLevel)
      {
      disp = XtDisplay(pgTopLevel);
      win  = XtWindow(pgTopLevel);
      i = 0;
      XtSetArg(args[i], XmNx, &x); i++;
      XtSetArg(args[i], XmNy, &y); i++;
      XtGetValues(pgTopLevel, args, i);
      
      sprintf(valeur,"%+d%+d",x,y);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   else
      {
      if (strlen(panneauGeoGeometrie) > 0)
         {
         strcpy(valeur,panneauGeoGeometrie);
         sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
         fprintf(fichierDemarrage,"%s\n",ligne);
         }
      }

   strcpy(item,"couleur");
   strcpy(valeur,labelOptionsCouleur[0][couleurGeo]);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   
   strcpy(item,"epaisseur");
   sprintf(valeur,"%2d",epaisseur);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   
   strcpy(item,"frontieres_politiques");
   if (frontieresPolitiques == OUI)
      strcpy(valeur, "on");
   else
      strcpy(valeur,"off");
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   
   
   strcpy(item,"meridiens");
   if (meridiens  == OUI)
      strcpy(valeur, "on");
   else
      strcpy(valeur,"off");
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   
   
   strcpy(item,"resolution");
   sprintf(valeur,"%6.3f",(float)(resolution*0.01));
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   strcpy(item,"espacement_meridiens");
   sprintf(valeur,"%6.0f",(float)espacementMeridiens);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   }


PgSetMeridiens(valeur)
int valeur;
{
   char valeurStr[8];

   meridiens = valeur;
   if (meridiens == OUI)
      strcpy(valeurStr,"OUI");
   else
      strcpy(valeurStr,"NON");
      
   c_gmpopts("MERIDIENS", valeurStr);

   }

PgSetFrontieresPolitiques(valeur)
int valeur;
{
   char valeurStr[8];

   frontieresPolitiques = valeur;

   if (frontieresPolitiques == OUI)
      strcpy(valeurStr,"GEOPOL");
   else
      strcpy(valeurStr,"GEO");
      
   c_gmpopts("OUTLINE", valeurStr);

   }

PgSetEpaisseur(valeur)
int valeur;
{
   epaisseur = valeur;
   c_gmpopti("OUTLINE_THICK", epaisseur);
   c_gmpopti("GRID_THICK", epaisseur);
   
   }

PgSetEspacementMeridiens(valeur)
int valeur;
{
   espacementMeridiens = valeur;
   c_gmpopti("GRID", espacementMeridiens);
   }

PgSetCouleurGeographie(valeur)
int valeur;
{
   int couleur, r, g, b;
   int indmin, indmax;

   couleurGeo = valeur;
   c_wglgetcolrange(&indmin, &indmax);
   c_wglgco(couleurGeo, &r, &g, &b);
   c_wglmco(indmax-OFFSET_GEO, r, g, b);
   c_gmpopti("GRID_COLOR", indmax-OFFSET_GEO);
   c_gmpopti("OUTLINE_COLOR", indmax-OFFSET_GEO);
   }

PgSetResolution(valeur)
int valeur;
{
   resolution = valeur;
   c_gmpopti("RESOLUTION",resolution);
   }







