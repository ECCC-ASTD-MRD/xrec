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
#include <c_wgl.h>
#include <rpnmacros.h>
#include <rec.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;
extern int facteurLissage;

Widget pcTopLevel = NULL;
Widget pcForme, pcFormeChamps, pcFormeAttributs, pcFrameChamps, pcFrameAttributs, pcRc, pcAfficher, pcOk;
Widget pcPanneauStyle, pcPanneauCouleur, pcPanneauEpaisseur;
Widget pcOptionsStyle, pcOptionsCouleur, pcOptionsEpaisseur;
Widget pcOptionsStyleItems[7], pcOptionsCouleurItems[9], pcOptionsEpaisseurItems[5];
Widget pcListeItems, pcChamps[6];
Widget pcSeparateur1,pcSeparateur2;
Widget pcPanneauContours,pcOptionsContours,pcContourItems[3];
Widget pcPanneauLabels,pcOptionsLabels,pcLabelItems[3];
Widget pcPanneauTailleLabels,pcOptionsTailleLabels,pcTailleLabelItems[5];
Widget pcPanneauValeursCentrales,pcOptionsValeursCentrales,pcValeurItems[3];
Widget pcPanneauTailleValeursCentrales,pcOptionsTailleValeursCentrales,pcTailleValeurItems[13];

static char *nomPanneauContour[] = {"PanneauContours", "ContourPanel"};
static char *labelTopLevel[] = {"Contours", "Contours"};
static char *labelChamps[][34] = { 
{"Fond   ", "Grille  ", "Champ  1", "Champ  2", "Champ  3", "Champ  4", 
 "Champ  5", "Champ  6", "Champ  7", "Champ  8", "Champ  9", "Champ 10", 
 "Champ 11", "Champ 12", "Champ 13", "Champ 14", "Champ 15", "Champ 16",
 "Champ 17", "Champ 18", "Champ 19", "Champ 20", "Champ 21", "Champ 22",
 "Champ 23", "Champ 24", "Champ 25", "Champ 26", "Champ 27", "Champ 28",
 "Champ 29", "Champ 30", "Champ 31", "Champ 32"}, 
{"Background", "Grid", "Field  1", "Field  2", "Field  3", "Field  4", 
 "Field  5", "Field  6", "Field  7", "Field  8", "Field  9", "Field 10", 
 "Field 11", "Field 12", "Field 13", "Field 14", "Field 15", "Field 16",
 "Field 17", "Field 18", "Field 19", "Field 20", "Field 21", "Field 22",
 "Field 23", "Field 24", "Field 25", "Field 26", "Field 27", "Field 28",
 "Field 29", "Field 30", "Field 31", "Field 32"}};

static char *pcValCentraleFontSize[][13] = {
{"10                  ","15","20","25","30","35","40","50","60","70","80","90","100"},
{"10                  ","15","20","25","30","35","40","50","60","70","80","90","100"}};


static char *pcLabelFontSize[][6] = {{"Auto","12                  ", "14", "17", "18", "24"},
{ "Auto","12                  ", "14", "17", "18", "24"}};

static char *pcLabelTailleLabels[]= {"Taille    \nLabels","Label     \nSize"};
static char *pcLabelTailleValeursCentrales[] = {"Taille val.\ncentrales","Central val.\nSize"};

static char *activationSelect[][3] = {
{"Selon menu Affichage", "Toujours", "Jamais"},
{"According to Display menu", "Always", "Never"}};


static char *suffixes[][3] = { {"fond", "grille", "champ"}, 
{"background", "grid", "field" }};

static char *colors[][9] = { {"blanc","noir","rouge","cyan", "jaune", "magenta","vert","bleu","gris"},
{"white","black","red","cyan","yellow","magenta","green","blue","gray"}};


static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Refresh"};


#define COULEUR   0
#define EPAISSEUR 1
#define STYLE     2



static char *labelStyle[] = {"Style     ", "Style     "};
static char *labelCouleur[] = {"Couleur   ", "Color     "};
static char *labelEpaisseur[] = {"Epaisseur \ndes lignes", "Line      \nThickness "};
static char *labelValeursCentrales[] = {"Valeurs   \ncentrales","Central \nValues"};
static char *labelTraitementVectoriel[] = {"Traitement\nVectoriel","Vector\nProcessing"};

