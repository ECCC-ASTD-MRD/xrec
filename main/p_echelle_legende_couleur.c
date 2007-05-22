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
#include <Xm/Text.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <rpnmacros.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl.h>
#include <xinit.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;
extern GeoMapInfoStruct    mapInfo;


Widget pcolorbarinfoTopLevel = NULL;
Widget pcolorbarinfoForme, pcolorbarinfoFrame, pcolorbarinfoFormeCBI, pcolorbarinfoFrameCBI,pcolorbarinfoRc, pcolorbarinfoAfficher, pcolorbarinfoOk;
Widget pcolorbarinfoFormePal, pcolorbarinfoFormeMin, pcolorbarinfoFormeMax;
Widget pcolorbarinfoLabelCBI, pcolorbarinfoLabelMin, pcolorbarinfoLabelMax;
Widget pcolorbarinfoPanneauCBI, pcolorbarinfoPanneauMin, pcolorbarinfoPanneauMax;
Widget pcolorbarinfoFrameValeurs,pcolorbarinfoRCValeurs,pcolorbarinfoFrameChoix,pcolorbarinfoChoixCBI,pcolorbarinfoAuto,pcolorbarinfoFixe;
Widget pcolorbarinfoFrameLimiteX,pcolorbarinfoFormeLimiteX,pcolorbarinfoLabelMinX,pcolorbarinfoTextMinX,pcolorbarinfoLabelMaxX,pcolorbarinfoTextMaxX;

static char *nomPanneauColorBarInfos[] = {"PanneauLegendeCouleur", "ColorBarPanel"};
static char *labelTopLevel[] = {"InfoLegendeCouleur", "ColorLegendInfo"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};

char panneaucolorbarinfoGeometrie[32];
static char pcolorbarinfoNomVar[256][5];

int pcolorbarinfoSelectionTerminee;


XtCallbackProc PColorBarInfoOk(Widget w, caddr_t unused1, caddr_t unused2)
{
   pcolorbarinfoSelectionTerminee = TRUE;
   DesactiverPanneauColorBarInfos();
   }

XtCallbackProc PColorBarInfoAfficher(Widget w, caddr_t unused1, caddr_t unused2)
{
   RedessinerFenetres();
   }


void InitPanneauColorBarInfos()
{

   int i;
   Arg args[16];
   char nomShell[128];

   int lng;
   char labelText[128];


/* Initialize toolkit */
   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauColorBarInfos[lng]);
   i = 0;
   if (0 < strlen(panneaucolorbarinfoGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneaucolorbarinfoGeometrie);
      i++;
      }
   
   XtSetArg(args[i], XmNallowShellResize, True); i++;
   pcolorbarinfoTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                 applicationShellWidgetClass,
                                 XtDisplay(SuperWidget.topLevel), args, i);
   
   i = 0;
   XtSetArg(args[i], XmNwidth, 540); i++;
   pcolorbarinfoForme = (Widget) XmCreateForm(pcolorbarinfoTopLevel, "form", args, i);
   XtManageChild(pcolorbarinfoForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcolorbarinfoOk = (Widget)XmCreatePushButton(pcolorbarinfoForme, labelOk[lng], args, i);
   XtAddCallback(pcolorbarinfoOk, XmNactivateCallback, PColorBarInfoOk, NULL);
   XtManageChild(pcolorbarinfoOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pcolorbarinfoOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pcolorbarinfoAfficher = (Widget)XmCreatePushButton(pcolorbarinfoForme, labelAfficher[lng], args, i);
   XtAddCallback(pcolorbarinfoAfficher, XmNactivateCallback, PColorBarInfoAfficher, NULL);
   XtManageChild(pcolorbarinfoAfficher);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pcolorbarinfoOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
/*   XtSetArg(args[i], XmNwidth, 400); i++;*/
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcolorbarinfoFrame = (Widget) XmCreateFrame(pcolorbarinfoForme, "form", args, i);
   XtManageChild(pcolorbarinfoFrame);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcolorbarinfoRc = (Widget)XmCreateRowColumn(pcolorbarinfoFrame, "pcolorbarinfoRc", args, i);
   XtManageChild(pcolorbarinfoRc);

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   sprintf(labelText, "ColorBarInfos...");
   XtSetArg(args[i], XmNvalue, labelText); i++;
   XtSetArg(args[i], XmNrows, 30); i++;
   XtSetArg(args[i], XmNheight, 200); i++;
   XtSetArg(args[i], XmNcolumns, 80); i++;
   XtSetArg(args[i], XmNeditMode, XmMULTI_LINE_EDIT); i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pcolorbarinfoLabelCBI = (Widget)XmCreateScrolledText(pcolorbarinfoRc, labelText, args, i);
   XtManageChild(pcolorbarinfoLabelCBI);
   }




void f77name(xpancbiact)()
{
   if (pcolorbarinfoTopLevel)
      {
      if (XtIsRealized(pcolorbarinfoTopLevel))
         {
         LocalEventLoop(pcolorbarinfoTopLevel);
         }
      }
   }

void ActiverPanneauColorBarInfos()
{
   
   if (!pcolorbarinfoTopLevel)
      InitPanneauColorBarInfos();
   
   if (!XtIsRealized(pcolorbarinfoTopLevel))
      {
      XtRealizeWidget(pcolorbarinfoTopLevel);
      }
   

   f77name(xpancbiact)();
   
   }


void DesactiverPanneauColorBarInfos()
{
  XtUnrealizeWidget(pcolorbarinfoTopLevel);

   }


XtCallbackProc AfficherColorBarInfos(Widget w, caddr_t client_data, caddr_t call_data)
{
   ActiverPanneauColorBarInfos();
   AfficherMessageInfoStandard();

   }
   
void RafraichirColorBarInfos(char texte[])
   {
   Arg args[10];
   XmString xmLabel;
   if (pcolorbarinfoTopLevel)
      {
      if (XtIsRealized(pcolorbarinfoTopLevel))
         {
         XmTextSetString(pcolorbarinfoLabelCBI, texte);
         }
      }
  }

void CalculerColorBarInfos(char texte[])
   {
   int i;
   _Champ *champ;
   
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&champ, i);
      CalculerColorBarInfos2(texte, i+1, champ);
      }
   }  

void CalculerColorBarInfos2(char texte[], int index, _Champ *champ)
   {
   }
  


void MettreAJourColorBarInfos()
  {
  MettreAJourSrcColorBarInfos();
  MettreAJourDstColorBarInfos();
  MettreAJourZoomColorBarInfos();
  }
        
void MettreAJourSrcColorBarInfos()
   {
   }
   
void MettreAJourDstColorBarInfos()
   {
   
   }


void MettreAJourZoomColorBarInfos(void)
   {
   }

