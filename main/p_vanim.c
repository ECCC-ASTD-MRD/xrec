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

#include <xinit.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <rec.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget pavAnimationPanel = NULL;
Widget pavFormeAnimPanel;
Widget    pavFormeFermer;
Widget       pavFrameFermer;
Widget          pavFermer;
Widget    pavFormeToggles;
Widget       pavFormeBoucle;
Widget          pavLabelBoucle;
Widget          pavFrameBoucle;
Widget             pavRadioBoxBoucle;
Widget                pavToggleTemps;
Widget                pavToggleNiveaux;
Widget       pavFormeDefilement;
Widget          pavLabelDefilement;
Widget          pavFrameAnimationRapide;
Widget             pavRadioBoxAnimation;
Widget                pavToggleAnimationRapide;
Widget          pavFrameTypeDeDefilement;
Widget             pavRadioBoxTypeDeDefilement;
Widget                pavToggleDefilementRegulier;
Widget                pavToggleDefilementAvantArriere;
Widget          pavFrameInterpolation;
Widget             pavRadioBoxInterpolation;
Widget                pavToggleInterpolation;
Widget    pavFrameScales;
Widget       pavFormeScales;
Widget          pavScaleHeureDebut;
Widget          pavScaleHeureFin;
Widget          pavScaleDelai;
Widget    pavFrameIntervalle;
Widget       pavFormeIntervalle;
Widget          pavScaleIntervalleDebut;
Widget          pavScaleIntervalleFin;
Widget          pavScaleIntervalle;
Widget    pavFrameBoutons;
Widget       pavFormeBoutons;
Widget          pavLastFrames;
Widget          pavLastFrame;
Widget          pavStop;
Widget          pavNextFrame;
Widget          pavNextFrames;

static char *nomPanneauVAnim[] = {"PanneauAnimationVerticale", "VerticalAnimationPanel"};
static char *labelTopLevel[] = {"Vertical_Animation", "Vertical_Animation"};
static char *labelOk[] = {"Fermer", "Close"};

static char *labelScaleDelai[] = {"Delai", "Delay"};
static char *labelScaleIntervalle[] = {"Increment vertical", "Vertical Increment"};

static char *labelDefilement[] = {"Defilement", "Sequence"};
static char *labelInterpolation[] = {"Interp. verticale", "Vertical Interpolation"};
static char *labelAnimationRapide[] = {"Animation rapide", "Fast animation"};
static char *labelDefilementRegulier[] = {"Regulier", "Standard"};
static char *labelDefilementAvantArriere[] = {"Avant-arriere", "Back and forth"};


extern _ColormapInfo recCmap;
extern int recColorTable[256];
_AnimInfo animInfo;
int pavSelectionTerminee = FALSE;


static XtCallbackProc PavSetDelai();
static XtCallbackProc PavSetIntervalle();
static XtCallbackProc PavToggleAnimationRapide();
static XtCallbackProc PavToggleInterpolation();
static XtCallbackProc PavToggleTemps();
static XtCallbackProc PavToggleNiveaux();
static XtCallbackProc PavToggleDefilementRegulier();
static XtCallbackProc PavToggleDefilementAvantArriere();


static XtCallbackProc PavNextFrame();
static XtCallbackProc PavNextFrames();
static XtCallbackProc PavLastFrame();
static XtCallbackProc PavLastFrames();
XtCallbackProc PavStop();
int interpolationVerticale = FALSE;

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavFermer(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   pavSelectionTerminee = TRUE;
   DesactiverPanneauVanim();
   }

/* -------------------------------------------------------------------------------------------------- */

