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
#include <rec_functions.h>
#include <rpnmacros.h>

/**
  objet:    c_wglcont
  fonction: Routine de gestion du traceur de contours
  auteur:   Yves Chartier, fevrier 1992
  Arguments:
    fld:            champ bi-dimensionnel contenant les valeurs a contourer
    ni, nj:         dimensions du champ
    intervalles:    liste d'intervalles de contours
    nbIntervalles:  nombre d'intervalles dans la liste
    facteur:        ordre de grandeur du champ
    min,max:        valeurs min et max du champ
    couleur:        indice du colormap utilise pour tracer les lignes
    epaisseur:      epaisseur des lignes en pixels
    codeDash:       code utilise pour determiner quelles lignes sont tracees en tirete:
                       == 0 --> toutes les lignes sont tracees "pleines"
                       == 1 --> toutes les lignes sont tracees en tirete
	               == 2 --> seulement les lignes < 0 sont tracees en tirete
    dashPattern:    indice du patron de tirete tel que defini par l'appel
                    a la routine c_wgldld()
    facteurLissage: niveau de lissage demande
    flagInterrupt:  drapeau indiquant si la routine devra s'informer
                    d'une demande eventuelle d'annulation

**/

void f77name(wglstl_fxfy2)(float *fld, float *px, float *py,  int *l, int *m , int *n, int *mdeb, int *ndeb, int *mfin, int *nfin, float *leContour, int *lissfac);

void c_wglcont(float *fld, int ni, int nj, float intervalles[], int nbIntervalles, float facteur, float rmin, float rmax, int couleur, int epaisseur, int codeDash, int dashPattern, int facteurLissage, int flagInterrupt)
{
   float contourMin, contourMax, leContour;
   int mdeb,ndeb,mfin,nfin;
   int linewidth, origPattern;
   int nContours, annulationDemandee;
   int i1, i2, j1, j2, dimensiong, dimensionf;
   float densiteX, densiteY, densite;
   int lissfac, altLissfac;
   int i;

   if (intervalles[0] == 0.0 && nbIntervalles == 1)
      return;

   annulationDemandee = FALSE;
   if (flagInterrupt)
      annulationDemandee = c_wglanul();

   DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, ni, nj);

   c_wglgvi(&i1, &j1, &i2, &j2);
   densiteX = (float)(i2 - i1)/(float)(mfin-mdeb);
   densiteY = (float)(j2 - j1)/(float)(nfin-ndeb);

   if (densiteX < densiteY)
      {
      densite = densiteX;
      dimensiong = mfin - mdeb;
      dimensionf = i2 - i1;
      }
   else
      {
      densite = densiteY;
      dimensiong = nfin - ndeb;
      dimensionf = j2 - j1;
      }

   altLissfac = ROUND(dimensionf/(5.0*dimensiong));
   altLissfac = (altLissfac < 1) ? 1 : altLissfac;
   lissfac  = (facteurLissage < altLissfac) ? facteurLissage : altLissfac;

   c_wglcol(couleur);
   origPattern = (codeDash == 1) ? dashPattern : 0;

   if (nbIntervalles == 1)
      {
      f77name(sminmax)(&contourMin, &contourMax, fld, &ni, &nj, &mdeb, &ndeb, &mfin, &nfin);
      contourMin = contourMin < rmin ? rmin : contourMin;
      contourMax = contourMax > rmax ? rmax : contourMax;
      AjusterMinMax(&contourMin, &contourMax, facteur, intervalles[0]);

      leContour = contourMin;
      nContours = 0;
      while ((leContour <= contourMax) && !annulationDemandee)
        {
        if (fabs(leContour/facteur) < 1.0e-4 && epaisseur > 1)
            c_wgllwi(2*epaisseur);
        else
            c_wgllwi(epaisseur);

        c_wgllwi(epaisseur); /** Modif pour Shilling **/
        c_wglsld((leContour < 0.0 && codeDash > 1) ? dashPattern : origPattern);

        f77name(wglstl)(fld, &ni, &ni , &nj, &mdeb, &ndeb, &mfin, &nfin, &leContour, &lissfac);
        leContour+=(intervalles[0]*facteur);
        nContours++;
        if (0 == (nContours % 8) && flagInterrupt)
            annulationDemandee = c_wglanul();
        }
      f77name(wglstl)(fld, &ni, &ni , &nj, &mdeb, &ndeb, &mfin, &nfin, &contourMax, &lissfac);

      }
   else
      {
      for (i=0; i < nbIntervalles; i++)
        {
        leContour = (intervalles[i]*facteur);
        c_wglsld((leContour < 0.0 && codeDash > 1) ? dashPattern : origPattern);

        if ((0.0 == leContour) && epaisseur > 1)
            c_wgllwi(2*epaisseur);
        else
            c_wgllwi(epaisseur);

        c_wgllwi(epaisseur); /** Modif pour Shilling **/
        f77name(wglstl)(fld, &ni, &ni , &nj, &mdeb, &ndeb, &mfin, &nfin, &leContour, &lissfac);
        }
      }

   }

