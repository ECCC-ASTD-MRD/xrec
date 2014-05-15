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
*                 SELECT.C                                  *
*******************************************************************************
**
** Fichier select.c
**
** Objet: Permet de selectionner des fichiers
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
#include <rpnmacros.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Shell.h>
#include <X11/Intrinsic.h>
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
#include <Xm/Text.h>
#include <Xm/TextF.h>


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
#include "select.h"
#include "edit.h"
#include "repertoire.h"

extern SuperWidgetStruct SuperWidget;

SelectStruct Select; /* La structure qui contient touts les widgets.    */




extern Display *wglDisp; 
/*extern XtAppContext SuperWidget; */

extern EditStruct       Edit;         
extern RepertoireStruct Repertoire;

static void InitWidgetsActions();
extern void pathinfo();
extern void XEditActiver();
extern void XEditOuvrir();
extern void XEditEnleveTout();
extern void XRepertoireOuvrir();
extern void XRepertoireActiver();

void XSelectDeselectItem();
void XSelectChangerRepertoire();


/* Initialisation des widgets. */

void c_selfic (char *liste, int maximum, int longueur, int *nombre);
void f77name(selfic) (char *liste, int  *maximum, int  *longueur, int  *nombre, F2Cl  len);

static void InitWidgetsActions();
static void InitWidgetsAll();
static void InitWidgetsBasic();
static void InitWidgetsCurrentPath();
static void InitWidgetsPulldown();
static void XSelectActiver();
static void XSelectAddToList ( Widget w, caddr_t client, caddr_t data );
static void XSelectCallbacks();
static void XSelectDirectory();
static void XSelectFermer(Widget w, caddr_t unused1, caddr_t unused2);
static void XSelectGetFichiers (char *liste, int   maximum, int   len, int  *nombre);
static void XSelectItemListeFichier();
static void XSelectMenuListeItem ( );
static void XSelectOk ( Widget w, caddr_t client, caddr_t data );
static void XSelectOuvrir();
static void XSelectTextCallback(Widget w, caddr_t unused1, caddr_t unused2);
static void XSelectUpdateDirectoryCourant();
static void XSelectUpdateListe();
static void XSelectUpdatePath();
void XSelectDeselectItem (Widget w, caddr_t client, caddr_t data);

/* Fonctions appeles de l'exterieur de select.c */

static Widget TrouverWidgetParent(Window eventWindow);

/*
 * variable globales
 */

 static char *dir_courant = NULL ;
 static char **dir_nom;
 static char **liste_fichiers;

 static int   langue    = 0;
 static int   nb_dir    = 0;
 static int   mx_dir    = 50;
 static int   nb_file   = 0;
 static int   select_done = 0;


/******************************************************************************

NOM:              InitWidgetsActions()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser les boutons ok, effacer, ajouter , 
                        editer, liste des path

APPELE PAR:       InitWidgetsAll().

GLOBALES AFFECTES:      Select.ok
                  Select.clear
                  Select.add_list
                  Select.edit_list
                  Select.path_list

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsActions()
{
   Arg            args[10]; /* Resources des widgets qui sont utilises. */
   int            i;     /* Nombre de resources utilisees.        */
   XmString label;       /* Le nom d'une extreme.                 */

/*.............................ok .......... ................................*/

   i = 0;
   label = XmStringCreateLtoR(char_select_ok[langue], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNtopPosition, 22);i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNbottomPosition, 35);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNleftPosition, 65);i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNrightPosition, 100);i++;

   Select.ok = (Widget)XmCreatePushButtonGadget(Select.form, 
                                                "bouton_ok", args, i);
   XtManageChild(Select.ok);
   XmStringFree(label);

/*.............................effacer.......................................*/

   i = 0;
   label = XmStringCreateLtoR(char_select_clear[langue], 
                              XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNtopPosition, 38);i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNbottomPosition, 51);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNleftPosition, 65);i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNrightPosition, 100);i++;

   Select.clear = (Widget)XmCreatePushButtonGadget(Select.form, 
                                                   "bouton_clear", args, i);

   XtManageChild(Select.clear);
   XmStringFree(label);

/*.............................add_list.......................................*/

   i = 0;
   label = XmStringCreateLtoR(char_select_add[langue],XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label ); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNtopPosition, 54);i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNbottomPosition, 67);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNleftPosition, 65);i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNrightPosition, 100);i++;

   Select.add_list = (Widget)XmCreatePushButtonGadget(Select.form,
                                                      "bouton_add_to_list", 
                                                      args, i);
   XtManageChild(Select.add_list);
   XmStringFree(label);

