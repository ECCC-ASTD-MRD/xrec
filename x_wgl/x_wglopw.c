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

int f77name(wglopw)(nomFenetre, lenNomFenetre)
char nomFenetre[];
int  lenNomFenetre;
{
   char nomTemp[255];
   int  winid;

   strncpy(nomTemp, nomFenetre, lenNomFenetre);
   nomTemp[lenNomFenetre] = '\0';
   
   winid = wglopw(nomTemp);
   return winid;
   }

/**
******
**/

int wglopw(nomFenetre)
char *nomFenetre;
{
   char tempNomFenetre[255];

   wglnewwin();

   if (nomFenetre[0] == '?')
      sprintf(tempNomFenetre, "X-%s", &nomFenetre[1]);
   else
      strcpy(tempNomFenetre, nomFenetre);
      
   lng = c_getulng();
   
   wgliniwin(tempNomFenetre);
   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);

   if (!wglForceAspectSet)
      wglForceAspect = True;

   w = wglWinAttr.width;
   h = wglWinAttr.height;

   wglssp(0.0, 0.0, (float) (w -1), (float) (h - 1), 0, 0, 
            w - 1, h - 1, 0);
   usSpace.axex = NULL;
   usSpace.axey = NULL;
   usSpace.ni   = 0;
   usSpace.nj   = 0;
   bgPix = NULL;

   if (!customWinSize)
      {
      wglwtwrsz();
      }

   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);
   wglgwz(&w,&h);

   wglcol(NOIR);
   wglclr(); 

   wglsetw(wglWin);
   wglrstdpar();
   return wglWin;
   }

