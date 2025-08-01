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

void x_wglcol(int couleur)
{
   int nplanes;
   wglfshlb();
   
   nplanes = x_wglgpl();

   switch(nplanes)
      {
      case 1:
        if (couleur == BLANC)
           {
           XSetForeground(wglDisp, wglLineGC, WhitePixel(wglDisp, wglScrNum));
           XSetForeground(wglDisp, wglFillGC, WhitePixel(wglDisp, wglScrNum));
           currentColor = BLANC;
           }
        else
           {	
           XSetForeground(wglDisp, wglLineGC, BlackPixel(wglDisp, wglScrNum));
           XSetForeground(wglDisp, wglFillGC, BlackPixel(wglDisp, wglScrNum));
           currentColor = NOIR;
           }
        break;

      case 8:
        XSetForeground(wglDisp, wglLineGC, wglColorTable[couleur]);
        XSetForeground(wglDisp, wglFillGC, wglColorTable[couleur]);
        currentColor = couleur;
        break;

      default:
        XSetForeground(wglDisp, wglLineGC, xcouleurs[couleur].pixel);
        XSetForeground(wglDisp, wglFillGC, xcouleurs[couleur].pixel);
        currentColor = couleur;
        break;
      }

}
