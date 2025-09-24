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

#include <stdio.h>
#include <rmn/rpnmacros.h>
#include <wgl.h>
#include <gd_wglfonct.h>


int gd_currentFontSize = 12;

int gd_wglfsz(fontSize)
int fontSize;
{
  /*   if (fontSize != gd_currentFontSize) 
      InitFonte(fontSize);
  */
   
   }



/**
  ------------------------------------------
  ------------------------------------------
  **/



/**
 ******
 **/

int gd_wglpsi(i, j, string, stringLength, size, orient, codeCentrage)
int i, j;
char string[];
int stringLength, size, orient, codeCentrage;
{
   gd_wglfsz(size);
   
   gd_wglmvi(i,j);
   /*   XDrawString(wglDisp, wglDrawable, wglLineGC, 
	wglPts[0].x, wglPts[0].y, string, stringLength);
   */
   
}



/**
  ------------------------------------------
  ------------------------------------------
  **/

int  gd_wglpsx(x, y, string, stringLength, size, orient, codeCentrage)
float x, y;
char string[];
int stringLength, size, orient, codeCentrage;
{

   f77name(wglfsz)(&size);


   f77name(wglmvx)(&x,&y);
   /*   XDrawString(wglDisp, wglDrawable, wglLineGC, 
                    wglPts[0].x, wglPts[0].y, string, stringLength);
   */
   }




/**
  ------------------------------------------
  ------------------------------------------
  **/


int gdInitFonte(fontSize)
int fontSize;
{   
   char fontName[132];

   gd_currentFontSize = fontSize;
   /*    XSetFont(wglDisp, wglLineGC, fonte->fid); */
   
   }



/**
  ------------------------------------------
  ------------------------------------------
  **/

int  gd_wglwsi(string, stringLength)
char string[];
int stringLength;
{
  /*    return XTextWidth(fonte, string, stringLength);*/

   }


/**
  ------------------------------------------
  ------------------------------------------
  **/

int wglhsi_(string, stringLength, ftnStringLength)
char string[];
int *stringLength,  ftnStringLength;
{
   return gd_wglhsi(string, *stringLength);
   }


/**
 ******
 **/

int  gd_wglhsi(string, stringLength)
char string[];
int stringLength;
{
   int ascent, descent;
   int direction;
   /*   XCharStruct fonteInfo; */

   /*    XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo);*/
   return (ascent + descent);

   }




/**
  ------------------------------------------
  ------------------------------------------
  **/

float gd_wglwsx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon =  gd_wglwsi(string, stringLength);
   gd_wgliax(&xbidon1, &ybidon1, 0, 0);
   gd_wgliax(&xbidon2, &ybidon2, bidon, 0);

   return (xbidon2 - xbidon1);
   }


float gd_wglhsx(char*, int);

float wglhsx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return gd_wglhsx(string, *stringLength);
   }


/**
 ******
 **/

float gd_wglhsx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon = gd_wglhsi(string, stringLength);
   gd_wgliax(&xbidon1, &ybidon1, 0, 0);
   gd_wgliax(&xbidon2, &ybidon2, 0, bidon);

   return (ybidon2 - ybidon1);
   }


/**
  ------------------------------------------
  ------------------------------------------
  **/

int  gd_wglasi(string, stringLength)
char string[];
int stringLength;
{
   int ascent, descent;
   int direction;
   /*    XCharStruct fonteInfo; */

   /*   XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo); */
   return (ascent);

   }


int  gd_wgldsi(string, stringLength)
char string[];
int stringLength;
{
   int ascent, descent;
   int direction;
   /*   XCharStruct fonteInfo; */

   /*    XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo); */
   return (descent);

   }


float gd_wglasx(char *, int);

float wglasx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return gd_wglasx(string, *stringLength);
   }


/**
 ******
 **/

float gd_wglasx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon = gd_wglasi(string, stringLength);
   gd_wgliax(&xbidon1, &ybidon1, 0, 0);
   gd_wgliax(&xbidon2, &ybidon2, 0, bidon);

   return (ybidon2 - ybidon1);
   }


float gd_wgldsx(char *, int);

float wgldsx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return gd_wgldsx(string, *stringLength);
   }


/**
 ******
 **/

float gd_wgldsx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon = gd_wgldsi(string, stringLength);
   gd_wgliax(&xbidon1, &ybidon1, 0, 0);
   gd_wgliax(&xbidon2, &ybidon2, 0, bidon);

   return (ybidon2 - ybidon1);
   }