static char *pcLabelOptionsStyle[][7] = {{"Lignes pleines      ", 
					     "--  --  --  --      ", 
					     "----    ----        ", 
					     "--------            ", 
					     "--  --   (< 0 seul.)", 
					     "----     (< 0 seul.)", 
					     "-------- (< 0 seul.)"},
					    {"Full lines      ", 
						"--  --  --  --  ", 
						"----    ----    ", 
						"--------        ",
						"--  --   (< 0 only)", 
						"----     (< 0 only)", 
						"-------- (< 0 only)"}};

static char *pcLabelOptionsCouleur[][9] = {{"blanc               ", "noir","rouge","cyan","jaune","magenta","vert","bleu","gris"},
					    {"white               ", "black","red","cyan","yellow","magenta","green","blue", "gray"}};
static char *pcLabelOptionsEpaisseur[][5] = {{"1                   ", "2", "3", "4", "5"}, 
					      {"1                   ", "2", "3", "4", "5"}};


int currentToggle = FOND;
int pcSelectionTerminee;
char panneauContoursGeometrie[32];
static int nbItemsListe = 34;

void CheckToggles (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int lng;
   Arg args[3];
   int i,j, n, ind, pos;

   XmListCallbackStruct *info = (XmListCallbackStruct *) call_data;
   Pixel back, fore;
   
   pos =    (int)(info->item_position - 1);

   switch (pos)
      {
      case 0:
      case 1:
      currentToggle = pos+32;
      break;

      default:
      currentToggle = pos-2;
      break;
      }

   XtSetArg(args[0], XmNbackground, &back);
   XtSetArg(args[1], XmNforeground, &fore);
   XtGetValues(pcOptionsCouleurItems[xc.attributs[currentToggle].couleurFore], args, 2);

   XtSetArg(args[0], XmNbackground, back);
   XtSetArg(args[1], XmNforeground, fore);
   XtSetValues(pcPanneauCouleur, args, 2);

   XtSetArg(args[0], XmNmenuHistory, pcOptionsCouleurItems[xc.attributs[currentToggle].couleurFore]);
   XtSetValues(pcPanneauCouleur, args, 1);

   i = 0;
   while (xc.attributs[currentToggle].epaisseur != (int) atoi(XtName(pcOptionsEpaisseurItems[i])))
      i++;

   XtSetArg(args[0], XmNmenuHistory, pcOptionsEpaisseurItems[i]);
   XtSetValues(pcPanneauEpaisseur, args, 1);

   switch(xc.attributs[currentToggle].codeDash)
      {
      case 0:
      ind = 0;
      break;
      
      case 1:
      ind = xc.attributs[currentToggle].style;
      break;
      
      case 2:
      ind = xc.attributs[currentToggle].style + 3;
      break;
      }
   
   XtSetArg(args[0], XmNmenuHistory, pcOptionsStyleItems[ind]);
   XtSetValues(pcPanneauStyle, args, 1);

   XtSetArg(args[0], XmNmenuHistory, pcContourItems[xc.attributs[currentToggle].displayContours]);
   XtSetValues(pcPanneauContours, args, 1);

   XtSetArg(args[0], XmNmenuHistory, pcLabelItems[xc.attributs[currentToggle].displayLabels]);
   XtSetValues(pcPanneauLabels, args, 1);

   XtSetArg(args[0], XmNmenuHistory, pcValeurItems[xc.attributs[currentToggle].displayValCentrales]);
   XtSetValues(pcPanneauValeursCentrales, args, 1);

   i = 0;
   while (xc.attributs[currentToggle].labelSize != (int) atoi(XtName(pcTailleLabelItems[i])))
      i++;

   XtSetArg(args[0], XmNmenuHistory, pcTailleLabelItems[i]);
   XtSetValues(pcPanneauTailleLabels, args, 1);

   i = 0;
   while (xc.attributs[currentToggle].centralValSize != (int) atoi(XtName(pcTailleValeurItems[i])))
      i++;

   XtSetArg(args[0], XmNmenuHistory, pcTailleValeurItems[i]);
   XtSetValues(pcPanneauTailleValeursCentrales, args, 1);

   }

