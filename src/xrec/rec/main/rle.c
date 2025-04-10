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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rmn/rpnmacros.h>
#include "rec_functions.h"

void EncodeRLEData(unsigned char *out, int  *nptsout, char *in, int  nptsin)
{
   int i,pos,repeat;
   unsigned char valOffset = 32;
   unsigned char repeatOffset = 128;
   
   strncpy(out,"RLE8",4);
   out[4] = valOffset;
   out[5] = repeatOffset;
   pos = 6;
   repeat = 1;
   
   for (i=1; i < nptsin; i++)
      {
      if (in[i-1] == in[i] && repeat < repeatOffset)
	 {
	 repeat += 1;
	 }
      else
	 {
	 if (repeat >= 1)
	    {
	    out[pos] = repeat+repeatOffset-1;
	    pos++;
	    repeat = 1;
	    }
	 
	 out[pos] = in[i-1]+valOffset;
	 pos++;
	 }
      }
   
   if (repeat > 1)
      {
      out[pos] = repeat+repeatOffset-1;
      pos++;
      }
   
   out[pos] = in[i-1]+valOffset;
   pos++;
   *nptsout = pos;
   }

/**
**********
**********
*/

void f77name(decrle)(float *fld, int *npts)
{
   char *tmp;
   int i;
   
   tmp = (char *)  calloc(*npts, sizeof(char));
   
   DecodeRLEData(tmp,*npts,(char *)fld);
   for (i=0; i < *npts; i++)
      {
      fld[i] = (float)tmp[i];
      }
   free(tmp);
   }

/**
**********
**********
*/

int DecodeRLEData(char *out, int  nptsout, unsigned char *in)
{
   int i,j,pos,repeat;
   unsigned char repeatOffset,valOffset;

   if (0 != strncmp("RLE8",in,4))
      {
      return -1;
      }
   
   valOffset = in[4];
   repeatOffset = in[5];

   i = 0;
   pos = 6;
   while (i < nptsout)
      {
      if (in[pos] < (repeatOffset-1))
	 {
	 out[i] = in[pos] - valOffset;
	 i++;
	 pos++;
	 }
      else
	 {
	 for (j=0; j < (in[pos]-(repeatOffset-1));j++)
	    {
	    out[i] = in[pos+1]-valOffset;
	    i++;
	    }
	 pos+=2;
	 }
      }

   return 0;
   }

