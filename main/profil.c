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

#include <stdio.h>
#include <c_wgl.h>
#include <rpnmacros.h>
   
courbe(x, y, npts, xmin, xmax, ymin, ymax, intX, nbIntX, intY, nbIntY, axeXRenverse, axeYRenverse, i1, j1, i2, j2)
float x[], y[];
int npts;
float xmin, xmax, ymin, ymax;
float *intX, *intY;
int nbIntX, nbIntY;
int axeXRenverse, axeYRenverse;
int i1, j1, i2, j2;
{
   int wini1, winj1, wini2, winj2;
   float wxmin, wxmax, wymin, wymax;
   int ii, jj;
   
   int i, j;
   float scalfac;
   
   c_wglias(1);
   c_wglcol(BLANC);
   c_wglclr();
   c_wglcol(BLEU);
   c_wgllwi(2);
   
   ProfilCalculerCoordsFenetre(&wini1, &winj1, &wini2, &winj2, i1, j1, i2, j2);
   c_wglssp(xmin, ymax, xmax, ymin, wini1, winj1, wini2, winj2);
   c_wglrli(i1, j1, i2, j2);
   c_wglrlx(xmin, ymin, xmax, ymax);

   ProfilAfficherAxe(x, npts, intX, nbIntX, xmin, xmax, xmin, ymax, 0);
   ProfilAfficherAxe(y, npts, intY, nbIntY, ymin, ymax, xmin, ymax, 1);

   ProfilAfficherTicks(x, npts, intX, nbIntX, xmin, xmax, xmin, ymax, ymin, 0);
   ProfilAfficherTicks(y, npts, intY, nbIntY, ymin, ymax, xmin, ymin, xmax, 1);

   c_wglmvx(x[0], y[0]);
   for (i=0; i < npts; i++)
      {
      c_wgldrx(x[i], y[i]);
      }
   
   for (i=0; i < npts; i++)
      {
      c_wglxai(&ii, &jj, x[i], y[i]);
      c_wglrli(ii-2,jj-2,ii+2,jj+2);
      }
   
   }

ProfilCalculerMinMax(xmin, ymin, xmax, ymax, x, y, npts)
float *xmin, *ymin, *xmax, *ymax, *x, *y;
int npts;
{
   int i;
   *xmin = x[0]; *ymin = y[0]; *xmax = x[0], *ymax = y[0];
   for (i=0; i < npts; i++)
      {
      *xmin = (*xmin > x[i]) ? x[i] : *xmin;
      *ymin = (*ymin > y[i]) ? y[i] : *ymin;
      *xmax = (*xmax < x[i]) ? x[i] : *xmax;
      *ymax = (*ymax < y[i]) ? y[i] : *ymax;
      }
   }

ProfilCalculerCoordsFenetre(wini1, winj1, wini2, winj2, i1, j1, i2, j2)
int *wini1, *winj1, *wini2, *winj2;
int i1, j1, i2, j2;
{
   int largeurNombres, hauteurNombres;
   char bidon[16];

   strcpy(bidon, "-0.0000e+00");
   largeurNombres = c_wglwsi(bidon, strlen(bidon));
   hauteurNombres = c_wglhsi(bidon, strlen(bidon));
   
   *wini1 = i1 + largeurNombres + 10;
   *winj1 = j1 + hauteurNombres + 10;
   *wini2 = i2 - 10;
   *winj2 = j2 - 10;
   }

ProfilAfficherAxe(axe, npts, inter, nbInter, axemin, axemax, xorig, yorig, direction)
float axe[];
int npts;
float inter[];
int nbInter;
float axemin, axemax, xorig, yorig;
int direction;
{
   float val, x, y;
   int i, j;
   int posX, posY;
   char str[16];

   c_wglfsz(14);

   switch (direction)
      {
      case 0:
      if (nbInter == 1)
	 {
	 val = axemin;
	 while (val <= axemax)
	    {
	    c_wglxai(&posX, &posY, val, yorig);
	    sprintf(str, "%6.0f", val);
	    posY -= c_wglhsi(str, strlen(str));
	    
	    c_wglpsi((int)posX - (int)(0.5 * c_wglwsi(str, strlen(str))), posY, 
		     str, strlen(str), 14, 0, 0);
	    val+= inter[0];
	    }
	 }
      else
	 {
	 i = 0;
	 while (i < nbInter)
	    {
	    val = inter[i];
	    c_wglxai(&posX, &posY, val, yorig);
	    sprintf(str, "%6.0f", val);
	    posY -= c_wglhsi(str, strlen(str));
	    
	    c_wglpsi((int)posX - (int)(0.5 * c_wglwsi(str, strlen(str))), posY, 
		     str, strlen(str), 14, 0, 0);
	    i++;
	    }
	 }
	 break;

      case 1:
      if (nbInter == 1)
	 {
	 val = axemin;
	 while (val <= axemax)
	    {
	    c_wglxai(&posX, &posY, xorig, val);
	    sprintf(str, "%6.0f", val);
	    posX -= c_wglwsi(str, strlen(str));
	    posX -= 10;
	    
	    c_wglpsi(posX, posY - (int)(0.5 * c_wglhsi(str, strlen(str))),
		     str, strlen(str), 14, 0, 0);
	    val+= inter[0];
	    }
	 }
      else
	 {
	 i = 0;
	 while (i < nbInter)
	    {
	    val = inter[i];
	    c_wglxai(&posX, &posY, xorig, val);
	    sprintf(str, "%6.0f", val);
	    posX -= c_wglwsi(str, strlen(str));
	    posX -= 10;
	    
	    c_wglpsi(posX, posY - (int)(0.5 * c_wglhsi(str, strlen(str))),
		     str, strlen(str), 14, 0, 0);
	    i++;
	    }
	 }
      break;
      }
   }

ProfilAfficherTicks(axe, npts, inter, nbInter, axemin, axemax, xorig, yorig, ymax, direction)
float axe[];
int npts;
float inter[];
int nbInter;
float axemin, axemax, xorig, yorig, ymax;
int direction;
{
   float val, x, y;
   int i, j;
   int posX, posY;

   switch (direction)
      {
      case 0:
      if (nbInter == 1)
	 {
	 val = axemin;
	 while (val <= axemax)
	    {
	    c_wglxai(&posX, &posY, val, yorig);
	    c_wglmvi(posX, posY);
	    c_wgldri(posX, posY + 10);

	    c_wglxai(&posX, &posY, val, ymax);
	    c_wglmvi(posX, posY);
	    c_wgldri(posX, posY - 10);

	    val+= inter[0];
	    }
	 }
      else
	 {
	 }
      break;
      
      case 1:
      if (nbInter == 1)
	 {
	 val = axemin;
	 while (val <= axemax)
	    {
	    c_wglxai(&posX, &posY, xorig, val);
	    c_wglmvi(posX, posY);
	    c_wgldri(posX + 10, posY);

	    c_wglxai(&posX, &posY, ymax, val);
	    c_wglmvi(posX, posY);
	    c_wgldri(posX - 10, posY);

	    val+= inter[0];
	    }
	 }
      else
	 {
	 }
      break;
      }
   }









