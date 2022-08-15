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

#include "wgl.h"
#include "rpn_f.xbm"
#include "rpn_e.xbm"
#include "service_f.xbm"
#include "service_e.xbm"
#include <stdio.h>
#include "xrec_build_info.h"
#include "rec_version.h"
#include "rec.h"
#include "rec_functions.h"

int logo_width, logo_height;
char *logo_bits;

void draw_rpn_logo()
{
   int i,j,k,n;
   int langue;
   int largeurFenetre, hauteurFenetre;

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   get_rpn_logo_size(&logo_width, &logo_height);
   draw_logo((largeurFenetre-logo_width)/2,
	     (hauteurFenetre-logo_height)/2+(service_f_height/2), logo_width, logo_height, logo_bits, NOIR);
   
   get_sea_logo_size(&logo_width, &logo_height);
   draw_logo((largeurFenetre-logo_width)/2, 10, logo_width, logo_height, logo_bits, NOIR);

   }

void draw_rec_version()
{
   int i,j,k,n;
   int langue;
   int largeurFenetre, hauteurFenetre;
   int hauteurVersion, largeurVersion, descent;
   char tmpStr[256];

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglfsz(18);
   descent = c_wgldsi("1234", 4);
   
   langue = c_getulng();
   
   strcpy(tmpStr, PROJECT_VERSION_STRING);
   largeurVersion = c_wglwsi(tmpStr, strlen(tmpStr));
   c_wglcol(BLANC);
   c_wglpsi((largeurFenetre-largeurVersion)/2,hauteurFenetre-5*descent,
            tmpStr, strlen(tmpStr), 18, 0, 0);
   c_wglcol(NOIR);
}

void get_rpn_logo_size(int *largeurLogo, int *hauteurLogo)
{
   int langue;
   
   langue = c_getulng();
   
   switch(langue)
      {
      case 0:
      *largeurLogo = rpn_f_width;
      *hauteurLogo = rpn_f_height;
      logo_bits   = rpn_f_bits;
      break;
      
      case 1:
      *largeurLogo = rpn_e_width;
      *hauteurLogo = rpn_e_height;
      logo_bits   = rpn_e_bits;
      break;
      }
   }

void get_sea_logo_size(int *largeurLogo, int *hauteurLogo)
{
   int langue;
   
   langue = c_getulng();
   
   switch(langue)
      {
      case 0:
      *largeurLogo = service_f_width;
      *hauteurLogo = service_f_height;
      logo_bits   = service_f_bits;
      break;
      
      case 1:
      *largeurLogo = service_e_width;
      *hauteurLogo = service_e_height;
      logo_bits   = service_e_bits;
      break;
      }
   }


void draw_logo(int idebut, int jdebut, int width, int height, char bits[], int fore)
{
   int i,j,k,n;
   int bytes_per_line;
   char temp, pix;
   struct p { short x,y;} pts[256];
   int npts;

   c_wglcol(fore);
   bytes_per_line = (width + 7)/8;

   npts = 0;
   for (j = 0; j < height; j++)
      {
      for (i=0; i < bytes_per_line; i++)
	 {
	 n = (i + j * bytes_per_line);
	 for (k=0; k < 8; k++)
	    {
#if defined (C910)
	    if (bits[n] > 0)
#endif
	       {
	       temp = bits[n];
	       pix = (char) temp >> (k);
	       pix = pix % 2;
	       if (pix)
		  {
		  pts[npts].x = i*8+k+idebut;
		  pts[npts].y = jdebut +height -j;
		  npts++;
		  if (npts == 256)
		     {
		     c_wglptis(pts, npts);
		     npts = 0;
		     }
		  }
	       }
	    }
	 }
      }
   c_wglptis(pts, npts);
   
   }

