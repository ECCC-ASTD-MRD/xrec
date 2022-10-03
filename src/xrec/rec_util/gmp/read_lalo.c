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

#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <gmp.h>
#include <rmn/rpnmacros.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>


extern GeoMapInfoStruct     mapInfo;
extern GeoMapInfoStruct     oldMapInfo;

extern GeoMapFlagsStruct    mapFlags;
extern GeoMapOptionsStruct mapOptions;


LireLatLon(ListePointsStruct *(*liste), int *nbItems)
{
   FILE *FichierEntree;
   float pts[20480];
   
   int i, j, n, res, nbSeg;
   int lectureNonTerminee = 1;
   float xmin, xmax;
   float ymin, ymax;
   float latMin, latMax, lonMin, lonMax, lonMaxTemp;
   float lalo[4];
   int npts;
   float temp;
   char *tmp;
   float lat, lon, lat1, lon1;
   float start,end;
   int oldNbItems = 0;

   char  nomFichierLatLon[128];
   char  ident[8];
   int gdid;
   int idebut, jdebut, ifin, jfin;
   float x, y;
   float tlat, tlon,gdlonmin,gdlonmax;
   float vlatmin, vlonmin, vlatmax, vlonmax;
   
   NewGeoItem(liste,nbItems); 

   tmp = (char *) getenv("ARMNLIB");
   if (tmp == NULL)
      {
      printf("La valeur de la variable d'environnement ARMNLIB est inconnue... \nImpossible de continuer.\n");
      exit(-1);
      }
   
   strcpy(nomFichierLatLon, tmp);
   strcat(nomFichierLatLon, ZZZLALO);
   FichierEntree = fopen(nomFichierLatLon, "r");
   gmp_gvlatlon(&vlatmin, &vlonmin, &vlatmax, &vlonmax, ZERO_360);
   
   while (feof(FichierEntree) == 0 && (0 < fscanf(FichierEntree, "%f %f %f %s", &lalo[0], &lalo[1], &lalo[2], ident)) &&
          ((0 == strcmp(ident, "LA")) || (0 == strcmp(ident, "LO"))))
      {
      if (0 == strcmp ("LA", ident))
         {
         latMin = lalo[2];
         latMax = lalo[2];
         lonMin = lalo[0];
         lonMax = lalo[1];

         if (lonMax < lonMin) 
	   {
	   swapFloats(&lonMin, &lonMax); 
	   }
	 
	 if (lonMin < 0.0) 
	   {
	   lonMin = lonMin + 360.0;
	   lonMax = lonMax + 360.0;
	   }
	 }
      
      if (0 == strcmp ("LO", ident))
	{
	latMin = lalo[0];
	latMax = lalo[1];
	lonMin = lalo[2];
	lonMax = lalo[2];
	
	 if (lonMin < 0.0) 
	   {
	   lonMin = lonMin + 360.0;
	   lonMax = lonMax + 360.0;
	   }

	if (latMax < latMin)
	  swapFloats(&latMin, &latMax);
	lat1 = latMin;
	}
      
      
      if (0 == strcmp ("LA", ident))
	{
	start = lonMin;
	end = lonMax;
	}
      else
	{
	start = latMin;
         end = latMax;
	}
      
      if (vlatmin > latMax) end = start+10;
      if (vlatmax < latMin) end = start+10;
      if (vlonmin > lonMax) end = start+10;
      if (vlonmax < lonMin) end = start+10;

      while (start <= end)
	{
	if (start >= 360.0 && end >= 360.0)
	  {
	  start -= 360.0;
	  end-=360.0;
	  }
	if (0 == strcmp ("LA", ident))
	  {
	  lonMin = start;
	  lonMax = lonMin + 10.0;
	  if (lonMax > end) lonMax = end;
	  }
	else
	  {
	  latMin = start;
	  latMax = latMin + 10.0;
	  if (latMax > end) latMax = end;
	  }
	
	nbSeg = 1;
	if (latMax < 99.0 && lonMax < 999.0)
	  {
	  res = gmp_perim(&xmin, &ymin, &xmax, &ymax, &latMin, &lonMin, &latMax, &lonMax, &nbSeg);
	  }
	else
	  {
	  res = 0;
	  }
	
	npts = 0;
	
	if (res == 1)
	  {
	  if (0 == strcmp ("LA", ident) && (0 == (abs((int)latMin) % (int)mapOptions.intervalleMeridiens)))
	    {
	    lon = lonMin;
	    n = 0;
	    while (lon <= lonMax)
	      {
	      pts[n] = latMin;
	      pts[n+1] = lon;
	      lon += 1.0;
	      n+=2;
	      }
	    
	    if (lon != lonMax)
	      {
	      pts[n] = latMin;
	      pts[n+1] = lonMax;
	      }
	    npts = n + 2;
	    gmp_convert(*liste, nbItems, pts, npts, xmin, ymin, xmax, ymax, nbSeg);
	    NewGeoItem(liste,nbItems);
	    }
	  
	  
	  if (0 == strcmp ("LO", ident) && (0 == (abs((int)lonMin) % (int)mapOptions.intervalleMeridiens)))
	    {
	    lat = latMin;
	    n = 0;
	    while (lat <= latMax)
	      {
	      pts[n] = lat;
	      pts[n+1] = lonMin;
	      lat += 1.0;
	      n+=2;
	      }
	    
	    if (lat != latMax)
	      {
	      pts[n] = latMax;
	      pts[n+1] = lonMax;
	      }
	    
	    npts = n + 2;
	    gmp_convert(*liste, nbItems, pts, npts, xmin, ymin, xmax, ymax, nbSeg);
	    NewGeoItem(liste,nbItems);
	    }         
	  }
	start += 10.0;
	}
      
      for (n=oldNbItems; n < *nbItems; n++)
	{
	switch(mapOptions.styleGeo)
	  {
	  case LIGNE:
	  case TIRET:
            TracerVecteurs(&(*liste)[n]);
            break;
            
	  case POINT:
            TracerPoints(&(*liste)[n]);
            break;
	  }
	}
      oldNbItems = *nbItems-1;
      }
   fclose(FichierEntree);
   
}


/*

 */
