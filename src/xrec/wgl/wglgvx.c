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

void f77name(wglgvx)(float *xdebut, float *ydebut, float *xfin, float *yfin)
{
   c_wglgvx(xdebut, ydebut, xfin, yfin);
   }

/**
******
**/

int c_wglgvx(float *xdebut, float *ydebut, float *xfin, float *yfin)
{
   c_wglgwz(&w, &h);
   c_wgliax(xdebut, ydebut, 0, 0);
   c_wgliax(xfin,   yfin,   w - 1, h - 1);

   switch (usSpace.mode)
      {
      case 3:
      case 4:
      *xdebut = LE_LOG(*xdebut);
      *xfin   = LE_LOG(*xfin);
      break;
      
      default:
      break;
      }

   switch (usSpace.mode)
      {
      case 2:
      case 4:
      *ydebut = LE_LOG(*ydebut);
      *yfin   = LE_LOG(*yfin);
      break;

      default:
      break;
      }

   if (usSpace.densiteX > 0.0)
      {
      if (*xdebut < usSpace.xdebut)
	 *xdebut = usSpace.xdebut;
      
      if (*xfin > usSpace.xfin)
	 *xfin = usSpace.xfin;
      }
   else
      {
      if (*xdebut > usSpace.xdebut)
	 *xdebut = usSpace.xdebut;
      
      if (*xfin < usSpace.xfin)
	 *xfin = usSpace.xfin;
      }

   if (usSpace.densiteY > 0.0)
      {
      if (*ydebut < usSpace.ydebut)
	 *ydebut = usSpace.ydebut;
      
      if (*yfin > usSpace.yfin)
	 *yfin = usSpace.yfin;
      }
   else
      {
      if (*ydebut > usSpace.ydebut)
	 *ydebut = usSpace.ydebut;
      
      if (*yfin < usSpace.yfin)
	 *yfin = usSpace.yfin;
      }

   switch (usSpace.mode)
      {
      case 3:
      case 4:
      *xdebut = (float)pow(10.0, *xdebut);
      *xfin   = (float)pow(10.0, *xfin);
      break;
      
      default:
      break;
      }

   switch (usSpace.mode)
      {
      case 2:
      case 4:
      *ydebut = (float)pow(10.0, *ydebut);
      *yfin   = (float)pow(10.0, *yfin);
      break;

      default:
      break;
      }

   return 0;
}

