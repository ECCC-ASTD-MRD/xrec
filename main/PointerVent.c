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

void PointerVent(xdepart, ydepart, dirVent, vitVent, rayon)
float xdepart, ydepart;
float dirVent, vitVent;
int rayon;
{
   float barbe, fraction;
   float radian  = 0.0174503293;
   float r0, r1;
   int idepart, jdepart;
   int i, j, i0, j0, i1, j1;

   float copieVitesse;
   int triangle[3][2];
   
   barbe = rayon * 0.50; 
   c_wglxai(&idepart, &jdepart, xdepart, ydepart);

   if (vitVent <= 2.0)
      {
      c_wglcli(idepart, jdepart, 5);
      return;
      }

   i0 = idepart;
   j0 = jdepart;
   r0 = rayon * sin(dirVent * radian);
   r1 = rayon * cos(dirVent * radian);
   i = (int)r0;
   j = (int)r1;
   
   fraction = 1.0;
   copieVitesse = vitVent;
   while (copieVitesse >= 50.0)
      {
      fraction += 0.3;
      copieVitesse -= 50.0;
      }
   
   c_wglmvi(idepart,jdepart);
   c_wgldri((int)(i*fraction + idepart), (int)(j*fraction + jdepart));
   

   while (fraction > 1.0)
      {
      i1 = (int)(barbe * sin((dirVent + 55.0) * radian));
      j1 = (int)(barbe * cos((dirVent + 55.0) * radian));
      triangle[0][0]=idepart + i * (fraction + 0.10) ;
      triangle[0][1]=jdepart + j * (fraction + 0.10) ;
      triangle[1][0]=idepart + i * (fraction + 0.10) + i1;
      triangle[1][1]=jdepart + j * (fraction + 0.10) + j1;
      triangle[2][0]=idepart + i * (fraction - 0.20) ;
      triangle[2][1]=jdepart + j * (fraction - 0.20) ;
      c_wglpfi(3,triangle);
      fraction -= 0.3;
      vitVent -= 50.0;
      }

   while (vitVent >= 10.0)
      {
      i1 = (int)(barbe * sin((dirVent + 55.0) * radian));
      j1 = (int)(barbe * cos((dirVent + 55.0) * radian));
      i0 = idepart + i * fraction ;
      j0 = jdepart + j * fraction ;
      i1 += i0;
      j1 += j0;
      c_wglmvi(i0, j0);
      c_wgldri(i1, j1);
      fraction = fraction -0.2;
      vitVent = vitVent - 10.0;
      }
   
   
   if (vitVent > 2.0) 
      {
      if (fraction == 1.0)
	 fraction = 0.8;
      i1 = (int)(barbe * 0.5 * sin((dirVent + 55.0) * radian));
      j1 = (int)(barbe * 0.5 * cos((dirVent + 55.0) * radian));
      i0 = idepart + i * fraction ;
      j0 = jdepart + j * fraction ;
      i1 += i0;
      j1 += j0;
      c_wglmvi(i0,j0);
      c_wgldri(i1, j1);
      }
   
   }
