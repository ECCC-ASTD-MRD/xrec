#include <gmp.h>
#include <rpnmacros.h>
#include <stdio.h>
#include "../include/gdb.h"

extern GeoMapOptionsStruct mapOptions;
static ListePointsStruct *gdb_liste;
static int gdb_nbItems;

extern ListeTextStruct *gmp_cities;
extern ListePointsStruct *gmp_vecs[];
extern int gmp_nbVecs[];
extern int gmp_nbCities;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapFlagsStruct   mapFlags;

void lire_rmn_latlon()
{
  if (mapFlags.etat[LATLON] == OUI)
    {
    ActiverParamsLigne(mapFlags.style[LATLON], mapFlags.indCouleur[LATLON], 
		       mapFlags.epaisseur[LATLON]);
    LireLatLon(&(gmp_vecs[LATLON]), &gmp_nbVecs[LATLON]);
    }
}
  
