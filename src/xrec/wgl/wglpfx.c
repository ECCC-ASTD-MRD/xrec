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

#include <wgl.h>

int f77name(wglpfx)(int *nbPoints, float polygone[][2])
{
   return c_wglpfx(*nbPoints, polygone);
   }

/**
 ******
 **/


int c_wglpfx(int nbPoints, float polygone[][2])
{
  int *ipoly;
  int i, i1, j1;
  
  ipoly = (int *) malloc(2*sizeof(int)*nbPoints);
  
  c_wglfshlb();
  for (i=0; i < nbPoints; i++)
    {
      c_wglxai(&(ipoly[2*i]), &(ipoly[2*i+1]), polygone[i][X], polygone[i][Y]);
    }
  
  c_wglpfi(nbPoints, (int (*)[2]) ipoly);
  
  free(ipoly);

  return 0;
  
}
