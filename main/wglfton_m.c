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

#include <wgl_x.h>
#include <rpnmacros.h>
#include <rec.h>

/**
  objet:    c_wglfton
  fonction: Routine de gestion des routines de peinture wglrfton et wglpfton
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

extern _XContour xc;

void c_wglfton_m(fld, ni, nj, intervalles, nbIntervalles, facteur, min, max, missing, 
	       colorTable, ncol, flagInterrupt, lissfac)
float *fld;
int ni, nj;
float intervalles[];
int nbIntervalles;
float facteur;
float min, max, missing;
int colorTable[], ncol, flagInterrupt, lissfac;
{
   int i, j, k;
   int ideb, jdeb, ifin, jfin, largeur, hauteur;
   float deltaCol;
   float delta, avg, tempX, tempY;
   float a, b, c, d;
   float ra, rb, rc, rd;
   int oldIndCouleur, indCouleur;
   float del;
   int largeurFenetre, hauteurFenetre;
   int nbint;
   long temps;

   float rint,rfac,rmin,rmax,rmissing;
   int rnbcol, rlissfac;

   float tempsAffPoly, tempsAffPix;
   float xdeb, ydeb, xfin, yfin;

   int mdeb,ndeb,mfin,nfin;

   DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, ni, nj);
   c_wglgvx(&xdeb, &ydeb, &xfin, &yfin);
   c_wglxai(&ideb, &jdeb, xdeb, ydeb);
   c_wglxai(&ifin,   &jfin, xfin, yfin);
   
   largeur = ifin - ideb + 1;
   hauteur = jfin - jdeb + 1;
   
   if (xc.statuts[LISSAGE])
     {
     c_wglgwz(&largeurFenetre, &hauteurFenetre);
     c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);
     if (visInfo.class == PseudoColor)
       {
       c_wglpfton(fld, ni, nj, intervalles, nbIntervalles, facteur, min, max, missing, colorTable, ncol, flagInterrupt, lissfac);
       }
     else
       {
       c_wglpfton32(fld, ni, nj, intervalles, nbIntervalles, facteur, min, max, missing, colorTable, ncol, flagInterrupt, lissfac);
       }
     }
   else
     {
     rmin = min; rmax = max; 
     rmissing = missing;
     rfac = facteur; nbint = nbIntervalles; 
     rnbcol = ncol; rlissfac = lissfac;
     
     f77name(wglrfton)(fld, &ni, &nj, &mdeb, &ndeb, &mfin, &nfin, intervalles, 
		       &nbint, &rfac, &rmin, &rmax, &rmissing, colorTable, &rnbcol, &flagInterrupt, &rlissfac);
     }
}
