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

/* ----------------------------------------------------------------------------------------- */

void f77name(wglpsx)(float *x, float *y, char *string, int *stringLength, int *size, 
		     int *orient, int *codeCentrage, F2Cl ftnStringLength)
{
  string[ftnStringLength] = '\0';
  c_wglpsx(*x, *y, string, *stringLength, *size, *orient, *codeCentrage);
}


/**
 ******
 **/

int  c_wglpsx(float x, float y, char *string, int stringLength, int size, 
	      int orient, int codeCentrage)
{
  int i, j;

  c_wglxai(&i, &j, x, y);
  return c_wglpsi(i, j, string, stringLength, size, orient, codeCentrage);
}

