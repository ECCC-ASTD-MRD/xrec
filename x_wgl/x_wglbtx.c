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

int f77name(wglbtx)(x, y)
float *x, *y;
{
   return (wglbtx(x, y));
   }

/**
 ******
 **/

int wglbtx(x, y)
float *x, *y;
{
   int i, j;
   
   wglfshlb();
   XDefineCursor(wglDisp,wglWin,croix);
   while (True)
      {
      XtAppPeekEvent(SuperWidget.contexte, &wglEvent);
      switch(wglEvent.type) 
         {
         case ButtonPress:
	 XDefineCursor(wglDisp,wglWin,None);
         if (wglEvent.xbutton.window == wglWin)
            {
	    XtAppNextEvent(SuperWidget.contexte, &wglEvent);
            i = wglEvent.xbutton.x;
            j = h - wglEvent.xbutton.y;
            wgliax(x, y, i, j);
            
            switch(wglEvent.xbutton.button)
               {
               case Button1:
               return BGAUCH;
               
               case Button2:
               return BMLIEU;
               
               case Button3:
               return BDROIT;
               }
	    }
	 else
	    {
	    XDefineCursor(wglDisp,wglWin,None);
	    return NIL;
	    }
            
	 default:
	 XtAppNextEvent(SuperWidget.contexte, &wglEvent);
	 XtDispatchEvent(&wglEvent);
	 break;
         }
      }
   }
