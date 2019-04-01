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

#include <gmp.h>
#include <rpnmacros.h>
#include <stdio.h>

#include <wgl.h>
#include <string.h>

extern GeoMapOptionsStruct mapOptions;
static ListePointsStruct *gdb_liste;
static int gdb_nbItems;

extern ListeTextStruct *gmp_cities;
extern ListePointsStruct *gmp_vecs[];
extern int gmp_nbVecs[];
extern int gmp_nbCities;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapFlagsStruct   mapFlags;
wordint c_gdllfxyz(wordint gdid, ftnfloat *lat, ftnfloat *lon, ftnfloat *x, ftnfloat *y, wordint n);
void
gmp_gvlatlon(float *vlatmin, float *vlonmin, float *vlatmax, float *vlonmax, int coord)
{
  char *tmp;
  
  int res;
  int n,i, j, nbSeg;
  float xmin, xmax;
  float ymin, ymax;
  float latMin, latMax, lonMin, lonMax;
  int oldNbItems = 0;
  static int gdb_initialized = -1;
  int gdid;
  int npts, vingtCinq, code;
  float lonmin, lonmax, latmin, latmax;
  float x, y, tlat, tlon;
  int idebut, jdebut, ifin, jfin;
  int pixperdegree = 1;

  gdid = mapInfo.gdid;
  gdid = c_ezgetgdout();
  c_wglgvx(&xmin, &ymin, &xmax, &ymax); 
  c_wglxai(&idebut, &jdebut, xmin, ymin);
  c_wglxai(&ifin, &jfin, xmax, ymax);
  c_gdllfxyz(gdid, &tlat, &tlon, &xmin, &ymin, 1);
  if (coord == M180_180 && tlon > 180.0) tlon -= 360.0;
  *vlatmin = tlat;
  *vlatmax = tlat;
  *vlonmin = tlon;
  *vlonmax = tlon;

  j = jdebut;
  while (j <= jfin)
    {
    i = idebut;
    while (i <= ifin)
      {
      c_wgliax(&x, &y, i, j);
      c_gdllfxyz(gdid, &tlat, &tlon, &x, &y, 1);
      if (coord == M180_180 && tlon > 180.0) tlon -= 360.0;
      *vlonmin = tlon < *vlonmin ? tlon : *vlonmin;
      *vlonmax = tlon > *vlonmax ? tlon : *vlonmax;
      *vlatmin = tlat < *vlatmin ? tlat : *vlatmin;
      *vlatmax = tlat > *vlatmax ? tlat : *vlatmax;
      i+=16;
      if (i >= ifin) i = ifin+1;
      }
    j+=16;
    if (j >= jfin) j = jfin+1;
    }

  if (coord == M180_180)
    {
    if (*vlonmin < -170.0) *vlonmin = -180.0;
    if (*vlonmax > 170.0)*vlonmax = 180.0;
    if (*vlonmin > 180.0 && *vlonmax > 180.0)
      {
      *vlonmin -= 360.0;
      *vlonmax -= 360.0;
      }
    }
  else
    {
    if (*vlonmin < 10.0) *vlonmin = 0.0;
    if (*vlonmax > 350.0)*vlonmax = 359.99;
    }

/*  fprintf(stderr, "gv_latlon limits : (%f,%f),(%f,%f)\n", *vlatmin, *vlonmin, *vlatmax, *vlonmax);*/
}