/*.............................edit_list.....................................*/

   i = 0;
   label = XmStringCreateLtoR(char_select_edit_list[langue], 
                          XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNtopPosition, 70);i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNbottomPosition, 83);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNleftPosition, 65);i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNrightPosition, 100);i++;

   Select.edit_list = (Widget)XmCreatePushButtonGadget(Select.form, 
                                                       "bouton_edit_list",
                                                       args, i);
   XtManageChild(Select.edit_list);
   XmStringFree(label);

/*.............................path_list......................................*/

   i = 0;
   label = XmStringCreateLtoR(char_select_path_list[langue], 
                          XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNtopPosition, 86);i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_POSITION);i++;
   XtSetArg(args[i], XmNbottomPosition, 98);i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNleftPosition, 65);i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION );i++;
   XtSetArg(args[i], XmNrightPosition, 100);i++;

   Select.path_list = (Widget)XmCreatePushButtonGadget(Select.form, 
                                                       "bouton_path_list", 
                                                       args, i);
   XtManageChild(Select.path_list);
   XmStringFree(label);
}

/******************************************************************************

NOM:              InitWidgetsAll()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser toutes les widgets.

APPELE PAR:       XSelectOuvrir.

ARGUMENTS :             argc   - nombre d'arguments passes a l'appel
                        argv   - liste des arguments

GLOBALES AFFECTES:      Select

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsAll()
{
   int ier;

   ier = wglinids("xrec");
   InitWidgetsBasic();
   InitWidgetsCurrentPath();
   InitWidgetsPulldown();
   XSelectUpdateListe(CREATION);
   InitWidgetsActions();
   XSelectCallbacks();
}

/******************************************************************************

NOM:              InitWidgetsCurrentPath()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser le text pour affichage du repertoire 
                        courant

APPELE PAR:       InitWidgetsAll().

GLOBALES AFFECTES:      Select.form
                  Select.current_path

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsCurrentPath()
{
   Arg            args[9]; /* Resources des widgets qui sont utilises.  */
   int            i;     /* Nombre de resources utilisees.        */
   char str[128];

   strcpy(str, "current_path");

   i = 0;
   XtSetArg(args[i], XmNvalue, (char *)dir_courant );i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM );i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM );i++;
   XtSetArg(args[i], XmNcursorPosition, strlen(dir_courant) );i++;
/*   XtSetArg(args[i], XmNrows, 1); i++;*/
/*   XtSetArg(args[i], XmNeditMode, XmSINGLE_LINE_EDIT); i++;*/
/*    XtSetArg(args[i], XmNwordWrap, True); i++; */

   Select.current_path = (Widget) XmCreateTextField(Select.form, (String)str, args, (Cardinal)i);
/*    Select.current_path = XtCreateWidget(str, xmTextFieldWidgetClass, Select.form, args, i);*/
    XtAddCallback(Select.current_path, XmNactivateCallback,
                                      (XtCallbackProc) XSelectTextCallback, (XtPointer)(long)0);
   XtManageChild(Select.current_path);
}

/******************************************************************************

NOM:              InitWidgetsPulldown()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser le pulldown pour choix du repertoire

APPELE PAR:       InitWidgetsAll().

GLOBALES AFFECTES:      menu_bar
                        pulldown
                       *dir
                        cascade

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsPulldown()
{
 Arg args[10];
 int i;
char str[128];
 XmString tmp;

/**
 **    creer menubar
 **/

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, Select.current_path); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightPosition, 65); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;

   Select.menu_bar = (Widget) XmCreateMenuBar(Select.form, "menubar", args, i);

/**
 **    creer pulldown
 **/

   i = 0;
   Select.pulldown = (Widget) XmCreatePulldownMenu(Select.menu_bar, "pulldown",
                                                   args, i);
/**
 **    creer bouton cascade
 **/

   i=0;
   XtSetArg(args[i], XmNrecomputeSize, False); i++;
   XtSetArg(args[i], XmNsubMenuId, Select.pulldown); i++;
   tmp = XmStringCreate("                  ", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, tmp ); i++;
   Select.cascade = (Widget) XmCreateCascadeButton(Select.menu_bar, "cascade",
                                                   args, i );
   XmStringFree ( tmp );

/**
 **    creer mx_dir items dans le menu
 **/

   if( nb_dir > mx_dir ) mx_dir = nb_dir;
   Select.dir = (Widget *)XtMalloc ( mx_dir * sizeof(Widget) );
   for( i=mx_dir-1; i >= 0; i--)
      {
      tmp = XmStringCreate("                  ", XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[0], XmNlabelString, tmp );
      Select.dir[i] = (Widget) XmCreatePushButtonGadget(Select.pulldown,
                                                        "dir_list", 
                                                        args, 1);
      XtAddCallback(Select.dir[i], XmNactivateCallback, (XtCallbackProc) XSelectMenuListeItem, (XtPointer) (long)i);
      XmStringFree ( tmp );
      }

