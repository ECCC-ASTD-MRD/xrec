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
#include <rpnmacros.h>

#include <wgl_x.h>


extern SuperWidgetStruct SuperWidget;

extern XFontStruct *fonte;
extern Display *wglDisp;
extern Window  wglDrawable;
extern GC      wglLineGC;
extern wgl_point  wglPts[];

extern int currentFontSize;

/**
 ******
 **/

int x_wglfsz(int fontSize)
{
   if (fontSize != currentFontSize) 
      InitFonte(fontSize);
   return fontSize;
   
   }


int InitFonte(int fontSize)
{   
   char fontName[132];

   if (fonte != NULL)
      XFreeFont(wglDisp, fonte);
   
   switch(fontSize)
      {
      case 10:
      fonte = XLoadQueryFont(wglDisp, "times-roman.10");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--10-100-75-75-m-60-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--10-100-75-75-p-54-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 12:
      fonte = XLoadQueryFont(wglDisp, "times-roman.12");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--12-120-75-75-m-70-iso8859-1");
	 
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--12-120-75-75-p-64-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 14:
      fonte = XLoadQueryFont(wglDisp, "times-roman.14");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--14-100-100-100-m-90-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--14-140-75-75-p-74-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 17:
      fonte = XLoadQueryFont(wglDisp, "times-roman.17");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--17-120-100-100-m-100-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--17-120-100-100-p-84-iso8859-1");
	 
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 18:
      fonte = XLoadQueryFont(wglDisp, "times-roman.18");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--18-180-75-75-m-110-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--18-180-75-75-p-94-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 24:
      fonte = XLoadQueryFont(wglDisp, "times-roman.24");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--24-240-75-75-m-150-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--24-240-75-75-p-124-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      default:
      fonte = XLoadQueryFont(wglDisp, "fixed");
      break;
      }
   
   
   currentFontSize = fontSize;
   XSetFont(wglDisp, wglLineGC, fonte->fid); 
   
   }

