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

extern int dimensionCoupe;
extern int echelle;
extern int calculMinMax;
extern int sensEchelle;
extern float grafMinX, grafMinY, grafMaxX, grafMaxY;
extern float grafMinUUtang,grafMaxUUtang,grafMinUUnorm,grafMaxUUnorm,grafMinUVW,grafMaxUVW,grafMinWW,grafMaxWW;

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern _Viewport    viewp;

XtCallbackProc PcpNouvelleCoupe();
XtCallbackProc PcpScanProfil();
XtCallbackProc PcpScanCoupe();
XtCallbackProc PcpScanCoupeVert();
XtCallbackProc PcpScanCoupeHoriz();
XtCallbackProc PcpSetIncrement();
XtCallbackProc PcpOk();

XtCallbackProc PcpAfficher();

XtCallbackProc PcpSetDimensionCoupeZP();
XtCallbackProc PcpSetDimensionCoupeT();

XtCallbackProc PcpSetEchelleLineaire();
XtCallbackProc PcpSetEchelleLog();

XtCallbackProc PcpSetMinMaxProfil();
XtCallbackProc PcpSetMinMaxGrilles();
XtCallbackProc PcpSetMinMaxUsager();

XtCallbackProc PcpSetEchelleCroissante();
XtCallbackProc PcpSetEchelleDecroissante();

Widget pcpTopLevel = NULL;
Widget pcpForme, pcpFrame, pcpForme1, pcpFrame1, pcpForme2, pcpFrame3, pcpForme3, pcpRC, pcpRC2, pcpAfficher, pcpOk;
Widget pcpDimensionCoupe, pcpDimensionCoupeZ, pcpDimensionCoupeT, pcpFrameDimensionCoupe, pcpLabelDimensionCoupe;
Widget pcpScanHoriz, pcpScanVert, pcpScanIncrement, pcpNouvelleCoupe, pcpScanProfil, pcpScanCoupe;
Widget pcpFormeBoutons, pcpFormeBoutons2, pcpFormeEchelle, pcpFormeLimite;
Widget pcpFrameBoutons, pcpFrameBoutons2, pcpFrameEchelle, pcpFrameLimite;
Widget pcpLabelEchelle, pcpFrameLineaire, pcpFrameSens, pcpLineariteEchelle, pcpSensEchelle;
Widget pcpLineaire, pcpLog, pcpCroissante, pcpDecroissante;
Widget pcpLabelLimite;
Widget pcpFrameTypeLimite, pcpFormeTypeLimite, pcpLimiteCoupe,pcpLimiteDimensionCoupe,pcpLimiteUsager;
Widget pcpFrameLimiteX, pcpFormeLimiteX, pcpLabelX, pcpLabelMinX, pcpLabelMaxX, pcpTextMinX,pcpTextMaxX;
Widget pcpFrameLimiteY, pcpFormeLimiteY, pcpLabelY, pcpLabelMinY, pcpLabelMaxY, pcpTextMinY,pcpTextMaxY;

int pcpSelectionTerminee = False;
extern int fenetreCoupe;
extern float cx1, cy1, cx2, cy2;

static float scanIncrement = 0.05;
static float lastcx1, lastcy1, lastcx2, lastcy2;
int lastEvent = NIL;
int statutCoupe = FALSE;

   char *messNouveauProfil[] = {"\
Dans la fenetre d'affichage...\n\n\
clic bouton gauche --> profil\n\
drag bouton gauche + clic  --> coupe\n\
clic bouton droit --> Terminer","\
In the drawing window...\n\n\
clic left button --> profile\n\
drag left button + clic --> cross section\n\
clic right button --> exit"};

   char *messScanProfil[] = {"\
Dans la fenetre d'affichage...\n\n\
bouton gauche enfonce --> profil\n\
lever le bouton gauche pour terminer", "\
In the drawing window...\n\n\
left button pressed --> profile\n\
release left button to exit"};

   char *messScanCoupe[] = {"\
Dans la fenetre d'affichage...\n\n\
bouton gauche enfonce --> coupe\n\
lever le bouton gauche pour terminer", "\
In the drawing window...\n\n\
left button pressed --> cross section\n\
release left button to exit"};

