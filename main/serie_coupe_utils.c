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

#include <Xm/Xm.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>

#include <xinit.h>
#include <rec.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <souris.h>
#include <math.h>

#define AUTO_PROFIL 0
#define AUTO_GRILLES 1
#define FIXES       2

#define TOPO_INACTIVE 0
#define TOPO_ACTIVE   1

TracerCercle(x, y)
float x,y;
{
   int ii,jj;
   float rx, ry;

   c_xy2fxfy(&rx, &ry, x, y);
   
   c_wgllwi(3);
   c_wglxai(&ii, &jj, rx, ry);
   c_wglcli(ii, jj, 10);
   c_wglmvi(ii, jj-10);
   c_wgldri(ii, jj+10);
   c_wglmvi(ii-10, jj);
   c_wgldri(ii+10, jj);

   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


Ligne(event,fcx1,fcy1,fcx2,fcy2)
int *event;
float *fcx1, *fcy1, *fcx2, *fcy2;
{
   int bouton;
   int x0, y0, xsize, ysize;
   int nbMenus;
   float temp;
   int i;
   int cx1, cx2, cy1, cy2;
   int fenetreAffichage;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

   nbMenus = 0;
   f77name(minisouris)(&bouton, event, 
           &x0, &y0, &xsize, &ysize, 
           &cx1, &cy1, &cx2, &cy2);

   cx1 = (cx1 - x0); 
   cx2 = (cx2 - x0); 
   cy1 = (cy1 - y0); 
   cy2 = (cy2 - y0); 
   
   switch(*event)
      {
      case DRAG:
      break;
      
      case MOVE:
      case CLIC:
      c_wgliax(fcx1, fcy1, cx1, cy1);
      c_wgliax(fcx2, fcy2, cx2, cy2);
      c_fxfy2xy(fcx1, fcy1, *fcx1, *fcy1);
      c_fxfy2xy(fcx2, fcy2, *fcx2, *fcy2);
      break;
      
      case RESET:
      break;
      
      default:
      break;
      }

   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


TracerLigne(rcx1, rcy1, rcx2, rcy2)
float rcx1,rcy1,rcx2,rcy2;
{
   int i, j, n;
   float theta;
   int rayon = 10;
   int i1, j1, i2, j2;
   int poly[4][2];
   float rx1, ry1, rx2, ry2;

   c_xy2fxfy(&rx1, &ry1, rcx1, rcy1);
   c_xy2fxfy(&rx2, &ry2, rcx2, rcy2);

   c_wgllwi(2);
   c_wglmvx(rx1, ry1);
   c_wgldrx(0.920*(rx2-rx1)+rx1, 0.920*(ry2-ry1)+ry1);

   if (rx2 != rx1)
      {
      theta = atan2(ry2-ry1,rx2-rx1);
      }
   else
      {
      if (0 == (ry2-ry1))
	 {
	 theta = 0.;
	 }
      else
	 {
	 if (0 < (ry2 - ry1))
	    theta = M_PI_2;
	 else
	    theta = -M_PI_2;
	 }
      }
   
   for (n = 1; n <= 7; n++)
      {
      rayon = 20 + 20 * ((n+1) % 2);
      i1 = (int)( 0.5 * rayon  * cos (theta + M_PI_2));
      j1 = (int)( 0.5 * rayon  * sin (theta + M_PI_2));

      c_wglxai(&i,&j,(n * 0.125)*(rx2-rx1)+rx1,(n * 0.125)*(ry2-ry1)+ry1);
      c_wglmvi(i+i1, j+j1);
      c_wgldri(i-i1, j-j1);
      }

   rayon = 30;
   i1 = (int)( 0.5 * rayon  * cos (theta + M_PI_2));
   j1 = (int)( 0.5 * rayon  * sin (theta + M_PI_2));
   
   c_wglxai(&i, &j, (0.875)*(rx2-rx1)+rx1,(0.875)*(ry2-ry1)+ry1);
   poly[0][0] = i+i1; poly[0][1] = j+j1;
   poly[3][0] = i+i1; poly[3][1] = j+j1;
   c_wglxai(&i, &j, (0.920)*(rx2-rx1)+rx1,(0.920)*(ry2-ry1)+ry1);
   poly[1][0] = i; poly[1][1] = j;
   c_wglxai(&i, &j, rx2, ry2);
   poly[2][0] = i; poly[2][1] = j;

   c_wglpfi(4, poly);

   c_wglxai(&i, &j, (0.875)*(rx2-rx1)+rx1,(0.875)*(ry2-ry1)+ry1);
   poly[0][0] = i-i1; poly[0][1] = j-j1;
   poly[3][0] = i-i1; poly[3][1] = j-j1;
   c_wglxai(&i, &j, (0.920)*(rx2-rx1)+rx1,(0.920)*(ry2-ry1)+ry1);
   poly[1][0] = i; poly[1][1] = j;
   c_wglxai(&i, &j, rx2, ry2);
   poly[2][0] = i; poly[2][1] = j;

   c_wglpfi(4, poly);

   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

AfficherSymbole(i,j,n)
int i,j,n;
{
   switch(n)
      {
      case 0:
      c_wglcli(i, j, 4);
      break;
      
      case 1:
      c_wglrli(i-4, j-4, i+4, j+4);
      break;
      
      case 2:
      c_wglmvi(i, j+6);
      c_wgldri(i+6, j);
      c_wgldri(i, j-6);
      c_wgldri(i-6,j);
      c_wgldri(i, j+6);
      break;
      
      case 3:
      c_wglmvi(i, j+6);
      c_wgldri(i+6, j-6);
      c_wgldri(i-6, j-6);
      c_wgldri(i, j+6);
      break;
      }
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

