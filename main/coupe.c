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

#include <Xm/Xm.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>

#include <xinit.h>
#include <rec.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <souris.h>
#include <math.h>

#define AUTO_PROFIL 0
#define AUTO_GRILLES 1
#define FIXES       2

#define TOPO_INACTIVE 0
#define TOPO_ACTIVE   1

int dimensionCoupe = ZP;
int echelle = LINEAIRE;
int calculMinMax = AUTO_PROFIL;
int sensEchelle = DECROISSANTE;
int toggleTopo = TOPO_INACTIVE;
float grafMinX, grafMinY, grafMaxX, grafMaxY;
float grafMinUUtang,grafMaxUUtang,grafMinUUnorm,grafMaxUUnorm,grafMinUVW,grafMaxUVW;
float grafMinUU,grafMaxUU,grafMinVV,grafMaxVV,grafMinWW,grafMaxWW;

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern _Viewport    viewp;

XtCallbackProc PcpNouvelleCoupe();
XtCallbackProc PcpScanProfil();
XtCallbackProc PcpScanCoupe();
XtCallbackProc PcpOk();

XtCallbackProc PcpAfficher();

XtCallbackProc PcpSetEchelleLineaire();
XtCallbackProc PcpSetEchelleLog();

XtCallbackProc PcpSetMinMaxProfil();
XtCallbackProc PcpSetMinMaxGrilles();
XtCallbackProc PcpSetMinMaxUsager();

XtCallbackProc PcpSetEchelleCroissante();
XtCallbackProc PcpSetEchelleDecroissante();

XtCallbackProc PcpSetToggleTopo();


extern Widget pcpForme, pcpFrame, pcpForme2, pcpFrame3, pcpForme3, pcpRC, pcpAfficher, pcpOk;
extern Widget pcpFormeBoutons, pcpFormeEchelle, pcpFormeLimite;
extern Widget pcpFrameBoutons, pcpFrameEchelle, pcpFrameLimite;
extern Widget pcpLabelEchelle, pcpFrameLineaire, pcpFrameSens, pcpLineariteEchelle, pcpSensEchelle;
extern Widget pcpFrameTopo,pcpPresenceTopo,pcpTopo;
extern Widget pcpLineaire, pcpLog, pcpCroissante, pcpDecroissante;
extern Widget pcpLabelLimite;
extern Widget pcpFrameTypeLimite, pcpFormeTypeLimite, pcpLimiteCoupe,pcpLimiteDomaine,pcpLimiteUsager;
extern Widget pcpFrameLimiteX, pcpFormeLimiteX, pcpLabelX, pcpLabelMinX, pcpLabelMaxX, pcpTextMinX,pcpTextMaxX;
extern Widget pcpFrameLimiteY, pcpFormeLimiteY, pcpLabelY, pcpLabelMinY, pcpLabelMaxY, pcpTextMinY,pcpTextMaxY;


int fenetreCoupe = 0;
float cx1, cy1, cx2, cy2;
float lastcx1, lastcy1, lastcx2, lastcy2;
extern int statutCoupe;

char *pasAssezDeNiveaux[] = {"Nombre insuffisant\nde niveaux.\nAu moins 4 niveaux\nsont necessaires",
				       "Insufficient number\nof levels.\nAt least 4 levels\n are needed"};
char *nbNiveauxDifferents[] = {"Le nombre de niveaux\nn'est pas le meme\npour tous les champs", 
					 "The number of levels\nis not the same\nfor all fields"};
char *niveauxDifferents[] = {"Les niveaux ne sont pas les memes\npour tous les champs",
				       "The levels are not the same\nfor all fields"};
char *pasAvecDesChampsVectoriels[] = {"On ne peut faire\nde coupes ou de profils\navec des champs vectoriels. Desactivez l'option \"Barbules\"\ndu menu \"Affichage\" pour regarder\nles composantes separement.", 
					      "Vector fields are not supported yet.\nTurn off the \"Wind Barbs\" option\nfrom the \"Display\" menu\nto look at individual components."};

char *bombe[] = {"Une erreur systeme est survenue.\nCette application pourrait inopinement quitter", "A system error occured.\nThis program could unexpectedly quit"};


/**
 ******************************************************************************
 ******************************************************************************
 **/

PreparerCoupeOuSerie(cx1,cy1,cx2,cy2)
float cx1,cy1,cx2,cy2;
{
   int dimensionCoupe;

   dimensionCoupe = CoupeMgrGetDimensionCoupe();

   switch(dimensionCoupe)
      {
      case ZP: 
      PreparerCoupe(cx1,cy1,cx2,cy2);
      break;

      case T: 
      PreparerSerie(cx1,cy1,cx2,cy2);
      break;
      }
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

PreparerSerie(cx1,cy1,cx2,cy2)
float cx1,cy1,cx2,cy2;
{
   _Champ *champ;
   int ixmin, ixmax, iymin, iymax;
   float xmin, xmax, ymin, ymax;
   float niveauMin, niveauMax;
   int i,j,ier;
   int fenetreAffichage, fenetreCoupe;
   int nbChampsActifs;
   float rx1,ry1,rx2,ry2;
   int lng;
   
   lng = c_getulng();
   
   ixmin = 0; iymin = 0;
   ixmax = 0; iymax = 0;

   rx1 = cx1; rx2 = cx2; ry1 = cy1; ry2 = cy2;
   
   CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
   c_wglsetw(fenetreCoupe);
   
   nbChampsActifs = FldMgrGetNbChampsActifs();
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      FldMgrFreeCoupeFlds(champ);
      }
   
   GetFenetreAffichageID(&fenetreAffichage);
   ier = FldMgrLoadTimeAnimationSeq();
   if (ier > 0)
      {
      switch (ier)
	 {
	 default:
	 MessageAvertissement(bombe[lng], AVERTISSEMENT);
	 break;
	 }
      return ier;
      }
   
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      
      if (champ->seqanim.nbFldsAnim > 0)
	 {
	 c_wglsetw(fenetreAffichage);
	 FldMgrPreparerSerie(champ,&rx1,&ry1,&rx2,&ry2);
	 c_wglsetw(fenetreCoupe);
	 }
      }
   
   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();

   return 0;
   
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