/**
 **    ecrit les labels dans les items du menu
 **/

   for( i=nb_dir-1; i >= 0; i--)
      {
      tmp = XmStringCreate(dir_nom[i], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[0], XmNlabelString, tmp );
      XtSetValues( Select.dir[i], args, 1 );
      XmStringFree ( tmp );
      }

/**
 **    afficher a l'ecran
 **/

   XtManageChild(Select.cascade);
   XtManageChild(Select.menu_bar);
   XtManageChildren(Select.dir, nb_dir);

   i=0;
   tmp = XmStringCreate(dir_nom[nb_dir-1], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString,tmp ); i++ ;
   XtSetValues( Select.cascade, args, i );
   XmStringFree ( tmp );
 }

/******************************************************************************

NOM:              XSelectUpdateListe()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:         Initialiser le widget qui contiendra la liste
                                    - Select.liste

APPELE PAR:       InitWidgetsAll

ARGUMENTS :             creation - indique si premiere fois ou modification
                                   de la liste

GLOBALES AFFECTES:      Select.liste

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectUpdateListe(int creation)
{
  Arg  args[20];
  int  i, visible;
  XmString *fichiers;

  if( dir_courant != NULL )
    {
    XtFree( dir_courant );
    dir_courant = NULL ;
    }
  dir_courant = (char *) XmTextGetString(Select.current_path);
  
/*
 * si le directory courant n'est pas / , ajouter le directory ../ a la liste
 */
  
  /*  if ( strlen(dir_courant) > 1 )
      {
      ++nb_file;
      fichiers = (XmString *) XtMalloc ( nb_file * sizeof(XmString) );
      fichiers[0] = XmStringCreateLtoR ( "../", XmSTRING_DEFAULT_CHARSET );
      for ( i = 1; i < nb_file; i++ )
      fichiers[i] = XmStringCreateLtoR ( liste_fichiers[i-1],
      XmSTRING_DEFAULT_CHARSET );
      }
      else
      {
      }
  */
  /**
   **  seter arguments  de la liste
   **/
  
  
  fichiers = (XmString *) XtMalloc ( nb_file * sizeof(XmString) );
  for ( i = 0; i < nb_file; i++ )
    {
    fichiers[i] = XmStringCreateLtoR ( liste_fichiers[i], XmSTRING_DEFAULT_CHARSET );
    }
  
  
  i = 0;
  XtSetArg(args[i], XmNsensitive, True);i++;
  XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM);i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION );i++;
  XtSetArg(args[i], XmNleftPosition, 0);i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION );i++;
  XtSetArg(args[i], XmNrightPosition, 65);i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM);i++;
   XtSetArg(args[i], XmNvisibleItemCount, 14);i++;
   XtSetArg(args[i], XmNlistMarginWidth, 10);i++;
   XtSetArg(args[i], XmNlistMarginHeight, 5);i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, Select.menu_bar); i++;
   XtSetArg(args[i], XmNitems, fichiers); i++;
   XtSetArg(args[i], XmNitemCount, nb_file);i++;
/*
   XtSetArg(args[i], XmNselectionPolicy, XmMULTIPLE_SELECT);i++;
*/
   XtSetArg(args[i], XmNselectionPolicy, XmEXTENDED_SELECT);i++;
   XtSetArg(args[i], XmNscrollBarPlacement, XmBOTTOM_LEFT ); i++;
   XtSetArg(args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++; 
/*
 * Regarde si c'est la creation ou une modification de liste
 */

   if ( creation )
      {
       Select.liste  = (Widget) XmCreateScrolledList(Select.form, 
                                                     "scrolledListe",
                                                     args, i);
       XtManageChild ( Select.liste );
      }
   else
       XtSetValues ( Select.liste, args, i );

/*
 * s'il y a plus d'items que le nombre d'items visibles, repositionne a 1
 */

   i = 0;
   XtSetArg ( args[0], XmNvisibleItemCount, &visible ); i++;
   XtGetValues ( Select.liste, args, i );
   if ( nb_file > visible )
       XmListSetPos ( Select.liste, 1 );

/*
 * libere l'espace memoire prise par fichiers
 */

   for ( i = 0; i < nb_file; i++ )
        XmStringFree ( fichiers[i] ); 
   XtFree ( (char *)fichiers );
}

/******************************************************************************

NOM:              InitWidgetsBasic()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Initialiser les widgets de haut niveau
                                    - Select.toplevel
                                    - Select.form

APPELE PAR:       InitWidgetsAll

ARGUMENTS :             argc     - nombre d'arguments passes a l'appel
                        argv     - liste des arguments 

GLOBALES AFFECTES:      Select.toplevel
                        Select.form
                        Select.message

VALEURE RETOURNEE:      Aucune.

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void InitWidgetsBasic()
{
   char nomShell[128];
   static char *nomSelecteurFichiers[] = {"SelecteurDeFichiers", "FileSelector"};

   int lng;
   Arg          args[8]; /* Resources des widgets qui sont utilises.      */
   Cardinal          i;           /* Nombre de resources utilisees.                  */
   XmString label1,label2,label3;   /* Le nom des boutons.          */
   Widget   tmp;

