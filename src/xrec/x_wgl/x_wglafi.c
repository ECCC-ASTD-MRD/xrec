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

#include <wgl_x.h>


void x_wglafi(int i, int j, int rayon, float angleDebut, float angleFin)
{
   int idebut, jdebut, ifin, jfin;
   int istartAngle, iendAngle;
   int width, height;

   idebut = i - rayon;
   jdebut = j - rayon;
   ifin   = i + rayon;
   jfin   = j + rayon;
   
   width = ifin - idebut + 1;
   height = jfin -jdebut + 1;
   
   istartAngle = (int) (angleDebut * 64);
   iendAngle   = (int) ((angleFin - angleDebut) * 64);
   XFillArc(wglDisp, wglDrawable, wglFillGC, idebut, h - jfin, width, height, istartAngle, iendAngle);
   }