PreparerCoupe(cx1,cy1,cx2,cy2)
float cx1,cy1,cx2,cy2;
{
   _Champ *champ;
   int ixmin, ixmax, iymin, iymax;
   float xmin, xmax, ymin, ymax;
   float niveauMin, niveauMax;
   int i,j,ier;
   int fenetreAffichage, fenetreCoupe;
   int nbChampsActifs;
   float rx1,ry1,rx2,ry2;
   int lng;
   
   lng = c_getulng();
   
   ixmin = 0; iymin = 0;
   ixmax = 0; iymax = 0;

   rx1 = cx1; rx2 = cx2; ry1 = cy1; ry2 = cy2;
   
   CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
   c_wglsetw(fenetreCoupe);
   
   nbChampsActifs = FldMgrGetNbChampsActifs();
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      FldMgrFreeAnimFlds(champ);
      }
   
   GetFenetreAffichageID(&fenetreAffichage);
   ier = FldMgrLoadVerticalXSection();
   if (ier > 0)
      {
      switch (ier)
	 {
      	 case PAS_ASSEZ_DE_NIVEAUX:
	 MessageAvertissement(pasAssezDeNiveaux[lng], AVERTISSEMENT);
	 break;
	 
	 case NB_NIVEAUX_INCONSISTANTS:
	 MessageAvertissement(nbNiveauxDifferents[lng], AVERTISSEMENT);
	 break;
	 
	 case NIVEAUX_INCONSISTANTS:
	 MessageAvertissement(niveauxDifferents[lng], AVERTISSEMENT);
	 break;
	 
	 default:
	 MessageAvertissement(bombe[lng], AVERTISSEMENT);
	 break;
	 }
      return ier;
      }
   
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      
      if (champ->coupe.coupeValide)
	 {
	 c_wglsetw(fenetreAffichage);
	 FldMgrPreparerCoupe (champ,&rx1,&ry1,&rx2,&ry2);
	 c_wglsetw(fenetreCoupe);
	 }
      }
   
   if (!VerifierExistenceCoupeValide())
      {
      MessageAvertissement(pasAssezDeNiveaux[lng], AVERTISSEMENT);
      return ier;
      }
   
   if (champ->coupe.coupeValide)   
      {
      c_wglsetw(fenetreAffichage);
      FldMgrGetChamp(&champ, 0);
      FldMgrPreparerTopo();
      c_wglsetw(fenetreCoupe);
      }
   
   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();

   return 0;
   
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


AfficherProfilCoupe(xx, yy)
float xx, yy;
{
   int i,j, fontSize;
   float *x, *y;
   float xmin, ymin, xmax, ymax;
   int npts;
   int axeXRev, axeYRev;
   int nbIntX, nbIntY;
   float intervalleX, intervalleY;
   _Champ *champ, *champ2;
   char titre[40], titrex[32],titrey[32];
   float **valeurs;
   int i1, j1, i2, j2;
   float x1, y1;
   int largeurTitre;
   int n, nbChampsActifs;
   int zero = 0;
   int ix, iy;
   int lng,op,trace;
   int largeurFenetre, hauteurFenetre;

   lng = c_getulng();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   valeurs = (float **) calloc(nbChampsActifs,sizeof(float *));

   FldMgrGetChamp(&champ, 0);
   if (champ->natureTensorielle == VECTEUR)
      {
      return;
      }
   for (n=0; n < nbChampsActifs; n++)
      {
      valeurs[n] = (float *) calloc(champ->coupe.nbNiveauxCoupe, sizeof(float));
      }

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);
   

   c_wglcol(BLANC);
   c_wglclr();

   if (!VerifierExistenceCoupeValide())
      return;

   i = 0;
   FldMgrGetChamp(&champ, i);
   while (champ->coupe.coupeValide == 0)
      {
      i++;
      FldMgrGetChamp(&champ, i);
      }

   npts = champ->coupe.nbNiveauxCoupe;

   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();

   fontSize = 18;
   strcpy(titre, "                                       ");
   

   sprintf(titrex, "$");
   sprintf(titrey, "$");
   f77name(setprof1)(&champ->coupe.niveauMin, &champ->coupe.niveauMax,&echelle,titrex,titrey,
		     0, 0);

   c_wglcol(NOIR);
   c_wgllwi(1);
   c_wglsld(0);
   i = 0;
   FldMgrGetChamp(&champ, i);
   while (champ->coupe.coupeValide == 0)
      {
      i++;
      FldMgrGetChamp(&champ, i);
      }

   f77name(xezxy)(champ->coupe.fld2d, champ->coupe.niveauxCoupe, &zero, titre, 0);

   c_wgllwi(1);
   f77name(setprof2)();
   c_wglgsx(&xmin, &ymin, &xmax, &ymax);
   c_wglcmx(xmin, ymin, xmax, ymax);

   op = CtrlMgrGetMathOp();
   for (n=0; n < nbChampsActifs; n++)
      {
      FldMgrGetChamp(&champ, n);
      if (champ->coupe.coupeValide )
	 {
	 for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
	    {
	    valeurs[n][i] = champ->coupe.fld2d[i];
	    }
	 }
      
      if (0 == n%2)
	 {
	 FldMgrGetChamp(&champ2, n+1);
	 DiffMgrCalcDiffs(valeurs[n], champ->coupe.fld2d,  champ2->coupe.fld2d,  champ->coupe.nbNiveauxCoupe, op);
	 }
      
      if (champ->coupe.coupeValide )
	 {
	 c_wglcol(xc.attributs[n].indCouleurFore);
	 c_wgllwi(xc.attributs[n].epaisseur);
	 c_wglsld(xc.attributs[n].style);
	 
	 trace = (op == NO_OP || 0 == n%2);
	 if (trace)
	    {
	    c_wglxai(&i1, &j1, valeurs[n][0]/champ->facteur, champ->coupe.niveauxCoupe[0]);
	    AfficherSymbole(i1,j1,n);
	    for (i=1; i < champ->coupe.nbNiveauxCoupe; i++)
	       {
	       c_wglmvx(valeurs[n][i-1]/champ->facteur, champ->coupe.niveauxCoupe[i-1]);
	       c_wgldrx(valeurs[n][i]/champ->facteur, champ->coupe.niveauxCoupe[i]);
	       c_wglxai(&i1, &j1, valeurs[n][i]/champ->facteur, champ->coupe.niveauxCoupe[i]);
	       AfficherSymbole(i1,j1,n);
	       }
	    }
	 }
      }

   c_wgllwi(1);
   c_wglsld(0);
   c_wglgsx(&xmin, &ymin, &xmax, &ymax);
   c_wglcmx(xmin, ymin, xmax, ymax);
   
   c_wglcol(NOIR);
   c_wgllwi(1);
   c_wglsld(0);
   
   AfficherLegendeSupProfil(xx, yy, xmin, ymin, xmax, ymax);
   c_wglcol(NOIR);
   c_wgllwi(2);
   c_wglrli(0, 0, largeurFenetre-1, hauteurFenetre-1);

   for (n=0; n < nbChampsActifs; n++)
      {
      free(valeurs[n]);
      }
   free(valeurs);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

