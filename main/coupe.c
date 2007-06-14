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

#include <rpnmacros.h>
#include <xinit.h>
#include <wgl.h>
#include <souris.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <math.h>

#define AUTO_PROFIL 0
#define AUTO_GRILLES 1
#define AUTO_LEVELS 1
#define FIXES       2

#define TOPO_INACTIVE 0
#define TOPO_ACTIVE   1

int dimensionCoupe = ZP;
int echelle = LINEAIRE;
int calculMinMax_X = AUTO_PROFIL;
int calculMinMax_Y = AUTO_LEVELS;
int sensEchelle = DECROISSANTE;
int toggleTopo = TOPO_INACTIVE;
float grafMinX, grafMinY, grafMaxX, grafMaxY;
float grafMinUUtang,grafMaxUUtang,grafMinUUnorm,grafMaxUUnorm,grafMinUVW,grafMaxUVW;
float grafMinUU,grafMaxUU,grafMinVV,grafMaxVV,grafMinWW,grafMaxWW;

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern _Viewport    viewp;

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


float cp_hyb(float psurf, float hyb, float ptop, float pref, float rcoef)
  {
  float term_a, term_b;
  
  term_b = powf(((hyb - ptop/pref) / (1.0 - (ptop/pref))), rcoef);
  term_a = pref*(hyb-term_b);
  return (term_a +  term_b*psurf);
  }

/**
 ******************************************************************************
 ******************************************************************************
 **/

