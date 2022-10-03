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
#include <rmn/rpnmacros.h>
#include <gmp.h>
#include <math.h>
#include <wgl.h>
#include <rec.h>

GeoMapInfoStruct     mapInfo = { ' ', -1, -1, -1, -1, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 };
GeoMapInfoStruct  oldMapInfo = { ' ', -1, -1, -1, -1, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 };
GeoMapInfoStruct nullMapInfo = { ' ', -1, -1, -1, -1, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 };


GeoMapFlagsStruct   mapFlags;
GeoMapOptionsStruct mapOptions;
ListePointsStruct *vecsContinents = NULL;
ListePointsStruct *vecsMeridiens  = NULL;
ListePointsStruct *gmp_vecs[NMAP_FLAGS];
ListeTextStruct   *gmp_cities = NULL;

static int gmp_init_done = 0;

int gmp_nbVecs[NMAP_FLAGS];
int gmp_nbCities = 0;
int nbVecsContinents = 0;
int nbVecsMeridiens = 0;
int gmpFlagInterrupt = TRUE;


int c_gmpinit()
{

  int i;


  if (gmp_init_done == 1)
    return;

  for (i=0; i < NMAP_FLAGS; i++)
    {
    gmp_nbVecs[i] = 0;
    gmp_vecs[i] = NULL;
    }
  
  mapOptions.fichierGeographie[0] = GEOGRHR;
  mapOptions.fichierGeographie[1] = GEOPOLR;
  mapOptions.nbFichiersGeographie = 2;
  mapOptions.intervalleMeridiens = 5.0;
  mapOptions.flagLabel= NON;
  mapOptions.flagPerim = OUI;
  mapOptions.styleGeo = LIGNE;
  mapOptions.couleurGeo = 7;
  mapOptions.epaisseurGeo = 1; 
  mapOptions.styleGeo = LIGNE;
  mapOptions.couleurMer = 7;
  mapOptions.epaisseurMer = 1;
  mapOptions.couleurPerim = 0;
  mapOptions.epaisseurPerim = 2;
  mapOptions.couleurLabel = 7;
  mapOptions.epaisseurLabel = 1; 
  mapOptions.resolution = 0.01;
  
  mapFlags.etat[CONTINENTS] = OUI;
  mapFlags.etat[LATLON] = OUI;
  mapFlags.etat[PAYS] = OUI;
  mapFlags.etat[PROVINCES] = OUI;
  mapFlags.etat[LACS] = OUI;
  
  mapFlags.lu[CONTINENTS] = NON;
  mapFlags.lu[LATLON] = NON;
  mapFlags.lu[RIVIERES] = NON;
  mapFlags.typeValide = NON;
  mapFlags.clipNecessaire = OUI;
  
  mapFlags.verifStatutNecessaire = OUI;
  
  for (i=0; i < NMAP_FLAGS; i++)
    {
    mapFlags.epaisseur[i] = 1;
    mapFlags.style[i] = 0;
    mapFlags.indCouleur[i] = NOIR;
    mapFlags.couleur[i][0] = 0;
    mapFlags.couleur[i][1] = 0;
    mapFlags.couleur[i][2] = 0;
    }
  gmp_init_done = 1;
  
}

