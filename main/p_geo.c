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
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/Separator.h>

#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>


#include <xinit.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>

extern SuperWidgetStruct SuperWidget;
extern int facteurLissage;
extern GeoMapFlagsStruct   mapFlags;


#define OUI 1
#define NON 0

static int resolution = 50;
static int espacementMeridiens = 10;
int epaisseur = 1;
int couleurGeo = GRIS;
int frontieresActives = OUI;
int meridiens = OUI;

char panneauGeoGeometrie[32];


Widget pgTopLevel = NULL;
Widget pgForme, pgFormeChamps, pgFormeAttributs, pgFrameChamps, pgFrameAttributs, pgRC, pgAfficher, pgOk;
Widget pgPanneauStyle, pgCouleur, pgPanneauCouleur, pgPanneauEpaisseur;
Widget pgOptionsStyle, pgOptionsCouleur, pgOptionsEpaisseur, pgOptionsActive;
Widget pgOptionsStyleItems[7], pgOptionsCouleurItems[9], pgOptionsEpaisseurItems[5];
Widget pgActive, pgOptionsActive, pgOptionsActiveItems[2];
Widget pgListeItems, pgChamps[6];
Widget pgSeparateur1,pgSeparateur2;
Widget pgPanneauContours,pgOptionsContours,pgContourItems[3];
Widget pgPanneauLabels,pgOptionsLabels,pgLabelItems[3];
Widget pgPanneauValeursCentrales,pgOptionsValeursCentrales,pgValeurItems[3];
Widget pgPanneauTailleValeursCentrales,pgOptionsTailleValeursCentrales,pgTailleValeurItems[13];
Widget pgRowCol, pgToggleBox, pgToggleItems[14];

static char *nomPanneauContour[] = {"PanneauGeo", "GeoPanel"};
static char *labelChamps[][14] = { 
{"Continents", "Lat-lon", "Pays", 
 "Provinces", "Villes", "Lacs", "Rivieres", "Routes", "Rails", 
 "Utilites", "Canaux", "Topographie", "Bathymetrie", "Type de Terrain"},
{"Continents", "Lat-lon", "Countries","Provinces", "Cities", "Lakes", "Rivers", "Roads", "Rails", 
   "Utilities", "Canals", "Topography", "Bathymetry", "Terrain Type"}};


static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Refresh"};


#define COULEUR   0
#define EPAISSEUR 1
#define STYLE     2



static char *labelStyle[] = {"Style     ", "Style     "};
static char *labelCouleur[] = {"Couleur   ", "Color     "};
static char *labelEpaisseur[] = {"Epaisseur \ndes lignes", "Line      \nThickness "};

static char *pgLabelOptionsStyle[]= { "--------------------", 
				      "--  --  --  --  --  ", 
				      "----    ----   -----", 
				      "--------      ------"}; 

static char *pgLabelOptionsCouleur[][9] = {{"blanc               ", "noir","rouge","cyan","jaune","magenta","vert","bleu","gris"},
					    {"white               ", "black","red","cyan","yellow","magenta","green","blue", "gray"}};
static char *pgLabelOptionsEpaisseur[]= {"1                   ", "2", "3", "4", "5"};

int currentGeoToggle = CONTINENTS;
int pgSelectionTerminee;

