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

#define wgl_x11 0
#define wgl_gl  1
#define wgl_gd  2
#define wgl_ps  3


int xprint(int i, int j, int k);
int gdprint(int i, int j, int k);

typedef struct
{
  int graphics_library;
  int (*wglprint)();
} _wglContexte;

_wglContexte x, gd, current;

main()
{
  int i, j, k;

  x.graphics_library = wgl_x11;
  x.wglprint = xprint;
  current = x;
  gd.wglprint = gdprint;
  
  i = 5;
  j = 7;
  k = 12;
  x.wglprint(i, j, k);
  gd.wglprint(i, j, k);
  current.wglprint(i, j, k);
  current = gd;
  current.wglprint(i, j, k);
}

xprint(int valeur, int valeur2, int valeur3)
{
  printf("X :%d %d %d\n", valeur, valeur2, valeur3);
}

gdprint(int valeur ,int valeur2, int valeur3)
{
  printf("GD: %d %d %d\n", valeur, valeur2, valeur3);
}
