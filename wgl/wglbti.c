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

int f77name(wglbti)(i,j)
int *i, *j;
{
   return (c_wglbti(i, j));
   }

/**
 ******
 **/

int c_wglbti(i, j)
int *i, *j;
{
   wglfshlb();
   XDefineCursor(wglDisp,wglWin,croix);
   while (True)
      {
      XtAppNextEvent(SuperWidget.contexte, &wglEvent);
      switch(wglEvent.type) 
         {
         case ButtonPress:
         if (wglEvent.xbutton.window == wglWin)
            {
	    XDefineCursor(wglDisp,wglWin,None);
            *i = wglEvent.xbutton.x;
            *j = h - wglEvent.xbutton.y;
            
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
	    XBell(wglDisp, 0);
	    XDefineCursor(wglDisp,wglWin,None);
	    return NULL;
	    }
	 
	 default:
	 XtDispatchEvent(&wglEvent);
	 break;
	 }
      }
   }

