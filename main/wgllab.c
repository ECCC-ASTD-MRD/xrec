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
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl.h>
#include <rpnmacros.h>

#define C2FTN(i,j,ni)  (int)((ni) * (j) + i)
#define BTN(a,b,c) ((((a < c) ? a : c) < b) && (((a > c) ? a : c) >= b))

/**
  objet:    c_wgllab
  fonction: Routine qui affiche les niveaux d'un champ
  auteur:   Yves Chartier, fevrier 1992
  Arguments:
    fld:            champ bi-dimensionnel contenant les valeurs a contourer
    ni, nj:         dimensions du champ
    intervalles:    liste d'intervalles de contours
    nbIntervalles:  nombre d'intervalles dans la liste
    facteur:        ordre de grandeur du champ
    min,max:        valeurs min et max du champ
    posLabels:      liste de positions indiquant les colonnes ou on veut
                    afficher les labels. Ces valeurs doivent etre 
		    entre 0 et 1, et refletent une fraction de la dimension
		    de la fenetre d'affichage.
    nbLabels:       nombre de positions dans la liste
    fore:           indice du colormap utilise pour afficher le texte
    back:           indice du colormap utilise pour afficher le fond du carre
                    entourant le texte
    lissfac:        niveau de lissage demande
**/

static int fz;


void SetLabFontSize(size)
int size;
{
   fz = size;
   }

void c_wgllab(float *fld, int ni, int nj, float intervalles[], int nbIntervalles, float facteur, float min, float max, float posLabels[], int nbLabels, int fore, int back, int indChamp, int lissfac)
{
   float contourMin, contourMax, leContour;
   int i, j, k, n, NbLabels, label, nbPoints;
   float      pt1, pt2;
   int LargeurTexte, hauteurTexte;
   int x1, y1, lastY1, oldj;
   float yy;
   int ascent, descent;
   char StrLabel[16], format[8];
   int  ValLabel;
   int mdeb,ndeb,mfin,nfin;
   float xdist, xdeb, ydeb, xfin, yfin, rx1, ry1, rx2, ry2, rx;
   int idebut, jdebut, ifin, jfin;
   float hx0, hy0, hx1, hy1;
   int ihx0, ihy0, ihx1, ihy1;
   int res, deltah;
   int *cells;
   
   if (intervalles[0] == 0.0 && nbIntervalles == 1)
      return;
   
   DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, ni, nj);
   
   cells = (int *) calloc (nfin - ndeb + 1, sizeof(int));
   if (nbIntervalles == 1)
      {
      f77name(sminmax)(&contourMin, &contourMax, fld, &ni, &nj, &mdeb, &ndeb, &mfin, &nfin);
      contourMin = contourMin < min ? min : contourMin;
      contourMax = contourMax > max ? max : contourMax;
      AjusterMinMax(&contourMin, &contourMax, facteur, intervalles[0]);
      }
   else
      {
      contourMin = intervalles[0] * facteur;
      contourMax = intervalles[nbIntervalles-1] * facteur;
      }
   
   GetFormat(format, intervalles, nbIntervalles, facteur);
   c_wglfsz(fz);
   descent = c_wgldsi("1234", 4);
   hauteurTexte = c_wglasi("1234", 4) + descent;
   
   c_xy2fxfy(&rx1, &ry1, (float)mdeb, (float)ndeb);
   c_xy2fxfy(&rx2, &ry2, (float)mfin, (float)ndeb);
   xdist = rx2 - rx1;

   c_wglcol(fore);
   for (label = 0; label < nbLabels; label++)
      {
      for (j=ndeb; j < nfin; j++)
	 cells[j-ndeb] = 0;

      c_xy2fxfy(&rx1, &ry1, (float)mdeb, (float)ndeb);
      rx = posLabels[label] * xdist + rx1;
      c_fxfy2xy(&rx2, &ry2, rx, (float)ndeb);
      
      i = ROUND(rx2);
      lastY1 = 0;
      
      leContour = contourMin;
      n = 0;
      while (leContour <= contourMax)
	 {
	 for (j=ndeb; j < nfin; j++)
	    {
	    pt1 = fld[C2FTN(i-1,j-1,ni)];
	    pt2 = fld[C2FTN(i-1,j,ni)];
	    
	    res = BTN(pt1, leContour, pt2);
	    
	    if (res && cells[j-ndeb] == 0)
	       {
	       yy = (leContour - pt1) / (pt2 - pt1);
	       c_xy2fxfy(&rx1, &ry1, (float)(i), (float)(j)+yy);
	       c_wglxai(&x1, &y1, rx1, ry1);
	       
	       sprintf(StrLabel, format, leContour/facteur);
	       LargeurTexte = c_wglwsi(StrLabel, strlen(StrLabel));
	       c_wglcol(back);
	       c_wglrfi(x1 - LargeurTexte - 4, y1,
			x1, y1 + descent + hauteurTexte + 2);
	       
	       c_wglcol(fore);
	       
	       c_wglrli(x1 - LargeurTexte - 4, y1,
			x1, y1 + descent + hauteurTexte + 2);
	       
	       c_wglpsi(x1 - LargeurTexte - 2, y1 + descent + 2, StrLabel, strlen(StrLabel), fz, 0, 0);


	       cells[j - ndeb] = 1;
	       deltah = 0;
	       c_xy2fxfy(&hx0, &hy0, (float)i, (float)j);
	       c_wglxai(&ihx0, &ihy0, hx0, hy0);
	       deltah = 0;

	       oldj = j;
	       while (deltah < ROUND(1.5 * hauteurTexte) && j > ndeb)
		  {
		  c_xy2fxfy(&hx1, &hy1, (float)i, (float)(j-1));
		  c_wglxai(&ihx1, &ihy1, hx1, hy1);
		  deltah = ihy0 - ihy1;
		  j--;
		  cells[j - ndeb] = 1;
		  }
	       
	       j = oldj;
	       deltah = 0;
	       while (deltah < ROUND(1.5*hauteurTexte) && j < nfin)
		  {
		  c_xy2fxfy(&hx1, &hy1, (float)i, (float)(j+1));
		  c_wglxai(&ihx1, &ihy1, hx1, hy1);
		  deltah = ihy1 - ihy0;
		  j++;
		  cells[j - ndeb] = 1;
		  }

	       }
	    }
	 
	 if (nbIntervalles == 1)
	    leContour += (intervalles[n]*facteur);
	 else
	    {
	    n++;
	    if (n < nbIntervalles)
	       leContour += ((intervalles[n]*facteur)-leContour);
	    else
	       leContour += 1.0e+10;
	    }
	 }
      }
   free(cells);
   }


