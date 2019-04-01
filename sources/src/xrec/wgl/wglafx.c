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

f77name(wglafx)(float *x, float *y, float *rayon, float *angleDebut, float *angleFin)
{
   c_wglafx(*x, *y, *rayon, *angleDebut, *angleFin);
   }

/**
 ******
 **/

c_wglafx(float x, float y, float rayon, float angleDebut, float angleFin)
{
   int i, j, idebut, jdebut, ifin, jfin;
   int istartAngle, iendAngle;
   int width, height;
   
   c_wglxai(&idebut, &jdebut, x - rayon, y - rayon);
   c_wglxai(&ifin, &jfin, x + rayon, y + rayon);
   c_wglxai(&i, &j, x, y);
   
   width = ifin - idebut + 1;
   height = jfin -jdebut + 1;

   
   istartAngle = (int) (angleDebut * 64);
   iendAngle   = (int) ((angleFin - angleDebut) * 64);
   c_wglafi(i, j, (int) rayon, (int)angleDebut, (int)angleFin);
   }







