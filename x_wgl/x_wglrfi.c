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

f77name(wglrfi)(ia, ja, ib, jb)
int *ia, *ja, *ib, *jb;
{
   wglrfi(*ia, *ja, *ib, *jb);
   }

/**
******
**/

wglrfi(ia, ja, ib, jb)
{
   XPoint p[5];

   wglfshlb();
   p[0].x = ia;
   p[0].y = h - ja;
   p[1].x = ib;
   p[1].y = p[0].y; 
   p[2].x = p[1].x;
   p[2].y = h - jb;
   p[3].x = p[0].x; 
   p[3].y = p[2].y; 
   p[4].x = p[0].x;
   p[4].y = p[0].y;
   XFillPolygon(wglDisp, wglDrawable, wglFillGC, p, 5, Convex, CoordModeOrigin);
   }

