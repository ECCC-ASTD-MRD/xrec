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
#include <wgl.h>
#include <rpnmacros.h>

int currentFontSize = 12;

/**
 ******
 **/

void f77name(wglfsz)(int *fontSize);
int c_wglfsz(int fontSize);

void f77name(wglpsi)(int *i, int *j, char *string, int *stringLength, int *size, 
		     int *orient, int *codeCentrage, int ftnStringLength);
int c_wglpsi(int i, int j, char *string, int stringLength, int size, int orient, int codeCentrage);

/* ----------------------------------------------------------------------------------------- */

void f77name(wglpsx)(float *x, float *y, char *string, int *stringLength, int *size, 
		     int *orient, int *codeCentrage, int ftnStringLength);
int  c_wglpsx(float x, float y, char *string, int stringLength, int size, 
	      int orient, int codeCentrage);
/* ----------------------------------------------------------------------------------------- */


int f77name(wglwsi)(char string[], int *stringLength, int ftnStringLength);
int  c_wglwsi(char string[], int stringLength);

/* ----------------------------------------------------------------------------------------- */

int f77name(wglhsi)(char string[], int *stringLength, int ftnStringLength);
int  c_wglhsi(char string[], int stringLength);

float f77name(wglwsx)(char string[], int *stringLength, int ftnStringLength);
float c_wglwsx(char *string, int stringLength);

float f77name(wglhsx)(char *string,int *stringLength, int ftnStringLength);
float c_wglhsx(char *string, int stringLength);

int f77name(wglasi)(char *string, int *stringLength,  int ftnStringLength);
int  c_wglasi(char *string,int stringLength);

int f77name(wgldsi)(char *string, int *stringLength,  int ftnStringLength);
int  c_wgldsi(char *string, int stringLength);

float wglasx_(char *string, int *stringLength, int ftnStringLength);
float c_wglasx(char *string, int stringLength);

float f77name(wgldsx)(char *string, int *stringLength, int ftnStringLength);
float c_wgldsx(char *string, int stringLength);
/* ----------------------------------------------------------------------------------------- */

void f77name(wglfsz)(int *fontSize)
{
  c_wglfsz(*fontSize);
}


/**
******
**/

int c_wglfsz(int fontSize)
{
  return wglc_wgl->wglfsz(fontSize);
}

/* ----------------------------------------------------------------------------------------- */

void f77name(wglpsi)(int *i, int *j, char *string, int *stringLength, 
		     int *size, int *orient, int *codeCentrage, int ftnStringLength)
{
  c_wglpsi(*i, *j, string, *stringLength, *size, *orient, *codeCentrage);
}


/**
******
**/

int c_wglpsi(int i, int j, char *string, int stringLength, int size, int orient, int codeCentrage)
{
  return wglc_wgl->wglpsi(i, j, string, stringLength, size, orient, codeCentrage);
   }


/* ----------------------------------------------------------------------------------------- */

void f77name(wglpsx)(float *x, float *y, char *string, int *stringLength, int *size, 
		     int *orient, int *codeCentrage, int ftnStringLength)
{
  string[ftnStringLength] = '\0';
  c_wglpsx(*x, *y, string, *stringLength, *size, *orient, *codeCentrage);
}


/**
 ******
 **/

int  c_wglpsx(float px, float py, char *string, int stringLength, int size, 
	      int orient, int codeCentrage)
{
  return wglc_wgl->wglpsx(px, py, string, stringLength, size, orient, codeCentrage);
}



/* ----------------------------------------------------------------------------------------- */


int f77name(wglwsi)(char *string, int *stringLength, int ftnStringLength)
{
   return c_wglwsi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wglwsi(char *string, int stringLength)
{
  return wglc_wgl->wglwsi(string, stringLength);

   }

/* ----------------------------------------------------------------------------------------- */

int f77name(wglhsi)(string, stringLength, ftnStringLength)
char *string;
int *stringLength,  ftnStringLength;
{
   return c_wglhsi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wglhsi(string, stringLength)
char *string;
int stringLength;
{
  return wglc_wgl->wglhsi(string, stringLength);
   }




/**
  ------------------------------------------
  ------------------------------------------
  **/


float wglwsx_(string, stringLength, ftnStringLength)
char *string;
int *stringLength, ftnStringLength;
{
   return c_wglwsx(string, *stringLength);
   }


/**
 ******
 **/

float c_wglwsx(string, stringLength)
char *string;
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   return wglc_wgl->wglwsx(string, stringLength);
   }


float f77name(wglhsx)(string, stringLength, ftnStringLength)
char *string;
int *stringLength, ftnStringLength;
{
   return c_wglhsx(string, *stringLength);
   }


/**
 ******
 **/

float c_wglhsx(string, stringLength)
char *string;
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   return wglc_wgl->wglhsx(string, stringLength);
   }


/**
  ------------------------------------------
  ------------------------------------------
  **/

int f77name(wglasi)(string, stringLength, ftnStringLength)
char *string;
int *stringLength,  ftnStringLength;
{
   return c_wglasi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wglasi(string, stringLength)
char *string;
int stringLength;
{
   return wglc_wgl->wglasi(string, stringLength);
   }


int f77name(wgldsi)(string, stringLength, ftnStringLength)
char *string;
int *stringLength,  ftnStringLength;
{
   return c_wgldsi(string, *stringLength);
   }


/**
 ******
 **/

int  c_wgldsi(string, stringLength)
char *string;
int stringLength;
{
   return wglc_wgl->wgldsi(string, stringLength);
   }


float wglasx_(string, stringLength, ftnStringLength)
char *string;
int *stringLength, ftnStringLength;
{
   return c_wglasx(string, *stringLength);
   }


/**
 ******
 **/

float c_wglasx(string, stringLength)
char *string;
int stringLength;
{  
   int bidon;
   float xbidon1, ybidon1, xbidon2, ybidon2;

   return wglc_wgl->wglasx(string, stringLength);
   }


float f77name(wgldsx)(string, stringLength, ftnStringLength)
char *string;
int *stringLength, ftnStringLength;
{
   return c_wgldsx(string, *stringLength);
   }


/**
 ******
 **/

float c_wgldsx(string, stringLength)
char *string;
int stringLength;
{  
   return wglc_wgl->wgldsx(string, stringLength);
   }


