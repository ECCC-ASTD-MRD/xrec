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

#include <wgl_x.h>

f77name(wgllpt)()
{
   c_wgllpt();
   }

/**
******
**/

c_wgllpt()
{
   char nomFichierPatrons[128];
   char *tmp;

   tmp = (char *) getenv("ARMNLIB");

   if (tmp == NULL)
      {
      printf("<c_wgllpt>: La valeur de la variable d'environnement ARMNLIB est inconnue... \nImpossible de continuer.\n");
      exit(-1);
      }
      
   strcpy(nomFichierPatrons, tmp);
   strcat(nomFichierPatrons, "/data/patrns");

   f77name(wwwlpt)(nomFichierPatrons, strlen(nomFichierPatrons));
   }
