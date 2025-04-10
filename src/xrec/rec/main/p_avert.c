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
#include <Xm/List.h>
#include <Xm/MessageB.h>
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
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <rmn/rpnmacros.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget avrtWidget, errorWidget, infoWidget, okCancelWidget, widgetCourant;

static char *labelOui[] = {"Oui", "Yes"};
static char *labelNon[] = {"Non", "No"};
static char *labelQuitter[] = {"Quitter", "Quit"};
static char *finiRaide[]    = {"Impossible d'ouvrir le fichier suivant:\n", 
				  "Can't open the following file:"};
int avrtSelectionTerminee;
int okCancel = 0;

void AvrtOk(Widget w, XtPointer client_data, XtPointer call_data) 
{
   avrtSelectionTerminee = TRUE;
   DesactiverPanneauAvertissement();

   okCancel = (int)client_data;
   return;
   }

void ErrorQuit(Widget w, XtPointer client_data, XtPointer call_data) 
{
   exit(13);
   }

void InitPanneauxInfo()
{
   avrtWidget = (Widget) CreateWarningDialog(xc.topLevel);
   infoWidget = (Widget) CreateInfoDialog(xc.topLevel);
   okCancelWidget = (Widget) CreateWarningDialogWithCancelBox(xc.topLevel);
   errorWidget = (Widget) CreateErrorDialog(SuperWidget.topLevel);
   }

/** ---------------------------------------------------------------------------------------- **/

int f77name(messerr)(char message[], F2Cl flen)
{
   char longMessage[256];
   int lng;
   int len=flen;

   message[len-1] = '\0';
   lng = c_getulng();

   strclean(message);
   strcpy(longMessage, finiRaide[lng]);
   strcat(longMessage, message);
   
   MessageErreur(longMessage);
   return 0;
   }

void MessageErreur(char message[])
{
   XEvent avrtEvent;
   Arg args[2];
   XmString label;
   
   Xinit("xrec");
   if (errorWidget  == NULL)
      {
      errorWidget = (Widget) CreateErrorDialog(SuperWidget.topLevel);
      }
   
   label = XmStringCreateLtoR(message, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNmessageString, label);
   
   XtSetValues(errorWidget, args, 1);
   widgetCourant = errorWidget;
   
   XmStringFree(label);
   
   XtManageChild(errorWidget);
   FlusherTousLesEvenements();

   avrtSelectionTerminee = False;
   while (0 == 0)
      {
      XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
      XtDispatchEvent(&(avrtEvent));
      }
   }

int MessageAvertissement(char *message, int typeMessage)
{   XEvent avrtEvent;
   Arg args[2];
   XmString label;
   Widget avrtWidgetParent;

   label = XmStringCreateLtoR(message, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNmessageString, label);
   
   switch(typeMessage)
      {
      case INFO:
      XtSetValues(infoWidget, args, 1);
      widgetCourant = infoWidget;
      break;
      
      case AVERTISSEMENT:
      XtSetValues(avrtWidget, args, 1);
      widgetCourant = avrtWidget;
      break;
      
      case OK_CANCEL:
      XtSetValues(okCancelWidget, args, 1);
      widgetCourant = okCancelWidget;
      break;
      }
   
   XmStringFree(label);
   
   avrtSelectionTerminee = False;
   
   DesactiverWidgetsControle();
   AjusterPositionForme(widgetCourant, xc.topLevel);
   
   XtManageChild(widgetCourant);
   FlusherTousLesEvenements();

   CheckColormap(widgetCourant);

   while (!avrtSelectionTerminee)
      {
      XtAppPeekEvent(SuperWidget.contexte, &(avrtEvent));
      switch(avrtEvent.type)
         {
         case ButtonPress:
         avrtWidgetParent =(Widget) TrouverWidgetParent(avrtEvent.xbutton.window);
         if (avrtWidgetParent == xc.topLevel)
            {
            XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
            XtDispatchEvent(&(avrtEvent));
            }
         else
            {
            XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
            }
         break;
         
         default:
         XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
         XtDispatchEvent(&(avrtEvent));
         break;
         }
      
      }
   ActiverWidgetsControle();
   return okCancel;
 
  }


