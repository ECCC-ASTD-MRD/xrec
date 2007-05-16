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
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <gmp.h>
#include <xinit.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <limits.h>
/***************************/
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
   
#ifndef HP
long times (struct tms *buffer);
#else
extern clock_t times();
#endif

struct tms buffer_temps;
extern SuperWidgetStruct SuperWidget;
extern Widget            wglTopLevel;
extern _XContour xc;
extern _AnimInfo animInfo;
extern GeoMapInfoStruct mapInfo;

#ifdef X_WGL
   extern Display *wglDisp;
   extern int  wglScrNum;
   extern Window  wglWin, wglDrawable;
   extern GC wglLineGC;
#endif

extern void EffacerFenetreAffichage();

static char *pasAssezDeMemoire[] = {"Quantite de memoire insuffisante.\nSeulement %4d images sur %4d peuvent etre chargees.\n\
L'animation serait possible si les dimensions de la fenetre\netaient reduites a %4d x %4d",
                                     "Not enough memory.\nOnly %4d images on %4d can be loaded.\nHowever, animation could be possible if window dimension is reduced to %4d x4d"};

static char *pasAssezDeNiveaux[] = {"Nombre insuffisant de niveaux.",
                                       "Insufficient number of levels."};

static char *uneSeulePeriode[] = {"Ce champ n'est disponible que pour une seule periode.\nImpossible d'animer!", 
				     "This field is available for one single time. Cannot animate..."};

int PasAssezDeMemoire();

float min, max;
int nbChampsAnim;
char messageErreur[512];
static int lng;

