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
#include <memory.h>
#include <rpnmacros.h>
#include <xinit.h>
#include <wgl.h>
#include <souris.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>

extern int lng;
extern GeoMapInfoStruct    mapInfo, oldMapInfo;
extern GeoMapFlagsStruct    mapFlags;

extern _Viewport    viewp;

static char *lblAvrtRecordsManquants[]={"\nLes descripteurs '^^' et '>>'\nsont manquants. Le champ sera affiche\nsans geographie\n", 
                               "\nThe records '^^' and '>>' are missing.\nThe field will be displayed without geography\n"};

static char *lblAvrtRecordsBizarres[]={"\nLes descripteurs '^^' et '>>'\nfont reference a un type de grille non supporte.\nLe champ sera affiche\nsans geographie\n", 
                               "\nThe records '^^' and '>>' refer to an unsupported grid type.\nThe field will be displayed without geography\n"};


static float *axeX = NULL;
static float *axeY = NULL;

void InitMapInfo(char type, int ni, int nj, int ig1, int ig2, int ig3, int ig4)
{
   int res;
   char blanc = ' ';
   int moins1 = -1;
   int un = 1;
   int ip1, ip2, ip3;
   char lat[3];
   char lon[3];
   int cle;
   int ni1, nj1, nk1, ni2, nj2, nk2;
   float fni, fnj, fx1, fy1, fx2, fy2;
   float rx1, ry1, rx2, ry2;
   int ier, ier1, ier2;
   int i;
   int clex, bidon,ig1ref,ig2ref,ig3ref,ig4ref;
   char grref[2];
   char grtyp[2];
   char etik[9], typ[2];
   int grillesIdentiques, ibidon;
   int iig1, iig2, iig3, iig4;
   float fun;
   float lonMin, lonMax, latMin, latMax;

   static char last_type = ' ';
   static int last_ni, last_nj, last_ig1, last_ig2, last_ig3, last_ig4;

   _Champ *champCourant;

   int mapId, mapIdOut;

   if (type == last_type && ni == last_ni &&
       nj == last_nj && ig1 == last_ig1 && ig2 == last_ig2 && ig3 == last_ig3 && ig4 == last_ig4)
      {
      return;
      }
   else
      {
      last_type = type;
      last_ni   = ni;
      last_nj   = nj;
      last_ig1   = ig1;
      last_ig2   = ig2;
      last_ig3   = ig3;
      last_ig4   = ig4;
      }

   lng = c_getulng();

   mapInfo.type = type;   
   mapInfo.ni  = ni;
   mapInfo.nj  = nj;
   mapInfo.ig1 = ig1;
   mapInfo.ig2 = ig2;
   mapInfo.ig3 = ig3;
   mapInfo.ig4 = ig4;
   mapFlags.verifStatutNecessaire = OUI;

   if (mapInfo.type != 'X')
     {
       mapId = c_ezgdefrec(mapInfo.ni, mapInfo.nj, &mapInfo.type, 
			   mapInfo.ig1, mapInfo.ig2, mapInfo.ig3, mapInfo.ig4);
       mapIdOut = c_ezgetgdout();
       
       c_ezgxprm(mapId, &ni1, &nj1, &grtyp, &iig1, &iig2, &iig3, &iig4,
		 &grref, &ig1ref, &ig2ref, &ig3ref, &ig4ref);
       mapInfo.gdid = mapId;
     }

   switch (mapInfo.type)
      {
      case '#':
      case 'Z':
        if (mapId < 0)
           {
           type = 'U';
           MessageAvertissement(lblAvrtRecordsManquants[lng], AVERTISSEMENT);
           c_xsetxy(0, axeX, mapInfo.ni, axeY, mapInfo.nj);
           c_gmpset(type, ni, nj, ig1, ig2, ig3, ig4);
           }
        else
           {
           if (grref[0] != 'N' && grref[0] != 'S' &&  grref[0] != 'L' && grref[0] != 'E')
              {
              type = 'U';
              MessageAvertissement(lblAvrtRecordsBizarres[lng], AVERTISSEMENT);
              c_xsetxy(0, axeX, mapInfo.ni, axeY, mapInfo.nj);
              c_gmpset(type, ni, nj, ig1, ig2, ig3, ig4);
              }
           else
              {
              if (axeX != NULL)
                 {
                 free(axeX);
                 axeX = NULL;
                 }
              
              if (axeY != NULL)
                 {
                 free(axeY);
                 axeY = NULL;
                 }
              
              axeX = (float *) calloc(ni1, sizeof(float));
              axeY = (float *) calloc(nj1, sizeof(float));
              c_gdgaxes(mapId, axeX, axeY);
              
	      /*              if (grref[0] == 'E')
			      {
			      for (i=0; i < nj1; i++)
			      {
			      axeY[i] += 90.0;
			      }
			      }
	      */
              
              
              c_xsetxy(3, axeX, mapInfo.ni, axeY, mapInfo.nj);
              c_gmpzset('Z', ni, nj, ig1, ig2, ig3, ig4, grref[0], ig1ref, ig2ref, ig3ref, ig4ref);
              mapInfo.typeref = grref[0];
              mapInfo.ig1ref = ig1ref;      
              mapInfo.ig2ref = ig2ref;      
              mapInfo.ig3ref = ig3ref;      
              mapInfo.ig4ref = ig4ref;
              }
           }

        AjusterViewport(&viewp);
        fx1 = 1.0;
        fy1 = 1.0;
        fx2 = (float) mapInfo.ni;
        fy2 = (float) mapInfo.nj;
        c_xy2fxfy(&rx1, &ry1, 1.0, 1.0);
        c_xy2fxfy(&rx2, &ry2, fx2, fy2);
        c_wglssp(rx1, ry1, rx2, ry2, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
        break;
        
      case 'Y':
        break;
	
      default:
	c_xsetxy(0, axeX, mapInfo.ni, axeY, mapInfo.nj);
	AjusterViewport(&viewp);
	c_wglssp(1.0, 1.0, (float)mapInfo.ni, (float)mapInfo.nj, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
	c_gmpset(type, ni, nj, ig1, ig2, ig3, ig4);
	break;
      }
   
}


void GeoMgrGetMapInfo(char *grtyp, char *grref, int *ni, int *nj, int *ig1, int *ig2, int *ig3, int *ig4, 
                 float **lat, float **lon)
{
   int gdid ;
   int ig1ref, ig2ref, ig3ref, ig4ref;
   
   if (mapInfo.type == ' ')
      {
      gdid  = c_ezgetgdout();
      if (gdid == -1)
         {
         gdid = c_ezgetgdin();
         }
      c_ezgxprm(gdid, grtyp, ni, nj, ig1, ig2, ig3, ig4, grref, ig1ref, ig2ref, ig3ref, ig4ref);

      InitMapInfo(*grtyp, *ni, *nj, *ig1, *ig2, *ig3, *ig4);
      }
   else
      {
      *grtyp = mapInfo.type;
      *grref = mapInfo.typeref;
      *ni    = mapInfo.ni;
      *nj    = mapInfo.nj;
      *ig1   = mapInfo.ig1;
      *ig2   = mapInfo.ig2;
      *ig3   = mapInfo.ig3;
      *ig4   = mapInfo.ig4;
      *lat   = mapInfo.lat;
      *lon   = mapInfo.lon;
      }
}


void GeoMgrClearMapInfo(void)
{
   mapInfo.type = ' ';
   mapInfo.typeref = ' ';
   mapInfo.ni = 0;
   mapInfo.nj = 0;
   mapInfo.ig1 = 0;
   mapInfo.ig2 = 0;
   mapInfo.ig3 = 0;
   mapInfo.ig4 = 0;
}


void GeoMgrSetLatLon(float *lat, float *lon)
{
   mapInfo.lat = lat;
   mapInfo.lon = lon;
}

int c_ezgdefrec(int ni, int nj, char *grtyp, int ig1, int ig2, int ig3, int ig4)
{
   int iig4;
   float *bidon = NULL;   
   float un;
   char lgrtyp[2];
   un = 1.0 * 1.0;

   lgrtyp[0] = grtyp[0];
   lgrtyp[1] = NULL;

   return c_ezqkdef(ni, nj, lgrtyp, ig1, ig2, ig3, ig4, 1);
   
}