/****
***********************************************************************
***********************************************************************
****/

void SetColorToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int r, g, b;
   Arg args[2];
   Pixel back, fore;

   XtSetArg(args[0], XmNbackground, &back);
   XtSetArg(args[1], XmNforeground, &fore);
   XtGetValues(w, args, 2);

   XtSetArg(args[0], XmNbackground, back);
   XtSetArg(args[1], XmNforeground, fore);
   XtSetValues(pcPanneauCouleur, args, 2);
   

   xc.attributs[currentToggle].couleurFore = (int)client_data;
   c_wglgco(xc.attributs[currentToggle].couleurFore, &r, &g, &b);
   if (8 <= c_wglgpl() && (currentToggle < 4 || currentToggle >= 32))
      c_wglmco(xc.attributs[currentToggle].indCouleurFore, r, g, b);
   else
      xc.attributs[currentToggle].indCouleurFore = xc.attributs[currentToggle].couleurFore;
   c_wglmco(xc.attributs[currentToggle].indCouleurFore, r, g, b);

   switch((int)client_data)
      {
      case JAUNE:
      case BLANC:
      case CYAN:
      xc.attributs[currentToggle].couleurBack = NOIR;
      if (8 <= c_wglgpl() && (currentToggle < 4 || currentToggle >= 32))
	 c_wglmco(xc.attributs[currentToggle].indCouleurBack, 0, 0, 0);
      else
	 xc.attributs[currentToggle].indCouleurBack = xc.attributs[currentToggle].couleurBack;
	 c_wglmco(xc.attributs[currentToggle].indCouleurBack, 0, 0, 0);
      break;

      default:
      xc.attributs[currentToggle].couleurBack = BLANC;
      if (8 <= c_wglgpl() && (currentToggle < 4 || currentToggle >= 32))
	 c_wglmco(xc.attributs[currentToggle].indCouleurBack, 255, 255, 255);
      else
	 xc.attributs[currentToggle].indCouleurBack = xc.attributs[currentToggle].couleurBack;
	 c_wglmco(xc.attributs[currentToggle].indCouleurBack, 255, 255, 255);
      break;
      }

   }

/****
***********************************************************************
***********************************************************************
****/

void SetThicknessToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].epaisseur = atoi(XtName(w));
   }

/****
***********************************************************************
***********************************************************************
****/

void SetLabelSizeToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].labelSize = atoi(XtName(w));
   }

/****
***********************************************************************
***********************************************************************
****/

void SetCentralValueSizeToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].centralValSize = atoi(XtName(w));
   }

/****
***********************************************************************
***********************************************************************
****/

void SetStyleToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].style = (int)client_data;

   switch ((int)client_data)
      {
      case 0:
      xc.attributs[currentToggle].codeDash = 0;
      break;
      
      case 1:
      case 2:
      case 3:
      xc.attributs[currentToggle].codeDash = 1;
      break;

      case 4:
      case 5:
      case 6:
      xc.attributs[currentToggle].style = (int)client_data - 3;
      xc.attributs[currentToggle].codeDash = 2;
      break;
      }
      

   }

/****
***********************************************************************
***********************************************************************
****/

void SetContoursToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].displayContours = (int)client_data;
   }

/****
***********************************************************************
***********************************************************************
****/

void SetLabelsToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].displayLabels = (int)client_data;
   }

/****
***********************************************************************
***********************************************************************
****/

void SetCentralValuesToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   xc.attributs[currentToggle].displayValCentrales = (int)client_data;
   }

/****
***********************************************************************
***********************************************************************
****/

static XtCallbackProc PcOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   pcSelectionTerminee = TRUE;
   DesactiverPanneauContour();
   }

/****
***********************************************************************
***********************************************************************
****/

static XtCallbackProc PcAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;

   RedessinerFenetres();

   }

/***
***************************************************************************
***************************************************************************
***/

