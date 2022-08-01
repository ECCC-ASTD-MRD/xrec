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
#include <rpnmacros.h>
#include "gmp.h"
#include "gdb.h"
#include "wgl.h"

extern GeoMapOptionsStruct mapOptions;
static ListePointsStruct *gdb_liste;
static int gdb_nbItems;

extern ListeTextStruct *gmp_cities;
extern ListePointsStruct *gmp_vecs[];
extern int gmp_nbVecs[];
extern int gmp_nbCities;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapFlagsStruct   mapFlags;

void
get_cityname(int gr_type, float lat, float lon, char *name)
{
  char *tmp;
  
  int res;
  int npts,i, j, n,nbSeg, gdid, ok;
  float rx, ry; 
  float latMin, latMax, lonMin, lonMax;
  int oldNbItems = 0;
  int len;
  char *big_only = NULL;


  big_only = (char *) getenv("GDB_CITY_SIZE");
  len = strlen(name);
  if (len == 0) return;
    
  if (big_only != NULL)
    {
    if (0 == strcmp(big_only, "BIG"))
      {
      i = 0;
      ok = 1;
      while (ok == 1 && i < len)
        {
        if (isupper(name[i]) || ispunct(name[i]))
          {
          i++;
          }
        else
          {
          ok = 0;
          }
        }
      if (!ok) return;
      }
    }
   
  NewTextItem(&gmp_cities,&gmp_nbCities);
  i = gmp_nbCities -1;
  if (len >= 32)
    {
    strncpy(gmp_cities[i].text, name, 31);
    gmp_cities[i].text[31] = '\0';
    }
  else
    {
    strcpy(gmp_cities[i].text, name);
    }

  for (n=1; n < strlen(gmp_cities[i].text); n++)
    {
    gmp_cities[i].text[n] = tolower(gmp_cities[i].text[n]);
    }
  for (n=1; n < strlen(gmp_cities[i].text); n++)
    {
    if (ispunct(gmp_cities[i].text[n-1]))
      {
      gmp_cities[i].text[n] = toupper(gmp_cities[i].text[n]);
      }
    }
  gmp_cities[i].lat = lat;
  gmp_cities[i].lon = lon;
  if (gmp_cities[i].lon < 0.0)
    {
    gmp_cities[i].lon += 360.0;
    }
  gmp_cities[i].statutPRGrille = 1;
  gdid = c_ezgetgdout();
  c_gdxyfll(gdid, &(gmp_cities[i].x), &(gmp_cities[i].y), &(gmp_cities[i].lat), &(gmp_cities[i].lon), 1);
  c_gmpDrawCityName(gmp_cities[i].x, gmp_cities[i].y, (char *)gmp_cities[i].text);


  
}


void c_gmpDrawCityName(float x, float y, char *text)
{
  int largeurTexte, hauteurTexte, rayon, posx, posy;
  float rx, ry;

  rayon = 4;
  c_wglcol(BLANC);
  c_wglfsz(12);
  largeurTexte = c_wglwsi(text, strlen(text));
  hauteurTexte = c_wglasi("1234",4) + c_wgldsi("1234", 4);
  c_xy2fxfy(&rx, &ry, x, y);
  c_wglxai(&posx, &posy, rx, ry);
  c_wglcfi(posx, posy, 4);
  c_wglcol(NOIR);
  c_wglcli(posx, posy, 4);
  DrawBoxedStr(text, posx - largeurTexte /2 , posy - hauteurTexte - rayon, 12,NOIR, BLANC);
}

void
get_coastline(int gr_type, int n, float lat0, float lon0, float lat1, float lon1,float *ll )
{
  float pts[24000];
  char *tmp;
  
  int res;
  int npts,i, j, nbSeg;
  float xmin, xmax;
  float ymin, ymax;
  float latMin, latMax, lonMin, lonMax;
  int oldNbItems = 0;
  
  latMin = lat0;
  latMax = lat1;
  lonMin = lon0;
  lonMax = lon1;
  if (lonMin < 0.0) lonMin += 360.0;
  if (lonMax < 0.0) lonMax += 360.0;
  if (lonMax < lonMin)
    swapFloats(&lonMin, &lonMax);
  
  res = gmp_perim(&xmin, &ymin, &xmax, &ymax, &latMin, &lonMin, &latMax, &lonMax, &nbSeg);
  /*   printf("%d [%f %f], [%f %f] %d\n", res, latMin, lonMin, latMax, lonMax, nbSeg); */
  if (res != 0)
    {
    if (n > 12000) n=12000;
    npts = 2*n;
    
    /*     gmp_trim(pts, &npts,mapOptions.resolution); */
    gmp_convert(gdb_liste, &gdb_nbItems, ll, npts, xmin, ymin, xmax, ymax, nbSeg);
    
/*    for (i=oldNbItems; i < gdb_nbItems; i++)
      {*/
      i = gdb_nbItems -1;
      switch(mapOptions.styleGeo)
        {
        case LIGNE:
        case TIRET:
          TracerVecteurs(&(gdb_liste[i]));
          break;

        case POINT:
          TracerPoints(&(gdb_liste[i]));
          break;
        }
/*      }*/
    NewGeoItem(&gdb_liste,&gdb_nbItems);
    oldNbItems = gdb_nbItems-1;
    }
}



