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


#define C_TO_FTN(i,j,ni)  (int)((ni) * (j) + i)

extern _Viewport    viewp;
extern _XContour xc;


void c_wglpfton32_m(float *fld, unsigned int *mask, int ni, int nj, float intervalles[], int nbIntervalles, float facteur,
	       float min, float max, float missing, int colorTable[], int ncol, int flagInterrupt, int lissfac)
{
  int i,j,k, idebut, jdebut, ifin, jfin, largeur, hauteur;
  unsigned int *pixdata, *pixels;
  float *axex, *axey, *axdx;
  int *iaxex,  ioffset, joffset;
  
  int ix, iy, iy1, iy2;
  float x,y,val, dx, dy, x1, y1, x2, y2, fld1, fld2, fld3;
  
  int local_ni, local_nj,  largeurFenetre, hauteurFenetre, lastx, lastCol, col;
  float lasty, ContourMin, ContourMax, leContour, delta;
  int indCouleur, mdeb, ndeb, mfin, nfin, entier;
  float fraction;
  
  unsigned int pixel;
  float rfac, eps, *tmpVals, *tmpInds,  colrange;
  int ii, jj, lh, slice, curslice;
  
  float f_fond;
  unsigned int pix_fond;
  
  XImage *image;
  
  c_wglgwz(&largeurFenetre,&hauteurFenetre);
  
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
  
  slice = 16;
  curslice = 0;
  lh = largeur * slice;
  
  axex    = (float *) calloc(largeur, sizeof(float));
  axdx    = (float *) calloc(largeur, sizeof(float));
  iaxex    = (int *) calloc(largeur, sizeof(int));
  axey    = (float *) calloc(hauteur, sizeof(float));
  tmpVals = (float *) calloc(lh, sizeof(float));
  tmpInds = (float *) calloc(lh, sizeof(float));
  pixels =  (unsigned int *)malloc(2*lh*sizeof(unsigned int));
  pixdata = (unsigned int *) malloc(2*lh*sizeof(unsigned int));
  
  
  image = XCreateImage(wglDisp, visInfo.visual, visInfo.depth, ZPixmap, 0, (char *)pixdata,largeur, slice, 32, 0);
  
  ContourMin = min;
  ContourMax = max;
  
  if (ContourMin == ContourMax)
    {
    c_wglcol(15);
    c_wglclr();
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
    ii = i - idebut;
    c_wgliax(&x, &y, i, 0);
    c_fxfy2xy(&x, &y, x, y);
    ix = (int) x;
    iaxex[ii] = (ix < 1) ? 1 : (ix > ni-1) ? ni-1 : ix;
    axdx[ii] = x - (float)ix;
    }
  
  for (j=jdebut; j <= jfin; j++)
    {
    c_wgliax(&x, &y, 0, j);
    c_fxfy2xy(&x, &y, x, y);
    axey[j - jdebut] = y;
    }
  
  
  colrange = (colorTable[255]-colorTable[0])/256.0;
  for (curslice=0; curslice < hauteur; curslice += slice)
    {
    slice = 16;
    if (((hauteur - curslice) < slice) & (hauteur % slice) != 0)
      {
      slice = hauteur % slice;
      lh = largeur*slice;
      }
    
    for (j=0; j < slice; j++)
      {
      jj = curslice + j;
      if (0 == (jj % 64) && flagInterrupt)
        {
        if (c_wglanul()) goto abort;
        }
      
      y = axey[hauteur-1-jj];
      iy = (int) y;
      iy = (iy < 1) ? 1 : (iy > nj-1) ? nj-1 : iy;
      iy1 = ni * (iy - 1);
      iy2 = ni * (iy);
      dy = y - (float)iy;
      for (i=idebut; i <= ifin; i++)
        {
        ii = i - idebut;
        ix = iaxex[ii];
        if ((0 == GETMSK(mask,(ix-1+iy1))) || (0 == GETMSK(mask,(ix+iy1)))  ||  (0 == GETMSK(mask,(ix-1+iy2)))  || (0 == GETMSK(mask,(ix+iy2))))
          {
          tmpVals[j*largeur+ii] =  MISSING;
          }
        else
          {
          fld1 = fld[ix-1+iy1] + (fld[ix+iy1] - fld[ix-1+iy1]) * axdx[ii];
          fld2 = fld[ix-1+iy2] + (fld[ix+iy2] - fld[ix-1+iy2]) * axdx[ii];
          tmpVals[j*largeur+ii] =  fld1 + (fld2 - fld1) * dy;
          }
        }
      }
      
      c_wglcalcolf_m(tmpInds, tmpVals, lh, ContourMin, delta, intervalles, nbIntervalles, facteur, ncol);
      
      for (i=0; i < lh; i++)
        {
        if (tmpVals[i] != MISSING)
          {
          tmpVals[i] = (float) (colorTable[0] + tmpInds[i]*colrange);
          }
        }
      
      pix_fond = (unsigned int) x_wglpix(xc.attributs[FOND].indCouleurFore);
      c_wglcolfs_fst(tmpVals, pixels, lh);
      for (j=0; j < slice; j++)
        {
        jj = j * largeur;
        for (i=0; i < largeur; i++)
          {
          if (tmpVals[i+jj] == MISSING)
            {
            XPutPixel(image, i, j, pix_fond);
            }
          else
            {
            XPutPixel(image, i, j, pixels[jj+i]);
            }
          }
        }
      
      XPutImage(wglDisp, wglDrawable, wglFillGC, image, 0, 0, idebut, hauteurFenetre - jfin + curslice, largeur, slice);
      }

  
  
 abort:
  free(axex);
  free(axdx);
  free(iaxex);
  free(axey);
  free(tmpVals);
  free(tmpInds);
  free(pixels);
  XDestroyImage(image);
  
}
