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
#include <wgl.h>
#include <rpnmacros.h>
#include <rec.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

int facteurDensite;
int echelleWW = 100;

Widget pventTopLevel = NULL;
Widget pventForme, pventFormeMenus, pventFrameVents, pventRc, pventAfficher, pventOk;
Widget pventPanneauDensite,pventOptionsDensite,pventOptionsDensiteItems[11];
Widget pventPanneauLongueur,pventOptionsLongueur,pventOptionsLongueurItems[8];
Widget pventPanneauEpaisseurF,pventOptionsEpaisseurF,pventOptionsEpaisseurFItems[8];
Widget pventPanneauCroissance,pventOptionsCroissance,pventOptionsCroissanceItems[5];
Widget pventPanneauEchelleWW,pventOptionsEchelleWW,pventOptionsEchelleWWItems[11];
Widget pventFormeToggles,pventFormeToggles2,pventFrameToggles;
Widget pventFormeBarbules,pventLabelBarbules,pventFrameBarbules;
Widget pventSelectionBarbules,pventAucun,pventBarbules,pventFleches;
Widget pventFormeModule,pventLabelModule,pventFrameModule;
Widget pventSelectionModule,pventModuleOn,pventModuleOff;

static char *nomPanneauVents[] = {"PanneauVents", "VectorPanel"};
static char *labelTopLevel[]   = {"Champs vectoriels", "Vector fields"};
static char *labelOk[]         = {"Fermer", "Close"};
static char *labelAfficher[]   = {"Redessiner", "Refresh"};

static char *labelDensite[]    = {"Increment  ", "Increment"};
static char *labelLongueur[]   = {"Longueur   ", "Length "};
static char *labelEpaisseurF[]   = {"Epaisseur \ndes fleches", "Arrow\nThickness"};
static char *labelCroissance[] = {  "Croissance\ndes fleches", "Arrow\nGrowth"};
static char *labelEchelleWW[]  = {"Amplif.   \nWW",          "Amplif.  \nWW"};
static char *labelAucun[]      = {"Aucun"               , "None"};
static char *labelFleches[]    = {"Fleches"               , "Arrows"};
static char *labelBarbules[]   = {"Barbules"              , "Wind Barbs"};
static char *labelSelectionBarbules[]  = {"Mode d'affichage","Display Mode"};
static char *labelModule[]             = {"Module du vent  ","Wind Modulus"};
static char *labelOui[]         = {"Oui", "Yes"};
static char *labelNon[]        = {"Non", "No"};

    
static char 
   *pventLabelOptionsDensite[][11] = {{"Auto         ","20","10","5","2","1","0.50","0.25","0.20","0.10","0.05"},
					 {"Auto         ","20","10","5","2","1","0.50","0.25","0.20","0.10","0.05"}};

static char 
   *pventLabelOptionsLongueur[][8] = {{"8", "16", "24", "32           ", "48","64","96","128"},
                                      {"8", "16", "24", "32           ", "48","64","96","128"}};
static char 
   *pventLabelOptionsEpaisseurF[][6] = {{"1", "2", "3", "4           ", "5","6"},
                                      {"1", "2", "3", "4           ", "5","6"}};

static char 
   *pventLabelOptionsEchelleWW[][11] = {{"1", "5", "10", "25           ", "50","100","150","200","400","500","1000"},
                                       {"1", "5", "10", "25           ", "50","100","150","200","400","500","1000"}};

static char
   *pventLabelOptionsCroissance[][5] = {{"Cubique", "Quadratique", "Lineaire", "Racine carree", "Racine cubique"},
                                        {"Cubic","Quadratic","Linear","Square root","Cubic root"}};

#define LINEAIRE        1
#define RACINE_CARREE   -2
#define RACINE_CUBIQUE  -3
#define CARRE           2
#define CUBIQUE         3

#define BARBULE        0
#define FLECHES         1
#define NONE           -1
#define NO_MODULE       0
#define MODULE          1

float densite = -1.0;
int croissance = 1;
int longueur = 32;
int epaisseurF = 1;
int displayMode = FLECHES;
int flagModule  = 0;

static int currentItem;
char panneauVentsGeometrie[32];

