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
#include <souris.h>
#include <math.h>

#define AUTO_PROFIL 0
#define AUTO_GRILLES 1
#define AUTO_LEVELS 1
#define FIXES       2

extern int dimensionCoupe;
extern int echelle;
extern int calculMinMax_X, calculMinMax_Y;
extern int sensEchelle;
extern float grafMinX, grafMinY, grafMaxX, grafMaxY;
extern float grafMinUUtang,grafMaxUUtang,grafMinUUnorm,grafMaxUUnorm,grafMinUVW,grafMaxUVW,grafMinWW,grafMaxWW;

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern _Viewport    viewp;


Widget pcpTopLevel = NULL;
Widget   pcpForme;
Widget     pcpOk, pcpAfficher;
Widget     pcpFrame;
Widget       pcpForme2;
Widget         pcpFormeBoutons;
Widget           pcpFrameBoutons;
Widget             pcpRC;
Widget               pcpNouvelleCoupe, pcpScanProfil, pcpScanCoupe;
Widget           pcpFrameBoutons2;
Widget             pcpRC2;
Widget               pcpScanHoriz, pcpScanVert, pcpStop;
Widget           pcpScanIncrement;
Widget           pcpFrameEchelle;
Widget             pcpFormeEchelle;
Widget               pcpLabelEchelle;
Widget               pcpFrameLineaire;
Widget                 pcpLineariteEchelle;
Widget                   pcpLineaire, pcpLog;
Widget               pcpFrameSens;
Widget                 pcpSensEchelle;
Widget                   pcpCroissante, pcpDecroissante;
Widget               pcpFrameVertCoord;
Widget                 pcpVertCoord;
Widget                   pcpNativeCoord, pcpPresCoord;
Widget         pcpFormeLimiteX;
Widget           pcpLabelLimiteX;
Widget           pcpFrameTypeLimiteX;
Widget             pcpFormeTypeLimiteX;
Widget               pcpLimiteCoupeX, pcpLimiteDimensionCoupeX, pcpLimiteUsagerX;
Widget         pcpForme3X;
Widget           pcpFrameLimiteX;
Widget             pcpFormeLimiteX;
Widget               pcpLabelMinX, pcpTextMinX, pcpLabelMaxX, pcpTextMaxX;
Widget         pcpFormeLimiteY;
Widget           pcpLabelLimiteY;
Widget           pcpFrameTypeLimiteY;
Widget             pcpFormeTypeLimiteY;
Widget               pcpLimiteDimensionCoupeY, pcpLimiteUsagerY;
Widget         pcpForme3Y;
Widget           pcpFrameLimiteY;
Widget             pcpFormeLimiteY;
Widget               pcpLabelMinY, pcpTextMinY, pcpLabelMaxY, pcpTextMaxY;


int pcpSelectionTerminee = False;
extern int fenetreCoupe;
extern float cx1, cy1, cx2, cy2;

static float scanIncrement = 0.01;
static int selectVertCoord = NATIVE_VCOORD;
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

  char *lblPasCapableEnMetres[] = {"Cette option n'est pas disponible\n\n\
lorsque la coordonnee verticale est en METRES", "\
This option is not available\n\n\
when the vertical coordinate is in METERS"};
  
static char *labelTopLevel[] = {"Profils_Coupes", "Profiles_XSections"};

char *labelDimensionCoupe[]  = {"Dimension: ", "Dimension: "};
char *labelDimensionCoupeZ[] = {"Coord. vert.","Vert. Coord."};
char *labelDimensionCoupeT[] = {"Temps","Time"};

static char *labelStop[] = {"Stop", "Stop"};
char *labelProfilVertical[] = {"Profil Vertical", "Vertical Profile"};
char *labelEchelleVerticale[] = {"Echelle verticale", "Vertical Scale"};

char *labelEchelleLineaire[] = {"Lineaire", "Linear"};
char *labelEchelleLog[] = {"Log", "Log"};

