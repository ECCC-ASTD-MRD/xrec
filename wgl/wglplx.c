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

f77name(wglplx)(nbPoints, polygone)
int *nbPoints;
float polygone[][2];
{
   c_wglplx(*nbPoints, polygone);
   }

/**
 ******
 **/


c_wglplx(nbPoints, polygone)
int nbPoints;
float polygone[][2];
{
   int i,j,k;
   int i1, j1, imax;
   XPoint p[64];
   
   wglfshlb();
   for (j=0; j < nbPoints; j+=63)
      {
      imax = (nbPoints - j) >= 64 ? 64 : nbPoints - j;
      for (i=j; i < (j+imax); i++)
	 {
	 c_wglxai(&i1, &j1, polygone[i][X], polygone[i][Y]);
	 p[i-j].x = i1;
	 p[i-j].y = h - j1; 
	 }
      
      if ((currentDashPattern == 0) || (serverIdentifier != SGI))
	 {
	 XDrawLines(wglDisp, wglDrawable, wglLineGC, p, i-j, CoordModeOrigin);
	 }
      else
	 {
	 for (k=0; k < (i-j-1); k++)
	    XDrawLine(wglDisp, wglDrawable, wglLineGC, p[k].x, p[k].y, p[k+1].x, p[k+1].y);
	 }
      }
   
   }

