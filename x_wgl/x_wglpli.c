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

x_wglpli(int nbPoints, int polygone[][2])
{
   int i;
   XPoint p[256], *xptr, *largeP;
   
   if (nbPoints > 256)
      {
      largeP = (XPoint *) calloc(nbPoints, sizeof(XPoint));
      xptr = largeP;
      }
   else
      {
      xptr = p;
      }

   c_wglfshlb();
   for (i=0; i < nbPoints; i++)
      {
      xptr[i].x = polygone[i][X];
      xptr[i].y = h - polygone[i][Y];
      }

   XDrawLines(wglDisp, wglDrawable, wglLineGC, xptr, nbPoints, CoordModeOrigin);
   if (nbPoints > 256)
      free(largeP);

   }