char *labelEchelleCroissante[] = {"Croissante", "Normal"};
char *labelEchelleDecroissante[] = {"Decroissante", "Reversed"};

char *labelNativeVerticalCoord[] = {"Native", "Native"};
char *labelPressVerticalCoord[]  = {"Pression", "Pressure"};
char *labelChoixVertCoord[]      = {"Coord. vert.", "Vert. Coord."};

char *labelLimiteX[] = {"Limites Champ", "Field Limits"};
char *labelLimiteY[] = {"Limites Niveaux Verticaux", "Vertical Level Limits"};
char *labelAutoRegion[] = {"Auto (local)", "Auto (local)"};
char *labelAutoGrilles[] = {"Auto (global)", "Auto (global)"};
char *labelAutoNiveaux[] = {"Auto (Tous les niveaux)", "Auto (All Levels)"};
char *labelFixes[] = {"Fixes", "Fixed"};

char *labelAxeXMin[] = {"FLD min:", "FLD min:"};
char *labelAxeXMax[] = {"FLD max:", "FLD max:"};
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
   XtAddCallback(pcpOk, XmNactivateCallback, (XtCallbackProc) PcpOk, NULL);
   XtManageChild(pcpOk);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pcpOk); i++;
   pcpAfficher = (Widget)XmCreatePushButton(pcpForme, labelAfficher[lng], args, i);
   XtAddCallback(pcpAfficher, XmNactivateCallback, (XtCallbackProc) PcpAfficher, NULL);
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
   XtAddCallback(pcpNouvelleCoupe, XmNactivateCallback, (XtCallbackProc) PcpNouvelleCoupe, NULL);
   XtManageChild(pcpNouvelleCoupe);

