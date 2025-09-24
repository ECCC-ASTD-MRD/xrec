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
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <xinit.h>
#include <wgl.h>
#include <rmn/rpnmacros.h>
#include <limits.h>
/***************************/
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
   
#if defined (Darwin_OSX_PPC)
#else
#if defined (AIX)
#else
long times (struct tms *buffer);
#endif
#endif

struct tms buffer_temps;
extern SuperWidgetStruct SuperWidget;
extern Widget            wglTopLevel;
extern _XContour xc;
extern _AnimInfo animInfo;
extern GeoMapInfoStruct mapInfo;

extern Display *wglDisp;
extern int  wglScrNum;
extern Window  wglWin, wglDrawable;
extern GC wglLineGC;

extern void EffacerFenetreAffichage();

static char *pasAssezDeNiveaux[] = {"Nombre insuffisant de niveaux.",
                                       "Insufficient number of levels."};

extern int interpolationVerticale;
static int nbNiveauxCoupe;
static int lng;

void VAnimerFrames(int nbFrames)
{
  if (interpolationVerticale)
    {
    VAnimerFramesAvecInterpolation(nbFrames);
    }
  else
    {
    VAnimerFramesSansInterpolation(nbFrames);
    }
}

void VAnimerFramesAvecInterpolation(int nbFrames)
{
   int i, k;
   int n;
   int signeIncrement;
   float incrementVertical;
   int res, fenetreAffichage;
   int nbChampsActifs;
   _Champ *champ;

   int largeurFenetre, hauteurFenetre;
   static int lastLargeur, lastHauteur;
   int animationContinue;
   int op;
   float niveauCourant;
   double delai;

   nbChampsActifs = FldMgrGetNbChampsActifs();
   
   if (nbChampsActifs == 0)
     {
     return;
     }
   
   lng = c_getulng();
   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   
   xc.annulationDemandee = False;
   xc.flagInterrupt = False;
   FldMgrGetChamp(&champ, 0);
   
   if ((*champ).champModifie)
     {
     RefuserOperation();
     return;
     }
   
   c_wglsetw(fenetreAffichage);
   res = FldMgrLoadVerticalXSection();
   
   if ((*champ).coupe.nbNiveauxCoupe < 2)
     {
     MessageAvertissement(pasAssezDeNiveaux[lng], AVERTISSEMENT);
     return;
     }
   
   if (res == CHARGEMENT_ANNULE)
     {
     AfficherOperationAnnulee();
     AfficherMessageInfoStandard();
     return;
     }
   
   op = CtrlMgrGetMathOp();
   i = (*champ).coupe.indChampCourant;
   
   XSetErrorHandler(PasAssezDeMemoire);
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglbbf();
   EffacerFenetreAffichage();
   
   if (nbFrames > 0)
     signeIncrement =  1;
   else
     signeIncrement = -1;
   
   if (nbFrames == 2 || nbFrames == -2)
     {
     UnSetCurseur(fenetreAffichage);
     SetIgnoreMode();
     animationContinue = TRUE;
     }
   else
     animationContinue = FALSE;
   
   FlusherTousLesEvenements();
   
   nbNiveauxCoupe = (*champ).coupe.nbNiveauxCoupe;
   niveauCourant = champ->coupe.niveauxCoupe[0];
   incrementVertical = animInfo.intervalle;
   do 
     {
     niveauCourant -= signeIncrement*animInfo.intervalle;
     if (niveauCourant < champ->coupe.niveauxCoupe[nbNiveauxCoupe-1])
       {     
       if (animInfo.typeDefilement == DEFILEMENT_AVANT_ARRIERE)
	 {
	 i = nbNiveauxCoupe-1;
	 niveauCourant = champ->coupe.niveauxCoupe[nbNiveauxCoupe-1];
	 }
       else
	 {
	 i = 0;
	 niveauCourant = champ->coupe.niveauxCoupe[0];
	 }
       }
     i = i % (*champ).coupe.nbNiveauxCoupe;
     if (i < 0) i = (*champ).coupe.nbNiveauxCoupe-1;
     
     for (n=0; n < nbChampsActifs; n++)
       {
       FldMgrGetChamp(&champ, n);
       if (champ->coupe.nbNiveauxCoupe > 0)
	 {
	 k = i;
	 while (niveauCourant <= champ->coupe.niveauxCoupe[k] && k < nbNiveauxCoupe)
	   k++;
	 k--;
	 i = k;
	 (*champ).coupe.indChampCourant = i;
	 (*champ).cle = (*champ).coupe.clesNiveaux[i];
	 (*champ).niveau = niveauCourant;
	 (*champ).ip1 = ROUND(niveauCourant);
	 switch (champ->natureTensorielle)
	   {
	   case SCALAIRE:
	     (*champ).fldmin[op] = (*champ).coupe.FLDmin3d[op];
	     (*champ).fldmax[op] = (*champ).coupe.FLDmax3d[op];
	     break;

	   case VECTEUR:
	     (*champ).uumin[op] = (*champ).coupe.UUmin3d[op];
	     (*champ).uumax[op] = (*champ).coupe.UUmax3d[op];
	     (*champ).vvmin[op] = (*champ).coupe.VVmin3d[op];
	     (*champ).vvmax[op] = (*champ).coupe.VVmax3d[op];
	     (*champ).uvmin[op] = (*champ).coupe.UVmin3d[op];
	     (*champ).uvmax[op] = (*champ).coupe.UVmax3d[op];
	     break;
	   }

	 FldMgrUpdateFldParams(champ);
	 
	 if ((*champ).coupe.clesNiveaux[i] >= 0)
	   {
	   VAnimMgrInterpolateVerticalField(champ, niveauCourant);
	   }
	 }
       }
     
     for (n=0; n < nbChampsActifs; n++)
       {
       AfficherCarte(n);
       }
     
     FlusherTousLesEvenements();
     c_wglswb();
     animInfo.flagsImagesChargees[i] = TRUE;
     
     delai = (double) animInfo.delai;
     f77name(micro_sleep)(&delai);
     FlusherTousLesEvenements();
     
     if (animInfo.typeDefilement == DEFILEMENT_AVANT_ARRIERE)
       {

       if (niveauCourant >= champ->coupe.niveauxCoupe[0] || niveauCourant <= champ->coupe.niveauxCoupe[nbNiveauxCoupe-1])
        {
        signeIncrement *= -1;
        }
       }
     } while (!c_wglanul() && animationContinue);
   
   UnsetIgnoreMode();
   UnSetCurseur(fenetreAffichage);
   VRemettreChampsAJour(i);
   lastLargeur = largeurFenetre;
   lastHauteur = hauteurFenetre;
   c_wglfbf();
}

