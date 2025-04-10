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
#include <gmp.h>
#include "rmn/ezscint.h"
#include "rec_functions.h"

extern GeoMapInfoStruct     mapInfo;

int gmp_perim(float *xgdmin, float *ygdmin,  float *xgdmax,  float *ygdmax, float  *latMin,  float *lonMin, float *latMax, float *lonMax, int *nbSeg)     
{
   int i, gdid, res;
   float XGrid[4], YGrid[4];
   int quatre, un;
   char type;

   un = 1;
   quatre = 4;
   
   gdid = mapInfo.gdid;
   
   c_gdxyzfll(gdid, &XGrid[0], &YGrid[0], latMin, lonMin, 1);
   c_gdxyzfll(gdid, &XGrid[1], &YGrid[1], latMin, lonMax, 1);
   c_gdxyzfll(gdid, &XGrid[2], &YGrid[2], latMax, lonMin, 1);
   c_gdxyzfll(gdid, &XGrid[3], &YGrid[3], latMax, lonMax, 1);
   
   f77name(aminmax)(xgdmin, xgdmax, XGrid, &quatre, &un);
   f77name(aminmax)(ygdmin, ygdmax, YGrid, &quatre, &un);
   

   res = clip(*xgdmin, *ygdmin, *xgdmax, *ygdmax);
   
   *nbSeg = 1;
   return res;
}