static char *labelTopLevel[] = {"Profils_Coupes", "Profiles_XSections"};

char *labelDimensionCoupe[]  = {"Dimension: ", "Dimension: "};
char *labelDimensionCoupeZ[] = {"Coord. vert.","Vert. Coord."};
char *labelDimensionCoupeT[] = {"Temps","Time"};

char *labelProfilVertical[] = {"Profil Vertical", "Vertical Profile"};
char *labelEchelleVerticale[] = {"Echelle verticale", "Vertical Scale"};

char *labelEchelleLineaire[] = {"Lineaire", "Linear"};
char *labelEchelleLog[] = {"Log", "Log"};

char *labelEchelleCroissante[] = {"Croissante", "Normal"};
char *labelEchelleDecroissante[] = {"Decroissante", "Reversed"};

char *labelLimite[] = {"Limites", "Limits"};
char *labelAutoRegion[] = {"Auto (local)", "Auto (local)"};
char *labelAutoGrilles[] = {"Auto (global)", "Auto (global)"};
char *labelFixes[] = {"Fixes", "Fixed"};

char *labelAxeXMin[] = {"X min:", "X min:"};
char *labelAxeXMax[] = {"X max:", "X max:"};
char *labelAxeYMin[] = {"Y min:", "Y min:"};
char *labelAxeYMax[] = {"Y max:", "Y max:"};

/**
 ******************************************************************************
 ******************************************************************************
 **/


void InitPanneauCoupe()
{
   int i;
   Arg args[16];
   XmString string;
   XmString label;
   char nomShell[128];
 
   static char *nomPanneauCoupe[] = {"PanneauCoupe", "XSectionPanel"};
   static char *labelOk[] = {"Fermer", "Close"};
   static char *labelAfficher[] = {"Redessiner", "Redraw"};

   static char *labelScanHoriz[] = {"Scan (axe Y)", "Y axis Scan"};
   static char *labelScanVert[] = {"Scan (axe X)", "X axix Scan"};
   static char *labelScanIncrement[] = {"Increment (%)", "Increment (%)"};

   static char *labelNouvelleCoupe[] = {"Nouveau\nprofil/coupe", "New profile\nor Xsection"};
   static char *labelScanProfil[] = {"Scan\nprofil", "Scan\nprofile"};
   static char *labelScanCoupe[] = {"Scan\ncoupe", "Scan\nXsection"};

   int lng;

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauCoupe[lng]);
   pcpTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);

   pcpForme = (Widget) XmCreateForm(pcpTopLevel, "form", NULL, 0);
   XtManageChild(pcpForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcpOk = (Widget)XmCreatePushButton(pcpForme, labelOk[lng], args, i);
   XtAddCallback(pcpOk, XmNactivateCallback, PcpOk, NULL);
   XtManageChild(pcpOk);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pcpOk); i++;
   pcpAfficher = (Widget)XmCreatePushButton(pcpForme, labelAfficher[lng], args, i);
   XtAddCallback(pcpAfficher, XmNactivateCallback, PcpAfficher, NULL);
   XtManageChild(pcpAfficher);

/**
****
**/

  i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcpFrame = (Widget) XmCreateFrame(pcpForme, "form", args, i);
   XtManageChild(pcpFrame);

   pcpForme2 = (Widget) XmCreateForm(pcpFrame, "form", NULL, 0);
   XtManageChild(pcpForme2);

