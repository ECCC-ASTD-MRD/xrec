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
#include <vcar.h>
#include <rpnmacros.h>
#include <stdlib.h>
#include <string.h>

f77name(initplot88)()
{
   init_plot88();

   }


init_plot88()
{
   char fichierFontes[132];
   char *armnlib;
   char *bidon = NULL;

   armnlib = (char *) getenv("ARMNLIB");
   if (armnlib == NULL)
      {
      printf("$ARMNLIB non definie. Adieu\n");
      exit(-1);
      }

   strcpy(fichierFontes, armnlib);
   strcat(fichierFontes, "/data/fontes.bin");
   lir_vcar(fichierFontes);

   set_vcar( bidon, YMIROIR, 1, 0 );
   set_vcar( bidon, HAUTEUR, 32, LARGEUR, 32,  FONTE  ,  0, 0 );
   }


