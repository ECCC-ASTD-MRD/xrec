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

/* #ident "CMC/DOE: %W% %G%" */

/******************************************************************************
*                 EDIT.C                                    *
*******************************************************************************
**
** Fichier edit.c
**
** Objet: Permet d'editer la liste des fichiers selectionnes
**
** Language: C
**
** Environnement: X11, Xt, Motif
**
** Creation: Decembre 1990
**    
** Revision : Mai 1991      Guy Asselin
**                          Standardisation pour mettre dans les librairies
**
******************************************************************************/

#include "xinit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/MessageB.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

#include "edit.h"
#include "repertoire.h"
#include "select.h"

/*
#include <Xm/AccTextT.h>
#include <Xm/ActivatableT.h>
#include <Xm/ArrowB.h>
#include <Xm/ArrowBG.h>
#include <Xm/ArrowBGP.h>
#include <Xm/ArrowBP.h>
#include <Xm/AtomMgr.h>
#include <Xm/BaseClassP.h>
#include <Xm/BulletinB.h>
#include <Xm/BulletinBP.h>
#include <Xm/CacheP.h>
#include <Xm/CareVisualT.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/CascadeBGP.h>
#include <Xm/CascadeBP.h>
#include <Xm/ColorObjP.h>
#include <Xm/ComboBox.h>
#include <Xm/ComboBoxP.h>
#include <Xm/Command.h>
#include <Xm/CommandP.h>
#include <Xm/ContItemT.h>
#include <Xm/Container.h>
#include <Xm/ContainerP.h>
#include <Xm/ContainerT.h>
#include <Xm/CutPaste.h>
#include <Xm/DesktopP.h>
#include <Xm/DialogS.h>
#include <Xm/DialogSEP.h>
#include <Xm/DialogSP.h>
#include <Xm/DialogSavvyT.h>
#include <Xm/Display.h>
#include <Xm/DisplayP.h>
#include <Xm/DragC.h>
#include <Xm/DragCP.h>
#include <Xm/DragDrop.h>
#include <Xm/DragIcon.h>
#include <Xm/DragIconP.h>
#include <Xm/DragOverS.h>
#include <Xm/DragOverSP.h>
#include <Xm/DrawP.h>
#include <Xm/DrawingA.h>
#include <Xm/DrawingAP.h>
#include <Xm/DrawnB.h>
#include <Xm/DrawnBP.h>
#include <Xm/DropSMgr.h>
#include <Xm/DropSMgrP.h>
#include <Xm/DropTrans.h>
#include <Xm/DropTransP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/FileSB.h>
#include <Xm/FileSBP.h>
#include <Xm/Form.h>
#include <Xm/FormP.h>
#include <Xm/Frame.h>
#include <Xm/FrameP.h>
#include <Xm/Gadget.h>
#include <Xm/GadgetP.h>
#include <Xm/GrabShell.h>
#include <Xm/GrabShellP.h>
#include <Xm/IconG.h>
#include <Xm/IconGP.h>
#include <Xm/IconH.h>
#include <Xm/JoinSideT.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/LabelGP.h>
#include <Xm/LabelP.h>
#include <Xm/LayoutT.h>
#include <Xm/List.h>
#include <Xm/ListP.h>
#include <Xm/MainW.h>
#include <Xm/MainWP.h>
#include <Xm/Manager.h>
#include <Xm/ManagerP.h>
#include <Xm/MenuShell.h>
#include <Xm/MenuShellP.h>
#include <Xm/MenuT.h>
#include <Xm/MenuUtilP.h>
#include <Xm/MessageB.h>
#include <Xm/MessageBP.h>
#include <Xm/MwmUtil.h>
#include <Xm/NavigatorT.h>
#include <Xm/Notebook.h>
#include <Xm/NotebookP.h>
#include <Xm/PanedW.h>
#include <Xm/PanedWP.h>
#include <Xm/Primitive.h>
#include <Xm/PrimitiveP.h>
#include <Xm/Print.h>
#include <Xm/PrintSP.h>
#include <Xm/Protocols.h>
#include <Xm/ProtocolsP.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/PushBGP.h>
#include <Xm/PushBP.h>
#include <Xm/RepType.h>
#include <Xm/RowColumn.h>
#include <Xm/RowColumnP.h>
#include <Xm/SSpinB.h>
#include <Xm/SSpinBP.h>
#include <Xm/SashP.h>
#include <Xm/Scale.h>
#include <Xm/ScaleP.h>
#include <Xm/Screen.h>
#include <Xm/ScreenP.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrollBarP.h>
#include <Xm/ScrollFrameT.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrolledWP.h>
#include <Xm/SelectioB.h>
#include <Xm/SelectioBP.h>
#include <Xm/SeparatoG.h>
#include <Xm/SeparatoGP.h>
#include <Xm/Separator.h>
#include <Xm/SeparatorP.h>
#include <Xm/ShellEP.h>
#include <Xm/SpecRenderT.h>
#include <Xm/SpinB.h>
#include <Xm/SpinBP.h>
#include <Xm/TakesDefT.h>
#include <Xm/TearOffBP.h>
#include <Xm/TearOffP.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/TextFP.h>
#include <Xm/TextFSelP.h>
#include <Xm/TextInP.h>
#include <Xm/TextOutP.h>
#include <Xm/TextP.h>
#include <Xm/TextSelP.h>
#include <Xm/TextStrSoP.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/ToggleBGP.h>
#include <Xm/ToggleBP.h>
#include <Xm/TraitP.h>
#include <Xm/Transfer.h>
#include <Xm/TransferP.h>
#include <Xm/TransferT.h>
#include <Xm/TransltnsP.h>
#include <Xm/TxtPropCv.h>
#include <Xm/VaSimpleP.h>
#include <Xm/VendorS.h>
#include <Xm/VendorSEP.h>
#include <Xm/VendorSP.h>
#include <Xm/VirtKeys.h>
#include <Xm/VirtKeysP.h>
#include <Xm/Xm.h>
#include <Xm/XmAll.h>
#include <Xm/XmIm.h>
#include <Xm/XmP.h>
#include <Xm/XmStrDefs.h>

#include <Xm/XmosP.h>
#include <Xm/XpmP.h>
*/
EditStruct       Edit; 

