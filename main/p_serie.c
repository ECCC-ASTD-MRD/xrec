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
#include <rec.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <souris.h>
#include <math.h>

#define AUTO_PROFIL 0
#define AUTO_GRILLES 1
#define FIXES       2

static int dimensionSerie;
extern int dimensionCoupe;
extern int echelle;
extern int calculMinMax;
extern int sensEchelle;
extern float grafMinX, grafMinY, grafMaxX, grafMaxY;
extern float grafMinUUtang,grafMaxUUtang,grafMinUUnorm,grafMaxUUnorm,grafMinUVW,grafMaxUVW,grafMinWW,grafMaxWW;

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern _Viewport    viewp;

XtCallbackProc PcsNouvelleSerie();
XtCallbackProc PcsScanProfilSerie();
XtCallbackProc PcsScanSerie();
XtCallbackProc PcsScanSerieVert();
XtCallbackProc PcsScanSerieHoriz();
XtCallbackProc PcsStop();
XtCallbackProc PcsSetIncrement();
XtCallbackProc PcsOk();

XtCallbackProc PcsAfficher();

XtCallbackProc PcsSetDimensionSerieZP();
XtCallbackProc PcsSetDimensionSerieT();

XtCallbackProc PcsSetEchelleLineaire();
XtCallbackProc PcsSetEchelleLog();

XtCallbackProc PcsSetMinMaxProfilSerie();
XtCallbackProc PcsSetMinMaxGrilles();
XtCallbackProc PcsSetMinMaxUsager();

XtCallbackProc PcsSetEchelleCroissante();
XtCallbackProc PcsSetEchelleDecroissante();

Widget pcsTopLevel = NULL;
Widget pcsForme, pcsFrame, pcsForme1, pcsFrame1, pcsForme2, pcsFrame3, pcsForme3, pcsRC, pcsRC2, pcsAfficher, pcsOk;
Widget pcsDimensionSerie, pcsDimensionSerieZ, pcsDimensionSerieT, pcsFrameDimensionSerie, pcsLabelDimensionSerie;
Widget pcsScanHoriz, pcsScanVert, pcsScanIncrement, pcsNouvelleSerie, pcsScanProfilSerie, pcsScanSerie,pcsStop;
Widget pcsFormeBoutons, pcsFormeBoutons2, pcsFormeEchelle, pcsRCLimite;
Widget pcsFrameBoutons, pcsFrameBoutons2, pcsFrameEchelle, pcsFrameLimite;
Widget pcsLabelEchelle, pcsFrameLineaire, pcsFrameSens, pcsLineariteEchelle, pcsSensEchelle;
Widget pcsLineaire, pcsLog, pcsCroissante, pcsDecroissante;
Widget pcsLabelLimite;
Widget pcsFrameTypeLimite, pcsFormeTypeLimite, pcsLimiteSerie,pcsLimiteDimensionSerie,pcsLimiteUsager;
Widget pcsFrameLimiteX, pcsRCLimiteX, pcsLabelX, pcsLabelMinX, pcsLabelMaxX, pcsTextMinX,pcsTextMaxX;
Widget pcsFrameLimiteY, pcsRCLimiteY, pcsLabelY, pcsLabelMinY, pcsLabelMaxY, pcsTextMinY,pcsTextMaxY;
Widget pcsFormeLimites, pcsFormeScan, pcsMainRC;

int pcsSelectionTerminee = False;
int fenetreSerie;
extern float cx1, cy1, cx2, cy2;

static float scanIncrement = 0.05;
static float lastcx1, lastcy1, lastcx2, lastcy2;
static int lastEvent = NIL;
int statutSerie = FALSE;

   char *messNouveauProfilSerie[] = {"\
Dans la fenetre d'affichage...\n\n\
clic bouton gauche --> profil\n\
drag bouton gauche + clic  --> Serie\n\
clic bouton droit --> Terminer","\
In the drawing window...\n\n\
clic left button --> profile\n\
drag left button + clic --> cross section\n\
clic right button --> exit"};

   char *messScanProfilSerie[] = {"\
Dans la fenetre d'affichage...\n\n\
bouton gauche enfonce --> profil\n\
lever le bouton gauche pour terminer", "\
In the drawing window...\n\n\
left button pressed --> profile\n\
release left button to exit"};

   char *messScanSerie[] = {"\
Dans la fenetre d'affichage...\n\n\
bouton gauche enfonce --> Serie\n\
lever le bouton gauche pour terminer", "\
In the drawing window...\n\n\
left button pressed --> cross section\n\
release left button to exit"};

