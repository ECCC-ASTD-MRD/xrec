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
#include <rpnmacros.h>
#include <rec.h>

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



f77name(wglcalcols_m)(indices, val, nbvals, min, delta, missing, intervalles, nbIntervalles, facteur, nbcol)
int indices[];
float val[];
int *nbvals;
float *min, *delta, *intervalles, *missing;
int *nbIntervalles; 
float *facteur;
int *nbcol;
{
   c_wglcalcols_m(indices, val, *nbvals, *min, *delta, *missing, intervalles, *nbIntervalles, *facteur, *nbcol);
   }


/**
***********************************************************************
***********************************************************************
**/

c_wglcalcols_m(int indices[], float val[], int nbVals, float min, float delta, float missing, 
	       float intervalles[], int nbIntervalles, float facteur, int nbCol)
{
  float fraction, avg, deltaCol, invInter, invDelta, rmax;
  int i, ind, nmaxcols, variation;
  
  variation = PaletteMgrGetVariation();
   nmaxcols = nbCol - 1;
   if (nbIntervalles == 1)
      {
      if (intervalles[0] == 0.0)
	{
	enhancefracs(val,nbVals,min,delta,variation);
	for (i=0; i < nbVals; i++)
	  {
	  if (val[i] != MISSING)
	    {
	    ind = ROUND(nbCol * val[i]);
	    indices[i] = (ind > nmaxcols) ? nmaxcols : (ind < 0) ? 0 : ind;
	    }
	  else
	    {
	    indices[i] = -1;
	    }
	  }
	}
      else
	{
	invInter = 1.0 / (intervalles[0] * facteur);
	for (i=0; i < nbVals; i++)
	  {
	  if (val[i] != MISSING)
	    {
#if defined (HP) || defined(Alpha)
	    val[i] = floor((val[i]-min) * invInter);
#else
	    val[i] = ffloor((val[i]-min) * invInter);
#endif
	    }
	  }
	
	enhancefracs(val,nbVals,0.0,delta/(intervalles[0]*facteur),variation);
	for  (i=0; i < nbVals; i++)
	  {
	  if (val[i] != MISSING)
	    {
	    ind = ROUND(nbCol * val[i]);
	    indices[i] = (ind > nmaxcols) ? nmaxcols : (ind < 0) ? 0 : ind;
	    }
	  else
	    {
	    indices[i] = -1;
	    }
	  }
	}
      }
   else
     {
     rmax = (float)nmaxcols / (float)(nbIntervalles);
     for (i=0; i < nbVals; i++)
       {
       if (val[i] != MISSING)
	 {
	 ind = 0;
	 while ((facteur*intervalles[ind]) <= val[i] && ind < nbIntervalles)
	   ind++;
	 indices[i] = (int)(rmax * ind);
	 }
       else
	 {
	 indices[i] = -1;
	 }
       }
     }
}

/**
***********************************************************************
***********************************************************************
**/

c_wglcalcolf_m(float indices[], float val[], int nbVals, float min, float delta, 
	     float intervalles[], int nbIntervalles, float facteur, int nbCol)
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
	if (val[i] != MISSING)
	  {
	  ind = ROUND(nbCol * val[i]);
	  indices[i] = nbCol * val[i];
	  }
	else
	  {
	  indices[i] = -1;
	  }
	}
      }
    else
      {
      invInter = 1.0 / (intervalles[0] * facteur);
      for (i=0; i < nbVals; i++)
	{
	if (val[i] != MISSING)
	  {
#if defined  (HP) || defined(Alpha)
	  val[i] = floor((val[i]-min) * invInter);
#else
	  val[i] = ffloor((val[i]-min) * invInter);
#endif
	  }
	else
	  {
	  indices[i] = -1;
	  }
	}
      
      enhancefracs(val,nbVals,0.0,delta/(intervalles[0]*facteur),variation);
      for  (i=0; i < nbVals; i++)
	{
	if (val[i] != MISSING)
	  {
	  ind = ROUND(nbCol * val[i]);
	  indices[i] = nbCol * val[i];
	  }
	else
	  {
	  indices[i] = -1;
	  }
	}
      }
    }
  else
    {
    rmax = (float)nmaxcols / (float)(nbIntervalles);
    for (i=0; i < nbVals; i++)
      {
       if (val[i] != MISSING)
	 {
	 ind = 0;
	 while ((facteur*intervalles[ind]) <= val[i] && ind < nbIntervalles)
	   ind++;
	 indices[i] = (int)(rmax * ind);
	 }
       else
	 {
	 ind = 0;
	 while ((facteur*intervalles[ind]) <= val[i] && ind < nbIntervalles)
	   ind++;
	 indices[i] = (rmax * ind);
	 }
      }
    }
}


