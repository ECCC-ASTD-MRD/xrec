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

#include <wgl.h>
#include <gmp.h>

extern GeoMapFlagsStruct   mapFlags;
f77name(tgeo)()
{
   int ia, ja, ib, jb;
   int i;
   float xg1,xg2,xg3,xg4;
   int ig1, ig2, ig3, ig4;
   char grtyp;
   int ni,nj;

   c_wglscon("x");
   x_wglopw("test-geo");
   mapFlags.continents = OUI;
   

   /*   c_gmpopt("GRID", 5);
   c_wglcol(BLANC);
   c_wglssp(1., 1., 151., 151., 0,0,0,0,0);
   c_wgldbf();
   c_wglbbf();
   grtyp = 'T';
   ni = 151;
   nj = 151;
   xg1 = 100000.0;
   xg2 = 0.0;
   xg4 = 260.0;

   c_gmpopt("RESOLUTION", 1);
   c_gmpopt("OUTLINE_COLOR", ROUGE);
   c_gmpopt("GRID_COLOR", ROUGE);
   for (i=-90; i < 90; i+=2)
     {
     xg3 = i * 1.0;
     f77name(cxgaig)(&grtyp,&ig1, &ig2, &ig3, &ig4, &xg1, &xg2, &xg3, &xg4);
     c_gmpset('T', 151, 151, ig1, ig2, ig3, ig4);
     c_wglcol(BLANC);
     c_wglclr();
     c_gmpdrw();
     c_wglswb();
     }
   while (!(c_wglbtn(BGAUCH)));


   for (i=0; i < 360; i+=2)
     {
     xg3 = 45.0;
     xg4 = i * 1.0;
     f77name(cxgaig)(&grtyp,&ig1, &ig2, &ig3, &ig4, &xg1, &xg2, &xg3, &xg4);
     c_gmpset('T', 151, 151, ig1, ig2, ig3, ig4);
     c_wglcol(BLANC);
     c_wglclr();
     c_gmpdrw();
     c_wglswb();
     }
   while (!(c_wglbtn(BGAUCH)));
   

   return;
   */
   c_gmpopti("GRID", 5);
   c_wglcol(BLANC);
   c_wglclr();
   c_wglssp(1., 1., 121., 61., 0,0,0,0,0);
   c_gmpset('L', 121, 61, 3, 7, 13890, 24020);
   c_gmpopti("RESOLUTION", 1);
   c_gmpopti("OUTLINE_COLOR", ROUGE);
   c_gmpopti("GRID_COLOR", ROUGE);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_gmpopti("GRID", 5);
   c_wglcol(BLANC);
   c_wglclr();
   c_wglssp(1., 1., 360., 180., 0,0,0,0,0);
   c_gmpset('G', 360, 180, 0, 0, 0, 0);
   c_gmpopti("RESOLUTION", 1);
   c_gmpopti("OUTLINE_COLOR", ROUGE);
   c_gmpopti("GRID_COLOR", ROUGE);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_gmpopti("GRID", 5);
   c_wglcol(BLANC);
   c_wglclr();
   c_wglssp(1., 1., 360., 180., 0,0,0,0,0);
   c_gmpset('E', 360, 180, 1410, 370, 55520, 49600);
   c_gmpopti("RESOLUTION", 1);
   c_gmpopti("OUTLINE_COLOR", ROUGE);
   c_gmpopti("GRID_COLOR", ROUGE);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(BLANC);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
   
   c_wglcol(BLANC);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
   
   c_wglcol(BLANC);
   c_wglclr();
   c_wglssp(0., 0., 360., 180., 0,0,0,0,0);
   c_gmpset('E', 360, 180, 900, 0, 43200, 43200);
   c_gmpopti("RESOLUTION", 10);
   c_gmpopti("OUTLINE_COLOR", BLEU);
   c_gmpopti("GRID_COLOR", BLEU);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
   
   c_wglcol(BLANC);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
   
   c_wglcol(BLANC);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
   
   for (i=0000; i <= 36000; i+=3000)
      {
      c_wglcol(NOIR);
      c_wglclr();
      c_gmpopti("OUTLINE_COLOR", 5);
      c_gmpopts("OUTLINE", "GEOPOL");
      c_gmpopti("GRID_COLOR", 5);
      c_gmpopti("GRID", 5);
      c_wglssp(1., 1., 181., 181., 0, 0, 0, 0, 0);
      c_gmpset('L', 181, 181, 100,  100, 0, i);
      c_gmpdrw();
      while (!(c_wglbtn(BGAUCH)));
      c_wglcol(NOIR);
      c_wglclr();
      c_gmpdrw();
      while (!(c_wglbtn(BGAUCH)));
      c_wglcol(NOIR);
      c_wglclr();
      c_gmpdrw();
      while (!(c_wglbtn(BGAUCH)));
      }

   c_wglcol(NOIR);
   c_wglclr();
   c_wglssp(1., 1., 851., 651., 0, 0, 0, 0, 0);
   c_gmpset('L', 851, 651, 20, 20, 2500, 17000);
   c_gmpopti("RESOLUTION", 200);
   c_gmpopti("OUTLINE_COLOR", JAUNE);
   c_gmpopti("GRID_COLOR", JAUNE);
   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   return;

   c_wglcol(NOIR);
   c_wglclr();
/**   c_gmpopts("RESOLUTION", 200); **/

   c_wglssp(1., 1., 301., 151., 0, 0, 0, 0, 0);
   c_gmpopti("GRID", 5);
   c_gmpopts("OUTLINE", "GEOPOL");
   c_gmpopti("OUTLINE_COLOR", JAUNE);
   c_gmpopti("GRID_COLOR", 4);
   c_gmpset('B', 301, 151, 0, 0, 0, 0);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_gmpopti("RESOLUTION", 50);
   c_gmpopti("GRID", 0);
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopts("OUTLINE", "GEO");
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopts("OUTLINE", "GEOPOL");
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopts("OUTLINE", "GEO");
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));


   c_wglcol(NOIR);
   c_wglclr();
   c_wglssp(1., 1., 21., 25., 0, 0, 0, 0, 0);
   c_gmpset('N', 21, 25, 650, 130, 34710, 1000);
   c_gmpopti("OUTLINE_COLOR", 5);
   c_gmpopti("GRID_COLOR", 5);
   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();

   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();

   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();

   c_wglssp(1., 1., 648., 400., 0, 0, 0, 0, 0);
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpset('L', 648,400,8,    8, 11900, 34900);
   c_gmpopti("OUTLINE_COLOR", 0);
   c_gmpopti("GRID_COLOR", 0);
   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();


   c_wglcol(6);
   c_wglrlx(1.0, 1.0, 151.0, 100.0);

   c_wglcol(BLANC);
   c_wglssp(1.0, 1.0, 151.0, 100.0, 0, 0, 200, 200, 0);
   c_gmpset('N',  151, 100, 820,  465,  2100,  1000);
   c_gmpdrw();

   c_wglssp(1.0, 1.0, 151.0, 100.0, 200, 000, 400, 200, 0);
   c_gmpdrw();
 
   c_wglssp(1.0, 1.0, 151.0, 100.0, 200, 200, 400, 400, 0);
   c_gmpdrw();
 
   c_wglssp(1.0, 1.0, 151.0, 100.0, 000, 200, 200, 400, 0);
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));

   c_wglssp(10.0, 10.0, 45.0, 45.0, 0,0,0,0,0);
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();

   c_wglcol(6);
   c_wglrlx(1.0, 1.0, 151.0, 100.0);

   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopti("OUTLINE_COLOR", 2);
   c_gmpopti("GRID_COLOR", 7);
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopts("OUTLINE_STYLE", "DASH");
   c_gmpopts("GRID_STYLE", "DASH");
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopts("OUTLINE_STYLE", "POINT");
   c_gmpopts("GRID_STYLE", "POINT");
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_wglssp(1., 1., 51., 55., 0, 0, 0, 0, 0);
   c_gmpset('N',  51, 55, 280,  160,  35000,  3810);
   c_gmpdrw();

   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopti("OUTLINE_COLOR", 7);
   c_gmpopti("GRID_COLOR", 7);
   c_gmpopts("OUTLINE_STYLE", "LINE");
   c_gmpopti("OUTLINE_THICK", 4);
   c_gmpopts("GRID_STYLE", "LINE");
   c_gmpopti("GRID", 30);
   c_gmpopti("GRID_THICK", 4);
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopti("OUTLINE_COLOR", 3);
   c_gmpopti("GRID_COLOR", 3);
   c_gmpopti("OUTLINE_THICK", 1);
   c_gmpopti("GRID_THICK", 1);
   c_gmpdrw();
 
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_wglssp(1., 1., 50., 50., 0, 0, 0, 0, 0);
   c_gmpset('L', 50, 50, 100,  100, 12000, 24001);
   c_gmpopts("OUTLINE", "CON_LR");
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));

   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopti("GRID", 10);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
 
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopti("GRID", 5);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));
 
   c_wglcol(NOIR);
   c_wglclr();
   c_gmpopti("GRID", 0);
   c_gmpdrw();
   while (!(c_wglbtn(BGAUCH)));



   }