AfficherProfilSerie(xx, yy)
float xx, yy;
{
   int i,j, fontSize;
   float *x, *y;
   float xmin, ymin, xmax, ymax;
   int npts;
   int axeXRev, axeYRev;
   int nbIntX, nbIntY;
   float intervalleX, intervalleY;
   _Champ *champ, *champ2;
   char titre[40], titrex[32],titrey[32];
   float **valeurs;
   int i1, j1, i2, j2,mx1,mx2,my1,my2;
   float x1, y1;
   int largeurTitre;
   int n, nbChampsActifs;
   int zero = 0;
   int ix, iy;
   int lng,op,trace;
   int largeurFenetre, hauteurFenetre;

   float valMin,valMax;

   valMin = xx;
   valMax = yy;

   lng = c_getulng();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   valeurs = (float **) calloc(nbChampsActifs,sizeof(float *));

   FldMgrGetChamp(&champ, 0);
   for (n=0; n < nbChampsActifs; n++)
      {
      valeurs[n] = (float *) calloc(champ->seqanim.nbFldsAnim, sizeof(float));
      }

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);
   

   c_wglcol(BLANC);
   c_wglclr();

   if (!VerifierExistenceCoupeValide())
      return;

   i = 0;
   FldMgrGetChamp(&champ, i);

   npts = champ->seqanim.nbFldsAnim;

   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();

   fontSize = 18;
   strcpy(titre, "                                       ");
   

   sprintf(titrex, "$");
   sprintf(titrey, "$");
   f77name(setprof1)(&valMin, &valMax,&echelle,titrex,titrey,
		     0, 0);

   c_wglcol(NOIR);
   c_wgllwi(1);
   c_wglsld(0);

   champ->seqanim.fdt = (float *) malloc(champ->seqanim.nbFldsAnim * sizeof(float));
   for (i=0; i < champ->seqanim.nbFldsAnim; i++)
     {
     champ->seqanim.fdt[i] = (float) champ->seqanim.dt[i];
     }

   for (i=0; i < champ->seqanim.nbFldsAnim; i++)
      {
      FldMgrGetChamp(&champ, n);
      
      mx1 =24; mx2 = 3; my1=10; my2 = 1;
      
      f77name(xezxy)(champ->seqanim.fdt, champ->seqanim.valeursSeries, &zero, titre, 0);

      c_wgllwi(1);
      f77name(setprof2)();
      c_wglgsx(&xmin, &ymin, &xmax, &ymax);
      c_wglcmx(xmin, ymin, xmax, ymax);
      
      op = CtrlMgrGetMathOp();
      for (n=0; n < nbChampsActifs; n++)
	 {
	 FldMgrGetChamp(&champ, n);
	 if (champ->seqanim.nbFldsAnim > 0)
	    {
	    for (i=0; i < champ->seqanim.nbFldsAnim; i++)
	       {
	       valeurs[n][i] = champ->seqanim.valeursSeries[i];
	       }
	    }
	 
	 if (0 == n%2 && op != NO_OP)
	    {
	    FldMgrGetChamp(&champ2, n+1);
	    DiffMgrCalcDiffs(valeurs[n], champ->seqanim.valeursSeries[i],champ2->seqanim.valeursSeries[i], 
			     champ->seqanim.nbFldsAnim, op);
	    }
	 
	 if (champ->seqanim.nbFldsAnim > 0)
	    {
	    c_wglcol(xc.attributs[n].indCouleurFore);
	    c_wgllwi(xc.attributs[n].epaisseur);
	    c_wglsld(xc.attributs[n].style);
	    
	    trace = (op == NO_OP || 0 == n%2);
	    if (trace)
	       {
	       c_wglxai(&i1, &j1,  (float)champ->seqanim.fdt[0], valeurs[n][0]/champ->facteur);
	       AfficherSymbole(i1,j1,n);
	       for (i=1; i < champ->seqanim.nbFldsAnim; i++)
		  {
		  c_wglmvx((float)champ->seqanim.fdt[i-1], valeurs[n][i-1]/champ->facteur);
		  c_wgldrx((float)champ->seqanim.fdt[i], valeurs[n][i]/champ->facteur);
		  c_wglxai(&i1, &j1, (float)champ->seqanim.fdt[i],valeurs[n][i]/champ->facteur);
		  AfficherSymbole(i1,j1,n);
		  }
	       }
	    }
	 }
      }
      

   c_wgllwi(1);
   c_wglsld(0);
   c_wglgsx(&xmin, &ymin, &xmax, &ymax);
   c_wglcmx(xmin, ymin, xmax, ymax);
   
   c_wglcol(NOIR);
   c_wgllwi(1);
   c_wglsld(0);
   
   AfficherLegendeSupProfil(xx, yy, xmin, ymin, xmax, ymax);
   c_wglcol(NOIR);
   c_wgllwi(2);
   c_wglrli(0, 0, largeurFenetre-1, hauteurFenetre-1);

   for (n=0; n < nbChampsActifs; n++)
      {
      free(valeurs[n]);
      }
   free(valeurs);
   free(champ->seqanim.fdt);
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


