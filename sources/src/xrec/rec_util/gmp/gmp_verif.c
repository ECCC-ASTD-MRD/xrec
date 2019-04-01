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

extern GeoMapInfoStruct mapInfo;

VerifierSegmentLatlon(float lonMin, float lonMax)
{
   float ybidon;
   float x1, x2, delx;
   
   c_gmpl2g(&x1, &ybidon, 0.0, fmod(lonMin, 360.0));
   c_gmpl2g(&x2, &ybidon, 0.0, fmod(lonMax, 360.0));
   c_gmpl2g(&delx,&ybidon, 0.0, fmod((float)mapInfo.lonOrigine + lonMax - lonMin, 360.0));
   
   return 1;
/*   if (((int)(x2 - x1+1.5) < (int)(delx))  && !((lonMin < -120.0) && (lonMax > 120.0)))
      {
      return 2;
      }
   else
      {
      return 1;
      }*/
   }

