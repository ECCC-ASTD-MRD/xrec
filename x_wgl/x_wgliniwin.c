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

#include <wgl_x.h>

x_wgliniwin(char *nomFenetre)
{
   char *serverID;

   int i;
   static char *geo = "-geometry";
   char geometrie[24];
   char *argv[3];
   int argc;

   x_wglinids(nomFenetre);

   serverID = (char *)(ServerVendor(wglDisp));
   wglfore = WhitePixel(wglDisp, wglScrNum);
   wglback = BlackPixel(wglDisp, wglScrNum);
   
   if (!customWinSize)
      {
      wglHints.x     = 0;
      wglHints.y     = 0;
      wglHints.width = 150;
      wglHints.height = 30;

      if (aspectRatioSet)
	 wglHints.flags = PAspect;
      else
	 wglHints.flags  = 0;
      }
   else
      {
      sprintf(geometrie, "%dx%d+%d+%d", wglHints.width, wglHints.height, wglHints.x, wglHints.y);
      argc = 3;
      argv[0] = nomFenetre;
      argv[1] = geo;
      argv[2] = geometrie;
      }
   
   attribmask = CWColormap | CWBackPixel;
   wglWinSetAttr.colormap = DefaultColormap(wglDisp, wglScrNum);
   wglWinSetAttr.background_pixel = BlackPixel(wglDisp, wglScrNum);
   
   x_wglinicmap(); 
   wglWin = XCreateWindow(wglDisp,  DefaultRootWindow(wglDisp), wglHints.x, wglHints.y, 
			  wglHints.width, wglHints.height, 5, visInfo.depth,InputOutput,visInfo.visual,
			  attribmask,  &wglWinSetAttr);
   if (customWinSize) 
      {
      XSetStandardProperties(wglDisp, wglWin, nomFenetre, nomFenetre, NULL, argv, argc, &wglHints);
      }

   if (1 != c_wglgpl())
      {
      if (nbFenetresActives <= 1)
	 x_wglinicmap(); 
      else
	 XSetWindowColormap(wglDisp, wglWin, cmap);
      }
   
   wglDrawable = wglWin;

   wglLineGC = XCreateGC(wglDisp, wglWin, 0, 0);
   wglFillGC = XCreateGC(wglDisp, wglWin, 0, 0);
   bgPix     = NULL;

   XSetBackground(wglDisp, wglLineGC, wglback);
   XSetForeground(wglDisp, wglLineGC, wglfore);
   
   XSetBackground(wglDisp, wglFillGC, wglback);
   XSetForeground(wglDisp, wglFillGC, wglfore);
   
   x_wglinipat();
   if (cmap_strategy == READ_WRITE_COLORMAP)
     {
       c_wglsetwcmap();
     }
   
   XStoreName(wglDisp, wglWin, nomFenetre); 
   XSelectInput(wglDisp, wglWin, EVENT_MASK);
   
   croix = XCreateFontCursor(wglDisp,XC_cross);
   
   if (aspectRatio != 0.0)
     {
       XSetNormalHints(wglDisp, wglWin, &wglHints);
     }
   
   InitFonte(14); 
   
   FlusherTousLesEvenements();
   XMapRaised(wglDisp, wglWin);
   XFlush(wglDisp);
   
   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);
   while (wglWinAttr.map_state != IsViewable)
      {
      XtAppNextEvent(SuperWidget.contexte, &wglEvent);
      XtDispatchEvent(&wglEvent);
      XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);
      }
   
   }
