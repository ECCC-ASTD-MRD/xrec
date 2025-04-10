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
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>

#include <xinit.h>
#include <wgl.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;
extern int facteurLissage;

Widget pattTopLevel = NULL;
Widget pattForme, pattFormeChamps, pattFormeAttributs, pattFrameChamps, pattFrameAttributs, pattRc, pattAfficher, pattOk;
Widget pattFrameFonte,pattRCFonte,pattLabelFonte;
Widget pattPanneauLissage,pattFrameLissage,pattOptionsLissage,pattOptionsLissageItems[7];
Widget pattPanneauInterp,pattFrameInterp,pattOptionsInterp,pattOptionsInterpItems[3];
Widget pattPanneauFontSizeLegend,pattOptionsFontSizeLegend,pattOptionsFontSizeLegendItems[5];
Widget pattPanneauFontSizeLabels,pattOptionsFontSizeLabels,pattOptionsFontSizeLabelItems[5];
Widget pattPanneauFontSizeColorLegend,pattOptionsFontSizeColorLegend,pattOptionsFontSizeColorLegendItems[5];
Widget pattChamps[6];

static char *nomPanneauAttributs[] = {"PanneauAttributs", "AttributesPanel"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Refresh"};
static char *labelFonte[] = {"Taille police","Font Size"};

static char *labelLissage[] =             {"Niveau de lissage    ", "Smoothing factor   "};
static char *labelInterp[] =              {"Degre d'interpolation", "Interpolation Level"};
static char *labelFontSizeLegend[] =      {"Legende              ", "Legend             "};
static char *labelFontSizeLabels[] =      {"Labels               ", "Labels             "};
static char *labelFontSizeColorLegend[] = {"Legende couleurs     ", "Color Table        "};

static char *pattLabelOptionsLissage[][7] = {{"Automatique         ", "2", "4", "6", "8", "10", "Maximum"},
					      {"Automatic          ", "2", "4", "6", "8", "10", "Maximum"}};
static char *pattLabelOptionsInterp[][3] = {{"Voisin              ", "Bi-lineaire", "Bi-cubique"},
					      {"Nearest neighbor    ", "Bilinear", "Bicubic"}};
static char *pattLabelOptionsFontSize[][5] = {{"12                  ", "14", "17", "18", "24"},
					      { "12                  ", "14", "17", "18", "24"}};

int fontSizeLegend = 12;
int fontSizeColorLegend = 12;
int fontSizeLabels = 12;
int currentItem, nmax;
char panneauAttributsGeometrie[32];

int pattSelectionTerminee;
static int interpolationLevel = 1;


void SetLissageToggle(Widget w, XtPointer client_data, XtPointer call_data)
{
   char *str;
   int lng;

   lng = c_getulng();

   str = XtName(w);
   if (0 == strcmp(str, pattLabelOptionsLissage[lng][0]))
      {
      facteurLissage = -32767;
      }
   else
      {
      if (0 == strcmp(str, pattLabelOptionsLissage[lng][6]))
	 {
	 facteurLissage = 32767;
	 }
      else
	 {
	 facteurLissage = atoi(XtName(w));
	 }
      }
   }

void SetInterpolationToggle(Widget w, XtPointer client_data, XtPointer call_data)
{
   char *str;
   int lng,i,nbChampsActifs;
   _Champ *champ;
   float x1, y1, x2, y2;

   lng = c_getulng();

   str = XtName(w);
   if (0 == strcmp(str, pattLabelOptionsInterp[lng][0]))
      {
      interpolationLevel = 0;
      c_ezsetopt("interp_degree","nearest");
      }
   else
      {
      if (0 == strcmp(str, pattLabelOptionsInterp[lng][1]))
         {
         interpolationLevel = 1;
         c_ezsetopt("interp_degree","linear");
         }
      else
         {
         interpolationLevel = 3;
         c_ezsetopt("interp_degree","cubic");
         }
      }
   nbChampsActifs = FldMgrGetNbChampsActifs();
   CoupeMgrGetCoupeCoords(&x1, &y1, &x2, &y2);
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ,i);
      if (champ->coupe.coupeValide == 1)
         {
         champ->coupe.coupeValide = 0;
         FldMgrLoadVerticalXSection();
         }
      FldMgrProcessChamp(champ);
      FldMgrPreparerCoupe(champ, &x1, &y1, &x2, &y2);
      }
   RedessinerFenetres();
   }

void SetFontSizeLegendToggle(Widget w, XtPointer client_data, XtPointer call_data)
{
   fontSizeLegend = atoi(XtName(w));
   }

