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
#include <rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

void PointerFleche(float xdepart, float ydepart, float dirVent, float vitVent, int rayon, int width)
{
   float radian  = 0.0174503293;
   float rs, rc;
   int idepart, jdepart;
   int i, j, i0, j0, i1, j1, i2, j2,i3,j3,i4,j4,i5,j5;
   float fraction;
   int poly[6][2];
   int signe,is,ic;

   float copieVitesse,theta;
   float offsetFleche1, offsetFleche2;

   c_wgllwi(width);
   offsetFleche1 = (180.0 - 30.0) * radian;
   offsetFleche2 = (180.0 + 30.0) * radian;

   c_wglxai(&idepart, &jdepart, xdepart, ydepart);
   theta = fmod(360.0+270-dirVent,360.0);
   theta = radian * theta;

   i0 = idepart;
   j0 = jdepart;
   rs = rayon * sin(theta);
   rc = rayon * cos(theta);
   i = (int)(rc);
   j = (int)(rs);
   i2 = i0 +i;
   j2 = j0 +j;
   i3 = i0+ROUND(0.60 * rc);
   j3 = j0+ROUND(0.60 * rs);
   i1 = ROUND( 0.33 * rayon  * cos (theta + offsetFleche1));
   j1 = ROUND( 0.33 * rayon  * sin (theta + offsetFleche1));
   i4 = ROUND( 0.33 * rayon  * cos (theta + offsetFleche2));
   j4 = ROUND( 0.33 * rayon  * sin (theta + offsetFleche2));

   is = ROUND((float)width*0.5*rs/(float)rayon);
   ic = ROUND((float)width*0.5*rc/(float)rayon);

   c_wglmvi(i0,j0);
   c_wgldri(i2,j2);

   c_wglmvi(i2+i1,j2+j1);
   c_wgldri(i2-ic,j2-is);

   c_wglmvi(i2+i4,j2+j4);
   c_wgldri(i2-ic,j2-is);
   }
