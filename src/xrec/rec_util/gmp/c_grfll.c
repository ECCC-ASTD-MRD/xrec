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

#include <math.h>
#include <gmp.h>

extern GeoMapInfoStruct mapInfo;

void c_grfll(float *x, float *y, float lat, float lon, float latOrigine, float lonOrigine, float deltaLat, float deltaLon, float xOrigine, float yOrigine, int   orientation)
{
   *x = fmod((lon - lonOrigine)+360.,360.) / deltaLon + xOrigine;

   if (*x > (float)mapInfo.ni && xOrigine != 0.0)
      {
      *x -= 360./deltaLon;
      }

   *y = (lat - latOrigine) / deltaLat + yOrigine;
   
   }