/**
****
**/


   pcpFormeBoutons = (Widget) XmCreateForm(pcpForme2, "form", NULL, 0);
   XtManageChild(pcpFormeBoutons);

   i = 0;
   pcpFrameBoutons = (Widget) XmCreateFrame(pcpFormeBoutons, "pcpFrameBoutons", args, i);
   XtManageChild(pcpFrameBoutons);

   i = 0;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pcpRC = XmCreateRowColumn(pcpFrameBoutons, "pcpRC", args, i);
   XtManageChild(pcpRC);

   i = 0;
   string = XmStringCreateLtoR(labelNouvelleCoupe[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpNouvelleCoupe = (Widget)XmCreatePushButton(pcpRC, labelNouvelleCoupe[lng], args, i);
   XtAddCallback(pcpNouvelleCoupe, XmNactivateCallback, PcpNouvelleCoupe, NULL);
   XtManageChild(pcpNouvelleCoupe);

/**
****
**/

   i = 0;
   string = XmStringCreateLtoR(labelScanProfil[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpScanProfil = (Widget)XmCreatePushButton(pcpRC, labelScanProfil[lng], args, i);
   XtAddCallback(pcpScanProfil, XmNactivateCallback, PcpScanProfil, NULL);
   XtManageChild(pcpScanProfil);

   i = 0;
   string = XmStringCreateLtoR(labelScanCoupe[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpScanCoupe = (Widget)XmCreatePushButton(pcpRC, labelScanCoupe[lng], args,i);
   XtAddCallback(pcpScanCoupe, XmNactivateCallback, PcpScanCoupe, NULL);
   XtManageChild(pcpScanCoupe);


/**
****
**/


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFrameBoutons); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcpFrameBoutons2 = (Widget) XmCreateFrame(pcpFormeBoutons, "form", args, i);
   XtManageChild(pcpFrameBoutons2);


   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pcpRC2 = XmCreateRowColumn(pcpFrameBoutons2, "pcpRC", args, i);
   XtManageChild(pcpRC2);

   i = 0;
   string = XmStringCreateLtoR(labelScanHoriz[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpScanHoriz = (Widget)XmCreatePushButton(pcpRC2, labelScanHoriz[lng], args, i);
   XtAddCallback(pcpScanHoriz, XmNactivateCallback, PcpScanCoupeHoriz, NULL);
   XtManageChild(pcpScanHoriz);

   i = 0;
   string = XmStringCreateLtoR(labelScanVert[lng], XmSTRING_DEFAULT_CHARSET);
   pcpScanVert = (Widget)XmCreatePushButton(pcpRC2, labelScanVert[lng], args, i);
   XtAddCallback(pcpScanVert, XmNactivateCallback, PcpScanCoupeVert, NULL);
   XtManageChild(pcpScanVert);

   label = XmStringCreateLtoR(labelScanIncrement[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFrameBoutons2); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 2); i++;
   XtSetArg(args[i], XmNminimum, 0); i++;
   XtSetArg(args[i], XmNmaximum, 1000); i++;
   XtSetArg(args[i], XmNvalue, 500); i++;
   pcpScanIncrement = (Widget)XmCreateScale(pcpFormeBoutons, labelScanIncrement[lng], args, i);
   XtManageChild(pcpScanIncrement);
   XmStringFree(label);
   
   XtAddCallback(pcpScanIncrement, XmNdragCallback, PcpSetIncrement, NULL);
   XtAddCallback(pcpScanIncrement, XmNvalueChangedCallback, PcpSetIncrement, NULL);




   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeBoutons); i++;
   pcpFrameEchelle = (Widget) XmCreateFrame(pcpForme2, "form",args, i);
   XtManageChild(pcpFrameEchelle);

   pcpFormeEchelle = (Widget) XmCreateRowColumn(pcpFrameEchelle, "form", NULL, 0);
   XtManageChild(pcpFormeEchelle);

   pcpLabelEchelle = (Widget) XmCreateLabel(pcpFormeEchelle, labelEchelleVerticale[lng], NULL, 0);
   XtManageChild(pcpLabelEchelle);

   pcpFrameLineaire = (Widget) XmCreateFrame(pcpFormeEchelle, "frame", NULL, 0);
   XtManageChild(pcpFrameLineaire);

   pcpLineariteEchelle = (Widget) XmCreateRadioBox(pcpFrameLineaire, "radiobox", NULL, 0);
   XtManageChild(pcpLineariteEchelle);

/**
****
**/

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLineaire = (Widget) XmCreateToggleButton(pcpLineariteEchelle, labelEchelleLineaire[lng], args, i);
   XtAddCallback(pcpLineaire, XmNvalueChangedCallback, PcpSetEchelleLineaire, NULL);
   XtManageChild(pcpLineaire);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLog = (Widget)XmCreateToggleButton(pcpLineariteEchelle, labelEchelleLog[lng], args, i);
   XtAddCallback(pcpLog, XmNvalueChangedCallback, PcpSetEchelleLog, NULL);
   XtManageChild(pcpLog);
   
   pcpFrameSens = (Widget) XmCreateFrame(pcpFormeEchelle, "frame", NULL, 0);
   XtManageChild(pcpFrameSens);

   pcpSensEchelle = XmCreateRadioBox(pcpFrameSens, "radiobox", NULL, 0);
   XtManageChild(pcpSensEchelle);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpCroissante = (Widget) XmCreateToggleButton(pcpSensEchelle, labelEchelleCroissante[lng], args, i);
   XtAddCallback(pcpCroissante, XmNvalueChangedCallback, PcpSetEchelleCroissante, NULL);
   XtManageChild(pcpCroissante);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpDecroissante = (Widget) XmCreateToggleButton(pcpSensEchelle, labelEchelleDecroissante[lng], args, i);
   XtAddCallback(pcpDecroissante, XmNvalueChangedCallback, PcpSetEchelleDecroissante, NULL);
   XtManageChild(pcpDecroissante);
   

/**
****
**/

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeBoutons); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpFrameEchelle); i++;
   pcpFormeLimite = (Widget) XmCreateRowColumn(pcpForme2, "form", args, i);
   XtManageChild(pcpFormeLimite);

   pcpLabelLimite = (Widget) XmCreateLabel(pcpFormeLimite, labelLimite[lng], NULL, 0);
   XtManageChild(pcpLabelLimite);

   pcpFrameTypeLimite = (Widget) XmCreateFrame(pcpFormeLimite, "Forme", NULL, 0);
   XtManageChild(pcpFrameTypeLimite);

   pcpFormeTypeLimite = (Widget) XmCreateRadioBox(pcpFrameTypeLimite, "radiobox", NULL, 0);
   XtManageChild(pcpFormeTypeLimite);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteCoupe = (Widget) XmCreateToggleButton(pcpFormeTypeLimite, labelAutoRegion[lng], args, i);
   XtAddCallback(pcpLimiteCoupe, XmNvalueChangedCallback, PcpSetMinMaxProfil, NULL);
   XtManageChild(pcpLimiteCoupe);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteDimensionCoupe = (Widget) XmCreateToggleButton(pcpFormeTypeLimite, labelAutoGrilles[lng], args, i);
   XtAddCallback(pcpLimiteDimensionCoupe, XmNvalueChangedCallback, PcpSetMinMaxGrilles, NULL);
   XtManageChild(pcpLimiteDimensionCoupe);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteUsager = (Widget) XmCreateToggleButton(pcpFormeTypeLimite, labelFixes[lng], args, i);
   XtAddCallback(pcpLimiteUsager, XmNvalueChangedCallback, PcpSetMinMaxUsager, NULL);
   XtManageChild(pcpLimiteUsager);


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeLimite); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpFrameEchelle); i++;
   pcpForme3 = (Widget) XmCreateForm(pcpForme2, "form", args, i);
   XtManageChild(pcpForme3);


   pcpFrameLimiteX = (Widget) XmCreateFrame(pcpForme3, "Forme", NULL, 0);
   XtManageChild(pcpFrameLimiteX);

   pcpFormeLimiteX = (Widget) XmCreateForm(pcpFrameLimiteX, "radiobox", NULL, 0);
   XtManageChild(pcpFormeLimiteX);

/**
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcpLabelX = (Widget) XmCreateLabel(pcpFormeLimiteX, labelAxeX[lng], args, i);
   XtManageChild(pcpLabelX);
**/

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM);i++;
   pcpLabelMinX = (Widget) XmCreateLabel(pcpFormeLimiteX, labelAxeXMin[lng], args, i);
   XtManageChild(pcpLabelMinX);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpLabelMinX);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcpTextMinX = (Widget) XmCreateTextField(pcpFormeLimiteX, "Min:", args, i);
   XtManageChild(pcpTextMinX);


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNtopWidget, pcpLabelX); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpTextMinX);i++;
   pcpLabelMaxX = (Widget) XmCreateLabel(pcpFormeLimiteX, labelAxeXMax[lng], args, i);
   XtManageChild(pcpLabelMaxX);

      
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpLabelMaxX);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpTextMinX);i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcpTextMaxX = (Widget) XmCreateTextField(pcpFormeLimiteX, "Max X:", args, i);
   XtManageChild(pcpTextMaxX);


   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFrameLimiteX); i++;   
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcpFrameLimiteY = (Widget) XmCreateFrame(pcpForme3, "Forme", args, i);
   XtManageChild(pcpFrameLimiteY);

   pcpFormeLimiteY = (Widget) XmCreateForm(pcpFrameLimiteY, "radiobox", NULL, 0);
   XtManageChild(pcpFormeLimiteY);

