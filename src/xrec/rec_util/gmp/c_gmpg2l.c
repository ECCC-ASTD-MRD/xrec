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
#include <rpnmacros.h>

extern GeoMapInfoStruct     mapInfo;

void c_gmpg2l(float *lat,float *lon,float x,float y)
{
   int un = 1;
   float tmpX, tmpY;
   char type;

   if (mapInfo.type == 'Z')
      type = mapInfo.typeref;
   else
      type = mapInfo.type;

   tmpX = x;
   tmpY = y;

   switch(type)
      {
      case 'N':
      case 'S':
      tmpX-=mapInfo.PosXDuPole;
      tmpY-=mapInfo.PosYDuPole;
      f77name(llfxy)(lat, lon, &tmpX, &tmpY, &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich, &mapInfo.hemisphere);
      break;
      
      case 'G':
      case 'A':
      case 'B':
      case 'L':
		case 'U':
      gmp_llfgr(lat, lon, tmpX, tmpY, mapInfo.latOrigine, mapInfo.lonOrigine, mapInfo.deltaLat, mapInfo.deltaLon);
      break;

      default:
      break;
      }
   }

