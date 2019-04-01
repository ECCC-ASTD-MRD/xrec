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

#include <gd_wgl.h>

gd_wgliax(x, y, i, j)
float *x, *y;
int i, j;
{
   *x = ((float)(i - usSpace.idebut) / usSpace.densiteX) + usSpace.xdebut;
   *y = ((float)(j - usSpace.jdebut) / usSpace.densiteY) + usSpace.ydebut;
   
   switch (usSpace.mode)
      {
      case 3:
      case 4:
      *x = (float)(pow(10.0, *x));
      break;
      
      default:
      break;
      }
   
   switch (usSpace.mode)
      {
      case 2:
      case 4:
      *y = (float)(pow(10.0, *y));
      break;

      default:
      break;
      }
   }
