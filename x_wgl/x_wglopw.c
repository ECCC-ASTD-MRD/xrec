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

void c_wglssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);

 int x_wglopw(char *nomFenetre)
{
   char tempNomFenetre[255];
   float xxx1, yyy1, xxx2, yyy2;
   int iii1, www1, iii2, www2;

   x_wglinit();

      
   lng = c_getulng();
   
      if (nomFenetre[0] == '?')
      sprintf(tempNomFenetre, "X-%s", &nomFenetre[1]);
   else
      strcpy(tempNomFenetre, nomFenetre);
   
   if (!wglForceAspectSet)
      wglForceAspect = True;

   x_wglnewwin();
   x_wgliniwin(tempNomFenetre);
   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);

   w = wglWinAttr.width;
   h = wglWinAttr.height;

   iii1 = 0;
   www1 = 0;
   iii2 = wglWinAttr.width-1;
   www2 = wglWinAttr.height-1;
   xxx1 = 0.0;
   xxx2=1.0*iii2;
   yyy1= 0.0;
   yyy2=1.0*www2;
   
   c_wglssp(xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2, 0);
   usSpace.axex = NULL;
   usSpace.axey = NULL;
   usSpace.ni   = 0;
   usSpace.nj   = 0;
   bgPix = NULL;
   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);

   if (!customWinSize)
      {
      x_wglwtwrsz();
      }

   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);
   x_wglgwz(&w,&h);

   x_wglcol(NOIR);
   x_wglclr(); 

   x_wglsetw(wglWin);
   x_wglrstdpar();

   return wglWin;

   }

