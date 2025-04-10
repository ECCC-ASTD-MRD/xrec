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

/**
 ** Fichier xmenu.c
 **
 ** Auteur: Yves Chartier (RPN)
 **
 ** Creation: Avril 1989
 **
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <Xm/Form.h>
#include <Xm/Xm.h>
//#include <Xm/LabelG.h>
#include <Xm/Label.h>
//#include <Xm/PushBG.h>
#include <Xm/PushB.h>
//#include <Xm/CascadeBG.h>
#include <Xm/CascadeB.h>
//#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/MenuShell.h>
#include <Xm/RowColumn.h>

#include <xinit.h>
#include <wgl.h>

#define SELECTION_EN_COURS 0
#define SELECTION_TERMINEE 1

/** 
 ** Fonctions de type "callback" utilisees par Xt
 **/


int StatutSelection, itemSelectionne;
extern SuperWidgetStruct SuperWidget;

void SelectItem(Widget w, XtPointer client_data, XtPointer call_data);
static XtErrorHandler  HandlerBidon();

static Widget  menuTitre[2] = {NULL,NULL};
static Widget  menuInfo  = NULL;
static Widget *menuItems = NULL;
static Widget topLevel = NULL;
static Widget forme = NULL;

static char *menuTitreStr = NULL;
static char **menuItemsStr = NULL;
static char *menuInfoStr = NULL;
static int   nbMenuItems = 0;

void NettoyerVieuxStrings(void);
void InitialiserMenusStr(char *menuStr);

/**
 ** Declaration des items contenus dans les menus
 **/

/************** Event Handler for Popup Menu ************************/

/**
PostIt (w, popup, event)
Widget w;
Widget popup;
XButtonEvent * event;
{
   if (event->button != Button3)
        return;
   XmMenuPosition(popup, event);
   XtManageChild(popup);
   }
**/

int defpup(menuStr)
char *menuStr;
{
   int i,n;
   int argc = 1;
   static char *argv[] = { "xmetaview", };
   static String RessourcesDeDefaut[] = {  NULL, };
   Arg args[10];
   XmString string;

   NettoyerVieuxStrings();
   InitialiserMenusStr(menuStr);
   
   return 1;
   }

