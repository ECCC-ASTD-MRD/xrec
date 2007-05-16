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
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>

#include <xinit.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;
extern int facteurCouleur;

float PtValMgrGetSymbolSize();

Widget p_ptvalTopLevel = NULL;
Widget p_ptvalForme, p_ptvalFormeChamps, p_ptvalFormePonctuelles, p_ptvalFrameChamps, p_ptvalFramePonctuelles, p_ptvalRc, p_ptvalAfficher, p_ptvalOk;
Widget p_ptvalPanneauCouleur,p_ptvalFrameCouleur,p_ptvalOptionsCouleur,p_ptvalOptionsCouleurItems[2];
Widget p_ptvalPanneauSymbole,p_ptvalFrameSymbole,p_ptvalOptionsSymbole,p_ptvalOptionsSymboleItems[3];
Widget p_ptvalPanneauAffichageValeurs,p_ptvalFrameAffichageValeurs,p_ptvalOptionsAffichageValeurs,p_ptvalOptionsAffichageValeursItems[2];
Widget p_ptvalPanneauAffichageLatlon,p_ptvalFrameAffichageLatlon,p_ptvalOptionsAffichageLatlon,p_ptvalOptionsAffichageLatlonItems[2];
Widget p_ptvalPanneauUnitesSymbole,p_ptvalFrameUnitesSymbole, p_ptvalOptionsUnitesSymbole,p_ptvalOptionsUnitesSymboleItems[4];
Widget p_ptvalPanneauTailleSymbole,p_ptvalFrameTailleSymbole, p_ptvalOptionsTailleSymbole,p_ptvalOptionsTailleSymboleItems[22];
Widget p_ptvalChamps[6];