void CheckGeoToggles (Widget w, caddr_t client_data, caddr_t call_data) 
{
  Arg args[3];
  int i, pos, toggleChanged;
  Pixel back, fore;
  static int last_pos = -1;
  
  pos = (int)client_data;
  
  currentGeoToggle=pos;
  toggleChanged = 0;
  
  XtSetArg(args[0], XmNbackground, &back);
  XtSetArg(args[1], XmNforeground, &fore);
  if (pos == last_pos)
    {
      toggleChanged = 1;
      XtGetValues(pgToggleItems[(pos+1)%8], args, 2);
    } 
  
  else
    {
      XtGetValues(w, args, 2);
    }
  
  
  /*
  XtSetArg(args[2], XmNindicatorOn, XmINDICATOR_CHECK_BOX);
  XtSetArg(args[3], XmNselectColor, XmREVERSED_GROUND_COLORS);
  */
  
  XtSetArg(args[0], XmNbackground, back);
  XtSetArg(args[1], XmNforeground, fore);
  for (i=0; i < 8; i++)
    {
      XtSetValues(pgToggleItems[i], args, 2);
    }

  XtSetArg(args[0], XmNbackground, fore);
  XtSetArg(args[1], XmNforeground, back);
  XtSetValues(w, args, 2);
  
  /*
  XtSetArg(args[0], XmNbackground, indCouleurs[BLANC]);

  XtSetArg(args[1], XmNindicatorOn, XmINDICATOR_CHECK_BOX);
  XtSetArg(args[2], XmNselectColor, XmREVERSED_GROUND_COLORS);
  XtSetValues(w, args, 1);  */

  
  XtSetArg(args[0], XmNbackground, &back);
  XtSetArg(args[1], XmNforeground, &fore);
  XtGetValues(pgOptionsCouleurItems[mapFlags.indCouleur[currentGeoToggle]], args, 2);
  
  XtSetArg(args[0], XmNbackground, back);
  XtSetArg(args[1], XmNforeground, fore);
  XtSetValues(pgPanneauCouleur, args, 2);
  
  XtSetArg(args[0], XmNmenuHistory, pgOptionsCouleurItems[mapFlags.indCouleur[currentGeoToggle]]);
  XtSetValues(pgPanneauCouleur, args, 1);
  
  XtSetArg(args[0], XmNmenuHistory, pgOptionsEpaisseurItems[mapFlags.epaisseur[currentGeoToggle]-1]);
  XtSetValues(pgPanneauEpaisseur, args, 1);
  
  
  XtSetArg(args[0], XmNmenuHistory, pgOptionsStyleItems[mapFlags.style[currentGeoToggle]]);
  XtSetValues(pgPanneauStyle, args, 1);
  
  if (pos == last_pos)
    {
      if (mapFlags.etat[currentGeoToggle] == OUI)
	{
	  XtSetArg (args[0], XmNset, False);
	  XtSetValues(pgToggleItems[currentGeoToggle], args, 1);
	  mapFlags.etat[currentGeoToggle] = NON;
	}
      else
	{
	  XtSetArg (args[0], XmNset, True);
	  XtSetValues(pgToggleItems[currentGeoToggle], args, 1);
	  mapFlags.etat[currentGeoToggle] = OUI;
	  mapFlags.lu[currentGeoToggle] = NON;
	}
      mapFlags.verifStatutNecessaire = OUI;
     RedessinerFenetres();
    }
  else
    {
      if (mapFlags.etat[currentGeoToggle] == OUI)
	{
	  XtSetArg (args[0], XmNset, True);
	  XtSetValues(pgToggleItems[currentGeoToggle], args, 1);
	}
      else
	{
	  XtSetArg (args[0], XmNset, False);
	  XtSetValues(pgToggleItems[currentGeoToggle], args, 1);
	}
    }
  last_pos = pos;
}

/****
***********************************************************************
***********************************************************************
****/

void SetGeoColorToggle (Widget w, caddr_t client_data, caddr_t call_data) 
{
  Arg args[2];
  Pixel back, fore;
  
  XtSetArg(args[0], XmNbackground, &back);
  XtSetArg(args[1], XmNforeground, &fore);
  XtGetValues(w, args, 2);
  
  XtSetArg(args[0], XmNbackground, back);
  XtSetArg(args[1], XmNforeground, fore);
  XtSetValues(pgPanneauCouleur, args, 2);
  
  
  mapFlags.indCouleur[currentGeoToggle] =  (int)client_data;
  
}

/****
***********************************************************************
***********************************************************************
****/

void SetGeoThicknessToggle (Widget w, caddr_t client_data, caddr_t call_data)
{
  mapFlags.epaisseur[currentGeoToggle] = atoi(XtName(w));
}

