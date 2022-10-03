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
#include <rmn/rpnmacros.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

/**
  objet:    c_wglcalcols
  fonction: Routine calculant les indices de colormap correspondant
            aux valeurs d'un champ
  auteur:   Yves Chartier, fevrier 1992
  Arguments:
    indices:        indices de la table de couleur correspondant a chaque valeur point flottant
    val:            liste de valeurs point flottant
    nbVals:         nombre de valeurs dans la liste
    min:	    valeur minimale du champ
    delta:	    difference entre le min et le max du champ	
    intervalles:    liste d'intervalles de contours
    nbIntervalles:  nombre d'intervalles dans la liste 
    facteur:	    facteur d'echelle du champ
    nbCol:	    nombre de couleurs dans la table de couleurs
**/

void c_wglcalcols(int indices[], float val[], int nbVals, float min, float delta, float intervalles[], int nbIntervalles, float facteur, int nbCol);
void c_wglcalcolf(float indices[], float val[], int nbVals, float min,  float delta,  float intervalles[], int nbIntervalles, float facteur, int nbCol);

void f77name(wglcalcols)(int indices[], float val[], int *nbvals, float *min, float *delta, float *intervalles, int *nbIntervalles, float *facteur, int *nbcol)
{
   c_wglcalcols(indices, val, *nbvals, *min, *delta, intervalles, *nbIntervalles, *facteur, *nbcol);
   }


/**
***********************************************************************
***********************************************************************
**/

void c_wglcalcols(int indices[], float val[], int nbVals, float min, float delta, float intervalles[], int nbIntervalles, float facteur, int nbCol)
{
   float avg, deltaCol;
   int i, ind;
   float invInter, invDelta, rmax;
   int nmaxcols;
   float fraction;
   int variation;
      

   variation = PaletteMgrGetVariation();
   nmaxcols = nbCol - 1;
   if (nbIntervalles == 1)
      {
      if (intervalles[0] == 0.0)
        {
        enhancefracs(val,nbVals,min,delta,variation);
        for (i=0; i < nbVals; i++)
            {
            ind = ROUND(nbCol * val[i]);
            indices[i] = (ind > nmaxcols) ? nmaxcols : (ind < 0) ? 0 : ind;
            }
        }
      else
        {
        invInter = 1.0 / (intervalles[0] * facteur);
        for (i=0; i < nbVals; i++)
            {
            val[i] = ffloor((val[i]-min) * invInter);
            }

        enhancefracs(val,nbVals,0.0,delta/(intervalles[0]*facteur),variation);
        for  (i=0; i < nbVals; i++)
            {
            ind = ROUND(nbCol * val[i]);
            indices[i] = (ind > nmaxcols) ? nmaxcols : (ind < 0) ? 0 : ind;
            }
        }
      }
   else
      {
      rmax = (float)nmaxcols / (float)(nbIntervalles);
      for (i=0; i < nbVals; i++)
        {
        ind = 0;
        while ((facteur*intervalles[ind]) <= val[i] && ind < nbIntervalles)
            ind++;
        indices[i] = (int)(rmax * ind);
        }
      }
   }

/**
***********************************************************************
***********************************************************************
**/

void c_wglcalcolf(float indices[], float val[], int nbVals, float min,  float delta,  float intervalles[], int nbIntervalles, float facteur, int nbCol)
{
   float avg, deltaCol;
   int i, ind;
   float invInter, invDelta, rmax;
   int nmaxcols;
   float fraction;
   int variation;
      

   variation = PaletteMgrGetVariation();
   nmaxcols = nbCol - 1;
   if (nbIntervalles == 1)
      {
      if (intervalles[0] == 0.0)
        {
        enhancefracs(val,nbVals,min,delta,variation);
        for (i=0; i < nbVals; i++)
            {
            ind = ROUND(nbCol * val[i]);
            indices[i] = nbCol * val[i];
            }
        }
      else
        {
        invInter = 1.0 / (intervalles[0] * facteur);
        for (i=0; i < nbVals; i++)
            {
            val[i] = ffloor((val[i]-min) * invInter);
            }

        enhancefracs(val,nbVals,0.0,delta/(intervalles[0]*facteur),variation);
        for  (i=0; i < nbVals; i++)
            {
            ind = ROUND(nbCol * val[i]);
            indices[i] = nbCol * val[i];
            }
         }
      }
   else
      {
      rmax = (float)nmaxcols / (float)(nbIntervalles);
      for (i=0; i < nbVals; i++)
        {
        ind = 0;
        while ((facteur*intervalles[ind]) <= val[i] && ind < nbIntervalles)
            ind++;
        indices[i] = (rmax * ind);
        }
      }
   }