int pventSelectionTerminee;

static XtCallbackProc PventAfficher();

void SetModuleOn (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   flagModule = MODULE;
   if (XmToggleButtonGetState(w))
      {
      PventAfficher(NULL,NULL,NULL);
      }
   }

void SetModuleOff (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   flagModule = NO_MODULE;
   if (XmToggleButtonGetState(w))
      {
      PventAfficher(NULL,NULL,NULL);
      }
   }

void SetModeNone(w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   displayMode = NONE;
   if (XmToggleButtonGetState(w))
      {
      PventAfficher(NULL,NULL,NULL);
      }
   }

void SetModeBarbules(w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   displayMode = BARBULE;
   if (XmToggleButtonGetState(w))
      {
      PventAfficher(NULL,NULL,NULL);
      }
   }

void SetModeFleches (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   displayMode = FLECHES;
   if (XmToggleButtonGetState(w))
      {
      PventAfficher(NULL,NULL,NULL);
      }
   }

void SetDensiteToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   if (0 == strcmp(XtName(w), pventLabelOptionsDensite[0][0]))
      {
      densite = -1.0;
      }
   else
      {
      sscanf(XtName(w),"%f",&densite);
      }
   PventAfficher(NULL,NULL,NULL);
   }

void SetLongueurToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   sscanf(XtName(w),"%d",&longueur);
   PventAfficher(NULL,NULL,NULL);
   }

void SetEpaisseurFToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   sscanf(XtName(w),"%d",&epaisseurF);
   PventAfficher(NULL,NULL,NULL);
   }

void SetEchelleWWToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int i;
   int oldEchelleWW = echelleWW;
   float x1,y1,x2,y2;
   sscanf(XtName(w),"%d",&echelleWW);
   
   
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrRescaleWW(i,echelleWW,oldEchelleWW);
      }
   
   CoupeMgrGetCoupeCoords(&x1, &y1, &x2, &y2);
   PreparerCoupe(x1,y1,x2,y2);
   RedessinerFenetreCoupe();
   }

void SetCroissanceToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   switch((int)client_data)
      {
      case 0:
      croissance = CUBIQUE;
      break;
      
      case 1:
      croissance = QUADRATIQUE;
      break;
      
      case 2:
      croissance = LINEAIRE;
      break;
      
      case 3:
      croissance = RACINE_CARREE;
      break;
      
      case 4:
      croissance = RACINE_CUBIQUE;
      break;
      }
   PventAfficher(NULL,NULL,NULL);
   }

static XtCallbackProc PventOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   pventSelectionTerminee = TRUE;
   DesactiverPanneauVents();
   }

static XtCallbackProc PventAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;

   RedessinerFenetres();
   LibererImages();

   }


