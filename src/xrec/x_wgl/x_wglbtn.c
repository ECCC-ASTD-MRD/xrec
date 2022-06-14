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

int x_wglbtn(int bouton)
{
   wglfshlb();

   XDefineCursor(wglDisp,wglWin,croix);
   while (True)
      {
      XtAppNextEvent(SuperWidget.contexte, &wglEvent);
      switch(wglEvent.type) 
         {
         case ButtonPress:
         if (wglEvent.xbutton.window != wglWin)
	    {
	    XDefineCursor(wglDisp,wglWin,None);
	    XBell(wglDisp, 0);
	    }
	 else
	    {
	    XDefineCursor(wglDisp,wglWin,None);
	    switch(bouton)
	       {
	       case BGAUCH:
	       switch(wglEvent.xbutton.button)
		  {
		  case Button1:
		  return True;
		  
		  default:
		  return False;
		  }
	       
	       case BMLIEU:
	       switch(wglEvent.xbutton.button)
		  {
		  case Button2:
		  return True;
		  
		  default:
		  return False;
		  }
	       
	       case BDROIT:
	       switch(wglEvent.xbutton.button)
		  {
		  case Button3:
		  return True;
		  
		  default:
		  return False;
		  }
	       
	       case BTOUS:
	       switch(wglEvent.xbutton.button)
		  {
		  case Button1:
		  case Button2:
		  case Button3:
		  return True;
		  
		  default:
		  return False;
		  }
	       }
            }
         
         default:
         XtDispatchEvent(&wglEvent);
         }
      }
   
   XDefineCursor(wglDisp,wglWin,None);
   }

