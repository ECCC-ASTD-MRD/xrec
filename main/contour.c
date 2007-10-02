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

#include <rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <xinit.h>
#include <wgl.h>
#include <time.h>
#include <math.h>

extern _XContour xc;
extern GeoMapInfoStruct mapInfo;
extern _InfoChamps *infoChamps;

#ifdef SGI 
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)flog10(x))
#endif

#ifdef IRIX5
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)flog10(x))
#endif

#ifdef IRIX6
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)flog10(x))
#endif

#ifdef HP 
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)log10(x))
#endif

#ifdef i386 
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)log10(x))
#endif

#ifdef Alpha
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)log10(x))
#endif

#ifdef Darwin_OSX_PPC
#define LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)flog10(x))
#endif

void   AjusterMinMax(float *ContourMin, float *ContourMax, float facteur, float intervalles);

float ContourMin, ContourMax, leContour;
float RGBvec[3];
float colorRange[5][2];

static char *labelIntervalle[] = {"Intervalle: %s * %6.1e %s", 
             "Interval: %s * %6.1e %s" };

/**
***********************************************************************
***********************************************************************
**/

void UpdateFld_IntervalleDeContour(_Champ *champ, float nouvelIntervalle[], int nbIntervalles)
{
   int lng;

   int i;
   lng = c_getulng();

   champ->nbIntervalles = nbIntervalles;
   for (i=0; i < nbIntervalles; i++)
      champ->intervalles[i] = nouvelIntervalle[i];
   UpdateTitreIntervalle(champ->titreIntervalle, champ->intervalles, champ->nbIntervalles, champ->facteur, champ->titreUnites);
  }
 

/**
***********************************************************************
***********************************************************************
**/

void UpdateTitreIntervalle(char titreIntervalles[], float intervalles[], int nbIntervalles, float facteurEchelle, char titreUnites[])
{
   char format[8], interInfo[32], tampon[16];
   int i;
   int lng;

   lng = c_getulng();
   GetFormat(format, intervalles, nbIntervalles, facteurEchelle);
   if (nbIntervalles == 1)
      {
      sprintf(interInfo, format, intervalles[0]);
      }
   else
      {
      strcpy(interInfo, "[");
      i = 0;
      while (i < nbIntervalles && 20 > strlen(interInfo))
   {
   sprintf(tampon, format, intervalles[i]);
   strcat(interInfo, tampon);
   if (i < (nbIntervalles-1))
      {
      strcat(interInfo, ",");
      }
   i++;
   }
      if (i < (nbIntervalles - 1))
   {
   strcat(interInfo, "...");
   }
      strcat(interInfo, "]");
      }
   sprintf(titreIntervalles, labelIntervalle[lng],   
     interInfo, facteurEchelle,  titreUnites);
   }


/**
***********************************************************************
***********************************************************************
**/

void DefinirFenetreGrille(int *ideb, int *jdeb, int *ifin, int *jfin, int ni, int nj)
{
   float xdeb, ydeb, xfin, yfin;

   c_wglgvx(&xdeb, &ydeb, &xfin, &yfin);
   c_fxfy2xy(&xdeb, &ydeb, xdeb, ydeb);
   c_fxfy2xy(&xfin, &yfin, xfin, yfin);
   *ideb = (int) xdeb - 1.0;
   *jdeb = (int) ydeb - 1.0;
   *ifin = ROUND(xfin + 1.0);
   *jfin = ROUND(yfin + 1.0);

   if (4 > (*ifin - *ideb))
      {
      *ideb--;
      *ifin++;
      }

   if (4 > (*jfin - *jdeb))
      {
      *jdeb-=2;
      *jfin+=2;
      }

   if (*ideb < 1)
      *ideb = 1;
   
   if (*jdeb < 1)
      *jdeb = 1;

   if (*ifin > ni)
      *ifin = ni;

   if (*jfin > nj)
      *jfin = nj;

   }

/**
***********************************************************************
***********************************************************************
**/

void f77name(ajusminmax)(float *cmin, float *cmax, float *facteur, float *intervalle)
{
   AjusterMinMax(cmin, cmax, *facteur, *intervalle);
   }

void AjusterMinMax(float *ContourMin, float *ContourMax, float facteur, float intervalle)
{
   float del;

   *ContourMin /= facteur;
   *ContourMax /= facteur;
/** intervalle /= facteur; **/
  
#if defined (HP) || defined (Alpha)
   if (fabs(*ContourMin) > 1)
   *ContourMin = floor(*ContourMin);
   if (fabs(*ContourMax) > 1)
   *ContourMax = ceil(*ContourMax);
#else
   if (fabs(*ContourMin) > 1)
   *ContourMin = ffloor(*ContourMin);
   if (fabs(*ContourMax) > 1)
   *ContourMax = fceil(*ContourMax);
#endif 
   
   del= fabs(fmod(*ContourMin,intervalle));
   if (*ContourMin >= 0)
/**      *ContourMin += (intervalle - del); **/
      *ContourMin -= del;
   else
      *ContourMin -= (intervalle - del);
   
   *ContourMin *= facteur;
   *ContourMax *= facteur;
   }