InitPanneauAnimVerticale()
{
  int i;
   Arg args[16];
   char nomShell[128];
   XmString label;

   int lng;

   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauVAnim[lng]);
   i = 0;
   pavAnimationPanel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);

   i = 0;
   pavFormeAnimPanel = (Widget) XmCreateForm(pavAnimationPanel, "form", args, i);
   XtManageChild(pavFormeAnimPanel);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pavFermer = (Widget)XmCreatePushButton(pavFormeAnimPanel, labelOk[lng], args, i);
   XtManageChild(pavFermer);
   XtAddCallback(pavFermer, XmNactivateCallback, PavFermer, NULL);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pavFermer); i++;
   pavFormeToggles = (Widget)XmCreateForm(pavFormeAnimPanel, "formeToggles", args, i);
   XtManageChild(pavFormeToggles);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pavFormeDefilement = (Widget)XmCreateRowColumn(pavFormeToggles, "Defilement", args, i);
   XtManageChild(pavFormeDefilement);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pavLabelDefilement = (Widget)XmCreateLabel(pavFormeDefilement, labelDefilement[lng], args, i);
   XtManageChild(pavLabelDefilement);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pavLabelDefilement); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pavFrameAnimationRapide = (Widget)XmCreateFrame(pavFormeDefilement, "frame", args, i);
   XtManageChild(pavFrameAnimationRapide);

   i = 0;
   pavRadioBoxAnimation = (Widget)XmCreateRadioBox(pavFrameAnimationRapide, "radiobox", args, i);
   XtManageChild(pavRadioBoxAnimation);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, True); i++;
   XtSetArg(args[i], XmNset, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   XtSetArg(args[i], XmNindicatorType, XmONE_OF_MANY); i++;
   pavToggleAnimationRapide = (Widget) XmCreateToggleButton(pavRadioBoxAnimation, labelAnimationRapide[lng], args, i);
   XtManageChild(pavToggleAnimationRapide);
   XtAddCallback(pavToggleAnimationRapide, XmNdisarmCallback, PavToggleAnimationRapide, NULL);

/* -------------------------------------------------------------------------------------------------- */
 
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pavFrameAnimationRapide); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pavFrameTypeDeDefilement = (Widget)XmCreateFrame(pavFormeDefilement, "frame", args, i);
   XtManageChild(pavFrameTypeDeDefilement);

   i = 0;
   pavRadioBoxTypeDeDefilement = (Widget)XmCreateRadioBox(pavFrameTypeDeDefilement, "radiobox", args, i);
   XtManageChild(pavRadioBoxTypeDeDefilement);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pavToggleDefilementRegulier = (Widget) XmCreateToggleButton(pavRadioBoxTypeDeDefilement, labelDefilementRegulier[lng], args, i);
   XtManageChild(pavToggleDefilementRegulier);
   XtAddCallback(pavToggleDefilementRegulier, XmNvalueChangedCallback, PavToggleDefilementRegulier, NULL);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pavToggleDefilementAvantArriere = (Widget)XmCreateToggleButton(pavRadioBoxTypeDeDefilement, labelDefilementAvantArriere[lng], args, i);
   XtManageChild(pavToggleDefilementAvantArriere);
   XtAddCallback(pavToggleDefilementAvantArriere, XmNvalueChangedCallback, PavToggleDefilementAvantArriere, NULL);