InitPanneauVents()
{

   int i;
   Arg args[6];
   XmString string;

   int n,lng;
   char nomShell[128];
   Pixel indCouleurs[16];
   Colormap cmap;

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauVents[lng]);
   i = 0;

   if (0 < strlen(panneauVentsGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneauVentsGeometrie);
      i++;
      }
      
   pventTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);
   i = 0;
   pventForme = (Widget) XmCreateForm(pventTopLevel, "form", args, i);
   XtManageChild(pventForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pventOk = (Widget)XmCreatePushButton(pventForme, labelOk[lng], args, i);
   XtAddCallback(pventOk, XmNactivateCallback, (XtCallbackProc) PventOk, NULL);
   XtManageChild(pventOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pventOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pventAfficher = (Widget)XmCreatePushButton(pventForme, labelAfficher[lng], args, i);
   XtAddCallback(pventAfficher, XmNactivateCallback, (XtCallbackProc) PventAfficher, NULL);
   XtManageChild(pventAfficher);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pventOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pventFormeToggles = (Widget) XmCreateForm(pventForme, "form", args, i);
   XtManageChild(pventFormeToggles);

   i = 0;
   pventFrameToggles = (Widget) XmCreateFrame(pventFormeToggles, "form", args, i);
   XtManageChild(pventFrameToggles);

   i = 0;
   pventFormeToggles2 = (Widget) XmCreateForm(pventFrameToggles, "form", args, i);
   XtManageChild(pventFormeToggles2);

   pventFormeBarbules = (Widget) XmCreateRowColumn(pventFormeToggles2, "form", NULL, 0);
   XtManageChild(pventFormeBarbules);

   pventLabelBarbules = (Widget) XmCreateLabel(pventFormeBarbules, labelSelectionBarbules[lng], NULL, 0);
   XtManageChild(pventLabelBarbules);

   pventFrameBarbules = (Widget) XmCreateFrame(pventFormeBarbules, "frame", NULL, 0);
   XtManageChild(pventFrameBarbules);

   pventSelectionBarbules = (Widget) XmCreateRadioBox(pventFrameBarbules, "radiobox", NULL, 0);
   XtManageChild(pventSelectionBarbules);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pventAucun = (Widget) XmCreateToggleButton(pventSelectionBarbules, labelAucun[lng], args, i);
   XtAddCallback(pventAucun, XmNvalueChangedCallback, (XtCallbackProc) SetModeNone, NULL);
   XtManageChild(pventAucun);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pventBarbules = (Widget) XmCreateToggleButton(pventSelectionBarbules, labelBarbules[lng], args, i);
   XtAddCallback(pventBarbules, XmNvalueChangedCallback, (XtCallbackProc) SetModeBarbules, NULL);
   XtManageChild(pventBarbules);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pventFleches = (Widget)XmCreateToggleButton(pventSelectionBarbules, labelFleches[lng], args, i);
   XtAddCallback(pventFleches, XmNvalueChangedCallback, (XtCallbackProc) SetModeFleches, NULL);
   XtManageChild(pventFleches);
   
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pventFormeBarbules); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pventFormeModule = (Widget) XmCreateRowColumn(pventFormeToggles2, "form", args, i);
   XtManageChild(pventFormeModule);

   pventLabelModule = (Widget) XmCreateLabel(pventFormeModule, labelModule[lng], NULL, 0);
   XtManageChild(pventLabelModule);

   pventFrameModule = (Widget) XmCreateFrame(pventFormeModule, "frame", NULL, 0);
   XtManageChild(pventFrameModule);

   pventSelectionModule = (Widget) XmCreateRadioBox(pventFrameModule, "radiobox", NULL, 0);
   XtManageChild(pventSelectionModule);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pventModuleOn = (Widget) XmCreateToggleButton(pventSelectionModule, labelOui[lng], args, i);
   XtAddCallback(pventModuleOn, XmNvalueChangedCallback, (XtCallbackProc) SetModuleOn, NULL);
   XtManageChild(pventModuleOn);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pventModuleOff = (Widget) XmCreateToggleButton(pventSelectionModule, labelNon[lng], args, i);
   XtAddCallback(pventModuleOff, XmNvalueChangedCallback, (XtCallbackProc) SetModuleOff, NULL);
   XtManageChild(pventModuleOff);
   
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pventOk); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pventFormeToggles); i++;
   pventFormeMenus = (Widget) XmCreateForm(pventForme, "form", args, i);
   XtManageChild(pventFormeMenus);

   i = 0;
   pventFrameVents = (Widget) XmCreateFrame(pventFormeMenus, "form", args, i);
   XtManageChild(pventFrameVents);

