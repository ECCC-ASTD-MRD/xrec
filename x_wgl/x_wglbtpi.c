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

Boolean wglbtpi();

Boolean f77name(wglbtpi)(bouton, i, j)
int *bouton;
int *i, *j;
{
   return (Boolean) wglbtpi(*bouton, i, j);
   }

/**
 ******
 **/

Boolean wglbtpi(bouton, i, j)
int bouton;
int *i, *j;
{
   Window root, child;
   int root_x, root_y;
   int win_x, win_y;
   unsigned int mouseButtonMask;
   Bool res;
   
   XDefineCursor(wglDisp,wglWin,croix);
   wglfshlb();
   XSync(wglDisp, True);
   mouseButtonMask = 0;
   res = XQueryPointer(wglDisp, wglWin, &root, &child, &root_x, &root_y, &win_x, &win_y, &mouseButtonMask);
   *i = win_x;
   *j = h - win_y;  
   res = False;
   
   if (mouseButtonMask)
      {
      switch(bouton)
         {
         case 1:
         if (mouseButtonMask == Button1Mask)
            res= True;
         break;
         
         case 2:
         if (mouseButtonMask == Button2Mask)
            res= True;
         break;
         
         case 3:
         if (mouseButtonMask == Button3Mask)
            res= True;
         break;
         
         case 4:
         if (mouseButtonMask == (Button1Mask | Button2Mask | Button3Mask))
            res= True;
         }
      
      }
   
   XSync(wglDisp, True);
   if (!res)
      {
      XDefineCursor(wglDisp,wglWin,None);
      }

   return res;
   }