/**
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcpLabelY = (Widget) XmCreateLabel(pcpFormeLimiteY, labelAxeY[lng], args, i);
   XtManageChild(pcpLabelY);
**/

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcpLabelMinY = (Widget) XmCreateLabel(pcpFormeLimiteY, labelAxeYMin[lng], args, i);
   XtManageChild(pcpLabelMinY);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpLabelMinY);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00");i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcpTextMinY = (Widget) XmCreateTextField(pcpFormeLimiteY, "Min:", args, i);
   XtManageChild(pcpTextMinY);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpTextMinY); i++;
   pcpLabelMaxY = (Widget) XmCreateLabel(pcpFormeLimiteY, labelAxeYMax[lng], args, i);
   XtManageChild(pcpLabelMaxY);
      
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpLabelMaxY); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpTextMinY);i++;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, "0.00"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   pcpTextMaxY = (Widget) XmCreateTextField(pcpFormeLimiteY, "Max:", args, i);
   XtManageChild(pcpTextMaxY);
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


ActiverPanneauCoupe()
{
   Arg args[2];
   int i;


   if (!pcpTopLevel)
      {
      InitPanneauCoupe();
      }
   
   if (!XtIsRealized(pcpTopLevel))
      {
      XtRealizeWidget(pcpTopLevel);
      i = 0;
      XtSetValues(pcpTopLevel, args, i);
      }
   
   f77name(xpanpcpact)();
   
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


f77name(xpanpcpact)()
{
   LocalEventLoop(pcpTopLevel);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/



DesactiverPanneauCoupe()
{
   XtUnrealizeWidget(pcpTopLevel);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/



XtCallbackProc PcpOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int fenetreAffichage;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   EnleverLigneCoupe(cx1, cy1, cx2, cy2);

   pcpSelectionTerminee = TRUE;
   EffacerCoupe();
   if (fenetreCoupe)
      {
      c_wglsetw(fenetreCoupe);
      c_wglclw(fenetreCoupe);
      }

   statutCoupe = False;
   fenetreCoupe = 0;
   DesactiverPanneauCoupe();
/**   ReAfficherBoutonsAnimation(); **/
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcpAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   RedessinerFenetreCoupe();

}

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcpNouvelleCoupe(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int ier, ok;
   int fenetreAffichage;
   int wwidth = 0;
   int wheight = 0;
   int event;
   int coupeValideTrouvee;
   _Champ *champ;
   int lng;

   lng = c_getulng();

   InvertWidget(w);
   if (!fenetreCoupe)
      {
#ifdef GL_WGL
      winconstraints();
#endif
      if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
      SetGeometrieFenetreAffichage(labelTopLevel[lng]);
      fenetreCoupe = c_wglopw(labelTopLevel[lng]);
#ifdef GL_WGL
      winconstraints();
      c_wgldbf();
      c_wglfbf();
#endif
      c_wglias(1);
      c_wglgwz(&wwidth, &wheight);
      lastcx1 = 0.0; lastcy1 = 0.0; lastcx2 = 0.0; lastcy2 = 0.0;
      }

   c_wglsetw(fenetreCoupe);
   ier = PreparerCoupeOuSerie(1.0, 1.0, 1.0, 1.0);

   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();
   

   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);
   MessageAide(messNouveauProfil[lng]);

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

   if (ier > 0)
      {
      InvertWidget(w);
      return;
      }

   coupeValideTrouvee = VerifierExistenceCoupeValide();
   if (coupeValideTrouvee == 0)
      {
      InvertWidget(w);
      c_wglsetw(fenetreCoupe);
      c_wglcol(NOIR);
      c_wglclr();
      c_wglsetw(fenetreAffichage);
      return;
      }

   statutCoupe = TRUE;
   lastEvent = NIL;
   EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
   Ligne(&event, &cx1, &cy1, &cx2, &cy2);

   while (event != RESET && event != NIL)
      {
      c_wglsetw(fenetreAffichage);
      c_wglcol(CYAN);
      EnterOverlayMode();
#ifdef GL_WGL
      color(0);
#endif
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
	 
	 c_wglsetw(fenetreCoupe);
	 ier = PreparerCoupeOuSerie(cx1, cy1, cx2, cy2);
	 RedessinerFenetreCoupe();
	 c_wglsetw(fenetreAffichage);
	 break;
	 
	 case MOVE:
	 c_wglsetw(fenetreAffichage);
	 EnterOverlayMode();
	 TracerLigne(cx1, cy1, cx2, cy2);
	 lastcx1 = cx1; lastcy1 = cy1; lastcx2 = cx2; lastcy2 = cy2;
	 RestoreNormalMode();
	 
	 c_wglsetw(fenetreCoupe);
	 if (c_wglgdbg())
	    {
	    printf("cx1:%f cy1:%f, cx2:%f cy2:%f\n", cx1,cy1,cx2,cy2);
	    }
	 ier = PreparerCoupeOuSerie(cx1, cy1, cx2, cy2);
	 RedessinerFenetreCoupe();

	 c_wglsetw(fenetreAffichage);
	 break;
	 }
      
      lastEvent = event;
      lastcx1 = cx1; lastcx2 = cx2; lastcy1 = cy1; lastcy2 = cy2;
      Ligne(&event, &cx1, &cy1, &cx2, &cy2);
      } 
   
   FldMgrGetChamp(&champ, 0);
   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      ok = champ->coupe.coupeValide;
      }
   else
      {
      ok = champ->seqanim.nbFldsAnim;
      }
   
   if (ok)
      {
#ifdef GL_WGL
      AfficherLigneCoupe();
#endif
      }
   else
      {
      c_wglsetw(fenetreCoupe);
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


XtCallbackProc PcpScanCoupe(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int ier;
   int fenetreAffichage;
   int wwidth = 0;
   int wheight = 0;
   int lng;
   int event;

   SetIgnoreMode();
   lng = c_getulng();

   InvertWidget(w);
   if (!fenetreCoupe)
      {
      if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
      fenetreCoupe = c_wglopw(labelTopLevel[lng]);
      c_wglias(1);
      c_wglgwz(&wwidth, &wheight);
      }

   c_wglsetw(fenetreCoupe);
   ier = PreparerCoupeOuSerie(1.0, 1.0, 1.0, 1.0);
   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);
   MessageAide(messScanCoupe[lng]);

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

   statutCoupe = TRUE;

   event = c_wglbtx(&cx1, &cy1);
   c_fxfy2xy(&cx1, &cy1, cx1, cy1);
   while (c_wglbtpx(BGAUCH, &cx2, &cy2))
      {
      c_wglsetw(fenetreAffichage);
      c_fxfy2xy(&cx2, &cy2, cx2, cy2);

      if (cx1 != cx2 || cy1 != cy2)
	 {
	 EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
	 EnterOverlayMode();
#ifdef GL_WGL
	 color(1);
#endif
#ifdef X_WGL
	 c_wglcol(CYAN);
#endif
	 TracerLigne(cx1, cy1, cx2, cy2);
	 RestoreNormalMode();
	 
	 c_wglsetw(fenetreCoupe);
	 ier = PreparerCoupeOuSerie(cx1, cy1, cx2, cy2);
	 RedessinerFenetreCoupe();
	 c_wglswb();
	 c_wglsetw(fenetreAffichage);
	 
	 lastcx1 = cx1;
	 lastcx2 = cx2;
	 lastcy1 = cy1;
	 lastcy2 = cy2;
	 }
      } 
   
   c_wglsetw(fenetreCoupe);
   c_wglfbf();
   c_wglsetw(fenetreAffichage);
   InvertWidget(w);
   UnsetIgnoreMode();
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/
XtCallbackProc PcpScanCoupeVert(w, unused1, unused2)
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
     if (!fenetreCoupe)
       {
       if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
       fenetreCoupe = c_wglopw(labelTopLevel[lng]);
       c_wglias(1);
       c_wglgwz(&wwidth, &wheight);
       }
     
     GetFenetreAffichageID(&fenetreAffichage);
     c_wglsetw(fenetreAffichage);
     EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
     /*      RedessinerFenetreAffichage(); */
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     c_wglsetw(fenetreCoupe);
     ier = PreparerCoupeOuSerie(1.0, 1.0, 1.0, 1.0);
     c_wglcol(NOIR);
     c_wglclr();
     c_wglcol(BLANC);
     
     c_wgldbf();
     c_wglbbf();
     
     RedessinerFenetreAffichage(); 
     EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
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
     
     yinit = ydeb;
     statutCoupe = TRUE;
     }

   if (status == 0)
     xc.annulationDemandee = True;

   while (!c_wglanul())
      {
      c_wglsetw(fenetreAffichage);
      c_xy2fxfy(&cx1, &cy1, xdeb, ydeb);
      c_xy2fxfy(&cx2, &cy2, xfin, ydeb);

      EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
#ifdef GL_WGL
      color(1);
#endif
#ifdef X_WGL
      c_wglcol(CYAN);
#endif
      TracerLigne(cx1, cy1, cx2, cy2);
      RestoreNormalMode();
	 
      c_wglsetw(fenetreCoupe);
      ier = PreparerCoupeOuSerie(cx1, cy1, cx2, cy2);
      RedessinerFenetreCoupe();
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
   
   UnsetIgnoreMode();
   c_wglsetw(fenetreCoupe);
   c_wglfbf();
   c_wglsetw(fenetreAffichage);
   InvertWidget(w);
   RedessinerFenetres();
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

XtCallbackProc PcpScanCoupeHoriz(w, unused1, unused2)
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
     if (!fenetreCoupe)
       {
       if (0 != (wwidth+wheight))
	 {
	 c_wglpsz(wwidth, wheight);
	 }
       fenetreCoupe = c_wglopw(labelTopLevel[lng]);
       c_wglias(1);
       c_wglgwz(&wwidth, &wheight);
       }
     
     GetFenetreAffichageID(&fenetreAffichage);
     c_wglsetw(fenetreAffichage);
     EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
     /*      RedessinerFenetreAffichage(); */
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     c_wglsetw(fenetreCoupe);
     ier = PreparerCoupeOuSerie(1.0, 1.0, 1.0, 1.0);
     c_wglcol(NOIR);
     c_wglclr();
     c_wglcol(BLANC);
     
     c_wgldbf();
     c_wglbbf();
     
     
     RedessinerFenetreAffichage(); 
     EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
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
     statutCoupe = TRUE;
     }

   if (status == 0)
     xc.annulationDemandee = True;

   SetIgnoreMode();
   while (!c_wglanul())
      {
      c_wglsetw(fenetreAffichage);
      c_xy2fxfy(&cx1, &cy1, xdeb, ydeb);
      c_xy2fxfy(&cx2, &cy2, xdeb, yfin);

      EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
#ifdef GL_WGL
      color(1);
#endif
#ifdef X_WGL
      c_wglcol(CYAN);
#endif
      TracerLigne(cx1, cy1, cx2, cy2);
      RestoreNormalMode();
	 
      c_wglsetw(fenetreCoupe);
      ier = PreparerCoupeOuSerie(cx1, cy1, cx2, cy2);
      RedessinerFenetreCoupe();
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
   
   UnsetIgnoreMode();
   c_wglsetw(fenetreCoupe);
   c_wglfbf();
   c_wglsetw(fenetreAffichage);
   InvertWidget(w);
   RedessinerFenetres();
   }



