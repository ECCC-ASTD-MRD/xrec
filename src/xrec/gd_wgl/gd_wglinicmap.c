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

#include <gd_wgl.h>

#define DIST(a,b,c) a*a + b*b + c*c

int gd_cmapInfo;
unsigned int gd_colorbitrange[3][3];

static char *les_couleurs[] = {"white", "black", "red", "cyan", "yellow", "magenta", "green2", "blue", "gray"};

void gd_wglinicmap()
{
   int i,plane_masks;
   int ncmaps;

   int couleurderemplacement;
   int ncolormaps;
   
   long vinfo_mask;
   int nitems_return;
   
   int nbPixs;
   
   }

