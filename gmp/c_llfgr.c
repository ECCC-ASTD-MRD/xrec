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

gmp_llfgr(lat, lon, x, y, latOrigine, lonOrigine, deltaLat, deltaLon)
float *lat, *lon, x, y;
float latOrigine, lonOrigine, deltaLat, deltaLon;
{
   *lon =  lonOrigine + deltaLon * (x-1);
   *lat =  latOrigine + deltaLat * (y-1);

   if ((*lon - lonOrigine) < 0.0)
      *lon += 360.0;
    }

