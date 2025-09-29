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

#include <string.h>
#include <fcntl.h>
#include <memory.h>
#include <xinit.h>
#include <math.h>
#include <wgl.h>
#include <souris.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>


#define  TERMINE         0
#define  OUVRIR          1
#define  FERMER          2
#define  INFO_REC        3
#define  NOUVEAU_CHAMP   4
#define  QUITTER        -1

extern _XContour     xc;
extern _Viewport     viewp;
extern _ColormapInfo recCmap;

extern int recColorTable[];
extern int facteurLissage;
extern int fenetreAffichage;
extern int fenetreCoupe,fenetreSerie;

extern float labelPos[][4];

/**
****
****
**/

void AfficherCoupe(int indChamp, float *fld, float *uu, float *vv, float *uut, float *uun, float *ww, float *uvw)
{
   Hilo hilo[64];
   int hlcount;
   int hlnmax = 64;

   int lissfac;
   int largeur, hauteur;
   float xdebut, ydebut, xfin, yfin, xd, yd, xf, yf;
   int idebut, jdebut, ifin, jfin;
   int largeurFenetre, hauteurFenetre;
   int ier, fond, k1, k2;
   int transformationFenetre, sensEchelle;
   float valMin, valMax;
   float uutanmin,uutanmax,uvwmin,uvwmax,uumin,uumax,vvmin,vvmax,wwmin,wwmax,nivmin,nivmax;
   float *niveaux;
   int i,j,op,npts;
   float *localfld;
   _Champ *champ;
   int nbNiveauxCoupe, zstart;
   int un = 1;
   int selectedVertCoord;
   float *x, *y,*z, termA, termB;

   z = NULL;
   zstart = 0;
   niveaux = NULL;
   if (!CoupeMgrGetStatutCoupe())
      return;

   FldMgrGetChamp(&champ, indChamp);

   c_wglsetw(fenetreCoupe);
   transformationFenetre = c_wglgmod();
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);
   selectedVertCoord = GetSelectedVertCoord();

   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   xc.statuts[AXE_Y] = TRUE;

   if (indChamp == 0)
      {
      if (champ->coupe.coupeValide)
        {
        AjusterViewport(&viewp);
        EffacerLaFenetreSiNecessaire();
        }
      else
        {
        c_wglcol(NOIR);
        c_wglclr();
        }
      }

   if (champ->coupe.coupeValide == 0)
      {
      xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
      xc.statuts[AXE_Y] = FALSE;
      return;
      }

   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();
   CoupeMgrGetLimites(&valMin, &valMax, &ydebut, &yfin);

   if ((*champ).coupe.niCoupe == 1)
      {
      if (indChamp == 0)
        {
        c_wgllwi(1);
        c_xsetxy(0, NULL, 0, NULL, 0);
        AfficherProfilCoupe(champ->coupe.xmin,champ->coupe.ymin);
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        xc.statuts[AXE_Y] = FALSE;
         }
      return;
      }

   c_wglgwz(&largeurFenetre, &hauteurFenetre);


   xdebut = 1.0;
   xfin   = (float)champ->coupe.niCoupe;

   switch(champ->natureTensorielle)
     {
     case VECTEUR:
     localfld = uvw;
     break;

     case SCALAIRE:
     localfld = fld;
     break;
     }

