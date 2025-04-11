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

int x_wglptxs(float *xxx, float *yyy, int *npts)
{
  int i,j,k;
  int i1, j1, imax;
  wgl_point p[256];
  
  for (j=0; j < *npts; j+=255)
    {
    imax = (*npts - j) >= 256 ? 256 : *npts - j;
    for (i=j; i < (j+imax); i++)
      {
      p[i-j].x = (short)(ROUND(((xxx[i] - usSpace.xdebut) * usSpace.densiteX) + usSpace.idebut));
      p[i-j].y = (short)(h - ROUND(((yyy[i] - usSpace.ydebut) * usSpace.densiteY) + usSpace.jdebut));
      }
    
    XDrawPoints(wglDisp, wglDrawable, wglLineGC, (XPoint *) p, imax, CoordModeOrigin);
    
    }
}