int PreparerCoupe(float cx1, float cy1, float cx2, float cy2)
{
   _Champ *champ;
   int ixmin, ixmax, iymin, iymax;
   int i,ier;
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


void AfficherProfilCoupe(float xx, float yy)
{
   int i,fontSize;
   float xxx, yyy;
   float xmin, ymin, xmax, ymax;
   int npts;
   _Champ *champ, *champ2;
   char titre[40], titrex[32],titrey[32];
   float **valeurs;
   int i1, j1;
   float p0,ptop,p;
   int n, nbChampsActifs;
   int zero = 0;
   int selectedVertCoord;
   int lng,op,trace;
   int largeurFenetre, hauteurFenetre;
   int un = 1;

   lng = c_getulng();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   valeurs = (float **) malloc(nbChampsActifs*sizeof(float *));
   selectedVertCoord = GetSelectedVertCoord();

   xxx = xx;
   yyy = yy;
   FldMgrGetChamp(&champ, 0);
   if (champ->natureTensorielle == VECTEUR)
      {
      return;
      }
   
   for (n=0; n < nbChampsActifs; n++)
      {
      FldMgrGetChamp(&champ, n);
      valeurs[n] = (float *) malloc(champ->coupe.nbNiveauxCoupe*sizeof(float));
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
   if ((champ->coordonneeVerticale == SIGMA || champ->coordonneeVerticale == HYBRIDE) && selectedVertCoord == PRES_VCOORD)
    {
    f77name(setprof1)(&champ->coupe.niveauPresMin, &champ->coupe.niveauPresMax,&echelle,titrex,titrey, 0, 0);
    }
   else
    {
    f77name(setprof1)(&champ->coupe.niveauMin, &champ->coupe.niveauMax,&echelle,titrex,titrey, 0, 0);
    }

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

/*   if ((champ->coordonneeVerticale == SIGMA || champ->coordonneeVerticale == HYBRIDE) && selectedVertCoord == PRES_VCOORD)
    {
     niveauxCoupePres = 
     f77name(xezxy)(champ->coupe.fld2d, champ->coupe.niveauxCoupePres, &zero, titre, 0);
    }
   else
    {*/
     f77name(xezxy)(champ->coupe.fld2d, champ->coupe.niveauxCoupe, &zero, titre, 0);
/*     } */

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
          if ((champ->coordonneeVerticale == SIGMA || champ->coordonneeVerticale == HYBRIDE) && selectedVertCoord == PRES_VCOORD)
            {
            ptop=champ->coupe.ptop;
/*            f77name(ez_rgdint_1_nw)(&p0, champ->coupe.x, champ->coupe.y, &champ->coupe.niCoupe, champ->coupe.montagnes, &champ->dst.ni, &un, &champ->dst.nj);*/
            f77name(ez_rgdint_1_nw)(&p0, &xxx, &yyy, &champ->coupe.niCoupe, champ->coupe.montagnes, &champ->dst.ni, &un, &champ->dst.nj);
            c_wglxai(&i1, &j1, valeurs[n][0]/champ->facteur, p0);
            AfficherSymbole(i1,j1,n);
            for (i=1; i < champ->coupe.nbNiveauxCoupe; i++)
              {
              p = cp_hyb(p0, champ->coupe.niveauxCoupe[i-1], champ->coupe.ptop, champ->coupe.pref, champ->coupe.rcoef);
/*              if (p < champ->coupe.ptop) p = ptop;*/
              c_wglmvx(valeurs[n][i-1]/champ->facteur, p);
              p = cp_hyb(p0, champ->coupe.niveauxCoupe[i], champ->coupe.ptop, champ->coupe.pref, champ->coupe.rcoef);
/*              if (p < champ->coupe.ptop) p = ptop;*/
              c_wgldrx(valeurs[n][i]/champ->facteur, p);
              c_wglxai(&i1, &j1, valeurs[n][i]/champ->facteur, p);
              AfficherSymbole(i1,j1,n);
              }
            }
          else
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

int CoupeMgrGetFenetreCoupeID(int *fenetreID)
{
   *fenetreID = fenetreCoupe;
   return 0;
   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


void AfficherLigneCoupe()
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



void EnleverLigneCoupe(float x1, float y1, float x2, float y2)
{
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


int  CoupeMgrGetCoupeCoords(float *x1, float *y1, float *x2, float *y2)
{
   *x1 = cx1;
   *x2 = cx2;
   *y1 = cy1;
   *y2 = cy2;
   return 0;
   }

int VerifierExistenceCoupeValide()
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

void EffacerCoupe ()
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

void CoupeMgrSetMinMax()
{
   if (dimensionCoupe == ZP)
      {
      CoupeMgrSetMinMaxCoupe();
      }
   else
      {
      SerieMgrSetMinMaxSerie();
      }
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/


void CoupeMgrSetMinMaxCoupe()
{   
   _Champ *champ, *champ2;
   int i,nbChampsActifs,op;
   int ixmin, iymin, ixmax, iymax;
   char tempStr[16];
   char *returnedStr;
   Arg args[4];
   int fenetreCoupe;
   float opmin[5],opmax[5];
   int un = 1;
   int npts;
   int selectedVertCoord;
   
   ixmin = 0; ixmax = 0;
   iymin = 0; iymax = 0;
   
   nbChampsActifs = FldMgrGetNbChampsActifs();

   op = CtrlMgrGetMathOp();
   FldMgrGetChamp(&champ, nbChampsActifs-1);

   if (nbChampsActifs == 1)
      {
      op = NO_OP;
      }
   else
      {
      for (i=0; i < nbChampsActifs; i+=2)
         {
         FldMgrGetChamp(&champ, i);
         FldMgrGetChamp(&champ2, i+1);
         if (champ->natureTensorielle != champ2->natureTensorielle)
            {
            op = NO_OP;
            }
         }
      }

   
   if (VerifierExistenceCoupeValide())
      {
      selectedVertCoord = GetSelectedVertCoord();
      switch (calculMinMax_X)
        {
        case AUTO_PROFIL:
        npts = champ->coupe.niCoupe*champ->coupe.njCoupe;
      
        switch (op)
          {
          case NO_OP:
          FldMgrGetChamp(&champ, 0);
          switch(champ->natureTensorielle)
            {
            case SCALAIRE:
            f77name(aminmax)(&grafMinX,&grafMaxX,champ->coupe.fld2d,&npts,&un);
            grafMinX /= champ->facteur;
            grafMaxX /= champ->facteur;
            break;
            
            case VECTEUR:
            f77name(aminmax)(&grafMinX,&grafMaxX,champ->coupe.uvw2d,&npts,&un);
            grafMinX /= champ->facteur;
            grafMaxX /= champ->facteur;
            break;
            }
          
          if (champ->coupe.niCoupe == 1)
            {
            for (i=1; i < nbChampsActifs; i++)
              {
              FldMgrGetChamp(&champ, i);
              switch(champ->natureTensorielle)
                {
                case SCALAIRE:
                npts = champ->coupe.niCoupe*champ->coupe.njCoupe;
                f77name(aminmax)(&opmin[op],&opmax[op],champ->coupe.fld2d,&npts,&un);
                opmin[op] /=  champ->facteur;
                opmax[op] /=  champ->facteur;
                grafMinX = grafMinX < opmin[op] ? grafMinX : opmin[op];
                grafMaxX = grafMaxX > opmax[op] ? grafMaxX : opmax[op];
                break;
                
                case VECTEUR:
                npts = champ->coupe.niCoupe*champ->coupe.njCoupe;
                f77name(aminmax)(&opmin[op],&opmax[op],champ->coupe.uvw2d,&npts,&un);
                opmin[op] /=  champ->facteur;
                opmax[op] /=  champ->facteur;
                grafMinX = grafMinX < opmin[op] ? grafMinX : opmin[op];
                grafMaxX = grafMaxX > opmax[op] ? grafMaxX : opmax[op];
                break;
                }
              }
            }
          break;
    
          default:
          FldMgrGetChamp(&champ, 0);
          FldMgrGetChamp(&champ2, 1);
          DiffMgrSetDiffs(champ->coupe.fld2d,champ2->coupe.fld2d,opmin,opmax,npts);
          grafMinX = opmin[op];
          grafMaxX = opmax[op];
    
          i = 2;
          while (i < nbChampsActifs)
            {
            if (i == nbChampsActifs - 1)
              {
              FldMgrGetChamp(&champ, i);
              f77name(aminmax)(&opmin[op],&opmax[op],champ->coupe.fld2d,&npts,&un);
              }
            else
              {
              FldMgrGetChamp(&champ, i);
              FldMgrGetChamp(&champ2, i+1);
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
        FldMgrGetChamp(&champ, 0);
        op = CtrlMgrGetMathOp();
                
        switch (champ->natureTensorielle)
          {
          case SCALAIRE:
          grafMinX = champ->coupe.FLDmin3d[op]/champ->facteur;
          grafMaxX = champ->coupe.FLDmax3d[op]/champ->facteur;
          break;
          
          case VECTEUR:
          grafMinX = champ->coupe.UVWmin3d[op]/champ->facteur;
          grafMaxX = champ->coupe.UVWmax3d[op]/champ->facteur;
          break;
          }
        
        if (champ->coupe.niCoupe == 1)
            {
            for (i=1; i < nbChampsActifs; i++)
              {
              FldMgrGetChamp(&champ, i);
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
        break;
        }
      
      switch (calculMinMax_Y)
        {
        case AUTO_LEVELS:
        FldMgrGetChamp(&champ, 0);        
        switch (champ->coordonneeVerticale)
          {
          case SIGMA:
          case HYBRIDE:
          switch (selectedVertCoord)
            {
            case NATIVE_VCOORD:
            grafMinY = champ->coupe.niveauMin;
            grafMaxY = champ->coupe.niveauMax;
            break;
            
            case PRES_VCOORD:
            grafMinY = champ->coupe.niveauPresMin;
            grafMaxY = champ->coupe.niveauPresMax;
            break;
            }
          break;
          
          default:
          grafMinY = champ->coupe.niveauMin;
          grafMaxY = champ->coupe.niveauMax;
          break;
          }
        break;
        
        case FIXES:
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

void CoupeMgrGetLimites(float *valmin, float *valmax, float *nivmin, float *nivmax)
  { 
  *valmin = grafMinX;
  *valmax = grafMaxX;
  *nivmin = grafMinY;
  *nivmax = grafMaxY;
  }

int CoupeMgrGetSensEchelle()
{
   return sensEchelle;
   }

int CoupeMgrGetToggleTopo()
  {
  return xc.statuts[TOPO];
  }


void InitFenetreCoupe()
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


void CoupeMgrSetUVWMinMax()
{   
   _Champ *champ, *champ2;
   int i,k,nbChampsActifs,op;
   int ixmin, iymin, ixmax, iymax;
   char tempStr[16];
   char *returnedStr;
   Arg args[4];
   int fenetreCoupe, selectedVertCoord;
   int npts, un;   
   float *uvwtang,*uvwnorm,*uu,*vv,*ww,*uv;

   un = 1;
   ixmin = 0; ixmax = 0;
   iymin = 0; iymax = 0;
   

   i = 0;
   nbChampsActifs = FldMgrGetNbChampsActifs();
   selectedVertCoord = GetSelectedVertCoord();

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
      if (champ->natureTensorielle != SCALAIRE && champ->coupe.coupeValide == 1)
        {
        npts = champ->coupe.niCoupe*champ->coupe.njCoupe;
        uvwtang = champ->coupe.uvwtang2d;
        uvwnorm = champ->coupe.uvwnorm2d;
        uu     = champ->coupe.uu2d;
        vv     = champ->coupe.vv2d;
        ww     = champ->coupe.ww2d;
        uv     = champ->coupe.uvw2d;

   switch (calculMinMax_X)
      {
      case AUTO_PROFIL:
      switch (champ->coordonneeVerticale)
        {
        case SIGMA:
        case HYBRIDE:
        switch (selectedVertCoord)
          {
          case NATIVE_VCOORD:
          grafMinY = champ->coupe.niveauMin;
          grafMaxY = champ->coupe.niveauMax;
          break;
          
          case PRES_VCOORD:
          grafMinY = champ->coupe.niveauPresMin;
          grafMaxY = champ->coupe.niveauPresMax;
          break;
          }
        break;
        
        default:
        grafMinY = champ->coupe.niveauMin;
        grafMaxY = champ->coupe.niveauMax;
        break;
        }
      
      switch(op)
        {
        case NO_OP:
        f77name(aminmax)(&(champ->coupe.UVWTANGmin2d[NO_OP]), &(champ->coupe.UVWTANGmax2d[NO_OP]),uvwtang,&npts,&un);
        f77name(aminmax)(&(champ->coupe.UVWNORMmin2d[NO_OP]),&(champ->coupe.UVWNORMmax2d[NO_OP]),uvwnorm,&npts,&un);
        f77name(aminmax)(&(champ->coupe.UUmin2d[NO_OP]),&(champ->coupe.UUmax2d[NO_OP]),uu,&npts,&un);
        f77name(aminmax)(&(champ->coupe.VVmin2d[NO_OP]),&(champ->coupe.VVmax2d[NO_OP]),vv,&npts,&un);
        f77name(aminmax)(&(champ->coupe.WWmin2d[NO_OP]),&(champ->coupe.WWmax2d[NO_OP]),ww,&npts,&un);
        f77name(aminmax)(&(champ->coupe.UVWmin2d[NO_OP]),&(champ->coupe.UVWmax2d[NO_OP]),uv,&npts,&un);
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
    switch (champ->coordonneeVerticale)
      {
      case SIGMA:
      case HYBRIDE:
      switch (selectedVertCoord)
        {
        case NATIVE_VCOORD:
        grafMinY = champ->coupe.niveauMin;
        grafMaxY = champ->coupe.niveauMax;
        break;
        
        case PRES_VCOORD:
        grafMinY = champ->coupe.niveauPresMin;
        grafMaxY = champ->coupe.niveauPresMax;
        break;
        }
      break;
      
      default:
      grafMinY = champ->coupe.niveauMin;
      grafMaxY = champ->coupe.niveauMax;
      break;
      }
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


void CoupeMgrGetLimitesUVW(float *uutanmin, float *uutanmax, float *uvwmin,float *uvwmax,float *uumin,float *uumax,float *vvmin,float *vvmax,float *wwmin,float *wwmax,float *nivmin,float *nivmax)
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

