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

#include <gmp.h>
#include <math.h>

gmp_trim(float pts[], int *nbpts, float resolution)
{
   int i,j, newnbpts;
   float lastLat, lastLon;
   float newpts[200];
   
   return 0;
   lastLat = pts[0];
   lastLon = pts[1];
   newpts[0] = pts[0];
   newpts[1] = pts[1];

   if (*nbpts <= 2)
      return;

   j = 2;
   for (i=2; i < *nbpts; i+=2)
      {
      if ((resolution <= fabs(pts[i] - lastLat)) || resolution <= fabs(pts[i+1] - lastLon))
        {
        newpts[j] = pts[i];
        newpts[j+1] = pts[i+1];
        lastLat = pts[i];
        lastLon = pts[i+1];
        j+=2;
        }
      }
   
   if (j != *nbpts)
      {
      newpts[j] = pts[*nbpts-2];
      newpts[j+1] = pts[*nbpts-1];
      j += 2;
      }

   newnbpts = j;
   for (j=0; j < newnbpts; j++)
      pts[j] = newpts[j];
   
   *nbpts = newnbpts;
   }
   
