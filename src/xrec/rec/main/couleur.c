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

#include <math.h>
#include <stdio.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl.h>

#define LIRE   0
#define ECRIRE 1

extern float hsvTable[256][3];
int  oldcmap[256][3], newcmap[256][3];
static int stdcmap[256][3];
static int stdcmapInitialisee = 0;
short   pseudoCmap[256];

static int indColMin = 64;
static int indColMax = 204;
static int nbColReservees = 13;
static _ColormapInfo lastColorMapInfo = { -1, 0.0, 0.0};
extern int externcmap[256][3];

extern _ColormapInfo recCmap;

/**
***********************************************************************
***********************************************************************
**/

void ResetColorMap(int colorTable[], int ncol, int colorMapType)
{
  float x;
  int availPlanes;
  int i, j, k, indCol;
  FILE *f;
  char *tmp, paletteDeDefaut[256];
  char ligne[80];

  int nbColEcrire, nbColLire, nbColAnim;
  int r, g, b;
  availPlanes = c_wglgpl();

  if (lastColorMapInfo.noPalette == recCmap.noPalette &&
      lastColorMapInfo.amplificationMin == recCmap.amplificationMin &&
      lastColorMapInfo.reductionMax == recCmap.reductionMax)
      {
      return;
      }
  else
      {
      lastColorMapInfo = recCmap;
      }

  c_wglncl(&nbColLire, &nbColEcrire, &nbColAnim);

#ifdef GL_WGL
  if (c_wglgpl() <= 8)
    {
    switch(colorMapType)
      {
      case COULEURS_STANDARDS:
          c_wglmco( 8,  0,     0,  127);
          c_wglmco( 9,  0,     13,  255);
          c_wglmco(10,  0,    228,  255);
          c_wglmco(11,  146,  255,  108);
          c_wglmco(12,  255,  191,  0);
          c_wglmco(13,  255,   23,  0);
          c_wglmco(14,  127,    0,  0);
          break;

        case COULEURS_STANDARDS_INVERSEES:
          c_wglmco(14,  0,     0,  127);
          c_wglmco(13,  0,     13,  255);
          c_wglmco(12,  0,    228,  255);
          c_wglmco(11,  146,  255,  108);
          c_wglmco(10,  255,  191,  0);
          c_wglmco(9,  255,   23,  0);
          c_wglmco(8,  127,    0,  0);
          break;

        case BLANC_NOIR:
        case UTOPIC_VZ:
          c_wglmco(14,  32,     32,  32);
          c_wglmco(13,  64,   64,  64);
          c_wglmco(12,  96, 96,  96);
          c_wglmco(11,  160, 160,  160);
          c_wglmco(10,  192, 192,  192);
          c_wglmco(9,  224, 224,  224);
          c_wglmco(8,  255, 255,  255);
          break;

        case NOIR_BLANC:
        case UTOPIC_IR:
          c_wglmco( 8,  32,     32,  32);
          c_wglmco( 9,  64,   64,  64);
          c_wglmco(10,  96, 96,  96);
          c_wglmco(11,  160, 160,  160);
          c_wglmco(12,  192, 192,  192);
          c_wglmco(13,  224, 224,  224);
          c_wglmco(14,  255, 255,  255);

          break;
      }
    }
#endif   

  switch(colorMapType)
      {
      case COULEURS_STANDARDS:
      case COULEURS_STANDARDS_INVERSEES:
      if (!stdcmapInitialisee)
         {
         tmp = (char *) getenv("ARMNLIB");
         strcpy(paletteDeDefaut, tmp);
         strcat(paletteDeDefaut, "/data/defpal.rec");
         f = fopen(paletteDeDefaut, "r");
         if (f != NULL)
               {
               while (fgets(ligne, 80, f) != NULL)
               {
               sscanf(ligne, "%d %d %d %d", &indCol, &r, &g, &b);
               stdcmap[indCol][0] = r;
               stdcmap[indCol][1] = g;
               stdcmap[indCol][2] = b;
               }
               fclose(f);
               }
         else
               {
               printf("<ResetColormap> : palette de defaut introuvable\n");
               BuildColIndexTable(colorTable, stdcmap, 256, indColMin, indColMax - nbColReservees, ECRIRE);
               }
         stdcmapInitialisee = 1;
         }

      for (i=0; i < 256; i++)
         for (j=0; j < 3; j++)
               {
               oldcmap[i][j] = stdcmap[i][j];
               hsvTable[i][j] = stdcmap[i][j];
               }
      break;

      case BLANC_NOIR:
      case NOIR_BLANC:
      for (i=0; i < 256; i++)
         {
         oldcmap[i][0] = i;
         oldcmap[i][1] = i;
         oldcmap[i][2] = i;
         }
      break;

      case UTOPIC_IR:
      case UTOPIC_VZ:
      for ( i = 0 ; i < 256 ; i++ )
         {
         oldcmap[i][0] = i; 
         oldcmap[i][1] = i; 
         oldcmap[i][2] = i; 
         }

      oldcmap[0][0] =   0 ; 
      oldcmap[0][1] =   0 ; 
      oldcmap[0][2] = 230 ;

      for ( i = 51 ; i >= 0 ; i-- )
         {
         oldcmap[i+2][0]   =   i * 5 ;
         oldcmap[i+2][1] = ( i * 5 < 192 ) ? i * 5 + 64  : i * 5 ;
         oldcmap[i+2][2]  = ( i * 5 < 128 ) ? i * 5 + 128 : oldcmap[i+2][1] ;
         }

      for (i=2; i < 53 ; i++)
         {
         j = ((i-2) * 5);
         newcmap[j][0] = oldcmap[i][0];
         newcmap[j][1] = oldcmap[i][1];
         newcmap[j][2] = oldcmap[i][2];
         
         newcmap[j+1][0] = oldcmap[i][0];
         newcmap[j+1][1] = oldcmap[i][1];
         newcmap[j+1][2] = oldcmap[i][2];
         
         newcmap[j+2][0] = oldcmap[i][0];
         newcmap[j+2][1] = oldcmap[i][1];
         newcmap[j+2][2] = oldcmap[i][2];
         
         newcmap[j+3][0] = oldcmap[i][0];
         newcmap[j+3][1] = oldcmap[i][1];
         newcmap[j+3][2] = oldcmap[i][2];
         
         newcmap[j+4][0] = oldcmap[i][0];
         newcmap[j+4][1] = oldcmap[i][1];
         newcmap[j+4][2] = oldcmap[i][2];
         
         newcmap[j+5][0] = oldcmap[i][0];
         newcmap[j+5][1] = oldcmap[i][1];
         newcmap[j+5][2] = oldcmap[i][2];
         }


      for (i=0; i < 256; i++)
         {
         oldcmap[i][0] = newcmap[i][0];
         oldcmap[i][1] = newcmap[i][1];
         oldcmap[i][2] = newcmap[i][2];
         }
      break;

      case EXP_1:
      for (i=0; i < 64; i++)
         {
         oldcmap[i][0] = (63 - i) * 4;
         oldcmap[i][1] = (63 - i) * 4;
         oldcmap[i][2] = 255;
         }

      for (i=64; i < 128; i++)
         {	
         oldcmap[i][0] = 0;
         oldcmap[i][1] = 0;
         oldcmap[i][2] = (127 - i) * 4;
         }

      for (i=128; i < 192; i++)
         {
         oldcmap[i][0] = (i-128)*4;
         oldcmap[i][1] = 0;
         oldcmap[i][2] = 0;
         }

      for (i=192; i < 256; i++)
         {
         oldcmap[i][0] = 255;
         oldcmap[i][1] = (i-192)*4;
         oldcmap[i][2] = (i-192)*4;
         }
      break;

      case EXP_2:
      for (i=0; i < 64; i++)
         {
         oldcmap[i][0] = 0;
         oldcmap[i][1] = 0;
         oldcmap[i][2] = 128 + i * 2;
         }

      for (i=64; i < 128; i++)
         {	
         oldcmap[i][0] = (i-64)*4;
         oldcmap[i][1] = (i-64)*4;
         oldcmap[i][2] = 255;
         }

      for (i=128; i < 192; i++)
         {
         oldcmap[i][0] = 255;
         oldcmap[i][1] = (191-i)*4;
         oldcmap[i][2] = (191-i)*4;
         }

      for (i=192; i < 256; i++)
         {
         oldcmap[i][0] = 128+ (255-i)*2;
         oldcmap[i][1] = 0; 
         oldcmap[i][2] = 0;
         }
/*      for (i=0; i < 256; i++)
         {
         printf("%d %d %d %d\n", i, oldcmap[i][0], oldcmap[i][1], oldcmap[i][2]);
         }*/
      break;

      default:
      for (i=0; i < 256; i++)
         {
         oldcmap[i][0] = externcmap[i][0];
         oldcmap[i][1] = externcmap[i][1];
         oldcmap[i][2] = externcmap[i][2];
         }
      break;

      }

  switch(colorMapType)
      {
      case COULEURS_STANDARDS_INVERSEES:
      case BLANC_NOIR:
      case UTOPIC_IR:
      for (i=0; i < 256; i++)
         {
         newcmap[255-i][0] = oldcmap[i][0];
         newcmap[255-i][1] = oldcmap[i][1];
         newcmap[255-i][2] = oldcmap[i][2];
         }

      for (i=0; i < 256; i++)
         {
         oldcmap[i][0] = newcmap[i][0];
         oldcmap[i][1] = newcmap[i][1];
         oldcmap[i][2] = newcmap[i][2];
         }
      break;

      default:
      break;

      }

  AjusterVariationPalette(newcmap, oldcmap, colorTable, recCmap.amplificationMin, recCmap.reductionMax);

  if (c_wglgpl() > 4)
      {
      for (i=0; i < 256; i++)
         colorTable[i] = indColMin + (int)((float) i / 255.0 * (indColMax - nbColReservees - 1 - indColMin));

      c_wglmcos(colorTable, 256, newcmap);
      }
  else
      {
      for (i=0; i < 256; i++)
         {
         x = 8.0 + (6.0 * i) / 255.0;
         memcpy(&colorTable[i], &x, sizeof(float));
         }


/**
      c_wglmcos(colorTable, 256, newcmap);
      switch(colorMapType)
  {
  case COULEURS_STANDARDS:
  case COULEURS_STANDARDS_INVERSEES:
  BuildColIndexTable(colorTable, newcmap, 256, 0, 14, LIRE);

  default:
  BuildColIndexTable(colorTable, newcmap, 256, 0, 15, LIRE);
  }
**/
      }
  }

