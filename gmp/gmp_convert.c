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
#include <math.h>
#include <malloc.h>

extern GeoMapInfoStruct     mapInfo;

int gmp_convert(ListePointsStruct *liste, int *nbItems, float pts[], int nbPoints, float xgdmin, float ygdmin, float xgdmax, float ygdmax, int nbSeg)
{
   wordint i, j, n,lastN,base;
   ftnfloat deltaxmax;
   wordint un = 1;
   wordint detected;
   char type;

   ftnfloat xmin,xmax,ymin,ymax;
   ftnfloat x[24000];
   ftnfloat y[24000];
   ftnfloat lats[24000];
   ftnfloat lons[24000];
   wordint npts;
   wordint currentItem;
   wordint gdid;

   npts = nbPoints / 2;
   if (npts > 24000 || npts == 0)
     {      
     fprintf(stderr,"Erreur: npts = %d\n", npts);
     return;
     }

   for (i=0; i < npts; i++)
     {
     lats[i] = pts[i*2];
     }

   for (i=0; i < npts; i++)
     {
     lons[i] = pts[i*2+1];
     lons[i] = fmod(lons[i]+360,360.0);
     }

   currentItem = *nbItems - 1;
   if (mapInfo.type == 'Z')
     {
     type = mapInfo.typeref;
     }
   else
     {
     type = mapInfo.type;
     }
   
   gdid = c_ezgetgdout();
   
   c_gdxyzfll(gdid, x, y, lats, lons, npts);

   f77name(aminmax)(&xmin, &xmax, x, &npts, &un);
   f77name(aminmax)(&ymin, &ymax, y, &npts, &un);
   
   if (npts > 0)
      {
      switch(type)
         {
         case 'N':
         case 'S':
	   detected = 0;
         break;
         
         default:
	   n = 1;
	   detected = 0;
	   while (n < npts && !detected)
	     {
	     if (fabs(x[n] - x[n-1]) > (0.85*mapInfo.ni))
	       {
	       detected = 1;
	       }
	     n++;

	     }
	   break;
         }
      
      if (!detected)
	{
	f77name(aminmax)(&liste[currentItem].xmin,&liste[currentItem].xmax,x,&npts,&un);
	f77name(aminmax)(&liste[currentItem].ymin,&liste[currentItem].ymax,y,&npts,&un);
	if (npts <= 0)
	  {
	  fprintf(stderr,"<gmp_convert: npts negatif! : %d\n", npts);
	  return;
	  }
	liste[currentItem].npts = npts;
	liste[currentItem].statutPRGrille = 1;
	liste[currentItem].pointsGeo = (PointGeoStruct *) calloc((npts), sizeof(PointGeoStruct));
	for (n=0; n < npts; n++)
	  {
	  liste[currentItem].pointsGeo[n].x = x[n];
	  liste[currentItem].pointsGeo[n].y = y[n];
	  }
	}
      else
	{
	liste[currentItem].pointsGeo = (PointGeoStruct *) calloc((npts), sizeof(PointGeoStruct));
	liste[currentItem].xmin = 1.0;
	liste[currentItem].xmax = 1.0;
	liste[currentItem].ymin = 1.0;
	liste[currentItem].ymax = 1.0;
	liste[currentItem].npts = 1;
	}
      }
   }
   