static char *nomPanneauPonctuelles[] = {"PanneauValeursPonctuelles", "PointValuesPanel"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Refresh"};

static char *labelCouleur[] =             {"Couleur:              ", "Color:            "};
static char *labelSymbole[] =             {"Type de Symbole:      ", "Symbol Type:      "};
static char *labelAffichageValeurs[] =    {"Affichage valeurs:    ", "Values Display:   "};
static char *labelAffichageLatlon[] =     {"Affichage lat-lons:   ", "Lat-lon Display:  "};
static char *labelTailleSymbole[] =       {"Taille symboles:      ", "Symbol Size:      "};
static char *labelUnitesSymbole[] =       {"Unites taille symbole:", "Symbol Size Units:"};

static char *labelOptionsCouleur[][2] =           {{"Oui             ", "Non"},{"Yes          ", "No"}};
static char *labelOptionsSymbole[][3] =           {{"croix           ", "cercle", "carre"},{"cross", "circle", "square"}};
static char *labelOptionsAffichageValeurs[][2] =  {{"Oui             ", "Non"},{"Yes          ", "No"}};
static char *labelOptionsAffichageLatlon[][2] =   {{"Oui             ", "Non"},{"Yes          ", "No"}};
static char *labelOptionsUnitesSymbole[][4] =     {{"Pixels          ", "Degres", "Km", "Milles nautiques"},{"Pixels", "Degrees", "Km", "Nautical Miles"}};
static char *labelOptionsTailleSymbole[][22] =     {{"0.1             ",  "0.2", "0.25", "0.33", "0.5", "0.75", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "20", "25", "33", "50", "75", "100"},
                                                   {"0.1             ",  "0.2", "0.25", "0.33", "0.5", "0.75", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "20", "25", "33", "50", "75", "100"}};

static int couleurSymbole = 1;
static int typeSymbole = 1;
static int affichageValeurs = 0;
static int affichageLatlon = 0;
static float tailleSymbole = 3.0;
static int unitesSymbole = 0;
static int currentItem;
static char panneauPonctuellesGeometrie[32];

static int p_ptvalSelectionTerminee;

void SetAffichageTailleSymbole(Widget w, caddr_t client_data, caddr_t call_data);

//--------------------------------------------------------------------------------------------------------------------------
void SetCouleurToggle (Widget w, caddr_t client_data, caddr_t call_data)
  {
  char *str;
  int lng;

  lng = c_getulng();

  str = XtName(w);
  if (0 == strcmp(str, labelOptionsCouleur[lng][0]))
     {
     couleurSymbole = 1;
     }
  else
     {
     couleurSymbole = 0;
     }
  RedessinerFenetres();
  }

//--------------------------------------------------------------------------------------------------------------------------
void SetSymboleToggle (Widget w, caddr_t client_data, caddr_t call_data)
{
  int lng;
  char *str;

  lng = c_getulng();

  str = XtName(w);
  if (0 == strcmp(str, labelOptionsSymbole[lng][0]))
     {
     typeSymbole = CROIX;
     }
  if (0 == strcmp(str, labelOptionsSymbole[lng][1]))
     {
     typeSymbole = CERCLE;
     }
  if (0 == strcmp(str, labelOptionsSymbole[lng][2]))
     {
     typeSymbole = RECTANGLE;
     }
    RedessinerFenetres();
   }

//--------------------------------------------------------------------------------------------------------------------------
void SetAffichageValeurToggle (Widget w, caddr_t client_data, caddr_t call_data)
{
  int lng;
  char *str;
  lng = c_getulng();

  str = XtName(w);
  if (0 == strncmp(str, labelOptionsAffichageValeurs[lng][0],strlen(str)))
     {
     affichageValeurs = 1;
     }
  else
     {
     affichageValeurs = 0;
     }
  RedessinerFenetres();
   }

//--------------------------------------------------------------------------------------------------------------------------
void SetAffichageValeurLatlon (Widget w, caddr_t client_data, caddr_t call_data)
{
   int lng;
  char *str;
 lng = c_getulng();

  str = XtName(w);
  if (0 == strncmp(str, labelOptionsAffichageLatlon[lng][0],strlen(str)))
     {
     affichageLatlon = 1;
     }
  else
     {
     affichageLatlon = 0;
     }
   RedessinerFenetres();
   }

// void SetAffichageValeurXY (Widget w, caddr_t client_data, caddr_t call_data)
// {
//    AffichageLatlon = atoi(XtName(w));
//    }

//--------------------------------------------------------------------------------------------------------------------------
void SetAffichageTailleSymbole (Widget w, caddr_t client_data, caddr_t call_data)
  {
  tailleSymbole = atof(XtName(w));
  RedessinerFenetres();
  }

//--------------------------------------------------------------------------------------------------------------------------
void SetUnitesSymbole (Widget w, caddr_t client_data, caddr_t call_data)
{
  int lng;
  char *str;
  lng = c_getulng();

  str = XtName(w);
  if (0 == strncmp(str, labelOptionsUnitesSymbole[lng][0],strlen(str)))
     {
     unitesSymbole = PIXELS;
     }
  if (0 == strcmp(str, labelOptionsUnitesSymbole[lng][1]))
     {
     unitesSymbole = DEGRES;
     }
  if (0 == strcmp(str, labelOptionsUnitesSymbole[lng][2]))
     {
     unitesSymbole = KM;
     }
  if (0 == strcmp(str, labelOptionsUnitesSymbole[lng][3]))
    {
    unitesSymbole = NAUTICAL_MILES;
    }

  RedessinerFenetres();
   }
//--------------------------------------------------------------------------------------------------------------------------
XtCallbackProc PtValOk(Widget w, caddr_t unused1, caddr_t unused2)
{
   p_ptvalSelectionTerminee = TRUE;
   DesactiverPanneauValeursPonctuelles();
   return 0;
   }

//--------------------------------------------------------------------------------------------------------------------------
XtCallbackProc PtValAfficher(Widget w, caddr_t unused1, caddr_t unused2)
{
   RedessinerFenetres();
   return 0;
   }


//--------------------------------------------------------------------------------------------------------------------------
void InitPanneauValeursPonctuelles()
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
   strcat(nomShell, nomPanneauPonctuelles[lng]);
   i = 0;

   if (0 < strlen(panneauPonctuellesGeometrie))
      {
      XtSetArg(args[i],XmNgeometry,panneauPonctuellesGeometrie);
      i++;
      }

   p_ptvalTopLevel = XtAppCreateShell(nomShell, nomShell,
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);
   i = 0;
   p_ptvalForme = (Widget) XmCreateForm(p_ptvalTopLevel, "form", args, i);
   XtManageChild(p_ptvalForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   p_ptvalOk = (Widget)XmCreatePushButton(p_ptvalForme, labelOk[lng], args, i);
   XtAddCallback(p_ptvalOk, XmNactivateCallback, (XtCallbackProc) PtValOk, NULL);
   XtManageChild(p_ptvalOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, p_ptvalOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   p_ptvalAfficher = (Widget)XmCreatePushButton(p_ptvalForme, labelAfficher[lng], args, i);
   XtAddCallback(p_ptvalAfficher, XmNactivateCallback, (XtCallbackProc)  PtValAfficher, NULL);
   XtManageChild(p_ptvalAfficher);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget,  p_ptvalOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   p_ptvalFormePonctuelles = (Widget) XmCreateForm(p_ptvalForme, "form", args, i);
   XtManageChild(p_ptvalFormePonctuelles);

   i = 0;
   p_ptvalFramePonctuelles = (Widget) XmCreateFrame(p_ptvalFormePonctuelles, "form", args, i);
   XtManageChild(p_ptvalFramePonctuelles);

/* Create RowColumn in p_ptvalTopLevel */

  i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   p_ptvalRc = XmCreateRowColumn(p_ptvalFramePonctuelles, "p_ptvalRc", args, i);
   XtManageChild(p_ptvalRc);

//--------------------------------------------------------------------------------------------------------------------------

   i = 0;
   p_ptvalFrameCouleur = (Widget) XmCreateFrame(p_ptvalRc, "form", args, i);
   XtManageChild(p_ptvalFrameCouleur);

   i = 0;
   p_ptvalOptionsCouleur = (Widget)XmCreatePulldownMenu(p_ptvalFrameCouleur, labelCouleur[lng], args, i);

   for (n=0; n < XtNumber(labelOptionsCouleur[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(labelOptionsCouleur[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    p_ptvalOptionsCouleurItems[n] = (Widget) XmCreatePushButtonGadget(p_ptvalOptionsCouleur, labelOptionsCouleur[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(p_ptvalOptionsCouleurItems[n], XmNactivateCallback, (XtCallbackProc)  SetCouleurToggle, (XtPointer) n);
    }

   XtManageChildren(p_ptvalOptionsCouleurItems, XtNumber(labelOptionsCouleur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelCouleur[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, p_ptvalOptionsCouleur); i++;
   XtSetArg(args[i], XmNmenuHistory, p_ptvalOptionsCouleurItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   p_ptvalPanneauCouleur = XmCreateOptionMenu(p_ptvalFrameCouleur, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(p_ptvalPanneauCouleur);

//--------------------------------------------------------------------------------------------------------------------------
   i = 0;
   p_ptvalFrameSymbole = (Widget) XmCreateFrame(p_ptvalRc, "frame", args, i);
   XtManageChild(p_ptvalFrameSymbole);

   p_ptvalOptionsSymbole = (Widget)XmCreatePulldownMenu(p_ptvalFrameSymbole, labelSymbole[lng], NULL, 0);

   for (n=0; n < 3; n++)
    {
    i = 0;
    string = XmStringCreateLtoR(labelOptionsSymbole[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    p_ptvalOptionsSymboleItems[n] = (Widget) XmCreatePushButtonGadget(p_ptvalOptionsSymbole, labelOptionsSymbole[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(p_ptvalOptionsSymboleItems[n], XmNactivateCallback, (XtCallbackProc)  SetSymboleToggle, (XtPointer) n);
    }

   XtManageChildren(p_ptvalOptionsSymboleItems, 3);

   i = 0;
   string = XmStringCreateLtoR(labelSymbole[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNmenuHistory, p_ptvalOptionsSymboleItems[1]); i++;
   XtSetArg(args[i], XmNsubMenuId, p_ptvalOptionsSymbole); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   p_ptvalPanneauSymbole = XmCreateOptionMenu(p_ptvalFrameSymbole, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(p_ptvalPanneauSymbole);

//--------------------------------------------------------------------------------------------------------------------------


   i = 0;
   p_ptvalFrameAffichageValeurs = (Widget) XmCreateFrame(p_ptvalRc, "frame", args, i);
   XtManageChild(p_ptvalFrameAffichageValeurs);

   p_ptvalOptionsAffichageValeurs = (Widget)XmCreatePulldownMenu(p_ptvalFrameAffichageValeurs, labelAffichageValeurs[lng], NULL, 0);

   for (n=0; n < 2; n++)
    {
    i = 0;
    string = XmStringCreateLtoR(labelOptionsAffichageValeurs[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    p_ptvalOptionsAffichageValeursItems[n] = (Widget)  XmCreatePushButtonGadget(p_ptvalOptionsAffichageValeurs, labelOptionsAffichageValeurs[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(p_ptvalOptionsAffichageValeursItems[n], XmNactivateCallback, (XtCallbackProc)  SetAffichageValeurToggle, (XtPointer) n);
    }

   XtManageChildren(p_ptvalOptionsAffichageValeursItems, 2);


   i = 0;
   string = XmStringCreateLtoR(labelAffichageValeurs[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, p_ptvalOptionsAffichageValeurs); i++;
   XtSetArg(args[i], XmNmenuHistory, p_ptvalOptionsAffichageValeursItems[1]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   p_ptvalPanneauAffichageValeurs = XmCreateOptionMenu(p_ptvalFrameAffichageValeurs, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(p_ptvalPanneauAffichageValeurs);

//--------------------------------------------------------------------------------------------------------------------------


   i = 0;
   p_ptvalFrameAffichageLatlon = (Widget) XmCreateFrame(p_ptvalRc, "frame", args, i);
   XtManageChild(p_ptvalFrameAffichageLatlon);

   p_ptvalOptionsAffichageLatlon = (Widget)XmCreatePulldownMenu(p_ptvalFrameAffichageLatlon, labelAffichageLatlon[lng], NULL, 0);

   for (n=0; n < 2; n++)
    {
    i = 0;
    string = XmStringCreateLtoR(labelOptionsAffichageLatlon[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    p_ptvalOptionsAffichageLatlonItems[n] = (Widget) XmCreatePushButtonGadget(p_ptvalOptionsAffichageLatlon, labelOptionsAffichageLatlon[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(p_ptvalOptionsAffichageLatlonItems[n], XmNactivateCallback, (XtCallbackProc)  SetAffichageValeurLatlon, (XtPointer) n);
    }

   XtManageChildren(p_ptvalOptionsAffichageLatlonItems, 2);


   i = 0;
   string = XmStringCreateLtoR(labelAffichageLatlon[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, p_ptvalOptionsAffichageLatlon); i++;
   XtSetArg(args[i], XmNmenuHistory, p_ptvalOptionsAffichageLatlonItems[1]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   p_ptvalPanneauAffichageLatlon = XmCreateOptionMenu(p_ptvalFrameAffichageLatlon, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(p_ptvalPanneauAffichageLatlon);


//--------------------------------------------------------------------------------------------------------------------------
   i = 0;
   p_ptvalFrameTailleSymbole = (Widget) XmCreateFrame(p_ptvalRc, "frame", args, i);
   XtManageChild(p_ptvalFrameTailleSymbole);

   p_ptvalOptionsTailleSymbole = (Widget)XmCreatePulldownMenu(p_ptvalFrameTailleSymbole, labelTailleSymbole[lng], NULL, 0);

   for (n=0; n < 22; n++)
    {
    i = 0;
    string = XmStringCreateLtoR(labelOptionsTailleSymbole[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    p_ptvalOptionsTailleSymboleItems[n] = (Widget) XmCreatePushButtonGadget(p_ptvalOptionsTailleSymbole, labelOptionsTailleSymbole[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(p_ptvalOptionsTailleSymboleItems[n], XmNactivateCallback, (XtCallbackProc)  SetAffichageTailleSymbole, (XtPointer) n);
    }

   XtManageChildren(p_ptvalOptionsTailleSymboleItems, 22);

   i = 0;
   string = XmStringCreateLtoR(labelTailleSymbole[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, p_ptvalOptionsTailleSymbole); i++;
   XtSetArg(args[i], XmNmenuHistory, p_ptvalOptionsTailleSymboleItems[8]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   p_ptvalPanneauTailleSymbole = XmCreateOptionMenu(p_ptvalFrameTailleSymbole, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(p_ptvalPanneauTailleSymbole);

//--------------------------------------------------------------------------------------------------------------------------
   i = 0;
   p_ptvalFrameUnitesSymbole = (Widget) XmCreateFrame(p_ptvalRc, "frame", args, i);
   XtManageChild(p_ptvalFrameUnitesSymbole);

   p_ptvalOptionsUnitesSymbole = (Widget)XmCreatePulldownMenu(p_ptvalFrameUnitesSymbole, labelUnitesSymbole[lng], NULL, 0);

   for (n=0; n < 4; n++)
    {
    i = 0;
    string = XmStringCreateLtoR(labelOptionsUnitesSymbole[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    p_ptvalOptionsUnitesSymboleItems[n] = (Widget) XmCreatePushButtonGadget(p_ptvalOptionsUnitesSymbole, labelOptionsUnitesSymbole[lng][n], args, i);
    XmStringFree(string);
  XtAddCallback(p_ptvalOptionsUnitesSymboleItems[n], XmNactivateCallback, (XtCallbackProc)  SetUnitesSymbole, (XtPointer) n);
    }

   XtManageChildren(p_ptvalOptionsUnitesSymboleItems, 4);

   i = 0;
   string = XmStringCreateLtoR(labelUnitesSymbole[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, p_ptvalOptionsUnitesSymbole); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   p_ptvalPanneauUnitesSymbole = XmCreateOptionMenu(p_ptvalFrameUnitesSymbole, "option_menu1", args, i);
   XmStringFree(string);

   XtManageChild(p_ptvalPanneauUnitesSymbole);
  }

//--------------------------------------------------------------------------------------------------------------------------
void ActiverPanneauValeursPonctuelles()
{
   if (!p_ptvalTopLevel)
      InitPanneauValeursPonctuelles();

   p_ptvalSelectionTerminee = False;

   if (!XtIsRealized(p_ptvalTopLevel))
      {
      XtRealizeWidget(p_ptvalTopLevel);
      CheckColormap(p_ptvalTopLevel);
      }

   f77name(xp_ptvalact)();

   }

//--------------------------------------------------------------------------------------------------------------------------
void f77name(xp_ptvalact)()
{
   LocalEventLoop(p_ptvalTopLevel);
   }


//--------------------------------------------------------------------------------------------------------------------------
void DesactiverPanneauValeursPonctuelles()
{

   XtUnrealizeWidget(p_ptvalTopLevel);
   }

//--------------------------------------------------------------------------------------------------------------------------
int PtValMgrGetColorStatus()
{
   return couleurSymbole;
}

//--------------------------------------------------------------------------------------------------------------------------
int PtValMgrGetAffichageValeurs()
{
   return affichageValeurs;
}

//--------------------------------------------------------------------------------------------------------------------------
float PtValMgrGetSymbolSize()
{
   return tailleSymbole;
}

//--------------------------------------------------------------------------------------------------------------------------
int PtValMgrGetSymboleType()
{
   return typeSymbole;
}

//--------------------------------------------------------------------------------------------------------------------------
int PtValMgrGetAffichageLatlon()
{
   return affichageLatlon;
}
//--------------------------------------------------------------------------------------------------------------------------

int PtValMgrGetUnitesSymbole()
{
   return unitesSymbole;
}

