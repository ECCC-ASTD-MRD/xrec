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
#include <xinit.h>
#include <wgl.h>
#include <limits.h>
#include <rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>


#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

extern SuperWidgetStruct SuperWidget;
extern Widget            wglTopLevel;
extern _XContour xc;
extern _AnimInfo animInfo;
extern GeoMapInfoStruct mapInfo;


extern Display *wglDisp;
extern int  wglScrNum;
extern Window  wglWin, wglDrawable;
extern GC wglLineGC;
extern int interpolationTemporelle;

extern void EffacerFenetreAffichage();

static char *pasAssezDeMemoire[] = {"Quantite de memoire insuffisante.\nSeulement %4d images sur %4d peuvent etre chargees.\n\
L'animation serait possible si les dimensions de la fenetre\netaient reduites a %4d x %4d",
                                    "Not enough memory.\nOnly %4d images on %4d can be loaded.\nHowever, animation could be possible if window dimension is reduced to %4d x4d"};

static char *uneSeulePeriode[] = {"Ce champ n'est disponible que pour une seule periode.\nImpossible d'animer!", 
            "This field is available for one single time. Cannot animate..."};

float min, max;
int nbChampsAnim;
char messageErreur[512];
static int lng;

/* -------------------------------------------------------------------------------------------------- */

void AnimerFrames(int nbFrames)
{
  if (interpolationTemporelle)
    {
    AnimerFramesAvecInterpolation(nbFrames);
    }
  else
    {
    AnimerFramesSansInterpolation(nbFrames);
    }
}

/* -------------------------------------------------------------------------------------------------- */