void AjusterVariationPalette(int newcmap[][3], int oldcmap[][3], int colorTable[], float amplificationMin, float reductionMax)
{
  int i, j, newi;
  int idebut, ifin;

  idebut = ROUND(255.0 * amplificationMin);
  ifin   = ROUND(255.0 * (1.0 - reductionMax));

  for (i=0; i < idebut; i++)
      {
      newcmap[i][0] = oldcmap[0][0];
      newcmap[i][1] = oldcmap[0][1];
      newcmap[i][2] = oldcmap[0][2];
      }

  for (i=idebut; i < ifin; i++)
      {
      double delta = (double) (i-idebut) / (double)(ifin - idebut);
      delta *= 255.0;
      newi = (int)delta;
      newcmap[i][0] = oldcmap[newi][0] + (delta - (float)newi)*(oldcmap[newi+1][0]-oldcmap[newi][0]);
      newcmap[i][1] = oldcmap[newi][1] + (delta - (float)newi)*(oldcmap[newi+1][1]-oldcmap[newi][1]);
      newcmap[i][2] = oldcmap[newi][2] + (delta - (float)newi)*(oldcmap[newi+1][2]-oldcmap[newi][2]);
      }

  for (i=ifin; i < 256; i++)
      {
      newcmap[i][0] = oldcmap[255][0];
      newcmap[i][1] = oldcmap[255][1];
      newcmap[i][2] = oldcmap[255][2];
      }

  }

void c_wglgetcolrange(int *indMin, int *indMax)
{
  *indMin = indColMin;
  *indMax = indColMax;
  }

void f77name(setcolors)(int *indMin, int *indMax)
{
  indColMin = *indMin;
  indColMax = *indMax;   
  }