/* -------------------------------------------------------------------------------------------------- */
   
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pavFormeToggles); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pavFrameScales = (Widget)XmCreateFrame(pavFormeAnimPanel, "frame", args, i); i++;
   XtManageChild(pavFrameScales);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   pavFormeScales = (Widget)XmCreateRowColumn(pavFrameScales, "form", args, i);
   XtManageChild(pavFormeScales);

   i = 0;
   pavRadioBoxInterpolation = (Widget)XmCreateRadioBox(pavFormeScales, "radiobox", args, i);
   XtManageChild(pavRadioBoxInterpolation);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, True); i++;
   XtSetArg(args[i], XmNset, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   XtSetArg(args[i], XmNindicatorType, XmONE_OF_MANY); i++;
   pavToggleInterpolation = (Widget) XmCreateToggleButton(pavRadioBoxInterpolation, labelInterpolation[lng], args, i);
   XtManageChild(pavToggleInterpolation);
   XtAddCallback(pavToggleInterpolation, XmNdisarmCallback, PavToggleInterpolation, NULL);

   i = 0;
   label = XmStringCreateLtoR(labelScaleIntervalle[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNminimum, 0); i++;
   XtSetArg(args[i], XmNmaximum, 100); i++;
   XtSetArg(args[i], XmNvalue, 10); i++;
   XtSetArg(args[i], XmNscaleMultiple, 10); i++;
   pavScaleIntervalle = (Widget)XmCreateScale(pavFormeScales, labelScaleIntervalle[lng], args, i);
   XtManageChild(pavScaleIntervalle);
   XmStringFree(label);

   XtAddCallback(pavScaleIntervalle, XmNdragCallback, PavSetIntervalle, NULL);
   XtAddCallback(pavScaleIntervalle, XmNvalueChangedCallback, PavSetIntervalle, NULL);

   animInfo.intervalle = 10.0;
   i = 0;
   label = XmStringCreateLtoR(labelScaleDelai[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 2); i++;
   XtSetArg(args[i], XmNminimum, 0); i++;
   XtSetArg(args[i], XmNmaximum, 50); i++;
   pavScaleDelai = (Widget)XmCreateScale(pavFormeScales, labelScaleDelai[lng], args, i);
   XtManageChild(pavScaleDelai);
   XmStringFree(label);

   XtAddCallback(pavScaleDelai, XmNdragCallback, PavSetDelai, NULL);
   XtAddCallback(pavScaleDelai, XmNvalueChangedCallback, PavSetDelai, NULL);



/* -------------------------------------------------------------------------------------------------- */

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pavFrameScales); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pavFrameBoutons = (Widget)XmCreateFrame(pavFormeAnimPanel, "frame", args, i); i++;
   XtManageChild(pavFrameBoutons);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   pavFormeBoutons = (Widget)XmCreateRowColumn(pavFrameBoutons, "form", args, i);
   XtManageChild(pavFormeBoutons);
   
   i = 0;
   pavLastFrames = (Widget) XmCreatePushButton(pavFormeBoutons, " << ", args, i);
   XtManageChild(pavLastFrames);
   XtAddCallback(pavLastFrames, XmNactivateCallback, PavLastFrames, NULL);

   i = 0;
   pavLastFrame = (Widget) XmCreatePushButton(pavFormeBoutons, " <  ", args, i);
   XtManageChild(pavLastFrame);
   XtAddCallback(pavLastFrame, XmNactivateCallback, PavLastFrame, NULL);

   i = 0;
   pavStop = (Widget) XmCreatePushButton(pavFormeBoutons, "Stop", args, i);
   XtManageChild(pavStop);
   XtAddCallback(pavStop, XmNactivateCallback, PavStop, NULL);

   i = 0;
   pavNextFrame = (Widget) XmCreatePushButton(pavFormeBoutons, "  > ", args, i);
   XtManageChild(pavNextFrame);
   XtAddCallback(pavNextFrame, XmNactivateCallback, PavNextFrame, NULL);

   i = 0;
   pavNextFrames = (Widget) XmCreatePushButton(pavFormeBoutons, " >> ", args, i);
   XtManageChild(pavNextFrames);
   XtAddCallback(pavNextFrames, XmNactivateCallback, PavNextFrames, NULL);

   animInfo.animationRapide = FALSE;
   }

/* -------------------------------------------------------------------------------------------------- */

ActiverPanneauAnimationVerticale()
{
   if (!pavAnimationPanel)
      InitPanneauAnimVerticale();
   
   if (!XtIsRealized(pavAnimationPanel))
      {
      XtRealizeWidget(pavAnimationPanel);
      CheckColormap(pavAnimationPanel);
      }
   
   f77name(xpanpavact)();
   
   }

/* -------------------------------------------------------------------------------------------------- */

f77name(xpanpavact)()
{
   LocalEventLoop(pavAnimationPanel);
   }

/* -------------------------------------------------------------------------------------------------- */

DesactiverPanneauVanim()
{
   int i;

   XtUnrealizeWidget(pavAnimationPanel);
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavSetIntervalle(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   animInfo.intervalle = (float)(donnees->value);
   
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavSetDelai(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   animInfo.delai = (float)(donnees->value);
   
   }


/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
static XtCallbackProc PavNextFrames(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }

   xc.flagInterrupt = FALSE;
   c_gmpopti("ACCEPT_INTERRUPTS", FALSE);

   InvertWidget(w);
   DesactiverWidgetsControle();
   AfficherBoutonAnnulation();
   
   VAnimerFrames(2);

   EnleverBoutonAnnulation();
   InvertWidget(w);
   ActiverWidgetsControle();

   xc.flagInterrupt = TRUE;
   c_gmpopti("ACCEPT_INTERRUPTS", TRUE);
   }


/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
XtCallbackProc PavStop(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   xc.annulationDemandee = TRUE;
   }


/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
static XtCallbackProc PavLastFrames(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }

   xc.flagInterrupt = FALSE;
   c_gmpopti("ACCEPT_INTERRUPTS", FALSE);

   InvertWidget(w);
   DesactiverWidgetsControle();
   AfficherBoutonAnnulation();
   
   VAnimerFrames(-2);

   EnleverBoutonAnnulation();
   InvertWidget(w);
   ActiverWidgetsControle();

   xc.flagInterrupt = TRUE;
   c_gmpopti("ACCEPT_INTERRUPTS", TRUE);
   }
/* -------------------------------------------------------------------------------------------------- */


/**
 ******************************************************************************
 ******************************************************************************
 **/

/** ARGSUSED **/
static XtCallbackProc PavLastFrame(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }

   InvertWidget(w);
   AfficherBoutonAnnulation();
   VAnimerFrames(-1);
   EnleverBoutonAnnulation();
   InvertWidget(w);
   }