XtCallbackProc PcpScanProfil(w, unused1, unused2)
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
   
   SetIgnoreMode();
   lng = c_getulng();
   
   InvertWidget(w);
   InitFenetreCoupe();
   c_wglsetw(fenetreCoupe);
   ier = PreparerCoupeOuSerie(1.0, 1.0, 1.0, 1.0);

   c_wglcol(NOIR);
   c_wglclr();
   c_wglcol(BLANC);
   MessageAide(messScanProfil[lng]);

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

   statutCoupe = TRUE;
   
   event = c_wglbtx(&cx1, &cy1);
   c_fxfy2xy(&cx1, &cy1, cx1, cy1);
   while (c_wglbtpx(BGAUCH, &cx1, &cy1))
      {
      c_fxfy2xy(&cx1, &cy1, cx1, cy1);
      cx2 = cx1;
      cy2 = cy1;
      c_wglsetw(fenetreAffichage);

      EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
#ifdef GL_WGL
      color(1);
#endif
#ifdef X_WGL
      c_wglcol(CYAN);
#endif
      TracerCercle(cx1, cy1);
      RestoreNormalMode();

      c_wglsetw(fenetreCoupe);
      ier = PreparerCoupeOuSerie(cx1, cy1, cx2, cy2);
      RedessinerFenetreCoupe();
      c_wglswb();
      c_wglsetw(fenetreAffichage);

      lastcx1 = cx1;
      lastcx2 = cx2;
      lastcy1 = cy1;
      lastcy2 = cy2;
      } 
   
   c_wglsetw(fenetreCoupe);
   c_wglfbf();
   c_wglsetw(fenetreAffichage);
   EnleverLigneCoupe(cx1, cy1, cx2, cy2);
   InvertWidget(w);
   UnsetIgnoreMode();
   }