int dopopup(indMenu, posX, posY)
int indMenu; 
int posX, posY;
{
   int i,n;
   XmString string;

   Arg args[10];
   XEvent theEvent;

   Position x, y;

   i = 0;
   XtSetArg(args[i], XtNoverrideRedirect, True); i++;  
   XtSetArg(args[i], XtNsaveUnder, True); i++;  
   XtSetArg(args[i], XmNheight, 1); i++;
   XtSetArg(args[i], XmNwidth,  1); i++;
   topLevel = XtAppCreateShell(NULL, "MenuSouris", 
                               applicationShellWidgetClass,
                               XtDisplay(SuperWidget.topLevel), args, i);
   
   XtSetWarningHandler((XtErrorHandler) HandlerBidon);

   forme = (Widget)XmCreatePopupMenu(topLevel, "forme", args, i);

   i = 0;
   string = XmStringCreate(menuTitreStr, XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNlabelString, string); i++;
   menuTitre[0] = (Widget)XmCreateLabel(forme, "Title", args, i);
   XmStringFree(string);

   menuTitre[1] = (Widget)XmCreateSeparator(forme, "separator", NULL, 0);
 
   menuItems = (Widget *) calloc(nbMenuItems, sizeof(Widget));
   for (n=0; n < nbMenuItems; n++)
      {
      i = 0;
      if (0 == strcmp(menuItemsStr[n],"separator"))
	 {
	 menuItems[n] = (Widget)XmCreateSeparator(forme, "separator", NULL, 0);
	 }
      else
	 {
	 string = XmStringCreate(menuItemsStr[n], XmSTRING_DEFAULT_CHARSET); 
	 XtSetArg(args[i], XmNlabelString, string); i++;
	 
	 menuItems[n] = (Widget)XmCreatePushButton(forme, "menuItem", args, i);
	 XtAddCallback(menuItems[n], XmNactivateCallback, SelectItem, NULL);
	 
	 XmStringFree(string);
	 }
      }

   i = 0;
   string = XmStringCreate(menuInfoStr, XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;

   menuInfo = (Widget)XmCreateLabel(forme, "Title", args, i);
   XmStringFree(string);

   i = 0;
   XtSetArg(args[i], XmNx, posX); i++;
   XtSetArg(args[i], XmNy, posY); i++;
   XtSetValues(forme, args, i);

   XtSetWarningHandler((XtErrorHandler)HandlerBidon);

   XtManageChildren(menuTitre, 2);
   XtManageChildren(menuItems, nbMenuItems);
   XtManageChild(menuInfo); 
   XtManageChild(forme); 

   XtRealizeWidget(topLevel);

   StatutSelection = SELECTION_EN_COURS;
   itemSelectionne = -100;
   while (StatutSelection == SELECTION_EN_COURS) /**  && c_wglbtp(BDROIT)) **/
      {
      XtAppNextEvent(SuperWidget.contexte, &theEvent);
      XtDispatchEvent(&theEvent);
      if (theEvent.type == ButtonRelease)
	 {
	 StatutSelection = SELECTION_TERMINEE;
	 while (0 != XtAppPending(SuperWidget.contexte))
	    {
	    XtAppNextEvent(SuperWidget.contexte, &theEvent);
	    XtDispatchEvent(&theEvent);
	    }
	 }

      }
      XtDestroyWidget(topLevel);
      XFlush(XtDisplay(SuperWidget.topLevel));
   return itemSelectionne+1;
   }


/**
******************************************************************************
******************************************************************************
**/

void SelectItem(Widget w, XtPointer client_data, XtPointer call_data)
{
   int i;

   i = 0;
   while (i < nbMenuItems && menuItems[i] != w)
      i++;

   itemSelectionne = i;
   StatutSelection = SELECTION_TERMINEE;
   }

static XtErrorHandler HandlerBidon(avertissement)
String avertissement;
{
   printf("Voici le message recu: %s\n\n\n", avertissement); 
   }

void NettoyerVieuxStrings()
   {
   int i;
   if (menuTitreStr != NULL)
      {
      free(menuTitreStr);
      menuTitreStr = NULL;
      }
   
   if (menuInfoStr != NULL)
      {
      free(menuInfoStr);
      menuInfoStr = NULL;
      }
   
   if (menuItemsStr != NULL)
      {
      for (i=0;i< nbMenuItems;i++)
	 {
	 free(menuItemsStr[i]);
	 menuItemsStr[i] = NULL;
	 }
      menuItemsStr = NULL;
      nbMenuItems = 0;
      }
   }
   

void InitialiserMenusStr(menuStr)
char *menuStr;
{
   int i;
   char *posBarre, *posTitre, *posSep;
   int lenBarre,lenTitre,lenSep;

   char *copieMenuStr;
   
   copieMenuStr = (char *) calloc(strlen(menuStr)+1, sizeof(char));
   strcpy(copieMenuStr, menuStr);

   posBarre = strstr(copieMenuStr, "|");
   while(posBarre != NULL)
      {
      posBarre = strstr(copieMenuStr,"|");
      posTitre = strstr(copieMenuStr,"%t|");
      posSep   = strstr(copieMenuStr,"%l|");
      
      lenBarre = posBarre - copieMenuStr;
      lenTitre = posTitre - copieMenuStr;
      lenSep   = posSep   - copieMenuStr;

      if (posTitre != NULL && posTitre < posBarre)
	 {
	 if (menuTitreStr != NULL)
	    free(menuTitreStr);
	 menuTitreStr = (char *) calloc(lenTitre, sizeof(char));
	 strncpy(menuTitreStr, copieMenuStr, lenTitre-1);
	 strcpy(copieMenuStr, copieMenuStr+ lenBarre+1);
	 posBarre = NULL;
	 posSep = NULL;
	 }
      
      if (posBarre != NULL)
	 {
	 if (menuItemsStr == NULL)
	    {
	    nbMenuItems = 1;
	    menuItemsStr = (char **) calloc(nbMenuItems, sizeof(char *));
	    }
	 else
	    {
	    nbMenuItems++;
	    menuItemsStr = (char **) realloc(menuItemsStr, nbMenuItems * sizeof(char *));
	    }
	 
	 i = nbMenuItems - 1;
	 menuItemsStr[i] = (char *) calloc(lenBarre, sizeof(char));
	 strncpy(menuItemsStr[i], copieMenuStr, lenBarre - 1);
	 strcpy(copieMenuStr, copieMenuStr+lenBarre+1);
	 }

      if (posSep != NULL && posSep < posBarre)
	 {
	 nbMenuItems++;
	 menuItemsStr = (char **) realloc(menuItemsStr, nbMenuItems * sizeof(char *));

	 i = nbMenuItems - 1;
	 menuItemsStr[i] = (char *) calloc(strlen("separator")+1, sizeof(char));
	 menuItemsStr[i-1][lenSep] = '\000';
	 strcpy(menuItemsStr[i], "separator");
	 }
      posBarre = strstr(copieMenuStr,"|");
      }

   if (menuInfoStr != NULL)
      free(menuInfoStr);
   menuInfoStr = (char *) calloc(strlen(copieMenuStr)+1, sizeof(char));
   strcpy(menuInfoStr, copieMenuStr);
   }
