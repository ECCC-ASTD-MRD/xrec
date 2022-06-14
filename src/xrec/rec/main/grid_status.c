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

#include <rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

static char grtyp = ' ';
static int ni,nj,nk, ig1,ig2,ig3,ig4;


int ThisIsTheCurrentGrid(char newgrtyp, int newni, int newnj, int newnk, 
                   int newig1, int newig2, int newig3, int newig4)
{
   grtyp = newgrtyp;
   ni = newni;
   nj = newnj;
   nk = newnk;
   ig1 = newig1;
   ig2 = newig2;
   ig3 = newig3;
   ig4 = newig4;

}

int HasGridChanged(char newgrtyp, int newni, int newnj, int newnk, 
                   int newig1, int newig2, int newig3, int newig4)
{
   if (GetGrilleSelectionnee() != 0)
      {
      return 0;
      }

   if (newgrtyp != grtyp) return 1;
   if (newni != ni) return 1;
   if (newnj != nj) return 1;
   if (newnk != nk) return 1;
   if (newig1!=ig1) return 1;
   if (newig2!=ig2) return 1;
   if (newig3!=ig3) return 1;
   if (newig4!=ig4) return 1;

   return 0;
}


