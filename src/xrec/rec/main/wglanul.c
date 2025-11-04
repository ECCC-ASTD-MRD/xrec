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

#include <rmn/rpnmacros.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <curseurs.xbm>
#include <wgl_x.h>

Cursor curseur[6];
Boolean ignoreMode;

void testc()
{
   XColor exactn,exactb,b,n;

   Pixmap pix[6],ipix[6];
   Pixel blanc,noir;
   
   int i,i0,j0,res;

   int winid;

   GetFenetreAffichageID(&winid);
   pix[0] = XCreatePixmapFromBitmapData(wglDisp,winid,b00_bits,b00_width,
				     b00_height,0,1,1);

   pix[1] = XCreatePixmapFromBitmapData(wglDisp,winid,b30_bits,b30_width,
				     b30_height,0,1,1);

   pix[2] = XCreatePixmapFromBitmapData(wglDisp,winid,b60_bits,b60_width,
				     b60_height,0,1,1);

   pix[3] = XCreatePixmapFromBitmapData(wglDisp,winid,b90_bits,b90_width,
				     b90_height,0,1,1);
   
   pix[4] = XCreatePixmapFromBitmapData(wglDisp,winid,b120_bits,b120_width,
				     b120_height,0,1,1);

   pix[5] = XCreatePixmapFromBitmapData(wglDisp,winid,b150_bits,b150_width,
				     b150_height,0,1,1);


   ipix[0] = XCreatePixmapFromBitmapData(wglDisp,winid,b00i_bits,b00i_width,
				     b00i_height,0,1,1);

   ipix[1] = XCreatePixmapFromBitmapData(wglDisp,winid,b30i_bits,b30i_width,
				     b30i_height,0,1,1);

   ipix[2] = XCreatePixmapFromBitmapData(wglDisp,winid,b60i_bits,b60i_width,
				     b60i_height,0,1,1);

   ipix[3] = XCreatePixmapFromBitmapData(wglDisp,winid,b90i_bits,b90i_width,
				     b90i_height,0,1,1);
   
   ipix[4] = XCreatePixmapFromBitmapData(wglDisp,winid,b120i_bits,b120i_width,
				     b120i_height,0,1,1);

   ipix[5] = XCreatePixmapFromBitmapData(wglDisp,winid,b150i_bits,b150i_width,
				     b150i_height,0,1,1);

   res = XAllocNamedColor(wglDisp, DefaultColormap(wglDisp, wglScrNum), "black",   &exactn,  &n);
   res = XAllocNamedColor(wglDisp, DefaultColormap(wglDisp, wglScrNum), "red",     &exactb, &b);

   for (i=0; i < 6; i++)
      {
      curseur[i] = XCreatePixmapCursor(wglDisp, pix[i], ipix[i], &b, &n,16,16);
      }

  }
extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

int f77name(wglanul)()
{
   return c_wglanul();
   }

int c_wglanul()
{
   int fenetreAffichage,fenetreCoupe;
   static int once = -1;
   int winid;
   GetFenetreAffichageID(&fenetreAffichage);
   CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
   
#ifdef X_WGL
   if (once == -1)
      {
      testc();
      once = 0;
      }
   
   FlusherTousLesEvenements();

   SetCurseur(fenetreAffichage);
   if (fenetreCoupe)
      {
      SetCurseur(fenetreCoupe);
      }
#endif
   return xc.annulationDemandee;
   }

void UnSetCurseur(Window window)
{
#ifdef X_WGL
   XDefineCursor(wglDisp,window,None);
#endif
   }

void SetIgnoreMode()
{
   ignoreMode = TRUE;
   }

void UnsetIgnoreMode()
{
  ignoreMode = FALSE;
   }

void SetCurseur(Window window)
{
#ifdef X_WGL
   static int i = 0;
   int i0,j0;
   Arg args[3];

   if (ignoreMode)
      return;

   if (xc.annulationDemandee)
      {
      XDefineCursor(wglDisp,window,None);
      }
   else
      {
      XDefineCursor(wglDisp,window,curseur[i]); 
      i++;
      i = i % 6;
      }
#endif
   }
