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

extern float gmp_xmin, gmp_xmax, gmp_ymin, gmp_ymax;
int clip(float x1, float y1, float x2, float y2)
{
   int c1, c2;
   float dx, dy;
   int res;
   
   c1 = CODE(x1, y1, gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax);
   c2 = CODE(x2, y2, gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax);
   /*    if (x1 == x2 || y1 == y2) return DEHORS; */
   
   if (c1 & c2)
      {
      return DEHORS;
      }
   else
      {
      while (c1 | c2)
	 {
	 if (c1 & c2)
	    return DEDANS;
	 
	 dx =x2 - x1;
	 dy = y2 - y1;
	 
	 if (c1)
	    {
	    if (x1<gmp_xmin)
	       {
	       y1 += dy * (gmp_xmin - x1) / dx;
	       x1 = gmp_xmin;
	       }
	    else
	       {
	       if (x1 > gmp_xmax)
		  {
		  y1 += dy * (gmp_xmax - x1) / dx;
		  x1 =  gmp_xmax;
		  }
	       else
		  {
		  if (y1 < gmp_ymin)
		     {
		     x1 += dx * (gmp_ymin - y1) / dy;
		     y1 = gmp_ymin;
		     }
		  else
		     {
		     x1 += dx * (gmp_ymax - y1) / dy;
		     y1 = gmp_ymax;
		     }
		  }
	       }
	    c1 = CODE(x1, y1,gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax);
	    }
	 else
	    if (x2<gmp_xmin)
	       {
	       y2 += dy * (gmp_xmin - x2) / dx;
	       x2 = gmp_xmin;
	       }
	    else
	       {
	       if (x2 > gmp_xmax)
		  {
		  y2 += dy * (gmp_xmax - x2) / dx;
		  x2 =  gmp_xmax;
		  }
	       else
		  {
		  if (y2 < gmp_ymin)
		     {
		     x2 += dx * (gmp_ymin - y2) / dy;
		     y2 = gmp_ymin;
		     }
		  else
		     {
		     x2 += dx * (gmp_ymax - y2) / dy;
		     y2 = gmp_ymax;
		     }
		  }
	       }
	 c2 = CODE(x2, y2,gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax);
	 }
      
      return DEDANS;
      }
   }
