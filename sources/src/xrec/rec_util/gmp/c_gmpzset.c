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

extern float gdxmin, gdymin, gdxmax, gdymax;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapInfoStruct     oldMapInfo;

extern GeoMapFlagsStruct    mapFlags;

int c_gmpzset(char grtyp, int ni, int nj,  int ig1, int ig2, int ig3, int ig4, char grtyp_ref, int ig1ref, int ig2ref, int ig3ref, int ig4ref)
{
   
   int iig1, iig2, iig3, iig4;
   int res;
   char typeref[2];

   typeref[0] = grtyp_ref;
   typeref[1] = '\0';

   gdxmin = 1.0;
   gdxmax = (float) (ni);
   gdymin = 1.0;
   gdymax = (float) (nj);

   c_xy2fxfy(&gdxmin, &gdymin, gdxmin, gdymin);
   c_xy2fxfy(&gdxmax, &gdymax, gdxmax, gdymax);
   
   mapInfo.ig1 = ig1;
   mapInfo.ig2 = ig2;
   mapInfo.ig3 = ig3;
   mapInfo.ig4 = ig4;

   mapInfo.typeref  = typeref[0];
   mapInfo.ig1ref = ig1ref;
   mapInfo.ig2ref = ig2ref;
   mapInfo.ig3ref = ig3ref;
   mapInfo.ig4ref = ig4ref;

   iig1 = ig1ref; iig2 = ig2ref; iig3 = ig3ref; iig4 = ig4ref;
   switch(typeref[0])
      {
      case 'L':
      mapInfo.xOrigine = 0.0;
      mapInfo.yOrigine = 0.0;
      mapInfo.type = grtyp;
      mapInfo.hemisphere = GLOBAL;
      mapInfo.indOrientation = NORD;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      f77name(cigaxg)(&typeref,  &mapInfo.latOrigine, &mapInfo.lonOrigine, &mapInfo.deltaLat, &mapInfo.deltaLon, &iig1, &iig2, &iig3, &iig4,(F2Cl) strlen(typeref));
      if (mapInfo.lonOrigine >= 180.0)
         mapInfo.lonOrigine -= 360.0;
      mapFlags.typeValide = OUI;
      break;
      
      case 'N':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = NORD;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&typeref, &mapInfo.PosXDuPole, &mapInfo.PosYDuPole,
              &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
              &iig1, &iig2, &iig3, &iig4,(F2Cl) strlen(typeref));
      mapFlags.typeValide = OUI;
      break;
      
      case 'S':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = SUD;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&typeref, &mapInfo.PosXDuPole, &mapInfo.PosYDuPole,
              &mapInfo.PasDeGrille, &mapInfo.AngleGreenwich,
              &iig1, &iig2, &iig3, &iig4,(F2Cl) strlen(typeref));
      mapFlags.typeValide = OUI;
      break;
      
      case 'E':
      mapInfo.type = grtyp;
      mapInfo.ni = ni;
      mapInfo.nj = nj;
      mapInfo.hemisphere = GLOBAL;
      mapInfo.indOrientation = NORD;
      f77name(cigaxg)(&typeref, &mapInfo.elat1, &mapInfo.elon1,
                      &mapInfo.elat2, &mapInfo.elon2,
                      &iig1, &iig2, &iig3, &iig4,(F2Cl) strlen(typeref));
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