void AnimerFramesSansInterpolation(int nbFrames)
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
  int synchro, datev;
  double weight;
  float *fld, *fld1, *fld2;
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

  npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;

  if ((*champ).champModifie)
    {
    RefuserOperation();
    return;
    }

  c_wglsetw(fenetreAffichage);
  EffacerCoupe();
  res  = FldMgrLoadTimeAnimationSeq();

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
  i = (*champ).seqanim.indChampCourant;

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
    {
    animationContinue = FALSE;
    }

  FlusherTousLesEvenements();

  nbChampsAnim = (*champ).seqanim.nbFldsAnim;

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

  do 
    {
    i += increment;
    i = i % ((*champ).seqanim.nbFldsAnim);
    if (i < 0) i = (*champ).seqanim.nbFldsAnim-1;

    if (animInfo.animationRapide && animInfo.flagsImagesChargees[i])
      {
      XCopyArea(wglDisp, animInfo.pixmaps[i], wglWin, wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
      }
    else
      {
      for (n=0; n < nbChampsActifs; n++)
        {
        FldMgrGetChamp(&champ, n);
        if (champ->seqanim.nbFldsAnim > 0)
          {
          (*champ).seqanim.indChampCourant = i;
          (*champ).cle = (*champ).seqanim.clesAnim[i];
      
          if ((*champ).seqanim.clesAnim[i] >= 0)
            {
            if ((*champ).natureTensorielle == VECTEUR)
              {
              memcpy((char *) (*champ).uu, (char *)(*champ).seqanim.animUUs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
              memcpy((char *) (*champ).vv, (char *)(*champ).seqanim.animVVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
              memcpy((char *) (*champ).module, (char *) (*champ).seqanim.animUVs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float)); 
              }
            else
              {
              memcpy((char *) (*champ).fld, (char *)(*champ).seqanim.animFLDs[i], (*champ).dst.ni*(*champ).dst.nj*sizeof(float));
              }
            FldMgrGetFstPrm(champ);
            FldMgrUpdateFldParams(champ);
            }
/*          AfficherCarte(n);*/
          }
        }

      for (n=0; n < nbChampsActifs; n++)
        {
        FldMgrGetChamp(&champ, n);
        if (champ->seqanim.nbFldsAnim > 0)
          {
          (*champ).seqanim.indChampCourant = i;
          (*champ).cle = (*champ).seqanim.clesAnim[i];
      
          if ((*champ).seqanim.clesAnim[i] >= 0)
            {
            FldMgrGetFstPrm(champ);
            FldMgrUpdateFldParams(champ);
            }
          AfficherCarte(n);
          }
        }
      
      FlusherTousLesEvenements();
      c_wglswb();
      if (animInfo.animationRapide)
        {
        animInfo.flagsImagesChargees[i] = TRUE;
        XCopyArea(wglDisp, wglWin, animInfo.pixmaps[i], wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
        }
      }

    if (animInfo.animationRapide)
      {
      j = 0;
      synchro = 1;
      while (j < ((*champ).seqanim.nbFldsAnim) && (1 == synchro))
        {
        if (0 == animInfo.flagsImagesChargees[j])
          {
          synchro = 0;
          }
        j++;
        }
      if (synchro == 1)
        {
        XSynchronize(wglDisp,True);
        /*   fprintf(stderr, "XSynchronize(wglDisp,True)\n"); */
        }
      else
        {
        XSynchronize(wglDisp,False);
        /* fprintf(stderr, "XSynchronize(wglDisp,False)\n"); */
        }
      }

    delai = (double) animInfo.delai;
    f77name(micro_sleep)(&delai);
    FlusherTousLesEvenements();

    if (animInfo.typeDefilement == DEFILEMENT_AVANT_ARRIERE)
      {
      if (i == 0 || i == (*champ).seqanim.nbFldsAnim-1)
        {
        increment *= -1;
        }
      }

    } while (!c_wglanul() && animationContinue);

  XSynchronize(wglDisp,False);
  UnsetIgnoreMode();
  UnSetCurseur(fenetreAffichage);
  FldMgrGetChamp(&champ, 0);
  i = i % ((*champ).seqanim.nbFldsAnim);
  if (i < 0) i = (*champ).seqanim.nbFldsAnim-1;
  for (n=0; n < nbChampsActifs; n++)
    {
      FldMgrGetChamp(&champ, n);
      (*champ).seqanim.indChampCourant = i;
      RemettreChampsAJour((*champ).seqanim.indChampCourant);
    }
  lastLargeur = largeurFenetre;
  lastHauteur = hauteurFenetre;
  c_wglfbf();
}

/* -------------------------------------------------------------------------------------------------- */

void AnimerFramesAvecInterpolation(int nbFrames)
{
  int i, j, k, ier, ii;
  int n;
  int annulationDemandee;
  int statut, increment;

  int res, fenetreAffichage, status;
  int nbChampsActifs;
  _Champ *champ;

  int temps1, temps2;
  int largeurFenetre, hauteurFenetre, nouvelleLargeur, nouvelleHauteur;
  static int lastLargeur, lastHauteur;
  int animationContinue;
  int op,i_initial;
  double dt,dt1,dt2;
  int datev;
  double weight;
  float *fld, *fld1, *fld2;
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
  EffacerCoupe();
  res  = FldMgrLoadTimeAnimationSeq();

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
  i = (*champ).seqanim.indChampCourant;

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

  nbChampsAnim = (*champ).seqanim.nbFldsAnim;

  dt = (double) (champ->deet * champ->npas) / 3600.0;
  f77name(incdatr)(&(champ->datev), &(champ->dateo), &dt);
  datev = champ->datev;
  
  do 
    {
    dt = dt + increment*(animInfo.intervalle/60.0);
    if (dt < 0.0) dt = 0.0;

    i = i % (*champ).seqanim.nbFldsAnim;
    if (i < 0) i = (*champ).seqanim.nbFldsAnim-1;

    for (n=0; n < nbChampsActifs; n++)
      {
      FldMgrGetChamp(&champ, n);
      if (champ->seqanim.nbFldsAnim > 0)
        {
        k = i;
        while (dt >= champ->seqanim.dt[k] && k < champ->seqanim.nbFldsAnim)
          k++;
        if (k == champ->seqanim.nbFldsAnim)
          {
          dt = 0.0;
          k--;
          }
        k--;
        i = k;
        (*champ).seqanim.indChampCourant = i;
        (*champ).cle = (*champ).seqanim.clesAnim[i];
      
        if ((*champ).seqanim.clesAnim[i] >= 0)
          {
          f77name(incdatr)(&datev, &champ->dateo, &dt);
          AnimMgrInterpolateTimeField(champ, datev);
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

/* -------------------------------------------------------------------------------------------------- */

void f77name(messinf)(char *message, F2Cl lenMessage)
{
  message[lenMessage] = '\0';
  strclean(message);
  MessageInfo(message, 0);
  }


/* -------------------------------------------------------------------------------------------------- */

void MessageInfo(char message[], int reverseVideo)
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

  XmUpdateDisplay(xc.labelInfo);
  FlusherTousLesEvenements();
  }

/* -------------------------------------------------------------------------------------------------- */

void AfficherOperationAnnulee()
{

  static char *operationAnnulee[] = {"\nOperation annulee\n\n", "\nOperation cancelled\n\n"};
  int lng;

  lng = c_getulng();

  MessageAvertissement(operationAnnulee[lng], AVERTISSEMENT);

  }

/* -------------------------------------------------------------------------------------------------- */

int PasAssezDeMemoire(Display *disp, XErrorEvent *erreur)
{
  char msg[80];

  XGetErrorText(disp, erreur->error_code, msg, 80);
  fprintf(stderr, "ErrorCode: %s\n", msg);
  }

/* -------------------------------------------------------------------------------------------------- */

void RemettreChampsAJour(int i)
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


/* -------------------------------------------------------------------------------------------------- */

void LibererImages()
{
  int i;

  animInfo.imagesDejaAllouees = FALSE;
  for (i=0; i < animInfo.nbImages; i++)
      {
      animInfo.flagsImagesChargees[i] = FALSE;
//      if (animInfo.pixmaps[i] != NULL)
      if (animInfo.pixmaps[i] != None)
  {
  XFreePixmap(wglDisp, animInfo.pixmaps[i]);
//  animInfo.pixmaps[i] = NULL;
  animInfo.pixmaps[i] = None;
  }
      }

  animInfo.nbImages = 0;
  }

/* -------------------------------------------------------------------------------------------------- */

int AllouerImages()
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
  nplanes = c_wglgpl();
  for (i=0; i < nbChampsAnim; i++)
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
//      animInfo.pixmaps[j] = NULL;
//      animInfo.flagsImagesChargees[j] = NULL;
      animInfo.pixmaps[j] = None;
      animInfo.flagsImagesChargees[j] = None;
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


/* -------------------------------------------------------------------------------------------------- */

void AnimMgrInterpolateTimeField(_Champ *champ, int datev)
{
  static int lastIndex = 0;

  int i, j, k, ier, n;
  int annulationDemandee;
  int statut, increment;

  int res, fenetreAffichage, status;
  int nbChampsActifs;

  int npts,temps1, temps2;
  int op,i_initial;
  double dt,dt1,dt2;
  double weight;
  float *fld, *fld1, *fld2, *uu1, *uu2, *vv1, *vv2, *uv1, *uv2;
  int ldatev, found;
  static int last_index;

  npts = champ->dst.ni*champ->dst.nj*champ->dst.nk;
  ldatev = datev;

  nbChampsAnim = (*champ).seqanim.nbFldsAnim;

  f77name(difdatr)(&ldatev, &(champ->dateo), &dt);

  i = last_index;
  i = 0;
  found = 0;
  if (dt > champ->seqanim.dt[i]) i = 0;
  while (dt > champ->seqanim.dt[i] && found == 0)
    {
    i++;
    }
  i--;
  if (i < 0) i = 0;
  found = i;
  if (found == champ->seqanim.nbFldsAnim) 
    {
    found = 0;
    }

  i = (i < champ->seqanim.nbFldsAnim-1 ) ? i : champ->seqanim.nbFldsAnim-2;

  weight = (dt - champ->seqanim.dt[i])/(champ->seqanim.dt[i+1]-champ->seqanim.dt[i]);
  switch (champ->natureTensorielle)
    {
    case SCALAIRE:
      for (n=0; n < champ->dst.ni*champ->dst.nj; n++)
  {
  champ->fld[n]=((1.0-weight)*champ->seqanim.animFLDs[i][n]+weight*champ->seqanim.animFLDs[i+1][n]);
  }
      break;

    case VECTEUR:
      for (n=0; n < champ->dst.ni*champ->dst.nj; n++)
  {
  champ->uu[n]=((1.0-weight)*champ->seqanim.animUUs[i][n]+weight*champ->seqanim.animUUs[i+1][n]);
  champ->vv[n]=((1.0-weight)*champ->seqanim.animVVs[i][n]+weight*champ->seqanim.animVVs[i+1][n]);
  champ->module[n]=((1.0-weight)*champ->seqanim.animUVs[i][n]+weight*champ->seqanim.animUVs[i+1][n]);
  }
      break;

    default:
      break;
    }

  FldMgrGetFstPrm(champ);
  champ->rnpas = (3600.0 * dt) / champ->deet;
  FldMgrUpdateFldParams(champ);
  last_index = i;

}

