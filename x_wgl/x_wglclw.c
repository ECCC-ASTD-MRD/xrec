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

x_wglclw(int indFenetre)
{
   int i;

/**
   if (cmap != DefaultColormap(wglDisp, wglScrNum))
      {
      XFreeColormap(wglDisp, cmap);
      }
   else
      {
      i = 0;
      while (!wglWritablePixs[i] && i < 255)
         i++;
      XFreeColors(wglDisp, cmap, &(pix[i]), 255 - i, 0);
      }

   customWinSize = False;
   aspectRatio   = 0.0;

   XFreePixmap(wglDisp, wglPatterns[0]);
   for (i=1; i < 257; i++)
      {
      if (wglPatterns[i] != wglPatterns[0])
         XFreePixmap(wglDisp, wglPatterns[i]);
      }
   
**/

  x_wglsavpfc();
  x_wglsetw(indFenetre);
   if (bgPix)
      {
      XFreePixmap(wglDisp, bgPix);
      bgPix = NULL;
      }

   XFreeGC(wglDisp, wglLineGC);
   XFreeGC(wglDisp, wglFillGC);
   XDestroyWindow(wglDisp, fenetre[fenetreCourante].wglWin);
   
   memset((char *) &fenetre[fenetreCourante], NULL, sizeof(_Fenetre));
   
   fenetre[fenetreCourante].libre = True;
   i = 0;
   nbFenetresActives--;
   while (fenetre[i].libre && i < nbFenetresActives)
      i++;

   if (nbFenetresActives > 0)
     {
     x_wgldefncw(fenetre[i].wglWin);
     }
   else
     {
     fenetreCourante = -1;
     }
}

