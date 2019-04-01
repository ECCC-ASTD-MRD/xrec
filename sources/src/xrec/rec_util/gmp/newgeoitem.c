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
#include <stdlib.h>
#include <stdio.h>

NewGeoItem(ListePointsStruct *(*liste), int *nbItems)
{
  int nbytes;
  
  if (*nbItems == 0)
    {
    *liste = (ListePointsStruct *) calloc(4096,sizeof(ListePointsStruct));
    }
  else
    {
    if ((*nbItems % 4096) == 0)
      {
      nbytes = (*nbItems+4096)*sizeof(ListePointsStruct);
      *liste = (ListePointsStruct *) realloc((ListePointsStruct *) *liste, (unsigned int) nbytes);
      }
    }
  
  
  liste[0][*nbItems].npts = -13;
  (*nbItems)++;
}


NewTextItem(ListeTextStruct *(*liste), int *nbItems)
{
  int nbytes;
  
  if (*nbItems == 0)
    {
    *liste = (ListeTextStruct *) calloc(256,sizeof(ListeTextStruct));
    }
  else
    {
    if ((*nbItems % 256) == 0)
      {
      nbytes = (*nbItems+256)*sizeof(ListeTextStruct);
      *liste = (ListeTextStruct *) realloc((ListeTextStruct *) *liste, (unsigned int) nbytes);
      }
    }
  
  
  (*nbItems)++;
}
