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

int echelleSerie = LINEAIRE;
int calculMinMaxSerie = AUTO_PROFIL;
float grafTMinX, grafTMinY, grafTMaxX, grafTMaxY;
float grafTMinUUtang,grafTMaxUUtang,grafTMinUUnorm,grafTMaxUUnorm,grafTMinUVW,grafTMaxUVW;
float grafTMinUU,grafTMaxUU,grafTMinVV,grafTMaxVV,grafTMinWW,grafTMaxWW;

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern _Viewport    viewp;

XtCallbackProc PcpNouvelleSerie();
XtCallbackProc PcpScanProfil();
XtCallbackProc PcpScanSerie();
XtCallbackProc PcpOk();

XtCallbackProc PcpAfficher();

XtCallbackProc PcpSetEchelleSerieLineaire();
XtCallbackProc PcpSetEchelleSerieLog();

XtCallbackProc PcpSetMinMaxProfil();
XtCallbackProc PcpSetMinMaxGrilles();
XtCallbackProc PcpSetMinMaxUsager();

extern Widget pcpForme, pcpFrame, pcpForme2, pcpFrame3, pcpForme3, pcpRC, pcpAfficher, pcpOk;
extern Widget pcpFormeBoutons, pcpFormeEchelleSerie, pcpFormeLimite;
extern Widget pcpFrameBoutons, pcpFrameEchelleSerie, pcpFrameLimite;
extern Widget pcpLabelEchelleSerie, pcpFrameLineaire, pcpFrameSens, pcpLineariteEchelleSerie, pcpSensEchelleSerie;
extern Widget pcpFrameTopo,pcpPresenceTopo,pcpTopo;
extern Widget pcpLineaire, pcpLog, pcpCroissante, pcpDecroissante;
extern Widget pcpLabelLimite;
extern Widget pcpFrameTypeLimite, pcpFormeTypeLimite, pcpLimiteSerie,pcpLimiteDomaine,pcpLimiteUsager;
extern Widget pcpFrameLimiteX, pcpFormeLimiteX, pcpLabelX, pcpLabelMinX, pcpLabelMaxX, pcpTextMinX,pcpTextMaxX;
extern Widget pcpFrameLimiteY, pcpFormeLimiteY, pcpLabelY, pcpLabelMinY, pcpLabelMaxY, pcpTextMinY,pcpTextMaxY;
extern Widget pcsFrameTypeLimite, pcsFormeTypeLimite, pcsLimiteSerie,pcsLimiteDomaine,pcsLimiteUsager;
extern Widget pcsFrameLimiteX, pcsFormeLimiteX, pcsLabelX, pcsLabelMinX, pcsLabelMaxX, pcsTextMinX,pcsTextMaxX;
extern Widget pcsFrameLimiteY, pcsFormeLimiteY, pcsLabelY, pcsLabelMinY, pcsLabelMaxY, pcsTextMinY,pcsTextMaxY;


extern int fenetreSerie;
float cxt1, cyt1, cxt2, cyt2;
float lastcxt1, lastcyt1, lastcxt2, lastcyt2;
extern int statutSerie;

static char *bombe[] = {"Une erreur systeme est survenue.\nCette application pourrait inopinement quitter", "A system error occured.\nThis program could unexpectedly quit"};


/**
 ******************************************************************************
 ******************************************************************************
 **/

