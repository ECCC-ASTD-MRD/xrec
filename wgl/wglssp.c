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

void c_wglssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);

void f77name(wglssp)(float *xdebut, float *ydebut, float *xfin, float *yfin, 
		     int *idebut, int *jdebut, int *ifin, int *jfin, int *mode)
{
  c_wglssp(*xdebut, *ydebut, *xfin, *yfin, *idebut, *jdebut, *ifin, *jfin, *mode);
}

/**
******
**/

void c_wglssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode)
{
  usSpace.idebut = idebut;
  usSpace.jdebut = jdebut;
  usSpace.ifin   = ifin;
  usSpace.jfin   = jfin;
  usSpace.mode   = mode;
  
  switch (mode)
    {
    case 0:
    case 1:
    case 2:
      usSpace.xdebut = xdebut;
      usSpace.xfin   = xfin;
      break;
      
    case 3:
    case 4:
      wglForceAspect = 0;
      usSpace.xdebut = LE_LOG(xdebut);
      usSpace.xfin = LE_LOG(xfin);
      break;
    }
  
  switch(mode)
    {
    case 0:
    case 1:
    case 3:
      usSpace.ydebut = ydebut;
      usSpace.yfin   = yfin;
      break;
      
    case 2:
    case 4:
      wglForceAspect = 0;
      usSpace.ydebut = LE_LOG(ydebut);
      usSpace.yfin = LE_LOG(yfin);
      break;
    }
  
  c_wglgwz(&w,&h);
  
  if ((idebut == 0) && (jdebut == 0) && (ifin == 0) & (jfin == 0))
    {
      usSpace.ifin = w  - 1;
      usSpace.jfin = h - 1;
    }
  
  usSpace.densiteX = (float)(usSpace.ifin - usSpace.idebut) / (usSpace.xfin - usSpace.xdebut);
  usSpace.densiteY = (float)(usSpace.jfin - usSpace.jdebut) / (usSpace.yfin - usSpace.ydebut);
  
  if (wglForceAspect)
    {
      if (usSpace.densiteX < usSpace.densiteY)
	{
	  usSpace.densiteY = usSpace.densiteX;
	}
      else
	{
	  usSpace.densiteX = usSpace.densiteY;
	}
      
      if (usSpace.ifin == (w - 1) && (usSpace.jfin == (h - 1)))
	c_wglxai(&usSpace.ifin, &usSpace.jfin, xfin, yfin);
    }
  
}