TracerCercle(x, y)
float x,y;
{
   int ii,jj;
   float rx, ry;

   c_xy2fxfy(&rx, &ry, x, y);
   
   c_wgllwi(3);
   c_wglxai(&ii, &jj, rx, ry);
   c_wglcli(ii, jj, 10);
   c_wglmvi(ii, jj-10);
   c_wgldri(ii, jj+10);
   c_wglmvi(ii-10, jj);
   c_wgldri(ii+10, jj);

   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


Ligne(event,fcx1,fcy1,fcx2,fcy2)
int *event;
float *fcx1, *fcy1, *fcx2, *fcy2;
{
   int bouton;
   int x0, y0, xsize, ysize;
   int nbMenus;
   float temp;
   int i;
   int cx1, cx2, cy1, cy2;
   int fenetreAffichage;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

   nbMenus = 0;
   f77name(minisouris)(&bouton, event, 
           &x0, &y0, &xsize, &ysize, 
           &cx1, &cy1, &cx2, &cy2);

   cx1 = (cx1 - x0); 
   cx2 = (cx2 - x0); 
   cy1 = (cy1 - y0); 
   cy2 = (cy2 - y0); 
   
   switch(*event)
      {
      case DRAG:
      break;
      
      case MOVE:
      case CLIC:
      c_wgliax(fcx1, fcy1, cx1, cy1);
      c_wgliax(fcx2, fcy2, cx2, cy2);
      c_fxfy2xy(fcx1, fcy1, *fcx1, *fcy1);
      c_fxfy2xy(fcx2, fcy2, *fcx2, *fcy2);
      break;
      
      case RESET:
      break;
      
      default:
      break;
      }

   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


TracerLigne(rcx1, rcy1, rcx2, rcy2)
float rcx1,rcy1,rcx2,rcy2;
{
   int i, j, n;
   float theta;
   int rayon = 10;
   int i1, j1, i2, j2;
   int poly[4][2];
   float rx1, ry1, rx2, ry2;

   c_xy2fxfy(&rx1, &ry1, rcx1, rcy1);
   c_xy2fxfy(&rx2, &ry2, rcx2, rcy2);

   c_wgllwi(2);
   c_wglmvx(rx1, ry1);
   c_wgldrx(0.920*(rx2-rx1)+rx1, 0.920*(ry2-ry1)+ry1);

   if (rx2 != rx1)
      {
      theta = atan2(ry2-ry1,rx2-rx1);
      }
   else
      {
      if (0 == (ry2-ry1))
	 {
	 theta = 0.;
	 }
      else
	 {
	 if (0 < (ry2 - ry1))
	    theta = M_PI_2;
	 else
	    theta = -M_PI_2;
	 }
      }
   
   for (n = 1; n <= 7; n++)
      {
      rayon = 20 + 20 * ((n+1) % 2);
      i1 = (int)( 0.5 * rayon  * cos (theta + M_PI_2));
      j1 = (int)( 0.5 * rayon  * sin (theta + M_PI_2));

      c_wglxai(&i,&j,(n * 0.125)*(rx2-rx1)+rx1,(n * 0.125)*(ry2-ry1)+ry1);
      c_wglmvi(i+i1, j+j1);
      c_wgldri(i-i1, j-j1);
      }

   rayon = 30;
   i1 = (int)( 0.5 * rayon  * cos (theta + M_PI_2));
   j1 = (int)( 0.5 * rayon  * sin (theta + M_PI_2));
   
   c_wglxai(&i, &j, (0.875)*(rx2-rx1)+rx1,(0.875)*(ry2-ry1)+ry1);
   poly[0][0] = i+i1; poly[0][1] = j+j1;
   poly[3][0] = i+i1; poly[3][1] = j+j1;
   c_wglxai(&i, &j, (0.920)*(rx2-rx1)+rx1,(0.920)*(ry2-ry1)+ry1);
   poly[1][0] = i; poly[1][1] = j;
   c_wglxai(&i, &j, rx2, ry2);
   poly[2][0] = i; poly[2][1] = j;

   c_wglpfi(4, poly);

   c_wglxai(&i, &j, (0.875)*(rx2-rx1)+rx1,(0.875)*(ry2-ry1)+ry1);
   poly[0][0] = i-i1; poly[0][1] = j-j1;
   poly[3][0] = i-i1; poly[3][1] = j-j1;
   c_wglxai(&i, &j, (0.920)*(rx2-rx1)+rx1,(0.920)*(ry2-ry1)+ry1);
   poly[1][0] = i; poly[1][1] = j;
   c_wglxai(&i, &j, rx2, ry2);
   poly[2][0] = i; poly[2][1] = j;

   c_wglpfi(4, poly);

   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

int CoupeMgrGetFenetreCoupeID(fenetreID)
int *fenetreID;
{
   *fenetreID = fenetreCoupe;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


AfficherLigneCoupe()
{
   _Champ *champ;
   int coupeOK;

   FldMgrGetChamp(&champ, 0);
#ifdef GL_WGL
   EnterOverlayMode();
   color(0);
   clear();
   RestoreNormalMode();
#endif
   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      coupeOK = champ->coupe.nbNiveauxCoupe > 0;
      if (coupeOK)
	 {
	 c_wglcol(CYAN);
	 EnterOverlayMode();
	 switch(champ->coupe.niCoupe)
	    {
	    case 1:
	    TracerCercle(champ->coupe.xmin, champ->coupe.ymin);
	    break;
	    
	    default:
	    TracerLigne(champ->coupe.xmin, champ->coupe.ymin, champ->coupe.xmax, champ->coupe.ymax);
	    break;
	    }
	 RestoreNormalMode();
	 }
      }
   else
      {
      coupeOK = champ->seqanim.nbFldsAnim > 0;
      if (coupeOK)
	 {
	 c_wglcol(CYAN);
	 EnterOverlayMode();
	 switch(champ->seqanim.niSerie)
	    {
	    case 1:
	    TracerCercle(champ->seqanim.xmin, champ->seqanim.ymin);
	    break;
	    
	    default:
	    TracerLigne(champ->seqanim.xmin, champ->seqanim.ymin, champ->seqanim.xmax, champ->seqanim.ymax);
	    break;
	    }
	 RestoreNormalMode();
	 }
      }
   
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/



EnleverLigneCoupe(x1, y1, x2, y2)
float x1, y1, x2, y2;
{
   _Champ *champ;
   int fenetreAffichage;

   if (!VerifierExistenceCoupeValide())
      return;

   if (x1 == 0.0 && y1 == 0.0 && x2 == 0.0 && y2 == 0.0)
      return;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

#ifdef X_WGL
   c_wglcol(CYAN);
   EnterOverlayMode();
   if (x1 == x2 && y1 == y2)
      {
      TracerCercle(x1, y1);
      }
   else
      {
      TracerLigne(x1, y1, x2, y2);
      }
   RestoreNormalMode();
#endif
#ifdef GL_WGL
   EnterOverlayMode();
   color(0);
   clear();
   RestoreNormalMode();
#endif
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

      
int CoupeMgrGetStatutCoupe()
{
   return statutCoupe;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


int CoupeMgrGetCoupeCoords(x1, y1, x2, y2)
float *x1, *y1, *x2, *y2;
{
   *x1 = cx1;
   *x2 = cx2;
   *y1 = cy1;
   *y2 = cy2;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


AfficherSymbole(i,j,n)
int i,j,n;
{
   switch(n)
      {
      case 0:
      c_wglcli(i, j, 4);
      break;
      
      case 1:
      c_wglrli(i-4, j-4, i+4, j+4);
      break;
      
      case 2:
      c_wglmvi(i, j+6);
      c_wgldri(i+6, j);
      c_wgldri(i, j-6);
      c_wgldri(i-6,j);
      c_wgldri(i, j+6);
      break;
      
      case 3:
      c_wglmvi(i, j+6);
      c_wgldri(i+6, j-6);
      c_wgldri(i-6, j-6);
      c_wgldri(i, j+6);
      break;
      }
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

VerifierExistenceCoupeValide()
{
   int i, nbChampsActifs, coupeValideTrouvee,dimensionCoupe;
   _Champ *champ;
   
   nbChampsActifs = FldMgrGetNbChampsActifs();
   coupeValideTrouvee = False;

   dimensionCoupe = CoupeMgrGetDimensionCoupe();

   if (dimensionCoupe == ZP)
      {
      
      i = 0;
      while (i < nbChampsActifs && !coupeValideTrouvee)
	 {
	 FldMgrGetChamp(&champ, i);
	 if (champ->coupe.coupeValide == 1)
	    coupeValideTrouvee = 1;
	 i++;
	 }
      }
   else
      {
      FldMgrGetChamp(&champ, 0);
      if (champ->seqanim.nbFldsAnim > 0)
	 coupeValideTrouvee = 1;
      }
   
   return coupeValideTrouvee;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

EffacerCoupe ()
{
   int i;
   _Champ *champ;
   
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&champ, i);
      if (dimensionCoupe == ZP)
	 {
	 FldMgrFreeCoupeFlds(champ);
	 }
      }

   RedessinerFenetreCoupe();
   statutCoupe = FALSE;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

CoupeMgrSetMinMax()
{
   if (dimensionCoupe == ZP)
      {
      CoupeMgrSetMinMaxCoupe();
      }
   else
      {
      CoupeMgrSetMinMaxSerie();
      }
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


CoupeMgrSetMinMaxSerie()
{   
   _Champ *champ, *champ2;
   int ltype = 1;
   int i,j,nbChampsActifs,op;
   int ixmin, iymin, ixmax, iymax;
   float temp,diff;
   char tempStr[16];
   char *returnedStr;
   Arg args[4];
   int fenetreCoupe;
   float xdebut, xfin;
   float opmin[5],opmax[5];
   int un = 1;
   int npts;
   int n;
   int listeChampsValides[32];
   
   ixmin = 0; ixmax = 0;
   iymin = 0; iymax = 0;
   
   nbChampsActifs = FldMgrGetNbChampsActifs();

   n = 0;
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      if (champ->natureTensorielle == SCALAIRE)
	 {
	 listeChampsValides[n] = i;
	 n++;
	 }
      }

   nbChampsActifs = n;
   if (nbChampsActifs == 0)
      {
      return;
      }

   op = CtrlMgrGetMathOp();
   FldMgrGetChamp(&champ, listeChampsValides[0]);

   if (nbChampsActifs == 1)
      {
      op = NO_OP;
      }

   if (VerifierExistenceCoupeValide())
      {
      switch (calculMinMax)
	 {
	 case AUTO_PROFIL:
	 grafMinX = (float) champ->seqanim.dt[0];
	 grafMaxX = (float) champ->seqanim.dt[champ->seqanim.njSerie-1];
	 npts = champ->seqanim.niSerie*champ->seqanim.njSerie;

	 switch (op)
	    {
	    case NO_OP:
	    FldMgrGetChamp(&champ, listeChampsValides[0]);
	    f77name(aminmax)(&grafMinY,&grafMaxY,champ->seqanim.valeursSeries,&npts,&un);
	    grafMinY /= champ->facteur;
	    grafMaxY /= champ->facteur;
	    
	    for (i=1; i < nbChampsActifs; i++)
	       {
	       FldMgrGetChamp(&champ, listeChampsValides[i]);
	       f77name(aminmax)(&opmin[op],&opmax[op],champ->seqanim.valeursSeries,&npts,&un);
	       opmin[op] /=  champ->facteur;
	       opmax[op] /=  champ->facteur;
	       grafMinX = grafMinX < opmin[op] ? grafMinX : opmin[op];
	       grafMaxX = grafMaxX > opmax[op] ? grafMaxX : opmax[op];
	       }
	    break;
	    
	    default:
	    FldMgrGetChamp(&champ, listeChampsValides[0]);
	    FldMgrGetChamp(&champ2, listeChampsValides[1]);
	    DiffMgrSetDiffs(champ->seqanim.valeursSeries,champ2->seqanim.valeursSeries,opmin,opmax,npts);
	    grafMinX = opmin[op];
	    grafMaxX = opmax[op];

	    i = 2;
	    while (i < nbChampsActifs)
	       {
	       if (i == nbChampsActifs - 1)
		  {
		  FldMgrGetChamp(&champ, listeChampsValides[i]);
		  f77name(aminmax)(&opmin[op],&opmax[op],champ->seqanim.valeursSeries,&npts,&un);
		  }
	       else
		  {
		  FldMgrGetChamp(&champ, listeChampsValides[i]);
		  FldMgrGetChamp(&champ2, listeChampsValides[i+1]);
		  DiffMgrSetDiffs(champ->seqanim.valeursSeries,champ2->seqanim.valeursSeries,opmin,opmax,npts);
		  opmin[op] /=  champ->facteur;
		  opmax[op] /=  champ->facteur;
		  grafMinX = grafMinX < opmin[op] ? grafMinX : opmin[op];
		  grafMaxX = grafMaxX > opmax[op] ? grafMaxX : opmax[op];
		  }
	       i+=2;
	       }
	    break;
	    }
	 break;
	 
	 case AUTO_GRILLES:
	 FldMgrGetChamp(&champ, listeChampsValides[0]);
	 op = CtrlMgrGetMathOp();
	 
	 grafMinY = champ->min/champ->facteur;
	 grafMaxY = champ->max/champ->facteur;
	 grafMinX = (float) champ->seqanim.dt[0];
	 grafMaxX = (float) champ->seqanim.dt[champ->seqanim.njSerie-1];
	 break;
	 
	 case FIXES:
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMinX);
	 sscanf(returnedStr, "%e", &grafMinX);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMaxX);
	 sscanf(returnedStr, "%e", &grafMaxX);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMinY);
	 sscanf(returnedStr, "%e", &grafMinY);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMaxY);
	 sscanf(returnedStr, "%e", &grafMaxY);
	 XtFree(returnedStr);
	 break;
	 }
      
      i = 0;
      XtSetArg(args[i], XmNvalue, tempStr); i++;

      sprintf(tempStr, "%-10.4f", grafMinX);
      XtSetValues(pcpTextMinX, args, i);

      sprintf(tempStr, "%-10.4f", grafMaxX);
      XtSetValues(pcpTextMaxX, args, i);

      sprintf(tempStr, "%-10.4f", grafMinY);
      XtSetValues(pcpTextMinY, args, i);

      sprintf(tempStr, "%-10.4f", grafMaxY);
      XtSetValues(pcpTextMaxY, args, i);
      
      CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
      c_wglsetw(fenetreCoupe);
      
      switch (sensEchelle)
	 {
	 case CROISSANTE:
	 f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafMinX, &grafMaxX,
		       &grafMinY, &grafMaxY, &echelle);
	 break;

	 case DECROISSANTE:
	 f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafMinX, &grafMaxX,
		       &grafMaxY, &grafMinY, &echelle);
	 break;
	 }
      
      }
   
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


CoupeMgrSetMinMaxCoupe()
{   
   _Champ *champ, *champ2;
   int ltype = 1;
   int i,j,nbChampsActifs,op;
   int ixmin, iymin, ixmax, iymax;
   float temp,diff;
   char tempStr[16];
   char *returnedStr;
   Arg args[4];
   int fenetreCoupe;
   float xdebut, xfin;
   float opmin[5],opmax[5];
   int un = 1;
   int npts;
   int n;
   int listeChampsValides[32];
   
   ixmin = 0; ixmax = 0;
   iymin = 0; iymax = 0;
   
   nbChampsActifs = FldMgrGetNbChampsActifs();

   n = 0;
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      if (champ->natureTensorielle == SCALAIRE)
	 {
	 listeChampsValides[n] = i;
	 n++;
	 }
      }

   nbChampsActifs = n;
   if (nbChampsActifs == 0)
      {
      return;
      }

   op = CtrlMgrGetMathOp();
   FldMgrGetChamp(&champ, listeChampsValides[0]);

   if (nbChampsActifs == 1)
      {
      op = NO_OP;
      }

   if (VerifierExistenceCoupeValide())
      {
      switch (calculMinMax)
	 {
	 case AUTO_PROFIL:
	 grafMinY = champ->coupe.niveauMin;
	 grafMaxY = champ->coupe.niveauMax;
	 npts = champ->coupe.niCoupe*champ->coupe.njCoupe;

	 switch (op)
	    {
	    case NO_OP:
	    FldMgrGetChamp(&champ, listeChampsValides[0]);
	    f77name(aminmax)(&grafMinX,&grafMaxX,champ->coupe.fld2d,&npts,&un);
	    grafMinX /= champ->facteur;
	    grafMaxX /= champ->facteur;
	    
	    if (champ->coupe.niCoupe == 1)
	       {
	       for (i=1; i < nbChampsActifs; i++)
		  {
		  FldMgrGetChamp(&champ, listeChampsValides[i]);
		  f77name(aminmax)(&opmin[op],&opmax[op],champ->coupe.fld2d,&npts,&un);
		  opmin[op] /=  champ->facteur;
		  opmax[op] /=  champ->facteur;
		  grafMinX = grafMinX < opmin[op] ? grafMinX : opmin[op];
		  grafMaxX = grafMaxX > opmax[op] ? grafMaxX : opmax[op];
		  }
	       }
	    break;

	    default:
	    FldMgrGetChamp(&champ, listeChampsValides[0]);
	    FldMgrGetChamp(&champ2, listeChampsValides[1]);
	    DiffMgrSetDiffs(champ->coupe.fld2d,champ2->coupe.fld2d,opmin,opmax,npts);
	    grafMinX = opmin[op];
	    grafMaxX = opmax[op];

	    i = 2;
	    while (i < nbChampsActifs)
	       {
	       if (i == nbChampsActifs - 1)
		  {
		  FldMgrGetChamp(&champ, listeChampsValides[i]);
		  f77name(aminmax)(&opmin[op],&opmax[op],champ->coupe.fld2d,&npts,&un);
		  }
	       else
		  {
		  FldMgrGetChamp(&champ, listeChampsValides[i]);
		  FldMgrGetChamp(&champ2, listeChampsValides[i+1]);
		  DiffMgrSetDiffs(champ->coupe.fld2d,champ2->coupe.fld2d,opmin,opmax,npts);
		  opmin[op] /=  champ->facteur;
		  opmax[op] /=  champ->facteur;
		  grafMinX = grafMinX < opmin[op] ? grafMinX : opmin[op];
		  grafMaxX = grafMaxX > opmax[op] ? grafMaxX : opmax[op];
		  }
	       i+=2;
	       }
	    break;
	    }
	 break;
	 
	 case AUTO_GRILLES:
	 FldMgrGetChamp(&champ, listeChampsValides[0]);
	 op = CtrlMgrGetMathOp();
	 
	 grafMinY = champ->coupe.niveauMin;
	 grafMaxY = champ->coupe.niveauMax;
	 grafMinX = champ->coupe.FLDmin3d[op]/champ->facteur;
	 grafMaxX = champ->coupe.FLDmax3d[op]/champ->facteur;
	 
	 if (champ->coupe.niCoupe == 1)
	    {
	    for (i=1; i < nbChampsActifs; i++)
	       {
	       FldMgrGetChamp(&champ, listeChampsValides[i]);
	       if (champ->coupe.coupeValide)
		  {
		  if (grafMinX > champ->coupe.FLDmin3d[op]/champ->facteur)
		     grafMinX = champ->coupe.FLDmin3d[op]/champ->facteur;
		  
		  if (grafMaxX < champ->coupe.FLDmax3d[op]/champ->facteur)
		     grafMaxX = champ->coupe.FLDmax3d[op]/champ->facteur;
		  }
	       }
	    }
	 break;
	 
	 case FIXES:
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMinX);
	 sscanf(returnedStr, "%e", &grafMinX);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMaxX);
	 sscanf(returnedStr, "%e", &grafMaxX);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMinY);
	 sscanf(returnedStr, "%e", &grafMinY);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcpTextMaxY);
	 sscanf(returnedStr, "%e", &grafMaxY);
	 XtFree(returnedStr);
	 }
      
      i = 0;
      XtSetArg(args[i], XmNvalue, tempStr); i++;

      sprintf(tempStr, "%-10.4f", grafMinX);
      XtSetValues(pcpTextMinX, args, i);

      sprintf(tempStr, "%-10.4f", grafMaxX);
      XtSetValues(pcpTextMaxX, args, i);

      sprintf(tempStr, "%-10.4f", grafMinY);
      XtSetValues(pcpTextMinY, args, i);

      sprintf(tempStr, "%-10.4f", grafMaxY);
      XtSetValues(pcpTextMaxY, args, i);
      
      CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
      c_wglsetw(fenetreCoupe);
      
      switch (sensEchelle)
	 {
	 case CROISSANTE:
	 f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafMinX, &grafMaxX,
		       &grafMinY, &grafMaxY, &echelle);
	 break;

	 case DECROISSANTE:
	 f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafMinX, &grafMaxX,
		       &grafMaxY, &grafMinY, &echelle);
	 break;
	 }
      
      }
   
   }

