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
#include <math.h>

ComparerMapInfos(GeoMapInfoStruct mapInfo1, GeoMapInfoStruct mapInfo2)
{
   if (mapInfo1.type != mapInfo2.type || mapInfo1.typeref != mapInfo2.typeref)
      return  PAS_PAREIL;

   if (mapInfo1.type != 'Z')
      {
      if (mapInfo1.ni != mapInfo2.ni)
         return  PAS_PAREIL;
      
      if (mapInfo1.nj != mapInfo2.nj)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig1 != mapInfo2.ig1)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig2 != mapInfo2.ig2)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig3 != mapInfo2.ig3)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig4 != mapInfo2.ig4)
         return  PAS_PAREIL;
      }
   else
      {
      if (mapInfo1.ig1ref != mapInfo2.ig1ref)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig2ref != mapInfo2.ig2ref)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig3ref != mapInfo2.ig3ref)
         return  PAS_PAREIL;
      
      if (mapInfo1.ig4ref != mapInfo2.ig4ref)
         return  PAS_PAREIL;
      }

   return PAREIL;

  }