extern Display *wglDisp; 
extern XtAppContext SuperWidget;

extern SelectStruct     Select; 
extern RepertoireStruct Repertoire;

extern void XSelectDeselectItem(Widget w, XtPointer client, XtPointer data);
static Widget TrouverWidgetParent(Window);


/* Initialisation des widgets. */

static void InitWidgetsActionFormEdit();
static void InitWidgetsActionsEdit();
static void InitWidgetsAllEdit();
static void InitWidgetsBasicEdit();
static void XEditCallbacks();
static void XEditEnleveListe(Widget w, XtPointer client, XtPointer data);
void XEditEnleveTout(Widget w, XtPointer client, XtPointer data);
static void XEditFermer(Widget, XtPointer, XtPointer);
static void XEditInitListe();
static void XEditMenuListeItem ( );

/* Fonctions appeles de l'exterieur de edit.c */

void  XEditActiver();
void  XEditOuvrir();

/* Initialisation des variables static */

static int   langue = 0;


/******************************************************************************

NOM:              InitWidgetsActionFormEdit()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser la forme qui recevra les boutons de
                        commandes

APPELE PAR:       InitWidgetsAllEdit().

GLOBALES AFFECTES:      Edit.actionform

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsActionFormEdit()
{
   Arg  args[6];
   int  i;

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNnumColumns, 4); i++;
   XtSetArg(args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg(args[i], XmNspacing, 10); i++;

   Edit.actionform = (Widget)XmCreateRowColumn(Edit.form, "actionform",
                                       args, i);
   XtManageChild(Edit.actionform);
}

/******************************************************************************

NOM:              InitWidgetsActionsEdit()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser les boutons ok, effacer, ajouter , 
                        editer, liste des path

APPELE PAR:       InitWidgetsAllEdit().

GLOBALES AFFECTES:      Edit.ok
                  Edit.clear
                  Edit.add_list
                  Edit.edit_list
                  Edit.path_list

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsActionsEdit()
{
   Arg            args[5]; /* Resources des widgets qui sont utilises.  */
   int            i;     /* Nombre de resources utilisees.        */
   XmString label;       /* Le nom d'une extreme.                 */

