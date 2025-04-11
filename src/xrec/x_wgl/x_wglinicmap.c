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
int MatchColorIndexX(int r, int g, int b, XColor colorTable[], int colDebut, int colFin);

#define DIST(a,b,c) a*a + b*b + c*c

XStandardColormap cmapInfo;
unsigned int colorbitrange[3][3];

static char *les_couleurs[] = {"white", "black", "red", "cyan", "yellow", "magenta", "green2", "blue", "gray"};

void x_wglinicmap()
{
   int i,plane_masks;
   XColor exact[16],col;
   Colormap defcmap;
   XStandardColormap *std_colormap_return;
   int ncmaps;
   Status res;

   int couleurderemplacement;
   int ncolormaps;
   
   long vinfo_mask;
   XVisualInfo vtemplate, *vinfo_template,*visualList,*defaultVis;
   Visual *defaultVisual;
   int nitems_return;
   
   int nbPixs;

   defaultVisual = DefaultVisual(wglDisp, wglScrNum);
   visualList = XGetVisualInfo(wglDisp, 0x0, &vtemplate, &nitems_return);
   
   for (i=0; i < nitems_return; i++)
      {
      if (visualList[i].visual == defaultVisual)
         {
         visInfo = visualList[i];
         }
      }
   
   if (visInfo.class == PseudoColor)
      {
      visualClass = PseudoColor;
      }
   else
      {
      if (visInfo.class != TrueColor && visInfo.class != DirectColor)
         {
         fprintf(stderr, "Type de visuel %d non supporte!\n\n", visInfo.class);
         exit(1);
         }
      else
         {
         visualClass = visInfo.class;
         }
      }
   
   if (debugMode)
      {
      fprintf(stderr, "Visual de type %d\n", visualClass);
      }
   
   defcmap = DefaultColormap(wglDisp, wglScrNum); 
   
   if (visualClass == PseudoColor)
     {
       for (i=0; i < 255; i++)
         {
	   xcouleurs[i].pixel = i;
	   wglColorTable[i] = i;
         }
       XQueryColors(wglDisp, defcmap, xcouleurs, 255);
     }
       
   for (i=0; i < 9; i++)
     {
       res = XAllocNamedColor(wglDisp, defcmap, les_couleurs[i],   &exact[i],  &col);
       if (res == 0)
	 {
	   if (visualClass == PseudoColor)
	     XQueryColors(wglDisp, defcmap, xcouleurs, 255);
	   res = XLookupColor(wglDisp, defcmap, les_couleurs[i],  &exact[i], &col);
	   couleurderemplacement = MatchColorIndexX(col.red/256, col.green/256, col.blue/256, 
                                                    (XColor *) couleurs, 0, 254);
	   exact[i] = xcouleurs[couleurderemplacement];
	 }
     }
   
   if (visualClass != PseudoColor)
      {
      x_wglbitfinder(visInfo.red_mask,   &colorbitrange[0][0], &colorbitrange[0][1]);
      x_wglbitfinder(visInfo.green_mask, &colorbitrange[1][0], &colorbitrange[1][1]);
      x_wglbitfinder(visInfo.blue_mask,  &colorbitrange[2][0], &colorbitrange[2][1]);
      colorbitrange[0][2] = 1 << (colorbitrange[0][1]-colorbitrange[0][0]+1);
      colorbitrange[1][2] = 1 << (colorbitrange[1][1]-colorbitrange[1][0]+1);
      colorbitrange[2][2] = 1 << (colorbitrange[2][1]-colorbitrange[2][0]+1);
      /*      colorbitrange[0][2] = 256 / colorbitrange[0][2];
	      colorbitrange[1][2] = 256 / colorbitrange[1][2];
	      colorbitrange[2][2] = 256 / colorbitrange[2][2];
      */
      }

   if (visualClass == PseudoColor)
     {
       for (i=0; i < 9; i++)
         {
	   xcouleurs[exact[i].pixel] =  exact[i];
	   wglColorTable[i] = exact[i].pixel;
         }
     }
   else
     {
       for (i=0; i < 9; i++)
         {
	   xcouleurs[i] =  exact[i];
	   wglColorTable[i] = exact[i].pixel;
         }
     }
   
   
   
   cmap = (int) defcmap;
   
   if (cmap_strategy == READ_WRITE_COLORMAP)
      {
      if (visualClass == PseudoColor)
         {
         for (i=0; i < 256; i++)
            {
            wglWritablePixs[i] = False;
            }
         
         cmap = (int) defcmap;
         
         i = 240;
         res = 0;
         while (res == 0 && i > 0)
            {
               res = XAllocColorCells(wglDisp, (Colormap) cmap, False, (unsigned long *) &plane_masks,0, (unsigned long *) pix,i);
            i--;   
            } 
         
         if (res == 0)
            {
            x_wglinstcmap();
            }
         
         if (i != 0)
           i++;
         
         if (debugMode)
            {
            printf("Nombre de couleurs disponibles: %d\n", i) ;
            printf("indice debut: %d\n", pix[0]) ;
            printf("indice fin: %d\n", pix[i-1]) ;
            }
         
         nbPixs = i;
         for (i=0; i < nbPixs;i++)
            {
            wglWritablePixs[pix[i]] = True;
            }
         
         i = 0;
         while (pix[i] < 32 && i < nbPixs)
            {
               XFreeColors(wglDisp, defcmap, (unsigned long*) &pix[i], 1, 0);
            wglWritablePixs[pix[i]] = False;
            i++;
            }
         
         }
      }
} 


int MatchColorIndexX(r, g, b, colorTable, colDebut, colFin)
     int r, g, b;
     XColor colorTable[];
     int   colDebut, colFin;
{
   int i, iMin;
   int dr, dg, db, diff, minDiff;
   
   dr = (r - colorTable[colDebut].red/256);
   dg = (g - colorTable[colDebut].green/256);
   db = (b - colorTable[colDebut].blue/256);

   iMin = colDebut;
   minDiff = DIST(dr,dg,db);
   
   for (i=colDebut; i <= colFin; i++)
      {
      dr = (r - colorTable[i].red/256);
      dg = (g - colorTable[i].green/256);
      db = (b - colorTable[i].blue/256);

      diff = DIST(dr,dg,db);
      if (diff < minDiff)
	 {
	 minDiff = diff;
	 iMin = i;
	 }
      }

   if (debugMode)
      {
      printf("Demande: %4d-%4d-%4d, Recu: %4d,%4d,%4d, indice: %4d\n", r,g,b,colorTable[iMin].red/256,
             colorTable[iMin].green/256,colorTable[iMin].blue/256, iMin);
      }
   return iMin;
   }

