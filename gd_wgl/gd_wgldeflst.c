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

#include <wgl_gd.h>

gd_wgldeflst(dashIndex, linestyle)
int dashIndex, linestyle;
{

   int i;
   int ind;
   short dp[16];
   short lastVal, indtab, valCherchee;


   ind = dashIndex + 128;
   
   /**
    ** Decomposition du patron en 0 et 1
    **/
   
   dp[15] = linestyle & 1;
   for (i=0; i < 15; i++)
      {
      linestyle = linestyle >> 1;
      dp[14 - i] = linestyle & 1;
      }

   /**
    ** Transformation du patron dans le format requis pour XSetDashes
    **/
   
   i = 0;
   indtab = 0;
   valCherchee = 0;

   while (i < 16 && dp[i] == 0)
      i++;

   lastVal = i;
   wglDashPatternOffsets[ind] = i;

   while (i < 16)
      {
      valCherchee = !valCherchee;
      lastVal = i;
      while (i < 16 && dp[i] == valCherchee)
         i++;

      wglDashPatterns[ind][indtab] = (char) (i - lastVal);
      indtab++;
      }
   
   if (wglDashPatternOffsets[ind] != 0)
      {
      if (((indtab - 1) % 2) == 1)
         {
         wglDashPatterns[ind][indtab - 1] += wglDashPatternOffsets[ind];
         }
      else
         {
         wglDashPatterns[ind][indtab] = wglDashPatternOffsets[ind];
         indtab++;
         }
      }

   wglDashPatternLengths[ind] = indtab;

   }

