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
*                 REPERTOIRE.C                              *
*******************************************************************************
**
** Fichier repertoire.c
**
** Objet: Permet d'editer la liste des repertoires preselectionnes
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

#include <xinit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#include "repertoire.h"
#include "select.h"
#include "errno.h"

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

RepertoireStruct Repertoire; /* La structure qui contient touts les widgets.*/

extern SelectStruct Select;
extern Display *wglDisp;
extern XtAppContext SuperWidget;

/* Initialisation des widgets. */

static void InitWidgetsActionFormRepertoire();
static void InitWidgetsActionsRepertoire();
static void InitWidgetsAllRepertoire();
static void InitWidgetsBasicRepertoire();
static void InitWidgetsFormRepertoire();
static void XRepertoireCallbacks();
static void XRepertoireEnleveListe();
static void XRepertoireFermer();
static void XRepertoireGetHome();
static void XRepertoireInit();
static void XRepertoireInserer();
static void XRepertoireMenuListeItem ();
static void XRepertoireSauver();
static void XRepertoireUpdateListe();

static Widget TrouverWidgetParent();

/* Fonctions appeles de l'exterieur de repertoire.c */

void XRepertoireActiver();
void XRepertoireOuvrir();

/* Fonctions appelees par repertoire.c mais qui proviennent de l'exterieur */

extern  void XSelectDeselectItem();
extern  void XSelectChangerRepertoire(); /* fichier select.c */

/* Initialisation des variables static */

static char  path_directory[256];

static int   langue = 0;
static int   nb_repertoire = 0; 

static XmString *liste_repertoires;



/******************************************************************************

NOM:              InitWidgetsActionFormRepertoire()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser la forme qui recevra les boutons de
                        commandes

APPELE PAR:       InitWidgetsAllRepertoire().

GLOBALES AFFECTES:      Repertoire.actionform

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsActionFormRepertoire()
{
   Arg  args[6];
   int  i;

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg(args[i], XmNnumColumns, 5); i++;
   XtSetArg(args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg(args[i], XmNspacing, 6); i++;

   Repertoire.actionform = (Widget)XmCreateRowColumn(Repertoire.form,
                                                     "actionform",
                                         args, i);
   XtManageChild(Repertoire.actionform);
}

/******************************************************************************

NOM:              InitWidgetsActionsRepertoire()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser les boutons ok, sauver, annuler, inserer , 
                        detruire

APPELE PAR:       InitWidgetsAllRepertoire().

GLOBALES AFFECTES:      Repertoire.ok
                  Repertoire.inserer
                  Repertoire.annuler
                  Repertoire.detruire
                  Repertoire.sauver

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsActionsRepertoire()
{
   Arg            args[5]; /* Resources des widgets qui sont utilises.  */
   int            i;     /* Nombre de resources utilisees.        */
   XmString label;       /* Le nom d'une extreme.                 */

/*.............................ok .......... ................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_repertoire_ok[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   Repertoire.ok = (Widget)XmCreatePushButtonGadget(Repertoire.actionform,
                                                   "bouton_ok", args, i);
   XtManageChild(Repertoire.ok);
   XmStringFree(label);

/*.............................annuler.......................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_repertoire_annuler[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   Repertoire.annuler = (Widget)XmCreatePushButtonGadget(Repertoire.actionform,
                                                         "bouton_annuler",
                                                         args, i);
   XtManageChild(Repertoire.annuler);
   XmStringFree(label);

/*.............................inserer.......................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_repertoire_inserer[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   Repertoire.inserer = (Widget)XmCreatePushButtonGadget(Repertoire.actionform,
                                                         "bouton_inserer",
                                                         args, i);
   XtManageChild(Repertoire.inserer);
   XmStringFree(label);

/*.............................detruire.......................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_repertoire_detruire[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label ); i++;
   Repertoire.detruire = (Widget)XmCreatePushButtonGadget(Repertoire.actionform,
                                                          "detruire", args, i);
   XtManageChild(Repertoire.detruire);
   XmStringFree(label);

/*.............................sauver....... .................................*/

   i = 0;
   label = XmStringCreateLtoR ( char_repertoire_sauver[langue],
                                XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label ); i++;
   Repertoire.sauver = (Widget)XmCreatePushButtonGadget(Repertoire.actionform,
                                                        "sauver", args, i);

   XtManageChild(Repertoire.sauver);
   XmStringFree(label);
}