AnimerFrames(nbFrames)
int nbFrames;
{
   int i, j, k, ier, ii;
   int n;
   int annulationDemandee;
   int statut, increment;
   
   int res, fenetreAffichage, status;
   int nbChampsActifs;
   _Champ *champ;

   int npts,temps1, temps2;
   int largeurFenetre, hauteurFenetre, nouvelleLargeur, nouvelleHauteur;
   static int lastLargeur, lastHauteur;
   int animationContinue;
   int op,i_initial;
   double dt,dt1,dt2;
   int datev;
   double weight;
   float *fld, *fld1, *fld2;

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
   
   c_wglsetw(fenetreAffichage);
   if (animInfo.variableBoucle == TEMPS)
     {
     EffacerCoupe();
     res  = FldMgrLoadTimeAnimationSeq();
     }
   else
     {
     res = FldMgrLoadVerticalXSection();
     if ((*champ).coupe.nbNiveauxCoupe < 2)
       {
       MessageAvertissement(pasAssezDeNiveaux[lng], AVERTISSEMENT);
       return;
       }
     }
   
   if (res == PAS_ASSEZ_DE_PERIODES)
     {
     MessageAvertissement(uneSeulePeriode[lng], AVERTISSEMENT);
     return;
     }
   
   if (res == CHARGEMENT_ANNULE)
     {
     AfficherOperationAnnulee();
     AfficherMessageInfoStandard();
     return;
     }
   
   op = CtrlMgrGetMathOp();
   if (animInfo.variableBoucle == TEMPS)
     i = (*champ).seqanim.indChampCourant;
   else 
     i = (*champ).coupe.indChampCourant;
   
   XSetErrorHandler(PasAssezDeMemoire);
   
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
   
   if (animInfo.variableBoucle == TEMPS)
     nbChampsAnim = (*champ).seqanim.nbFldsAnim;
   else
     nbChampsAnim = (*champ).coupe.nbNiveauxCoupe;
   
   if (animInfo.animationRapide)
     {
     if (!animInfo.imagesDejaAllouees || lastLargeur != largeurFenetre || lastHauteur != hauteurFenetre)
       {
       LibererImages();
       do
	 {
	 } while (AllouerImages() == PAS_ASSEZ_DE_MEMOIRE);
       }
     }
   
   if (animInfo.variableBoucle == TEMPS)
     {
     if (nbFrames == 2)
       {
       dt = (double) (champ->deet * champ->npas) / 3600.0;
       f77name(incdatr)(&datev, &(champ->date), &dt);
       }
     else
       {
       i = i % (*champ).seqanim.nbFldsAnim;
       if (i < 0)
	 i += (*champ).seqanim.nbFldsAnim;
       }
     }
   else
     {
     i = (*champ).coupe.indChampCourant;
     i_initial = i;
     }
   
   do 
     {
     if (nbFrames == 2)
       {
       dt = dt + (animInfo.intervalle/60.0);
       }
     else
       {
       }
     if (animInfo.variableBoucle == TEMPS)
       {
       i = i % (*champ).seqanim.nbFldsAnim;
       if (i < 0)
	 i += (*champ).seqanim.nbFldsAnim;
       }
     else
       {
       i = i % (*champ).coupe.nbNiveauxCoupe;
       if (i < 0)
	 i += (*champ).coupe.nbNiveauxCoupe;
       }
     
     if (animInfo.animationRapide && animInfo.flagsImagesChargees[i])
       {
#ifdef X_WGL
       XCopyArea(wglDisp, animInfo.pixmaps[i], wglWin, wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
#else
       rectwrite(0, 0, largeurFenetre-1, hauteurFenetre-1, animInfo.pixmaps[i]);
       c_wglswb();
#endif
       }
     else
       {
       for (n=0; n < nbChampsActifs; n++)
	 {
	 FldMgrGetChamp(&champ, n);
	 if (animInfo.variableBoucle == TEMPS)
	   {
	   if (champ->seqanim.nbFldsAnim > 0)
	     {
	     k = i;
	     while (dt >= champ->seqanim.dt[k] && k < champ->seqanim.nbFldsAnim)
	       k++;
	     if (k == champ->seqanim.nbFldsAnim)
	       {
	       dt = 0.0;
	       k = 1;
	       }
	     k--;
	     i = k;
	     (*champ).seqanim.indChampCourant = i;
	     (*champ).cle = (*champ).seqanim.clesAnim[i];
	     if ((*champ).seqanim.clesAnim[i] >= 0)
	       {
	       if (champ->natureTensorielle == VECTEUR)
		 {
		 memcpy((char *) (*champ).uu, (char *)(*champ).seqanim.animUUs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		 memcpy((char *) (*champ).vv, (char *)(*champ).seqanim.animVVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		 memcpy((char *) (*champ).module, (char *) (*champ).seqanim.animUVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		 }
	       else
		 {
		 fld2 = champ->seqanim.animFLDs[i+1];
		 fld1 = champ->seqanim.animFLDs[i];
		 weight = (dt - champ->seqanim.dt[i])/(champ->seqanim.dt[i+1]-champ->seqanim.dt[i]);
		 /*		 printf("dt: %f intervalle: %f weight: %f\n", dt, animInfo.intervalle, weight); */
		 for (ii=0; ii < champ->dst.ni*champ->dst.nj; ii++)
		   {
		   champ->fld[ii]=((1.0-weight)*fld1[ii]+weight*fld2[ii]);
		   }
		 }
	       FldMgrGetFstPrm(champ);
	       champ->rnpas = (3600.0 * dt) / champ->deet;
	       FldMgrUpdateFldParams(champ);
	       }
	     }
	   }
	 else
	   {
	   if ((*champ).coupe.fld3d != NULL)
	     {
	     (*champ).coupe.indChampCourant = i;
	     (*champ).cle = (*champ).coupe.clesNiveaux[i];
	     if ((*champ).cle >= 0)
	       {
	       (*champ).fld = (*champ).coupe.fld3d[i];
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
#ifdef X_WGL
       c_wglswb();
#endif
       animInfo.flagsImagesChargees[i] = TRUE;
       
       if (animInfo.animationRapide)
	 {
#ifdef X_WGL
	 XCopyArea(wglDisp, wglWin, animInfo.pixmaps[i], wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
#else
	 rectread(0, 0, largeurFenetre-1, hauteurFenetre-1, animInfo.pixmaps[i]);
#endif
	 }
       
#ifdef GL_WGL
       c_wglswb();
#endif	 
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
     } while (!c_wglanul() && animationContinue);
   
   UnsetIgnoreMode();
   UnSetCurseur(fenetreAffichage);
   RemettreChampsAJour(i);
   lastLargeur = largeurFenetre;
   lastHauteur = hauteurFenetre;
   c_wglfbf();
   }

/**
 **************************************************************
 **************************************************************
 **/

f77name(messinf)(message, lenMessage)
char *message;
int lenMessage;
{
   message[lenMessage] = '\0';
   strclean(message);
   MessageInfo(message);
   }


/**
 **************************************************************
 **************************************************************
 **/

MessageInfo(message, reverseVideo)
char message[];
int reverseVideo;
{
   Arg args[10];
   int i;
   static XmString xmMessage = NULL;

   XColor couleur, exact;
   Display *disp;
   Colormap cmap;
   static Pixel red = 65535;
   static Pixel back= 65535;
   static Pixel fore= 65535;

   if (back == 65535)
      {
      XtSetArg(args[0], XmNbackground, &back);
      XtSetArg(args[1], XmNforeground, &fore);
      XtGetValues(xc.menuFichier, args, 2);   
      }


   i = 0;
   if (reverseVideo)
      {
      XtSetArg(args[i], XmNbackground, fore); i++;
      XtSetArg(args[i], XmNforeground, back); i++;
      }
   else
      {
      XtSetArg(args[i], XmNbackground, back); i++;
      XtSetArg(args[i], XmNforeground, fore); i++;
      }

   XtSetValues(xc.labelInfo, args, i);

   i = 0;
   XtSetArg(args[i], XmNvalue, message); i++;
   XtSetValues(xc.labelInfo, args, i);

   /*
     if (xmMessage)
     XmStringFree(xmMessage);
     xmMessage = XmStringCreateLtoR(message, XmSTRING_DEFAULT_CHARSET);
     
     i = 0;
     XtSetArg(args[i], XmNlabelString, xmMessage); i++;
     XtSetValues(xc.labelInfo, args, i);
   */
   
   XmUpdateDisplay(xc.labelInfo);
   FlusherTousLesEvenements();
   }

/**
 **************************************************************
 **************************************************************
 **/

AfficherOperationAnnulee()
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

int PasAssezDeMemoire(disp, erreur)
Display *disp;
XErrorEvent *erreur;
{
   char msg[80];
   
   XGetErrorText(disp, erreur->error_code, msg, 80);
   fprintf(stderr, "ErrorCode: %s\n", msg);
   }


/**
 **************************************************************
 **************************************************************
 **/

RemettreChampsAJour(i)
int i;
{
   _Champ (*champ);
   int n, nbChampsActifs,op,npts;
   
   op = CtrlMgrGetMathOp();
   nbChampsActifs = FldMgrGetNbChampsActifs();

   FldMgrGetChamp(&champ, 0);
   npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;

   if (animInfo.variableBoucle == TEMPS)
      {
      for  (n=0; n < nbChampsActifs; n++)
	 {
	 FldMgrGetChamp(&champ, n);
	 if (champ->seqanim.nbFldsAnim > 0)
	    {
	    (*champ).seqanim.indChampCourant = i;
	    (*champ).cle = (*champ).seqanim.clesAnim[i];
	    if ((*champ).seqanim.clesAnim[i] >= 0)
	       {
	       if (champ->natureTensorielle == VECTEUR)
		  {
		  memcpy((char *) (*champ).uu, (char *)(*champ).seqanim.animUUs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		  memcpy((char *) (*champ).vv, (char *)(*champ).seqanim.animVVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		  memcpy((char *) (*champ).module, (char *) (*champ).seqanim.animUVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		  }
	       else
		  {
		  memcpy((char *)(*champ).fld, (*champ).seqanim.animFLDs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
		  }
	       
	       }
	    FldMgrGetFstPrm(champ);
	    FldMgrUpdateFldParams(champ);
	    }
	 }
      }
   else
      {
      for  (n=0; n < nbChampsActifs; n++)
	 {
	 FldMgrGetChamp(&champ, n);
	 if ((*champ).coupe.fld3d != NULL)
	    {
	    (*champ).coupe.indChampCourant = i;
	    (*champ).cle = (*champ).coupe.clesNiveaux[i];
	    if ((*champ).cle >= 0)
	       {
	       (*champ).fld = (*champ).coupe.fld3d[i];
	       (*champ).fldmin[op] = (*champ).coupe.FLDmin3d[op];
	       (*champ).fldmax[op] = (*champ).coupe.FLDmax3d[op];
	       
	       FldMgrGetFstPrm(champ);
	       FldMgrUpdateFldParams(champ);
	       }
	    }
	 }
      }
   }


/**
 **************************************************************
 **************************************************************
 **/

LibererImages()
{
   int i;

   animInfo.imagesDejaAllouees = FALSE;
   for (i=0; i < animInfo.nbImages; i++)
      {
      animInfo.flagsImagesChargees[i] = FALSE;
      if (animInfo.pixmaps[i] != NULL)
	 {
#ifdef X_WGL
	 XFreePixmap(wglDisp, animInfo.pixmaps[i]);
#else
	 free(animInfo.pixmaps[i]);
#endif
	 animInfo.pixmaps[i] = NULL;
	 }
      }

   animInfo.nbImages = 0;
   }



/**
 **************************************************************
 **************************************************************
 **/

AllouerImages()
{
   int i, j;
   int x, y;
   Window root;
   unsigned int width, height, border_width, depth, nplanes;
   int largeurFenetre, hauteurFenetre;
   int status, nouvelleLargeur, nouvelleHauteur; 
   int wglWin;
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   animInfo.nbImages = nbChampsAnim;
#ifdef X_WGL
   nplanes = c_wglgpl();
#endif
   for (i=0; i < nbChampsAnim; i++)
      {
#ifdef X_WGL
      animInfo.pixmaps[i] = XCreatePixmap(wglDisp, RootWindow(wglDisp, wglScrNum), largeurFenetre, hauteurFenetre, nplanes);
      status = XGetGeometry(wglDisp, animInfo.pixmaps[i], &root, &x, &y, &width, &height, &border_width, &depth);
      if (status == 0)
	 {
	 nouvelleLargeur = (int)((float)largeurFenetre*sqrt((float)(i)/(float)animInfo.nbImages));
	 nouvelleHauteur = (int)((nouvelleLargeur*hauteurFenetre)/largeurFenetre);
	 nouvelleLargeur = (int)(0.95*nouvelleLargeur);
	 nouvelleHauteur = (int)(0.95*nouvelleHauteur);
/**
	 sprintf(messageErreur, pasAssezDeMemoire[lng], i,animInfo.nbImages, nouvelleLargeur, nouvelleHauteur);
	 MessageAvertissement(messageErreur, AVERTISSEMENT);
**/
	 for (j=0; j <= i; j++)
	    {
	    XFreePixmap(wglDisp, animInfo.pixmaps[j]);
	    animInfo.pixmaps[j] = NULL;
	    animInfo.flagsImagesChargees[j] = NULL;
	    }
	 wglWin = c_wglgacw();
	 XResizeWindow(wglDisp,wglWin,nouvelleLargeur,nouvelleHauteur);
	 c_wglsbf();
	 c_wgldbf();
	 c_wglbbf();
	 return PAS_ASSEZ_DE_MEMOIRE;
	 }
#else
      animInfo.pixmaps[i] = (unsigned short *) calloc(largeurFenetre*hauteurFenetre, sizeof(unsigned short));
      if (animInfo.pixmaps[i] == NULL)
	 {
	 MessageAvertissement(messageErreur, AVERTISSEMENT);
	 for (j=0; j < i; j++)
	    {
	    free(animInfo.pixmaps[j]);
	    animInfo.pixmaps[j] = NULL;
	    animInfo.flagsImagesChargees[j] = NULL;
	    }
	 return PAS_ASSEZ_DE_MEMOIRE;
	 }
#endif
      }

   animInfo.imagesDejaAllouees = TRUE;
   return 0;
   }
   
   