/*..................................toplevel..................................*/

   Xinit("xregarder");

   lng = c_getulng();

   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomSelecteurFichiers[lng]);

   i = 0;
   Select.toplevel = XtAppCreateShell(nomShell, nomShell, applicationShellWidgetClass, wglDisp, args, i);

/*.................................message....................................*/
   i = 0;
   label1 = XmStringCreateLtoR ( select_tx_message[langue], 
                                 XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNmessageString, label1); i++;
   label2 = XmStringCreateLtoR(char_select_ok[langue],XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNokLabelString, label2); i++;
   label3 = XmStringCreateLtoR(label_message[langue], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNdialogTitle, label3); i++;
   Select.message = (Widget)XmCreateMessageDialog(Select.toplevel, "message",
                                                 args, i);
   XmStringFree(label1);
   XmStringFree(label2);
   XmStringFree(label3);

/*   tmp = (Widget)XmMessageBoxGetChild(Select.message, XmDIALOG_CANCEL_BUTTON);*/
/*   XtUnmanageChild(tmp);*/
/*   tmp = (Widget)XmMessageBoxGetChild(Select.message, XmDIALOG_HELP_BUTTON);*/
/*    XtUnmanageChild(tmp); */

   tmp = (Widget)XmMessageBoxGetChild(Select.message, XmDIALOG_OK_BUTTON);
   XtAddCallback ( tmp,  XmNactivateCallback, (XtCallbackProc)XSelectOk , NULL );

/*.................................form....................................*/

   i = 0;
/*
   XtSetArg(args[i], XmNresizable, True); i++;
   XtSetArg(args[i], XmNresizePolicy, XmRESIZE_GROW); i++;
*/
   XtSetArg(args[i], XmNwidth, 300); i++;
   XtSetArg(args[i], XmNheight, 350); i++;
   Select.form = (Widget)XmCreateForm(Select.toplevel,"form",args,i);
   XtManageChild(Select.form);
}

/******************************************************************************

NOM:              XSelectActiver()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:         Controle les evenements du Select

APPELE PAR:       Programme Principal

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Select.statut_select

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectActiver()
{

Widget widgetParent;


   XtMapWidget(Select.toplevel);
   XFlush(XtDisplay(Select.toplevel));
   Select.statut_select = SELECT_EN_COURS;

   while (Select.statut_select == SELECT_EN_COURS)
         {
         XtAppPeekEvent(SuperWidget.contexte, &(Select.theEvent));
         switch(Select.theEvent.type)
               {
               case ButtonPress:
                    widgetParent =
                    (Widget)TrouverWidgetParent(Select.theEvent.xbutton.window);
                    if (widgetParent == Select.toplevel)
                       {
                       XtAppNextEvent(SuperWidget.contexte,&(Select.theEvent));
                       XtDispatchEvent(&(Select.theEvent));
                       }
                    else if (widgetParent == Edit.toplevel)
                       {
                       XEditActiver();
                       }
                    else if (widgetParent == Repertoire.toplevel)
                       {
                       XRepertoireActiver();
                       }
                    else
                       {
                       XtAppNextEvent(SuperWidget.contexte,&(Select.theEvent));
                       XRaiseWindow(XtDisplay(Select.toplevel),
                                      XtWindow(Select.toplevel));
                       } 
                    break;
               default:
                       XtAppNextEvent(SuperWidget.contexte,&(Select.theEvent));
                       XtDispatchEvent(&(Select.theEvent));
                       break;
               }
         }
}

/******************************************************************************

NOM:              XSelectFermer()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Fermer le menu de Select

APPELE PAR:       Callback du bouton OK

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Select.statut_select

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void  XSelectFermer(Widget w, caddr_t unused1, caddr_t unused2)
{
   XSelectAddToList ( NULL, NULL, NULL );
   XtUnmapWidget(Repertoire.toplevel);
   XtUnmapWidget(Edit.toplevel);
   XtUnmapWidget(Select.toplevel);
   XFlush(XtDisplay(Select.toplevel));
   Select.statut_select = SELECT_FINIE;
}

/******************************************************************************

NOM:              XSelectOuvrir()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Ouvrir le menu de Select

APPELE PAR:       Programme Principal

ARGUMENTS :             argc - nombre d'arguments passes a l'appel
                        argv - liste des arguments passes a l'appel

GLOBALES AFFECTES:      Select.statut_select

VALEURE RETOURNEE:      Select.toplevel

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectOuvrir()
{
   Select.statut_select = SELECT_EN_COURS;
   InitWidgetsAll();
   XtRealizeWidget(Select.toplevel);
}

/******************************************************************************

NOM:              XSelectDirectory()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C 

FONCTION:           Va chercher le directory courant

APPELE PAR:       Programme Principal

ARGUMENTS :             NONE

GLOBALES AFFECTES:      dir_courant 
                        nb_dir
                        dir_nom
                        nb_file
                        liste_fichiers

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectDirectory()
{
  char *toto=NULL;
  char buffer [256];
  int len_toto = 0;

  /*
   * nom du directory courant
   */

    dir_nom = NULL;
    liste_fichiers = NULL ;
    if ( dir_courant != NULL ) 
      {
      XtFree( dir_courant );
      dir_courant = NULL ;
      }
