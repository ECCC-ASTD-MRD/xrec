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

x_wgldefncw(int winid)
{
   int i = 0;
   
   while (i < nbFenetresActives && (Window)winid != fenetre[i].wglWin)
      i++;

   if (i == nbFenetresActives)
      {
      printf("Fenetre non existante\n\n");
      exit(1);
      }

   fenetreCourante = i;

   fenetre[fenetreCourante].libre = False;
   wglWin             = fenetre[fenetreCourante].wglWin;
   wglDrawable        = fenetre[fenetreCourante].wglDrawable;
   wglLineGC          = fenetre[fenetreCourante].wglLineGC;
   wglFillGC          = fenetre[fenetreCourante].wglFillGC;
   bgPix              = fenetre[fenetreCourante].bgPix;
   wglWinAttr         = fenetre[fenetreCourante].wglWinAttr;
   wglWinSetAttr      = fenetre[fenetreCourante].wglWinSetAttr;
   aspectRatio        = fenetre[fenetreCourante].aspectRatio;
   wglForceAspect     = fenetre[fenetreCourante].wglForceAspect;
   customPlaneMask    = fenetre[fenetreCourante].customPlaneMask;
   doubleBufferMode   = fenetre[fenetreCourante].doubleBufferMode;
   currentColor       = fenetre[fenetreCourante].currentColor;
   currentFillStyle   = fenetre[fenetreCourante].currentFillStyle;
   currentFillPattern = fenetre[fenetreCourante].currentFillPattern;
   currentDashPattern = fenetre[fenetreCourante].currentDashPattern;
   currentLineWidth   = fenetre[fenetreCourante].currentLineWidth;
   currentPlaneMask   = fenetre[fenetreCourante].currentPlaneMask;
   /*   pixmapDimensions   = fenetre[fenetreCourante].pixmapDimensions; */
   usSpace            = fenetre[fenetreCourante].usSpace;

   c_wglgwz(&w,&h);
   }

