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

f77name(wglgsx)(float *xdebut, float *ydebut, float *xfin, float *yfin)
{
   c_wglgsx(xdebut, ydebut, xfin, yfin);
   }

/**
******
**/


c_wglgsx(float *xdebut, float *ydebut, float *xfin, float *yfin)
{
   switch (usSpace.mode)
      {
      case 3:
      case 4:
      *xdebut = (float)pow(10.0, usSpace.xdebut);
      *xfin   = (float)pow(10.0, usSpace.xfin);
      
      default:
      *xdebut = usSpace.xdebut;
      *xfin   = usSpace.xfin;
      break;
      }

   switch (usSpace.mode)
      {
      case 2:
      case 4:
      *ydebut = (float)pow(10.0, usSpace.ydebut);
      *yfin   = (float)pow(10.0, usSpace.yfin);
      break;

      default:
      *ydebut = usSpace.ydebut;
      *yfin   = usSpace.yfin;
      break;
      }

   }