XtCallbackProc PcpSetDimensionCoupeZP(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   dimensionCoupe = ZP;
   }

XtCallbackProc PcpSetDimensionCoupeT(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   dimensionCoupe = T;
   }

XtCallbackProc PcpSetEchelleLineaire(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   echelle = LINEAIRE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
   }

XtCallbackProc PcpSetEchelleLog(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   echelle = LOG;
   if (XmToggleButtonGetState(w))
   RedessinerFenetreCoupe();
   }

XtCallbackProc PcpSetMinMaxProfil(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   calculMinMax = AUTO_PROFIL;
   PcpDesactiverTextWidgets();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
   }

XtCallbackProc PcpSetMinMaxGrilles(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   calculMinMax = AUTO_GRILLES;
   PcpDesactiverTextWidgets();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
   }

XtCallbackProc PcpSetMinMaxUsager(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   calculMinMax = FIXES;
   PcpActiverTextWidgets();
   }

PcpActiverTextWidgets()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, True); i++;
   XtSetArg(args[i], XmNsensitive, True); i++;
   
   XtSetValues(pcpTextMinX, args, i);
   XtSetValues(pcpTextMinY, args, i);
   XtSetValues(pcpTextMaxX, args, i);
   XtSetValues(pcpTextMaxY, args, i);
   }

PcpDesactiverTextWidgets()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   XtSetArg(args[i], XmNsensitive, False); i++;
   
   XtSetValues(pcpTextMinX, args, i);
   XtSetValues(pcpTextMinY, args, i);
   XtSetValues(pcpTextMaxX, args, i);
   XtSetValues(pcpTextMaxY, args, i);
   }

XtCallbackProc PcpSetEchelleCroissante(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   sensEchelle = CROISSANTE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
   }

XtCallbackProc PcpSetEchelleDecroissante(w, arg1, arg2)
Widget w;
caddr_t arg1, arg2;
{
   sensEchelle = DECROISSANTE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
   }

int CoupeMgrGetDimensionCoupe()
{
   return dimensionCoupe;
   }


static XtCallbackProc PcpSetIncrement(w, client_data, call_data)
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

