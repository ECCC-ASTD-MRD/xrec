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
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

#include <xinit.h>
#include <wgl.h>
#include <rmn/rpnmacros.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget paAnimationPanel = NULL;
Widget paFormeAnimPanel;
Widget    paFormeFermer;
Widget       paFrameFermer;
Widget          paFermer;
Widget    paFormeToggles;
Widget       paFormeBoucle;
Widget          paLabelBoucle;
Widget          paFrameBoucle;
Widget             paRadioBoxBoucle;
Widget                paToggleTemps;
Widget                paToggleNiveaux;
Widget       paFormeDefilement;
Widget          paLabelDefilement;
Widget          paFrameAnimationRapide;
Widget             paRadioBoxAnimation;
Widget                paToggleAnimationRapide;
Widget          paFrameTypeDeDefilement;
Widget             paRadioBoxTypeDeDefilement;
Widget                paToggleDefilementRegulier;
Widget                paToggleDefilementAvantArriere;
Widget          paFrameInterpolation;
Widget             paRadioBoxInterpolation;
Widget                paToggleInterpolation;
Widget    paFrameScales;
Widget       paFormeScales;
Widget          paScaleHeureDebut;
Widget          paScaleHeureFin;
Widget       paFrameDelai;
Widget          paScaleDelai;
Widget    paFrameIntervalle;
Widget       paFormeIntervalle;
Widget          paScaleIntervalleDebut;
Widget          paScaleIntervalleFin;
Widget          paScaleIntervalle;
Widget    paFrameBoutons;
Widget       paFormeBoutons;
Widget          paLastFrames;
Widget          paLastFrame;
Widget          paStop;
Widget          paNextFrame;
Widget          paNextFrames;

static char *nomPanneauAnim[] = {"PanneauAnimation", "AnimationPanel"};
static char *labelOk[] = {"Fermer", "Close"};

static char *labelScaleDelai[] = {"Delai entre les images (sec.)", "Delay between frames (sec.)"};
static char *labelScaleIntervalle[] = {"Intervalle temporel (minutes)", "Time Interval (minutes)"};

static char *labelDefilement[] = {"Defilement", "Sequence"};
static char *labelInterpolation[] = {"Interp. temporelle", "Time Interpolation"};
static char *labelAnimationRapide[] = {"Animation rapide", "Fast animation"};
static char *labelDefilementRegulier[] = {"Regulier", "Standard"};
static char *labelDefilementAvantArriere[] = {"Aller-retour", "Back and forth"};


extern _ColormapInfo recCmap;
extern int recColorTable[256];
_AnimInfo animInfo;
int paSelectionTerminee = FALSE;
int interpolationTemporelle = FALSE;


/* -------------------------------------------------------------------------------------------------- */