PreparerSerie(cxt1,cyt1,cxt2,cyt2)
float cxt1,cyt1,cxt2,cyt2;
{
   _Champ *champ;
   int ixmin, ixmax, iymin, iymax;
   float xmin, xmax, ymin, ymax;
   float niveauMin, niveauMax;
   int i,j,ier;
   int fenetreAffichage, fenetreSerie;
   int nbChampsActifs;
   float rx1,ry1,rx2,ry2;
   int lng;
   
   lng = c_getulng();
   
   ixmin = 0; iymin = 0;
   ixmax = 0; iymax = 0;

   rx1 = cxt1; rx2 = cxt2; ry1 = cyt1; ry2 = cyt2;
   
   nbChampsActifs = FldMgrGetNbChampsActifs();
   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champ, i);
      FldMgrFreeCoupeFlds(champ);
      }
   
   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   SerieMgrGetFenetreSerieID(&fenetreSerie);
   c_wglsetw(fenetreSerie);
   
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
	 c_wglsetw(fenetreSerie);
	 }
      }
   
   SerieMgrSetMinMax();
   SerieMgrSetUVWMinMax();

   return 0;
   
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
   _Champ *champ, *champ2, *champ0;
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

   FldMgrGetChamp(&champ0, 0);
   FldMgrGetChamp(&champ, 0);
   for (n=0; n < nbChampsActifs; n++)
      {
      valeurs[n] = (float *) calloc(champ->seqanim.nbFldsAnim, sizeof(float));
      }

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);
   

   c_wglcol(BLANC);
   c_wglclr();

   if (!VerifierExistenceSerieValide())
      return;

   i = 0;
   FldMgrGetChamp(&champ, i);

   npts = champ->seqanim.nbFldsAnim;

   SerieMgrSetMinMax();
   SerieMgrSetUVWMinMax();

   fontSize = 18;
   strcpy(titre, "                                       ");
   

   sprintf(titrex, "$");
   sprintf(titrey, "$");
   f77name(setprof1)(&valMin, &valMax,&echelleSerie,titrex,titrey,
		     0, 0);

   c_wglcol(NOIR);
   c_wgllwi(1);
   c_wglsld(0);

   champ0->seqanim.fdt = (float *) malloc(champ0->seqanim.nbFldsAnim * sizeof(float));
   for (i=0; i < champ0->seqanim.nbFldsAnim; i++)
     {
     champ0->seqanim.fdt[i] = (float) champ0->seqanim.dt[i];
     }

   mx1 =24; mx2 = 3; my1=10; my2 = 1;
      
   f77name(xezxy)(champ0->seqanim.fdt, champ->seqanim.valeursSeries, &zero, titre, 0);
   
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
	 c_wglxai(&i1, &j1,  (float)champ0->seqanim.fdt[0], valeurs[n][0]/champ->facteur);
	 AfficherSymbole(i1,j1,n);
	 for (i=1; i < champ->seqanim.nbFldsAnim; i++)
	   {
	   c_wglmvx((float)champ0->seqanim.fdt[i-1], valeurs[n][i-1]/champ->facteur);
	   c_wgldrx((float)champ0->seqanim.fdt[i], valeurs[n][i]/champ->facteur);
	   c_wglxai(&i1, &j1, (float)champ0->seqanim.fdt[i],valeurs[n][i]/champ->facteur);
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
   free(champ0->seqanim.fdt);
   }

/*
 ******************************************************************************
 ******************************************************************************
 **/

