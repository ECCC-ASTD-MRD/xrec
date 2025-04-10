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
#include <rmn/rpnmacros.h>
#include <rmn/ezscint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "rec_functions.h"

extern GeoMapInfoStruct     mapInfo;

void gmp_convert(ListePointsStruct *liste, int *nbItems, float pts[], int nbPoints, float xgdmin, float ygdmin, float xgdmax, float ygdmax, int nbSeg)
{
   int32_t i, j, n,lastN,base;
   float deltaxmax;
   int32_t un = 1;
   int32_t detected;
   char type;

   float xmin,xmax,ymin,ymax,limite,correction;
   float x[24000];
   float y[24000];
   float lats[24000];
   float lons[24000];
   int32_t npts, nleft, nright;
   int32_t currentItem;
   int32_t gdid;
   float ldiff, fni, fmiddle;

   npts = nbPoints / 2;
   if (npts > 24000 || npts == 0)
     {      
     fprintf(stderr,"Erreur: npts = %d\n", npts);
     return;
     }

   for (i=0; i < npts; i++)
     {
     lats[i] = pts[i*2];
     lons[i] = pts[i*2+1]; 
     if (lons[i] < 0.0) lons[i] += 360.0;
	 /*      lons[i] = fmod(lons[i]+360,360.0); */
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

   f77name(aminmax)(&liste[currentItem].xmin,&liste[currentItem].xmax,x,&npts,&un);
   f77name(aminmax)(&liste[currentItem].ymin,&liste[currentItem].ymax,y,&npts,&un);
   if (npts <= 0)
     {
     fprintf(stderr,"<gmp_convert: npts negatif! : %d\n", npts);
     return;
     }
   liste[currentItem].npts = npts;
   liste[currentItem].statutPRGrille = 1;
   liste[currentItem].full_line = 1;
   liste[currentItem].pointsGeo = (PointGeoStruct *) calloc((npts), sizeof(PointGeoStruct));
   liste[currentItem].pen = (unsigned char *) calloc((npts), sizeof(unsigned char));
   for (n=0; n < npts; n++)
     {
     liste[currentItem].pointsGeo[n].x = x[n];
     liste[currentItem].pointsGeo[n].y = y[n];
     liste[currentItem].pen[n] = (unsigned char)1;
     }

   
   if (npts > 0)
     {
     switch(type)
       {
       case 'N':
       case 'S':
       case 'T':
		 case 'U':
       case '!':
	 detected = 0;
	 break;
	 
       default:
	 if (mapInfo.type == 'Z')
	   {
	   correction = 360.0;
	   limite = 0.850 * 360.0;
	   }
	 else
	   {
	   correction = 1.0 * mapInfo.ni;
	   limite = 0.85 * mapInfo.ni;
	   }
	 if ((liste[currentItem].xmax - liste[currentItem].xmin) > limite)
	   {
	   detected = 1;
	   }
       }
     }


   if (detected == 1)
     {
     liste[currentItem].full_line = 0;
     for (n=1; n < npts; n++)
       {
       ldiff = x[n] - liste[currentItem].pointsGeo[n-1].x;
       if (limite < fabs(ldiff))
	 {
	 liste[currentItem].pen[n] = 0;
	 }
       }
     }

   /*     NewGeoItem(&liste,nbItems);
	  currentItem = *nbItems - 1;
	  liste[currentItem].npts = npts;
	  liste[currentItem].statutPRGrille = 1;
	  liste[currentItem].pointsGeo = (PointGeoStruct *) calloc((npts), sizeof(PointGeoStruct));
     for (n=0; n < npts; n++)
       {
	 liste[currentItem].pointsGeo[n].x = liste[currentItem-1].pointsGeo[n].x + correction;
	 liste[currentItem].pointsGeo[n].y = liste[currentItem-1].pointsGeo[n].y;
       }
     
     liste[currentItem].xmin = liste[currentItem-1].xmin + correction;
     liste[currentItem].xmax = liste[currentItem-1].xmax + correction;
     liste[currentItem].ymin = liste[currentItem-1].ymin;
     liste[currentItem].ymax = liste[currentItem-1].ymax;
     */
   
   
}