XtCallbackProc PaFermer(Widget w, caddr_t unused1, caddr_t unused2)
{
   paSelectionTerminee = TRUE;
   DesactiverPanneauAnim();
   return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

void InitPanneauAnim()
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
   strcat(nomShell, nomPanneauAnim[lng]);
   i = 0;
   paAnimationPanel = XtAppCreateShell(nomShell, nomShell, 
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);

   i = 0;
   paFormeAnimPanel = (Widget) XmCreateForm(paAnimationPanel, "form", args, i);
   XtManageChild(paFormeAnimPanel);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   paFermer = (Widget)XmCreatePushButton(paFormeAnimPanel, labelOk[lng], args, i);
   XtManageChild(paFermer);
   XtAddCallback(paFermer, XmNactivateCallback, (XtCallbackProc) PaFermer, NULL);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, paFermer); i++;
   paFormeToggles = (Widget)XmCreateForm(paFormeAnimPanel, "formeToggles", args, i);
   XtManageChild(paFormeToggles);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   paFormeDefilement = (Widget)XmCreateRowColumn(paFormeToggles, "Defilement", args, i);
   XtManageChild(paFormeDefilement);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   paLabelDefilement = (Widget)XmCreateLabel(paFormeDefilement, labelDefilement[lng], args, i);
   XtManageChild(paLabelDefilement);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, paLabelDefilement); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   paFrameAnimationRapide = (Widget)XmCreateFrame(paFormeDefilement, "frame", args, i);
   XtManageChild(paFrameAnimationRapide);

   i = 0;
   paRadioBoxAnimation = (Widget)XmCreateRadioBox(paFrameAnimationRapide, "radiobox", args, i);
   XtManageChild(paRadioBoxAnimation);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, True); i++;
   XtSetArg(args[i], XmNset, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   XtSetArg(args[i], XmNindicatorType, XmONE_OF_MANY); i++;
   paToggleAnimationRapide = (Widget) XmCreateToggleButton(paRadioBoxAnimation, labelAnimationRapide[lng], args, i);
   XtManageChild(paToggleAnimationRapide);
   XtAddCallback(paToggleAnimationRapide, XmNdisarmCallback, (XtCallbackProc) PaToggleAnimationRapide, NULL);

/* -------------------------------------------------------------------------------------------------- */

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, paFrameAnimationRapide); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   paFrameTypeDeDefilement = (Widget)XmCreateFrame(paFormeDefilement, "frame", args, i);
   XtManageChild(paFrameTypeDeDefilement);

   i = 0;
   paRadioBoxTypeDeDefilement = (Widget)XmCreateRadioBox(paFrameTypeDeDefilement, "radiobox", args, i);
   XtManageChild(paRadioBoxTypeDeDefilement);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, True); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   paToggleDefilementRegulier = (Widget) XmCreateToggleButton(paRadioBoxTypeDeDefilement, labelDefilementRegulier[lng], args, i);
   XtManageChild(paToggleDefilementRegulier);
   XtAddCallback(paToggleDefilementRegulier, XmNvalueChangedCallback, (XtCallbackProc) PaToggleDefilementRegulier, NULL);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   paToggleDefilementAvantArriere = (Widget)XmCreateToggleButton(paRadioBoxTypeDeDefilement, labelDefilementAvantArriere[lng], args, i);
   XtManageChild(paToggleDefilementAvantArriere);
   XtAddCallback(paToggleDefilementAvantArriere, XmNvalueChangedCallback, (XtCallbackProc) PaToggleDefilementAvantArriere, NULL);