int  MessageAvertissementAux(char *message, int typeMessage, Widget widgetMessage, Widget widgetParent)
{
   Widget avrtWidgetParent;
   XEvent avrtEvent;
   Arg args[2];
   XmString label;

   label = XmStringCreateLtoR(message, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNmessageString, label);
   
   XtSetValues(widgetMessage, args, 1);
   widgetCourant = widgetMessage;

   XmStringFree(label);

   avrtSelectionTerminee = False;
   
   AjusterPositionForme(widgetCourant, widgetParent);

   XtManageChild(widgetCourant);
   FlusherTousLesEvenements();
   CheckColormap(widgetCourant);

   while (!avrtSelectionTerminee)
      {
      XtAppPeekEvent(SuperWidget.contexte, &(avrtEvent));
      switch(avrtEvent.type)
         {
         case ButtonPress:
         avrtWidgetParent =(Widget) TrouverWidgetParent(avrtEvent.xbutton.window);
         if (avrtWidgetParent == widgetParent)
            {
            XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
            XtDispatchEvent(&(avrtEvent));
            }
         else
            {
            XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
            }
         break;
         
         default:
         XtAppNextEvent(SuperWidget.contexte, &(avrtEvent));
         XtDispatchEvent(&(avrtEvent));
         break;
         }

      }
   return okCancel;
   }


void DesactiverPanneauAvertissement()
{
   XtUnmanageChild(widgetCourant);
   }


Widget CreateWarningDialogWithCancelBox(Widget widgetParent)
{
   int i, lng;
   Widget warning, bouton;
   XmString label1, label2;
   Arg args[8];
   
   lng = c_getulng();

   i = 0;
   label1 = XmStringCreateLtoR(labelNon[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNcancelLabelString, label1); i++;

   label2 = XmStringCreateLtoR(labelOui[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNokLabelString, label2); i++;
   XtSetArg(args[i], XmNinitialResourcesPersistent, False); i++;

   warning = (Widget)XmCreateWarningDialog(widgetParent, "popup", args, i);
   CheckColormap(warning);

   XmStringFree(label1);
   XmStringFree(label2);

   bouton = (Widget)XmMessageBoxGetChild(warning, XmDIALOG_HELP_BUTTON);
   XtUnmanageChild(bouton);

   XtAddCallback(warning, XmNcancelCallback, AvrtOk, BTN_CANCEL);
   XtAddCallback(warning, XmNokCallback, AvrtOk, (XtPointer) BTN_OK);

   return warning;
   }


Widget CreateInfoDialog(Widget widgetParent)
{
   int i, lng;
   Widget info, bouton;
   Arg args[8];

   lng = c_getulng();

   i = 0;
   XtSetArg(args[i], XmNinitialResourcesPersistent, False); i++;
   info = (Widget)XmCreateWarningDialog(widgetParent, "popup", args, i);
   CheckColormap(info);

   bouton = (Widget)XmMessageBoxGetChild(info, XmDIALOG_CANCEL_BUTTON);
   XtUnmanageChild(bouton);

   bouton = (Widget)XmMessageBoxGetChild(info, XmDIALOG_HELP_BUTTON);
   XtUnmanageChild(bouton);

   XtAddCallback(info, XmNokCallback, AvrtOk, NULL);

   return info;

   }


Widget CreateWarningDialog(Widget widgetParent)
{
   int i, lng;
   Widget warning, bouton;
   Arg args[8];

   lng = c_getulng();

   i = 0;
   warning = (Widget)XmCreateWarningDialog(widgetParent, "popup", args, i);
   XtSetArg(args[i], XmNinitialResourcesPersistent, False); i++;
   CheckColormap(warning);

   bouton = (Widget)XmMessageBoxGetChild(warning, XmDIALOG_CANCEL_BUTTON);
   XtUnmanageChild(bouton);

   bouton = (Widget)XmMessageBoxGetChild(warning, XmDIALOG_HELP_BUTTON);
   XtUnmanageChild(bouton);

   XtAddCallback(warning, XmNokCallback, AvrtOk, NULL);

   return warning;

   }


Widget CreateErrorDialog(Widget widgetParent)
{
   int i, lng;
   Widget error, bouton;
   XmString label;
   Arg args[8];

   lng = c_getulng();

   i = 0;
   label = XmStringCreateLtoR(labelQuitter[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNokLabelString, label); i++;
   error = (Widget)XmCreateErrorDialog(widgetParent, "popup", args, i);
   XtSetArg(args[i], XmNinitialResourcesPersistent, False); i++;
   CheckColormap(error);

   bouton = (Widget)XmMessageBoxGetChild(error, XmDIALOG_CANCEL_BUTTON);
   XtUnmanageChild(bouton);

   bouton = (Widget)XmMessageBoxGetChild(error, XmDIALOG_HELP_BUTTON);
   XtUnmanageChild(bouton);

   XtAddCallback(error, XmNokCallback, ErrorQuit, NULL);

   return error;

   }

