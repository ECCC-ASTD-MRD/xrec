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
#include <rec.h>
#include <memory.h>
#include <rpnmacros.h>
#include <xinit.h>
#include <math.h>
#include <gmp.h>
#include <wgl.h>
#include <souris.h>

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


AfficherCoupe(indChamp, fld, uu, vv, ww, uut, uun, uvw)
int indChamp;
float *fld, *uu, *vv, *uut, *uun, *ww, *uvw;
{
   Hilo hilo[64];
   int hlcount;
   int hlnmax = 64;

   int lissfac;
   int mdim, ndim, ifac, jfac,idim,jdim;
   int mdeb, ndeb, mfin, nfin;
   float xdebut, ydebut, xfin, yfin, xd, yd, xf, yf;
   int idebut, jdebut, ifin, jfin;
   int largeurFenetre, hauteurFenetre;
   int fond;
   int annulationDemandee;
   int transformationFenetre, sensEchelle;
   float valMin, valMax;
   float uutanmin,uutanmax,uvwmin,uvwmax,uumin,uumax,vvmin,vvmax,wwmin,wwmax,nivmin,nivmax;
   float *niveaux;
   int i,op,nbChampsActifs,npts,afficheChampCourant;
   float *localfld, localmin, localmax;
   _Champ *champ,*champ1, *champ2;
   
   if (!CoupeMgrGetStatutCoupe())
      return;

   FldMgrGetChamp(&champ, indChamp);
   c_wglsetw(fenetreCoupe);
   transformationFenetre = c_wglgmod();
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);

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
	 {
	 if (0 == strcmp(champ->nomvar, "UU"))
	   {
	   localfld = uu;
	   }
	 else
	   {
	   localfld = vv;
	   }
	 }
       break;
       
     case SCALAIRE:
       localfld = fld;
       break;
     }
   
   f77name(aminmax)(&localmin,&localmax,localfld,&champ->coupe.niCoupe, &champ->coupe.njCoupe);

   if (champ->natureTensorielle == SCALAIRE)
     {
     if (CUSTOM == DictMgrGetMinMaxMode(champ->nomvar))
       {
       DictMgrGetMinMaxValues(champ->nomvar,&valMin,&valMax);
       valMin /= champ->facteur;
       valMax /= champ->facteur;
       }
     else
       {
       valMax = localmax/champ->facteur;
       valMin = localmin/champ->facteur;
       }
     }

   sensEchelle = CoupeMgrGetSensEchelle();
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
	 f77name(permut)(uu , &champ->coupe.niCoupe, &champ->coupe.njCoupe);
	 f77name(permut)(vv , &champ->coupe.niCoupe, &champ->coupe.njCoupe);
	 f77name(permut)(ww , &champ->coupe.niCoupe, &champ->coupe.njCoupe);
	 f77name(permut)(uvw, &champ->coupe.niCoupe, &champ->coupe.njCoupe);
	 }

      niveaux = (float *) calloc( champ->coupe.nbNiveauxCoupe, sizeof(float));
      for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
	 {
	 niveaux[i] = champ->coupe.niveauxCoupe[champ->coupe.nbNiveauxCoupe-i-1];
	 }
      
      c_wglssp(xdebut, ydebut, xfin, yfin, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
      c_xsetxy(2, NULL, 0, niveaux, champ->coupe.njCoupe);
      free(niveaux);
      }
   else
      {
      c_wglssp(xdebut, yfin, xfin, ydebut, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, transformationFenetre);
      c_xsetxy(2, NULL, 0, champ->coupe.niveauxCoupe, champ->coupe.njCoupe);
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
      c_wglssp(xdebut, ydebut, xfin, yfin, idebut, jdebut, ifin, jfin, transformationFenetre);
      
      c_wglfton(localfld, champ->coupe.niCoupe, champ->coupe.njCoupe, champ->intervalles, 
                champ->nbIntervalles,
                champ->facteur, valMin*champ->facteur, valMax*champ->facteur,
                recColorTable, 256, xc.flagInterrupt, lissfac);
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
       AfficherLIC(uut, ww, champ->coupe.niCoupe, champ->coupe.njCoupe, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].epaisseur,uvwmax);
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
   c_fxfy2xy(&xd, &yd, xdebut, ydebut);
   c_fxfy2xy(&xf, &yf, xfin,   yfin);
   TracerLigne(xd, yd, xf, yd);
   if (indChamp == (FldMgrGetNbChampsActifs()-1) || op != NO_OP) 
      {
      AfficherAxeY(*champ);
      }
   
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
   
   xc.statuts[AXE_Y] = FALSE;
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   UnSetCurseur(fenetreCoupe);
   UnSetCurseur(fenetreAffichage);
}