/*
     if ((dir_courant = (char *) getcwd ((char *)NULL, 256)) == NULL )
*/
     toto = (char *) getcwd ((char *)buffer, 256);
     if (toto != NULL)
       {
       len_toto = strlen(toto);
       dir_courant = (char *) malloc (len_toto + 10);
       strcpy(dir_courant, toto);
       }
     
     if (dir_courant == NULL)
       {
       perror("getcwd");
       exit(2);
       }
     pathinfo ( dir_courant, &nb_dir, &dir_nom, &nb_file, &liste_fichiers );
}

/******************************************************************************

NOM:              XSelectCallbacks()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:           Associe les callbacks aux differents widgets

APPELE PAR:       InitWidgetsAll

ARGUMENTS :             NONE

GLOBALES AFFECTES:      Tous les widgets ayant un callback

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectCallbacks()

{
 XtAddCallback ( Select.ok,        XmNactivateCallback,
                                   (XtCallbackProc) XSelectFermer ,          NULL );
 XtAddCallback ( Select.clear,     XmNactivateCallback,
                                    (XtCallbackProc) XSelectDeselectItem ,    Select.liste );
 XtAddCallback ( Select.add_list,  XmNactivateCallback,
                                    (XtCallbackProc) XSelectAddToList ,       NULL );
 XtAddCallback ( Select.edit_list, XmNactivateCallback,
                                    (XtCallbackProc)XEditActiver ,           NULL );
 XtAddCallback ( Select.path_list, XmNactivateCallback,
                                    (XtCallbackProc)XRepertoireActiver ,     NULL );
/*
 XtAddCallback ( Select.liste,     XmNmultipleSelectionCallback,
                                   XSelectItemListeFichier, NULL );
*/
 XtAddCallback ( Select.liste,     XmNextendedSelectionCallback,
                                    (XtCallbackProc)XSelectItemListeFichier, NULL );
 XtAddCallback ( Select.liste,     XmNdefaultActionCallback,
                                    (XtCallbackProc)XSelectAddToList,        NULL );
}

/******************************************************************************

NOM:                    XSelectTextCallback()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Change le directory courant a partir du widget text

APPELE PAR:             Callback du Select.current_path

ARGUMENTS :             w - ou s'est produite l'action
                        unused1, unused2 

GLOBALES AFFECTES:      dir_courant

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectTextCallback(Widget w, caddr_t unused1, caddr_t unused2)
{
   char *pos;
   int i, nombre, turn_on;
   Arg args[1];

  if ( dir_courant != NULL ) {
      XtFree( dir_courant );
      dir_courant = NULL ;
  }

  dir_courant = (char *) XmTextGetString(w);

/**
 **  Verifier si l'usager a mis un '/' a la fin du path
 **  et, si oui, l'enlever.
 **/

  if( strlen(dir_courant) != 1 && dir_courant[strlen(dir_courant)-1] == '/' )
     dir_courant[strlen(dir_courant)-1] = '\0';

/*
 * verifie si le dernier mot du path a des * ou des ? 
 */

  turn_on = 0;
  pos = strrchr(dir_courant,'/');
  if( pos != NULL &&
    ( strrchr(pos,'?') != NULL || strrchr(pos,'*') != NULL )) turn_on = 1;

  XSelectChangerRepertoire(dir_courant);

/*
 * if dernier mot du path a des * ou des ? 
 * Va chercher le nombre de fichiers dans la liste
 * Boucle sur tous les items pour les selectionnees
 */
   if( turn_on == 1 )
    {
     i = 0;
     XtSetArg ( args[i], XmNitemCount, &nombre );i++;
     XtGetValues ( Select.liste, args, i );
     if( nombre > 1 )
       {
       for( i=nombre-1; i>1 ;i-- )
          XmListSelectPos ( Select.liste, i, FALSE );
       XmListSelectPos ( Select.liste, 0, FALSE );
       }
       if( pos == dir_courant ) XmListSelectPos ( Select.liste, 1, FALSE );
     }
   else
     XmListDeselectAllItems ( Select.liste );
        

}

