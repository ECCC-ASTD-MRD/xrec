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

void f77name(wglcmx)(float *left, float *bottom, float *right, float *top)
{
   return c_wglcmx(*left, *bottom, *right, *top);
   }

/**
******
**/

void c_wglcmx(float left, float bottom, float right, float top)
{
  int ibottom, ileft, iright, itop;
  
  c_wglxai(&ileft, &ibottom, left, bottom);
  c_wglxai(&iright, &itop, right, top);
  
  return c_wglcmi(ileft, ibottom, iright, itop);
}