/*.............................ok .......... ................................*/

   i = 0;
   label = XmStringCreateLtoR(char_edit_ok[langue], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   Edit.ok = (Widget)XmCreatePushButtonGadget(Edit.actionform,
                                              "bouton_ok", args, i);
   XtManageChild(Edit.ok);
   XmStringFree(label);

/*.............................annuler.......................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_edit_clear[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   Edit.clear = (Widget)XmCreatePushButtonGadget(Edit.actionform,
                                                 "bouton_clear", args, i);
   XtManageChild(Edit.clear);
   XmStringFree(label);

/*.............................enlever.......................................*/

   i = 0;
   label = XmStringCreateLtoR(char_edit_del[langue],XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label ); i++;
   Edit.enlever = (Widget)XmCreatePushButtonGadget(Edit.actionform,
                                                   "enlever",
                                                   args, i);
   XtManageChild(Edit.enlever);
   XmStringFree(label);

/*.............................enlever tout .................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_edit_delall[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label ); i++;
   Edit.enlever_tout = (Widget)XmCreatePushButtonGadget(Edit.actionform,
                                                        "enlever_tout",
                                                        args, i);

   XtManageChild(Edit.enlever_tout);
   XmStringFree(label);
}

/******************************************************************************

NOM:              InitWidgetsAllEdit()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser toutes les widgets.

APPELE PAR:       XEditOuvrir.

ARGUMENTS :             argc   - nombre d'arguments passes a l'appel
                        argv   - liste des arguments

GLOBALES AFFECTES:      Edit

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsAllEdit()
{

   char *lng = "english";

   if ( lng != NULL )
      if ( tolower(lng[0]) == 'e' || tolower(lng[0]) == 'a' )
         langue = 1;

   InitWidgetsBasicEdit();
   InitWidgetsActionFormEdit();
   InitWidgetsActionsEdit();
   XEditInitListe();
   XEditCallbacks();
}

/******************************************************************************

NOM:              XEditInitListe()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser le widget qui contiendra la liste
                                    - Edit.liste

APPELE PAR:       InitWidgetsAllEdit

ARGUMENTS :             creation - indique si premiere fois ou modification
                                   de la liste

GLOBALES AFFECTES:      Edit.liste

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XEditInitListe()
{
  Arg  args[10];
  int  i;

/*
 *  seter arguments  de la liste
 */

   i = 0;
   XtSetArg(args[i], XmNvisibleItemCount, 12);i++;
   XtSetArg(args[i], XmNlistMarginWidth, 10);i++;
   XtSetArg(args[i], XmNlistMarginHeight, 5);i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, Edit.actionform); i++;
/*
   XtSetArg(args[i], XmNselectionPolicy, XmMULTIPLE_SELECT);i++;
*/
   XtSetArg(args[i], XmNselectionPolicy, XmEXTENDED_SELECT);i++;
   Edit.liste  = (Widget) XmCreateScrolledList(Edit.form,
                                               "scrolledListe",
                                               args, i);
   XtManageChild ( Edit.liste );
}


/******************************************************************************

NOM:              InitWidgetsBasicEdit()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser les widgets de haut niveau
                                    - Edit.toplevel
                                    - Edit.form

APPELE PAR:       InitWidgetsAllEdit

ARGUMENTS :             argc     - nombre d'arguments passes a l'appel
                        argv     - liste des arguments

GLOBALES AFFECTES:      Edit.toplevel
                        Edit.form

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsBasicEdit()
{
   Arg          args[7]; /* Resources des widgets qui sont utilises.      */
   int          i;           /* Nombre de resources utilisees.                  */

