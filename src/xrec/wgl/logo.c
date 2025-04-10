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
#include <stdio.h>

/*
  main()
  {
  int i,j,k,n;
  int largeurFenetre, hauteurFenetre;
  int colorTable[256];
  
  char temp, pix;
  
  c_wglpsz(600, 800); 
  c_wglopw("logo");
  LoadDefPal(colorTable);
  c_wglgwz(&largeurFenetre, &hauteurFenetre);
  for (i=hauteurFenetre-1; i >= 0; i--)
  {
  c_wglcol(colorTable[(int)(40 + 180 * (1.0-(float)i/((float)(hauteurFenetre))))]);
  c_wglmvi(0, i);
  c_wgldri(largeurFenetre-1, i);
  }
  
  draw_sea_logo();
  
  while (!c_wglbtn(BGAUCH));
  }
*/


void LoadDefPal(int colorTable[])
{         
  int availPlanes;
  int i, j, k, indCol;
  FILE *f;
  char *tmp, paletteDeDefaut[256];
  char ligne[80];
  int stdcmap[256][3];
  
  int indColMin = 64;
  int indColMax = 192;
  int nbColReservees = 0;
  int r, g, b;
  
  tmp = (char *) getenv("ARMNLIB");
  strcpy(paletteDeDefaut, tmp);
  strcat(paletteDeDefaut, "/data/defpal.rec");
  f = fopen(paletteDeDefaut, "r");
  if (f != NULL)
    {
    while (fgets(ligne, 80, f) != NULL)
      {
      sscanf(ligne, "%d %d %d %d", &indCol, &r, &g, &b);
      stdcmap[indCol][0] = r;
      stdcmap[indCol][1] = g;
      stdcmap[indCol][2] = b;
      }
    fclose(f);
    }
  
  for (i=0; i < 256; i++)
    colorTable[i] = indColMin + (int)((float) i / 255.0 * (indColMax - nbColReservees - 1 - indColMin));
  
  c_wglmcos(colorTable, 256, stdcmap);
}