static char  *labelProfilSerieVertical[] = {"Serie Temporelle", "Time Serie"};
static char  *labelEchelleVerticale[] = {"Echelle verticale", "Vertical Scale"};

static char  *labelEchelleLineaire[] = {"Lineaire", "Linear"};
static char  *labelEchelleLog[] = {"Log", "Log"};

static char  *labelEchelleCroissante[] = {"Croissante", "Normal"};
static char  *labelEchelleDecroissante[] = {"Decroissante", "Reversed"};

static char  *labelLimite[] = {"Limites", "Limits"};
static char  *labelAutoRegion[] = {"Auto (local)", "Auto (local)"};
static char  *labelAutoGrilles[] = {"Auto (global)", "Auto (global)"};
static char  *labelFixes[] = {"Fixes", "Fixed"};

static char  *labelAxeXMin[] = {"X min:", "X min:"};
static char  *labelAxeXMax[] = {"X max:", "X max:"};
static char  *labelAxeYMin[] = {"Y min:", "Y min:"};
static char  *labelAxeYMax[] = {"Y max:", "Y max:"};

/**
 ******************************************************************************
 ******************************************************************************
 **/


void InitPanneauSerie()
{
   int i;
   Arg args[16];
   XmString string;
   Pixel indCouleurs[16];
   XmString label;
   char nomShell[128];
 
   static char *nomPanneauSerie[] = {"PanneauSerie", "XSectionPanel"};
   static char *labelOk[] = {"Fermer", "Close"};
   static char *labelAfficher[] = {"Redessiner", "Redraw"};

   static char *labelScanHoriz[] = {"Scan (axe Y)", "Y axis Scan"};
   static char *labelScanVert[] = {"Scan (axe X)", "X axix Scan"};
   static char *labelStop[] = {"Stop", "Stop"};
   static char *labelScanIncrement[] = {"Increment (%)", "Increment (%)"};

   static char *labelNouvelleSerie[] = {"Nouveau\nprofil/Serie", "New profile\nor Xsection"};
   static char *labelScanProfilSerie[] = {"Scan\nprofil", "Scan\nprofile"};
   static char *labelScanSerie[] = {"Scan\nSerie", "Scan\nXsection"};

   int n,lng;
   Colormap cmap;

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauSerie[lng]);
   pcsTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);

   pcsForme = (Widget) XmCreateForm(pcsTopLevel, "form", NULL, 0);
   XtManageChild(pcsForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcsOk = (Widget)XmCreatePushButton(pcsForme, labelOk[lng], args, i);
   XtAddCallback(pcsOk, XmNactivateCallback, PcsOk, NULL);
   XtManageChild(pcsOk);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pcsOk); i++;
   pcsAfficher = (Widget)XmCreatePushButton(pcsForme, labelAfficher[lng], args, i);
   XtAddCallback(pcsAfficher, XmNactivateCallback, PcsAfficher, NULL);
   XtManageChild(pcsAfficher);

/**
****
**/

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcsFrame = (Widget) XmCreateFrame(pcsForme, "form", args, i);
   XtManageChild(pcsFrame);

   pcsMainRC = (Widget) XmCreateRowColumn(pcsFrame, "form", NULL, 0);
   XtManageChild(pcsMainRC);

   pcsFormeBoutons = (Widget) XmCreateForm(pcsMainRC, "form", NULL, 0);
   XtManageChild(pcsFormeBoutons);

   pcsFormeScan = (Widget) XmCreateForm(pcsMainRC, "form", NULL, 0);
   XtManageChild(pcsFormeScan);

   pcsFormeLimites = (Widget) XmCreateForm(pcsMainRC, "form", NULL, 0);
   XtManageChild(pcsFormeLimites);

   /* ---------------------------------------------------------------------- */

