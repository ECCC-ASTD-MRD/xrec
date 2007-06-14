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
#include <stdio.h>
#include <gdb.h>
#include <stdlib.h>
#include <string.h>

extern GeoMapOptionsStruct mapOptions;
static ListePointsStruct **gdb_liste;
static int *gdb_nbItems;
extern int geography_source;
char *rmn_geo = NULL;

lire_geo()
{
  char *gdb_path;
  float pts[200];
  int deuxCents = 200;
  char nomFichierGeographie[128];
  
  int res;
  int n,i, j, nbSeg;
  float xmin, xmax;
  float ymin, ymax;
  float latMin, latMax, lonMin, lonMax;
  int oldNbItems = 0;
  static int gdb_initialized = -1;
  
  int npts, vingtCinq, code;
  
  if (geography_source == -1)
    {
    gdb_path = (char *)getenv("GDB_PATH");
    if (NULL == gdb_path)
      {
      geography_source = RMN_GEO;
      }
    else
      {
      geography_source = GDB_GEO;
      if (gdb_initialized == -1)
        {
#ifndef Darwin_OSX_PPC
        gdb_init();
        gdb_initialized = 1;
#endif
        }
      }
    }
  
  switch (geography_source)
    {
    case RMN_GEO:
      rmn_geo = (char *) getenv("DCW");
      
      if (rmn_geo == NULL)
        {
        rmn_geo = (char *) getenv("ARMNLIB");

        if (rmn_geo == NULL)
          {
          printf("La valeur de la variable d'environnement ARMNLIB est inconnue... \nImpossible de continuer.\n");
          exit(-1);
          }
        }
      
      lire_rmn_geo();
      break;
      
    case GDB_GEO:
#ifndef Darwin_OSX_PPC
      lire_gdb_geo();
#endif
      break;
    }
  
  lire_rmn_latlon();

}