/******************************************************************************

NOM:              InitWidgetsAllRepertoire()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser toutes les widgets.

APPELE PAR:       XRepertoireOuvrir.

ARGUMENTS :             argc   - nombre d'arguments passes a l'appel
                        argv   - liste des arguments

GLOBALES AFFECTES:      Repertoire

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsAllRepertoire()
{

   char *lng = "english";

   if ( lng != NULL )
      if ( tolower(lng[0]) == 'e' || tolower(lng[0]) == 'a' )
         langue = 1;

   InitWidgetsBasicRepertoire();
   InitWidgetsActionFormRepertoire();
   InitWidgetsActionsRepertoire();
   XRepertoireUpdateListe();
   XRepertoireCallbacks();
}

/******************************************************************************

NOM:              XRepertoireUpdateListe()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser le widget qui contiendra la liste
                                    - Repertoire.liste

APPELE PAR:       InitWidgetsAllRepertoire

ARGUMENTS :             creation - indique si premiere fois ou modification
                                   de la liste

GLOBALES AFFECTES:      Repertoire.liste

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireUpdateListe()
{
  Arg  args[10];
  int  i;

/**
 **  seter arguments  de la liste
 **/

   XRepertoireGetHome();
   i = 0;
   XtSetArg(args[i], XmNitems, liste_repertoires);i++;
   XtSetArg(args[i], XmNitemCount, nb_repertoire);i++;
   XtSetArg(args[i], XmNvisibleItemCount, 12);i++;
   XtSetArg(args[i], XmNlistMarginWidth, 10);i++;
   XtSetArg(args[i], XmNlistMarginHeight, 5);i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, Repertoire.actionform); i++;
   XtSetArg(args[i], XmNselectionPolicy, XmMULTIPLE_SELECT);i++;

   Repertoire.liste  = (Widget) XmCreateScrolledList(Repertoire.form,
                                                     "scrolledListe",
                                                     args, i);
   XtManageChild ( Repertoire.liste );

   for ( i = 0 ; i < nb_repertoire ; i++ )
   {
       XmStringFree ( liste_repertoires[i] );
       liste_repertoires[i] = NULL;
   }
   XtFree( liste_repertoires );
}

/******************************************************************************

NOM:              InitWidgetsBasicRepertoire()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser les widgets de haut niveau
                                    - Repertoire.toplevel
                                    - Repertoire.form

APPELE PAR:       InitWidgetsAll

ARGUMENTS :             argc     - nombre d'arguments passes a l'appel
                        argv     - liste des arguments 

GLOBALES AFFECTES:      Repertoire.toplevel
                        Repertoire.form

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsBasicRepertoire()
{
   Arg          args[7]; /* Resources des widgets qui sont utilises.      */
   int          i;           /* Nombre de resources utilisees.                  */

/*..................................toplevel..................................*/

   i = 0;
   XtSetArg(args[i], XmNsaveUnder, True); i++;
   XtSetArg(args[i], XmNallowShellResize, True); i++;
   XtSetArg(args[i], XmNmappedWhenManaged, False); i++;
   XtSetArg(args[i], XmNtitle, application_repertoire[langue]); i++;
   XtSetArg(args[i], XmNmwmFunctions, MWM_FUNC_MOVE| MWM_FUNC_RESIZE|MWM_FUNC_MINIMIZE|MWM_FUNC_MAXIMIZE); i++;
   XtSetArg(args[i], XmNmwmDecorations, MWM_DECOR_ALL);i++;
   Repertoire.toplevel = XtAppCreateShell(NULL, "repertoire",
                                    applicationShellWidgetClass,
                             wglDisp, 
                              args, i);

/*.................................form....................................*/

   i = 0;
   XtSetArg(args[i], XmNresizable, True); i++;
   XtSetArg(args[i], XmNresizePolicy, XmRESIZE_GROW); i++;
   Repertoire.form = (Widget)XmCreateForm(Repertoire.toplevel,"form",args,i);
   XtManageChild(Repertoire.form);
}