/******************************************************************************

NOM:                    XSelectChangerRepertoire()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Change le directory courant a partir du widget text
                        et reinitialise la liste

APPELE PAR:             XRepertoireInit
                        XSelectTextCallback
                        XSelectMenuListeItem
                        XSelectItemListeFichier

ARGUMENTS :             dir - directory

GLOBALES AFFECTES:      liste_fichiers
                        nb_file

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XSelectChangerRepertoire(char *path)
{
   Arg args[5];
   static char entree[256];
   static char testdr[256];
   int i;
   XmString tmp;

   testdr[0] = '\0';

   strcpy(entree,path);
   XFlush(XtDisplay(Select.toplevel));
   XtUnmanageChildren ( Select.dir, nb_dir );
/**
 **  detruire et recreer la liste des directory
 **/

   pathinfo(entree, &nb_dir, &dir_nom, &nb_file, &liste_fichiers );

/**
 **  cree des PushButtonGadget dans le pulldown s'il en manque
 **/

   if( nb_dir > mx_dir )
     {
     Select.dir = (Widget *)XtRealloc ( (char *)Select.dir, (Cardinal) (nb_dir*sizeof(Widget)) );
     for (i=nb_dir-1; i >= mx_dir; i-- )
        {
        tmp = XmStringCreate("                  ", XmSTRING_DEFAULT_CHARSET);
        XtSetArg(args[0], XmNlabelString , tmp );
        Select.dir[i] = (Widget) XmCreatePushButtonGadget(Select.pulldown, 
                                                          "directory_list",
                                                          args , 1);
        XtAddCallback(Select.dir[i],XmNactivateCallback,(XtCallbackProc) XSelectMenuListeItem,(XtPointer)(long)i);
        XmStringFree ( tmp ); 
        }
     mx_dir = nb_dir;
     }

/**
 **  ecrit dans le menu les directory accessibles
 **/

   for (i=nb_dir-1; i >= 0; i-- )
      {
      tmp = XmStringCreate(dir_nom[i], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[0], XmNlabelString , tmp );
      XtSetValues( Select.dir[i], args , 1);
      XmStringFree ( tmp ); 
      }

   XtManageChildren ( Select.dir, nb_dir );

   XSelectUpdatePath ( entree );

   XSelectUpdateListe ( MODIF );

   XSelectUpdateDirectoryCourant ( dir_nom[nb_dir-1] );

}

/******************************************************************************

NOM:                    XSelectUpdatePath()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Met a jour le path dans le widget text

APPELE PAR:             XSelectChangerRepertoire

ARGUMENTS :             path - path ou est rendu le selecteur

GLOBALES AFFECTES:      Select.current_path

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectUpdatePath ( char *path )
{
 Arg  arg[1];

 XmTextSetString ( Select.current_path, path );
 XtSetArg ( arg[0], XmNcursorPosition, strlen(path) );
 XtSetValues ( Select.current_path, arg, 1 );
}

/******************************************************************************

NOM:                    XSelectUpdateDirectoryCourant()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Met a jour le nom du directory courant dans le pulldown

APPELE PAR:             XSelectChangerRepertoire

ARGUMENTS :             current - directory courant

GLOBALES AFFECTES:      Select.cascade 

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectUpdateDirectoryCourant ( char *current )
{
 Arg  arg[1];
 XmString str;

 str = XmStringCreateLtoR ( current , XmSTRING_DEFAULT_CHARSET ); 
 XtSetArg ( arg[0], XmNlabelString, str );
 XtSetValues ( Select.cascade, arg, 1 );
 XmStringFree ( str );
}

/******************************************************************************

NOM:                    XSelectMenuListeItem()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Reconstitue le directory courant ainsi que la liste
                        des fichiers, suite a une action sur un directory
                        de la liste.

APPELE PAR:             Callback de la liste des directorys

ARGUMENTS :             w        widget ou a eu lieu l'action
                        client
                        data

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectMenuListeItem ( Widget w, caddr_t client, caddr_t data )
{
 char nouveau_dir[256];
 int i;

 strcpy ( nouveau_dir, "/" );

 for ( i = 1; i < (long)client + 1; i ++ )
    {
     strcat ( nouveau_dir , dir_nom[i] );
     strcat ( nouveau_dir, "/" );
    }

 if ( strlen(nouveau_dir) > 1 )
     nouveau_dir[strlen(nouveau_dir) - 1] = '\0';

 XSelectChangerRepertoire ( nouveau_dir );
}

/******************************************************************************

NOM:                    XSelectItemListeFichier()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Appele lorsqu'un fichier ou un directory est selectionne
                        dans le directory courant ( callback de la liste )

APPELE PAR:             Callback de Select.liste

ARGUMENTS :             w        widget ou a eu lieu l'action
                        client
                        data

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectItemListeFichier ( Widget w, caddr_t client, caddr_t data )
{
 char *last_dir;
 char nouveau_dir[256], nom[128];
 char *nom1 = NULL ;
 XmListCallbackStruct *item = (XmListCallbackStruct *) data;

 /**
  **  conversion de XmString a string
  **/
     XmStringGetLtoR(item->item, XmSTRING_DEFAULT_CHARSET, &nom1);
       strcpy ( nom, nom1 );
     XtFree(nom1); /* VANH 15/02/95 */

     if (strrchr(nom, '/') != NULL)   /*  l'usager a choisi un directory  */
        {
 /**
  **  ajouter le dir choisi au bout du dir courant.
  **/
         XmListDeselectAllItems ( Select.liste );
         nom1 = XmTextGetString ( Select.current_path);
         strcpy(nouveau_dir, nom1 );
         XtFree( nom1 );
         nom1 = NULL;
         last_dir = strrchr ( nouveau_dir, '/' );
         if ( nom[0] == '.' )
         {
                  if ( last_dir != NULL )
                  {
                        if ( last_dir != nouveau_dir )
                              last_dir[0] = '\0' ;
                        else
                        last_dir[1] = '\0';
            }
             }
         else
            {
             if (strlen(nouveau_dir) != 1 )
                 strcat(nouveau_dir, "/");
             strcat(nouveau_dir, nom);
             nouveau_dir[strlen(nouveau_dir)-1] = '\0';
            }

         XSelectChangerRepertoire ( nouveau_dir );
        }
}

