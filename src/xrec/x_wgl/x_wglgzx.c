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

void x_wglgzx(float *xsize, float *ysize)
{
   switch (usSpace.mode)
      {
      case 3:
      case 4:
      *xsize = (float)pow(10.0, usSpace.xfin) - (float)pow(10.0, usSpace.xdebut) + 1.0;
      break;

      default:
      *xsize = usSpace.xfin - usSpace.xdebut + 1.0;
      break;
      }

   switch (usSpace.mode)
      {
      case 2:
      case 4:
      *ysize = (float)pow(10.0, usSpace.yfin) - (float)pow(10.0, usSpace.ydebut) + 1.0;
      break;
      
      default:
      *ysize = usSpace.yfin - usSpace.ydebut + 1.0;
      break;
      }

   }
