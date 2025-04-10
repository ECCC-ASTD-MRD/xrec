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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xinit.h>
#include "rec_functions.h"
#include "wgl.h"

extern SuperWidgetStruct SuperWidget;

void SetGeometrieFenetreAffichage(char *nomFenetre)
{
   int x, y, width, height;
   int xoffset, yoffset;
   char *geometrie;
   int  geoParams;
   
   Xinit(nomFenetre);
   
   geometrie = (char *) XGetDefault(XtDisplay(SuperWidget.topLevel), nomFenetre, "geometry");
   if (!geometrie)
      return;

   geoParams = XParseGeometry(geometrie, &x, &y, &width, &height);
   
   if ((geoParams & WidthValue) && (geoParams & HeightValue))
      {
      if ((geoParams & XValue) || (geoParams & YValue))
	 {
	 if (x == 0)
	    {
	    if (geoParams & XNegative)
	       {
	       x = DisplayWidth(XtDisplay(SuperWidget.topLevel), DefaultScreen(XtDisplay(SuperWidget.topLevel))) - width;
	       xoffset = -5;
	       }
	    else
	       {
	       xoffset = 5;
	       }
	    }
	 
	 if (x < 0)
	    {
	    x = DisplayWidth(XtDisplay(SuperWidget.topLevel), DefaultScreen(XtDisplay(SuperWidget.topLevel))) + x - width;
	    xoffset = -5;
	    }
	 
	 if (y > 0 || !(geoParams & YNegative))
	    {
	    y = DisplayHeight(XtDisplay(SuperWidget.topLevel), DefaultScreen(XtDisplay(SuperWidget.topLevel))) - height - y;
	    yoffset = -5;
	    }
	 
	 if (y < 0 || (geoParams & YNegative))
	    {
	    y = -y;
	    yoffset = 5;
	    }
	 
#ifdef X_WGL
	 c_wglppo(x, y, width, height);
#endif
#ifdef GL_WGL
	 c_wglppo(x+xoffset, y+yoffset, width, height);
#endif
	 }
      else
	 {
	 c_wglpsz(width, height);
	 }
      }
   
   }
