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

#include <stdio.h>
#include <rpnmacros.h>
#include <stdlib.h>
#include <xinit.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>

extern SuperWidgetStruct SuperWidget;

Widget clsTopLevel, clsForme, clsOK, clsToutSelectionner;
Widget clsEffacerSelection, clsAnnuler, clsFermerFichiers, clsListe, clsWidgetParent;
char **clsListeFich = NULL;
int clsNbFich;
int fermeurSelectionTerminee = False;

/*
  static XtCallbackProc FermeurAnnuler();
static XtCallbackProc FermeurOK();
static XtCallbackProc FermeurToutSelectionner();
static XtCallbackProc FermeurEffacerSelection();
static XtCallbackProc FermeurFermerFichiers();
*/


void XFileFermeur(char **listeFich, int *nbFich)
{
   int i;
   XEvent clsEvent;
   int numItems;
   XmStringTable fichiersRestants;
   Arg args[10];
   
   Colormap cmap;

  
   InitFermeurShell();
   InitFermeurForm(listeFich, *nbFich);
   
   XtRealizeWidget(clsTopLevel);
   c_wglgetcmap(&cmap);
   i = 0;
   if (cmap != -1)
      {
      XtSetArg(args[i], XmNcolormap, cmap); i++;
      }
   XtSetValues(clsTopLevel, args, i);
   
   fermeurSelectionTerminee = False;
   while (!fermeurSelectionTerminee)
      {
      XtAppPeekEvent(SuperWidget.contexte, &(clsEvent));
      switch(clsEvent.type)
         {
         case ButtonPress:
         clsWidgetParent =(Widget) TrouverWidgetParent(clsEvent.xbutton.window);
         if (clsWidgetParent == clsTopLevel)
            {
            XtAppNextEvent(SuperWidget.contexte, &(clsEvent));
            XtDispatchEvent(&(clsEvent));
            }
         else
            {
	    fermeurSelectionTerminee = TRUE;
	    XtDestroyWidget(clsTopLevel);
	    return;
            }
         break;
         
         default:
         XtAppNextEvent(SuperWidget.contexte, &(clsEvent));
         XtDispatchEvent(&(clsEvent));
         break;
         }
      }
   
   i=0;  
   XtSetArg(args[i], XmNitemCount, &numItems); i++;
   XtSetArg(args[i], XmNitems, &fichiersRestants); i++;
   XtGetValues(clsListe, args, i);

   *nbFich = numItems;
   for (i=0; i < numItems; i++)
      {
      XmStringGetLtoR(fichiersRestants[i], XmSTRING_DEFAULT_CHARSET, &listeFich[i]);
      }
   
   XtDestroyWidget(clsTopLevel);
   
   }

void f77name(xfcl)(char *listeFich, int *nfich, int lenFich)
{
   int i;

   clsNbFich = *nfich;
   clsListeFich = (char **) calloc(*nfich, sizeof(char *));
   for (i=0; i < clsNbFich; i++)
      {
      clsListeFich[i] = calloc(lenFich+1, sizeof(char));
      strncpy(clsListeFich[i], &(listeFich[lenFich*i]), lenFich);
      strclean(clsListeFich[i]);
      /*      for (j=0; j < strlen(clsListeFich[i]); j++)
	      clsListeFich[i][j] = (char)tolower((int)clsListeFich[i][j]);
      */
      }
   
   XFileFermeur(clsListeFich, nfich);

   for (i=0; i < *nfich; i++)
      {
      strcpy(&(listeFich[i*lenFich]), clsListeFich[i]);
      }

   for (i=0; i < clsNbFich; i++)
      {
      free(clsListeFich[i]);
      }
	 
   free(clsListeFich);
   return;
   }



void InitFermeurShell()
{
   int i, lng;
   Arg args[10];
   static char *titreFenetreCls[] = {"Fermeture de fichiers", "File Fermeur" };

   lng = c_getulng();

   i = 0;
   XtSetArg(args[i], XtNtitle, titreFenetreCls[lng]); i++;
   clsTopLevel = XtAppCreateShell(NULL, "classeBidon", 
				  applicationShellWidgetClass,
				  XtDisplay(SuperWidget.topLevel), args, i);

   }