/*..................................toplevel..................................*/

   i = 0;
   XtSetArg(args[i], XmNsaveUnder, True); i++;
   XtSetArg(args[i], XmNallowShellResize, True); i++;
   XtSetArg(args[i], XmNmappedWhenManaged, False); i++;
   XtSetArg(args[i], XmNtitle, application[langue]); i++;
   XtSetArg(args[i], XmNmwmFunctions, MWM_FUNC_MOVE| MWM_FUNC_RESIZE|MWM_FUNC_MINIMIZE|MWM_FUNC_MAXIMIZE); i++;
   XtSetArg(args[i], XmNmwmDecorations, MWM_DECOR_ALL); i++;
   Edit.toplevel = XtAppCreateShell(NULL, "editeur",
                              applicationShellWidgetClass,
                              wglDisp,
                              args, i);

/*.................................form....................................*/

   i = 0;
   XtSetArg(args[i], XmNresizable, True); i++;
   XtSetArg(args[i], XmNresizePolicy, XmRESIZE_GROW); i++;
   Edit.form = (Widget)XmCreateForm(Edit.toplevel,"form",args,i);
   XtManageChild(Edit.form);
}

/******************************************************************************

NOM:              XEditActiver()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Controle les evenements du Edit

APPELE PAR:       Programme Principal

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Edit.statut_edit

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XEditActiver()
{
Widget widgetParent;

   XtMapWidget(Edit.toplevel);
   XFlush(XtDisplay(Edit.toplevel));
   Edit.statut_edit = EDIT_EN_COURS;

   while (Edit.statut_edit == EDIT_EN_COURS)
         {
         XtAppPeekEvent(SuperWidget, &(Edit.theEvent));
         switch(Edit.theEvent.type)
               {
               case ButtonPress:
                    widgetParent = 
                    (Widget)TrouverWidgetParent(Edit.theEvent.xbutton.window);
                    if (widgetParent == Edit.toplevel)
                       {
                        XtAppNextEvent(SuperWidget,&(Edit.theEvent));
                        XtDispatchEvent(&(Edit.theEvent));
                       }
                    else
                       Edit.statut_edit = EDIT_FINIE;
                    break;
               default:
                        XtAppNextEvent(SuperWidget, &(Edit.theEvent));
                        XtDispatchEvent(&(Edit.theEvent));
                        break;
               }
         }
}

/******************************************************************************

NOM:              XEditFermer()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Fermer le menu de Edit      

APPELE PAR:       Callback du bouton OK

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      Edit.statut_edit  

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XEditFermer(w, client, data)
Widget w; XtPointer client, data;
{
   XtUnmapWidget(Edit.toplevel);
   XFlush(XtDisplay(Edit.toplevel));
   Edit.statut_edit = EDIT_FINIE;
}

/******************************************************************************

NOM:              XEditOuvrir()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:           Ouvrir le menu de Edit      

APPELE PAR:       Programme Principal

ARGUMENTS :             argc - nombre d'arguments passes a l'appel
                        argv - liste des arguments passes a l'appel 

GLOBALES AFFECTES:      Edit.statut_edit

VALEURE RETOURNEE:      Edit.toplevel

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XEditOuvrir()
{
   Edit.statut_edit = EDIT_EN_COURS;
   InitWidgetsAllEdit();
   XtRealizeWidget(Edit.toplevel);
}

/******************************************************************************

NOM:              XEditCallbacks()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Associe les callbacks aux differents widgets

APPELE PAR:       InitWidgetsAllEdit

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Tous les widgets ayant un callback

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XEditCallbacks()
{

 XtAddCallback ( Edit.ok,           XmNactivateCallback, XEditFermer ,
                                                         NULL );
 XtAddCallback ( Edit.clear,        XmNactivateCallback, XSelectDeselectItem ,
                                                         Edit.liste );
 XtAddCallback ( Edit.enlever,      XmNactivateCallback, XEditEnleveListe ,
                                                         NULL );
 XtAddCallback ( Edit.enlever_tout, XmNactivateCallback, XEditEnleveTout,
                                                         NULL);
}

/******************************************************************************

NOM:                    XEditEnleveListe()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Enlever items selectionnes dans la liste 

APPELE PAR:             Callback du bouton ENLEVE

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      Edit.liste

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XEditEnleveListe (w, client, data )
Widget w; XtPointer client, data;
{
 int i, nb_delete;
 Arg args[1];
 XmString *liste, *tmp;

/*
 * Va chercher la liste a enlever
 */

 i = 0;
 XtSetArg ( args[i], XmNselectedItemCount, &nb_delete );i++;
 XtGetValues ( Edit.liste, args, i );

 i = 0;
 XtSetArg ( args[i], XmNselectedItems, &liste );i++;
 XtGetValues ( Edit.liste, args, i );

  /*
   * transfert de la liste a enlever
   */
 tmp = (XmString *)XtMalloc ( nb_delete * sizeof(XmString) );
 for ( i = 0; i <  nb_delete ; i ++ )
     tmp[i] = XmStringCopy ( liste[i] );
  /*
   * On enleve les items selectionnes de la liste
   */

 for ( i = 0; i <  nb_delete ; i ++ )
     XmListDeleteItem ( Edit.liste, tmp[i] );

  /*
   * On libere l'espace de travail
   */
 for ( i = 0; i <  nb_delete ; i ++ )
   XmStringFree(tmp[i]);

  /*
   *  libere derniers pointeurs
   */

   if ( nb_delete != 0 )
      {
/*     XtFree ( liste ); */
       XtFree ( (char*) tmp );
      }

   XmListDeselectAllItems ( Edit.liste );
}

