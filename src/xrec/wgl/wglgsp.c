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

void f77name(wglgsp)(float *xdebut, float *ydebut, float *xfin, float *yfin, int   *idebut, int *jdebut, int *ifin, int *jfin)
{
   c_wglgsp(xdebut, ydebut, xfin, yfin, idebut, jdebut, ifin, jfin);
   }

/**
******
**/


void c_wglgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int   *idebut, int *jdebut, int *ifin, int *jfin)
{
   c_wglgsx(xdebut, ydebut, xfin, yfin);
   wglc_wgl->wglgsi(idebut, jdebut, ifin, jfin);
   }
