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

void c_gmpl2g(float *x,float *y,float lat,float lon)
{
   float tmpLat, tmpLon;
   float xori, yori;
   char type;

   xori = 1.0;
   yori = 1.0;

   if (mapInfo.type == 'Z')
      {
      type = mapInfo.typeref;
      }
   else
      {
      type = mapInfo.type;
      }

   tmpLat = lat;
   tmpLon = lon;

   switch(type)
      {
      case 'N':
      case 'S':
      f77name(xyfll)(x,y,&tmpLat,&tmpLon,
             &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
             &mapInfo.hemisphere);
      *x += mapInfo.PosXDuPole;
      *y += mapInfo.PosYDuPole;
      break;
      
      case 'G':
      case 'A':
      case 'B':
      case 'L':
		case 'U':
      c_grfll(x, y, tmpLat, tmpLon,
	      mapInfo.latOrigine, mapInfo.lonOrigine, mapInfo.deltaLat, mapInfo.deltaLon, 
	      mapInfo.xOrigine, mapInfo.yOrigine, mapInfo.indOrientation);
      break;
      }
   }


