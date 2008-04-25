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


void f77name(xsetxy)(int *mapping, float *tx, int *nx, float *ty, int *ny)
{
   c_xsetxy(*mapping, tx, *nx, ty, *ny);
   }

void c_xsetxy(int mapping, float *tx, int nx, float *ty, int ny)
{
   int i,j;

   usSpace.mapping = mapping;
   switch(mapping)
    {
    case 1:
    case 3:
    if (usSpace.axex) free(usSpace.axex);

    usSpace.ni = nx;
    usSpace.axex = (float *)calloc(usSpace.ni+1, sizeof(float));
    for (i=0; i < usSpace.ni; i++)
      {
      usSpace.axex[i+1] = tx[i];
      }
    break;

    case 0:
    case 2:
    if (usSpace.axex) free(usSpace.axex);
    usSpace.axex = NULL;
    break;

    }

   switch (mapping)
    {
    case 2:
    case 3:
    usSpace.nj = ny;
    if (usSpace.axey) free(usSpace.axey);

    usSpace.axey = (float *)calloc(usSpace.nj+1, sizeof(float));
    for (j=0; j < usSpace.nj; j++)
      {
      usSpace.axey[j+1] = ty[j];
      }
    break;

    case 0:
    case 1:
    if (usSpace.axey) free(usSpace.axey);
    usSpace.axey = NULL;
    break;
    }

  switch (mapping)
    {
    case 5:
    if (usSpace.z) free(usSpace.z);
    usSpace.ni = nx;
    usSpace.nj = ny;
    usSpace.z = (float *)malloc(nx*ny*sizeof(float));
    memcpy(usSpace.z, ty, sizeof(float)*nx*ny);
    if (usSpace.axey) free(usSpace.axey);

    usSpace.axey = (float *)calloc(usSpace.nj+1, sizeof(float));
    for (j=0; j < usSpace.nj; j++)
      {
      usSpace.axey[j+1] = ty[usSpace.ni*j];
      }
    break;
    }

  switch (mapping)
    {
    case 6:
    if (usSpace.zx) free(usSpace.zx);
    if (usSpace.zx) free(usSpace.zy);
    usSpace.ni = nx;
    usSpace.nj = ny;
    usSpace.zx = (float *)malloc(nx*ny*sizeof(float));
    usSpace.zy = (float *)malloc(nx*ny*sizeof(float));
    usSpace.azy = (float *)malloc(ny*sizeof(float));
    memcpy(usSpace.zx, tx, sizeof(float)*nx*ny);
    memcpy(usSpace.zy, ty, sizeof(float)*nx*ny);
    usSpace.axex = (float *)calloc(usSpace.ni+1, sizeof(float));
    for (i=0; i < usSpace.ni; i++)
      {
      usSpace.axex[i+1] = i * 360.0/nx;
      }
    usSpace.axey = (float *)calloc(usSpace.nj+1, sizeof(float));
    for (j=0; j < usSpace.nj; j++)
      {
      usSpace.axey[j+1] = -90.0 + j * 180.0/ny;
      }

    for (j=0; j < usSpace.nj; j++)
      {
      usSpace.azy[j] = ty[j*nx];
      for (i=0; i < nx; i++)
         {
         if (usSpace.azy[j] < ty[j*nx+i]) usSpace.azy[j] = ty[j*nx+i];
         }
      printf("j, azy[j]: %d %f\n",j,usSpace.azy[j]);
      }

    break;

    default:
    break;
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
  int un = 1;
  float x_x, y_y;

  x_x = x;
  y_y = y;

  switch (usSpace.mapping)
    {
    case 5:
    *fx = x;
    f77name(ez_rgdint_1_nw)(fy,&x_x,&y_y, &un, usSpace.z, &usSpace.ni,&un, &usSpace.nj);
    break;

    case 6:
    f77name(ez_rgdint_1_nw)(fx,&x_x,&y_y, &un, usSpace.zx, &usSpace.ni,&un, &usSpace.nj);
    f77name(ez_rgdint_1_nw)(fy,&x_x,&y_y, &un, usSpace.zy, &usSpace.ni,&un, &usSpace.nj);
    break;

    default:
    if (usSpace.axex)
      {
      INTERP(*fx, x, usSpace.axex, usSpace.ni)
      }
    else
      {
      *fx = x_x;
      }

    if (usSpace.axey)
      {
      INTERP(*fy, y_y, usSpace.axey, usSpace.nj)
      }
    else
      {
      *fy = y_y;
      }
     break;
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
   float xx, yy;
   int i,j,found;
   if (usSpace.mapping == 6)
      {
      found = 0;
      i = 0;
      while (!found && i < (usSpace.ni-1))
         {
         if (fx > usSpace.zx[i] && fx <= usSpace.zx[i+1])
            {
            found = 1;
            *x = i * 1.0 + (fx - usSpace.zx[i])/(usSpace.zx[i+1]-usSpace.zx[i]);
            }
         else
            {
            i++;
            }
         }
      if (found == 0)
         {
         if (fx < 0.0)
            {
            *x = 1.0;
            }
         if (fx > 360.0)
            {
            *x = 1.0*usSpace.ni;
            }
         }

      found = 0;
      j = 0;
      if (fy < usSpace.zy[0])
         {
         *y = 1.0;
         found = 1;
         }
      if (fy > usSpace.zy[usSpace.nj-1])
         {
         *y = 1.0 * usSpace.nj;
         found = 1;
         }
      while (!found && j < (usSpace.nj-1))
         {
         if (fy > usSpace.azy[j] && fy <= usSpace.azy[j])
            {
            found = 1;
            *y = j * 1.0 + (fy - usSpace.azy[j])/(usSpace.azy[j+1]-usSpace.azy[j]);
            }
         else
            {
            j++;
            }
         }
      if (found == 0) *y = fy;


      return;
      }

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
        {
        CHERCHE(*y, fy, usSpace.axey, usSpace.nj)
        }
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

/*      if (usSpace.mapping == 5)
        {
        xx = *x;
        yy = *y;
        c_xy2fxfy(x,y,xx,yy);
        yy = *y;
        CHERCHE(*y, yy, usSpace.axey, usSpace.nj)

        }  */

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

