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

extern float gdxmin, gdymin, gdxmax, gdymax;
extern float gmp_xmin, gmp_ymin, gmp_xmax, gmp_ymax;
extern float old_gmp_xmin, old_gmp_ymin, old_gmp_xmax, old_gmp_ymax;
extern GeoMapInfoStruct     mapInfo;
extern GeoMapInfoStruct     oldMapInfo;

extern GeoMapFlagsStruct    mapFlags;
extern GeoMapOptionsStruct mapOptions;

extern ListePointsStruct *vecsContinents;
extern ListePointsStruct *vecsMeridiens;
extern int nbVecsContinents;
extern int nbVecsMeridiens;
extern int gmpFlagInterrupt;

void c_gmpdrw();

f77name(gmpdrw)()
{
   c_gmpdrw();
}

void c_gmpdrw()
{
   if (mapFlags.typeValide == NON)
      return;


   gdxmin = 1.0;
   gdxmax = (float) mapInfo.ni;
   gdymin = 1.0;
   gdymax = (float) mapInfo.nj;
   c_xy2fxfy(&gdxmin, &gdymin, gdxmin, gdymin);
   c_xy2fxfy(&gdxmax, &gdymax, gdxmax, gdymax);

   c_wglgvx(&gmp_xmin, &gmp_ymin, &gmp_xmax, &gmp_ymax);
   if (gmp_xmin < gdxmin)
      gmp_xmin = gdxmin;
   
   if (gmp_xmax > gdxmax)
      gmp_xmax = gdxmax;
		     
   if (gmp_ymin < gdymin)
      gmp_ymin = gdymin;
   
   if (gmp_ymax > gdymax)
      gmp_ymax = gdymax;

/*    c_wglcmx(xmin, ymin, xmax, ymax); */
   if (gmp_xmin == old_gmp_xmin && gmp_xmax == old_gmp_xmax && gmp_ymin == old_gmp_ymin && gmp_ymax == old_gmp_ymax)
      mapFlags.verifStatutNecessaire = NON;
   else
      mapFlags.verifStatutNecessaire = OUI;
   
   if (mapFlags.continents == OUI)
      {
      ActiverParamsLigne(mapOptions.styleGeo, mapOptions.couleurGeo, mapOptions.epaisseurGeo);
      if (mapFlags.vecsContinentsLus == NON)
         {
         gmp_xmin = gdxmin;
         gmp_xmax = gdxmax;
         gmp_ymin = gdymin;
         gmp_ymax = gdymax;
         LibererCarte(&vecsContinents, &nbVecsContinents);
         LireFichierGeographie(&vecsContinents, &nbVecsContinents, mapOptions.fichierGeographie, mapOptions.nbFichiersGeographie);
         mapFlags.vecsContinentsLus = OUI;
         }
      else
         AfficherVecteurs(vecsContinents, nbVecsContinents, mapOptions.styleGeo, mapOptions.couleurGeo, mapOptions.epaisseurGeo);
      }
   

   if (mapFlags.meridiens == OUI)
      {
      ActiverParamsLigne(mapOptions.styleMer, mapOptions.couleurMer, mapOptions.epaisseurMer);
      if (mapFlags.vecsMeridiensLus == NON)
         {
         gmp_xmin = gdxmin;
         gmp_xmax = gdxmax;
         gmp_ymin = gdymin;
         gmp_ymax = gdymax;
         LibererCarte(&vecsMeridiens, &nbVecsMeridiens);
         LireLatLon(&vecsMeridiens, &nbVecsMeridiens);
         mapFlags.vecsMeridiensLus = OUI;
         }
      else
         AfficherVecteurs(vecsMeridiens, nbVecsMeridiens, 
                          mapOptions.styleMer, mapOptions.couleurMer, mapOptions.epaisseurMer);
      }
   
   old_gmp_xmin = gmp_xmin;
   old_gmp_xmax = gmp_xmax;
   old_gmp_ymin = gmp_ymin;
   old_gmp_ymax = gmp_ymax;
   }