/******************************************************************************

NOM:                    XSelectDeselectItem()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Deselectionne les items choisis dans la liste

APPELE PAR:             Callback du bouton ANNULER

ARGUMENTS :             w        widget ou a eu lieu l'action
                        client
                        data

GLOBALES AFFECTES:      NONE 

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void XSelectDeselectItem (Widget w, caddr_t client, caddr_t data)
{
  XmListDeselectAllItems ( (Widget) w );
}

/******************************************************************************

NOM:                    XSelectAddToList()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Ajouter les fichiers selectionnes a la liste

APPELE PAR:             Callback du bouton AJOUTER A LA LISTE

ARGUMENTS :             w        widget ou a eu lieu l'action
                        client
                        data

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectAddToList ( Widget w, caddr_t client, caddr_t data )
{
   int i, nombre;
   Arg args[1];
   char current_dir[256], temp[256], *text = NULL;
   XmString *liste, tmp;
/*
 * Va chercher le nombre de fichiers selectionnes
 */

   i = 0;
   XtSetArg ( args[i], XmNselectedItemCount, &nombre );i++;
   XtGetValues ( Select.liste, args, i );

   if ( nombre == 0 ) return;

/*
 * Va chercher le nom du directoire courant
 */
   text = (char *) XmTextGetString(Select.current_path) ;
   strcpy(current_dir, text );
   XtFree( text );
   text = NULL;
   if (strlen(current_dir) > 1 )
      strcat ( current_dir, "/" );

/*
 * Va chercher la liste des fichiers selectionnes
 */

   i = 0;
   XtSetArg ( args[i], XmNselectedItems, &liste );i++;
   XtGetValues ( Select.liste, args, i );

  /*
   * ajouter les fichiers selectionnes a la liste
   */

   for ( i = 0; i < nombre ; i ++ )
      {
       XmStringGetLtoR ( liste[i], XmSTRING_DEFAULT_CHARSET, &text );
       strcpy ( temp, current_dir );
       strcat ( temp, text );
       tmp = XmStringCreateLtoR ( temp, XmSTRING_DEFAULT_CHARSET );
       if ( !XmListItemExists ( Edit.liste, tmp ) )
            XmListAddItem ( Edit.liste, tmp, 0 );
       XmStringFree ( tmp );
       XtFree( text );
     }

   XmListDeselectAllItems ( Select.liste );
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
static Widget TrouverWidgetParent(Window eventWindow)
{
   Widget  widgetCourant; /* Le widget ou l'evenement X s'est produit.   */
   Widget  widgetParent;  /* L'ancetre du widget ou evenement X s'est produit.*/

   widgetCourant = XtWindowToWidget (
                   wglDisp, eventWindow);

   if( widgetCourant == NULL ) return(NULL);

   widgetParent = XtParent(widgetCourant);
   while (widgetParent != NULL)
      {
      widgetCourant = widgetParent;
      widgetParent = XtParent(widgetCourant);
      };

   return widgetCourant;
}

