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

#define F2C(i,j,ni) (ni*(j-1)+(i-1))
#define ZLIN(z1,z2,dx) ((z1) + ((z2)-(z1))*(dx))

i2rgdint(zout,x,y,npts,z,ni,nj)
float *zout, *x, *y;
int npts;
unsigned short *z;
int ni,nj;
{
   int i,j,n;
   float dx,dy;
   int k11,k12,k21,k22;
   float z11,z12,z21,z22;
   float y1,y2,y3;
   
   for (n=0; n < npts; n++)
      {
      i = (int)x[n];
      j = (int)y[n];

      dx = x[n]-i;
      dy = y[n]-j;

      k11 = F2C(i,j,ni);
      k12 = F2C(i,j+1,ni);
      k21 = F2C(i+1,j,ni);
      k22 = F2C(i+1,j+1,ni);

      z11 = (float)z[k11];
      z12 = (float)z[k12];
      z21 = (float)z[k21];
      z22 = (float)z[k22];

      y1  = z11 + (z21-z11)*dx;
      y2  = z21 + (z22-z21)*dx;
      zout[n] = y1 + (y2 - y1)*dy;
      }

   }
