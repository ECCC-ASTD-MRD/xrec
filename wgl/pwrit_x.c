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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <wgl_x.h>
#include <rpnmacros.h>


extern SuperWidgetStruct SuperWidget;

extern XFontStruct *fonte;
extern Display *wglDisp;
extern Window  wglDrawable;
extern GC      wglLineGC;
extern XPoint  wglPts[];

int currentFontSize = 12;

/**
 ******
 **/

void f77name(wglfsz)(fontSize)
int *fontSize;
{
   c_wglfsz(*fontSize);
   }


/**
 ******
 **/

int c_wglfsz(fontSize)
int fontSize;
{
   if (fontSize != currentFontSize) 
      InitFonte(fontSize);
   
   }



/**
  ------------------------------------------
  ------------------------------------------
  **/



/**
 ******
 **/

void f77name(wglpsi)(i, j, string, stringLength, size, orient, codeCentrage, ftnStringLength)
int *i, *j;
char string[];
int *stringLength, *size, *orient, *codeCentrage;
int ftnStringLength;
{
   c_wglpsi(*i, *j, string, *stringLength, *size, *orient, *codeCentrage);
   }


/**
 ******
 **/

int c_wglpsi(i, j, string, stringLength, size, orient, codeCentrage)
int i, j;
char string[];
int stringLength, size, orient, codeCentrage;
{
   c_wglfsz(size);
   
   c_wglmvi(i,j);
   XDrawString(wglDisp, wglDrawable, wglLineGC, 
                    wglPts[0].x, wglPts[0].y, string, stringLength);

   }



/**
  ------------------------------------------
  ------------------------------------------
  **/

void f77name(wglpsx)(x, y, string, stringLength, size, orient, codeCentrage, ftnStringLength)
float *x, *y;
char string[];
int *stringLength, *size, *orient, *codeCentrage;
int ftnStringLength;
{
   c_wglpsx(*x, *y, string, *stringLength, *size, *orient, *codeCentrage);
   }


/**
 ******
 **/

int  c_wglpsx(x, y, string, stringLength, size, orient, codeCentrage)
float x, y;
char string[];
int stringLength, size, orient, codeCentrage;
{
   c_wglfsz(size);

   c_wglmvx(x,y);
   XDrawString(wglDisp, wglDrawable, wglLineGC, 
                    wglPts[0].x, wglPts[0].y, string, stringLength);

   }




/**
  ------------------------------------------
  ------------------------------------------
  **/


int InitFonte(fontSize)
int fontSize;
{   
   char fontName[132];

   if (fonte != NULL)
      XFreeFont(wglDisp, fonte);
   
   switch(fontSize)
      {
      case 10:
      fonte = XLoadQueryFont(wglDisp, "times-roman.10");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--10-100-75-75-m-60-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--10-100-75-75-p-54-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 12:
      fonte = XLoadQueryFont(wglDisp, "times-roman.12");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--12-120-75-75-m-70-iso8859-1");
	 
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--12-120-75-75-p-64-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 14:
      fonte = XLoadQueryFont(wglDisp, "times-roman.14");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--14-100-100-100-m-90-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--14-140-75-75-p-74-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 17:
      fonte = XLoadQueryFont(wglDisp, "times-roman.17");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--17-120-100-100-m-100-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--17-120-100-100-p-84-iso8859-1");
	 
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 18:
      fonte = XLoadQueryFont(wglDisp, "times-roman.18");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--18-180-75-75-m-110-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--18-180-75-75-p-94-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      case 24:
      fonte = XLoadQueryFont(wglDisp, "times-roman.24");
      if (fonte == NULL)
	 {
	 fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--24-240-75-75-m-150-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "-adobe-times-medium-r-normal--24-240-75-75-p-124-iso8859-1");
	 if (fonte == NULL)
	    fonte = XLoadQueryFont(wglDisp, "fixed");
	 }
      break;
      
      default:
      fonte = XLoadQueryFont(wglDisp, "fixed");
      break;
      }
   
   
   currentFontSize = fontSize;
   XSetFont(wglDisp, wglLineGC, fonte->fid);
   
   }



/**
  ------------------------------------------
  ------------------------------------------
  **/

int f77name(wglwsi)(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return c_wglwsi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wglwsi(string, stringLength)
char string[];
int stringLength;
{
   return XTextWidth(fonte, string, stringLength);

   }


/**
  ------------------------------------------
  ------------------------------------------
  **/

int wglhsi_(string, stringLength, ftnStringLength)
char string[];
int *stringLength,  ftnStringLength;
{
   return c_wglhsi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wglhsi(string, stringLength)
char string[];
int stringLength;
{
   int ascent, descent;
   int direction;
   XCharStruct fonteInfo;

   XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo);
   return (ascent + descent);

   }




/**
  ------------------------------------------
  ------------------------------------------
  **/

float f77name(wglwsx)();
float c_wglwsx();

float wglwsx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return c_wglwsx(string, *stringLength);
   }


/**
 ******
 **/

float c_wglwsx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon =  c_wglwsi(string, stringLength);
   c_wgliax(&xbidon1, &ybidon1, 0, 0);
   c_wgliax(&xbidon2, &ybidon2, bidon, 0);

   return (xbidon2 - xbidon1);
   }


float f77name(wglhsx)();
float c_wglhsx();

float wglhsx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return c_wglhsx(string, *stringLength);
   }


/**
 ******
 **/

float c_wglhsx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon = c_wglhsi(string, stringLength);
   c_wgliax(&xbidon1, &ybidon1, 0, 0);
   c_wgliax(&xbidon2, &ybidon2, 0, bidon);

   return (ybidon2 - ybidon1);
   }


/**
  ------------------------------------------
  ------------------------------------------
  **/

int f77name(wglasi)(string, stringLength, ftnStringLength)
char string[];
int *stringLength,  ftnStringLength;
{
   return c_wglasi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wglasi(string, stringLength)
char string[];
int stringLength;
{
   int ascent, descent;
   int direction;
   XCharStruct fonteInfo;

   XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo);
   return (ascent);

   }


int f77name(wgldsi)(string, stringLength, ftnStringLength)
char string[];
int *stringLength,  ftnStringLength;
{
   return c_wgldsi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wgldsi(string, stringLength)
char string[];
int stringLength;
{
   int ascent, descent;
   int direction;
   XCharStruct fonteInfo;

   XTextExtents(fonte, string, stringLength, &direction, &ascent, &descent, &fonteInfo);
   return (descent);

   }


float f77name(wglasx)();
float c_wglasx();

float wglasx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return c_wglasx(string, *stringLength);
   }


/**
 ******
 **/

float c_wglasx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon = c_wglasi(string, stringLength);
   c_wgliax(&xbidon1, &ybidon1, 0, 0);
   c_wgliax(&xbidon2, &ybidon2, 0, bidon);

   return (ybidon2 - ybidon1);
   }


float f77name(wgldsx)();
float c_wgldsx();

float wgldsx_(string, stringLength, ftnStringLength)
char string[];
int *stringLength, ftnStringLength;
{
   return c_wgldsx(string, *stringLength);
   }


/**
 ******
 **/

float c_wgldsx(string, stringLength)
char string[];
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   bidon = c_wgldsi(string, stringLength);
   c_wgliax(&xbidon1, &ybidon1, 0, 0);
   c_wgliax(&xbidon2, &ybidon2, 0, bidon);

   return (ybidon2 - ybidon1);
   }


