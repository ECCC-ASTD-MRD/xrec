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
#include <gdb.h>
#include <rec.h>

extern float gdxmin, gdymin, gdxmax, gdymax;
extern float gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax;
extern float old_gmp_xmin, old_gmp_ymin, old_gmp_xmax, old_gmp_ymax;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapInfoStruct     oldMapInfo;

extern GeoMapFlagsStruct    mapFlags;
extern GeoMapOptionsStruct mapOptions;

extern ListePointsStruct *vecsContinents;
extern ListePointsStruct *vecsMeridiens;
extern ListePointsStruct *gmp_vecs[];

extern ListeTextStruct *gmp_cities;
extern int gmp_nbCities;

extern int gmp_nbVecs[];
extern int nbVecsContinents;
extern int nbVecsMeridiens;
extern int gmpFlagInterrupt;
extern _XContour xc;

void c_gmpdrw();

f77name(gmpdrw)()
{
   c_gmpdrw();
}

void c_gmpdrw()
{
  int i,n;

  if (mapFlags.typeValide == NON)
    return;
  
  
  c_wglgvx(&gmp_xmin, &gmp_ymin, &gmp_xmax, &gmp_ymax);
  if (mapFlags.verifStatutNecessaire == NON && gmp_xmin == old_gmp_xmin && gmp_xmax == old_gmp_xmax && gmp_ymin == old_gmp_ymin && gmp_ymax == old_gmp_ymax)
    {
    mapFlags.verifStatutNecessaire = NON;
    }
  else
    {
    mapFlags.verifStatutNecessaire = OUI;
    }
  
  if (mapFlags.verifStatutNecessaire == OUI)
    {
    LibererCarte(&(gmp_vecs[CONTINENTS]), &gmp_nbVecs[CONTINENTS]);
    LibererCarte(&(gmp_vecs[PAYS]), &gmp_nbVecs[PAYS]);
    LibererCarte(&(gmp_vecs[PROVINCES]), &gmp_nbVecs[PROVINCES]);
    LibererCarte(&(gmp_vecs[LATLON]), &gmp_nbVecs[LATLON]);
    lire_geo();
    }
  else
    {
    for (i=0; i < 8; i++)
      {
      if (mapFlags.etat[i] == OUI)
	{
	switch (i)
	  {
	  case VILLES:
	    for (n=0; n < gmp_nbCities; n++)
	      {
	      c_gmpDrawCityName(gmp_cities[n].x, gmp_cities[n].y, (char *)gmp_cities[n].text);
	      }
	    /* absence de break intentionnelle */
	    
	  default:
	    ActiverParamsLigne(mapFlags.style[i], mapFlags.indCouleur[i], mapFlags.epaisseur[i]);
	    AfficherVecteurs(gmp_vecs[i], gmp_nbVecs[i], mapFlags.style[i], mapFlags.indCouleur[i], mapFlags.epaisseur[i]);
	    break;
	    
	  }
	
	}
      }
    }
      
  
  old_gmp_xmin = gmp_xmin;
  old_gmp_xmax = gmp_xmax;
  old_gmp_ymin = gmp_ymin;
  old_gmp_ymax = gmp_ymax;
  mapFlags.verifStatutNecessaire = NON;
}