/**
****
**/

   i = 0;
   string = XmStringCreateLtoR(labelScanProfil[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpScanProfil = (Widget)XmCreatePushButton(pcpRC, labelScanProfil[lng], args, i);
   XtAddCallback(pcpScanProfil, XmNactivateCallback, (XtCallbackProc) PcpScanProfil, NULL);
   XtManageChild(pcpScanProfil);

   i = 0;
   string = XmStringCreateLtoR(labelScanCoupe[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpScanCoupe = (Widget)XmCreatePushButton(pcpRC, labelScanCoupe[lng], args,i);
   XtAddCallback(pcpScanCoupe, XmNactivateCallback, (XtCallbackProc) PcpScanCoupe, NULL);
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
   XtAddCallback(pcpScanHoriz, XmNactivateCallback, (XtCallbackProc) PcpScanCoupeHoriz, NULL);
   XtManageChild(pcpScanHoriz);

   i = 0;
   string = XmStringCreateLtoR(labelScanVert[lng], XmSTRING_DEFAULT_CHARSET);
   pcpScanVert = (Widget)XmCreatePushButton(pcpRC2, labelScanVert[lng], args, i);
   XtAddCallback(pcpScanVert, XmNactivateCallback, (XtCallbackProc) PcpScanCoupeVert, NULL);
   XtManageChild(pcpScanVert);

   i = 0;
   string = XmStringCreateLtoR(labelStop[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, string); i++;
   pcpStop = (Widget)XmCreatePushButton(pcpRC2, labelStop[lng], args,i);
   XtAddCallback(pcpStop, XmNactivateCallback, (XtCallbackProc) PcpStop, NULL);
   XtManageChild(pcpStop);

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
   
   XtAddCallback(pcpScanIncrement, XmNdragCallback, (XtCallbackProc) PcpSetIncrement, NULL);
   XtAddCallback(pcpScanIncrement, XmNvalueChangedCallback, (XtCallbackProc) PcpSetIncrement, NULL);

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
   XtAddCallback(pcpLineaire, XmNvalueChangedCallback, (XtCallbackProc) PcpSetEchelleLineaire, NULL);
   XtManageChild(pcpLineaire);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLog = (Widget)XmCreateToggleButton(pcpLineariteEchelle, labelEchelleLog[lng], args, i);
   XtAddCallback(pcpLog, XmNvalueChangedCallback, (XtCallbackProc) PcpSetEchelleLog, NULL);
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
   XtAddCallback(pcpCroissante, XmNvalueChangedCallback, (XtCallbackProc) PcpSetEchelleCroissante, NULL);
   XtManageChild(pcpCroissante);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpDecroissante = (Widget) XmCreateToggleButton(pcpSensEchelle, labelEchelleDecroissante[lng], args, i);
   XtAddCallback(pcpDecroissante, XmNvalueChangedCallback, (XtCallbackProc) PcpSetEchelleDecroissante, NULL);
   XtManageChild(pcpDecroissante);
   
/**
****
**/

   pcpFrameVertCoord = (Widget) XmCreateFrame(pcpFormeEchelle, "frame", NULL, 0);
   XtManageChild(pcpFrameVertCoord);

   pcpVertCoord = XmCreateRadioBox(pcpFrameVertCoord, "radiobox", NULL, 0);
   XtManageChild(pcpVertCoord);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpNativeCoord = (Widget) XmCreateToggleButton(pcpVertCoord, labelNativeVerticalCoord[lng], args, i);
   XtAddCallback(pcpNativeCoord, XmNvalueChangedCallback, (XtCallbackProc) PcpSetNativeVertCoord, NULL);
   XtManageChild(pcpNativeCoord);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpPresCoord = (Widget)XmCreateToggleButton(pcpVertCoord, labelPressVerticalCoord[lng], args, i);
   XtAddCallback(pcpPresCoord, XmNvalueChangedCallback, (XtCallbackProc) PcpSetPresVertCoord, NULL);
   XtManageChild(pcpPresCoord);
   

/**
****
**/

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeBoutons); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpFrameEchelle); i++;
   pcpFormeLimiteX = (Widget) XmCreateRowColumn(pcpForme2, "form", args, i);
   XtManageChild(pcpFormeLimiteX);

   pcpLabelLimiteX = (Widget) XmCreateLabel(pcpFormeLimiteX, labelLimiteX[lng], NULL, 0);
   XtManageChild(pcpLabelLimiteX);

   pcpFrameTypeLimiteX = (Widget) XmCreateFrame(pcpFormeLimiteX, "Forme", NULL, 0);
   XtManageChild(pcpFrameTypeLimiteX);

   pcpFormeTypeLimiteX = (Widget) XmCreateRadioBox(pcpFrameTypeLimiteX, "radiobox", NULL, 0);
   XtManageChild(pcpFormeTypeLimiteX);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteCoupeX = (Widget) XmCreateToggleButton(pcpFormeTypeLimiteX, labelAutoRegion[lng], args, i);
   XtAddCallback(pcpLimiteCoupeX, XmNvalueChangedCallback, (XtCallbackProc) PcpSetMinMaxProfil_X, NULL);
   XtManageChild(pcpLimiteCoupeX);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteDimensionCoupeX = (Widget) XmCreateToggleButton(pcpFormeTypeLimiteX, labelAutoGrilles[lng], args, i);
   XtAddCallback(pcpLimiteDimensionCoupeX, XmNvalueChangedCallback, (XtCallbackProc) PcpSetMinMaxGrilles_X, NULL);
   XtManageChild(pcpLimiteDimensionCoupeX);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteUsagerX = (Widget) XmCreateToggleButton(pcpFormeTypeLimiteX, labelFixes[lng], args, i);
   XtAddCallback(pcpLimiteUsagerX, XmNvalueChangedCallback, (XtCallbackProc) PcpSetMinMax_X_Usager, NULL);
   XtManageChild(pcpLimiteUsagerX);


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeLimiteX); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpFrameEchelle); i++;
   pcpForme3X = (Widget) XmCreateForm(pcpForme2, "form", args, i);
   XtManageChild(pcpForme3X);


   pcpFrameLimiteX = (Widget) XmCreateFrame(pcpForme3X, "Forme", NULL, 0);
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


   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeLimiteX); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpFrameEchelle); i++;
   pcpFormeLimiteY = (Widget) XmCreateRowColumn(pcpForme2, "form", args, i);
   XtManageChild(pcpFormeLimiteY);

   pcpLabelLimiteY = (Widget) XmCreateLabel(pcpFormeLimiteY, labelLimiteY[lng], NULL, 0);
   XtManageChild(pcpLabelLimiteY);

   pcpFrameTypeLimiteY = (Widget) XmCreateFrame(pcpFormeLimiteY, "Forme", NULL, 0);
   XtManageChild(pcpFrameTypeLimiteY);

   pcpFormeTypeLimiteY = (Widget) XmCreateRadioBox(pcpFrameTypeLimiteY, "radiobox", NULL, 0);
   XtManageChild(pcpFormeTypeLimiteY);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteDimensionCoupeY = (Widget) XmCreateToggleButton(pcpFormeTypeLimiteY, labelAutoNiveaux[lng], args, i);
   XtAddCallback(pcpLimiteDimensionCoupeY, XmNvalueChangedCallback, (XtCallbackProc) PcpSetMinMaxGrilles_Y, NULL);
   XtManageChild(pcpLimiteDimensionCoupeY);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pcpLimiteUsagerY = (Widget) XmCreateToggleButton(pcpFormeTypeLimiteY, labelFixes[lng], args, i);
   XtAddCallback(pcpLimiteUsagerY, XmNvalueChangedCallback, (XtCallbackProc) PcpSetMinMax_Y_Usager, NULL);
   XtManageChild(pcpLimiteUsagerY);


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFormeLimiteY); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pcpFrameEchelle); i++;
   pcpForme3Y = (Widget) XmCreateForm(pcpForme2, "form", args, i);
   XtManageChild(pcpForme3Y);


   pcpFrameLimiteY = (Widget) XmCreateFrame(pcpForme3Y, "Forme", NULL, 0);
   XtManageChild(pcpFrameLimiteY);

   pcpFormeLimiteY = (Widget) XmCreateForm(pcpFrameLimiteY, "radiobox", NULL, 0);
   XtManageChild(pcpFormeLimiteY);

   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcpFrameLimiteX); i++;   
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pcpFrameLimiteY = (Widget) XmCreateFrame(pcpForme3Y, "Forme", args, i);
   XtManageChild(pcpFrameLimiteY);

   pcpFormeLimiteY = (Widget) XmCreateForm(pcpFrameLimiteY, "radiobox", NULL, 0);
   XtManageChild(pcpFormeLimiteY);

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
   
   PcpDesactiverTextWidgets_X();
   PcpDesactiverTextWidgets_Y();
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