/**
***********************************************************************
***********************************************************************
**/

void DrawBoxedStr(char *str, int x1, int y1, int size, int foreground, int background)
{
   int   strWidth, strHeight;

   c_wglfsz(size);
   strWidth = c_wglwsi(str, strlen(str));
   strHeight = c_wglasi(str, strlen(str)) + c_wgldsi(str, strlen(str));

   c_wglcol(background);
   c_wglrfi(x1 - 2, y1 - 4, x1 + strWidth+2, y1 + strHeight);
   
   c_wglcol(foreground);
   c_wglrli(x1 - 2, y1 - 4, x1 + strWidth +2, y1 + strHeight);

   c_wglpsi(x1, y1, str, strlen(str), size, 0, 0);

   }


/**
***********************************************************************
***********************************************************************
**/


void ConvertFloatToString(char str[], float val)
{
   if (0.01 < fabs(val) && 1.0e6 > fabs(val))
      {
      sprintf(str, "%12.6g", val);
      }
   else
      {
      sprintf(str, "%12.4e", val);
      }
   
   strclean(str);
   }


/**
***********************************************************************
***********************************************************************
**/

void ConvertFloatToString2(char str[], float val)
{
   int exposant;
   int nbDecimales;
   float expo;
   char format[8], tmp[8];
   
   nbDecimales = 0;
   expo = pow(10.0, (float)nbDecimales);
   while (((int)(fabs(expo * val)) != (int)((int)(fabs(val)) * expo)) && nbDecimales <= 5)
      {
      nbDecimales++;
      expo = pow(10.0, (float)nbDecimales);
      }
   
   if (nbDecimales == 0)
      {
      sprintf(tmp, "%2d", nbDecimales);
      strcpy(format, "");
      strcat(format, "%");
      strcat(format, tmp);
      strcat(format, "d");
      sprintf(str, format, (int)rint(val));
      }
   else
      {
      sprintf(tmp, "%2d", nbDecimales);
      strcpy(format, "");
      strcat(format, "%.");
      strcat(format, tmp);
      strcat(format, "f");
      sprintf(str, format, val);
      }
   }


/**
***********************************************************************
***********************************************************************
**/


void GetFormat(char str[], float intervalles[], int nbIntervalles, float facteurEchelle)
{
   int e, exposantMin, exposantMax;
   int i;
   int nbDecimales = 0;
   int nbDecimalesMax;
   float expo, inter, logg, r1;
   char format[8], tmp[8];
   
   if (intervalles[0] == 0.0)
      inter = 1.0;
   else
      inter = intervalles[0];
   
   logg = LE_LOG(fabs(inter));
   exposantMin = (int)(logg-1.0);
   exposantMax = (int)(logg-1.0);
   
   for (i=1; i < nbIntervalles; i++)
      {
      if (intervalles[i] != 0.0) 
         logg =  LE_LOG(fabs(intervalles[i]));
      else
         logg = 0.0;
      e = (int)(logg-1.0);
      
      if (exposantMin > e)
   exposantMin = e;
      
      if (exposantMax < e)
   exposantMax = e;
      }
   
   if (exposantMin < -3 || exposantMax > 5)
      {
      strcpy(str, "%5.1e");
      }
   else
      {
      nbDecimalesMax = 0;
      for (i=0; i < nbIntervalles; i++)
   {
   nbDecimales = 0;
   expo = pow(10.0, (float)(nbDecimales));
   r1 = fmod(intervalles[i]*expo, 1.0);
   while ((nbDecimales < 5) && ((1.0e-4 < fabs(0.0 - r1)) && (1.0e-4 < fabs(1.0 - r1))))
      {
      nbDecimales++;
      logg = fabs(0.0 - fmod(intervalles[i]*expo, 1.0));
      expo = pow(10.0, (float)nbDecimales);
      r1 = fmod(intervalles[i]*expo, 1.0);
      }
   
   if (nbDecimalesMax < nbDecimales)
      nbDecimalesMax = nbDecimales;
   
   }
      
      sprintf(tmp, "%1d", nbDecimalesMax);
      strcpy(format, "");
      strcat(format, "%.");
      strcat(format, tmp);
      strcat(format, "f");
      strcpy(str, format);
      }
   }
