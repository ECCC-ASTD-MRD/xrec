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

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <Xm/Xm.h>
#include <xinit.h>

extern SuperWidgetStruct SuperWidget;

void CheckColormap(Widget w)
{
   Colormap cmap;
   int i;
   Arg args[2];
   Window wglwin,widgetwin;
   Visual *wglvis,*widgetvis;
   XWindowAttributes attr;
   
   wglwin = c_wglgacw();
   widgetwin = XtWindow(w);

   return;
   if (widgetwin == 0)
      return;

#ifdef X_WGL
   XGetWindowAttributes(XtDisplay(SuperWidget.topLevel), wglwin, &attr);
   wglvis = attr.visual;

   XGetWindowAttributes(XtDisplay(SuperWidget.topLevel), widgetwin, &attr);
   widgetvis = attr.visual;

   if (wglvis != widgetvis)
      return;

   c_wglgetcmap(&cmap);
   i = 0;
   if (cmap != -1)
      {
      XtSetArg(args[i], XmNcolormap, cmap); i++;
      }
   
   XtSetValues(w, args, i);
#endif

   }