/****
***********************************************************************
***********************************************************************
****/

void SetGeoStyleToggle (Widget w, caddr_t client_data, caddr_t call_data)
{
   mapFlags.style[currentGeoToggle] = (int)client_data;

   switch ((int)client_data)
      {
      case 0:
      mapFlags.style[currentGeoToggle] = 0;
      break;
      
      case 1:
      case 2:
      case 3:
      mapFlags.style[currentGeoToggle] = 1;
      break;
      }
   }

/****
***********************************************************************
***********************************************************************
****/
 
XtCallbackProc PgOk(Widget w, caddr_t client_data, caddr_t call_data)
{
   pgSelectionTerminee = TRUE;
   DesactiverPanneauGeo();
   return 0;
   }

/****
***********************************************************************
***********************************************************************
****/

XtCallbackProc PgAfficher(Widget w, caddr_t client_data, caddr_t call_data)
{
   RedessinerFenetres();
   return 0;
   }

/***
***************************************************************************
***************************************************************************
***/

void InitPanneauGeo()
{
   int i;
   Arg args[6];
   XmString string;
   char *gdb_path;

   int n,lng;
   char nomShell[128];
   Pixel indCouleurs[16];
   
   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauContour[lng]);
   i = 0;
   pgTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);
   i = 0;
   pgForme = (Widget) XmCreateForm(pgTopLevel, "form", args, i);
   XtManageChild(pgForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pgOk = (Widget)XmCreatePushButton(pgForme, labelOk[lng], args, i);
   XtAddCallback(pgOk, XmNactivateCallback, (XtCallbackProc)  PgOk, NULL);
   XtManageChild(pgOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pgOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pgAfficher = (Widget)XmCreatePushButton(pgForme, labelAfficher[lng], args, i);
   XtAddCallback(pgAfficher, XmNactivateCallback, (XtCallbackProc)  PgAfficher, NULL);
   XtManageChild(pgAfficher);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pgOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pgFrameChamps = (Widget) XmCreateFrame(pgForme, "form", args, i);
   XtManageChild(pgFrameChamps);

   i = 0;
   pgFormeChamps = (Widget) XmCreateForm(pgFrameChamps, "form", args, i);
   XtManageChild(pgFormeChamps);

   pgRowCol = XmCreateRowColumn (pgFormeChamps, "rowcolumn", args, i);
   i = 0; 
   XtSetArg (args[i], XmNpacking, XmPACK_COLUMN); i++; 
   XtSetArg (args[i], XmNnumColumns, 1); i++; 
   pgToggleBox = XmCreateRowColumn (pgRowCol, "togglebox", args, i); 
   /* simply loop through the strings creating a widget for each one */ 

   gdb_path = (char *) getenv("GDB_PATH");

   for (i = 0; i < 8; i++) 
     { 
     /*
       XtSetArg(args[0], XmNindicatorOn, XmINDICATOR_CHECK_BOX);
       XtSetArg(args[1], XmNselectColor, XmREVERSED_GROUND_COLORS);
     */
     XtSetArg (args[0], XmNvisibleWhenOff, False);
     pgToggleItems[i] = (Widget) XmCreateToggleButton(pgToggleBox, labelChamps[lng][i], args, 1); 
     XtAddCallback (pgToggleItems[i], XmNvalueChangedCallback, (XtCallbackProc) CheckGeoToggles, (XtPointer) i); 
     XtManageChild (pgToggleItems[i]); 
     }
   XtManageChild(pgToggleBox);
   XtManageChild(pgRowCol);
   
   if (gdb_path == NULL)
     {
     XtSetArg (args[0], XmNsensitive, False);
     XtSetValues(pgToggleItems[VILLES], args, 1);
     XtSetValues(pgToggleItems[LACS], args, 1);
     XtSetValues(pgToggleItems[RIVIERES], args, 1);
     XtSetValues(pgToggleItems[ROUTES], args, 1);
     mapFlags.etat[VILLES]=NON;
     mapFlags.etat[LACS]=NON;
     mapFlags.etat[RIVIERES]=NON;
     mapFlags.etat[ROUTES]=NON;
     }

   
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pgOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pgFrameChamps); i++;
   pgFormeAttributs = (Widget) XmCreateForm(pgForme, "form", args, i);
   XtManageChild(pgFormeAttributs);

   i = 0;
   pgFrameAttributs = (Widget) XmCreateFrame(pgFormeAttributs, "form", args, i);
   XtManageChild(pgFrameAttributs);

/* Create RowColumn in pgTopLevel */

   i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pgRC = XmCreateRowColumn(pgFrameAttributs, "pgRC", args, i);
   XtManageChild(pgRC);

/**
****
**/
   InitPixelsCouleursDeBase(indCouleurs);

   i = 0;
   XtSetArg(args[i], XmNbackground, indCouleurs[0]); i++;
   pgOptionsCouleur = (Widget)XmCreatePulldownMenu(pgRC, labelCouleur[lng], args, i);
   
   for (n=0; n < XtNumber(pgLabelOptionsCouleur[lng]); n++)
	{
	i = 0;
	XtSetArg(args[i], XmNbackground, indCouleurs[n]); i++;
	switch(n)
	   {
	   case JAUNE:
	   case BLANC:
	   case CYAN:
	   XtSetArg(args[i], XmNforeground, indCouleurs[NOIR]); i++;
	   break;

	   default:
	   XtSetArg(args[i], XmNforeground, indCouleurs[BLANC]); i++;
	   break;
	   }
	   
	pgOptionsCouleurItems[n] = (Widget) XmCreatePushButton(pgOptionsCouleur, pgLabelOptionsCouleur[lng][n], args, i);
	XtAddCallback(pgOptionsCouleurItems[n], XmNactivateCallback, (XtCallbackProc)  SetGeoColorToggle, (XtPointer) n);
	}

   XtManageChildren(pgOptionsCouleurItems, XtNumber(pgLabelOptionsCouleur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelCouleur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsCouleur); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsCouleurItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgPanneauCouleur = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   

   i = 0;
   XtSetArg(args[i], XmNbackground, indCouleurs[NOIR]); i++;
   XtSetArg(args[i], XmNforeground, indCouleurs[BLANC]); i++;
   XtSetValues(XmOptionLabelGadget(pgPanneauCouleur), args, i);

   XtManageChild(pgPanneauCouleur);

/**
****
**/

   pgOptionsEpaisseur = (Widget)XmCreatePulldownMenu(pgRC, labelEpaisseur[lng], NULL, 0);

   for (n=0; n < XtNumber(pgLabelOptionsEpaisseur[lng]); n++)
	{
	i = 0;
	pgOptionsEpaisseurItems[n] = XmCreatePushButtonGadget(pgOptionsEpaisseur, pgLabelOptionsEpaisseur[n], args, i);
	XtAddCallback(pgOptionsEpaisseurItems[n], XmNactivateCallback, (XtCallbackProc)  SetGeoThicknessToggle, (XtPointer) n);
	}

   XtManageChildren(pgOptionsEpaisseurItems, XtNumber(pgLabelOptionsEpaisseur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelEpaisseur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsEpaisseur); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsEpaisseurItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgPanneauEpaisseur = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pgPanneauEpaisseur);

/**
**
**/

   pgOptionsStyle = (Widget)XmCreatePulldownMenu(pgRC, labelStyle[lng], NULL, 0);

   for (n=0; n < XtNumber(pgLabelOptionsStyle[lng]); n++)
     {
     i = 0;
     string = XmStringCreateLtoR(pgLabelOptionsStyle[n], XmSTRING_DEFAULT_CHARSET); 
     XtSetArg(args[i], XmNlabelString, string); i++;
     pgOptionsStyleItems[n] = XmCreatePushButtonGadget(pgOptionsStyle, pgLabelOptionsStyle[n], args, i);
     XmStringFree(string);   
     XtAddCallback(pgOptionsStyleItems[n], XmNactivateCallback, (XtCallbackProc)  SetGeoStyleToggle, (XtPointer) n);
     }
   
   XtManageChildren(pgOptionsStyleItems, XtNumber(pgLabelOptionsStyle[lng]));
   
   i = 0;
   string = XmStringCreateLtoR(labelStyle[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pgOptionsStyle); i++;
   XtSetArg(args[i], XmNmenuHistory, pgOptionsStyleItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pgPanneauStyle = XmCreateOptionMenu(pgRC, "option_menu1", args, i);
   XmStringFree(string);   
   
   XtManageChild(pgPanneauStyle);
   
   /**
    **
    **/
   
   n = 0;
   pgSeparateur1 =  (Widget) XmCreateSeparator(pgRC, "sep 1", args, n);
   XtManageChild(pgSeparateur1);

   
   XtSetArg (args[0], XmNset, True);
   for (i=0; i < 8; i++)
     {
     if (mapFlags.etat[i] == OUI)
       {
       XtSetValues(pgToggleItems[i], args, 1);
       }
     }
   
   }

/****
***********************************************************************
***********************************************************************
****/


void EcrGeoAtr(FILE *fichierDemarrage)
{
   }


/****
***********************************************************************
***********************************************************************
****/

void PGSplitItem(int *attribut, int *ind, char *item)
{
   
   }

void ActiverPanneauGeo()
{

   if (!pgTopLevel)
      InitPanneauGeo();
   
   if (!XtIsRealized(pgTopLevel))
      {
      XtRealizeWidget(pgTopLevel);
      CheckColormap(pgTopLevel);
      }
   
   f77name(xpangact)();
   
   }

void f77name(xpangact)()
{
   LocalEventLoop(pgTopLevel);
   }


void DesactiverPanneauGeo()
{
   XtUnrealizeWidget(pgTopLevel);
   }



XtCallbackProc PgResolution(Widget w, caddr_t unused1, caddr_t unused2)
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
   return 0;
   }

XtCallbackProc PgMeridiens(Widget w, caddr_t unused1, caddr_t unused2)
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
   return 0;
   }

XtCallbackProc PgActive(Widget w, caddr_t unused1, caddr_t unused2)
{
   
   switch ((int)unused1)
      {
      case 0:
      PgSetFrontieresActives(OUI);
      break;
      
      case 1:
      PgSetFrontieresActives(NON);
      break;
      }
   return 0;
   }

XtCallbackProc PgEspaceMeridiens(Widget w, caddr_t unused1, caddr_t unused2)
{
   float grid;

   sscanf(XtName(w), "%f", &grid);
   PgSetEspacementMeridiens((int)grid);
   return 0;
   }

XtCallbackProc PgEpaisseur(Widget w, caddr_t unused1, caddr_t unused2)
{
   int epaisseur;

   sscanf(XtName(w), "%d", &epaisseur);
   PgSetEpaisseur(epaisseur);
   return 0;
   }


void f77name(c_sgeoatr)(char item[],char valeur[], int lenItem, int lenValeur)
{

   }
      
         


void PgSetMeridiens(int valeur)
{
   char valeurStr[8];

   meridiens = valeur;
   if (meridiens == OUI)
      strcpy(valeurStr,"OUI");
   else
      strcpy(valeurStr,"NON");
      
   c_gmpopts("MERIDIENS", valeurStr);

   }

void PgSetFrontieresActives(int valeur)
{
   frontieresActives = valeur;

   }

void PgSetEpaisseur(int valeur)
{
   epaisseur = valeur;
   c_gmpopti("OUTLINE_THICK", epaisseur);
   c_gmpopti("GRID_THICK", epaisseur);
   
   }

void PgSetEspacementMeridiens(int valeur)
{
   espacementMeridiens = valeur;
   c_gmpopti("GRID", espacementMeridiens);
   }

void PgSetResolution(int valeur)
{
   resolution = valeur;
   c_gmpopti("RESOLUTION",resolution);
   }

