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

int printbidon(char *string, float xxx1, float yyy1, float xxx2, float yyy2, 
	       int iii1, int jjj1, int iii2, int jjj2);
void c_wglssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);

 int x_wglopw(char *nomFenetre)
{
   char tempNomFenetre[255];
   float xxx1, yyy1, xxx2, yyy2;
   int iii1, www1, iii2, www2;

   x_wglinit();
   x_wglnewwin();

      
   lng = c_getulng();
   
   iii1 = 0;
   www1 = 0;
   iii2 = 640;
   www2 = 480;
   xxx1 = 0.0;
   xxx2=1.0*iii2;
   yyy1= 0.0;
   yyy2=1.0*www2;
   
      if (nomFenetre[0] == '?')
      sprintf(tempNomFenetre, "X-%s", &nomFenetre[1]);
   else
      strcpy(tempNomFenetre, nomFenetre);
   
   printf("printf fonction %f %f %f %f - %d %d %d %d\n ", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
   printf("printf etoiles %x %x %x %x - %x %x %x %x\n ", &xxx1, &yyy1, &xxx2, &yyy2, &iii1, &www1, &iii2, &www2);
   printbidon("avant x_wgliniwin", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
   printbidonetoile("avant x_wgliniwin", &xxx1, &yyy1, &xxx2, &yyy2, &iii1, &www1, &iii2, &www2);
       x_wgliniwin(tempNomFenetre);
   printbidon("apres x_wgliniwin", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);

   if (!wglForceAspectSet)
      wglForceAspect = True;

   XGetWindowAttributes(wglDisp, wglWin, &wglWinAttr);

   printbidon("apres x_getwindowattrib", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);

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
   
   printbidon("avant wglssp", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
   
   c_wglssp(xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2, 0);
   printbidon("apres  wglssp", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
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
   c_wglgwz(&w,&h);

   c_wglcol(NOIR);
   c_wglclr(); 

   c_wglsetw(wglWin);
   x_wglrstdpar();

   return wglWin;

   }

