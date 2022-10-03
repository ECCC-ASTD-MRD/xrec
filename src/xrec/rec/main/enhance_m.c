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

#include <math.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

void enhancefracs_m(float *vals, int nbVals, float min, float delta, int variation)
{
   int i;
   float fraction;

   for (i=0; i < nbVals; i++)
     {
     if (vals[i] != MISSING)
       {
       switch (variation)
	 {
	 case LINEAIRE:
	   vals[i] = (vals[i]-min)/delta;
	   break;
	   
	 case QUADRATIQUE:
	   fraction = (vals[i]-min)/delta;
	   vals[i] = fraction*fraction;
	   break;
	   
	 case CUBIQUE:
	   fraction = (vals[i]-min)/delta;
	   vals[i] = fraction*fraction*fraction;
	   break;
	   
	 case RACINE_CARREE:
	   fraction = (vals[i]-min)/delta;
	   fraction = fraction < 0.0 ? 0.0 : (fraction > 1.0 ? 1.0 : fraction);
	   vals[i] = sqrt(fraction);
	   break;
	   
	 case RACINE_CUBIQUE:
	   fraction = (vals[i]-min)/delta;
	   fraction = fraction < 0.0 ? 0.0 : (fraction > 1.0 ? 1.0 : fraction);
	   vals[i] = pow(fraction,0.3333);
	   break;
	   
	 case EXTREMUM:
	   fraction = (vals[i]-min)/delta;
	   fraction = fraction < 0.0 ? 0.0 : (fraction > 1.0 ? 1.0 : fraction);
	   fraction -= 0.5;
	   vals[i] = 0.5 + 4.0 * (pow(fraction,3.0));
	   break;
	   
	 case MOYENNE:
	   fraction = (vals[i]-min)/delta;
	   fraction = fraction < 0.0 ? 0.0 : (fraction > 1.0 ? 1.0 : fraction);
	   fraction -= 0.5;
	   vals[i] =pow(fabs(fraction),0.33);
	   if (fraction < 0.0)
	     {
	     vals[i] = -vals[i];
	     }
	   
	   vals[i] = 0.625*(0.8+vals[i]);
	   break;
	 }
       }
     }
}