/*    f77name(aminmax)(&localmin,&localmax,localfld,&champ->coupe.niCoupe, &champ->coupe.njCoupe); */

     if (CUSTOM == DictMgrGetMinMaxMode(champ->nomvar))
       {
       DictMgrGetMinMaxValues(champ->nomvar,&valMin,&valMax);
       valMin /= champ->facteur;
       valMax /= champ->facteur;
       }

   sensEchelle = CoupeMgrGetSensEchelle();
   if (sensEchelle == CROISSANTE)
      {
      nbNiveauxCoupe = champ->coupe.nbNiveauxCoupe;
      niveaux = (float *) malloc( champ->coupe.nbNiveauxCoupe *sizeof(float));
      for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
        {
        niveaux[i] = champ->coupe.niveauxCoupe[champ->coupe.nbNiveauxCoupe-i-1];
        }

      if (champ->natureTensorielle == SCALAIRE)
        {
        f77name(permut)(localfld, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        }
      else
        {
        f77name(permut)(uut, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(uun, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(uu , &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(vv , &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(ww , &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(uvw, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        }

      switch (champ->coordonneeVerticale)
        {
        case SIGMA:
        case HYBRIDE:
        switch (selectedVertCoord)
          {
          case NATIVE_VCOORD:
          c_wglssp(xdebut, ydebut, xfin, yfin, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
          c_xsetxy(2, NULL, 0, niveaux, champ->coupe.njCoupe);
          break;

          case PRES_VCOORD:
          if (NULL == champ->coupe.montagnes)
            {
            ier = GetSurfacePressure(champ);
            }
          npts = champ->coupe.niCoupe*champ->coupe.nbNiveauxCoupe;
          z = (float *) malloc(npts * sizeof(float));
          zstart = FTN2C(0, (nbNiveauxCoupe-1),champ->coupe.niCoupe);
          f77name(ez_rgdint_1_nw)(&z[zstart], champ->coupe.x, champ->coupe.y, &champ->coupe.niCoupe, champ->coupe.montagnes, &champ->dst.ni, &un, &champ->dst.nj);
          switch (champ->coordonneeVerticale)
            {
            case SIGMA:
            case HYBRIDE:
            for (j=1; j < champ->coupe.njCoupe; j++)
              {
              termB = powf(((niveaux[j] - (champ->coupe.ptop/champ->coupe.pref)) / (1.0 - (champ->coupe.ptop/champ->coupe.pref))), champ->coupe.rcoef);
              termA = champ->coupe.pref*(niveaux[j]-termB);
              for (i=0; i < champ->coupe.niCoupe; i++)
                {
                k1 = FTN2C(i, j, champ->coupe.niCoupe);
                k2 = FTN2C(i, (nbNiveauxCoupe-1), champ->coupe.niCoupe);
                z[k1] = termA +  termB*z[k2]  ;
                }
              }
            break;
            }
          c_wglssp(xdebut, ydebut, xfin, yfin, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
          c_xsetxy(5, NULL, champ->coupe.niCoupe, z, champ->coupe.njCoupe);
          break;
          }
        break;

        default:
        c_wglssp(xdebut, ydebut, xfin, yfin, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
        c_xsetxy(2, NULL, 0, niveaux, champ->coupe.njCoupe);
        break;
        }
      }
   else  /*Echelle decroissante */
      {
      switch (champ->coordonneeVerticale)
        {
        case SIGMA:
        case HYBRIDE:
        switch (selectedVertCoord)
          {
          case NATIVE_VCOORD:
          niveaux = (float *) malloc( champ->coupe.nbNiveauxCoupe*sizeof(float));
          for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
            {
            niveaux[i] = champ->coupe.niveauxCoupe[i];
            }
          c_wglssp(xdebut, yfin, xfin, ydebut, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
          c_xsetxy(2, NULL, 0, niveaux, champ->coupe.njCoupe);
          break;

          case PRES_VCOORD:
          if (NULL == champ->coupe.montagnes)
            {
            ier = GetSurfacePressure(champ);
            }
          niveaux = (float *) calloc( champ->coupe.nbNiveauxCoupe, sizeof(float));
          for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
            {
            niveaux[i] = champ->coupe.niveauxCoupe[i];
            }
          npts = champ->coupe.niCoupe*champ->coupe.nbNiveauxCoupe;
          z = (float *) malloc(npts * sizeof(float));
          f77name(ez_rgdint_1_nw)(z, champ->coupe.x, champ->coupe.y, &champ->coupe.niCoupe, champ->coupe.montagnes, &champ->dst.ni, &un, &champ->dst.nj);
          switch (champ->coordonneeVerticale)
            {
            case SIGMA:
   /*
   P    = termA + termB * Ps;
   termA = Pref*(HYB-termB);
   termB = [(HYB-Pt/Pref)/(1.0-Pt/Pref)]**Coef
   */
            case HYBRIDE:
            for (j=1; j < champ->coupe.njCoupe; j++)
              {
              termB = powf(((champ->coupe.niveauxCoupe[j] - (champ->coupe.ptop/champ->coupe.pref)) / (1.0 - (champ->coupe.ptop/champ->coupe.pref))), champ->coupe.rcoef);
              termA = champ->coupe.pref*(champ->coupe.niveauxCoupe[j]-termB);
              for (i=0; i < champ->coupe.niCoupe; i++)
                {
                k1 = FTN2C(i, j, champ->coupe.niCoupe);
                k2 = FTN2C(i, 0, champ->coupe.niCoupe);
                z[k1] = termA +  termB*z[k2]  ;
                }
              }
            break;
            }
          c_wglssp(xdebut, yfin, xfin, ydebut, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
          c_xsetxy(5, NULL, champ->coupe.niCoupe, z, champ->coupe.njCoupe);
  /*        free(niveaux);*/
          break;
          }
        break;

        default:
        niveaux = (float *) malloc( champ->coupe.nbNiveauxCoupe*sizeof(float));
        for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
          {
          niveaux[i] = champ->coupe.niveauxCoupe[i];
          }
        c_wglssp(xdebut, yfin, xfin, ydebut, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
        c_xsetxy(2, NULL, 0, niveaux, champ->coupe.njCoupe);
        }
      }

    c_wglgsx(&xdebut, &ydebut, &xfin, &yfin);
    c_wglcmx(xdebut, ydebut, xfin, yfin);


   lissfac = facteurLissage;
   if (xc.statuts[LISSAGE])
      {
      if (lissfac == -32767)
        {
        lissfac = CalculerFacteurLissage(champ->coupe.niCoupe, champ->coupe.njCoupe);
        }
      else
        {
        lissfac = facteurLissage;
        }
      }
   else
      {
      lissfac = 1;
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[AXE_Y] = FALSE;
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

   c_wglgsp(&xdebut, &ydebut, &xfin, &yfin, &idebut, &jdebut, &ifin, &jfin);

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[AXE_Y] = FALSE;
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

   if (AfficherItem(indChamp, COULEURS))
      {
      largeur = viewp.vi2- viewp.vi1 + 1;
      hauteur = viewp.vj2- viewp.vj1 + 1;
      c_wglcol(xc.attributs[0].indCouleurBack);
      c_wglrfi(0, 0, largeurFenetre, hauteurFenetre);
      c_wglssp(xdebut, ydebut, xfin, yfin, idebut, jdebut, ifin, jfin, transformationFenetre);

      if ((champ->coordonneeVerticale == SIGMA || champ->coordonneeVerticale == HYBRIDE) && selectedVertCoord == PRES_VCOORD)
        {
        x = (float *) malloc(largeur*hauteur*sizeof(float));
        y = (float *) malloc(largeur*hauteur*sizeof(float));
        if (sensEchelle == DECROISSANTE)
          {
          f77name(ij2xy_warp)(x, y, champ->coupe.niveauxCoupe, z, &(champ->coupe.ptop),
            &(champ->coupe.pref), &(champ->coupe.rcoef), &transformationFenetre,
            &(champ->coupe.niCoupe), &(champ->coupe.njCoupe), &viewp.vi1, &viewp.vj1, &viewp.vi2, &viewp.vj2,
            &xdebut, &ydebut, &xfin, &yfin);
          }
        else
          {
          f77name(ij2xy_warp_r)(x, y,champ->coupe.niveauxCoupe , &z[zstart], &(champ->coupe.ptop),
            &(champ->coupe.pref), &(champ->coupe.rcoef), &transformationFenetre,
            &(champ->coupe.niCoupe), &(champ->coupe.njCoupe), &viewp.vi1, &viewp.vj1, &viewp.vi2, &viewp.vj2,
            &xdebut, &ydebut, &xfin, &yfin);
          }
        c_wglpfton32_x(localfld, x, y, champ->coupe.niCoupe, champ->coupe.njCoupe, champ->intervalles,
                  champ->nbIntervalles,
                  champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
                  recColorTable, 256, xc.flagInterrupt, lissfac);
        free(x);
        free(y);
        }
      else
        {
        c_wglfton(localfld, champ->coupe.niCoupe, champ->coupe.njCoupe, champ->intervalles,
                  champ->nbIntervalles,
                  champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
                  recColorTable, 256, xc.flagInterrupt, lissfac);
        }
      }
   else
      {
      if (indChamp == 0)
         {
         c_wglcol(xc.attributs[0].indCouleurBack);
         c_wglrfi(0, 0, largeurFenetre, hauteurFenetre);
         }
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
         {
         xc.statuts[AXE_Y] = FALSE;
         xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
         return;
         }
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
         {
         xc.statuts[AXE_Y] = FALSE;
         xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
         return;
         }
      }

   if (champ->natureTensorielle == VECTEUR && champ->cle >= 0 && xc.statuts[BARBULES])
     {
     if (0 != WindMgrGetLICState())
       {
       SetClipMask();
       uvwmax = 0; /* pas utilisé */
       AfficherLIC(uut, ww, champ->coupe.niCoupe, champ->coupe.njCoupe, xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].epaisseur, uvwmax);
       }
     }

   if (AfficherItem(indChamp, CONTOURS))
      {
      c_wglcont(localfld, champ->coupe.niCoupe, champ->coupe.njCoupe, champ->intervalles,
                champ->nbIntervalles,
                champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
                xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].epaisseur,
                xc.attributs[indChamp].codeDash, xc.attributs[indChamp].style, lissfac, xc.flagInterrupt);

      c_wgllwi(1);
      c_wglsld(0);
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
         {
         xc.statuts[AXE_Y] = FALSE;
         xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
         return;
         }
      }

   if (xc.statuts[LABELS] && champ->natureTensorielle == SCALAIRE && champ->cle >= 0)
      {
      switch(xc.attributs[indChamp].couleurFore)
         {
         case BLANC:
         case JAUNE:
         case CYAN:
           fond = NOIR;
           break;

         default:
           fond = BLANC;
           break;
         }

      c_wgllab(localfld, champ->coupe.niCoupe, champ->coupe.njCoupe, champ->intervalles, champ->nbIntervalles,
               champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
               labelPos[indChamp%4], 4,
               xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].indCouleurBack, indChamp, lissfac);
      }

   if (champ->natureTensorielle == VECTEUR && xc.statuts[BARBULES] &&champ->cle >= 0)
      {
      SetClipMask();
      c_wgllwi(1);
      if (-1 != WindMgrGetDisplayMode())
         {
         CoupeMgrGetLimitesUVW(&uutanmin,&uutanmax,&uvwmin,&uvwmax,
                               &uumin,&uumax,&vvmin,&vvmax,
                               &wwmin,&wwmax,&nivmin,&nivmax);

         if (CUSTOM == DictMgrGetMinMaxMode("UV"))
            {
            DictMgrGetMinMaxValues("UV",&uvwmin,&uvwmax);
            }

         AfficherFleches(uut, ww, champ->coupe.niCoupe, champ->coupe.njCoupe,
                         xc.attributs[indChamp].indCouleurFore,
                         xc.attributs[indChamp].epaisseur,uvwmax);
         if (0 != WindMgrGetDisplayMode())
           AfficherLegendeVentUVW(uvwmax);
         }
      }

   if (!xc.statuts[BARBULES] && xc.statuts[VALEURS_CENTRALES])
      {
      hl_find (localfld, champ->coupe.niCoupe, champ->coupe.njCoupe, 1.0, hilo,&hlcount,hlnmax);
      hl_print (hilo,hlcount,champ->facteur, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].indCouleurBack);
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
         {
         xc.statuts[AXE_Y] = FALSE;
         xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
         return;
         }
      }

   op = CtrlMgrGetMathOp();
   if (indChamp == (FldMgrGetNbChampsActifs()-1) || op != NO_OP)
      {
      AfficherTopo();
      }

   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);

   AfficherLegendeCoupe(indChamp);

   if (xc.statuts[COULEURS])
      {
      c_wglcol(xc.attributs[FOND].indCouleurBack);
      }
   else
      {
      c_wglcol(NOIR);
      }

   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);
   if (indChamp == (FldMgrGetNbChampsActifs()-1) || op != NO_OP)
      {
      AfficherAxeY(*champ);
      }

   c_xsetxy(2, NULL, 0, niveaux, champ->coupe.njCoupe);
   c_fxfy2xy(&xd, &yd, xdebut, ydebut);
   c_fxfy2xy(&xf, &yf, xfin,   yfin);
   TracerLigne(xd, yd, xf, yd);

   AfficherPerimetreFenetre();
   if (sensEchelle == CROISSANTE)
      {
      if (champ->natureTensorielle == SCALAIRE)
        {
        f77name(permut)(localfld, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        }
      else
        {
        f77name(permut)(uut, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(uun, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(uu, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(vv, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(ww, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
        f77name(permut)(uvw, &champ->coupe.niCoupe, &champ->coupe.njCoupe);

        }
      }

  if (z != NULL)
    {
    free(z);
    }
  xc.statuts[AXE_Y] = FALSE;
  xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
  UnSetCurseur(fenetreCoupe);
  UnSetCurseur(fenetreAffichage);
  if (NULL != niveaux) free(niveaux);
}

/**
 ******************************************************************************
 ******************************************************************************
 **/

void AfficherSerie(int indChamp, float *fld, float *uut, float *uun, float *ww, float *uvw)
{
   Hilo hilo[64];
   int hlcount;
   int hlnmax = 64;

   int lissfac;
   float xdebut, ydebut, xfin, yfin, tdebut, tfin, xd, yd, xf, yf;
   int idebut, jdebut, ifin, jfin;
   int largeurFenetre, hauteurFenetre;
   int fond, un;
   int transformationFenetre;
   float valMin, valMax;
   float *niveaux;
   int i, op, npts;
   _Champ *champ;

   if (!SerieMgrGetStatutSerie())
      return;

   FldMgrGetChamp(&champ, indChamp);
   c_wglsetw(fenetreSerie);
   transformationFenetre = c_wglgmod();
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);

   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   xc.statuts[AXE_Y] = TRUE;

   if (indChamp == 0)
      {
      if (champ->seqanim.nbFldsAnim > 0)
        {
        AjusterViewport(&viewp);
        EffacerLaFenetreSiNecessaire();
        }
      else
        {
        c_wglcol(NOIR);
        c_wglclr();
        }
      }

   if (champ->seqanim.nbFldsAnim == 0)
      {
      xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
      xc.statuts[AXE_Y] = FALSE;
      return;
      }

   SerieMgrSetMinMax();
   SerieMgrSetUVWMinMax();

   if ((*champ).seqanim.niSerie == 1)
      {
      if (indChamp == 0)
        {
        SerieMgrGetLimites(&valMin, &valMax, &ydebut, &yfin);
        c_wgllwi(1);
        c_xsetxy(0, NULL, 0, NULL, 0);
        AfficherProfilSerie(champ->seqanim.xmin,champ->seqanim.ymin);
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        xc.statuts[AXE_Y] = FALSE;
        }
      return;
      }

   c_wglgwz(&largeurFenetre, &hauteurFenetre);


   npts = champ->seqanim.niSerie*champ->seqanim.njSerie;
   un = 1;
   f77name(aminmax)(&valMin,&valMax,champ->seqanim.valeursSeries,&npts,&un);
   SerieMgrGetLimites(&tdebut, &tfin, &valMin, &valMax);
/*   fprintf(stderr, "valMin: %f valMax: %f\n", valMin, valMax); */

   xdebut = 1.0;
   xfin   = (float)champ->seqanim.niSerie;
   ydebut = (float)(champ->seqanim.ip2s[0]);
   yfin = (float)(champ->seqanim.ip2s[champ->seqanim.nbFldsAnim-1]);
   niveaux = (float *) calloc(champ->seqanim.nbFldsAnim, sizeof(float));

   for (i=0; i < champ->seqanim.nbFldsAnim; i++)
     {
     niveaux[i] = (float)(champ->seqanim.ip2s[i]);
     }

   c_wglssp(xdebut, ydebut, xfin, yfin, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
   c_xsetxy(2, NULL, 0, niveaux, champ->seqanim.njSerie);

   free(niveaux);
   c_wglgsx(&xdebut, &ydebut, &xfin, &yfin);
   c_wglcmx(xdebut, ydebut, xfin, yfin);


   lissfac = facteurLissage;
   if (xc.statuts[LISSAGE])
      {
      if (lissfac == -32767)
        {
        lissfac = CalculerFacteurLissage(champ->seqanim.niSerie, champ->seqanim.njSerie);
        }
      else
        {
        lissfac = facteurLissage;
        }
      }
   else
      {
      lissfac = 1;
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[AXE_Y] = FALSE;
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

   c_wglgsp(&xdebut, &ydebut, &xfin, &yfin, &idebut, &jdebut, &ifin, &jfin);

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[AXE_Y] = FALSE;
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

      if (indChamp == 0)
        {
        if (champ->natureTensorielle == SCALAIRE && champ->cle >= 0)
            {
            switch(xc.statuts[COULEURS])
              {
              case TRUE:
              c_wglssp(xdebut, ydebut, xfin, yfin, idebut, jdebut, ifin, jfin, transformationFenetre);

              c_wglfton(fld, champ->seqanim.niSerie, champ->seqanim.njSerie, champ->intervalles,
                        champ->nbIntervalles,
                        champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
                        recColorTable, 256, xc.flagInterrupt, lissfac);
              break;

              case FALSE:
              break;
              }
            }
        else
            {
            c_wglcol(xc.attributs[0].indCouleurBack);
            c_wglrfi(0, 0, largeurFenetre, hauteurFenetre);
            }

        if (xc.flagInterrupt)
            {
            if (c_wglanul())
              {
              xc.statuts[AXE_Y] = FALSE;
              xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
              return;
              }
            }
        }

      if (xc.flagInterrupt)
        {
        if (c_wglanul())
            {
            xc.statuts[AXE_Y] = FALSE;
            xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
            return;
            }
        }

      if ((xc.statuts[CONTOURS] == TRUE || indChamp > 0) && champ->natureTensorielle == SCALAIRE && champ->cle >= 0)
        {
        c_wglcont(fld, champ->seqanim.niSerie, champ->seqanim.njSerie, champ->intervalles,
            champ->nbIntervalles,
            champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
            xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].epaisseur,
            xc.attributs[indChamp].codeDash, xc.attributs[indChamp].style, lissfac, xc.flagInterrupt);

        c_wgllwi(1);
        c_wglsld(0);
        }

      if (xc.flagInterrupt)
        {
              if (c_wglanul())
            {
            xc.statuts[AXE_Y] = FALSE;
            xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
            return;
            }
        }

      if (xc.statuts[LABELS] && champ->natureTensorielle == SCALAIRE && champ->cle >= 0)
        {
        switch(xc.attributs[indChamp].couleurFore)
            {
            case BLANC:
            case JAUNE:
            case CYAN:
            fond = NOIR;
            break;

            default:
            fond = BLANC;
            break;
            }

        c_wgllab(fld, champ->seqanim.niSerie, champ->seqanim.njSerie, champ->intervalles, champ->nbIntervalles,
            champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
            labelPos[indChamp%4], 4,
            xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].indCouleurBack, indChamp, lissfac);
        }

      if (xc.statuts[VALEURS_CENTRALES])
        {
        hl_find (fld, champ->seqanim.niSerie, champ->seqanim.njSerie, 1.0, hilo,&hlcount,hlnmax);
        hl_print (hilo,hlcount,champ->facteur, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].indCouleurBack);
        }

      if (xc.flagInterrupt)
        {
        if (c_wglanul())
            {
            xc.statuts[AXE_Y] = FALSE;
            xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
            return;
            }
        }

      c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);

      AfficherLegendeSerie(indChamp);

      if (xc.statuts[COULEURS])
         {
         c_wglcol(xc.attributs[FOND].indCouleurBack);
         }
      else
         {
         c_wglcol(NOIR);
         }

      c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);
      c_fxfy2xy(&xd, &yd, xdebut, ydebut);
      c_fxfy2xy(&xf, &yf, xfin,   yfin);
      TracerLigne(xd, yd, xf, yd);
      op = CtrlMgrGetMathOp();
      if (indChamp == (FldMgrGetNbChampsActifs()-1) || op != NO_OP)
         {
         AfficherAxeY(*champ);
         }

      AfficherPerimetreFenetre();

      xc.statuts[AXE_Y] = FALSE;
      xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
      UnSetCurseur(fenetreSerie);
      UnSetCurseur(fenetreAffichage);
}

/**
******************************************************************************
******************************************************************************
**/

void AfficherTopo()
{
   _Champ *champ;
   int i,valtoggle,ier;
   int sensEchelle, selectedVertCoord;
    int indmin,indmax;
    float poly[5][2];
    float startpt;

    FldMgrGetChamp(&champ,0);

    valtoggle = CoupeMgrGetToggleTopo();
    selectedVertCoord = GetSelectedVertCoord();

    if (!valtoggle && selectedVertCoord != PRES_VCOORD)
         return;

    ier = FldMgrPreparerTopo();
    if (champ->coordonneeVerticale != SIGMA && champ->coordonneeVerticale != HYBRIDE)
      {
      c_wglsetw(fenetreAffichage);
      if (ier < 0)
          return;
      }

    c_wglsetw(fenetreCoupe);

    c_wglgetcolrange(&indmin, &indmax);
    c_wglcol(xc.attributs[TOPOG].indCouleurFore);

    if (c_wglgdbg())
       {
       for (i=0; i < champ->coupe.niCoupe - 1; i++)
           {
           printf("%d %f\n",i,champ->coupe.ligneMontagnes[i]);
           }
       }

    switch(champ->coordonneeVerticale)
      {
      case METRES:
      startpt =  champ->coupe.niveauMin;
      break;

      case PRESSION:
      startpt =  champ->coupe.niveauMax;
      break;

      case SIGMA:
      case HYBRIDE:
      switch (selectedVertCoord)
        {
        case PRES_VCOORD:
        startpt =  champ->coupe.niveauPresMax;
        break;

        default:
        startpt =  champ->coupe.niveauMax;
        break;
        }
      break;

      default:
      startpt =  champ->coupe.niveauMin;
      break;
      }

    for (i=0; i < champ->coupe.niCoupe - 1; i++)
        {
        poly[0][0] = (float) (i+1);
        poly[0][1] = startpt;
        poly[1][0] = (float) (i+2);
        poly[1][1] = poly[0][1];
        poly[2][0] = poly[1][0];
        poly[2][1] = champ->coupe.ligneMontagnes[i+1];
        poly[3][0] = poly[0][0];
        poly[3][1] = champ->coupe.ligneMontagnes[i];
        poly[4][0] = poly[0][0];
        poly[4][1] = poly[0][1];
        c_wglpfx(5,poly);
        }
    }


void ManipulerEtAfficherCoupeVerticale(int indChamp)
  {
  int op,nbChampsActifs,npts;
  float *tmpfld,*valeurs,*fld,*uu,*vv,*uut,*uun,*ww,*uvw;
  _Champ *champ,*champ1, *champ2;
  int customFld;

  customFld = False;
  FldMgrGetChamp(&champ, indChamp);
  fld = champ->coupe.fld2d;
  uu      = champ->coupe.uu2d;
  vv      = champ->coupe.vv2d;
  ww      = champ->coupe.ww2d;

  uut     = champ->coupe.uvwtang2d;
  uun     = champ->coupe.uvwnorm2d;
  uvw     = champ->coupe.uvw2d;

  op = CtrlMgrGetMathOp();
  nbChampsActifs = FldMgrGetNbChampsActifs();
  valeurs = fld;
  if (0 == (nbChampsActifs %2) && op != NO_OP)
    {
    if (0 != indChamp%2)
      {
      return;
      }

    champ1 = champ;
    FldMgrGetChamp(&champ2,indChamp+1);

    customFld = True;
    npts = (*champ).coupe.niCoupe*(*champ).coupe.njCoupe;
    tmpfld = (float *)calloc(npts,  sizeof(float));
    valeurs = tmpfld;

    DiffMgrCalcDiffs(tmpfld,(*champ1).coupe.fld2d,(*champ2).coupe.fld2d,npts,op);
    }

   AfficherCoupe(indChamp,valeurs,uu,vv,uut,uun,ww, uvw);
   /*   AfficherCoupe(indChamp,fld,uut,uun,ww,uvw);*/

  if (customFld)
     {
     free(tmpfld);
     }
  }
/**
****
****
**/

void ManipulerEtAfficherSerieTemporelle(int indChamp)
{
   int op,nbChampsActifs,npts;
   float *tmpfld,*valeurs,*fld,*uut,*uun,*ww,*uvw;
   _Champ *champ,*champ1, *champ2;
   int customFld;

   customFld = False;
   FldMgrGetChamp(&champ, indChamp);
   fld = champ->seqanim.valeursSeries;

   op = CtrlMgrGetMathOp();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   valeurs = fld;
   if (0 == (nbChampsActifs %2) && op != NO_OP)
      {
      if (0 != indChamp%2)
         {
         return;
         }

      champ1 = champ;
      FldMgrGetChamp(&champ2,indChamp+1);

      customFld = True;
      npts = (*champ).seqanim.niSerie*(*champ).seqanim.njSerie;
      tmpfld = (float *)malloc(npts* sizeof(float));
      valeurs = tmpfld;

      DiffMgrCalcDiffs(tmpfld,(*champ1).seqanim.valeursSeries,(*champ2).seqanim.valeursSeries,npts,op);
      }

   uut = uun = ww = uvw = NULL; /* pas utilisés */
   AfficherSerie(indChamp,valeurs,uut,uun,ww,uvw);

   if (customFld)
      {
      free(tmpfld);
      }
   }

