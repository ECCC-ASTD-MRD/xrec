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

extern GeoMapOptionsStruct mapOptions;



LireFichierGeographie(ListePointsStruct *(*liste), int *nbItems, char *nomFichier[], int nbFichiers)
{
   float pts[200];
   int deuxCents = 200;
   char nomFichierGeographie[128];
   char *tmp;

   int res;
   int n,i, j, nbSeg;
   float xmin, xmax;
   float ymin, ymax;
   float latMin, latMax, lonMin, lonMax;
   int oldNbItems = 0;
   
   int npts, vingtCinq, code;
   
   NewGeoItem(liste,nbItems);
   
   tmp = (char *) getenv("DCW");

   if (tmp == NULL)
      {
      tmp = (char *) getenv("ARMNLIB");
      
      if (tmp == NULL)
	 {
	 printf("La valeur de la variable d'environnement ARMNLIB est inconnue... \nImpossible de continuer.\n");
	 exit(-1);
	 }
      }
   
   for (n=0; n < nbFichiers; n++)
      {
      strcpy(nomFichierGeographie, tmp);
      strcat(nomFichierGeographie, nomFichier[n]);
      vingtCinq = 0;
      f77name(opllfl)(&vingtCinq, nomFichierGeographie, &code, strlen(nomFichier));
      
      npts = -1;
      while (npts != 0)
	 {
	 f77name(rdllfl)(&vingtCinq, &npts, &latMax, &latMin, &lonMax, &lonMin, pts, &deuxCents);

	 if (latMax < latMin)
	    swapFloats(&latMin, &latMax);
	 
	 if (lonMax < lonMin)
	    swapFloats(&lonMin, &lonMax);

	 res = gmp_perim(&xmin, &ymin, &xmax, &ymax, &latMin, &lonMin, &latMax, &lonMax, &nbSeg);
	 if (res)
	    {
            gmp_trim(pts, &npts,mapOptions.resolution);
            gmp_convert(liste, nbItems, pts, npts, xmin, ymin, xmax, ymax, nbSeg);
            
            for (i=oldNbItems; i < *nbItems; i++)
               {
               switch(mapOptions.styleGeo)
                  {
                  case LIGNE:
                  case TIRET:
                  TracerVecteurs(&(*liste)[i]);
                  break;
                  
                  case POINT:
                  TracerPoints(&(*liste)[i]);
                  break;
                  }
               }
            NewGeoItem(liste,nbItems);
            oldNbItems = *nbItems-1;
            }
	 }
      f77name(clllfl)(&vingtCinq);
      }
   }

/**
 ****************************************************************************
 ****************************************************************************
 **/