/* -------------------------------------------------------------------------------------------------- */
   
   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, paFormeToggles); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   paFrameScales = (Widget)XmCreateFrame(paFormeAnimPanel, "frame", args, i); 
   XtManageChild(paFrameScales);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   paFormeScales = (Widget)XmCreateRowColumn(paFrameScales, "form", args, i);
   XtManageChild(paFormeScales);

   i = 0;
   paRadioBoxInterpolation = (Widget)XmCreateRadioBox(paFormeScales, "radiobox", args, i);
   XtManageChild(paRadioBoxInterpolation);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, True); i++;
   XtSetArg(args[i], XmNset, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   XtSetArg(args[i], XmNindicatorType, XmONE_OF_MANY); i++;
   paToggleInterpolation = (Widget) XmCreateToggleButton(paRadioBoxInterpolation, labelInterpolation[lng], args, i);
   XtManageChild(paToggleInterpolation);
   XtAddCallback(paToggleInterpolation, XmNdisarmCallback, (XtCallbackProc) PaToggleInterpolation, NULL);

   i = 0;
   label = XmStringCreateLtoR(labelScaleIntervalle[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNminimum, 5); i++;
   XtSetArg(args[i], XmNmaximum, 725); i++;
   XtSetArg(args[i], XmNvalue, 180); i++;
   XtSetArg(args[i], XmNscaleMultiple, 15); i++;
   paScaleIntervalle = (Widget)XmCreateScale(paFormeScales, labelScaleIntervalle[lng], args, i);
   XtManageChild(paScaleIntervalle);
   XmStringFree(label);

   XtAddCallback(paScaleIntervalle, XmNdragCallback, (XtCallbackProc) PaSetIntervalle, NULL);
   XtAddCallback(paScaleIntervalle, XmNvalueChangedCallback, (XtCallbackProc) PaSetIntervalle, NULL);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, paFormeScales); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   paFrameDelai = (Widget)XmCreateFrame(paFormeAnimPanel, "frame", args, i); 
   XtManageChild(paFrameDelai);

   animInfo.intervalle = 180.0;
   animInfo.delai = 0.12;
   i = 0;
   label = XmStringCreateLtoR(labelScaleDelai[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 2); i++;
   XtSetArg(args[i], XmNminimum, 0); i++;
   XtSetArg(args[i], XmNvalue, 12); i++;
   XtSetArg(args[i], XmNmaximum, 100); i++;
   XtSetArg(args[i], XmNtopWidget, paScaleIntervalle); i++;
   paScaleDelai = (Widget)XmCreateScale(paFrameDelai, labelScaleDelai[lng], args, i);
   XtManageChild(paScaleDelai);
   XmStringFree(label);

   XtAddCallback(paScaleDelai, XmNdragCallback, (XtCallbackProc) PaSetDelai, NULL);
   XtAddCallback(paScaleDelai, XmNvalueChangedCallback, (XtCallbackProc) PaSetDelai, NULL);



/* -------------------------------------------------------------------------------------------------- */

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, paFrameDelai); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   paFrameBoutons = (Widget)XmCreateFrame(paFormeAnimPanel, "frame", args, i); i++;
   XtManageChild(paFrameBoutons);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   paFormeBoutons = (Widget)XmCreateRowColumn(paFrameBoutons, "form", args, i);
   XtManageChild(paFormeBoutons);
   
   i = 0;
   paLastFrames = (Widget) XmCreatePushButton(paFormeBoutons, " << ", args, i);
   XtAddCallback(paLastFrames, XmNactivateCallback, (XtCallbackProc) PaLastFrames, NULL);
   XtManageChild(paLastFrames);

   i = 0;
   paLastFrame = (Widget) XmCreatePushButton(paFormeBoutons, " <  ", args, i);
   XtAddCallback(paLastFrame, XmNactivateCallback, (XtCallbackProc) PaLastFrame, NULL);
   XtManageChild(paLastFrame);

   i = 0;
   paStop = (Widget) XmCreatePushButton(paFormeBoutons, "Stop", args, i);
   XtAddCallback(paStop, XmNactivateCallback, (XtCallbackProc) PaStop, NULL);
   XtManageChild(paStop);

   i = 0;
   paNextFrame = (Widget) XmCreatePushButton(paFormeBoutons, "  > ", args, i);
   XtAddCallback(paNextFrame, XmNactivateCallback, (XtCallbackProc) PaNextFrame, NULL);
   XtManageChild(paNextFrame);

   i = 0;
   paNextFrames = (Widget) XmCreatePushButton(paFormeBoutons, " >> ", args, i);
   XtAddCallback(paNextFrames, XmNactivateCallback, (XtCallbackProc) PaNextFrames, NULL);
   XtManageChild(paNextFrames);

   animInfo.animationRapide = FALSE;
   }

/* -------------------------------------------------------------------------------------------------- */

void ActiverPanneauAnimation()
{
  
   if (!paAnimationPanel)
      InitPanneauAnim();
   
   if (!XtIsRealized(paAnimationPanel))
      {
      XtRealizeWidget(paAnimationPanel);
      CheckColormap(paAnimationPanel);
      }
   
   f77name(xpanpaact)();
   
   }

/* -------------------------------------------------------------------------------------------------- */

void f77name(xpanpaact)()
{
   LocalEventLoop(paAnimationPanel);
   }

/* -------------------------------------------------------------------------------------------------- */

