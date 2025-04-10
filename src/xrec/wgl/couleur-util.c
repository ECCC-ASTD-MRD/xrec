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

#include <stdlib.h>
#include <stdio.h>
#include "wgl.h"

int dist(int dr, int dg, int db)
{
   return dr*dr + db*db + dg*dg;
   }

#define LIRE   0
#define ECRIRE 1

int hsvTable[256][3];

int MatchColorIndex(int r, int g, int b, int colorTable[][3], int colDebut, int colFin)
{
   int i, iMin;
   int dr, dg, db, diff, minDiff;
   
   i = 0; iMin = 0;
   dr = abs(r - colorTable[0][0]);
   dg = abs(g - colorTable[0][1]);
   db = abs(b - colorTable[0][2]);

   minDiff = dist(dr,dg,db);
   
   for (i=colDebut; i <= colFin; i++)
      {
      dr = abs(r - colorTable[i][0]);
      dg = abs(g - colorTable[i][1]);
      db = abs(b - colorTable[i][2]);

      diff = dist(dr,dg,db);
      if (diff < minDiff)
	 {
	 minDiff = diff;
	 iMin = i;
	 }
      }

   /**printf("Demande: <%d> %4d-%4d-%4d, Recu: %4d,%4d,%4d, indice: %4d\n", i,r,g,b,colorTable[iMin][0],colorTable[iMin][1],colorTable [iMin][2], iMin);**/
   return iMin;
   }


float maximum(float a, float b, float c)
{
   float max;

   max=a;
   if (b > max)
      max = b;

   if (c > max)
      max = c;

   return max;
   }

float minimum(float a, float b, float c)
{
   float min;

   min=a;
   if (b < min)
      min = b;

   if (c < min)
      min = c;

   return min;
   }

void RGBaHSV(int r,int g,int b,int *h,int *s, int *v)
{
   float cmin, cmax;
   float fh, fs, fv;
   float fr,fg,fb;
   float rc, gc, bc;
   float del;
   
   fr = r / 256.0; fg = g / 256.0; fb = b / 256.0;
   cmin = minimum(fr,fg,fb);
   cmax = maximum(fr,fg,fb);
   del  = cmax - cmin;

   fv = cmax;
   if (cmax != 0.0)
      fs = (cmax - cmin) / cmax;
   else
      fs = 0.0;

   if (fs == 0.0)
      fh = 0.0;
   else
      {
      rc = (cmax - fr)/del;
      gc = (cmax - fg)/del;
      bc = (cmax - fb)/del;

      if (fr == cmax)
	 fh = bc - gc;
      else
	 {
	 if (fg == cmax)
	    fh = 2.0 + rc - bc;
	 else
	    {
	    if (fb == cmax)
	       fh = 4.0 + gc - rc;
	    }
	 }

      }
   
   *h = (int)(60 * fh);
   if (*h < 0)
      *h += 360;
   *h = (int)( *h / 360.0 * 256.0);
   *s = (int)(256 * fs);
   *v = (int)(256 * fv);
   
   }


void HSVaRGB(int h,int s,int v, int *r, int *g, int *b)
{
   float fh, fs, fv;
   float fr,fg,fb;
   int i;
   float f, p, q, t;

   fh = (float) h / 256.0 * 360.0;
   fs = (float) s / 256.0;
   fv = (float) v / 256.0;

   if (fs == 0.0)
      {
      fr=0.0;
      fg=0.0;
      fb=0.0;
      }
   else
      {
      if (fh == 360.0)
	 fh = 0.0;
      
      fh = fh / 60.0;
      i = (int) fh;
      f = fh - (float) i;

      p = fv * (1.0 - fs);
      q = fv * (1.0 - (fs * f));
      t = fv * (1.0 - (fs * (1.0 - f)));

      switch (i)
	 {
	 case 0:
	 fr = fv;
	 fg = t;
	 fb = p;
	 break;

	 case 1:
	 fr = q;
	 fg = fv;
	 fb = p;
	 break;

	 case 2:
	 fr = p;
	 fg = fv;
	 fb = t;
	 break;

	 case 3:
	 fr = p;
	 fg = q;
	 fb = fv;
	 break;

	 case 4:
	 fr = t;
	 fg = p;
	 fb = fv;
	 break;

	 case 5:
	 fr = fv;
	 fg = p;
	 fb = q;
	 break;
	 }
      }

   *r = (int)(256 * fr);
   *g = (int)(256 * fg);
   *b = (int)(256 * fb);

   }

