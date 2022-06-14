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
/* extern wgl_point  wglPts[]; */
extern wgl_point  wglPts[];

extern int currentFontSize;


int  x_wglhsi(char *string, int stringLength)
{
   int ascent, descent;
   int direction;
   XCharStruct fonteInfo;

   XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo);
   return (ascent + descent);

   }

