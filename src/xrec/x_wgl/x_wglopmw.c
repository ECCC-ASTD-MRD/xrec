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

#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <rmn/rpnmacros.h>
#include <xinit.h>
#include <wgl.h>
#include <stdlib.h>
#include "x_wglfonct.h"

/**
** Declaration des constantes globales
**/

extern SuperWidgetStruct SuperWidget; 

/**
** Variables de base utilisees par X
**/

extern Display *wglDisp;
extern int wglScr;
extern Window wglWin, wglDrawable;
extern GC wglLineGC, wglFillGC;
extern int cmap;
extern int nbFenetresActives;

/**
 ** Attributs de la fenetre
 **/

extern XWindowAttributes wglWinAttr;
extern XSetWindowAttributes wglWinSetAttr;
extern XSizeHints wglHints;
extern float  aspectRatio;
extern int  wglForceAspect;

extern int debug;
extern int customWinSize;
extern int customPlaneMask;
extern int doubleBufferMode;
extern XEvent wglEvent;

#define SGI 13

extern int serverIdentifier;

extern unsigned long wglfore, wglback;

#define EVENT_MASK      (ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask | ColormapChangeMask )

/**
 ******************************************************************************
 ******************************************************************************
 **/

int x_wglopmw(char *nomFenetre, int wglWinID)
   {
   char tempNomFenetre[256];
   /**
      static char       *messageAgrandir[] = {"Agrandissez-moi", "Enlarge me"};
      
      CreerNouvelleFenetre();
      
      if (nomFenetre[0] == '?')
      sprintf(tempNomFenetre, "X-%s", &nomFenetre[1]);
      else
      strcpy(tempNomFenetre, nomFenetre);
      
      InitWglMotifWindow(tempNomFenetre, wglWinID);
      XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);
      
      c_wglssp(0.0, 0.0, (float) (wglWinAttr.width -1), (float) (wglWinAttr.height - 1), 0, 0, 
      wglWinAttr.width - 1, wglWinAttr.height - 1);
      
      
      if (!customWinSize)
      {
      InitMessageAgrandir(messageAgrandir[c_getulng()]);
      WaitforWindowResize();
      }
**/
   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);

   x_wglcol(NOIR);
   x_wglclr(); 

   x_wglsetw(wglWin);
   /*    RestorerParametresDeDefaut(); */
   return wglWin;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

/* 
   InitWglMotifWindow(nomFenetre, wglWinID)
   char *nomFenetre;
   int wglWinID;
   {
   int height, width;
   char *serverID;
   
   static char *geo = "-geometry";
   char geometrie[24];
   char *argv[3];
   int argc;
   
   InitWglDisplayAndScreen(nomFenetre);
   
   serverID = (char *)(ServerVendor(wglDisp));
   if (0 == strcmp(serverID, "Silicon Graphics Inc."))
   serverIdentifier = SGI;
   else
   serverIdentifier = 0;
   
   
   wglfore = WhitePixel(wglDisp, wglScr);
   wglback = BlackPixel(wglDisp, wglScr);
   
   if (!customWinSize)
   {
   wglHints.x     = 0;
   wglHints.y     = 0;
   wglHints.width = 150;
   wglHints.height = 30;
   }
   else
   {
   sprintf(geometrie, "%dx%d+%d+%d", wglHints.width, wglHints.height, wglHints.x, wglHints.y);
   argc = 3;
   argv[0] = nomFenetre;
   argv[1] = geo;
   argv[2] = geometrie;
   }
   
   wglWin = XtWindow((Widget)wglWinID);
   
   if (customWinSize)
   {
   XSetStandardProperties(wglDisp, wglWin, nomFenetre, nomFenetre, NULL, argv, argc, &wglHints);
   }
   
   wglDrawable = wglWin;
   
   wglLineGC = XCreateGC(wglDisp, wglWin, 0, 0);
   wglFillGC = XCreateGC(wglDisp, wglWin, 0, 0);
   
   XSetBackground(wglDisp, wglLineGC, wglback);
   XSetForeground(wglDisp, wglLineGC, wglfore);
   
   XSetBackground(wglDisp, wglFillGC, wglback);
   XSetForeground(wglDisp, wglFillGC, wglfore);
   
   if (1 != wglgpl())
   {
   if (nbFenetresActives == 1)
   InitColormap();
   else
   XSetWindowColormap(wglDisp, wglWin, cmap);
   }
   
   XSync(wglDisp, False);
   InitPatterns();
   
   XSync(wglDisp, False);
   XSetBackground(wglDisp, wglLineGC, wglback);
   XSetForeground(wglDisp, wglLineGC, wglfore);
   
   XSync(wglDisp, False);
   XSetBackground(wglDisp, wglFillGC, wglback);
   XSetForeground(wglDisp, wglFillGC, wglfore);
   
   XSync(wglDisp, False);
   XStoreName(wglDisp, wglWin, nomFenetre);
   
   XSync(wglDisp, False);
   XSelectInput(wglDisp, wglWin, EVENT_MASK);
   
   InitFonte(14);
   
   if (aspectRatio != 0.0)
   {
   XSetNormalHints(wglDisp, wglWin, &wglHints);
   }
   
   
   XMapRaised(wglDisp, wglWin);
   XtAppNextEvent(SuperWidget.contexte, &wglEvent);
   }
   
*/
/**
******************************************************************************
******************************************************************************
**/
