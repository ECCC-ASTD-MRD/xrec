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

f77name(wglplx)(int *nbPoints, float polygone[][2])
{
   c_wglplx(*nbPoints, polygone);
   }

/**
 ******
 **/


c_wglplx(int nbPoints, float polygone[][2])
{
  int i;
  
  int p[512], *ipoly, *largeP;
  
  if (nbPoints == 0) return;
  if (nbPoints > 256)
    {
    largeP = (int *) malloc(2*sizeof(int)*nbPoints);
    ipoly = largeP;
    }
  else
    {
    ipoly = p;
    }
  
  c_wglfshlb();
  for (i=0; i < nbPoints; i++)
    {
    c_wglxai(&(ipoly[2*i]), &(ipoly[2*i+1]), polygone[i][X], polygone[i][Y]);
    }
    c_wglpli(nbPoints, ipoly);
  
  if (nbPoints > 256)
    free(largeP);
}