void rgbahsv_(int *h,int *s,int *v,int *r,int *g,int *b)
{
   int r1,g1,b1,h1,s1,v1;

   r1 = (int)*r;
   g1 = (int)*g;
   b1 = (int)*b;

   RGBaHSV(r1,g1,b1,&h1,&s1,&v1);

   *h = (int) h1;
   *s = (int) s1;
   *v = (int) v1;
     
   }

void hsvargb_(int *h,int *s,int *v,int *r,int *g,int *b)
{
   int r1,g1,b1,h1,s1,v1;

   h1 = (int)*h;
   s1 = (int)*s;
   v1 = (int)*v;

   HSVaRGB(h1,s1,v1,&r1,&g1,&b1);

   *r = (int) r1;
   *g = (int) g1;
   *b = (int) b1;
     
   }

void InitHSVTable()
{
   int i, j;
   float h, s, v;

   h = 0.6667;
   s = 1.0;

   for (i=0; i < 35; i++)
      {
      hsvTable[i][0] = h;
      hsvTable[i][1] = s;
      hsvTable[i][2] = 0.5 + 0.5 * ((float)i / 34.0);
      }
  
   v = 1.0;
   s = 1.0;

   for (i=35; i < 220; i++)
      {
      hsvTable[i][0] = 0.6667 * (1.0 - ((float)i - 35.0)/(219.0 - 35.0));
      hsvTable[i][1] = s;
      hsvTable[i][2] = v;
      }
  
   h = 0.0;
   s = 1.0;

   for (i=220; i < 256; i++)
      {
      hsvTable[i][0] = h;
      hsvTable[i][1] = s;
      hsvTable[i][2] = (1.0 - 0.5 * (float)(i - 220) / 35.0);
      }
   
   }

void BuildColIndexTable(int indTable[], int hsvTable[][3], int nbCol, int colDebut, int colFin, int mode)
{
   int i, j, indcol;
   float delcol;
   int r, g, b, h, s, v;
   int old_indcol;
   int  wsHSVTable[256][3];
   int red, green, blue;
   int rgbTable[256][3];
   
   for (i=colDebut; i <= colFin; i++)
      {
      c_wglgco(i, &red, &green, &blue);
      wsHSVTable[i][0] = red;
      wsHSVTable[i][1] = green;
      wsHSVTable[i][2] = blue;
      }
   
   old_indcol = -1;
   
   delcol = (float) (colFin - colDebut);
   for (i=0; i < 256; i++)
      {
      h = hsvTable[i][0];
      s = hsvTable[i][1];
      v = hsvTable[i][2];
      
      switch(mode)
	 {
	 case LIRE:
	 indTable[i] = MatchColorIndex(h, s, v, wsHSVTable, colDebut, colFin); 
	 break;
	 
	 case ECRIRE:
	 HSVaRGB(h, s, v, &r, &g, &b);
	 indcol = (int)((float) i * delcol / 255.0 + 0.5) + colDebut;
	 rgbTable[indcol][0] = r;
	 rgbTable[indcol][1] = g;
	 rgbTable[indcol][2] = b;

	 indTable[i] = indcol;
	 old_indcol = indcol;
	 break;
	 }
      }
   if (mode == ECRIRE)
      c_wglmcos(indTable, 256, rgbTable);
   }
