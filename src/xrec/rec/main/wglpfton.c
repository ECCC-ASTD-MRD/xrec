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

#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl_x.h>

/**
  objet:    c_wglpfton
  fonction: Affichage en couleurs d'un champ par lignes de pixels
  auteur:   Yves Chartier, fevrier 1992
  Arguments:
    fld:            champ bi-dimensionnel contenant les valeurs a contourer
    ni, nj:         dimensions du champ
    intervalles:    liste d'intervalles de contours
    nbIntervalles:  nombre d'intervalles dans la liste
    facteur:        ordre de grandeur du champ
    min,max:        valeurs min et max du champ
    colortable:     table d'indices de couleurs
    ncol:           nombre de couleurs dans la table
    flagInterrupt:  drapeau indiquant si la routine devra s'informer
                    d'une demande eventuelle d'annulation
    lissfac: niveau de lissage demande
**/

extern _Viewport viewp;
extern int recColorTable[];
#define C_TO_FTN(i,j,ni)  (int)((ni) * (j) + i)

void c_wglpfton(float *fld, int ni, int nj, float intervalles[], int nbIntervalles, float facteur, float min, float max, int colorTable[], int ncol, int flagInterrupt, int lissfac)
{
   int i,j,k;
   int idebut, jdebut, ifin, jfin, largeur, hauteur;
   char *pixdata;
   float *axex, *axey;
   int ioffset, joffset;
   
   int ix, iy, iy1, iy2;
   float x,y,val;
   float dx, dy;
   float x1, y1, x2, y2; 
/**   float x1, y1, x2, y2; **/
   float fld1, fld2, fld3;
   
   int local_ni, local_nj;
   int largeurFenetre, hauteurFenetre;
   int lastx, lastCol, col;
   float lasty;
   float ContourMin, ContourMax, leContour;
   float delta;
   int indCouleur;
   int mdeb, ndeb, mfin, nfin;
   
   float rfac, eps;
   float *tmpVals;
   int *tmpInds;
   
   XImage *image;
   
   c_wglgwz(&largeurFenetre,&hauteurFenetre);
   c_wgllwi(1);
   rfac = (float)lissfac;
   eps = 0.5 / rfac;
   local_ni = ni;
   local_nj = nj;
   
   DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, ni, nj);
   c_xy2fxfy(&x, &y, (float)mdeb, (float)ndeb);
   c_wglxai(&idebut, &jdebut, x, y);
   
   c_xy2fxfy(&x, &y, (float)mfin, (float)nfin);
   c_wglxai(&ifin, &jfin, x, y);
   
   if (idebut < viewp.vi1)
      idebut = viewp.vi1;
   
   if (jdebut < viewp.vj1)
      jdebut = viewp.vj1;
   
   if (ifin > viewp.vi2)
      ifin = viewp.vi2;
   
   if (jfin > viewp.vj2)
      jfin = viewp.vj2;
   
   largeur = ifin - idebut + 1;
   hauteur = jfin - jdebut + 1;
   
   pixdata = (char *) calloc(largeur*hauteur, sizeof(char));

   axex    = (float *) calloc(largeur, sizeof(float));
   axey    = (float *) calloc(hauteur, sizeof(float));
   tmpVals = (float *) calloc(largeur*hauteur, sizeof(float));
   tmpInds = (int   *) calloc(largeur*hauteur, sizeof(int));
   

   image = XCreateImage(wglDisp, visInfo.visual, 8, ZPixmap, 0, pixdata,largeur, hauteur, 8, 0);
   
   ContourMin = min;
   ContourMax = max;
   delta = ContourMax - ContourMin;

   if (ContourMin == ContourMax)
      {
      c_wglcol(15);
      c_wglclr();
      free(tmpVals);
      free(tmpInds);
      return;
      }
   
   if (nbIntervalles == 1)
      {
      if (intervalles[0] != 0.0)
	 {
	 AjusterMinMax(&ContourMin, &ContourMax, facteur, intervalles[0]);
	 }
      
      delta = ContourMax - ContourMin;
      }

   for (i=idebut; i <= ifin; i++)
      {
      c_wgliax(&x, &y, i, 0);
      c_fxfy2xy(&x, &y, x, y);
      axex[i - idebut] = 1.0/rfac*ROUND(rfac*x);
      }
   
   for (j=jdebut; j <= jfin; j++)
      {
      c_wgliax(&x, &y, 0, j);
      c_fxfy2xy(&x, &y, x, y);
      axey[j - jdebut] = 1.0/rfac*ROUND(rfac*y);
      }
   
   for (j=jfin; j > jdebut; j--)
     {
     if (0 == (j % 64) && flagInterrupt)
       {
       if (c_wglanul())
	 {
	 free(tmpVals);
	 free(tmpInds);
	 return;
	 }
       }
     
     y = axey[j-jdebut];
     iy = (int) y;
     iy = (iy < 1) ? 1 : (iy > nj-1) ? nj-1 : iy;
     iy1 = ni * (iy - 1);
     iy2 = ni * (iy);
     dy = y - (float)iy;
     for (i=idebut; i < ifin; i++)
       {
       k = (largeur * (hauteur-(j-jdebut)) + (i - idebut));
       x = axex[i-idebut];
       ix = (int) x;
       ix = (ix < 1) ? 1 : (ix > ni-1) ? ni-1 : ix;
       dx = x - (float)ix;
       if ((dx > eps) || (dy > eps))
	 {
	 x1 = fld[ix-1+iy1];
	 x2 = fld[ix+iy1];
	 y1 = fld[ix-1+iy2];
	 y2 = fld[ix+iy2];
	 fld1 = x1 + (x2 - x1) * dx;
	 fld2 = y1 + (y2 - y1) * dx;
	 fld3= fld1 + (fld2 - fld1) * dy;
	 tmpVals[k] = fld3;
	 }
       else
	 {
	 tmpVals[k] = fld[C_TO_FTN(ix-1,iy-1,ni)];
	 }
       
       }
     }
     
   
   c_wglcalcols(tmpInds, tmpVals, largeur*hauteur, ContourMin, delta, intervalles, nbIntervalles, facteur, ncol);
   
   for (j=jdebut; j <= jfin; j++)
     {
     for (i=idebut; i <= ifin; i++)
       {
       k = (largeur * (j-jdebut) + (i - idebut));
       pixdata[k] = (char) recColorTable[tmpInds[k]];
       }
     }
   
   
   XPutImage(wglDisp, wglDrawable, wglFillGC, image, 0, 0, idebut, hauteurFenetre - jfin, largeur, hauteur);
   XDestroyImage(image);
   
   free(axex);
   free(axey);
   free(tmpVals);
   free(tmpInds);
   
}
