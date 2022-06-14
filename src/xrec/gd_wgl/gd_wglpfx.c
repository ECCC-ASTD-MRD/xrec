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

#include <gd_wgl.h>

gd_wglpfx(nbPoints, polygone)
int nbPoints;
float polygone[][2];
{
   int i;
   int i1, j1;
   wgl_point p[255];
   
   wglfshlb();
   for (i=0; i < nbPoints; i++)
      {
      gd_wglxai(&i1, &j1, polygone[i][X], polygone[i][Y]);
      p[i].x = i1;
      p[i].y = h - j1; 
      }

   /*    XFillPolygon(wglDisp, wglDrawable, wglFillGC, p,nbPoints, Convex, CoordModeOrigin); */
   }
