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

x_wglgco(int indCol, int *red, int *green, int *blue)
{
   int nplanes;

   nplanes = c_wglgpl();

   switch(nplanes)
      {
      case 8:
        *red   = couleurs[wglColorTable[indCol % 256]].red / 256;
        *green = couleurs[wglColorTable[indCol % 256]].green / 256;
        *blue  = couleurs[wglColorTable[indCol % 256]].blue / 256;
        break;

      default:
        *red   = couleurs[indCol].red / 256;
        *green = couleurs[indCol].green / 256;
        *blue  = couleurs[indCol].blue / 256;
        break;
      }
}
