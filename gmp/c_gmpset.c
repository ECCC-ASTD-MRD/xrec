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
#include <rpnmacros.h>
#include <gmp.h>
#include <math.h>

extern GeoMapInfoStruct     mapInfo;
extern GeoMapInfoStruct  oldMapInfo;
extern GeoMapInfoStruct nullMapInfo;

extern GeoMapOptionsStruct mapOptions;

extern GeoMapFlagsStruct   mapFlags;
extern ListePointsStruct *gmp_vecs[NMAP_FLAGS];


extern int gmp_nbVecs[];
extern int nbVecsContinents;
extern int nbVecsMeridiens;
extern int gmpFlagInterrupt;

float gdxmin, gdymin, gdxmax, gdymax;
float gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax;
float old_gmp_xmin, old_gmp_ymin, old_gmp_xmax, old_gmp_ymax;
int   geography_source = -1;

int c_gmpset(char grtyp, int ni, int nj, int ig1, int ig2, int ig3, int ig4);

int f77name(gmpset)(char *grtyp, int *ni, int *nj, int *ig1, int *ig2, int *ig3, int *ig4)
{
  c_gmpset(*grtyp, *ni, *nj, *ig1, *ig2, *ig3, *ig4);
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

int c_gmpset(char grtyp, int ni, int nj, int ig1, int ig2, int ig3, int ig4)
{
   int iig1, iig2, iig3, iig4;
   int gdid,res;
   char lgrtyp[2];

   c_gmpinit();

   iig1 = ig1;
   iig2 = ig2;
   iig3 = ig3,
   iig4 = ig4;

   gdxmin = 1.0;
   gdxmax = (float) (ni);
   gdymin = 1.0;
   gdymax = (float) (nj);

   lgrtyp[0] = grtyp;
   lgrtyp[1] = '\0';
   mapInfo.gdid = c_ezqkdef(ni, nj, lgrtyp, ig1, ig2, ig3, ig4, 0);

   mapInfo.ig1 = ig1;
   mapInfo.ig2 = ig2;
   mapInfo.ig3 = ig3;
   mapInfo.ig4 = ig4;

   switch(grtyp)
      {
      case 'A':
      case 'G':
      mapInfo.type = grtyp;
      mapInfo.xOrigine = 1.0;
      mapInfo.yOrigine = 1.0;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = ig1;
      mapInfo.indOrientation = NORD;
      mapInfo.deltaLon = 360.0 / (float) (mapInfo.ni);
      mapInfo.lonOrigine = 0.0;
      switch (mapInfo.hemisphere)
         {
         case GLOBAL:
         mapInfo.deltaLat = 180.0 / (float) (mapInfo.nj);
         mapInfo.latOrigine = -90.0 + mapInfo.deltaLat * 0.50;
         mapInfo.lonOrigine = 0.0;
         break;

         case NORD:
         mapInfo.deltaLat = 90.0 / (float) (mapInfo.nj);
         mapInfo.latOrigine = mapInfo.deltaLat * 0.50;
         break;

         case SUD:
         mapInfo.deltaLat = 90.0 / (float) (mapInfo.nj);
         mapInfo.latOrigine = -90.0 + mapInfo.deltaLat * 0.50;
         break;
         }
      mapFlags.typeValide = OUI;
      break;

      case 'O':
      mapInfo.type = grtyp;
      mapInfo.xOrigine = 1.0;
      mapInfo.yOrigine = 1.0;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = GLOBAL;
      mapInfo.indOrientation = NORD;
      mapInfo.deltaLon = 1.0;
      mapInfo.lonOrigine = 0.0;
      mapInfo.deltaLat = 180.0 / (float) (mapInfo.nj);
      mapInfo.latOrigine = -90.0 + mapInfo.deltaLat * 0.50;
      mapInfo.lonOrigine = 0.0;
      mapFlags.typeValide = OUI;
      break;

      case 'B':
      mapInfo.type = grtyp;
      mapInfo.xOrigine = 1.0;
      mapInfo.yOrigine = 1.0;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = ig1;
      mapInfo.indOrientation = NORD;
      mapInfo.deltaLon = 360.0 / (float) (mapInfo.ni - 1);
      mapInfo.lonOrigine = 0.0;
      switch (mapInfo.hemisphere)
         {
         case GLOBAL:
         mapInfo.deltaLat = 180.0 / (float) (mapInfo.nj - 1);
         mapInfo.latOrigine = -90.0;
         break;

         case NORD:
         mapInfo.deltaLat = 90.0 / (float) (mapInfo.nj - 1);
         mapInfo.latOrigine = 0.0;
         break;

         case SUD:
         mapInfo.deltaLat = 90.0 / (float) (mapInfo.nj - 1);
         mapInfo.latOrigine = -90.0;
         break;
         }
      mapFlags.typeValide = OUI;
      break;

      case 'E':
      mapInfo.type = grtyp;
      mapInfo.hemisphere = GLOBAL;
      mapInfo.indOrientation = NORD;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      f77name(cigaxg)(&grtyp,&mapInfo.elat1,&mapInfo.elon1,&mapInfo.elat2,&mapInfo.elon2,
                      &iig1, &iig2, &iig3, &iig4);
      mapFlags.typeValide = OUI;
      break;

      case 'L':
      mapInfo.type = grtyp;
      mapInfo.hemisphere = GLOBAL;
      mapInfo.indOrientation = NORD;
      mapInfo.xOrigine = 1.0;
      mapInfo.yOrigine = 1.0;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      f77name(cigaxg)(&grtyp,  &mapInfo.latOrigine, &mapInfo.lonOrigine, &mapInfo.deltaLat, &mapInfo.deltaLon,
                      &iig1, &iig2, &iig3, &iig4);
      if (mapInfo.lonOrigine >= 180)
         mapInfo.lonOrigine -= 360.0;
      mapFlags.typeValide = OUI;
      break;

      case 'N':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = NORD;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&grtyp, &mapInfo.PosXDuPole, &mapInfo.PosYDuPole,
              &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
              &iig1, &iig2, &iig3, &iig4);
      mapFlags.typeValide = OUI;
      break;

      case 'S':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = SUD;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&grtyp, &mapInfo.PosXDuPole, &mapInfo.PosYDuPole,
              &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
              &iig1, &iig2, &iig3, &iig4);
      mapFlags.typeValide = OUI;
      break;

      case 'T':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = NORD;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&grtyp, &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
		      &mapInfo.latOrigine, &mapInfo.lonOrigine,
              &iig1, &iig2, &iig3, &iig4);
      mapFlags.typeValide = OUI;
      break;

      case '!':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = NORD;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&grtyp, &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
		      &mapInfo.latOrigine, &mapInfo.lonOrigine,
              &iig1, &iig2, &iig3, &iig4);
      mapFlags.typeValide = OUI;
      break;

      default:
      mapFlags.typeValide = NON;
      return -1;
      }

   res = ComparerMapInfos(oldMapInfo, mapInfo);
   if (res == PAS_PAREIL)
      {
      mapFlags.lu[CONTINENTS] = NON;
      mapFlags.lu[LATLON] = NON;
      }

   CopierMapInfos(&oldMapInfo, &mapInfo);
   return 1;
   }

