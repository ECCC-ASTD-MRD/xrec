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

AfficherFenetreLatLon(latmin, lonmin, latmax, lonmax, couleurFore, couleurBack)
float latmin, lonmin, latmax, lonmax;
int couleurFore, couleurBack;
{
   int i,j;
   float x,y,lat,lon;



/**
   c_wglcol(couleur);
   c_gmpl2g(&x, &y, latmin, lonmin);
   c_wglmvx(x, y);

   lat = latmin;
   lon = lonmin;
   while (lon <= lonmax)
      {
      c_gmpl2g(&x, &y, lat, lon);
      c_wgldrx(x, y);
      lon += 1.0;
      }

   lat = latmin;
   lon = lonmax;
   while (lat <= latmax)
      {
      c_gmpl2g(&x, &y, lat, lon);
      c_wgldrx(x, y);
      lat += 1.0;
      }

   lat = latmax;
   lon = lonmax;
   while (lon >= lonmin)
      {
      c_gmpl2g(&x, &y, lat, lon);
      c_wgldrx(x, y);
      lon -= 1.0;
      }

   lat = latmax;
   lon = lonmin;
   while (lat >= latmin)
      {
      c_gmpl2g(&x, &y, lat, lon);
      c_wgldrx(x, y);
      lat -= 1.0;
      }

**/


   c_wgllwi(1);
   c_wglxai(&i, &j, 05.0, 58.5);
   DrawBoxedStr("18 km", i, j, 18,couleurBack, couleurFore);

   c_wglxai(&i, &j, 19.0, 52.5);
   DrawBoxedStr("6 km", i, j, 18,couleurBack, couleurFore);

   c_wglxai(&i, &j, 26.0, 49.5);
   DrawBoxedStr("3 km", i, j, 18,couleurBack, couleurFore);

   c_wgllwi(4);
   c_wglcol(couleurFore);
   c_wglrlx(05.0, 15.0, 47.0, 58.0);
   c_wglrlx(19.0, 31.0, 45.0, 52.0);
   c_wglrlx(26.0, 35.0, 41.0, 49.0);

   c_wgllwi(1);
   }