/******************************************************************************

NOM:                    XSelectGetFichiers()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Retrouve les fichiers qui sont dans la liste
                        selectionnee

APPELE PAR:             Programme principal

ARGUMENTS :             NONE

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
static void XSelectGetFichiers (char *liste, int   maximum, int   len, int  *nombre)
{
 int i, lu;
 Arg args[1];
 char *text;
 XmString *xmliste;


  Select.etat = SELECT_EN_COURS;
  *nombre = 0;

/*
 * Trouve le nombre de fichiers
 */

   i = 0;
   XtSetArg ( args[i], XmNitemCount, &lu );i++;
   XtGetValues ( Edit.liste, args, i );

   if ( lu == 0 ) return;

   if ( lu > maximum )
     {
      XtManageChild ( Select.message );
      while ( Select.etat == SELECT_EN_COURS )
           {
            XtAppNextEvent ( SuperWidget.contexte, &Select.theEvent );
            XtDispatchEvent(&Select.theEvent);
           }
      XFlush(XtDisplay(Select.message));
     }

/*
 * Trouve la liste selectionnee ainsi que la longueur de cette derniere
 */

   i = 0;
   XtSetArg ( args[i], XmNitems, &xmliste );i++;
   XtGetValues ( Edit.liste, args, i );

   /*
    * transfert de la liste
    */

   *nombre = lu > maximum ? maximum : lu ;

   for ( i = 0; i <  *nombre  ; i ++ )
      {
       XmStringGetLtoR (xmliste[i], XmSTRING_DEFAULT_CHARSET, &text );
       if  (strlen(text) > len)
            printf("nom du dir est trop long\n");
       strcpy ( &liste[i*len], text );
       XtFree( text );
      }

    XEditEnleveTout ( NULL, NULL, NULL );
}

/******************************************************************************

NOM:                    c_selfic()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Sous-routine principale du selecteur de fichiers
                        qui permet de retourner une liste de fichiers

APPELE PAR:             EXTERIEUR

ARGUMENTS :             liste   - liste des fichiers selectionnes
                        maximum - nombre maximum de fichiers a selectionnes
                        nombre  - nombre de fichiers selectionnes

GLOBALES AFFECTES:      NONE 

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif

------------------------------------------------------------------------------*/
void c_selfic (char *liste, int maximum, int longueur, int *nombre)
{
   char *lng = (char *)getenv("CMCLNG");
   register int i;

   if ( lng != NULL )
      if ( tolower(lng[0]) == 'e' || tolower(lng[0])== 'a')
          langue = 1;
   if ( select_done !=  SELECT_DONE )
      {
       (void) Xinit( "select" );
       XSelectDirectory();
       XSelectOuvrir();
       XEditOuvrir();
       XRepertoireOuvrir();
       select_done = SELECT_DONE;
      }  
   
   XSelectActiver();
   XSelectGetFichiers (liste, maximum, longueur, nombre );
}

/******************************************************************************

NOM:                    selfic()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Sous-routine principale du selecteur de fichiers
                        qui permet de retourner une liste de fichiers

APPELE PAR:             EXTERIEUR

ARGUMENTS :             liste   - liste des fichiers selectionnes
                        maximum - nombre maximum de fichiers a selectionnes
                        nombre  - nombre de fichiers selectionnes

GLOBALES AFFECTES:      NONE

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif
------------------------------------------------------------------------------*/
void f77name(selfic) (char *liste, int  *maximum, int  *longueur, int  *nombre, F2Cl  len)
{
  c_selfic ( liste, *maximum, *longueur, nombre);
}

/******************************************************************************

NOM:                    XSelectOk()

AUTEUR :                Guy Asselin     CMCOG 421-4615

REVISION:               NONE

LANGAGE:                C

FONCTION:               Redonne le controle au programme principal

APPELE PAR:             Callback du bouton OK pour les messages

ARGUMENTS :             w      - dummy
                        client - dummy
                        data   - dummy

GLOBALES AFFECTES:      Select.etat 

VALEURE RETOURNEE:      NONE

LIBRAIRIES :            Motif
------------------------------------------------------------------------------*/
static void XSelectOk ( Widget w, caddr_t client, caddr_t data )
{
   Select.etat = SELECT_FINIE;
}

/*
main(argc, argv)
int argc;
char **argv;
{
   char fichiers[10][256];
   int  nombre, i, timex;

   c_selfic ( fichiers, 10, 256, &nombre, NULL );
   printf ("dans main\n");
   for ( i = 0; i < nombre; i ++)
      printf ("%s\n", fichiers[i] );

   c_selfic ( fichiers, 10, 256, &nombre, NULL );
   printf ("dans main\n");
   for ( i = 0; i < nombre; i ++)
      printf ("%s\n", fichiers[i] );

}
*/