void VAnimerFramesSansInterpolation(int nbFrames)
{
   int i;
   int n;
   int increment;
   
   int res, fenetreAffichage;
   int nbChampsActifs;
   _Champ *champ;

   int npts,temps1, temps2;
   int largeurFenetre, hauteurFenetre;
   static int lastLargeur, lastHauteur;
   int animationContinue;
   int op,i_initial;

   nbChampsActifs = FldMgrGetNbChampsActifs();
   
   if (nbChampsActifs == 0)
     {
     return;
     }
   
   if (nbFrames == 1)
     {
     VAnimerFrame();
     return;
     }

   lng = c_getulng();
   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   
   xc.annulationDemandee = False;
   xc.flagInterrupt = False;
   FldMgrGetChamp(&champ, 0);
   
   npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;
   
   if ((*champ).champModifie)
     {
     RefuserOperation();
     return;
     }
   
   c_wglsetw(fenetreAffichage);
   res = FldMgrLoadVerticalXSection();
   if ((*champ).coupe.nbNiveauxCoupe < 2)
     {
     MessageAvertissement(pasAssezDeNiveaux[lng], AVERTISSEMENT);
     return;
     }
   
   if (res == CHARGEMENT_ANNULE)
     {
     AfficherOperationAnnulee();
     AfficherMessageInfoStandard();
     return;
     }
   
   op = CtrlMgrGetMathOp();
   i = (*champ).coupe.indChampCourant;
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglbbf();
   EffacerFenetreAffichage();
   
   if (nbFrames > 0)
     increment =  1;
   else
     increment = -1;
   
   if (nbFrames == 2 || nbFrames == -2)
     {
     UnSetCurseur(fenetreAffichage);
     SetIgnoreMode();
     animationContinue = TRUE;
     }
   else
     animationContinue = FALSE;
   
   FlusherTousLesEvenements();
   
   nbNiveauxCoupe = (*champ).coupe.nbNiveauxCoupe;
   
   if (animInfo.animationRapide)
     {
     if (!animInfo.imagesDejaAllouees || lastLargeur != largeurFenetre || lastHauteur != hauteurFenetre)
       {
       LibererImages();
      do
        {
        } while (VAllouerImages() == PAS_ASSEZ_DE_MEMOIRE);
      }
    }
   
     i = (*champ).coupe.indChampCourant;
     i_initial = i;
   
   do 
     {
     i = i % (*champ).coupe.nbNiveauxCoupe;
     if (i < 0)
       i += (*champ).coupe.nbNiveauxCoupe;
   
     if (animInfo.animationRapide && animInfo.flagsImagesChargees[i])
       {
       XCopyArea(wglDisp, animInfo.pixmaps[i], wglWin, wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
       }
     else
       {
       for (n=0; n < nbChampsActifs; n++)
        {
        FldMgrGetChamp(&champ, n);
        if ((*champ).coupe.fld3d != NULL)
          {
          (*champ).coupe.indChampCourant = i;
          (*champ).cle = (*champ).coupe.clesNiveaux[i];
          if ((*champ).cle >= 0)
            {
            memcpy((char *) (*champ).fld, (char *)(*champ).coupe.fld3d[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float));
            (*champ).fldmin[op] = (*champ).coupe.FLDmin3d[op];
            (*champ).fldmax[op] = (*champ).coupe.FLDmax3d[op];

            FldMgrGetFstPrm(champ);
            FldMgrUpdateFldParams(champ);
            }
          }
        }
       
       for (n=0; n < nbChampsActifs; n++)
        {
        AfficherCarte(n);
        }
       
       FlusherTousLesEvenements();
       c_wglswb();
       animInfo.flagsImagesChargees[i] = TRUE;
       
       if (animInfo.animationRapide)
        {
        XCopyArea(wglDisp, wglWin, animInfo.pixmaps[i], wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
        }
       
       temps1 = times(&buffer_temps);
       temps2 = times(&buffer_temps) - temps1;
       while (animInfo.delai > temps2)
        {
        temps2 = times(&buffer_temps) - temps1;
        }
       FlusherTousLesEvenements();
       
       if (animInfo.typeDefilement == DEFILEMENT_AVANT_ARRIERE)
        {
        if (i == 0 || i == (animInfo.nbImages-1))
          {
          increment *= -1;
          }
        }
       }
     i+=increment;
     } while (!c_wglanul() && animationContinue);
   
   UnsetIgnoreMode();
   UnSetCurseur(fenetreAffichage);
   if (i < 0) i += (*champ).coupe.nbNiveauxCoupe;

   VRemettreChampsAJour(i); 
   lastLargeur = largeurFenetre;
   lastHauteur = hauteurFenetre;
   c_wglfbf();
   }

/**
 **************************************************************
 **************************************************************
 **/

void VAfficherOperationAnnulee()
{

   static char *operationAnnulee[] = {"\nOperation annulee\n\n", "\nOperation cancelled\n\n"};
   int lng;

   lng = c_getulng();
   
   MessageAvertissement(operationAnnulee[lng], AVERTISSEMENT);

   }

/**
 **************************************************************
 **************************************************************
 **/

void VRemettreChampsAJour(int i)
{
   _Champ (*champ);
   int n, nbChampsActifs,op,npts;
   
   op = CtrlMgrGetMathOp();
   nbChampsActifs = FldMgrGetNbChampsActifs();

   FldMgrGetChamp(&champ, 0);
   npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;

   for  (n=0; n < nbChampsActifs; n++)
     {
     FldMgrGetChamp(&champ, n);
     if ((*champ).coupe.fld3d != NULL)
       {
       (*champ).coupe.indChampCourant = i;
       (*champ).cle = (*champ).coupe.clesNiveaux[i];
       if ((*champ).cle >= 0)
        {
        switch (champ->natureTensorielle)
          {
          case SCALAIRE:
        		memcpy((char *) (*champ).fld, (char *)(*champ).coupe.fld3d[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float));
            (*champ).fldmin[op] = (*champ).coupe.FLDmin3d[op];
            (*champ).fldmax[op] = (*champ).coupe.FLDmax3d[op];
            break;

          case VECTEUR:
          memcpy((char *) (*champ).uu, (char *)(*champ).coupe.uu3d[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
          memcpy((char *) (*champ).vv, (char *)(*champ).coupe.vv3d[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
/*        memcpy((char *) (*champ).module, (char *) (*champ).seqanim.animUVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); */
            (*champ).uumin[op] = (*champ).coupe.UUmin3d[op];
            (*champ).uumax[op] = (*champ).coupe.UUmax3d[op];
            (*champ).vvmin[op] = (*champ).coupe.UUmin3d[op];
            (*champ).vvmax[op] = (*champ).coupe.UUmax3d[op];
            (*champ).uvmin[op] = (*champ).coupe.UVmin3d[op];
            (*champ).uvmax[op] = (*champ).coupe.UVmax3d[op];
            break;
          }

        FldMgrGetFstPrm(champ);
        FldMgrUpdateFldParams(champ);
        }
       }
     }
}

/**
 **************************************************************
 **************************************************************
 **/

int VAllouerImages()
{
   int i, j;
   int x, y;
   Window root;
   unsigned int width, height, border_width, depth, nplanes;
   int largeurFenetre, hauteurFenetre;
   int status, nouvelleLargeur, nouvelleHauteur; 
   int wglWin;
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   animInfo.nbImages = nbNiveauxCoupe;
   nplanes = c_wglgpl();
   for (i=0; i < nbNiveauxCoupe; i++)
      {
      animInfo.pixmaps[i] = XCreatePixmap(wglDisp, RootWindow(wglDisp, wglScrNum), largeurFenetre, hauteurFenetre, nplanes);
      status = XGetGeometry(wglDisp, animInfo.pixmaps[i], &root, &x, &y, &width, &height, &border_width, &depth);
      if (status == 0)
	 {
	 nouvelleLargeur = (int)((float)largeurFenetre*sqrt((float)(i)/(float)animInfo.nbImages));
	 nouvelleHauteur = (int)((nouvelleLargeur*hauteurFenetre)/largeurFenetre);
	 nouvelleLargeur = (int)(0.95*nouvelleLargeur);
	 nouvelleHauteur = (int)(0.95*nouvelleHauteur);
	 for (j=0; j <= i; j++)
	    {
	    XFreePixmap(wglDisp, animInfo.pixmaps[j]);
	    animInfo.pixmaps[j] = (int)NULL;
	    animInfo.flagsImagesChargees[j] = (int)NULL;
	    }
	 wglWin = c_wglgacw();
	 XResizeWindow(wglDisp,wglWin,nouvelleLargeur,nouvelleHauteur);
	 c_wglsbf();
	 c_wgldbf();
	 c_wglbbf();
	 return PAS_ASSEZ_DE_MEMOIRE;
	 }
      }

   animInfo.imagesDejaAllouees = TRUE;
   return 0;
   }
   
   
void VAnimerFrame()
{
  int i;
  int n;
  int increment;
  
  int res, fenetreAffichage;
  int nbChampsActifs;
  _Champ *champ;
  
  int npts;
  int largeurFenetre, hauteurFenetre;
  static int lastLargeur, lastHauteur;
  int animationContinue;
  int op,i_initial;
  
  nbChampsActifs = FldMgrGetNbChampsActifs();
  
  if (nbChampsActifs == 0)
    {
    return;
    }

  lng = c_getulng();
  GetFenetreAffichageID(&fenetreAffichage);
  c_wglsetw(fenetreAffichage);
  
  xc.annulationDemandee = False;
  xc.flagInterrupt = False;
  FldMgrGetChamp(&champ, 0);
  
  npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;
  
  if ((*champ).champModifie)
    {
    RefuserOperation();
    return;
    }
  
  res = FldMgrLoadVerticalXSection();
  if ((*champ).coupe.nbNiveauxCoupe < 2)
    {
    MessageAvertissement(pasAssezDeNiveaux[lng], AVERTISSEMENT);
    return;
    }
   
  if (res == CHARGEMENT_ANNULE)
    {
    AfficherOperationAnnulee();
    AfficherMessageInfoStandard();
    return;
    }
  
  i = (*champ).coupe.indChampCourant;
  
  XSetErrorHandler(PasAssezDeMemoire);
   
  c_wglgwz(&largeurFenetre, &hauteurFenetre);
  c_wglbbf();
  EffacerFenetreAffichage();
  
  increment =  1;
  
  animationContinue = FALSE;
  
  FlusherTousLesEvenements();
  
  nbNiveauxCoupe = (*champ).coupe.nbNiveauxCoupe;
  
  op = CtrlMgrGetMathOp();
  i = (*champ).coupe.indChampCourant;
  i_initial = i;

  i += increment;
  i = i % (*champ).coupe.nbNiveauxCoupe;
  if (i < 0)
    i += (*champ).coupe.nbNiveauxCoupe;
  
  if (animInfo.animationRapide && animInfo.flagsImagesChargees[i])
    {
    XCopyArea(wglDisp, animInfo.pixmaps[i], wglWin, wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
    }
  else
    {
    for (n=0; n < nbChampsActifs; n++)
      {
      FldMgrGetChamp(&champ, n);
      if ((*champ).coupe.fld3d != NULL)
        {
        (*champ).coupe.indChampCourant = i;
        (*champ).cle = (*champ).coupe.clesNiveaux[i];
        if ((*champ).cle >= 0)
          {
          memcpy((char *) (*champ).fld, (char *)(*champ).coupe.fld3d[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float));
          (*champ).fldmin[op] = (*champ).coupe.FLDmin3d[op];
          (*champ).fldmax[op] = (*champ).coupe.FLDmax3d[op];
          
          FldMgrGetFstPrm(champ);
          FldMgrUpdateFldParams(champ);
          }
        }
      }
    }
      
  for (n=0; n < nbChampsActifs; n++)
    {
    AfficherCarte(n);
    }
        
  FlusherTousLesEvenements();
  c_wglswb();
      
  animInfo.flagsImagesChargees[i] = TRUE;
  
  if (animInfo.animationRapide)
    {
    XCopyArea(wglDisp, wglWin, animInfo.pixmaps[i], wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
    }
  
  FlusherTousLesEvenements();
    
  if (animInfo.typeDefilement == DEFILEMENT_AVANT_ARRIERE)
      {
      if (i == 0 || i == (animInfo.nbImages-1))
        {
        increment *= -1;
        }
      }
    
  UnsetIgnoreMode();
  UnSetCurseur(fenetreAffichage);
  VRemettreChampsAJour(i);
  lastLargeur = largeurFenetre;
  lastHauteur = hauteurFenetre;
  c_wglfbf();
  }

void VAnimMgrInterpolateVerticalField(_Champ *champ, float niveau)
{
   int i, n;
   int npts;
   double weight;
   int found,nbNiveauxCoupe;
   static int last_index;

   npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;
   
   nbNiveauxCoupe = (*champ).coupe.nbNiveauxCoupe;
   
   i = 0;
   found = 0;
   while (niveau < champ->coupe.niveauxCoupe[i] && found == 0 && i < nbNiveauxCoupe)
     {
     i++;
     }
   i--;
   if (i < 0) i = 0;
   found = i;
   if (found == champ->coupe.nbNiveauxCoupe) 
     {
     found = 0;
     }

   i = (i < champ->coupe.nbNiveauxCoupe-1 ) ? i : champ->coupe.nbNiveauxCoupe-2;

   weight = (niveau - champ->coupe.niveauxCoupe[i])/(champ->coupe.niveauxCoupe[i+1]-champ->coupe.niveauxCoupe[i]);
   switch (champ->natureTensorielle)
     {
     case SCALAIRE:
       for (n=0; n < champ->dst.ni*champ->dst.nj; n++)
	 {
	 champ->fld[n]=((1.0-weight)*champ->coupe.fld3d[i][n]+weight*champ->coupe.fld3d[i+1][n]);
	 }
       break;

     case VECTEUR:
       for (n=0; n < champ->dst.ni*champ->dst.nj; n++)
	 {
	 champ->uu[n]=((1.0-weight)*champ->coupe.uu3d[i][n]+weight*champ->coupe.uu3d[i+1][n]);
	 champ->vv[n]=((1.0-weight)*champ->coupe.vv3d[i][n]+weight*champ->coupe.vv3d[i+1][n]);
	 champ->module[n]=sqrt(champ->uu[n]*champ->uu[n]+champ->vv[n]*champ->vv[n]);
	 }
       break;

     default:
       break;
     }
   
   last_index = i;
   
}