/******************************************************************************

NOM:              XRepertoireActiver()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Controle les evenements du Repertoire

APPELE PAR:       XSelectActiver

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Repertoire.statut_repertoire

VALEURE RETOURNEE:      Repertoire.statut_repertoire

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XRepertoireActiver()
{
Widget widgetParent;

   XtMapWidget(Repertoire.toplevel);
   XFlush(XtDisplay(Repertoire.toplevel));
   Repertoire.statut_repertoire = REPERTOIRE_EN_COURS;

   while (Repertoire.statut_repertoire == REPERTOIRE_EN_COURS)
         {
         XtAppPeekEvent(SuperWidget, &(Repertoire.theEvent));
         switch(Repertoire.theEvent.type)
               {
               case ButtonPress:
                    widgetParent =
                    (Widget)TrouverWidgetParent(Repertoire.theEvent.xbutton.window);
                    if (widgetParent == Repertoire.toplevel)
                       {
                        XtAppNextEvent(SuperWidget,
                                       &(Repertoire.theEvent));
                        XtDispatchEvent(&(Repertoire.theEvent));
                       }
                    else
                       Repertoire.statut_repertoire = REPERTOIRE_FINIE; 
                    break;
               default:
                        XtAppNextEvent(SuperWidget,
                                       &(Repertoire.theEvent));
                        XtDispatchEvent(&(Repertoire.theEvent));
                        break;
               }
         }
}

/******************************************************************************

NOM:              XRepertoireFermer()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Fermer le menu de Repertoire

APPELE PAR:       XRepertoireInit

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      Repertoire.statut_repertoire  

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireFermer()
{
   XtUnmapWidget(Repertoire.toplevel);
   XFlush(XtDisplay(Repertoire.toplevel));
   Repertoire.statut_repertoire == REPERTOIRE_FINIE;
}

/******************************************************************************

NOM:              XRepertoireOuvrir()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:           Ouvrir le menu de Repertoire

APPELE PAR:       Programme Principal

ARGUMENTS :             argc - nombre d'arguments passes a l'appel
                        argv - liste des arguments passes a l'appel 

GLOBALES AFFECTES:      Repertoire.statut_repertoire

VALEURE RETOURNEE:      Repertoire.toplevel

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XRepertoireOuvrir()
{

   Repertoire.statut_repertoire = REPERTOIRE_EN_COURS;
   InitWidgetsAllRepertoire();
   XtRealizeWidget(Repertoire.toplevel);
}

/******************************************************************************

NOM:              XRepertoireCallbacks()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Associe les callbacks aux differents widgets

APPELE PAR:       InitWidgetsAllRepertoire

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Tous les widgets ayant un callback

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireCallbacks()
{
 XtAddCallback ( Repertoire.ok,      XmNactivateCallback,
                                     XRepertoireInit,         NULL );
 XtAddCallback ( Repertoire.annuler, XmNactivateCallback,
                                     XSelectDeselectItem,     Repertoire.liste);
 XtAddCallback ( Repertoire.detruire,XmNactivateCallback,
                                     XRepertoireEnleveListe,  NULL );
 XtAddCallback ( Repertoire.inserer, XmNactivateCallback,
                                     XRepertoireInserer,      NULL );
 XtAddCallback ( Repertoire.sauver,  XmNactivateCallback,
                                     XRepertoireSauver,       NULL );
}

/******************************************************************************

NOM:                    XRepertoireEnleveListe()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Enlever items selectionnes dans la liste

APPELE PAR:             Callback du bouton ENLEVER

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      Repertoire.liste

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireEnleveListe (w, client, data )
Widget w;
XtPointer client, data;
{
 int i, nb_delete;
 Arg args[1];
 XmString *liste, *tmp;


/*
 * Trouve la liste selectionnee ainsi que la longueur de cette derniere
 */

   i = 0;
   XtSetArg ( args[i], XmNselectedItemCount, & nb_delete );i++;
   XtGetValues ( Repertoire.liste, args, i );

   tmp = (XmString *)XtMalloc ( nb_delete * sizeof(XmString) );

   i = 0;
   XtSetArg ( args[i], XmNselectedItems, &liste );i++;
   XtGetValues ( Repertoire.liste, args, i );

  /*
   * transfert de la liste a enlever
   */

   for ( i = 0; i <  nb_delete ; i ++ )
       tmp[i] = XmStringCopy ( liste[i] );

   /*
    * On enleve les items selectionnes de la liste 
    */

   for ( i = 0; i <  nb_delete ; i ++ )
       XmListDeleteItem ( Repertoire.liste, tmp[i] );

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
       XtFree ( tmp );
      }

   XmListDeselectAllItems ( Repertoire.liste );
}

