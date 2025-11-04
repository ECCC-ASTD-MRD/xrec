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
#include <vcar.h>
#include "rec_util.h"
   
void f77name(xfrstpt)(int *x, int *y)
{
int flag = 0;
   f77name(xplotit)(x, y, &flag); 
   }

 

void f77name(xvector)(float *x, float *y)
{
   c_wgldrx(*x, *y);
   }

void f77name(xgetsi)(int *i, int *j)
{
   *i = 8;
   *j = 8;
   }

void f77name(xpolygon)()	
{
   }

void f77name(xframe)()	
{
   }

void f77name(xset)(int *ixmin, int *ixmax, int *iymin, int *iymax, float *xmin, float *xmax, float *ymin, float *ymax, int *ltype)
{
   float *x1, *y1, *x2, *y2;
   int i1, i2, j1, j2;
   int largeurFenetre, hauteurFenetre;

   if (*ixmin > 0 && *ixmin < 32768 && *ixmax > 0 && *ixmax < 32768) 
      {
      c_wglssp(*xmin, *ymin, *xmax, *ymax, 0, 0, 0, 0, *ltype);
      c_wglgsi(ixmin, iymin, ixmax, iymax);
      }
   else
      {
      x1 = (float *) ixmin; x2 = (float *) ixmax;
      y1 = (float *) iymin; y2 = (float *) iymax;
      c_wglgwz(&largeurFenetre, &hauteurFenetre);
      i1 = (int)(*x1 * largeurFenetre);
      i2 = (int)(*x2 * largeurFenetre);
      j1 = (int)(*y1 * hauteurFenetre);
      j2 = (int)(*y2 * hauteurFenetre);
      c_wglssp(*xmin, *ymin, *xmax, *ymax, i1, j1, i2, j2, *ltype);
      }
   }

void f77name(xqqqa16)()
{
   }

void f77name(xwindow)(float *x1, float *y1, float *x2, float *y2)
{
   c_wglcmx(*x1, *y1, *x2, *y2);
   }

void f77name(xoptn)(int *optn, int *val)
{
   char *ptr, option[4];

   ptr = (char *) optn;
   option[0] = *ptr;
   ptr++;
   option[1] = *ptr;
   option[2] = '\0';
   if (0 == strcmp(option, "TH"))
      {
      c_wgllwi(*val);
      }
   }