int SerieMgrGetFenetreSerieID(fenetreID)
int *fenetreID;
{
   *fenetreID = fenetreSerie;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


AfficherLigneSerie()
{
   _Champ *champ;
   int seqanimOK;

   FldMgrGetChamp(&champ, 0);
   seqanimOK = champ->seqanim.nbFldsAnim > 0;
   if (seqanimOK)
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

/**
 ******************************************************************************
 ******************************************************************************
 **/



EnleverLigneSerie(x1, y1, x2, y2)
float x1, y1, x2, y2;
{
   _Champ *champ;
   int fenetreAffichage;

   if (!VerifierExistenceSerieValide())
      return;

   if (x1 == 0.0 && y1 == 0.0 && x2 == 0.0 && y2 == 0.0)
      return;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);

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
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/

      
int SerieMgrGetStatutSerie()
{
   return statutSerie;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


int SerieMgrGetSerieCoords(x1, y1, x2, y2)
float *x1, *y1, *x2, *y2;
{
   *x1 = cxt1;
   *x2 = cxt2;
   *y1 = cyt1;
   *y2 = cyt2;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


VerifierExistenceSerieValide()
{
   int i, nbChampsActifs, seqanimValideTrouvee;
   _Champ *champ;
   
   nbChampsActifs = FldMgrGetNbChampsActifs();
   seqanimValideTrouvee = False;
   
   FldMgrGetChamp(&champ, 0);
   if (champ->seqanim.nbFldsAnim > 0)
     seqanimValideTrouvee = 1;
   
   return seqanimValideTrouvee;
}


/**
 ******************************************************************************
 ******************************************************************************
 **/

EffacerSerie ()
{
   int i;
   _Champ *champ;
   
   statutSerie = FALSE;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

SerieMgrSetMinMaxSerie()
{   
   _Champ *champ, *champ2;
   int ltype = 1;
   int i,j,nbChampsActifs,op;
   int ixmin, iymin, ixmax, iymax;
   float temp,diff;
   char tempStr[16];
   char *returnedStr;
   Arg args[4];
   int fenetreSerie;
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

   if (VerifierExistenceSerieValide())
      {
      switch (calculMinMaxSerie)
	 {
	 case AUTO_PROFIL:
	 grafTMinX = (float) champ->seqanim.dt[0];
	 grafTMaxX = (float) champ->seqanim.dt[champ->seqanim.njSerie-1];
	 npts = champ->seqanim.niSerie*champ->seqanim.njSerie;

	 switch (op)
	    {
	    case NO_OP:
	    FldMgrGetChamp(&champ, listeChampsValides[0]);
	    f77name(aminmax)(&grafTMinY,&grafTMaxY,champ->seqanim.valeursSeries,&npts,&un);
	    grafTMinY /= champ->facteur;
	    grafTMaxY /= champ->facteur;
	    
	    for (i=1; i < nbChampsActifs; i++)
	       {
	       FldMgrGetChamp(&champ, listeChampsValides[i]);
	       f77name(aminmax)(&opmin[op],&opmax[op],champ->seqanim.valeursSeries,&npts,&un);
	       opmin[op] /=  champ->facteur;
	       opmax[op] /=  champ->facteur;
	       grafTMinY = grafTMinY < opmin[op] ? grafTMinY : opmin[op];
	       grafTMaxY = grafTMaxY > opmax[op] ? grafTMaxY : opmax[op];
	       }
	    break;
	    
	    default:
	    FldMgrGetChamp(&champ, listeChampsValides[0]);
	    FldMgrGetChamp(&champ2, listeChampsValides[1]);
	    DiffMgrSetDiffs(champ->seqanim.valeursSeries,champ2->seqanim.valeursSeries,opmin,opmax,npts);
	    grafTMinY = opmin[op];
	    grafTMaxY = opmax[op];

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
		  grafTMinY = grafTMinY < opmin[op] ? grafTMinY : opmin[op];
		  grafTMaxY = grafTMaxY > opmax[op] ? grafTMaxY : opmax[op];
		  }
	       i+=2;
	       }
	    break;
	    }
	 break;
	 
	 case AUTO_GRILLES:
	 FldMgrGetChamp(&champ, listeChampsValides[0]);
	 op = CtrlMgrGetMathOp();
	 
	 grafTMinY = champ->min/champ->facteur;
	 grafTMaxY = champ->max/champ->facteur;
	 grafTMinX = (float) champ->seqanim.dt[0];
	 grafTMaxX = (float) champ->seqanim.dt[champ->seqanim.njSerie-1];
	 break;
	 
	 case FIXES:
	 returnedStr = (char *) XmTextFieldGetString(pcsTextMinX);
	 sscanf(returnedStr, "%e", &grafTMinX);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcsTextMaxX);
	 sscanf(returnedStr, "%e", &grafTMaxX);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcsTextMinY);
	 sscanf(returnedStr, "%e", &grafTMinY);
	 XtFree(returnedStr);
	 
	 returnedStr = (char *) XmTextFieldGetString(pcsTextMaxY);
	 sscanf(returnedStr, "%e", &grafTMaxY);
	 XtFree(returnedStr);
	 break;
	 }
      }
   
   i = 0;
   XtSetArg(args[i], XmNvalue, tempStr); i++;
   
   sprintf(tempStr, "%-10.4f", grafTMinX);
   XtSetValues(pcsTextMinX, args, i);
   
   sprintf(tempStr, "%-10.4f", grafTMaxX);
   XtSetValues(pcsTextMaxX, args, i);
   
   sprintf(tempStr, "%-10.4f", grafTMinY);
   XtSetValues(pcsTextMinY, args, i);
   
   sprintf(tempStr, "%-10.4f", grafTMaxY);
   XtSetValues(pcsTextMaxY, args, i);
   
   SerieMgrGetFenetreSerieID(&fenetreSerie);
   c_wglsetw(fenetreSerie);
   
   f77name(xset)(&ixmin , &ixmax, &iymin, &iymax, &grafTMinX, &grafTMaxX,
		 &grafTMinY, &grafTMaxY, &echelleSerie);
   
}

/**
******************************************************************************
******************************************************************************
**/


SerieMgrGetLimites(valmin,valmax,nivmin,nivmax)
     float *valmin, *valmax, *nivmin, *nivmax;
{
  
  *valmin = grafTMinX;
  *valmax = grafTMaxX;
  *nivmin = grafTMinY;
  *nivmax = grafTMaxY;
}

InitFenetreSerie()
{
   int lng;
   static char *labelFenetre[] = {"Profils/Series", "Profiles/XSections"};

   lng = c_getulng();

   if (!fenetreSerie)
      {
      fenetreSerie = c_wglopw(labelFenetre[lng]);
      c_wglias(1);
      }
   }


SerieMgrGetLimitesUVW(uutanmin,uutanmax,uvwmin,uvwmax,uumin,uumax,vvmin,vvmax,wwmin,wwmax,nivmin,nivmax)
float *uutanmin,*uutanmax,*uvwmin,*uvwmax,*uumin,*uumax,*vvmin,*vvmax,*wwmin,*wwmax,*nivmin,*nivmax;
{

   }

