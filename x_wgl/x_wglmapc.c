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

x_wglmapc(int colorIndex, int r, int g, int b)
{
   int res;
   XColor xcol;
   int coldebut,colfin;
   
   coldebut = 0;
   
   if (visualClass == PseudoColor)
     {
       colorIndex = colorIndex % 256;
       colfin   = 254;
     }
   
   if (colorIndex < 32 && !customPlaneMask)
     return;
   
   if (debugMode)
     printf("indice: %d\n", colorIndex);
   
   xcol.pixel = colorIndex;
   xcol.red = (unsigned short) r;
   xcol.green = (unsigned short) g;
   xcol.blue = (unsigned short) b;
   xcol.red *= 256;
   xcol.green *= 256;
   xcol.blue *= 256;
   
   xcol.flags = DoRed | DoGreen | DoBlue;

   couleurs[colorIndex].r = r;
   couleurs[colorIndex].g = g;
   couleurs[colorIndex].b = b;
   
   switch (visualClass)
     {
     case PseudoColor:
       switch (cmap_strategy)
	 {
	 case READ_ONLY_COLORMAP:
	   wglColorTable[colorIndex]= MatchColorIndexX(r, g, b, xcouleurs, coldebut, colfin);
	   if (wglColorTable[colorIndex] > colfin)
	     {
	       fprintf(stderr,"################################!!!!!!!!!!!!!\n");
	       exit(1);
	     }
	   break;
	   
	 default:
	   if (wglWritablePixs[colorIndex] == False)
	     {
	       if (debugMode)
		 printf("Collision avec le colormap de defaut a l'indice: %d\n", colorIndex);
	       x_wglinstcmap();
	       x_wglsetwcmap();
	     }
	   XStoreColor(wglDisp, (Colormap) cmap, &xcol);
	   xcouleurs[colorIndex] = xcol;
	   wglColorTable[colorIndex] = xcol.pixel;
	   break;
	 }
       break;
       
      default:
        res = XAllocColor(wglDisp, (Colormap) cmap, &xcol); 
        xcouleurs[colorIndex] = xcol;
        wglColorTable[colorIndex] = xcol.pixel;
        break;
      }
   
   
   XFlush(wglDisp);
   
}
