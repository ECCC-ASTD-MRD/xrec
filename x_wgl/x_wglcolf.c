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

extern unsigned int colorbitrange[3][3];
unsigned int x_wglcolf(float couleur);

unsigned int x_wglcolf(float couleur)
{
   int r,g,b;
   int r1,g1,b1;
   int r2,g2,b2;
   unsigned int icol, pixel;
   float dcol;
   int normal;

   wglfshlb();
   
   icol = (int) couleur;
   dcol = couleur - icol;
   r1=couleurs[icol].red;
   g1=couleurs[icol].green;
   b1=couleurs[icol].blue;

   r2=couleurs[icol+1].red;
   g2=couleurs[icol+1].green;
   b2=couleurs[icol+1].blue;
   
   r = r1 + dcol * (r2-r1);
   g = g1 + dcol * (g2-g1);
   b = b1 + dcol * (b2-b1);

   pixel =  ((r * colorbitrange[0][2]) >>16) << colorbitrange[0][0];
   pixel += ((g * colorbitrange[1][2]) >>16)<< colorbitrange[1][0];
   pixel += ((b * colorbitrange[2][2]) >>16)<< colorbitrange[2][0];

   XSetForeground(wglDisp, wglLineGC, pixel);
   XSetForeground(wglDisp, wglFillGC, pixel);

   return pixel;

}

unsigned int x_wglcolfs(float *cols, unsigned int *pixels, int n)
{
  int r,g,b;
  int r1,g1,b1;
  int r2,g2,b2;
  unsigned int icol, pixel;
  float dcol;
  int i,normal;
  
  for (i=0; i < n; i++)
    {
    icol = (int) cols[i];
    dcol = cols[i] - icol;
    r1=couleurs[icol].red;
    g1=couleurs[icol].green;
    b1=couleurs[icol].blue;
    
    r2=couleurs[icol+1].red;
    g2=couleurs[icol+1].green;
    b2=couleurs[icol+1].blue;
    
    r = r1 + dcol * (r2-r1);
    g = g1 + dcol * (g2-g1);
    b = b1 + dcol * (b2-b1);
    
    pixels[i] =  ((r*colorbitrange[0][2])>>16) << colorbitrange[0][0];
    pixels[i] += ((g*colorbitrange[1][2])>>16) << colorbitrange[1][0];
    pixels[i] += ((b*colorbitrange[2][2])>>16) << colorbitrange[2][0];
    }
}

unsigned int wglcolfs_fst(float *cols, unsigned int *pixels, int n)
{
  int r,g,b;
  int r1,g1,b1;
  int r2,g2,b2;
  unsigned int icol, pixel;
  float dcol;
  int i,normal;
  int ir0,ig0,ib0,ir2,ig2,ib2;

  ir0 = colorbitrange[0][0];
  ig0 = colorbitrange[1][0];
  ib0 = colorbitrange[2][0];

  ir2 = colorbitrange[0][2];
  ig2 = colorbitrange[1][2];
  ib2 = colorbitrange[2][2];
  
  for (i=0; i < n; i++)
    {
    icol = (int) cols[i];
    
    pixels[i] =  (((couleurs[icol].red * ir2) >>16) << ir0) + (((couleurs[icol].green * ig2) >>16) << ig0) + (((couleurs[icol].blue*ib2) >>16) << ib0);
    }
}
