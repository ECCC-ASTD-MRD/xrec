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

static int   ii;
static int   debut, milieu, fin;

#define zlin(x1,x2,dx) ((x1)+((x2)-(x1))*(dx))
#define INTERP(fx,x,t,N) { ii = (int) (x); \
			   ii = (ii < 1 ? 1 : ( ii < N ? ii : N - 1)); \
			   fx = zlin(t[ii],t[ii+1], x-(float)ii); }

#define CHERCHE(x, fx, t, n) { \
			       debut = 1; \
			       fin = n; \
			       milieu = (debut+fin)*0.5; \
			       \
			       while (milieu != debut) \
				  { \
				  if (fx <= t[milieu]) \
				     fin   = milieu; \
				  else \
				     debut = milieu; \
				  \
				  milieu = (debut+fin)*0.5; \
				  } \
			       \
			       x = milieu + (fx - t[milieu])/(t[milieu+1]-t[milieu]); \
			       }

#define CHERCHE2(x, fx, t, n) { \
			       debut = 1; \
			       fin = n; \
			       milieu = (debut+fin)*0.5; \
			       \
			       while (milieu != debut) \
				  { \
				  if (fx >= t[milieu]) \
				     fin   = milieu; \
				  else \
				     debut = milieu; \
				  \
				  milieu = (debut+fin)*0.5; \
				  } \
			       \
			       x = milieu - (fx - t[milieu])/(t[milieu]-t[milieu-1]); \
			       }


void f77name(xsetxy)(int *mode, float *tx, int *nx, float *ty, int *ny)
{
   c_xsetxy(*mode, tx, *nx, ty, *ny);
   }

void c_xsetxy(int mode, float *tx, int nx, float *ty, int ny)
{
   int i,j;

   if ((mode % 2) != 0)
      {
      if (usSpace.axex)
	 free(usSpace.axex);

      usSpace.ni = nx;
      usSpace.axex = (float *)calloc(usSpace.ni+1, sizeof(float));
      for (i=0; i < usSpace.ni; i++)
	 usSpace.axex[i+1] = tx[i];
      }
   else
      {
      if (usSpace.axex)
	 free(usSpace.axex);

      usSpace.axex = NULL;
      }
   
   if ((mode / 2) != 0)
      {
      if (usSpace.axey)
	 free(usSpace.axey);

      usSpace.nj = ny;
      usSpace.axey = (float *)calloc(usSpace.nj+1, sizeof(float));
      for (j=0; j < usSpace.nj; j++)
	 usSpace.axey[j+1] = ty[j];
      }
   else
      {
      if (usSpace.axex)
	 free(usSpace.axey);
      usSpace.axey = NULL;
      }
   
   }


void f77name(xy2fxfy)(float *fx, float *fy, float *x, float *y)
{
   c_xy2fxfy(fx, fy, *x, *y);
   }

void c_xy2fxfy(float *fx, float *fy, float x, float y)
{
   int ix;
   int iy;

   if (usSpace.axex)
      {
      INTERP(*fx, x, usSpace.axex, usSpace.ni)
      }
   else
      {
      *fx = x;
      }
		 
   if (usSpace.axey)
      {
      INTERP(*fy, y, usSpace.axey, usSpace.nj)
      }
   else
      {
      *fy = y;
      }
		 
   
   }

void f77name(vxy2fxfy)(float *fx, float *fy, float *x, float *y, int *npts)
{
   c_vxy2fxfy(fx, fy, x, y, *npts);
   }

void c_vxy2fxfy(float *fx, float *fy, float *x, float *y, int npts)
{
   int i;
   
   if (usSpace.axex)
      {
      for (i=0; i < npts; i++)
	 {
	 INTERP(fx[i], x[i], usSpace.axex, usSpace.ni)
	 }
      }
   else
      {
      memcpy(fx, x, npts * sizeof(float));
      }
		 
   if (usSpace.axey)
      {
      for (i=0; i < npts; i++)
	 {
	 INTERP(fy[i], y[i], usSpace.axey, usSpace.nj)
	 }
      }
   else
      {
      memcpy(fy, y, npts * sizeof(float));
      }
		 
   
   }

void f77name(fxfy2xy)(float *x, float *y, float *fx, float *fy)
{
   c_fxfy2xy(x, y, *fx, *fy);
   }

void c_fxfy2xy(float *x, float *y, float fx, float fy)
{
   if (usSpace.axex)
      {
      if (usSpace.axex[1]< usSpace.axex[usSpace.ni-1])
	 CHERCHE(*x, fx, usSpace.axex, usSpace.ni)
      else
	 CHERCHE2(*x, fx, usSpace.axex, usSpace.ni)
	    
      }
   else
      {
      *x = fx;
      }
		 
   if (usSpace.axey)
      {
      if (usSpace.axey[1]< usSpace.axey[usSpace.nj-1])
	 CHERCHE(*y, fy, usSpace.axey, usSpace.nj)
      else
	 {
	 debut = 1; 
	 fin = usSpace.nj; 
	 milieu = (int)(0.5+(debut+fin)*0.5); 
	 
	 while (fin != milieu) 
	    { 
	    if (fy >= usSpace.axey[milieu]) 
	       fin   = milieu; 
	    else 
	       debut = milieu; 
	    
	    milieu = (int)(0.5+(debut+fin)*0.5); 
	    } 
	 
	 *y = milieu - (fy - usSpace.axey[milieu])/(usSpace.axey[milieu-1]-usSpace.axey[milieu]); 
	 }
      }
   else
      {
      *y = fy;
      }
		 
   
   }

void f77name(vfxfy2xy)(float *x, float *y, float *fx, float *fy, int *npts)
{
   c_vfxfy2xy(x, y, fx, fy, *npts);
   }

void c_vfxfy2xy(float *x, float *y, float *fx, float *fy, int npts)
{
   int i;
   
   if (usSpace.axex)
      {
      for (i=0; i < npts; i++)
	 {
	 if (usSpace.axex[1]< usSpace.axex[usSpace.ni-1])
	    CHERCHE(x[i], fx[i], usSpace.axex, usSpace.ni)
	 else
	    CHERCHE2(x[i], fx[i], usSpace.axex, usSpace.ni)
	 }
      }
   else
      {
      memcpy(x, fx, npts * sizeof(float));
      }
		 
   if (usSpace.axey)
      {
      for (i=0; i < npts; i++)
	 {
	 if (usSpace.axey[1]< usSpace.axey[1])
	    CHERCHE(y[i], fy[i], usSpace.axey, usSpace.nj)
	 else
	    CHERCHE2(y[i], fy[i], usSpace.axey, usSpace.nj)
	 }
      }
   else
      {
      memcpy(y, fy, npts * sizeof(float));
      }
		 
   
   }