void SetFontSizeColorLegendToggle(Widget w, XtPointer client_data, XtPointer call_data)
{
   fontSizeColorLegend = atoi(XtName(w));
   }

void SetFontSizeLabelToggle(Widget w, XtPointer client_data, XtPointer call_data)
{
   fontSizeLabels = atoi(XtName(w));
   }

void PattOk(Widget w, XtPointer client_data, XtPointer call_data)
{
   pattSelectionTerminee = TRUE;
   DesactiverPanneauAttributs();
   return;
   }

void PattAfficher(Widget w, XtPointer client_data, XtPointer call_data)
{

   RedessinerFenetres();
   return;
   }


void InitPanneauAttributs()
{

   int i;
   Arg args[6];
   XmString string;

   int n,lng;
   char nomShell[128];

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauAttributs[lng]);
   i = 0;

   if (0 < strlen(panneauAttributsGeometrie))
      {
      XtSetArg(args[i],XmNgeometry,panneauAttributsGeometrie);
      i++;
      }

   pattTopLevel = XtAppCreateShell(nomShell, nomShell,
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);
   i = 0;
   pattForme = (Widget) XmCreateForm(pattTopLevel, "form", args, i);
   XtManageChild(pattForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pattOk = (Widget)XmCreatePushButton(pattForme, labelOk[lng], args, i);
   XtAddCallback(pattOk, XmNactivateCallback, PattOk, NULL);
   XtManageChild(pattOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pattOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pattAfficher = (Widget)XmCreatePushButton(pattForme, labelAfficher[lng], args, i);
   XtAddCallback(pattAfficher, XmNactivateCallback,  PattAfficher, NULL);
   XtManageChild(pattAfficher);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pattOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pattFormeAttributs = (Widget) XmCreateForm(pattForme, "form", args, i);
   XtManageChild(pattFormeAttributs);

   i = 0;
   pattFrameAttributs = (Widget) XmCreateFrame(pattFormeAttributs, "form", args, i);
   XtManageChild(pattFrameAttributs);

/* Create RowColumn in pattTopLevel */

  i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pattRc = XmCreateRowColumn(pattFrameAttributs, "pattRc", args, i);
   XtManageChild(pattRc);


   i = 0;
   pattFrameLissage = (Widget) XmCreateFrame(pattRc, "form", args, i);
   XtManageChild(pattFrameLissage);

   i = 0;
   pattOptionsLissage = (Widget)XmCreatePulldownMenu(pattFrameLissage, labelLissage[lng], args, i);

   for (n=0; n < XtNumber(pattLabelOptionsLissage[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pattLabelOptionsLissage[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pattOptionsLissageItems[n] = XmCreatePushButtonGadget(pattOptionsLissage, pattLabelOptionsLissage[lng][n], args, i);
	XmStringFree(string);
	XtAddCallback(pattOptionsLissageItems[n], XmNactivateCallback,  SetLissageToggle, (XtPointer) n);
	}

   XtManageChildren(pattOptionsLissageItems, XtNumber(pattLabelOptionsLissage[lng]));

   switch(facteurLissage)
      {
      case 32767:
      currentItem = 6;
      break;

      case -32767:
      currentItem = 0;
      break;

      case 2:
      currentItem = 1;
      break;

      case 4:
      currentItem = 2;
      break;

      case 6:
      currentItem = 3;
      break;

      case 8:
      currentItem = 4;
      break;

      case 10:
      currentItem = 5;
      break;
      }

   i = 0;
   string = XmStringCreateLtoR(labelLissage[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pattOptionsLissage); i++;
   XtSetArg(args[i], XmNmenuHistory, pattOptionsLissageItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pattPanneauLissage = XmCreateOptionMenu(pattFrameLissage, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(pattPanneauLissage);

   /*
   **
   **
   */

   i = 0;
   pattFrameInterp = (Widget) XmCreateFrame(pattRc, "frame", args, i);
   XtManageChild(pattFrameInterp);

   pattOptionsInterp = (Widget)XmCreatePulldownMenu(pattFrameInterp, labelInterp[lng], NULL, 0);

   nmax = XtNumber(pattLabelOptionsInterp[lng][n]);
   nmax = 3;
   for (n=0; n < nmax; n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pattLabelOptionsInterp[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pattOptionsInterpItems[n] = XmCreatePushButton(pattOptionsInterp, pattLabelOptionsInterp[lng][n], args, i);
	XmStringFree(string);
	XtAddCallback(pattOptionsInterpItems[n], XmNactivateCallback,  SetInterpolationToggle, (XtPointer) n);
	}

   XtManageChildren(pattOptionsInterpItems, 3);

   currentItem = 1;

   i = 0;
   string = XmStringCreateLtoR(labelInterp[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pattOptionsInterp); i++;
   XtSetArg(args[i], XmNmenuHistory, pattOptionsInterpItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pattPanneauInterp = XmCreateOptionMenu(pattFrameInterp, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(pattPanneauInterp);


/**
**
**/

   i = 0;
   pattFrameFonte = (Widget) XmCreateFrame(pattRc, "frame", args, i);
   XtManageChild(pattFrameFonte);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pattRCFonte = XmCreateRowColumn(pattFrameFonte, "pattRc", args, i);
   XtManageChild(pattRCFonte);

   pattLabelFonte = (Widget) XmCreateLabel(pattRCFonte, labelFonte[lng], NULL, 0);
   XtManageChild(pattLabelFonte);

   pattOptionsFontSizeLabels = (Widget)XmCreatePulldownMenu(pattRCFonte, labelFontSizeLabels[lng], NULL, 0);

   for (n=0; n < XtNumber(pattLabelOptionsFontSize[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pattLabelOptionsFontSize[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pattOptionsFontSizeLabelItems[n] = XmCreatePushButtonGadget(pattOptionsFontSizeLabels, pattLabelOptionsFontSize[lng][n], args, i);
	XmStringFree(string);
	XtAddCallback(pattOptionsFontSizeLabelItems[n], XmNactivateCallback,  SetFontSizeLabelToggle, (XtPointer) n);
	}

   XtManageChildren(pattOptionsFontSizeLabelItems, XtNumber(pattLabelOptionsFontSize[lng]));

   switch(fontSizeLabels)
      {
      case 12:
      currentItem = 0;
      break;

      case 14:
      currentItem = 1;
      break;

      case 17:
      currentItem = 2;
      break;

      case 18:
      currentItem = 3;
      break;

      case 24:
      currentItem = 4;
      break;
      }

   i = 0;
   string = XmStringCreateLtoR(labelFontSizeLabels[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pattOptionsFontSizeLabels); i++;
   XtSetArg(args[i], XmNmenuHistory, pattOptionsFontSizeLabelItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pattPanneauFontSizeLabels = XmCreateOptionMenu(pattRCFonte, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(pattPanneauFontSizeLabels);

/**
**
**/

   pattOptionsFontSizeColorLegend = (Widget)XmCreatePulldownMenu(pattRCFonte, labelFontSizeColorLegend[lng], NULL, 0);

   for (n=0; n < XtNumber(pattLabelOptionsFontSize[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pattLabelOptionsFontSize[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pattOptionsFontSizeColorLegendItems[n] = XmCreatePushButtonGadget(pattOptionsFontSizeColorLegend, pattLabelOptionsFontSize[lng][n], args, i);
	XmStringFree(string);
	XtAddCallback(pattOptionsFontSizeColorLegendItems[n], XmNactivateCallback,  SetFontSizeColorLegendToggle, (XtPointer) n);
	}

   XtManageChildren(pattOptionsFontSizeColorLegendItems, XtNumber(pattLabelOptionsFontSize[lng]));

   switch(fontSizeColorLegend)
      {
      case 12:
      currentItem = 0;
      break;

      case 14:
      currentItem = 1;
      break;

      case 17:
      currentItem = 2;
      break;

      case 18:
      currentItem = 3;
      break;

      case 24:
      currentItem = 4;
      break;
      }

   i = 0;
   string = XmStringCreateLtoR(labelFontSizeColorLegend[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pattOptionsFontSizeColorLegend); i++;
   XtSetArg(args[i], XmNmenuHistory, pattOptionsFontSizeColorLegendItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pattPanneauFontSizeColorLegend = XmCreateOptionMenu(pattRCFonte, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(pattPanneauFontSizeColorLegend);

/**
**
**/

   pattOptionsFontSizeLegend = (Widget)XmCreatePulldownMenu(pattRCFonte, labelFontSizeLegend[lng], NULL, 0);

   for (n=0; n < XtNumber(pattLabelOptionsFontSize[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pattLabelOptionsFontSize[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pattOptionsFontSizeLegendItems[n] = XmCreatePushButtonGadget(pattOptionsFontSizeLegend, pattLabelOptionsFontSize[lng][n], args, i);
	XmStringFree(string);
	XtAddCallback(pattOptionsFontSizeLegendItems[n], XmNactivateCallback,  SetFontSizeLegendToggle, (XtPointer) n);
	}

   XtManageChildren(pattOptionsFontSizeLegendItems, XtNumber(pattLabelOptionsFontSize[lng]));

   switch(fontSizeLegend)
      {
      case 12:
      currentItem = 0;
      break;

      case 14:
      currentItem = 1;
      break;

      case 17:
      currentItem = 2;
      break;

      case 18:
      currentItem = 3;
      break;

      case 24:
      currentItem = 4;
      break;
      }

   i = 0;
   string = XmStringCreateLtoR(labelFontSizeLegend[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pattOptionsFontSizeLegend); i++;
   XtSetArg(args[i], XmNmenuHistory, pattOptionsFontSizeLegendItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pattPanneauFontSizeLegend = XmCreateOptionMenu(pattRCFonte, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(pattPanneauFontSizeLegend);

   }

void ActiverPanneauAttributs()
{
   if (!pattTopLevel)
      InitPanneauAttributs();

   pattSelectionTerminee = False;

   if (!XtIsRealized(pattTopLevel))
      {
      XtRealizeWidget(pattTopLevel);
      CheckColormap(pattTopLevel);
      }

   f77name(xpattact)();

   }

void f77name(xpattact)()
{
   LocalEventLoop(pattTopLevel);
   }


void DesactiverPanneauAttributs()
{

   XtUnrealizeWidget(pattTopLevel);
   }

int AttrMgrGetFontSizeLegend()
{
   return fontSizeLegend;
}

int AttrMgrGetFontSizeColorTable()
{
   return fontSizeColorLegend;
}

int AttrMgrGetFontSizeLabels()
{
   return fontSizeLabels;
}

int AttrMgrGetInterpolationLevel()
{
   return interpolationLevel;
}

void f77name(c_satratr)(char item[], char valeur[], F2Cl flenItem, F2Cl flenValeur)
{

   int lenItem=flenItem, lenValeur=flenValeur;
   
   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);

   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauAttributsGeometrie,valeur);
      }

   if (0 == strcmp(item,"niveau_de_lissage") || 0 == strcmp(item,"smoothing_factor"))
      {
      if (0 == strncmp("automati",valeur,8))
         {
         facteurLissage =-32767;
         }
      else
         {
         if (0 == strcmp(valeur,"maximum"))
            {
            facteurLissage = 32767;
            }
         else
            {
            facteurLissage = atoi(valeur);
            }
         }
      }

   if (0 == strcmp(item,"taille_police_labels") || 0 == strcmp(item,"labels_font_size"))
      {
      fontSizeLabels = atoi(valeur);
      }

   if (0 == strcmp(item,"taille_police_legende_couleurs") || 0 == strcmp(item,"color_legend_font_size"))
      {
      fontSizeColorLegend = atoi(valeur);
      }

   if (0 == strcmp(item,"taille_police_legende") || 0 == strcmp(item,"legend_font_size"))
      {
      fontSizeLegend = atoi(valeur);
      }
}

void f77name(c_sanmatr)(char item[], char valeur[], F2Cl lenItem, F2Cl lenValeur)
{
}


void f77name(c_scpeatr)(char item[], char valeur[], F2Cl lenItem, F2Cl lenValeur)
{
}


void EcrAtrAtr(FILE *fichierDemarrage)
{
   char tableau[32];
   char ligne[80];
   char item[32],valeur[32];
   int i;

   Arg  args[10];
   Position x,y;
   Display *disp;
   Window win;

   strcpy(tableau, "attributs_divers");

   strcpy(item,"geometrie");
   if (pattTopLevel)
      {
      disp = XtDisplay(pattTopLevel);
      win  = XtWindow(pattTopLevel);
      i = 0;
      XtSetArg(args[i], XmNx, &x); i++;
      XtSetArg(args[i], XmNy, &y); i++;
      XtGetValues(pattTopLevel, args, i);

      sprintf(valeur,"%+d%+d",x,y);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   else
      {
      if (strlen(panneauAttributsGeometrie) > 0)
         {
         strcpy(valeur,panneauAttributsGeometrie);
         sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
         fprintf(fichierDemarrage,"%s\n",ligne);
         }
      }


   strcpy(item, "niveau_de_lissage");
   switch (facteurLissage)
      {
      case -32767:
      strcpy(valeur, "automatique");
      break;

      case 32767:
      strcpy(valeur, "maximum");
      break;

      default:
      sprintf(valeur, "%2d", facteurLissage);
      break;
      }
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   strcpy(item, "taille_police_labels");
   sprintf(valeur, "%2d", fontSizeLabels);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   strcpy(item, "taille_police_legende");
   sprintf(valeur, "%2d", fontSizeLegend);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   strcpy(item, "taille_police_legende_couleurs");
   sprintf(valeur, "%2d", fontSizeColorLegend);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   }