/******************************************************************************

NOM:                    XRepertoireGetHome()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Va chercher le home directory du user et construit
                        le path pour aller chercher les path par defaut
                        dans le fichier .sfpaths

APPELE PAR:             XRepertoireUpdateListe

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      path_directory
                        nb_repertoire
                        liste_repertoires

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireGetHome ( )
{
char directory[256];
int  nombre_dir;
FILE *fpi;
XmString label;

 char *name = (char *)getenv("HOME");

 nombre_dir = NB_DIRECTORYS;
 liste_repertoires = (XmString *)XtMalloc ( nombre_dir * sizeof(XmString) );
 strcpy ( path_directory, name );
 strcat ( path_directory, "/.sfpaths" );
 fpi = fopen ( path_directory, "r" );
 if ( fpi == NULL )
 {
   if ( errno == ENOENT )
   {
     if ( isdir( name ))
     {
       fpi = fopen ( path_directory, "w" );
       fprintf (fpi, "%s\n", name );
       fclose(fpi);
       fpi = fopen ( path_directory, "r" );
     }
   }
 }
 if ( fpi != NULL )
 {
   while (  fscanf ( fpi, "%s", directory) != EOF )
   {
       if ( nb_repertoire < nombre_dir )
          liste_repertoires[nb_repertoire ++] =
                        XmStringCreateLtoR (directory,XmSTRING_DEFAULT_CHARSET);
       else
         {
          nombre_dir += 5;
          liste_repertoires = (XmString *) XtRealloc ( liste_repertoires,
                                           nombre_dir * sizeof(XmString) );
          liste_repertoires[nb_repertoire ++] =
                        XmStringCreateLtoR (directory,XmSTRING_DEFAULT_CHARSET);
         }
   }
   fclose(fpi);
 }
}

/******************************************************************************

NOM:                    XRepertoireInserer()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Insere un nouveau fichier dans la liste

APPELE PAR:             Callback du bouton INSERER

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      Repertoire.liste

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireInserer ( w, client, data )
XtPointer w, client, data;
{
    char *tmp;
    XmString str;

    tmp = XmTextGetString ( Select.current_path);
    str = XmStringCreateLtoR ( tmp , XmSTRING_DEFAULT_CHARSET );
    XmListAddItem ( Repertoire.liste, str , 0 );
    XtFree ( tmp );
    XmStringFree ( str );
}

/******************************************************************************

NOM:                    XRepertoireSauver()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Sauver la liste des repertoire par defaut

APPELE PAR:             Callback du bouton SAUVER

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireSauver ( w, client, data )
Widget w;
XtPointer client, data;
{
 Arg arg[2];
 char *text;
 FILE *fpi;
 int nombre, i;
 XmString *directorys;

/*
 * Va chercher les paths dans la liste
 */

 XtSetArg ( arg[0], XmNitemCount, &nombre );
 XtGetValues ( Repertoire.liste, arg, 1 );

/* VANH 15/02/95 
 directorys = (XmString *)XtMalloc ( nombre * sizeof(XmString) ); 
*/
 XtSetArg ( arg[0], XmNitems, &directorys );
 XtGetValues ( Repertoire.liste, arg, 1 );

/*
 * Sauver le fichier de paths
 */

 unlink ( path_directory );
 fpi = fopen ( path_directory, "w" );
 for ( i = 0; i < nombre; i ++ )
     {
      XmStringGetLtoR ( directorys[i], XmSTRING_DEFAULT_CHARSET , &text );
      fprintf (fpi, "%s\n", text );
      XtFree( text );
     }
 fclose(fpi);
}

/******************************************************************************

NOM:                    XRepertoireInit()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Prend le directoire choisi dans la liste et en faire
                        le directory courant

APPELE PAR:             Callback du bouton OK

ARGUMENTS :             NONE 

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XRepertoireInit ( w, client, data )
Widget w;
XtPointer client, data;
{
 Arg args[2];
 char *text;
 static char path[256];
 int nombre, i;
 XmString *directorys, entree;

   i = 0;
   XtSetArg ( args[i], XmNselectedItemCount, &nombre );i++;
   XtSetArg ( args[i], XmNselectedItems, &directorys );i++;
   XtGetValues ( Repertoire.liste, args, i );

/*
 * S'il y a un directoire de choisi, on en fait le directoire courant
 * S'il y a plus d'une selection on prend la derniere selectionnee
 * S'il n'y a rien de selectionne on sort tout simplement
 */
   if ( nombre != 0 )
      {
       XmStringGetLtoR( directorys[nombre-1],XmSTRING_DEFAULT_CHARSET,&text);
       XSelectChangerRepertoire ( text );
       XtFree( text );
/*     XtFree ( directorys ); */
       XmListDeselectAllItems ( Repertoire.liste );
/*     XmStringFree ( entree ); */
      }
   
    XRepertoireFermer();
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