/**
****
**/


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsForme); i++;
   pcsFrameBoutons = (Widget) XmCreateFrame(pcsFormeBoutons, "form", args, i);
   XtManageChild(pcsFrameBoutons);

   i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pcsRC = XmCreateRowColumn(pcsFrameBoutons, "pcsRC", args, i);
   XtManageChild(pcsRC);

   i = 0;
   string = XmStringCreateLtoR(labelNouvelleSerie[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcsNouvelleSerie = (Widget)XmCreatePushButton(pcsRC, labelNouvelleSerie[lng], args, i);
   XtAddCallback(pcsNouvelleSerie, XmNactivateCallback, PcsNouvelleSerie, NULL);
   XtManageChild(pcsNouvelleSerie);

/**
****
**/

   i = 0;
   string = XmStringCreateLtoR(labelScanProfilSerie[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcsScanProfilSerie = (Widget)XmCreatePushButton(pcsRC, labelScanProfilSerie[lng], args, i);
   XtAddCallback(pcsScanProfilSerie, XmNactivateCallback, PcsScanProfilSerie, NULL);
   XtManageChild(pcsScanProfilSerie);

   i = 0;
   string = XmStringCreateLtoR(labelScanSerie[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcsScanSerie = (Widget)XmCreatePushButton(pcsRC, labelScanSerie[lng], args,i);
   XtAddCallback(pcsScanSerie, XmNactivateCallback, PcsScanSerie, NULL);
   XtManageChild(pcsScanSerie);


/**
****
**/

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsScanSerie); i++;
   pcsRCLimite = (Widget) XmCreateRowColumn(pcsFormeLimites, "form", args, i);
   XtManageChild(pcsRCLimite);

   pcsLabelLimite = (Widget) XmCreateLabel(pcsRCLimite, labelLimite[lng], NULL, 0);
   XtManageChild(pcsLabelLimite);

   pcsFrameTypeLimite = (Widget) XmCreateFrame(pcsRCLimite, "Forme", NULL, 0);
   XtManageChild(pcsFrameTypeLimite);

   pcsFormeTypeLimite = (Widget) XmCreateRadioBox(pcsFrameTypeLimite, "radiobox", NULL, 0);
   XtManageChild(pcsFormeTypeLimite);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcsLimiteSerie = (Widget) XmCreateToggleButton(pcsFormeTypeLimite, labelAutoRegion[lng], args, i);
   XtAddCallback(pcsLimiteSerie, XmNvalueChangedCallback, PcsSetMinMaxProfilSerie, NULL);
   XtManageChild(pcsLimiteSerie);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcsLimiteDimensionSerie = (Widget) XmCreateToggleButton(pcsFormeTypeLimite, labelAutoGrilles[lng], args, i);
   XtAddCallback(pcsLimiteDimensionSerie, XmNvalueChangedCallback, PcsSetMinMaxGrilles, NULL);
   XtManageChild(pcsLimiteDimensionSerie);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcsLimiteUsager = (Widget) XmCreateToggleButton(pcsFormeTypeLimite, labelFixes[lng], args, i);
   XtAddCallback(pcsLimiteUsager, XmNvalueChangedCallback, PcsSetMinMaxUsager, NULL);
   XtManageChild(pcsLimiteUsager);


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsScanSerie); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcsRCLimite); i++;
   pcsForme3 = (Widget) XmCreateForm(pcsFormeLimites, "form", args, i);
   XtManageChild(pcsForme3);


   pcsFrameLimiteX = (Widget) XmCreateFrame(pcsForme3, "Forme", NULL, 0);
   XtManageChild(pcsFrameLimiteX);

   pcsRCLimiteX = (Widget) XmCreateForm(pcsFrameLimiteX, "radiobox", NULL, 0);
   XtManageChild(pcsRCLimiteX);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM);i++;
   pcsLabelMinX = (Widget) XmCreateLabel(pcsRCLimiteX, labelAxeXMin[lng], args, i);
   XtManageChild(pcsLabelMinX);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsLabelMinX);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcsTextMinX = (Widget) XmCreateTextField(pcsRCLimiteX, "Min:", args, i);
   XtManageChild(pcsTextMinX);


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNtopWidget, pcsLabelX); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcsTextMinX);i++;
   pcsLabelMaxX = (Widget) XmCreateLabel(pcsRCLimiteX, labelAxeXMax[lng], args, i);
   XtManageChild(pcsLabelMaxX);

      
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsLabelMaxX);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcsTextMinX);i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcsTextMaxX = (Widget) XmCreateTextField(pcsRCLimiteX, "Max X:", args, i);
   XtManageChild(pcsTextMaxX);


   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsFrameLimiteX); i++;   
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcsFrameLimiteY = (Widget) XmCreateFrame(pcsForme3, "Forme", args, i);
   XtManageChild(pcsFrameLimiteY);

   pcsRCLimiteY = (Widget) XmCreateForm(pcsFrameLimiteY, "radiobox", NULL, 0);
   XtManageChild(pcsRCLimiteY);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcsLabelMinY = (Widget) XmCreateLabel(pcsRCLimiteY, labelAxeYMin[lng], args, i);
   XtManageChild(pcsLabelMinY);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsLabelMinY);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00");i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcsTextMinY = (Widget) XmCreateTextField(pcsRCLimiteY, "Min:", args, i);
   XtManageChild(pcsTextMinY);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcsTextMinY); i++;
   pcsLabelMaxY = (Widget) XmCreateLabel(pcsRCLimiteY, labelAxeYMax[lng], args, i);
   XtManageChild(pcsLabelMaxY);
      
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsLabelMaxY); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcsTextMinY);i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcsTextMaxY = (Widget) XmCreateTextField(pcsRCLimiteY, "Max:", args, i);
   XtManageChild(pcsTextMaxY);

   /* ---------------------------------------------------------------------------- */

   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcsFrameBoutons2 = (Widget) XmCreateFrame(pcsFormeScan, "form", args, i);
   XtManageChild(pcsFrameBoutons2);


   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pcsRC2 = XmCreateRowColumn(pcsFrameBoutons2, "pcsRC", args, i);
   XtManageChild(pcsRC2);

   i = 0;
   string = XmStringCreateLtoR(labelScanHoriz[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcsScanHoriz = (Widget)XmCreatePushButton(pcsRC2, labelScanHoriz[lng], args, i);
   XtAddCallback(pcsScanHoriz, XmNactivateCallback, PcsScanSerieHoriz, NULL);
   XtManageChild(pcsScanHoriz);

   i = 0;
   string = XmStringCreateLtoR(labelScanVert[lng], XmSTRING_DEFAULT_CHARSET);
   pcsScanVert = (Widget)XmCreatePushButton(pcsRC2, labelScanVert[lng], args, i);
   XtAddCallback(pcsScanVert, XmNactivateCallback, PcsScanSerieVert, NULL);
   XtManageChild(pcsScanVert);

   i = 0;
   string = XmStringCreateLtoR(labelStop[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcsStop = (Widget)XmCreatePushButton(pcsRC2, labelStop[lng], args,i);
   XtAddCallback(pcsStop, XmNactivateCallback, PcsStop, NULL);
   XtManageChild(pcsStop);

   label = XmStringCreateLtoR(labelScanIncrement[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcsFrameBoutons2); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 2); i++;
   XtSetArg(args[i], XmNminimum, 0); i++;
   XtSetArg(args[i], XmNmaximum, 1000); i++;
   XtSetArg(args[i], XmNvalue, 500); i++;
   pcsScanIncrement = (Widget)XmCreateScale(pcsFormeScan, labelScanIncrement[lng], args, i);
   XtManageChild(pcsScanIncrement);
   XmStringFree(label);
   
   XtAddCallback(pcsScanIncrement, XmNdragCallback, PcsSetIncrement, NULL);
   XtAddCallback(pcsScanIncrement, XmNvalueChangedCallback, PcsSetIncrement, NULL);

   dimensionCoupe = T;
   dimensionSerie = T;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


ActiverPanneauSerie()
{
   XEvent pcsEvent;
   Widget pcsWidgetParent;

   Colormap cmap;
   Arg args[2];
   int i;


   if (!pcsTopLevel)
      {
      InitPanneauSerie();
      }
   
   if (!XtIsRealized(pcsTopLevel))
      {
      XtRealizeWidget(pcsTopLevel);
      i = 0;
      XtSetValues(pcsTopLevel, args, i);
      }
   
   f77name(xpanpcsact)();
   
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


f77name(xpanpcsact)()
{
   LocalEventLoop(pcsTopLevel);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/



DesactiverPanneauSerie()
{
   XtUnrealizeWidget(pcsTopLevel);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/



XtCallbackProc PcsOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int fenetreAffichage;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   EnleverLigneSerie(cx1, cy1, cx2, cy2);

   pcsSelectionTerminee = TRUE;
   EffacerSerie();
   if (fenetreSerie)
      {
      c_wglsetw(fenetreSerie);
      c_wglclw(fenetreSerie);
      }

   statutSerie = False;
   fenetreSerie = 0;
   DesactiverPanneauSerie();
/**   ReAfficherBoutonsAnimation(); **/
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcsAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   RedessinerFenetreSerie();

}

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcsNouvelleSerie(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i, ier, ok;
   int fenetreAffichage;
   float x, y;
   int ix, iy, iix, iiy;
   int lastiix = -1;
   int lastiiy = -1;
   int wwidth = 0;
   int wheight = 0;
   int event;
   int nbChampsActifs, SerieValideTrouvee;
   _Champ *champ;
   int lng;

   static char *labelTopLevel[] = {"Series_temporelles", "Time_Series"};
   dimensionCoupe = T;
   lng = c_getulng();

   InvertWidget(w);
   if (!fenetreSerie)
      {
      if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
      SetGeometrieFenetreAffichage(labelTopLevel[lng]);
      fenetreSerie = c_wglopw(labelTopLevel[lng]);
      c_wglias(1);
      c_wglgwz(&wwidth, &wheight);
      lastcx1 = 0.0; lastcy1 = 0.0; lastcx2 = 0.0; lastcy2 = 0.0;
      }

   c_wglsetw(fenetreSerie);
   ier = PreparerSerie(1.0, 1.0, 1.0, 1.0);

   SerieMgrSetMinMax();
   SerieMgrSetUVWMinMax();
   

   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);
   MessageAide(messNouveauProfilSerie[lng]);

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

   if (ier > 0)
      {
      InvertWidget(w);
      return;
      }

   SerieValideTrouvee = VerifierExistenceSerieValide();
   if (SerieValideTrouvee == 0)
      {
      InvertWidget(w);
      c_wglsetw(fenetreSerie);
      c_wglcol(NOIR);
      c_wglclr();
      c_wglsetw(fenetreAffichage);
      return;
      }

   statutSerie = TRUE;
   lastEvent = NIL;
   EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
   Ligne(&event, &cx1, &cy1, &cx2, &cy2);

   while (event != RESET && event != NIL)
      {
      c_wglsetw(fenetreAffichage);
      c_wglcol(CYAN);
      EnterOverlayMode();
      switch (lastEvent)
	 {
	 case CLIC:
	 case BACK_CLIC:
	 TracerCercle(lastcx1, lastcy1);
	 break;

	 case MOVE:
	 TracerLigne(lastcx1, lastcy1, lastcx2, lastcy2);
	 break;

	 default:
	 break;
	 }
      RestoreNormalMode();

      c_wglcol(CYAN);
      switch (event)
	 {
	 case CLIC:
	 case BACK_CLIC:
	 EnterOverlayMode();
	 cx1 = (float)(ROUND(cx1));
	 cy1 = (float)(ROUND(cy1));
	 cx2 = cx1; cy2 = cy1;
	 TracerCercle(cx1, cy1);
	 RestoreNormalMode();
	 
	 c_wglsetw(fenetreSerie);
	 ier = PreparerSerie(cx1, cy1, cx2, cy2);
	 RedessinerFenetreSerie();
	 c_wglsetw(fenetreAffichage);
	 break;
	 
	 case MOVE:
	 c_wglsetw(fenetreAffichage);
	 EnterOverlayMode();
	 TracerLigne(cx1, cy1, cx2, cy2);
	 lastcx1 = cx1; lastcy1 = cy1; lastcx2 = cx2; lastcy2 = cy2;
	 RestoreNormalMode();
	 
	 c_wglsetw(fenetreSerie);
	 if (c_wglgdbg())
	    {
	    printf("cx1:%f cy1:%f, cx2:%f cy2:%f\n", cx1,cy1,cx2,cy2);
	    }
	 ier = PreparerSerie(cx1, cy1, cx2, cy2);
	 RedessinerFenetreSerie();

	 c_wglsetw(fenetreAffichage);
	 break;
	 }
      
      lastEvent = event;
      lastcx1 = cx1; lastcx2 = cx2; lastcy1 = cy1; lastcy2 = cy2;
      Ligne(&event, &cx1, &cy1, &cx2, &cy2);
      } 
   
   FldMgrGetChamp(&champ, 0);
   ok = champ->seqanim.nbFldsAnim;
   
   if (ok)
      {
      }
   else
      {
      c_wglsetw(fenetreSerie);
      c_wglcol(NOIR);
      c_wglclr();
      }
   
   c_wglsetw(fenetreAffichage);
   InvertWidget(w);
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcsScanSerie(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i, ier;
   int fenetreAffichage;
   float x, y;
   int ix, iy, iix, iiy;
   int lastiix = -1;
   int lastiiy = -1;
   int wwidth = 0;
   int wheight = 0;
   int event;
   _Champ *champ;
   int lng;

   dimensionCoupe = T;
   SetIgnoreMode();
   lng = c_getulng();

   InvertWidget(w);
   if (!fenetreSerie)
      {
      if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
      fenetreSerie = c_wglopw("Series_temporelles");
      c_wglias(1);
      c_wglgwz(&wwidth, &wheight);
      }

   c_wglsetw(fenetreSerie);
   ier = PreparerSerie(1.0, 1.0, 1.0, 1.0);
   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);
   MessageAide(messScanSerie[lng]);

   c_wgldbf();
   c_wglbbf();

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

   if (ier < 0)
      {
      InvertWidget(w);
      UnsetIgnoreMode();
      return;
      }

   statutSerie = TRUE;

   event = c_wglbtx(&cx1, &cy1);
   c_fxfy2xy(&cx1, &cy1, cx1, cy1);
   while (c_wglbtpx(BGAUCH, &cx2, &cy2))
      {
      c_wglsetw(fenetreAffichage);
      c_fxfy2xy(&cx2, &cy2, cx2, cy2);

      if (cx1 != cx2 || cy1 != cy2)
	 {
	 EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
	 EnterOverlayMode();
	 c_wglcol(CYAN);
	 TracerLigne(cx1, cy1, cx2, cy2);
	 RestoreNormalMode();
	 
	 c_wglsetw(fenetreSerie);
	 ier = PreparerSerie(cx1, cy1, cx2, cy2);
	 RedessinerFenetreSerie();
	 c_wglswb();
	 c_wglsetw(fenetreAffichage);
	 
	 lastcx1 = cx1;
	 lastcx2 = cx2;
	 lastcy1 = cy1;
	 lastcy2 = cy2;
	 }
      } 
   
   c_wglsetw(fenetreSerie);
   c_wglsbf();
   c_wglsetw(fenetreAffichage);
   c_wglsbf();
   InvertWidget(w);
   UnsetIgnoreMode();
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/
XtCallbackProc PcsScanSerieVert(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i, ier;
   int fenetreAffichage;
   float x, y;
   int ix, iy, iix, iiy;
   float yinit, xdeb, ydeb, xfin, yfin;
   int ideb, jdeb, ifin, jfin;
   int lastiix = -1;
   int lastiiy = -1;
   int wwidth = 0;
   int wheight = 0;
   int event;
   _Champ *champ;
   int lng;
   int largeurFenetre, hauteurFenetre;
   float xdel, ydel;
   static int status = 0;

   dimensionCoupe = T;
   if (status == 1)
     {
     status = 0;
     xc.annulationDemandee = True;
     }
   else
     {
     status = 1;
     xc.annulationDemandee = False;
     }
   
   lng = c_getulng();

   InvertWidget(w);
   if (status == 1)
     {
     if (!fenetreSerie)
       {
       if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
       fenetreSerie = c_wglopw("ProfilSerie");
       c_wglias(1);
       c_wglgwz(&wwidth, &wheight);
       }
     
     GetFenetreAffichageID(&fenetreAffichage);
     c_wglsetw(fenetreAffichage);
     EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     c_wglsetw(fenetreSerie);
     ier = PreparerSerie(1.0, 1.0, 1.0, 1.0);
     c_wglcol(NOIR);
     c_wglclr();
     c_wglcol(BLANC);
     
     c_wgldbf();
     c_wglbbf();
     
     EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
     GetFenetreAffichageID(&fenetreAffichage);
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     if (ier < 0)
       {
       InvertWidget(w);
       return;
       }
     
     yinit = ydeb;
     statutSerie = TRUE;
     }
     
   if (status == 0)
     xc.annulationDemandee = True;
   
   SetIgnoreMode();
   while (!c_wglanul() && status == 1)
     {
     c_wglsetw(fenetreAffichage);
     c_xy2fxfy(&cx1, &cy1, xdeb, ydeb);
     c_xy2fxfy(&cx2, &cy2, xfin, ydeb);
     
     EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
     EnterOverlayMode();
     c_wglcol(CYAN);
     TracerLigne(cx1, cy1, cx2, cy2);
     RestoreNormalMode();
     
     c_wglsetw(fenetreSerie);
     ier = PreparerSerie(cx1, cy1, cx2, cy2);
     xc.statuts[AXE_Y] = TRUE;
     RedessinerFenetreSerie();
     c_wglswb();
     c_wglsetw(fenetreAffichage);
      
     lastcx1 = cx1;
     lastcx2 = cx2;
     lastcy1 = cy1;
     lastcy2 = cy2;
     ydeb += (scanIncrement * (yfin - yinit));
     if (ydeb > yfin || ydeb < yinit)
       {
       scanIncrement = -scanIncrement;
       if (ydeb < yinit)
	 ydeb = yinit;
       if (ydeb > yfin)
	 ydeb = yfin;
       }
      }
   
   xc.annulationDemandee = False;
   UnsetIgnoreMode();
   c_wglsetw(fenetreSerie);
   c_wglfbf();
   xc.statuts[AXE_Y] = TRUE;
   RedessinerFenetreSerie();
   c_wglsetw(fenetreAffichage);
   c_wglfbf();
   xc.statuts[AXE_Y] = FALSE;
   RedessinerFenetreAffichage();
   status=0;
   InvertWidget(w);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

XtCallbackProc PcsScanSerieHoriz(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i, ier;
   int fenetreAffichage;
   float x, y;
   int ix, iy, iix, iiy;
   float xinit, xdeb, ydeb, xfin, yfin;
   int ideb, jdeb, ifin, jfin;
   int lastiix = -1;
   int lastiiy = -1;
   int wwidth = 0;
   int wheight = 0;
   int event;
   _Champ *champ;
   int lng;
   int largeurFenetre, hauteurFenetre;
   float xdel, ydel;
   static int status = 0;

   dimensionCoupe = T;
   if (status == 1)
     {
     status = 0;
     xc.annulationDemandee = True;
     }
   else
     {
     status = 1;
     xc.annulationDemandee = False;
     }
   
   lng = c_getulng();

   InvertWidget(w);
   if (status == 1)
     {
     if (!fenetreSerie)
       {
       if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
       fenetreSerie = c_wglopw("ProfilSerie");
       c_wglias(1);
       c_wglgwz(&wwidth, &wheight);
       }
     
     GetFenetreAffichageID(&fenetreAffichage);
     c_wglsetw(fenetreAffichage);
     EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     c_wglsetw(fenetreSerie);
     ier = PreparerSerie(1.0, 1.0, 1.0, 1.0);
     c_wglcol(NOIR);
     c_wglclr();
     c_wglcol(BLANC);
     
     c_wgldbf();
     c_wglbbf();
     
     EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
     GetFenetreAffichageID(&fenetreAffichage);
     c_wglsetw(fenetreAffichage);
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     if (ier < 0)
       {
       InvertWidget(w);
       return;
       }
     
     xinit = xdeb;
     statutSerie = TRUE;
     }

   if (status == 0)
     xc.annulationDemandee = True;

   SetIgnoreMode();
   while (!c_wglanul() && status == 1)
      {
      c_wglsetw(fenetreAffichage);
      c_xy2fxfy(&cx1, &cy1, xdeb, ydeb);
      c_xy2fxfy(&cx2, &cy2, xdeb, yfin);

      EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
      c_wglcol(CYAN);
      TracerLigne(cx1, cy1, cx2, cy2);
      RestoreNormalMode();
	 
      c_wglsetw(fenetreSerie);
      ier = PreparerSerie(cx1, cy1, cx2, cy2);
      xc.statuts[AXE_Y] = TRUE;
      RedessinerFenetreSerie();
      c_wglswb();
      c_wglsetw(fenetreAffichage);
      
      lastcx1 = cx1;
      lastcx2 = cx2;
      lastcy1 = cy1;
      lastcy2 = cy2;
      xdeb += (scanIncrement * (xfin - xinit));
      if (xdeb > xfin || xdeb < xinit)
	{
	scanIncrement = -scanIncrement;
	if (xdeb < xinit)
	  xdeb = xinit;
	if (xdeb > xfin)
	  xdeb = xfin;
	}
      }
   
   xc.annulationDemandee = False;
   UnsetIgnoreMode();
   c_wglsetw(fenetreSerie);
   c_wglfbf();
   xc.statuts[AXE_Y] = TRUE;
   RedessinerFenetreSerie();
   c_wglsetw(fenetreAffichage);
   c_wglfbf();
   xc.statuts[AXE_Y] = FALSE;
   RedessinerFenetreAffichage();
   
   status=0;
   InvertWidget(w);
   }



XtCallbackProc PcsScanProfilSerie(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i, ier;
   int fenetreAffichage;
   float x, y;
   int ix, iy, iix, iiy;
   int lastiix = -1;
   int lastiiy = -1;
   int wwidth = 0;
   int wheight = 0;
   int event;
   int lng;
   
   dimensionCoupe = T;
   SetIgnoreMode();
   lng = c_getulng();
   
   InvertWidget(w);
   InitFenetreSerie();
   c_wglsetw(fenetreSerie);
   ier = PreparerSerie(1.0, 1.0, 1.0, 1.0);

   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);
   MessageAide(messScanProfilSerie[lng]);

   c_wgldbf();
   c_wglbbf();

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

   if (ier < 0)
      {
      InvertWidget(w);
      UnsetIgnoreMode();
      return;
      }

   statutSerie = TRUE;
   
   event = c_wglbtx(&cx1, &cy1);
   c_fxfy2xy(&cx1, &cy1, cx1, cy1);
   while (c_wglbtpx(BGAUCH, &cx1, &cy1))
      {
      c_fxfy2xy(&cx1, &cy1, cx1, cy1);
      cx2 = cx1;
      cy2 = cy1;
      c_wglsetw(fenetreAffichage);

      EnleverLigneSerie(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
#ifdef GL_WGL
      color(1);
#endif
#ifdef X_WGL
      c_wglcol(CYAN);
#endif
      TracerCercle(cx1, cy1);
      RestoreNormalMode();

      c_wglsetw(fenetreSerie);
      ier = PreparerSerie(cx1, cy1, cx2, cy2);
      RedessinerFenetreSerie();
      c_wglswb();
      c_wglsetw(fenetreAffichage);

      lastcx1 = cx1;
      lastcx2 = cx2;
      lastcy1 = cy1;
      lastcy2 = cy2;
      } 
   
   c_wglsetw(fenetreSerie);
   c_wglfbf();
   c_wglsetw(fenetreAffichage);
   EnleverLigneSerie(cx1, cy1, cx2, cy2);
   InvertWidget(w);
   UnsetIgnoreMode();
   }


XtCallbackProc PcsStop(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
  xc.annulationDemandee = TRUE;
   }

XtCallbackProc PcsSetDimensionSerieZP(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   dimensionSerie = ZP;
   }

XtCallbackProc PcsSetDimensionSerieT(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   }

XtCallbackProc PcsSetEchelleLineaire(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   echelle = LINEAIRE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreSerie();
   }

XtCallbackProc PcsSetEchelleLog(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   echelle = LOG;
   if (XmToggleButtonGetState(w))
   RedessinerFenetreSerie();
   }

XtCallbackProc PcsSetMinMaxProfilSerie(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   calculMinMax = AUTO_PROFIL;
   PcsDesactiverTextWidgets();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreSerie();
   }

XtCallbackProc PcsSetMinMaxGrilles(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   calculMinMax = AUTO_GRILLES;
   PcsDesactiverTextWidgets();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreSerie();
   }

XtCallbackProc PcsSetMinMaxUsager(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   calculMinMax = FIXES;
   PcsActiverTextWidgets();
   }

PcsActiverTextWidgets()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, True); i++;
   XtSetArg(args[i], XmNsensitive, True); i++;
   
   XtSetValues(pcsTextMinX, args, i);
   XtSetValues(pcsTextMinY, args, i);
   XtSetValues(pcsTextMaxX, args, i);
   XtSetValues(pcsTextMaxY, args, i);
   }

PcsDesactiverTextWidgets()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   XtSetArg(args[i], XmNsensitive, False); i++;
   
   XtSetValues(pcsTextMinX, args, i);
   XtSetValues(pcsTextMinY, args, i);
   XtSetValues(pcsTextMaxX, args, i);
   XtSetValues(pcsTextMaxY, args, i);
   }

static XtCallbackProc PcsSetIncrement(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{
  XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
  
  if (scanIncrement < 0.0)
    {
    scanIncrement = -0.0001 * (float)(donnees->value);
    }
  else
    { 
    scanIncrement = 0.0001 * (float)(donnees->value);
    }
  
}

SerieMgrSetMinMax()
{
  SerieMgrSetMinMaxSerie();
}

SerieMgrSetUVWMinMax()
{
  CoupeMgrSetUVWMinMax();
}