CoupeMgrGetLimites(valmin,valmax,nivmin,nivmax)
float *valmin, *valmax, *nivmin, *nivmax;
{
   
   *valmin = grafMinX;
   *valmax = grafMaxX;
   *nivmin = grafMinY;
   *nivmax = grafMaxY;
   }

CoupeMgrGetSensEchelle()
{
   return sensEchelle;
   }

CoupeMgrGetToggleTopo()
{
   return xc.statuts[TOPOGRAPHIE];
   
   }


InitFenetreCoupe()
{
   int lng;
   static char *labelFenetre[] = {"Profils/Coupes", "Profiles/XSections"};

   lng = c_getulng();

   if (!fenetreCoupe)
      {
      fenetreCoupe = c_wglopw(labelFenetre[lng]);
      c_wglias(1);
      }
   }


CoupeMgrSetUVWMinMax()
{   
   _Champ *champ, *champ2;
   static int ltype = 1;
   int i,j,k,nbChampsActifs,op;
   int ixmin, iymin, ixmax, iymax;
   float temp,diff;
   char tempStr[16];
   char *returnedStr;
   Arg args[4];
   int fenetreCoupe;
   float xdebut, xfin;
   int npts, nptsCoupe,un;   
   float *uvwtang,*uvwnorm,*uu,*vv,*ww,*uv;
   float  uvwtmin[5],uvwtmax[5],uvwnmin[5],uvwnmax[5],wwmin[5],wwmax[5],uvmin[5],uvmax[5];
   float minuutang,minuunorm,minww,maxuutang,maxuunorm,maxww,minuvw,maxuvw;

   un = 1;
   ixmin = 0; ixmax = 0;
   iymin = 0; iymax = 0;
   

   i = 0;
   nbChampsActifs = FldMgrGetNbChampsActifs();

   grafMinUUtang =  1.0e+30;
   grafMaxUUtang = -1.0e+30;
   grafMinUUnorm =  1.0e+30;
   grafMaxUUnorm = -1.0e+30;
   grafMinUU =  1.0e+30;
   grafMaxUU = -1.0e+30;
   grafMinVV =  1.0e+30;
   grafMaxVV = -1.0e+30;
   grafMinWW =  1.0e+30;
   grafMaxWW = -1.0e+30;
   grafMinUVW =  1.0e+30;
   grafMaxUVW = -1.0e+30;
	    
   op = CtrlMgrGetMathOp();
   if (nbChampsActifs < 2)
      op = NO_OP;
   
   for (k=0; k < nbChampsActifs; k++)
      {
      FldMgrGetChamp(&champ, k);
      if (champ->natureTensorielle > SCALAIRE && champ->coupe.coupeValide == 1)
	 {
	 npts = champ->coupe.niCoupe*champ->coupe.njCoupe;
	 uvwtang = champ->coupe.uvwtang2d;
	 uvwnorm = champ->coupe.uvwnorm2d;
	 uu     = champ->coupe.uu2d;
	 vv     = champ->coupe.vv2d;
	 ww     = champ->coupe.ww2d;
	 uv     = champ->coupe.uvw2d;

	 switch (calculMinMax)
	    {
	    case AUTO_PROFIL:
	    grafMinY = champ->coupe.niveauMin;
	    grafMaxY = champ->coupe.niveauMax;
	    
	    switch(op)
	       {
	       case NO_OP:
	       f77name(aminmax)(&(champ->coupe.UVWTANGmin2d[NO_OP]),
				&(champ->coupe.UVWTANGmax2d[NO_OP]),
				uvwtang,&npts,&un);
	       f77name(aminmax)(&(champ->coupe.UVWNORMmin2d[NO_OP]),
				&(champ->coupe.UVWNORMmax2d[NO_OP]),
				uvwnorm,&npts,&un);
	       f77name(aminmax)(&(champ->coupe.UUmin2d[NO_OP]),
				&(champ->coupe.UUmax2d[NO_OP]),
				uu,&npts,&un);
	       f77name(aminmax)(&(champ->coupe.VVmin2d[NO_OP]),
				&(champ->coupe.VVmax2d[NO_OP]),
				vv,&npts,&un);
	       f77name(aminmax)(&(champ->coupe.WWmin2d[NO_OP]),
				&(champ->coupe.WWmax2d[NO_OP]),
				ww,&npts,&un);
	       f77name(aminmax)(&(champ->coupe.UVWmin2d[NO_OP]),
				&(champ->coupe.UVWmax2d[NO_OP]),
				uv,&npts,&un);
	       break;
	       
	       default:
	       if (0 == k%2)
		  {
		  FldMgrGetChamp(&champ2, k+1);
		  DiffMgrSetDiffs(champ->coupe.uvwtang2d,champ2->coupe.uvwtang2d,
				  champ->coupe.UVWTANGmin2d,champ->coupe.UVWTANGmax2d,npts);
		  DiffMgrSetDiffs(champ->coupe.uvwnorm2d,champ2->coupe.uvwnorm2d,
				  champ->coupe.UVWNORMmin2d,champ->coupe.UVWNORMmax2d,npts);
		  DiffMgrSetDiffs(champ->coupe.uu2d,champ2->coupe.uu2d,
				  champ->coupe.UUmin2d,champ->coupe.UUmax2d,npts);
		  DiffMgrSetDiffs(champ->coupe.vv2d,champ2->coupe.vv2d,
				  champ->coupe.VVmin2d,champ->coupe.VVmax2d,npts);
		  DiffMgrSetDiffs(champ->coupe.ww2d,champ2->coupe.ww2d,
				  champ->coupe.WWmin2d,champ->coupe.WWmax2d,npts);
		  DiffMgrSetDiffs(champ->coupe.uvw2d,champ2->coupe.uvw2d,
				  champ->coupe.UVWmin2d,champ->coupe.UVWmax2d,npts);
		  }
	       break;
	       }

	    grafMinUUtang = grafMinUUtang < champ->coupe.UVWTANGmin2d[op] ? grafMinUUtang : champ->coupe.UVWTANGmin2d[op];
	    grafMaxUUtang = grafMaxUUtang > champ->coupe.UVWTANGmax2d[op] ? grafMaxUUtang : champ->coupe.UVWTANGmax2d[op];
	    grafMinUUnorm = grafMinUUnorm < champ->coupe.UVWNORMmin2d[op] ? grafMinUUnorm : champ->coupe.UVWNORMmin2d[op];
	    grafMaxUUnorm = grafMaxUUnorm > champ->coupe.UVWNORMmin2d[op] ? grafMaxUUnorm : champ->coupe.UVWNORMmin2d[op];
	    grafMinUU = grafMinUU <  champ->coupe.UUmin2d[op]    ? grafMinUU :  champ->coupe.UUmin2d[op];
	    grafMaxUU = grafMaxUU >  champ->coupe.UUmax2d[op]    ? grafMaxUU :  champ->coupe.UUmax2d[op];
	    grafMinVV = grafMinVV <  champ->coupe.VVmin2d[op]    ? grafMinVV :  champ->coupe.VVmin2d[op];
	    grafMaxVV = grafMaxVV >  champ->coupe.VVmax2d[op]    ? grafMaxVV :  champ->coupe.VVmax2d[op];
	    grafMinWW = grafMinWW <  champ->coupe.WWmin2d[op]    ? grafMinWW :  champ->coupe.WWmin2d[op];
	    grafMaxWW = grafMaxWW >  champ->coupe.WWmax2d[op]    ? grafMaxWW :  champ->coupe.WWmax2d[op];
	    grafMinUVW = grafMinUVW < champ->coupe.UVWmin2d[op]  ? grafMinUVW : champ->coupe.UVWmin2d[op];
	    grafMaxUVW = grafMaxUVW > champ->coupe.UVWmax2d[op]  ? grafMaxUVW : champ->coupe.UVWmax2d[op];
	    break;
	    
	    case AUTO_GRILLES:
	    grafMinUUtang = grafMinUUtang < champ->coupe.UVWmin3d[op] ? grafMinUUtang : champ->coupe.UVWmin3d[op];
	    grafMaxUUtang = grafMaxUUtang > champ->coupe.UVWmax3d[op] ? grafMaxUUtang : champ->coupe.UVWmax3d[op];
	    grafMinUU = grafMinUU <  champ->coupe.UUmin3d[op]    ? grafMinUU :  champ->coupe.UUmin3d[op];
	    grafMaxUU = grafMaxUU >  champ->coupe.UUmax3d[op]    ? grafMaxUU :  champ->coupe.UUmax3d[op];
	    grafMinVV = grafMinVV <  champ->coupe.VVmin3d[op]    ? grafMinVV :  champ->coupe.VVmin3d[op];
	    grafMaxVV = grafMaxVV >  champ->coupe.VVmax3d[op]    ? grafMaxVV :  champ->coupe.VVmax3d[op];
	    grafMinWW = grafMinWW <  champ->coupe.WWmin3d[op]    ? grafMinWW :  champ->coupe.WWmin3d[op];
	    grafMaxWW = grafMaxWW >  champ->coupe.WWmax3d[op]    ? grafMaxWW :  champ->coupe.WWmax3d[op];
	    grafMinUVW = grafMinUVW < champ->coupe.UVWmin3d[op]  ? grafMinUVW : champ->coupe.UVWmin3d[op];
	    grafMaxUVW = grafMaxUVW > champ->coupe.UVWmax3d[op]  ? grafMaxUVW : champ->coupe.UVWmax3d[op];
	    break;
	    
	    case FIXES:
	    returnedStr = (char *) XmTextFieldGetString(pcpTextMinX);
	    sscanf(returnedStr, "%e", &grafMinUVW);
	    XtFree(returnedStr);
	    
	    returnedStr = (char *) XmTextFieldGetString(pcpTextMaxX);
	    sscanf(returnedStr, "%e", &grafMaxUVW);
	    XtFree(returnedStr);
	    
	    returnedStr = (char *) XmTextFieldGetString(pcpTextMinY);
	    sscanf(returnedStr, "%e", &grafMinY);
	    XtFree(returnedStr);
	    
	    returnedStr = (char *) XmTextFieldGetString(pcpTextMaxY);
	    sscanf(returnedStr, "%e", &grafMaxY);
	    XtFree(returnedStr);
	    break;
	    }
	 
	 i = 0;
	 XtSetArg(args[i], XmNvalue, tempStr); i++;
	 
	 sprintf(tempStr, "%-10.4f", grafMinUVW);
	 XtSetValues(pcpTextMinX, args, i);
	 
	 sprintf(tempStr, "%-10.4f", grafMaxUVW);
	 XtSetValues(pcpTextMaxX, args, i);
	 
	 sprintf(tempStr, "%-10.4f", grafMinY);
	 XtSetValues(pcpTextMinY, args, i);
	 
	 sprintf(tempStr, "%-10.4f", grafMaxY);
	 XtSetValues(pcpTextMaxY, args, i);
	 
	 CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
	 c_wglsetw(fenetreCoupe);
	 
	 switch (sensEchelle)
	    {
	    case CROISSANTE:
	    f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafMinUVW, &grafMaxUVW,
			  &grafMinY, &grafMaxY, &echelle);
	    break;
	    
	    case DECROISSANTE:
	    f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafMinUVW, &grafMaxUVW,
			  &grafMaxY, &grafMinY, &echelle);
	    break;
	    }
	 
	 }
      
      }
   }


CoupeMgrGetLimitesUVW(uutanmin,uutanmax,uvwmin,uvwmax,uumin,uumax,vvmin,vvmax,wwmin,wwmax,nivmin,nivmax)
float *uutanmin,*uutanmax,*uvwmin,*uvwmax,*uumin,*uumax,*vvmin,*vvmax,*wwmin,*wwmax,*nivmin,*nivmax;
{
   *uutanmin  = grafMinUUtang;
   *uutanmax  = grafMaxUUtang;

   *wwmin  = grafMinWW;
   *wwmax  = grafMaxWW;

   *uvwmin = grafMinUVW;
   *uvwmax = grafMaxUVW;

   *uumin  = grafMinUU;
   *uumax  = grafMaxUU;

   *vvmin  = grafMinVV;
   *vvmax  = grafMaxVV;

   *nivmin = grafMinY;
   *nivmax = grafMaxY;
   }

