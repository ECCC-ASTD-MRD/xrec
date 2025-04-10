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
#include <gmp.h>
#include <rmn/rpnmacros.h>

extern float gdxmin, gdymin, gdxmax, gdymax;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapInfoStruct     oldMapInfo;

extern GeoMapFlagsStruct    mapFlags;
extern GeoMapOptionsStruct mapOptions;
extern int gmpFlagInterrupt;

int c_gmpopts(char *option, char *valeur)
{
  int valeurInt;
  float valeurFloat;
  
  if (0 == strcmp(option, "OUTLINE"))
    {
    if (mapOptions.resolution < 0.1)
      {
      if (0 == strcmp(valeur, "GEO"))
	{
	mapOptions.fichierGeographie[0] = GEOGRHR;
	mapOptions.fichierGeographie[1] = NULL;
	mapOptions.nbFichiersGeographie = 1;
	mapFlags.lu[CONTINENTS] = NON;
	mapFlags.lu[LATLON] = NON;
	}
      
      if (0 == strcmp(valeur, "GEOPOL"))
	{
	mapOptions.fichierGeographie[0] = GEOGRHR;
	mapOptions.fichierGeographie[1] = GEOPOHR;
	mapOptions.nbFichiersGeographie = 2;
	mapFlags.lu[CONTINENTS] = NON;
	mapFlags.lu[LATLON] = NON;
	}
      }
    else
      {
      if (0 == strcmp(valeur, "GEO"))
	{
	mapOptions.fichierGeographie[0] = GEOGRLR;
	mapOptions.fichierGeographie[1] = NULL;
	mapOptions.nbFichiersGeographie = 1;
	mapFlags.lu[CONTINENTS] = NON;
	mapFlags.lu[LATLON] = NON;
	}
      
      if (0 == strcmp(valeur, "GEOPOL"))
	{
	mapOptions.fichierGeographie[0] = GEOPOLR;
	mapOptions.fichierGeographie[1] = NULL;
	mapOptions.nbFichiersGeographie = 1;
	mapFlags.lu[CONTINENTS] = NON;
	mapFlags.lu[LATLON] = NON;
	}
      }
    
    return 1;
    }
  
  if (0 == strcmp(option, "MERIDIENS"))
    {
    if (0 == strcmp(valeur, "OUI"))
      mapFlags.etat[LATLON] = OUI;
    else
      mapFlags.etat[LATLON] = NON;
    return 1;
    }
  
  if (0 == strcmp(option, "LABEL"))
    {
    if (0 == strcmp(valeur, "OUI"))
      {
      mapOptions.flagLabel = OUI;
      return 1;
      }
    
    if (0 == strcmp(valeur, "NON"))
      {
      mapOptions.flagLabel = NON;
      return 1;
      }
    return -1;
    }
  
  if (0 == strcmp(option, "PERIM"))
    {
    if (0 == strcmp(valeur, "OUI"))
      {
      mapOptions.flagPerim = OUI;
      return 1;
      }
    
    if (0 == strcmp(valeur, "NON"))
      {
      mapOptions.flagPerim = NON;
      return 1;
      }
    return -1;
    }
  
  if (0 == strcmp(option, "OUTLINE_STYLE"))
    {
    if (0 == strcmp("LINE", valeur))
      {
      mapOptions.styleGeo = LIGNE;
      return 1;
      }
    
    if (0 == strcmp("DASH", valeur))
      {
      mapOptions.styleGeo = TIRET;
      return 1;
      }
    
    if (0 == strcmp("POINT", valeur))
      {
      mapOptions.styleGeo = POINT;
      return 1;
      }
    return -1;
    }
  
  if (0 == strcmp(option, "GRID_STYLE"))
    {
    if (0 == strcmp("LINE", valeur))
      {
      mapOptions.styleMer = LIGNE;
      return 1;
      }
    
    if (0 == strcmp("DASH", valeur))
      {
      mapOptions.styleMer = TIRET;
      return 1;
      }
    
    if (0 == strcmp("POINT", valeur))
      {
      mapOptions.styleMer = POINT;
      return 1;
      }
    return -1;
    }
  
  return -1;
}


int c_gmpopti(char *option, int32_t valeur)
{
  int valeurInt;
  float valeurFloat;
  
  if (0 == strcmp(option, "GRID"))
    {
    mapOptions.intervalleMeridiens = (float) (int) valeur;
    mapFlags.lu[LATLON]= NON;
    mapFlags.lu[CONTINENTS]= NON;
    if (mapOptions.intervalleMeridiens == 0.0)
         mapFlags.etat[LATLON] = NON;
    return 1;
    }
  
  if (0 == strcmp(option, "RESOLUTION"))
    {
    mapOptions.resolution = (float)(0.01 * (int)valeur);
    mapFlags.lu[CONTINENTS] = NON;
    mapFlags.lu[LATLON] = NON;
    if ((0 == strcmp(mapOptions.fichierGeographie[0], GEOPOLR)) || mapOptions.fichierGeographie[1])
      c_gmpopts("OUTLINE", "GEOPOL");
    else
      c_gmpopts("OUTLINE", "GEO");
    return 1;
    }
  
  if (0 == strcmp(option, "OUTLINE_COLOR"))
    {
    mapOptions.couleurGeo = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "OUTLINE_THICK"))
    {
    mapOptions.epaisseurGeo = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "GRID_COLOR"))
    {
    mapOptions.couleurMer = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "GRID_THICK"))
    {
    mapOptions.epaisseurMer = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "PERIM_COLOR"))
    {
    mapOptions.couleurPerim = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "PERIM_THICK"))
    {
    mapOptions.epaisseurPerim = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "LABEL_COLOR"))
    {
    mapOptions.couleurLabel = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "LABEL_THICK"))
    {
    mapOptions.epaisseurLabel = (int) valeur;
    return 1;
    }
  
  if (0 == strcmp(option, "ACCEPT_INTERRUPTS"))
    {
    gmpFlagInterrupt = (int) valeur;
    return 1;
    }
  
  return -1;
}