lire_gdb_geo()
{
  int res;
  int n,i, j, nbSeg;
  float xmin, xmax;
  float ymin, ymax;
  float latMin, latMax, lonMin, lonMax;
  int oldNbItems = 0;
  static int gdb_initialized = -1;
  int gdid;
  int npts, vingtCinq, code;
  float lonmin, lonmax, latmin, latmax;
  float x, y, tlat, tlon;
  int idebut, jdebut, ifin, jfin;
  int pixperdegree = 1;
  int exposant = 0;
  int ex=1;
  int ppd_mode = 0;
  int user_ppd = -1;
  int ppd_maxval = 128;
  char *ppd_env, *ppd_max;
  static GeoMapInfoStruct oldMapInfo;
  static int once = 0;
  int rereadmapFlag = 0;
#ifndef Darwin_OSX_PPC
  if (once == 0)
    {
    memset(&oldMapInfo, NULL, sizeof(GeoMapInfoStruct));
    once = 1;
    }
  ppd_env = (char *) getenv("GDB_RESOLUTION");
  ppd_max = (char *) getenv("GDB_MAX");
  
  if (ppd_env != NULL)
    {
    sscanf(ppd_env, "%d", &user_ppd);
    ppd_mode = 1;
    }
  
  if (ppd_max != NULL)
   {
   sscanf(ppd_max, "%d", &ppd_maxval);
   }
   
  if (0 != memcmp(&oldMapInfo, &mapInfo, sizeof(GeoMapInfoStruct)))
    {
    rereadmapFlag = 1;
    }
  
    rereadmapFlag = 1;
  gdid = mapInfo.gdid;
  c_wglgvi(&idebut,&jdebut,&ifin,&jfin);

  gmp_gvlatlon(&latmin, &lonmin, &latmax, &lonmax, M180_180);

  gdb_limit(latmin, lonmin, latmax, lonmax);
/*  fprintf(stderr, "limits : (%f,%f),(%f,%f)\n", latmin, lonmin, latmax, lonmax);*/
  
  switch (ppd_mode)
    {
    case 0:
    pixperdegree = (jfin - jdebut) / (latmax - latmin);
    do 
      {
      exposant++;
      ex = ex*2;
      } while ((ex*2) < pixperdegree);

    if (ex == 0) ex = 1;
    pixperdegree = ex;
    if (pixperdegree > ppd_maxval)
      {
      pixperdegree = ppd_maxval;
       }

    break;
    
    case 1:
    pixperdegree = user_ppd;
    break;
    }
  for (i=0; i < NMAP_FLAGS; i++)
    {
    if (i != LATLON)
      {
      gmp_nbVecs[i] = 0;
      }
    }
  gmp_nbVecs[LATLON] = 0;
  
/*  fprintf(stderr, "Pixperdegree : %d\n", pixperdegree);*/
  
/*  if (ppd_mode == 1 && rereadmapFlag == 1)*/
  if (rereadmapFlag == 1)
    {
    for (i=0; i < NMAP_FLAGS; i++)
      {
      if (i != VILLES)
        {
        LibererCarte(&(gmp_vecs[i]), &gmp_nbVecs[i]);
        }
      else
        {
        gmp_nbCities = 0;
        gmp_nbVecs[VILLES] = 0;
        }
      }
    }
  for (i=0; i < NMAP_FLAGS; i++)
    {
    if (i != LATLON)
      {
      gmp_nbVecs[i] = 0;
      }
    }
  gmp_nbVecs[LATLON] = 0;
  
  for (i=0; i < 8; i++)
    {
    if (mapFlags.etat[i] == OUI  && gmp_nbVecs[i] == 0)
      {
      ActiverParamsLigne(mapFlags.style[i], mapFlags.indCouleur[i], mapFlags.epaisseur[i]);
      gdb_liste = gmp_vecs[i];
      gdb_nbItems = gmp_nbVecs[i];
      NewGeoItem(&gdb_liste,&gdb_nbItems);
      switch (i)
        {
        case CONTINENTS:
          gdb_line(pixperdegree,GDB_LIN_COAST,get_coastline);
          break;

        case PAYS:
          gdb_line(pixperdegree,GDB_LIN_POLIT,get_coastline);
          break;

        case PROVINCES:
          gdb_line(pixperdegree,GDB_LIN_ADMIN,get_coastline);
          break;

        case VILLES:
          gdb_line(pixperdegree,GDB_LIN_CITY,get_coastline);
          gdb_text(pixperdegree, GDB_TXT_CITY, get_cityname);
          break;

        case LACS:
          gdb_line(pixperdegree,GDB_LIN_LAKE,get_coastline);
          break;

        case RIVIERES:
          gdb_line(pixperdegree,GDB_LIN_RIVER,get_coastline);
          break;

        case ROUTES:
          gdb_line(pixperdegree,GDB_LIN_ROAD,get_coastline);
          break;

        default:
          break;

        }

      gmp_vecs[i] = gdb_liste;
      gmp_nbVecs[i] = gdb_nbItems;
/*      fprintf(stderr, "%d : %d\n", i, gmp_nbVecs[i]);*/
      }
    else
      {
      if (mapFlags.etat[i] == OUI)
        {
        switch (i)
          {
          case VILLES:
            /* absence de break intentionnelle */

          default:
            ActiverParamsLigne(mapFlags.style[i], mapFlags.indCouleur[i], mapFlags.epaisseur[i]);
            AfficherVecteurs(gmp_vecs[i], gmp_nbVecs[i], mapFlags.style[i], mapFlags.indCouleur[i], mapFlags.epaisseur[i]);
            break;
          }
        }
      }
    }
  
  if (mapFlags.etat[VILLES] == OUI)
    {
    for (n=0; n < gmp_nbCities; n++)
      {
      c_gmpDrawCityName(gmp_cities[n].x, gmp_cities[n].y, (char *)gmp_cities[n].text);
      }
    }

  memcpy(&oldMapInfo, &mapInfo, sizeof(GeoMapInfoStruct));
#endif
  }

