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

int gmp_convert(ListePointsStruct *(*liste), int *nbItems, float pts[], int nbPoints, float xgdmin, float ygdmin, float xgdmax, float ygdmax, int nbSeg)
{
   wordint i, j, n,lastN,base;
   ftnfloat deltaxmax;
   wordint un = 1;
   wordint detected;
   char type;

   ftnfloat xmin,xmax,ymin,ymax;
   ftnfloat x[400];
   ftnfloat y[400];
   ftnfloat lats[400];
   ftnfloat lons[400];
   wordint npts;
   wordint currentItem;
   wordint gdid;

   npts = nbPoints / 2;
   if (npts > 400 || npts == 0)
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
   
   c_gdxyfll(gdid, x, y, lats, lons, npts);
   c_vxy2fxfy(x, y, x, y, npts);

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
		
	   /*	   if (detected == 1)
	     {
	     printf("Probleme detecte!\n");
	     }
	   */
	   break;
         }
      
      if (!detected)
	{
	f77name(aminmax)(&liste[0][currentItem].xmin,&liste[0][currentItem].xmax,x,&npts,&un);
	f77name(aminmax)(&liste[0][currentItem].ymin,&liste[0][currentItem].ymax,y,&npts,&un);
	if (npts <= 0)
	  {
	  fprintf(stderr,"<gmp_convert: npts negatif! : %d\n", npts);
	  return;
	  }
	liste[0][currentItem].npts = npts;
	liste[0][currentItem].statutPRGrille = 1;
	liste[0][currentItem].pointsGeo = (PointGeoStruct *) calloc((npts), sizeof(PointGeoStruct));
	for (n=0; n < npts; n++)
	  {
	  liste[0][currentItem].pointsGeo[n].x = x[n];
	  liste[0][currentItem].pointsGeo[n].y = y[n];
	  }
	}
      else
	{
	liste[0][currentItem].pointsGeo = (PointGeoStruct *) calloc((npts), sizeof(PointGeoStruct));
	liste[0][currentItem].xmin = 1.0;
	liste[0][currentItem].xmax = 1.0;
	liste[0][currentItem].ymin = 1.0;
	liste[0][currentItem].ymax = 1.0;
	liste[0][currentItem].npts = 1;
	}
      }
   }
   


	   /*         while (n < npts)
		      if (fabs(x[n] - x[n-1]) > (0.95*mapInfo.ni))
		      {
		      liste[0][currentItem].npts = n;
		      f77name(aminmax)(&liste[0][currentItem].xmin,&liste[0][currentItem].xmax,x,&liste[0][currentItem].npts,&un);
		      f77name(aminmax)(&liste[0][currentItem].ymin,&liste[0][currentItem].ymax,y,&liste[0][currentItem].npts,&un);
		      liste[0][currentItem].pointsGeo = (PointGeoStruct *) calloc(liste[0][currentItem].npts, sizeof(PointGeoStruct));
		      liste[0][currentItem].statutPRGrille = 1;
		      for (j=0; j < n; j++)
		      {
		      liste[0][currentItem].pointsGeo[j].x = x[j];
		      liste[0][currentItem].pointsGeo[j].y = y[j];
		      }
		      
		      for (j=n; j < npts; j++)
		      {
		      x[j-n] = x[j];
		      y[j-n] = y[j];
		      }
		      npts -= n;
		      n = 0;
		      NewGeoItem(liste,nbItems);
		      currentItem = *nbItems - 1;
		      }
		      n++;
		      }
	   */
