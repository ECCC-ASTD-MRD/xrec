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
#include <gd.h>

extern gdImagePtr gdwin;

void gd_wglplx(nbPoints, polygone)
int nbPoints;
float polygone[][2];
{
   int i,j,k;
   int i1, j1, i2, j2;
   
   wglfshlb();
   c_wglxai(&i1, &j1, polygone[0][X], polygone[0][Y]);
   
   for (i=1; i < nbPoints; i++)
      {
      c_wglxai(&i2, &j2, polygone[i][X], polygone[i][Y]);
      gdImageLine(gdwin, i1, j1, i2, j2, currentColor);
      i1 = i2;
      j1 = j2;
      }
}