/******************************************************************************

NOM:                    XEditEnleveTout()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Enlever tous les items dans la liste

APPELE PAR:             Callback du bouton EFFACE

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Edit.liste

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XEditEnleveTout ( w, client, data)
Widget w; XtPointer client, data;
{
 Arg arg[1];
 int i, nombre;


/*
 * Va chercher combien d'items il y a a enlever
 */

  XtSetArg ( arg[0], XmNitemCount, &nombre );
  XtGetValues ( Edit.liste, arg, 1 );

 /*
  * Libere l'espace occupee par la liste precedente
  */

 for ( i = nombre ; i > 0 ; i -- )
     XmListDeletePos ( Edit.liste, 0 );
}

/******************************************************************************

NOM:                    TrouverWidgetParent()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Trouver le widget qui est le plus vieil ancetre de la
                        fenetre ou l'evenement X s'est produit

APPELE PAR:             XSelectActiver

ARGUMENTS :             w        widget ou a eu lieu l'action
                        client
                        data

GLOBALES AFFECTES:      Select.statut_select 


VALEURE RETOURNEE:      Le widget ID du plus vieil ancetre de la fenetre 
                        ou l'evenment X s'est produit.

LIBRAIRIES :            Motif
------------------------------------------------------------------------------*/
static Widget TrouverWidgetParent(eventWindow)
Window  eventWindow; /* La fenetre ou l'evenment X s'est produit. */
{
   Widget  widgetCourant; /* Le widget ou l'evenement X s'est produit.   */
   Widget  widgetParent;  /* L'ancetre du widget ou evenement X s'est produit.*/

   widgetCourant = XtWindowToWidget (
                   wglDisp, eventWindow);
   
   widgetParent = XtParent(widgetCourant);
   while (widgetParent != NULL)
      {
      widgetCourant = widgetParent;
      widgetParent = XtParent(widgetCourant);
      };

   return widgetCourant;
}

