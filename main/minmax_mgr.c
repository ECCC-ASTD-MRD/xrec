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

#include <rec.h>
#include <gmp.h>
#include <wgl.h>
#include <rpnmacros.h>

extern _Viewport viewp;
extern _XContour xc;
extern int recColorTable[];
extern int sizeRecColorTable;
extern int facteurLissage;
extern float labelPos[][4];


xrecGetCurrentMinMax(float *min, float *max, int mode)
{
  int nbChampsActifs;
  int minmaxset;
  int i,j;
  float tmpmin, tmpmax;
  int indChamp;

   _Champ *champ;

  nbChampsActifs = FldMgrGetNbChampsActifs();

  minmaxset = NON;
  for (i=0; i < nbChampsActifs; i++)
    {
    FldMgrGetChamp(&champ, indChamp);
    if (CUSTOM == DictMgrGetMinMaxMode(champ->nomvar))
      {
      minmaxset = 1;
      DictMgrGetMinMaxValues(champ->nomvar,&tmpmin,&tmpmax);
      if (i==0)
	{
	*min = tmpmin;
	*max = tmpmax;
	}
      else
	{
	*min = tmpmin < *min ? tmpmin : *min;
	*max = tmpmax > *max ? tmpmax : *max;
	}
      }
    }

  if (minmaxset == OUI) return;
  

}

int GetMinMaxUV(float *uvmin, float *uvmax)
{
  int i, nbChampsActifs;
  _Champ *champ;
  float lmin, lmax;
  int op;

  *uvmin = 0.0;
  *uvmax = 0.0;

  nbChampsActifs = FldMgrGetNbChampsActifs();
  op = CtrlMgrGetMathOp();

  for (i=0; i < nbChampsActifs; i++)
    {
    FldMgrGetChamp(&champ, i);
    if (champ->natureTensorielle == VECTEUR)
      {
      switch (i)
	{
	case 0:
	  *uvmin = champ->uvmin[0];
	  *uvmax = champ->uvmax[0];
	  break;
	  
	default:
	  lmin = champ->uvmin[op];
	  lmax = champ->uvmax[op];
	  if (lmin < *uvmin) *uvmin = lmin;
	  if (lmax > *uvmax) *uvmax = lmax;
	  break;
	}
      }
    }
}