void DesactiverPanneauAnim()
{
   XtUnrealizeWidget(paAnimationPanel);
   }

/* -------------------------------------------------------------------------------------------------- */

XtCallbackProc PaSetIntervalle(Widget w, caddr_t client_data, caddr_t call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   animInfo.intervalle = (float)(donnees->value);
   
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

XtCallbackProc PaSetDelai(Widget w, caddr_t client_data, caddr_t call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   animInfo.delai = 0.01 * (float)(donnees->value);
   
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
XtCallbackProc PaNextFrames(Widget w, caddr_t unused1, caddr_t unused2)
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return 0;
      }

   xc.flagInterrupt = FALSE;
   c_gmpopti("ACCEPT_INTERRUPTS", FALSE);

   InvertWidget(w);
   /*   DesactiverWidgetsControle(); */
   AfficherBoutonAnnulation();
   
   AnimerFrames(2);

   EnleverBoutonAnnulation();
   InvertWidget(w);
   ActiverWidgetsControle();

   xc.flagInterrupt = TRUE;
   c_gmpopti("ACCEPT_INTERRUPTS", TRUE);
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
XtCallbackProc PaStop(Widget w, caddr_t unused1, caddr_t unused2)
{
   xc.annulationDemandee = TRUE;
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
XtCallbackProc PaLastFrames(Widget w, caddr_t unused1, caddr_t unused2)
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return 0;
      }

   xc.flagInterrupt = FALSE;
   c_gmpopti("ACCEPT_INTERRUPTS", FALSE);

   InvertWidget(w);
   /*    DesactiverWidgetsControle(); */
   AfficherBoutonAnnulation();
   
   AnimerFrames(-2);

   EnleverBoutonAnnulation();
   InvertWidget(w);
   ActiverWidgetsControle();

   xc.flagInterrupt = TRUE;
   c_gmpopti("ACCEPT_INTERRUPTS", TRUE);
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
XtCallbackProc PaLastFrame(Widget w, caddr_t unused1, caddr_t unused2)
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return 0;
      }

   InvertWidget(w);
   AfficherBoutonAnnulation();
   AnimerFrames(-1);
   EnleverBoutonAnnulation();
   InvertWidget(w);
   return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

/** ARGSUSED **/
XtCallbackProc PaNextFrame(Widget w, caddr_t unused1, caddr_t unused2)
{
   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return 0;
      }

   InvertWidget(w);
   AfficherBoutonAnnulation();

   AnimerFrames(1);

   EnleverBoutonAnnulation();
   InvertWidget(w);
   return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

XtCallbackProc PaToggleDefilementRegulier(Widget w, caddr_t unused1, caddr_t unused2)
{
   animInfo.typeDefilement = DEFILEMENT_REGULIER;
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */
XtCallbackProc PaToggleDefilementAvantArriere(Widget w, caddr_t unused1, caddr_t unused2)
{
   animInfo.typeDefilement = DEFILEMENT_AVANT_ARRIERE;
  return 0;
   }

/* -------------------------------------------------------------------------------------------------- */
XtCallbackProc PaToggleAnimationRapide(Widget w, caddr_t u1, caddr_t u2)
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

  if (interpolationTemporelle)
    {
    i = 0;
    XtSetArg(args[i], XmNset, False); i++;
    XtSetValues(paToggleInterpolation, args, i);
    interpolationTemporelle = False;
    }

  return 0;
  
  
}

/* -------------------------------------------------------------------------------------------------- */

XtCallbackProc PaToggleInterpolation(Widget w, caddr_t unused1, caddr_t unused2)
{
  Arg args[8];
  int i;
  
  interpolationTemporelle = !interpolationTemporelle;
  
  i = 0;
  if (interpolationTemporelle)
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
    XtSetValues(paToggleAnimationRapide, args, i);
    animInfo.animationRapide = False;
    }
  return 0;
}

