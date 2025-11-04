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

#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

float ip12lvl(int ip1)
{
   if (ip1 < 2000)
      return (float)ip1;

   if (ip1 > 12000)
      return (float)(ip1-12000)*5.0;

   return (float)(ip1-10000)/2000.0;
   }

/**
*************************************
*************************************
**/

int lvl2ip1(double lvl,int coord)
{
   switch (coord)
      {
      case SIGMA:
      return ROUND(lvl*10000+2000);

      case PRESSION:
      return ROUND(lvl);

      case METRES:
      return ROUND(lvl*0.20+12001);

      default:
      fprintf(stderr,"<lvl2ip1>: Bad coordinate!: %d\n",coord);
      }
   return 0;
   }


/**
*************************************
*************************************
**/

int ip12coord(int ip1)
{
   if (ip1 < 2000)
      return PRESSION;

   if (ip1 > 12000)
      return METRES;

   return SIGMA;
   }