/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
static XtCallbackProc PavNextFrame(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }

   InvertWidget(w);
   AfficherBoutonAnnulation();

   VAnimerFrames(1);

   EnleverBoutonAnnulation();
   InvertWidget(w);
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavToggleTemps(w, u1, u2)
Widget w;
caddr_t u1, u2;
{
   int i;

   FldMgrFreeVerticalXSection();
   LibererImages();
   animInfo.variableBoucle = TEMPS;
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavToggleNiveaux(w, u1, u2)
Widget w;
caddr_t u1, u2;
{
   int i;

   LibererImages();
   FldMgrFreeTimeAnimationSeq();
   animInfo.variableBoucle = NIVEAUX;
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavToggleDefilementRegulier(w, u1, u2)
Widget w;
caddr_t u1, u2;
{
   animInfo.typeDefilement = DEFILEMENT_REGULIER;
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavToggleDefilementAvantArriere(w, u1, u2)
Widget w;
caddr_t u1, u2;
{
   animInfo.typeDefilement = DEFILEMENT_AVANT_ARRIERE;
   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavToggleAnimationRapide(w, u1, u2)
Widget w;
caddr_t u1, u2;
{
  Arg args[2];
  int i;
  
  animInfo.animationRapide = !animInfo.animationRapide;
  
  i = 0;
  if (animInfo.animationRapide)
    {
    XtSetArg(args[i], XmNset, True); i++;
    }
  else
    {
    XtSetArg(args[i], XmNset, False); i++;
    }
  XtSetValues(w, args, i);

  if (interpolationVerticale)
    {
    i = 0;
    XtSetArg(args[i], XmNset, False); i++;
    XtSetValues(pavToggleInterpolation, args, i);
    interpolationVerticale = False;
    }



   }

/* -------------------------------------------------------------------------------------------------- */

static XtCallbackProc PavToggleInterpolation(w, u1, u2)
Widget w;
caddr_t u1, u2;
{
  Arg args[8];
  int i;
  
  interpolationVerticale = !interpolationVerticale;
  
  i = 0;
  if (interpolationVerticale)
    {
    XtSetArg(args[i], XmNset, True); i++;
    }
  else
    {
    XtSetArg(args[i], XmNset, False); i++;
    }
  XtSetValues(w, args, i);
  
  if (animInfo.animationRapide)
    {
    i = 0;
    XtSetArg(args[i], XmNset, False); i++;
    XtSetValues(pavToggleAnimationRapide, args, i);
    animInfo.animationRapide = False;
    }
  
}

