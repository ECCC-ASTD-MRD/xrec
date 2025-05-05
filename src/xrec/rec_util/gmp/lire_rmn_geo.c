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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <rmn/rpnmacros.h>
#include "gmp.h"
#include "gdb.h"
#include "rec_util.h"

static ListePointsStruct *gdb_liste;
static int gdb_nbItems;


extern GeoMapOptionsStruct mapOptions;
extern GeoMapFlagsStruct mapFlags;
extern ListePointsStruct *gmp_vecs[];
extern int gmp_nbVecs[];
extern int nbVecsContinents;
extern char *rmn_geo;

void lire_rmn_geo()
{
  float pts[200];
  int deuxCents = 200;
  char nomFichierGeographie[128], nomFichier[128];
  
  int res;
  int n,i, j, nbSeg;
  float xmin, xmax;
  float ymin, ymax;
  float latMin, latMax, lonMin, lonMax;
  int oldNbItems = 0;
  static int gdb_initialized = -1;
  int nbFichiers;
  int nbItems;

  
  int npts, centCinquanteHuit, code;  

  for (i=0; i<127; i++)
    nomFichierGeographie[i] = ' ';
  nomFichierGeographie[127] = '\0';

  for (i=0; i < NMAP_FLAGS; i++)
    {
    if (i != LATLON)
      {
      gmp_nbVecs[i] = 0;
      }
    }
  gmp_nbVecs[LATLON] = 0;
  
  gdb_liste = gmp_vecs[CONTINENTS];
  gdb_nbItems = gmp_nbVecs[CONTINENTS];
  ActiverParamsLigne(mapFlags.style[CONTINENTS], mapFlags.indCouleur[CONTINENTS], mapFlags.epaisseur[CONTINENTS]);
  NewGeoItem(&gdb_liste,&gdb_nbItems);
  for (n=0; n < mapOptions.nbFichiersGeographie; n++)
    {
    strcpy(nomFichierGeographie, rmn_geo);
    strcat(nomFichierGeographie, mapOptions.fichierGeographie[n]);

    centCinquanteHuit = 0;
    f77name(opllfl)(&centCinquanteHuit, nomFichierGeographie, &code, (F2Cl) strlen((char *)nomFichierGeographie));
    
    npts = -1;
    while (npts != 0)
      {
      f77name(rdllfl)(&centCinquanteHuit, &npts, &latMax, &latMin, &lonMax, &lonMin, pts, &deuxCents);
      
      if (latMax < latMin)
	swapFloats(&latMin, &latMax);
      
      if (lonMin < 0.0) lonMin += 360.0;
      if (lonMax < 0.0) lonMax += 360.0;
       if (lonMax < lonMin)
	swapFloats(&lonMin, &lonMax);
      
      res = gmp_perim(&xmin, &ymin, &xmax, &ymax, &latMin, &lonMin, &latMax, &lonMax, &nbSeg);

      if (res)
	{
	gmp_trim(pts, &npts,mapOptions.resolution);
	gmp_convert(gdb_liste, &gdb_nbItems, pts, npts, xmin, ymin, xmax, ymax, nbSeg);
	
	for (i=oldNbItems; i < gdb_nbItems; i++)
	  {
	  switch(mapOptions.styleGeo)
	    {
	    case LIGNE:
	    case TIRET:
	      TracerVecteurs(&(gdb_liste)[i]);
	      break;
	      
	    case POINT:
	      TracerPoints(&(gdb_liste)[i]);
	      break;
	    }
	  }
	NewGeoItem(&gdb_liste,&gdb_nbItems);
	oldNbItems = gdb_nbItems-1;
	}
      }
    f77name(clllfl)(&centCinquanteHuit);
    }
  gmp_vecs[CONTINENTS] = gdb_liste;
  gmp_nbVecs[CONTINENTS] = gdb_nbItems;
}