InitPanneauContour()
{
   int i,j;
   Arg args[6];
   XmString string;

   int n,lng;
   char nomShell[128];
   Pixel indCouleurs[16];
   Colormap cmap;
   XmStringTable table;

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauContour[lng]);
   i = 0;
   pcTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);
   i = 0;
   pcForme = (Widget) XmCreateForm(pcTopLevel, "form", args, i);
   XtManageChild(pcForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcOk = (Widget)XmCreatePushButton(pcForme, labelOk[lng], args, i);
   XtAddCallback(pcOk, XmNactivateCallback, (XtCallbackProc)  PcOk, NULL);
   XtManageChild(pcOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pcOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pcAfficher = (Widget)XmCreatePushButton(pcForme, labelAfficher[lng], args, i);
   XtAddCallback(pcAfficher, XmNactivateCallback, (XtCallbackProc)  PcAfficher, NULL);
   XtManageChild(pcAfficher);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcFrameChamps = (Widget) XmCreateFrame(pcForme, "form", args, i);
   XtManageChild(pcFrameChamps);

   i = 0;
   pcFormeChamps = (Widget) XmCreateForm(pcFrameChamps, "form", args, i);
   XtManageChild(pcFormeChamps);

   table = (XmString *) calloc(nbItemsListe, sizeof(XmString));
   for (j=0; j < nbItemsListe; j++)
      {
      table[j] = XmStringCreate(labelChamps[lng][j], XmSTRING_DEFAULT_CHARSET);
      }
   
   i = 0;
   XtSetArg(args[i], XmNitemCount, nbItemsListe); i++;
   XtSetArg(args[i], XmNvisibleItemCount, 20); i++;
   XtSetArg(args[i], XmNitems, table); i++;
   XtSetArg(args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
   pcListeItems = (Widget) XmCreateScrolledList(pcFormeChamps, "option_menu1", args, i);

   XmListSelectPos(pcListeItems, 1, True);
   XtAddCallback(pcListeItems, XmNsingleSelectionCallback, (XtCallbackProc)  CheckToggles, NULL);
   XtManageChild(pcListeItems);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcFrameChamps); i++;
   pcFormeAttributs = (Widget) XmCreateForm(pcForme, "form", args, i);
   XtManageChild(pcFormeAttributs);

   i = 0;
   pcFrameAttributs = (Widget) XmCreateFrame(pcFormeAttributs, "form", args, i);
   XtManageChild(pcFrameAttributs);

/* Create RowColumn in pcTopLevel */

   i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pcRc = XmCreateRowColumn(pcFrameAttributs, "pcRc", args, i);
   XtManageChild(pcRc);

   InitPixelsCouleursDeBase(indCouleurs);

   i = 0;
   XtSetArg(args[i], XmNbackground, indCouleurs[0]); i++;
   pcOptionsCouleur = (Widget)XmCreatePulldownMenu(pcRc, labelCouleur[lng], args, i);
   
   for (n=0; n < XtNumber(pcLabelOptionsCouleur[lng]); n++)
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
	   
	pcOptionsCouleurItems[n] = (Widget) XmCreatePushButton(pcOptionsCouleur, pcLabelOptionsCouleur[lng][n], args, i);
	XtAddCallback(pcOptionsCouleurItems[n], XmNactivateCallback, (XtCallbackProc)  SetColorToggle, (XtPointer) n);
	}

   XtManageChildren(pcOptionsCouleurItems, XtNumber(pcLabelOptionsCouleur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelCouleur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsCouleur); i++;
   XtSetArg(args[i], XmNmenuHistory, pcOptionsCouleurItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauCouleur = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   i = 0;
   XtSetArg(args[i], XmNbackground, indCouleurs[NOIR]); i++;
   XtSetArg(args[i], XmNforeground, indCouleurs[BLANC]); i++;
   XtSetValues(XmOptionLabelGadget(pcPanneauCouleur), args, i);

   XtManageChild(pcPanneauCouleur);

/**
****
**/

   pcOptionsEpaisseur = (Widget)XmCreatePulldownMenu(pcRc, labelEpaisseur[lng], NULL, 0);

   for (n=0; n < XtNumber(pcLabelOptionsEpaisseur[lng]); n++)
	{
	i = 0;
	pcOptionsEpaisseurItems[n] = XmCreatePushButtonGadget(pcOptionsEpaisseur, pcLabelOptionsEpaisseur[lng][n], args, i);
	XtAddCallback(pcOptionsEpaisseurItems[n], XmNactivateCallback, (XtCallbackProc)  SetThicknessToggle, (XtPointer) n);
	}

   XtManageChildren(pcOptionsEpaisseurItems, XtNumber(pcLabelOptionsEpaisseur[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelEpaisseur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsEpaisseur); i++;
   XtSetArg(args[i], XmNmenuHistory, pcOptionsEpaisseurItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauEpaisseur = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauEpaisseur);

/**
**
**/

   pcOptionsStyle = (Widget)XmCreatePulldownMenu(pcRc, labelStyle[lng], NULL, 0);

   for (n=0; n < XtNumber(pcLabelOptionsStyle[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pcLabelOptionsStyle[lng][n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pcOptionsStyleItems[n] = XmCreatePushButtonGadget(pcOptionsStyle, pcLabelOptionsStyle[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(pcOptionsStyleItems[n], XmNactivateCallback, (XtCallbackProc)  SetStyleToggle, (XtPointer) n);
	}

   XtManageChildren(pcOptionsStyleItems, XtNumber(pcLabelOptionsStyle[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelStyle[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsStyle); i++;
   XtSetArg(args[i], XmNmenuHistory, pcOptionsStyleItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauStyle = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauStyle);

/**
**
**/

   n = 0;
   pcSeparateur1 =  (Widget) XmCreateSeparator(pcRc, "sep 1", args, (XtPointer) n);
   XtManageChild(pcSeparateur1);

/**
**
**/

   pcOptionsContours = (Widget)XmCreatePulldownMenu(pcRc, "Contours  ", NULL, 0);

   for (n=0; n < XtNumber(activationSelect[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(activationSelect[lng][n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pcContourItems[n] = XmCreatePushButtonGadget(pcOptionsContours, activationSelect[lng][n], args, i);
	XtAddCallback(pcContourItems[n], XmNactivateCallback, (XtCallbackProc)  SetContoursToggle, (XtPointer) n);
	XmStringFree(string);   
	}

   XtManageChildren(pcContourItems, XtNumber(activationSelect[lng]));

   i = 0;
   string = XmStringCreateLtoR("Contours  ", XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsContours); i++;
   XtSetArg(args[i], XmNmenuHistory, pcContourItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauContours = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauContours);
   

/**
**
**/

   pcOptionsLabels = (Widget)XmCreatePulldownMenu(pcRc, "Labels    ", NULL, 0);

   for (n=0; n < XtNumber(activationSelect[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(activationSelect[lng][n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pcLabelItems[n] = XmCreatePushButtonGadget(pcOptionsLabels, activationSelect[lng][n], args, i);
	XtAddCallback(pcLabelItems[n], XmNactivateCallback, (XtCallbackProc)  SetLabelsToggle, (XtPointer) n);
	XmStringFree(string);   
	}

   XtManageChildren(pcLabelItems, XtNumber(activationSelect[lng]));

   i = 0;
   string = XmStringCreateLtoR("Labels    ", XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsLabels); i++;
   XtSetArg(args[i], XmNmenuHistory, pcLabelItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauLabels = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauLabels);
   
/**
**
**/

   pcOptionsTailleLabels = (Widget)XmCreatePulldownMenu(pcRc, "TailleLabels", NULL, 0);

   for (n=0; n < XtNumber(pcLabelFontSize[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pcLabelFontSize[lng][n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pcTailleLabelItems[n] = XmCreatePushButtonGadget(pcOptionsTailleLabels, pcLabelFontSize[lng][n], args, i);
	XtAddCallback(pcTailleLabelItems[n], XmNactivateCallback, (XtCallbackProc)  SetLabelSizeToggle, (XtPointer) n);
	XmStringFree(string);   
	}

   XtManageChildren(pcTailleLabelItems, XtNumber(pcLabelFontSize[lng]));

   i = 0;
   string = XmStringCreateLtoR(pcLabelTailleLabels[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsTailleLabels); i++;
   XtSetArg(args[i], XmNmenuHistory, pcTailleLabelItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauTailleLabels = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauTailleLabels);
   
/**
**
**/


   n = 0;
   pcSeparateur2 =  (Widget) XmCreateSeparator(pcRc, "sep 1", args, (XtPointer) n);
   XtManageChild(pcSeparateur2);

/**
**
**/
   pcOptionsValeursCentrales = (Widget)XmCreatePulldownMenu(pcRc, labelValeursCentrales[lng], NULL, 0);

   for (n=0; n < XtNumber(activationSelect[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(activationSelect[lng][n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pcValeurItems[n] = XmCreatePushButtonGadget(pcOptionsValeursCentrales, activationSelect[lng][n], args, i);
	XtAddCallback(pcValeurItems[n], XmNactivateCallback, (XtCallbackProc)  SetCentralValuesToggle, (XtPointer) n);
	XmStringFree(string);   
	}

   XtManageChildren(pcValeurItems, XtNumber(activationSelect[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelValeursCentrales[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsValeursCentrales); i++;
   XtSetArg(args[i], XmNmenuHistory, pcValeurItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauValeursCentrales = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauValeursCentrales);
   
/**
**
**/

   pcOptionsTailleValeursCentrales = (Widget)XmCreatePulldownMenu(pcRc, pcLabelTailleValeursCentrales[lng], NULL, 0);

   for (n=0; n < XtNumber(pcValCentraleFontSize[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pcValCentraleFontSize[lng][n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pcTailleValeurItems[n] = XmCreatePushButtonGadget(pcOptionsTailleValeursCentrales, pcValCentraleFontSize[lng][n], args, i);
	XtAddCallback(pcTailleValeurItems[n], XmNactivateCallback, (XtCallbackProc)  SetCentralValueSizeToggle, (XtPointer) (XtPointer) n);
	XmStringFree(string);   
	}

   XtManageChildren(pcTailleValeurItems, XtNumber(pcValCentraleFontSize[lng]));

   i = 0;
   string = XmStringCreateLtoR(pcLabelTailleValeursCentrales[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pcOptionsTailleValeursCentrales); i++;
   XtSetArg(args[i], XmNmenuHistory, pcTailleValeurItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pcPanneauTailleValeursCentrales = XmCreateOptionMenu(pcRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pcPanneauTailleValeursCentrales);
   
   }

/****
***********************************************************************
***********************************************************************
****/

ActiverPanneauContour()
{

   Colormap cmap;
   Arg args[2];
   int i;

   if (!pcTopLevel)
      InitPanneauContour();

   pcSelectionTerminee = False;
/**   CheckToggles(pcChamps[0], 0, 0); **/
   currentToggle = 0;
   
   if (!XtIsRealized(pcTopLevel))
      {
      XtRealizeWidget(pcTopLevel);
      CheckColormap(pcTopLevel);
      }
      
   f77name(xpancact)();

   }

/****
***********************************************************************
***********************************************************************
****/

f77name(xpancact)()
{
   LocalEventLoop(pcTopLevel);
   }


/****
***********************************************************************
***********************************************************************
****/

DesactiverPanneauContour()
{
   int i;

   XtUnrealizeWidget(pcTopLevel);
   }

/****
***********************************************************************
***********************************************************************
****/

f77name(c_sconatr)(item,valeur,lenItem,lenValeur)
char item[],valeur[];
int lenItem,lenValeur;
{
   Arg args[10];
   int i,j,r,g,b;
   int indItem;
   int found;
   char tmpItem1[32],tmpItem2[32];
   int attribut ,ind;
   
   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);
   
   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauContoursGeometrie,valeur);
      return;
      }
   
   PCSplitItem(&attribut, &ind, item);   
   
   switch(attribut)
      {
      case COULEUR:
      j = 0;
      found = 0;
      while (j < 9 && !found)
	 {
	 if (0 == strcmp(valeur,colors[0][j]) || 0 == strcmp(valeur,colors[1][j]))
	    {
	    found = 1;
	    xc.attributs[ind].couleurFore = j;
	    }
	 j++;
	 }

      j--;
      if (ind < 4 ||  ind >= 32)
	 {
	 c_wglgco(xc.attributs[ind].couleurFore,&r,&g,&b);
	 c_wglmco(xc.attributs[ind].indCouleurFore,r,g,b);
	 switch(j)
	    {
	    case JAUNE:
	    case BLANC:
	    case CYAN:
	    xc.attributs[ind].couleurBack = NOIR;
	    c_wglmco(xc.attributs[ind].indCouleurBack,0,0,0);
	    break;
	    
	    default:
	    xc.attributs[ind].couleurBack = BLANC;
	    c_wglmco(xc.attributs[ind].indCouleurBack,255,255,255);
	    break;
	    }
	 }
      else
	 {
	 xc.attributs[ind].indCouleurFore = xc.attributs[ind].couleurFore;
	 }
      break;
      
      case EPAISSEUR:
      xc.attributs[ind].epaisseur = atoi(valeur);
      break;
      
      case STYLE:
      xc.attributs[ind].style = atoi(valeur);
      switch (xc.attributs[ind].style)
	 {
	 case 0:
	 xc.attributs[ind].codeDash = 0;
	 break;
	 
	 case 1:
	 case 2:
	 case 3:
	 xc.attributs[ind].codeDash = 1;
	 break;
	 
	 case 4:
	 case 5:
	 case 6:
	 xc.attributs[ind].codeDash = 2;
	 xc.attributs[ind].style -= 3;
	 break;
	 
	 default:
	 break;
	 }
      }
   }

/****
***********************************************************************
***********************************************************************
****/

EcrConAtr(fichierDemarrage)
FILE *fichierDemarrage;
{
   char tableau[32];
   char ligne[80];
   char item[32],valeur[32];
   int i,indSuf;

   Arg  args[10];
   XmString label;
   XmFontList fontListe;
   char *geom;
   Window root;
   Position x,y;
   Display *disp;
   Window win;
   strcpy(tableau, "contours");
   
   strcpy(item,"geometrie");
   if (pcTopLevel)
      {
      disp = XtDisplay(pcTopLevel);
      win  = XtWindow(pcTopLevel);
      i = 0;
      XtSetArg(args[i], XmNx, &x); i++;
      XtSetArg(args[i], XmNy, &y); i++;
      XtGetValues(pcTopLevel, args, i);
      
      sprintf(valeur,"%+d%+d",x,y);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   else
      {
      if (strlen(panneauContoursGeometrie) > 0)
         {
         strcpy(valeur,panneauContoursGeometrie);
         sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
         fprintf(fichierDemarrage,"%s\n",ligne);
         }
      }

   for (i=0; i < 34; i++)
      {
      switch (i)
	 {
	 case FOND:
	 indSuf =0;
	 break;

	 case GRID:
	 indSuf = 1;
	 break;

	 default:
	 indSuf = 2;
	 break;
	 }
	 
      strcpy(item,"couleur_");
      strcat(item,suffixes[0][indSuf]);
      if (i < 32)
	 {
	 sprintf(item,"%s_%02d",item,i+1);
	 }
      strcpy(valeur,colors[0][xc.attributs[i].couleurFore]);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);

      strcpy(item,"epaisseur_");
      strcat(item,suffixes[0][indSuf]);
      if (i < 32)
	 {
	 sprintf(item,"%s_%02d",item,i+1);
	 }
      sprintf(valeur,"%2d",xc.attributs[i].epaisseur);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);

      strcpy(item,"style_");
      strcat(item,suffixes[0][indSuf]);
      if (i < 32)
	 {
	 sprintf(item,"%s_%02d",item,i+1);
	 }
      if (xc.attributs[i].codeDash > 0)
         sprintf(valeur,"%2d",xc.attributs[i].style+3*(xc.attributs[i].codeDash-1));
      else
         sprintf(valeur,"%2d",xc.attributs[i].style);

      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   
   }


/****
***********************************************************************
***********************************************************************
****/

PCSplitItem(attribut, ind, item)   
int *attribut;
int *ind;
char *item;
{
   

   if (strstr(item,"couleur_"))
      {
      *attribut = COULEUR;
      }
   else
      {
      if (strstr(item,"epaisseur_"))
	 {
	 *attribut = EPAISSEUR;
	 }
      else
	 {
	 *attribut = STYLE;
	 }
      }
   
   if (strstr(item,"fond"))
      {
      *ind = FOND;
      }
   else
      {
      if (strstr(item,"grille"))
	 {
	 *ind = GRID;
	 }
      else
	 {
	 *ind = atoi(&item[strlen(item)-2])-1;
	 }
      }

   }
