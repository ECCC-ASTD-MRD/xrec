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

TracerVecteurs(ListePointsStruct *itemListe)
{
   int n, res;
   
   ListePointsStruct item;

   if (itemListe == NULL) return -1;

   itemListe->statutPRGrille= clip(itemListe->xmin, itemListe->ymin, itemListe->xmax, itemListe->ymax);
   
   switch (itemListe->statutPRGrille && itemListe->npts > 0)
      {
      case DEDANS:
      if (itemListe->npts > 0)
        {
        if (itemListe->full_line == 1)
          {
          c_wglplx(itemListe->npts, itemListe->pointsGeo);
          }
        else
          {
          c_wglmvx(itemListe->pointsGeo[0].x, itemListe->pointsGeo[0].y);
          for (n=1; n < itemListe->npts; n++)
            {
            if (itemListe->pen[n] == 1)
              {
              c_wgldrx(itemListe->pointsGeo[n].x, itemListe->pointsGeo[n].y);
              }
            else
              {
              c_wglmvx(itemListe->pointsGeo[n].x, itemListe->pointsGeo[n].y);
              }
            }
          }
        }
      break;
      
      case DEHORS:
      break;
      }
   
   }

