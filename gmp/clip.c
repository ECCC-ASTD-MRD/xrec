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

extern float  xmin, ymin, xmax, ymax;

clip(x1, y1, x2, y2)
float x1, y1, x2, y2;
{
   int c1, c2;
   float dx, dy;
   int res;
   
   c1 = CODE(x1, y1, xmin, ymin, xmax, ymax);
   c2 = CODE(x2, y2, xmin, ymin, xmax, ymax);
   if (x1 == x2 || y1 == y2) return DEHORS;
   
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
	    if (x1<xmin)
	       {
	       y1 += dy * (xmin - x1) / dx;
	       x1 = xmin;
	       }
	    else
	       {
	       if (x1 > xmax)
		  {
		  y1 += dy * (xmax - x1) / dx;
		  x1 =  xmax;
		  }
	       else
		  {
		  if (y1 < ymin)
		     {
		     x1 += dx * (ymin - y1) / dy;
		     y1 = ymin;
		     }
		  else
		     {
		     x1 += dx * (ymax - y1) / dy;
		     y1 = ymax;
		     }
		  }
	       }
	    c1 = CODE(x1, y1,xmin, ymin, xmax, ymax);
	    }
	 else
	    if (x2<xmin)
	       {
	       y2 += dy * (xmin - x2) / dx;
	       x2 = xmin;
	       }
	    else
	       {
	       if (x2 > xmax)
		  {
		  y2 += dy * (xmax - x2) / dx;
		  x2 =  xmax;
		  }
	       else
		  {
		  if (y2 < ymin)
		     {
		     x2 += dx * (ymin - y2) / dy;
		     y2 = ymin;
		     }
		  else
		     {
		     x2 += dx * (ymax - y2) / dy;
		     y2 = ymax;
		     }
		  }
	       }
	 c2 = CODE(x2, y2,xmin, ymin, xmax, ymax);
	 }
      
      return DEDANS;
      }
   }