/**
 ******************************************************************************
 ******************************************************************************
 **/
 
AfficherSerie(indChamp, fld, uut, uun, ww, uvw)
int indChamp;
float *fld, *uut, *uun, *ww, *uvw;
{
   Hilo hilo[64];
   int hlcount;
   int hlnmax = 64;

   int lissfac;
   int mdim, ndim, ifac, jfac,idim,jdim;
   int mdeb, ndeb, mfin, nfin;
   float xdebut, ydebut, xfin, yfin, xd, yd, xf, yf;
   int idebut, jdebut, ifin, jfin;
   int largeurFenetre, hauteurFenetre;
   int fond, un;
   int annulationDemandee;
   int transformationFenetre, sensEchelle;
   float valMin, valMax;
   float uumin,uumax,uvwmin,uvwmax,wwmin,wwmax,nivmin,nivmax;
   float *niveaux;
   int i,op,nbChampsActifs,npts,afficheChampCourant;
   _Champ *champ,*champ1, *champ2;
   
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
	 AfficherProfilSerie(champ->seqanim.xmin,champ->seqanim.xmax);
	 xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
	 xc.statuts[AXE_Y] = FALSE;
         }
      return;
      }
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   

   npts = champ->seqanim.niSerie*champ->seqanim.njSerie;
   un = 1;
   f77name(aminmax)(&valMin,&valMax,champ->seqanim.valeursSeries,&npts,&un);
   
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

AfficherTopo()
{
   _Champ *champ;
   int i,valtoggle,ier;
   int sensEchelle;
    int indmin,indmax;
    struct point
       {
       float x;
       float y;
       } poly[5];
    
    float startpt;
    
    valtoggle = CoupeMgrGetToggleTopo();
    
    if (!valtoggle)
         return;
         
    c_wglsetw(fenetreAffichage);
    ier = FldMgrPreparerTopo();
    c_wglsetw(fenetreCoupe);
    if (ier < 0)
        return;
        
    FldMgrGetChamp(&champ,0);     
    c_wglgetcolrange(&indmin, &indmax);
    c_wglcol(indmax);
    
    if (c_wglgdbg())
       {
       for (i=0; i < champ->coupe.niCoupe - 1; i++)
           {
           printf("%d %f\n",i,champ->coupe.ligneMontagnes[i]);
           }
       }
       
    if (champ->ip1 >= 12000)
       {
       startpt =  champ->coupe.niveauMin;
       }
    else
       {
       startpt =  champ->coupe.niveauMax;
       }
      
    for (i=0; i < champ->coupe.niCoupe - 1; i++)
        {
        poly[0].x = (float) (i+1);
        poly[0].y = startpt;
        poly[1].x = (float) (i+2);
        poly[1].y = poly[0].y;
        poly[2].x = poly[1].x;
        poly[2].y = champ->coupe.ligneMontagnes[i+1];
        poly[3].x = poly[0].x;
        poly[3].y = champ->coupe.ligneMontagnes[i];
        poly[4].x = poly[0].x;
        poly[4].y = poly[0].y;
        c_wglpfx(5,poly);
        }
    }


ManipulerEtAfficherCoupeVerticale(indChamp)
{
   int i,op,nbChampsActifs,npts,afficheChampCourant;
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
   
   AfficherCoupe(indChamp,valeurs,uu,vv,ww,uut,uun,ww);
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

ManipulerEtAfficherSerieTemporelle(indChamp)
{
   int i,op,nbChampsActifs,npts,afficheChampCourant;
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
      tmpfld = (float *)calloc(npts,  sizeof(float));
      valeurs = tmpfld;
      
      DiffMgrCalcDiffs(tmpfld,(*champ1).seqanim.valeursSeries,(*champ2).seqanim.valeursSeries,npts,op);
      }
   
   AfficherSerie(indChamp,valeurs,uut,uun,ww,uvw);

   if (customFld)
      {
      free(tmpfld);
      }
   }

