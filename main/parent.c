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
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <xinit.h>

extern SuperWidgetStruct SuperWidget;

Widget TrouverWidgetParent(eventWindow)
Window  eventWindow;
{
   Display *display;
   Widget   widgetCourant, widgetParent;
   
   display = XtDisplay(SuperWidget.topLevel);
   widgetCourant = XtWindowToWidget(display, eventWindow);

   if (widgetCourant == NULL)
      return NULL;
   
   widgetParent = XtParent(widgetCourant);
   while (widgetParent != NULL)
      {
      widgetCourant = widgetParent;
      widgetParent = XtParent(widgetCourant);
      };
   
   return widgetCourant;
   }