void InitFermeurForm(char **listeFich, int nbFich)
{
   int i, lng;
   Arg args[16];
   XmString label;
   XmStringTable table;

   static char *labelOK[] = {"Ok", "Ok"};
   static char *labelToutSelectionner[] = {"Selectionner\ntous les fichiers", "Select all files"};
   static char *labelEffacerSelection[] = {"Effacer\nselection", "Erase\nselection"};
   static char *labelAnnuler[] = {"Annuler", "Cancel"};
   static char *labelFermerFichiers[] = {"Fermer\nles fichiers\nselectionnes", "Close\nselected\nfiles"};

   lng = c_getulng();

   i = 0;
   clsForme = (Widget)XmCreateForm(clsTopLevel,
                                        "panneauContour",  
                                         args, i);
   XtManageChild(clsForme);

   
   table = calloc(nbFich, sizeof(XmString *));
   for (i=0; i < nbFich; i++)
      {
      table[i] = XmStringCreate(listeFich[i], XmSTRING_DEFAULT_CHARSET);
      }
		  
   i=0;  
   XtSetArg(args[i], XmNitems, table); i++;
   XtSetArg(args[i], XmNitemCount, nbFich); i++;
   XtSetArg(args[i], XmNvisibleItemCount, 12); i++;
   XtSetArg(args[i], XmNselectionPolicy, XmMULTIPLE_SELECT); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++; 
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++; 
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightPosition, 75); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNscrollBarPlacement, XmBOTTOM_LEFT); i++; 
   XtSetArg(args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++; 

   clsListe = (Widget)XmCreateScrolledList(clsForme,"Liste",args, i);
   XtManageChild(clsListe);

   for (i=0; i < nbFich; i++)
      {
      XmStringFree(table[i]);
      }
   free(table);
		  

   i = 0;
   label = XmStringCreateLtoR(labelOK[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 75); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   clsOK = (Widget)XmCreatePushButtonGadget(clsForme, labelOK[lng], args, i);
   XtAddCallback(clsOK, XmNactivateCallback, (XtCallbackProc) FermeurOK, NULL);
   XmStringFree(label);
   XtManageChild(clsOK);
   
   i = 0;
   label = XmStringCreateLtoR(labelAnnuler[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, clsOK); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 75); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   clsAnnuler = (Widget)XmCreatePushButtonGadget(clsForme, labelAnnuler[lng], args, i); 
   XtAddCallback(clsAnnuler, XmNactivateCallback, (XtCallbackProc) FermeurAnnuler, NULL);
   XmStringFree(label);
   XtManageChild(clsAnnuler);
   
   i = 0;
   label = XmStringCreateLtoR(labelToutSelectionner[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, clsAnnuler); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 75); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   clsToutSelectionner = (Widget)XmCreatePushButton(clsForme, labelToutSelectionner[lng], args, i);
   XtAddCallback(clsToutSelectionner, XmNactivateCallback, (XtCallbackProc) FermeurToutSelectionner, NULL);
   XmStringFree(label);
   XtManageChild(clsToutSelectionner);
   
   i = 0;
   label = XmStringCreateLtoR(labelEffacerSelection[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, clsToutSelectionner); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 75); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   clsEffacerSelection = (Widget)XmCreatePushButton(clsForme, labelEffacerSelection[lng], args, i);
   XtAddCallback(clsEffacerSelection, XmNactivateCallback, (XtCallbackProc) FermeurEffacerSelection, NULL);
   XmStringFree(label);
   XtManageChild(clsEffacerSelection);
   
   i = 0;
   label = XmStringCreateLtoR(labelFermerFichiers[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, clsEffacerSelection); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 75); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   clsFermerFichiers = (Widget)XmCreatePushButton(clsForme, labelFermerFichiers[lng], args, i);
   XtAddCallback(clsFermerFichiers, XmNactivateCallback, (XtCallbackProc) FermeurFermerFichiers, NULL);
   XmStringFree(label);
   XtManageChild(clsFermerFichiers);
   return;
   }

XtCallbackProc FermeurToutSelectionner()
{
   int i;
   Arg args[2];

   int numItems;
   XmStringTable items;

   i=0;  
   XtSetArg(args[i], XmNitemCount, &numItems); i++;
   XtSetArg(args[i], XmNitems, &items); i++;
   XtGetValues(clsListe, args, i);

   for (i=0; i < numItems; i++)
      XmListSelectItem(clsListe, items[i], False);
   return 0;
   }

XtCallbackProc FermeurEffacerSelection()
{
   XmListDeselectAllItems(clsListe);
   return 0;
   }

XtCallbackProc FermeurOK()
{
   FermeurFermerFichiers();
   fermeurSelectionTerminee = True;
   return 0;
   }

XtCallbackProc FermeurFermerFichiers()
{
   int i;
   Arg args[10];

   int numItems;
   XmStringTable selectedItems;

   i=0;  

   i=0;  
   XtSetArg(args[i], XmNselectedItemCount, &numItems); i++;
   XtSetArg(args[i], XmNselectedItems, &selectedItems); i++;
   XtGetValues(clsListe, args, i);

   XmListDeleteItems(clsListe, selectedItems, numItems);
   return 0;
   }

XtCallbackProc FermeurAnnuler(Widget w, caddr_t unused1, caddr_t unused2)
{
   int i;
   Arg args[10];

   XmStringTable table;

   table = calloc(clsNbFich, sizeof(XmString *));
   for (i=0; i < clsNbFich; i++)
      {
      table[i] = XmStringCreate(clsListeFich[i], XmSTRING_DEFAULT_CHARSET);
      }
		  
   i=0;  
   XtSetArg(args[i], XmNitems, table); i++;
   XtSetArg(args[i], XmNitemCount, clsNbFich); i++;
   XtSetValues(clsListe, args, i);

   for (i=0; i < clsNbFich; i++)
      {
      XmStringFree(table[i]);
      }
   free(table);
   fermeurSelectionTerminee = True;
   return 0;
   }