/* Create RowColumn in pventTopLevel */

  i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pventRc = XmCreateRowColumn(pventFrameVents, "pventRc", args, i);
   XtManageChild(pventRc);

   i = 0;
   pventOptionsDensite = (Widget)XmCreatePulldownMenu(pventRc, labelDensite[lng], args, i);

   for (n=0; n < XtNumber(pventLabelOptionsDensite[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pventLabelOptionsDensite[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pventOptionsDensiteItems[n] = XmCreatePushButtonGadget(pventOptionsDensite, pventLabelOptionsDensite[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(pventOptionsDensiteItems[n], XmNactivateCallback, (XtCallbackProc) SetDensiteToggle, (XtPointer) n);
	}

   XtManageChildren(pventOptionsDensiteItems, XtNumber(pventLabelOptionsDensite[lng]));

   i = 0;
   string = XmStringCreateLtoR(labelDensite[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pventOptionsDensite); i++;
   XtSetArg(args[i], XmNmenuHistory, pventOptionsDensiteItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pventPanneauDensite = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pventPanneauDensite);
   
/**
**
**/

   
   pventOptionsLongueur = (Widget)XmCreatePulldownMenu(pventRc, labelLongueur[lng], NULL, 0);

   for (n=0; n < XtNumber(pventLabelOptionsLongueur[lng]); n++)
      {
      i = 0;
      string = XmStringCreateLtoR(pventLabelOptionsLongueur[lng][n], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, string); i++;
      pventOptionsLongueurItems[n] = XmCreatePushButtonGadget(pventOptionsLongueur, pventLabelOptionsLongueur[lng][n], args, i);
      XmStringFree(string);   
      XtAddCallback(pventOptionsLongueurItems[n], XmNactivateCallback, (XtCallbackProc) SetLongueurToggle, (XtPointer) n);
      }

   XtManageChildren(pventOptionsLongueurItems, XtNumber(pventLabelOptionsLongueur[lng]));

   i = 0;
   currentItem = 3;
   string = XmStringCreateLtoR(labelLongueur[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pventOptionsLongueur); i++;
   XtSetArg(args[i], XmNmenuHistory, pventOptionsLongueurItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pventPanneauLongueur = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
   XmStringFree(string);   
   
   XtManageChild(pventPanneauLongueur);
   
/**
**
**/

   pventOptionsEpaisseurF = (Widget)XmCreatePulldownMenu(pventRc, labelEpaisseurF[lng], NULL, 0);

   for (n=0; n < XtNumber(pventLabelOptionsEpaisseurF[lng]); n++)
      {
      i = 0;
      string = XmStringCreateLtoR(pventLabelOptionsEpaisseurF[lng][n], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, string); i++;
      pventOptionsEpaisseurFItems[n] = XmCreatePushButtonGadget(pventOptionsEpaisseurF, pventLabelOptionsEpaisseurF[lng][n], args, i);
      XmStringFree(string);   
      XtAddCallback(pventOptionsEpaisseurFItems[n], XmNactivateCallback, (XtCallbackProc) SetEpaisseurFToggle, (XtPointer) n);
      }

   XtManageChildren(pventOptionsEpaisseurFItems, XtNumber(pventLabelOptionsEpaisseurF[lng]));

   i = 0;
   currentItem = 0;
   string = XmStringCreateLtoR(labelEpaisseurF[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pventOptionsEpaisseurF); i++;
   XtSetArg(args[i], XmNmenuHistory, pventOptionsEpaisseurFItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pventPanneauEpaisseurF = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
   XmStringFree(string);   
   
   XtManageChild(pventPanneauEpaisseurF);
   
/**
**
**/

   pventOptionsCroissance = (Widget)XmCreatePulldownMenu(pventRc, labelCroissance[lng], NULL, 0);

   for (n=0; n < XtNumber(pventLabelOptionsCroissance[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(pventLabelOptionsCroissance[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	pventOptionsCroissanceItems[n] = XmCreatePushButtonGadget(pventOptionsCroissance, pventLabelOptionsCroissance[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(pventOptionsCroissanceItems[n], XmNactivateCallback, (XtCallbackProc) SetCroissanceToggle, (XtPointer) n);
	}

   XtManageChildren(pventOptionsCroissanceItems, XtNumber(pventLabelOptionsCroissance[lng]));

   i = 0;
   currentItem = 2;
   string = XmStringCreateLtoR(labelCroissance[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pventOptionsCroissance); i++;
   XtSetArg(args[i], XmNmenuHistory, pventOptionsCroissanceItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pventPanneauCroissance = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pventPanneauCroissance);

/**
**
**/


   pventOptionsEchelleWW = (Widget)XmCreatePulldownMenu(pventRc, labelEchelleWW[lng], NULL, 0);

   for (n=0; n < XtNumber(pventLabelOptionsEchelleWW[lng]); n++)
      {
      i = 0;
      string = XmStringCreateLtoR(pventLabelOptionsEchelleWW[lng][n], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, string); i++;
      pventOptionsEchelleWWItems[n] = XmCreatePushButtonGadget(pventOptionsEchelleWW, pventLabelOptionsEchelleWW[lng][n], args, i);
      XmStringFree(string);   
      XtAddCallback(pventOptionsEchelleWWItems[n], XmNactivateCallback, (XtCallbackProc) SetEchelleWWToggle, (XtPointer) n);
      }

   XtManageChildren(pventOptionsEchelleWWItems, XtNumber(pventLabelOptionsEchelleWW[lng]));

   i = 0;
   currentItem = 5;
   string = XmStringCreateLtoR(labelEchelleWW[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pventOptionsEchelleWW); i++;
   XtSetArg(args[i], XmNmenuHistory, pventOptionsEchelleWWItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pventPanneauEchelleWW = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
   XmStringFree(string);   
   
   XtManageChild(pventPanneauEchelleWW);
   
   }

ActiverPanneauVents()
{
   Colormap cmap;
   Arg args[2];
   int i;
   
   if (!pventTopLevel)
      InitPanneauVents();
   
   pventSelectionTerminee = False;
   
   if (!XtIsRealized(pventTopLevel))
      {
      XtRealizeWidget(pventTopLevel);
      CheckColormap(pventTopLevel);
      }
   
   f77name(xpventact)();
   
   }

f77name(xpventact)()
{
   LocalEventLoop(pventTopLevel);
   }


DesactiverPanneauVents()
{
   int i;
   
   XtUnrealizeWidget(pventTopLevel);
   }

float WindMgrGetDensite()
{
   return densite;
   }

int WindMgrGetCroissance()
{
   return croissance;
}

WindMgrGetDisplayMode()
{
   return displayMode;
}

WindMgrGetLongueur()
{
   return longueur;
}

WindMgrGetEpaisseur()
{
   return epaisseurF;
}

WindMgrGetModulusState()
{
   return flagModule;
}

WindMgrGetEchelleWW()
{
   return echelleWW;
}

f77name(c_swndatr)(item,valeur,lenItem,lenValeur)
char item[],valeur[];
{
   Arg args[10];
   int i;
   int indItem;
   char fakeWidgetName[32];

   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);

   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauVentsGeometrie,valeur);
      }
      
   if (0 == strcmp(item,"niveau_de_densite") || 0 == strcmp(item,"smoothing_factor"))
      {
      if (0 == strncmp("automati",valeur,8))
         {
         densite =-32767;
         printf("densite: %d\n", densite);
         }
      else
         {
         if (0 == strcmp(valeur,"maximum"))
            {
            densite = 32767;
            printf("densite: %d\n", densite);
            }
         else
            {
            densite = atoi(valeur);
            printf("densite: %d\n", densite);
            }
         }
      }

   if (0 == strcmp(item,"taille_police_labels") || 0 == strcmp(item,"labels_font_size"))
      {
      longueur = atoi(valeur);
      }

   if (0 == strcmp(item,"taille_police_legende_couleurs") || 0 == strcmp(item,"color_legend_font_size"))
      {
      croissance = atoi(valeur);
      }

   if (0 == strcmp(item,"taille_police_legende") || 0 == strcmp(item,"legend_font_size"))
      {
      densite = atoi(valeur);
      }
}

EcrWndAtr(fichierDemarrage)
FILE *fichierDemarrage;
{
   char tableau[32];
   char ligne[80];
   char item[32],valeur[32];
   int i;

   Arg  args[10];
   XmString label;
   XmFontList fontListe;
   char *geom;
   Window root;
   Position x,y;
   Display *disp;
   Window win;

   strcpy(tableau, "Vents_divers");
   
   strcpy(item,"geometrie");
   if (pventTopLevel)
      {
      disp = XtDisplay(pventTopLevel);
      win  = XtWindow(pventTopLevel);
      i = 0;
      XtSetArg(args[i], XmNx, &x); i++;
      XtSetArg(args[i], XmNy, &y); i++;
      XtGetValues(pventTopLevel, args, i);
      
      sprintf(valeur,"%+d%+d",x,y);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   else
      {
      if (strlen(panneauVentsGeometrie) > 0)
         {
         strcpy(valeur,panneauVentsGeometrie);
         sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
         fprintf(fichierDemarrage,"%s\n",ligne);
         }
      }
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   }
