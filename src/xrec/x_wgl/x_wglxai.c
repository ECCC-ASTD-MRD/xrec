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

#include <wgl_x.h>

x_wglxai(int *i, int *j, float x, float y)
{
   float x1, y1;

   switch (usSpace.mode)
      {
      case 0:
      case 1:
      case 2:
      *i = ROUND(((x - usSpace.xdebut) * usSpace.densiteX) + usSpace.idebut);
      break;

      case 3:
      case 4:
      x1 = LE_LOG(x);
      *i = ROUND(((x1 - usSpace.xdebut) * usSpace.densiteX) + usSpace.idebut);
      break;
      }
   
   switch (usSpace.mode)
      {
      case 0:
      case 1:
      case 3:
      *j = ROUND(((y - usSpace.ydebut) * usSpace.densiteY) + usSpace.jdebut);
      break;


      case 2:
      case 4:
      y1 = LE_LOG(y);
      *j = ROUND(((y1 - usSpace.ydebut) * usSpace.densiteY) + usSpace.jdebut);
      break;
      }
   }