void ActiverPanneauCoupe()
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


void f77name(xpanpcpact)()
{
   LocalEventLoop(pcpTopLevel);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/



void DesactiverPanneauCoupe()
{
   XtUnrealizeWidget(pcpTopLevel);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

XtCallbackProc PcpStop(Widget w, caddr_t arg1, caddr_t arg2)
{
  xc.annulationDemandee = TRUE;
  return 0;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcpOk(Widget w, caddr_t unused1, caddr_t unused2)
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
   return 0;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcpAfficher(Widget w, caddr_t unused1, caddr_t unused2)
{
   RedessinerFenetreCoupe();
return 0;
}

/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcpNouvelleCoupe(Widget w, caddr_t unused1, caddr_t unused2)
{
   int ier, ok;
   int fenetreAffichage;
   int wwidth = 0;
   int wheight = 0;
   int event;
   int coupeValideTrouvee;
   _Champ *champ;
   int lng;

   dimensionCoupe = ZP;
   lng = c_getulng();

   InvertWidget(w);
   if (!fenetreCoupe)
      {
      if (0 != (wwidth+wheight))
   {
   c_wglpsz(wwidth, wheight);
   }
      SetGeometrieFenetreAffichage(labelTopLevel[lng]);
      fenetreCoupe = c_wglopw(labelTopLevel[lng]);
      c_wgldbf();
      c_wglfbf();
      c_wglias(1);
      c_wglgwz(&wwidth, &wheight);
      lastcx1 = 0.0; lastcy1 = 0.0; lastcx2 = 0.0; lastcy2 = 0.0;
      }

   c_wglsetw(fenetreCoupe);
   ier = PreparerCoupe(1.0, 1.0, 1.0, 1.0);

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
      return 0;
      }

   coupeValideTrouvee = VerifierExistenceCoupeValide();
   if (coupeValideTrouvee == 0)
      {
      InvertWidget(w);
      c_wglsetw(fenetreCoupe);
      c_wglcol(NOIR);
      c_wglclr();
      c_wglsetw(fenetreAffichage);
      return 0;
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
   ier = PreparerCoupe(cx1, cy1, cx2, cy2);
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
   ier = PreparerCoupe(cx1, cy1, cx2, cy2);
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
   return 0;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


XtCallbackProc PcpScanCoupe(Widget w, caddr_t unused1, caddr_t unused2)
{
   int ier;
   int fenetreAffichage;
   int wwidth = 0;
   int wheight = 0;
   int lng;
   int event;

   dimensionCoupe = ZP;
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
   ier = PreparerCoupe(1.0, 1.0, 1.0, 1.0);
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
      return 0;
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
        ier = PreparerCoupe(cx1, cy1, cx2, cy2);
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
   return 0;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/
XtCallbackProc PcpScanCoupeVert(Widget w, caddr_t unused1, caddr_t unused2)
{
   int ier;
   int fenetreAffichage;
   float yinit, xdeb, ydeb, xfin, yfin;
   int ideb, jdeb, ifin, jfin;
   int wwidth = 0;
   int wheight = 0;
   int lng;
   int largeurFenetre, hauteurFenetre;
   static int status = 0;

   dimensionCoupe = ZP;
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
     ier = PreparerCoupe(1.0, 1.0, 1.0, 1.0);
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
       return 0;
       }
     
     yinit = ydeb;
     statutCoupe = TRUE;
     }

   if (status == 0)
     xc.annulationDemandee = True;

   SetIgnoreMode();
   while (!c_wglanul() && status == 1)
      {
      c_wglsetw(fenetreAffichage);
      c_fxfy2xy(&cx1, &cy1, xdeb, ydeb);
      c_fxfy2xy(&cx2, &cy2, xfin, ydeb);
/*      c_fxfy2xy(&cx1, &cy1, cx1, cy1);
      c_fxfy2xy(&cx2, &cy2, cx2, cy2);*/
      
      EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
      c_wglcol(CYAN);
      TracerLigne(cx1, cy1, cx2, cy2);
      RestoreNormalMode();
   
      c_wglsetw(fenetreCoupe);
      ier = PreparerCoupe(cx1, cy1, cx2, cy2);
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
   
   xc.annulationDemandee = False;
   UnsetIgnoreMode();
   c_wglsetw(fenetreCoupe);
   c_wglfbf();
   xc.statuts[AXE_Y] = TRUE;
   RedessinerFenetreCoupe();
   c_wglsetw(fenetreAffichage);
   c_wglfbf();
   xc.statuts[AXE_Y] = FALSE;
   RedessinerFenetreAffichage();
   status=0;
   InvertWidget(w);
   return 0;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

XtCallbackProc PcpScanCoupeHoriz(Widget w, caddr_t unused1, caddr_t unused2)
{
   int ier;
   int fenetreAffichage;
   float xinit, xdeb, ydeb, xfin, yfin;
   int ideb, jdeb, ifin, jfin;
   int wwidth = 0;
   int wheight = 0;
   int lng;
   int largeurFenetre, hauteurFenetre;
   static int status = 0;

   dimensionCoupe = ZP;
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
     c_wglgvx(&xdeb, &ydeb, &xfin, &yfin); 
     c_wglgvi(&ideb, &jdeb, &ifin, &jfin); 
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     
     c_wglsetw(fenetreCoupe);
     ier = PreparerCoupe(1.0, 1.0, 1.0, 1.0);
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
       return 0;
       }
     
     xinit = xdeb;
     statutCoupe = TRUE;
     }

   if (status == 0)
     xc.annulationDemandee = True;

   SetIgnoreMode();
   while (!c_wglanul() && status == 1)
      {
      c_wglsetw(fenetreAffichage);
      c_fxfy2xy(&cx1, &cy1, xdeb, ydeb);
      c_fxfy2xy(&cx2, &cy2, xdeb, yfin);
      EnleverLigneCoupe(lastcx1, lastcy1, lastcx2, lastcy2);
      EnterOverlayMode();
      c_wglcol(CYAN);
      TracerLigne(cx1, cy1, cx2, cy2);
      RestoreNormalMode();
   
      c_wglsetw(fenetreCoupe);
      ier = PreparerCoupe(cx1, cy1, cx2, cy2);
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
   
   xc.annulationDemandee = False;
   UnsetIgnoreMode();
   c_wglsetw(fenetreCoupe);
   c_wglfbf();
   xc.statuts[AXE_Y] = TRUE;
   RedessinerFenetreCoupe();
   c_wglsetw(fenetreAffichage);
   c_wglfbf();
   xc.statuts[AXE_Y] = FALSE;
   RedessinerFenetreAffichage();
   
   status=0;
   InvertWidget(w);
   return 0;
   }



XtCallbackProc PcpScanProfil(Widget w, caddr_t unused1, caddr_t unused2)
{
   int ier;
   int fenetreAffichage;
   int event;
   int lng;
   
   dimensionCoupe = ZP;
   SetIgnoreMode();
   lng = c_getulng();
   
   InvertWidget(w);
   InitFenetreCoupe();
   c_wglsetw(fenetreCoupe);
   ier = PreparerCoupe(1.0, 1.0, 1.0, 1.0);

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
      return 0;
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
      ier = PreparerCoupe(cx1, cy1, cx2, cy2);
      RedessinerFenetreCoupe();
      c_wglswb();
      c_wglsetw(fenetreAffichage);

      lastcx1 = cx1;
      lastcx2 = cx2;
      lastcy1 = cy1;
      lastcy2 = cy2;
      } 
   
   c_wglsetw(fenetreCoupe);
   c_wglsbf();
   c_wglsetw(fenetreAffichage);
   c_wglsbf();
   EnleverLigneCoupe(cx1, cy1, cx2, cy2);
   InvertWidget(w);
   UnsetIgnoreMode();
   return 0;
   }


XtCallbackProc PcpSetDimensionCoupeZP(Widget w, caddr_t unused1, caddr_t unused2)
{
   dimensionCoupe = ZP;
  return 0;
   }

XtCallbackProc PcpSetDimensionCoupeT(Widget w, caddr_t unused1, caddr_t unused2)
{
   dimensionCoupe = T;
  return 0;
   }

XtCallbackProc PcpSetEchelleLineaire(Widget w, caddr_t unused1, caddr_t unused2)
{
   echelle = LINEAIRE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetEchelleLog(Widget w, caddr_t unused1, caddr_t unused2)
{
   echelle = LOG;
   if (XmToggleButtonGetState(w))
   RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetMinMaxProfil_X(Widget w, caddr_t unused1, caddr_t unused2)
{
   calculMinMax_X = AUTO_PROFIL;
   PcpDesactiverTextWidgets_X();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetMinMaxProfil_Y(Widget w, caddr_t unused1, caddr_t unused2)
{
   calculMinMax_Y = AUTO_PROFIL;
   PcpDesactiverTextWidgets_Y();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetMinMaxGrilles_X(Widget w, caddr_t unused1, caddr_t unused2)
{
   calculMinMax_X = AUTO_GRILLES;
   PcpDesactiverTextWidgets_X();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetMinMaxGrilles_Y(Widget w, caddr_t unused1, caddr_t unused2)
{
   calculMinMax_Y = AUTO_LEVELS;
   PcpDesactiverTextWidgets_Y();
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetMinMax_X_Usager(Widget w, caddr_t unused1, caddr_t unused2)
{
   calculMinMax_X = FIXES;
   PcpActiverTextWidgets_X();
  return 0;
   }

XtCallbackProc PcpSetMinMax_Y_Usager(Widget w, caddr_t unused1, caddr_t unused2)
{
   calculMinMax_Y = FIXES;
   PcpActiverTextWidgets_Y();
  return 0;
   }

void PcpActiverTextWidgets_X()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, True); i++;
   XtSetArg(args[i], XmNsensitive, True); i++;
   
   XtSetValues(pcpTextMinX, args, i);
   XtSetValues(pcpTextMaxX, args, i);
   }

void PcpActiverTextWidgets_Y()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, True); i++;
   XtSetArg(args[i], XmNsensitive, True); i++;
   
   XtSetValues(pcpTextMinY, args, i);
   XtSetValues(pcpTextMaxY, args, i);
   }

void PcpDesactiverTextWidgets_X()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   XtSetArg(args[i], XmNsensitive, False); i++;
   
   XtSetValues(pcpTextMinX, args, i);
   XtSetValues(pcpTextMaxX, args, i);
   }

void PcpDesactiverTextWidgets_Y()
{
   Arg args[2];
   int i;

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   XtSetArg(args[i], XmNsensitive, False); i++;
   
   XtSetValues(pcpTextMinY, args, i);
   XtSetValues(pcpTextMaxY, args, i);
   }
   
XtCallbackProc PcpSetEchelleCroissante(Widget w, caddr_t unused1, caddr_t unused2)
{
   sensEchelle = CROISSANTE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

XtCallbackProc PcpSetEchelleDecroissante(Widget w, caddr_t unused1, caddr_t unused2)
{
   sensEchelle = DECROISSANTE;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
   }

int CoupeMgrGetDimensionCoupe()
{
   return dimensionCoupe;
   }


XtCallbackProc PcpSetIncrement(Widget w, caddr_t client_data, caddr_t call_data)
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
  
  return 0;
}

XtCallbackProc PcpSetPresVertCoord(Widget w, caddr_t client_data, caddr_t call_data)
{
  int i, lng, nbChampsActifs;
  _Champ *champ;
  Arg args[2];
  int pasCapable;
  
  pasCapable = 0;
  lng = c_getulng();
  nbChampsActifs = FldMgrGetNbChampsActifs();
  for (i=0; i < nbChampsActifs; i++)
    {
    FldMgrGetChamp(&champ, i);
    switch (champ->coordonneeVerticale)
      {
      case METRES:
      case METRES_AGL:
      pasCapable = 1;
      break;
      
      default:
      break;
      }
    }
  
  if (pasCapable == 1)
    {
    MessageAvertissement(lblPasCapableEnMetres[lng], AVERTISSEMENT);
    i = 0;
    XtSetArg(args[i], XmNset, False); i++;
    XtSetValues(w, args, i);
    i = 0;
    XtSetArg(args[i], XmNset, True); i++;
    XtSetValues(pcpNativeCoord, args, i);

    }
  else
    {
    selectVertCoord = PRES_VCOORD;
    if (XmToggleButtonGetState(w))
        RedessinerFenetreCoupe();
    }
  return 0;
}

XtCallbackProc PcpSetNativeVertCoord(Widget w, caddr_t client_data, caddr_t call_data)
{
  selectVertCoord = NATIVE_VCOORD;
   if (XmToggleButtonGetState(w))
      RedessinerFenetreCoupe();
  return 0;
}

int GetSelectedVertCoord()
{
  return selectVertCoord;
}
