/* Rmnlibx - Library of useful routines for C and FORTRAN programming
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
#include <wgl.h>
#include <rmn/rpnmacros.h>
#include <rmn/fstd98.h>
#include <rmn.h>
#include <rec.h>
#include <rec_functions.h>
#include <stdlib.h>
#include <math.h>

#define NB_MAX_CHAMPS_ACTIFS 32

/* -------------------------------------------------------------------------------------------------- */

_Champ fmflds[NB_MAX_CHAMPS_ACTIFS];
_GrilleMenuItem grmenuitems[64];

static int faux           = 0;
static int vrai           = 1;
static int nbChampsActifs = 0;
static int indChampCourant = -1;

static char *labelAnal[] = {"Analyse valide %s:%sZ le %s %s %s",
            "Analysis valid %s:%sZ %s %s %s"};
static char *labelPrev[] = {"Prevision %02d heures valide %s:%sZ le %s %s %s",
            "%02d hour fcst valid %s:%sZ %s %s %s"};
static char *labelDiff[] = {"Differences a %02d heures valides %s:%sZ le %s %s %s",
            "Differences at %02d hours valid %s:%sZ %s %s %s"};
static char *labelClim[] = {"Champ climatologique valide %s:%sZ le %s %s %s",
            "Climatological field valid %s:%sZ %s %s %s"};
static char *labelResidus[] = {"Residus valides %s:%sZ le %s %s %s",
          "Residues valid %s:%sZ %s %s %s"};
static char *labelDefaut[] = {"Champ valide %s:%sZ le %s %s %s",
        "Field valid %s:%sZ %s %s %s" };
static char *labelEtiquette[] = {"Etiquette: %s",
            "Stamp: %s"};
static char *labelSurface[] = {"Niveau: surface",
          "Level : surface"};
static char *lecture[] = { "Lecture: %s-%s-%4d-%3d-%s-%s",
            "Reading: %s-%s-%4d-%3d-%s-%s"};
static char *lectureVents[] = {"Lecture: UU-VV-%s-%4d-%3d-%s-%s",
          "Reading: UU-VV-%s-%4d-%3d-%s-%s" };
static char *lectureVents3D[] = {"Lecture: UU-VV-WW-%s-%4d-%3d-%s-%s",
          "Reading: UU-VV-WW-%s-%4d-%3d-%s-%s" };
/* -------------------------------------------------------------------------------------------------- */

int f77name(read_decode_hyb)(int *iun,char nom[],int *ip2,int *ip3, char etik[],int *date, float *ptop,float *pref,float *rcoef, F2Cl len_nom, F2Cl len_etiket);

int  c_fstinlo(int iun, int *ni, int *nj, int *nk, int date, char etiket[], int ip1, int ip2, int ip3, char typvar[], char nomvar[], int listeCles[], int *nbCles, int nbMaxCles)
{
  int ivalide, ier;

  if (0 != strcmp(nomvar, "DZ"))
      {
      ier = c_fstinl(iun, ni, nj, nk,
        -1, etiket, ip1,
        -1, -1, typvar, nomvar,
        listeCles, nbCles, nbMaxCles);
      }
  else
      {
      ier = c_fstinl(iun, ni, nj, nk,
        -1, etiket, ip1,
        ip2, -1, typvar, nomvar,
        listeCles, nbCles, nbMaxCles);
      }

  ivalide = *nbCles;
  *nbCles = ivalide;

  return 0;
}

/* -------------------------------------------------------------------------------------------------- */

int FldMgrAddChamp(int cle, int iun)
{
  int i, ier, ind;
  int cleTrouvee = False;

  if (nbChampsActifs == NB_MAX_CHAMPS_ACTIFS)
      {
      return TROP_DE_CHAMPS;
      }

  indChampCourant++;
  nbChampsActifs++;
  ind = indChampCourant;

  fmflds[ind].iun = iun;
  fmflds[ind].cle = cle;
  ier = FldMgrGetFstPrm(&fmflds[ind]);

  if (ind > 0)
      {
      switch (fmflds[0].domaine)
        {
        case XY:
        break;

        case XZ:
        case YZ:
            if (fmflds[ind].src.ni != fmflds[0].src.ni ||
          fmflds[ind].src.nj != fmflds[0].src.nj ||
          fmflds[ind].src.nk != fmflds[0].src.nk ||
          fmflds[ind].src.grtyp[0] != fmflds[0].src.grtyp[0] ||
          fmflds[ind].src.ig1 != fmflds[0].src.ig1 || fmflds[ind].src.ig2 != fmflds[0].src.ig2 ||
          fmflds[ind].src.ig3 != fmflds[0].src.ig3 || fmflds[ind].src.ig4 != fmflds[0].src.ig4)
              {
              return CHAMP_NON_SUPERPOSABLE;
              }
            break;

        default:
            break;
        }
      }

  if (fmflds[ind].seqanim.clesAnim != NULL)
      {
      for (i=0; i < fmflds[ind].seqanim.nbFldsAnim; i++)
        {
        if (fmflds[ind].seqanim.clesAnim[i] == cle)
            cleTrouvee = True;
        }
      }

  FldMgrFreeMainFlds(&fmflds[ind]);
  if (!cleTrouvee)
      FldMgrFreeAnimFlds(&fmflds[ind]);

  cleTrouvee = False;
  if (fmflds[ind].coupe.clesNiveaux != NULL)
      {
      for (i=0; i < fmflds[ind].coupe.nbNiveauxCoupe; i++)
        {
        if (fmflds[ind].coupe.clesNiveaux[i] == cle)
                  cleTrouvee = True;
        }
      }

  if (!cleTrouvee)
      FldMgrFreeCoupeFlds(&fmflds[ind]);

  FldMgrProcessChamp(&fmflds[ind]);

  FldMgrSetDiffMinMax(ind);
  return OK;
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrAjusterDimensionsGrille(_Champ *champ)
{
  switch (champ->src.nk)
    {
    case 1:
      champ->domaine = XY;
      break;

    default:
      if (champ->src.ni == 1)
        {
        champ->domaine = YZ;
        champ->dst.ni = champ->src.nj;
        champ->dst.nj = champ->src.nk;
        champ->dst.nk = 1;
        }
      else
        {
        champ->domaine = XZ;
        champ->dst.nj = champ->src.nk;
        champ->src.nk = 1;
        }

      break;
    }


}

/* -------------------------------------------------------------------------------------------------- */
void DictMgrSetFacteurDeConversion(float facteur, int indDict);

void FldMgrDefineDefaultIntervals(_Champ *champ)
{
  static char *menuDeDefaut[] = { "0.0", "0.01", "0.02", "0.05", "0.1", "0.2", "0.5", "1.0", "2.0", "5.0",
              "10.0", "20.0", "50.0", "100.0", "200.0", "500.0"};
  float contourMin, contourMax, intervalles;
  int exposant;
  int i;
  float facteur;
  float inter;
  int npts, un;


  facteur = 1.0;
  npts = champ->src.ni * champ->src.nj * champ->src.nk;
  un = 1;

  f77name(aminmax)(&contourMin, &contourMax, champ->fld, &npts, &un);
  intervalles = contourMax - contourMin;

  exposant = 0;
  if (intervalles != 0.0)
    {
    if (intervalles > 1.0)
      {
      while (intervalles /  facteur >= 10.0)
        {
        facteur *= 10.0;
        exposant++;
        }
      facteur = (float)pow((double)10.0, (double)(exposant));
      }
    else
      {
      while (intervalles / facteur  <= 10.0)
        {
        facteur /= 10.0;
        exposant--;
        }
      facteur = (float)pow((double)10.0, (double)(exposant));
      }

    DictMgrSetIndIntervalleDeDefaut(7, champ->indDict);
    DictMgrSetFacteurDeConversion(facteur, champ->indDict);
    }

  for (i = 0; i < 16; i++)
    {
    sscanf(menuDeDefaut[i], "%f", &inter);
    DictMgrSetIntervallesDeContour(&inter, &un, champ->indDict, i);
    }
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrFreeAllFlds(_Champ *champ)
{
  FldMgrFreeMainFlds(champ);
  FldMgrFreeCoupeFlds(champ);
  FldMgrFreeAnimFlds(champ);
}

/* -------------------------------------------------------------------------------------------------- */

int FldMgrFreeAnimFlds(_Champ *champ)
{
    int i;

    if (champ->seqanim.nbFldsAnim == 0)
        return 0;

    for (i=0; i < champ->seqanim.nbFldsAnim; i++)
      {
      free(champ->seqanim.pdfDates[i]);
      }
    free(champ->seqanim.pdfDates);

    if (champ->natureTensorielle == VECTEUR)
      {
      for (i=0; i < champ->seqanim.nbFldsAnim; i++)
        {
        free(champ->seqanim.animUUs[i]);
                free(champ->seqanim.animVVs[i]);
                free(champ->seqanim.animUVs[i]);
        }

      free(champ->seqanim.animUUs);
      free(champ->seqanim.animVVs);
      free(champ->seqanim.animUVs);

      free(champ->seqanim.animUUmin);
      free(champ->seqanim.animUUmax);
      free(champ->seqanim.animVVmin);
      free(champ->seqanim.animVVmax);
      free(champ->seqanim.animUVmin);
      free(champ->seqanim.animUVmax);
      }
    else
      {
      for (i=0; i < champ->seqanim.nbFldsAnim; i++)
        {
        free(champ->seqanim.animFLDs[i]);
        }
      free(champ->seqanim.animFLDmin);
      free(champ->seqanim.animFLDmax);
      free(champ->seqanim.animFLDs);
      }

    free(champ->seqanim.ip1s);
    free(champ->seqanim.ip2s);
    free(champ->seqanim.ip3s);
    free(champ->seqanim.dates);
    free(champ->seqanim.dt);
    free(champ->seqanim.clesAnim);

    memset((char *)&(champ->seqanim), '\0', sizeof(_SequenceAnimee));
return 0;
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrFreeCoupeFlds(_Champ *champ)
{
  int i;

  champ->coupe.coupeValide = 0;

  if (champ->coupe.nbNiveauxCoupe == 0)
      return;

  free(champ->coupe.x);
  free(champ->coupe.y);
  champ->coupe.x = NULL;
  champ->coupe.y = NULL;

  if (champ->natureTensorielle == SCALAIRE)
    {
    if (champ->coupe.fld3d != NULL)
      {
      for (i=champ->coupe.nbNiveauxCoupe-1; i >= 0; i--)
        {
        free(champ->coupe.fld3d[i]);
        champ->coupe.fld3d[i] = (float *)NULL;
        }
      free(champ->coupe.fld3d);
      }

    if (champ->coupe.fld2d != NULL)
      {
      free(champ->coupe.fld2d);
      champ->coupe.fld2d = NULL;
      }
    }
  else
    {
    if (champ->coupe.uu3d != NULL)
      {
      for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
        {
        free(champ->coupe.uu3d[i]);
        free(champ->coupe.vv3d[i]);
        free(champ->coupe.ww3d[i]);
        champ->coupe.uu3d[i] = NULL;
        champ->coupe.vv3d[i] = NULL;
        champ->coupe.ww3d[i] = NULL;
        }
      free(champ->coupe.uu3d);
      free(champ->coupe.vv3d);
      free(champ->coupe.ww3d);
      champ->coupe.uu3d = NULL;
      champ->coupe.vv3d = NULL;
      champ->coupe.ww3d = NULL;
      }

    if (champ->coupe.ww2d != NULL)
      {
      free(champ->coupe.uvwtang2d);
      free(champ->coupe.uvwtang2d);
      free(champ->coupe.ww2d);
      free(champ->coupe.uvw2d);
      champ->coupe.uvwtang2d = NULL;
      champ->coupe.uvwnorm2d = NULL;
      champ->coupe.uvw2d = NULL;
      champ->coupe.ww2d = NULL;
      }
    }

  if (champ->coupe.montagnes != NULL)
    {
    free( champ->coupe.montagnes);
    free( champ->coupe.ligneMontagnes);
    }

  if (champ->coupe.niveauxCoupe != NULL)
    free(champ->coupe.niveauxCoupe);

  if (champ->coupe.clesNiveaux != NULL)
    free(champ->coupe.clesNiveaux);

  memset((char *)&(champ->coupe), '\0', sizeof(_CoupeVerticale));
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrFreeMainFlds(_Champ *champ)
  {
  if (champ->fld)
    {
    free(champ->fld);
    champ->fld = NULL;
    }

  if (champ->fld_orig)
    {
    free(champ->fld_orig);
    champ->fld_orig = NULL;
    }

  if (champ->x)
    {
    free(champ->x);
    champ->x = NULL;
    }

  if (champ->y)
    {
    free(champ->y);
    champ->y = NULL;
    }

  if (champ->other)
    {
    free(champ->other);
    champ->other = NULL;
    }

  if (champ->module)
    {
    free(champ->uu);
    free(champ->vv);
    free(champ->module);
    champ->module = NULL;
    }
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrFreeTimeAnimationSeq()
{
    int i;

    for (i=0; i < nbChampsActifs; i++)
        {
        FldMgrFreeAnimFlds(&fmflds[i]);
        }
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrFreeVerticalXSection()
{
    int i;

    for (i=0; i < nbChampsActifs; i++)
        {
        FldMgrFreeCoupeFlds(&fmflds[i]);
        }
}

int FldMgrComparerDates();

/* -------------------------------------------------------------------------------------------------- */

int string_compare(const void *str1, const void *str2)
{
  return strcmp((char *) str1, (char *) str2);
}

void FldMgrGetAllDates(char dates[][16], int *nbDates)
{

    _Champ bidon;
    char allDates[2000][16], lastpdfdate[16], listeDates[2000][16];
    int allIP2s[2000], allKeys[2000],ier, i, n;
    int ind, listeCles[2000],nbCles, nbTotalCles, ni, nj, nk;
    int nbMaxCles = 2000;

    n = 0;
    nbTotalCles = 0;
    while (n < nbChampsActifs)
        {
        ier = c_fstinlo(fmflds[n].iun, &ni, &nj, &nk,
                        fmflds[n].dateo, fmflds[n].etiket, fmflds[n].ip1,
                        fmflds[n].ip2, fmflds[n].ip3, fmflds[n].typvar, fmflds[n].nomvar,
                        listeCles, &nbCles, nbMaxCles);
        FldMgrVerConsistanceGrilles(fmflds[n], listeCles, &nbCles);

        for (i=nbTotalCles; i < (nbTotalCles + nbCles); i++)
            {
            allKeys[i] = listeCles[i - nbTotalCles];
            }
        nbTotalCles += nbCles;
        n++;
        }

    for (i=0; i < nbTotalCles; i++)
        {
        bidon.cle = allKeys[i];
        FldMgrGetFstPrm(&bidon);
        allIP2s[i]  = bidon.ip2;

        FldMgrCalcPDFDatev(bidon.pdfdatev,&bidon.datev,bidon.dateo,bidon.deet,bidon.npas,bidon.ip2);
        strcpy(allDates[i],bidon.pdfdatev);
        }

    qsort(allDates, nbTotalCles, 16, string_compare);

    strcpy(lastpdfdate,"");
    i = 0;
    ind = 0;
    for (i=0; i < nbTotalCles; i++)
        {
        if (0 != strcmp(allDates[i],lastpdfdate))
            {
            strcpy(listeDates[ind],allDates[i]);
            strcpy(lastpdfdate,allDates[i]);
            ind++;
            }
        }

    for (i=0; i < ind; i++)
        strcpy(dates[i],listeDates[i]);
    *nbDates = ind;

}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrGetChamp(_Champ **champ, int indice)
{
    *champ = &fmflds[indice];
}

/* -------------------------------------------------------------------------------------------------- */

int FldMgrGetFstPrm(_Champ *champ)
{
    int ier;
    strcpy(champ->nomvar,"    ");
    strcpy(champ->typvar,"  ");
    strcpy(champ->etiket, "            ");
    strcpy(champ->src.grtyp, " ");
    ier = c_fstprm(champ->cle, &champ->dateo, &champ->deet, &champ->npas,
                  &champ->src.ni, &champ->src.nj, &champ->src.nk, &champ->nbits,
                  &champ->datyp, &champ->ip1, &champ->ip2, &champ->ip3,
                  champ->typvar, champ->nomvar, champ->etiket, champ->src.grtyp,
                  &champ->src.ig1, &champ->src.ig2, &champ->src.ig3, &champ->src.ig4,
                  &champ->swa, &champ->lng, &champ->dltf, &champ->ubc,
                  &champ->extra1, &champ->extra2, &champ->extra3);
    champ->rnpas = champ->npas;
    nettoyer(champ->nomvar);
    nettoyer(champ->typvar);
    nettoyer(champ->etiket);
/*    strncat(champ->etiket, "            ", 12-strlen(champ->etiket));*/
    FldMgrAjusterDimensionsGrille(champ);
    return ier;
}



/* -------------------------------------------------------------------------------------------------- */

int FldMgrGetNbChampsActifs()
{
    return nbChampsActifs;
}


/* -------------------------------------------------------------------------------------------------- */

void FldMgrInitAnimCles(_Champ *champ, char dates[][16], int nbDates)
{
    int i, j, ier;
    int ni, nj, nk;

    int nbClesChamp, listeCles[2000];
    int nbMaxCles = 2000;
    int trouve;
    _Champ bidon;

    for (i=0; i < nbDates; i++)
        {
        champ->seqanim.clesAnim[i] = -1;
        }


    ier = c_fstinlo(champ->iun, &ni, &nj, &nk,
                    champ->dateo, champ->etiket, champ->ip1,
                    champ->ip2, champ->ip3, champ->typvar, champ->nomvar,
                    listeCles, &nbClesChamp, nbMaxCles);
    FldMgrVerConsistanceGrilles(*champ, listeCles, &nbClesChamp);

    for (i=0; i < nbClesChamp; i++)
        {
        bidon.cle = listeCles[i];
        FldMgrGetFstPrm(&bidon);
        FldMgrCalcPDFDatev(bidon.pdfdatev,&(bidon.datev),bidon.dateo,bidon.deet,bidon.npas,bidon.ip2);

        j = 0;
        trouve = False;
        while (!trouve && j < nbDates)
            {
            if (0 == strcmp(dates[j],bidon.pdfdatev))
                trouve = True;
            j++;
            }

        if (trouve)
            champ->seqanim.clesAnim[j-1] = listeCles[i];
        }
}


/* -------------------------------------------------------------------------------------------------- */

void FldMgrInitAnimFlds(_Champ *champ, int nbCles)
{
    int k;

    champ->seqanim.nbFldsAnim = nbCles;
    champ->seqanim.ip1s     = (int *)    calloc(nbCles, sizeof(int));
    champ->seqanim.ip2s     = (int *)    calloc(nbCles, sizeof(int));
    champ->seqanim.ip3s     = (int *)    calloc(nbCles, sizeof(int));
    champ->seqanim.dates    = (int *)    calloc(nbCles, sizeof(int));
    champ->seqanim.dt    = (double *)    calloc(nbCles, sizeof(double));
    champ->seqanim.pdfDates    = (char **)    calloc(nbCles, sizeof(char *));
    champ->seqanim.clesAnim = (int *)    calloc(nbCles, sizeof(int));

    for (k=0; k < nbCles; k++)
        {
        champ->seqanim.pdfDates[k] = (char *) calloc(16,sizeof(char));
        }

    if (champ->natureTensorielle == VECTEUR)
        {
        champ->seqanim.animUUs = (float **) calloc(nbCles, sizeof(float *));
        champ->seqanim.animVVs = (float **) calloc(nbCles, sizeof(float *));
        champ->seqanim.animUVs = (float **) calloc(nbCles, sizeof(float *));

        champ->seqanim.animUUmin  = (float *)  calloc(nbCles, sizeof(float));
        champ->seqanim.animUUmax  = (float *)  calloc(nbCles, sizeof(float));

        champ->seqanim.animVVmin  = (float *)  calloc(nbCles, sizeof(float));
        champ->seqanim.animVVmax  = (float *)  calloc(nbCles, sizeof(float));

        champ->seqanim.animUVmin  = (float *)  calloc(nbCles, sizeof(float));
        champ->seqanim.animUVmax  = (float *)  calloc(nbCles, sizeof(float));

        for (k=0; k < nbCles; k++)
            {
            champ->seqanim.animUUs[k] = (float *) calloc(champ->dst.ni*champ->dst.nj*champ->dst.nk,sizeof(float));
            champ->seqanim.animVVs[k] = (float *) calloc(champ->dst.ni*champ->dst.nj*champ->dst.nk,sizeof(float));
            champ->seqanim.animUVs[k] = (float *) calloc(champ->dst.ni*champ->dst.nj*champ->dst.nk,sizeof(float));
            }
        }
    else
        {
        champ->seqanim.animFLDs = (float **) calloc(nbCles, sizeof(float *));
        champ->seqanim.animFLDmin  = (float *)  calloc(nbCles, sizeof(float));
        champ->seqanim.animFLDmax  = (float *)  calloc(nbCles, sizeof(float));

        for (k=0; k < nbCles; k++)
            {
            champ->seqanim.animFLDs[k] = (float *) calloc(champ->dst.ni*champ->dst.nj*champ->dst.nk,sizeof(float));
            }

        }
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrInitChamps()
{
    int i;

    for (i=0; i < NB_MAX_CHAMPS_ACTIFS; i++)
        memset((char *) &fmflds[i], '\0', sizeof(_Champ));
}

/* -------------------------------------------------------------------------------------------------- */

int FldMgrLoadTimeAnimationSeq()
{
  int langue;
  int annulationDemandee = FALSE;
  int nbTotalChamps, it;

  char pdfdatev[24];
  double dt1, dt2, dt3;
  int date1, date2;

  int i, k, n;
  char texteLecture[128];
  int champsACharger[NB_MAX_CHAMPS_ACTIFS];
  int o;

  _Champ tmpchamp;
  char dates[2000][16];
  int nbDates;

  if (nbChampsActifs == 0)
      return -1;

  bzero(&tmpchamp, sizeof(_Champ));
  langue = c_getulng();
  FldMgrGetAllDates(dates, &nbDates);

  if (nbDates == 1)
      {
      return PAS_ASSEZ_DE_PERIODES;
      }

  nbTotalChamps = 0;
  for (n=0; n < nbChampsActifs; n++)
      {
      if (fmflds[n].seqanim.animFLDs == NULL && fmflds[n].seqanim.animUUs == NULL)
        {
        champsACharger[n] = 1;
        FldMgrInitAnimFlds(&fmflds[n], nbDates);
        FldMgrInitAnimCles(&fmflds[n], dates, nbDates);
        if (n == 0)
            {
            k = 0;
            FldMgrCalcPDFDatev(fmflds[0].pdfdatev,&(fmflds[0].datev),fmflds[0].dateo,fmflds[0].deet,fmflds[0].npas,fmflds[0].ip2);
            while (0 != strcmp(fmflds[0].pdfdatev,dates[k]))
              k++;
            fmflds[n].seqanim.indChampCourant = k;
            }
        else
            {
            fmflds[n].seqanim.indChampCourant = fmflds[0].seqanim.indChampCourant;
            }
        nbTotalChamps += nbDates;
        }
      else
        {
        champsACharger[n] = 0;
        }
      }

  it = 0;
  o = NO_OP;
  for (n=0; n < nbChampsActifs; n++)
      {
      if (champsACharger[n] == 1)
        {
        for (i=0; i < fmflds[n].seqanim.nbFldsAnim; i++)
          {
          tmpchamp = fmflds[n];
          tmpchamp.cle = fmflds[n].seqanim.clesAnim[i];
          if (tmpchamp.cle >= 0)
            {
            FldMgrGetFstPrm(&tmpchamp);
            FldMgrProcessChamp(&tmpchamp);
            if (tmpchamp.natureTensorielle == VECTEUR)
              {
              FldMgrCalcPDFDatev(pdfdatev,&(tmpchamp.datev),tmpchamp.dateo,tmpchamp.deet,tmpchamp.npas,tmpchamp.ip2);
              sprintf(texteLecture, lectureVents[langue],
                tmpchamp.typvar, tmpchamp.ip1,
                tmpchamp.ip2, pdfdatev, tmpchamp.etiket);
              fmflds[n].seqanim.animUUmin[i] = tmpchamp.uumin[o];
              fmflds[n].seqanim.animUUmax[i] = tmpchamp.uumax[o];
              fmflds[n].seqanim.animVVmin[i] = tmpchamp.vvmin[o];
              fmflds[n].seqanim.animVVmax[i] = tmpchamp.vvmax[o];
              fmflds[n].seqanim.animUVmin[i] = tmpchamp.uvmin[o];
              fmflds[n].seqanim.animUVmax[i] = tmpchamp.uvmax[o];

              fmflds[n].uumin[o] = fmflds[n].uumin[o] > tmpchamp.uumin[o] ?  tmpchamp.uumin[o] : fmflds[n].uumin[o];
              fmflds[n].uumax[o] = fmflds[n].uumax[o] < tmpchamp.uumax[o] ?  tmpchamp.uumax[o] : fmflds[n].uumax[o];
              fmflds[n].vvmin[o] = fmflds[n].vvmin[o] > tmpchamp.vvmin[o] ?  tmpchamp.vvmin[o] : fmflds[n].vvmin[o];
              fmflds[n].vvmax[o] = fmflds[n].vvmax[o] < tmpchamp.vvmax[o] ?  tmpchamp.vvmax[o] : fmflds[n].vvmax[o];
              fmflds[n].uvmin[o] = fmflds[n].uvmin[o] > tmpchamp.uvmin[o] ?  tmpchamp.uvmin[o] : fmflds[n].uvmin[o];
              fmflds[n].uvmax[o] = fmflds[n].uvmax[o] < tmpchamp.uvmax[o] ?  tmpchamp.uvmax[o] : fmflds[n].uvmax[o];

              memcpy((char *)fmflds[n].seqanim.animUUs[i], (char *)tmpchamp.uu,tmpchamp.dst.ni*tmpchamp.dst.nj*sizeof(float));
              memcpy((char *)fmflds[n].seqanim.animVVs[i], (char *)tmpchamp.vv,tmpchamp.dst.ni*tmpchamp.dst.nj*sizeof(float));
              memcpy((char *)fmflds[n].seqanim.animUVs[i], (char *)tmpchamp.module,tmpchamp.dst.ni*tmpchamp.dst.nj*sizeof(float));
              }
            else
              {
              FldMgrCalcPDFDatev(pdfdatev,&(tmpchamp.datev),tmpchamp.dateo,tmpchamp.deet,tmpchamp.npas,tmpchamp.ip2);
              sprintf(texteLecture, lecture[langue],
                tmpchamp.nomvar, tmpchamp.typvar, tmpchamp.ip1,
                tmpchamp.ip2, pdfdatev, tmpchamp.etiket);
              fmflds[n].seqanim.animFLDmin[i] = tmpchamp.fldmin[o];
              fmflds[n].seqanim.animFLDmax[i] = tmpchamp.fldmax[o];
              fmflds[n].fldmin[o] = fmflds[n].fldmin[o] > tmpchamp.fldmin[o] ?  tmpchamp.min : fmflds[n].fldmin[o];
              fmflds[n].fldmax[o] = fmflds[n].fldmax[o] < tmpchamp.fldmax[o] ?  tmpchamp.max : fmflds[n].fldmax[o];

              memcpy((char *)fmflds[n].seqanim.animFLDs[i], (char *)tmpchamp.fld,tmpchamp.dst.ni*tmpchamp.dst.nj*sizeof(float));
              }

            FldMgrMessageChargementFichiers(texteLecture, it, nbTotalChamps);

            fmflds[n].seqanim.ip1s[i]  = tmpchamp.ip1;
            fmflds[n].seqanim.ip2s[i]  = tmpchamp.ip2;
            fmflds[n].seqanim.ip3s[i]  = tmpchamp.ip3;

            dt2 = (double) ((tmpchamp.deet * tmpchamp.npas) / 3600.0);
            dt1 = (double) ((fmflds[n].deet * fmflds[n].npas) / 3600.0);
            f77name(incdatr)(&date1, &fmflds[n].dateo, &dt1);
            f77name(incdatr)(&date2, &tmpchamp.dateo, &dt2);
            f77name(difdatr)(&date2, &date1,  &dt3);
            fmflds[n].seqanim.ip2s[i] = ROUND((float)dt3);
            fmflds[n].seqanim.dates[i] = date2;
            FldMgrFreeMainFlds(&tmpchamp);
            }
          annulationDemandee = c_wglanul();
          if (annulationDemandee)
            {
            for (k=0; k <= n; k++)
          FldMgrFreeAnimFlds(&fmflds[k]);
            return CHARGEMENT_ANNULE;
            }
          it++;
          }

        fmflds[n].min = fmflds[n].fldmin[o];
        fmflds[n].max = fmflds[n].fldmax[o];
        for (i=0; i < fmflds[n].seqanim.nbFldsAnim; i++)
          {
          f77name(difdatr)(&(fmflds[n].seqanim.dates[i]), &(fmflds[n].seqanim.dates[0]),  &dt3);
          fmflds[n].seqanim.dt[i] = dt3;
          }
        }
      else
        {
        /*FldMgrReorgAnimCles(&fmflds[n], dates, nbDates); */
        }
      FldMgrSetAnimDiffMinMax(n);
      }
  AfficherMessageInfoStandard();
  return OK;
  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrLoadVerticalXSection()
{
  int listeCles[2000];
  int i, n, ier, nbCles;
  int nbMaxCles = 2000;
  int ni, nj, nk;
  int datev;
  char texteLecture[132];
  int langue;
  int annulationDemandee, message;
  int it, nbTotalCles, indFautif, found;
  int champsACharger[NB_MAX_CHAMPS_ACTIFS], correctionNecessaire;
  double deltaT;
  float opmin[5],opmax[5],p0min,p0max;

  _Champ tmpchamp;
  int j;
  int versPression = -1;
  int kind;

  nbTotalCles = 0;
  bzero(&tmpchamp, sizeof(_Champ));
  langue = c_getulng();

  for (n=0; n < nbChampsActifs; n++)
    {
    if ((fmflds[n].fld == NULL && fmflds[n].uu == NULL) || fmflds[n].cle < 0)
    return CHAMP_INEXISTANT;

    langue = c_getulng();
    annulationDemandee = c_wglanul();
    if (fmflds[n].coupe.coupeValide == 1)
      {
      champsACharger[n] = 0;
      }
    else
      {
      deltaT = fmflds[n].npas*fmflds[n].deet/3600.0;
      f77name(incdatr)(&datev, &fmflds[n].dateo, &deltaT);
      ier = c_fstinl(fmflds[n].iun, &ni, &nj, &nk,
                      datev, fmflds[n].etiket, -1,
                      fmflds[n].ip2, fmflds[n].ip3, fmflds[n].typvar, fmflds[n].nomvar,
                      listeCles, &nbCles, nbMaxCles);

      if (nbCles == 0)  /** patche pour essayer quand meme de charger des champs **/
        {
        ier = c_fstinl(fmflds[n].iun, &ni, &nj, &nk,
                        -1, fmflds[n].etiket, -1,
                        fmflds[n].ip2, fmflds[n].ip3, fmflds[n].typvar, fmflds[n].nomvar,
                        listeCles, &nbCles, nbMaxCles);
        }


      FldMgrVerConsistanceGrilles(fmflds[n], listeCles, &nbCles);
      FldMgrVerConsistanceNiveaux(fmflds[n], listeCles, &nbCles);
      FldMgrTrierClesSelonIP1(listeCles, &nbCles);

      if (nbCles > 1)
        {
        champsACharger[n] = 1;
        nbTotalCles += nbCles;
        fmflds[n].coupe.cleRef    = fmflds[n].cle;
        fmflds[n].coupe.nbNiveauxCoupe = nbCles;
        fmflds[n].coupe.niveauxCoupe = (float *) malloc(nbCles *sizeof(float));
        fmflds[n].coupe.clesNiveaux= (int *) malloc(nbCles * sizeof(int));

        for (i=0; i < nbCles; i++)
          {
          fmflds[n].coupe.clesNiveaux[i] = listeCles[i];
          tmpchamp.cle = fmflds[n].coupe.clesNiveaux[i];
          tmpchamp.iun = fmflds[n].iun;
          tmpchamp.travailEnCours = COUPE;
          FldMgrGetFstPrm(&tmpchamp);
          f77name(convip_plus)(&tmpchamp.ip1, &fmflds[n].coupe.niveauxCoupe[i], &kind, &versPression, NULL, &faux,(F2Cl) 0);
          }
        }
      }
    }

  message = FldMgrVerConsistanceNiveauxChamps(&indFautif);
  if (message != 0)
    {
    for (i=nbChampsActifs-1; i >= indFautif; i--)
      {
      FldMgrRemoveChamp(i);
      }
    return message;
    }

  it = 0;
  for (n=0; n < nbChampsActifs; n++)
    {
    if (champsACharger[n] == 1)
      {
      if (fmflds[n].coupe.montagnes != NULL)
        {
        free(fmflds[n].coupe.montagnes);
        }

      fmflds[n].coupe.niMaxCoupe = 1+ (int)(sqrt(fmflds[n].dst.ni*fmflds[n].dst.ni+fmflds[n].dst.nj*fmflds[n].dst.nj));
      fmflds[n].coupe.x =              (float *) malloc(fmflds[n].coupe.niMaxCoupe* sizeof(float));
      fmflds[n].coupe.y =              (float *) malloc(fmflds[n].coupe.niMaxCoupe* sizeof(float));
      fmflds[n].coupe.montagnes =      (float *) malloc(fmflds[n].dst.ni * fmflds[n].dst.nj *sizeof(float));
      fmflds[n].coupe.ligneMontagnes = (float *) malloc(fmflds[n].coupe.niMaxCoupe* sizeof(float));
      switch(fmflds[n].natureTensorielle)
        {
        case SCALAIRE:
        fmflds[n].coupe.fld2d =     (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        break;

        case VECTEUR:
        fmflds[n].coupe.uu2d =      (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        fmflds[n].coupe.vv2d =      (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        fmflds[n].coupe.ww2d =      (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        fmflds[n].coupe.uvwtang2d = (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        fmflds[n].coupe.uvwnorm2d = (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        fmflds[n].coupe.uvw2d =     (float *) malloc(fmflds[n].coupe.niMaxCoupe * fmflds[n].coupe.nbNiveauxCoupe * sizeof(float));
        break;
        }

      if (fmflds[n].natureTensorielle == VECTEUR)
        {
        fmflds[n].coupe.uu3d  = (float **) malloc(fmflds[n].coupe.nbNiveauxCoupe * sizeof(float *));
        fmflds[n].coupe.vv3d  = (float **) malloc(fmflds[n].coupe.nbNiveauxCoupe * sizeof(float *));
        fmflds[n].coupe.ww3d  = (float **) malloc(fmflds[n].coupe.nbNiveauxCoupe * sizeof(float *));
        }
      else
        {
        fmflds[n].coupe.fld3d  = (float **) malloc(fmflds[n].coupe.nbNiveauxCoupe * sizeof(float *));
        }


      if (fmflds[n].coordonneeVerticale == SIGMA || fmflds[n].coordonneeVerticale == HYBRIDE)
        {
        FldMgrLoad_HY(n);
        ier = GetSurfacePressure(&fmflds[n]);
        if (ier >= 0)
          {
          f77name(aminmax)(&p0min,&p0max,fmflds[n].coupe.montagnes,&(fmflds[n].dst.ni),&(fmflds[n].dst.nj));
          }
        fmflds[n].coupe.niveauPresMin = fmflds[n].coupe.ptop;
        fmflds[n].coupe.niveauPresMax = p0max;
        }

      correctionNecessaire = 0;
      i = fmflds[n].coupe.nbNiveauxCoupe -1;
      found = -1;
      while (i >= 0 && found == -1)
        {
        tmpchamp.cle = fmflds[n].coupe.clesNiveaux[i];
        tmpchamp.iun = fmflds[n].iun;
        FldMgrGetFstPrm(&tmpchamp);
        if (tmpchamp.ip1 == 2000)
          {
          correctionNecessaire=1;
          found = 1;
          }
        i--;
        }

      for (i=0; i < fmflds[n].coupe.nbNiveauxCoupe; i++)
        {
        tmpchamp.cle = fmflds[n].coupe.clesNiveaux[i];
        tmpchamp.iun = fmflds[n].iun;
        FldMgrGetFstPrm(&tmpchamp);
        FldMgrProcessChamp(&tmpchamp);
        if (fmflds[n].natureTensorielle == VECTEUR)
          {
          CheckForWW(&tmpchamp);
          }

        if (fmflds[n].coordonneeVerticale == SIGMA || fmflds[n].coordonneeVerticale == HYBRIDE)
          {
          if (correctionNecessaire == 1)
            {
            /*fprintf(stderr, "Avant: %f", tmpchamp.niveau);*/
            tmpchamp.niveau = tmpchamp.niveau*(1.0-fmflds[n].coupe.ptop/fmflds[n].coupe.pref) + fmflds[n].coupe.ptop/fmflds[n].coupe.pref;
            /*fprintf(stderr, "\tApres: %f\n", tmpchamp.niveau);*/
            }
          }

        if (i == 0)
          {
          fmflds[n].coupe.niveauMin = tmpchamp.niveau;
          fmflds[n].coupe.niveauMax = tmpchamp.niveau;
          }
        else
          {
          fmflds[n].coupe.niveauMin = (fmflds[n].coupe.niveauMin < tmpchamp.niveau) ? fmflds[n].coupe.niveauMin : tmpchamp.niveau;
          fmflds[n].coupe.niveauMax = (fmflds[n].coupe.niveauMax > tmpchamp.niveau) ? fmflds[n].coupe.niveauMax : tmpchamp.niveau;
          }

        fmflds[n].coupe.niveauxCoupe[i] = (float)tmpchamp.niveau;

        if (fmflds[n].natureTensorielle == VECTEUR)
          {
          sprintf(texteLecture, lectureVents3D[langue], tmpchamp.typvar, tmpchamp.ip1, tmpchamp.ip2, tmpchamp.pdfdatev, tmpchamp.etiket);

          if (i==0)
            {
            fmflds[n].coupe.UUmin3d[0] = tmpchamp.uumin[0];
            fmflds[n].coupe.UUmax3d[0] = tmpchamp.uumax[0];
            fmflds[n].coupe.VVmin3d[0] = tmpchamp.vvmin[0];
            fmflds[n].coupe.VVmax3d[0] = tmpchamp.vvmax[0];
            fmflds[n].coupe.WWmin3d[0] = tmpchamp.wwmin[0];
            fmflds[n].coupe.WWmax3d[0] = tmpchamp.wwmax[0];
            }
          else
            {
            fmflds[n].coupe.UUmin3d[0] = (fmflds[n].coupe.UUmin3d[0]  < tmpchamp.uumin[0]) ? fmflds[n].coupe.UUmin3d[0] : tmpchamp.uumin[0];
            fmflds[n].coupe.UUmax3d[0] = (fmflds[n].coupe.UUmax3d[0]  > tmpchamp.uumax[0]) ? fmflds[n].coupe.UUmax3d[0] : tmpchamp.uumax[0];

            fmflds[n].coupe.VVmin3d[0] = (fmflds[n].coupe.VVmin3d[0]  < tmpchamp.vvmin[0]) ? fmflds[n].coupe.VVmin3d[0] : tmpchamp.vvmin[0];
            fmflds[n].coupe.VVmax3d[0] = (fmflds[n].coupe.VVmax3d[0]  > tmpchamp.vvmax[0]) ? fmflds[n].coupe.VVmax3d[0] : tmpchamp.vvmax[0];

            fmflds[n].coupe.WWmin3d[0] = (fmflds[n].coupe.WWmin3d[0]  < tmpchamp.wwmin[0]) ? fmflds[n].coupe.WWmin3d[0] : tmpchamp.wwmin[0];
            fmflds[n].coupe.WWmax3d[0] = (fmflds[n].coupe.WWmax3d[0]  > tmpchamp.wwmax[0]) ? fmflds[n].coupe.WWmax3d[0] : tmpchamp.wwmax[0];
            }

          fmflds[n].coupe.uu3d[i] = tmpchamp.uu;
          fmflds[n].coupe.vv3d[i] = tmpchamp.vv;
          fmflds[n].coupe.ww3d[i] = tmpchamp.ww;
          }
        else
          {
          sprintf(texteLecture, lecture[langue],
            tmpchamp.nomvar, tmpchamp.typvar, tmpchamp.ip1,
            tmpchamp.ip2, tmpchamp.pdfdatev, tmpchamp.etiket);

          if (i==0)
            {
            fmflds[n].coupe.FLDmin3d[0] = tmpchamp.fldmin[0];
            fmflds[n].coupe.FLDmax3d[0] = tmpchamp.fldmax[0];
            }
          else
            {
            fmflds[n].coupe.FLDmin3d[0] = (fmflds[n].coupe.FLDmin3d[0]  < tmpchamp.fldmin[0]) ? fmflds[n].coupe.FLDmin3d[0] : tmpchamp.fldmin[0];
            fmflds[n].coupe.FLDmax3d[0] = (fmflds[n].coupe.FLDmax3d[0]  > tmpchamp.fldmax[0]) ? fmflds[n].coupe.FLDmax3d[0] : tmpchamp.fldmax[0];
            }
          fmflds[n].coupe.fld3d[i] = (float *)malloc(sizeof(float)*fmflds[n].dst.ni*fmflds[n].dst.nj);
          memcpy(fmflds[n].coupe.fld3d[i], tmpchamp.fld, sizeof(float)*fmflds[n].dst.ni*fmflds[n].dst.nj);
          free(tmpchamp.fld);
          free(tmpchamp.fld_orig);
          }


          FldMgrMessageChargementFichiers(texteLecture, it, nbTotalCles);


          annulationDemandee = c_wglanul();
          if (annulationDemandee)
            {
            FldMgrFreeCoupeFlds(&fmflds[n]);
            return CHARGEMENT_ANNULE;
            }
          it++;
          }

      if (fmflds[n].natureTensorielle == VECTEUR && kind == METRES)
        {
        CalcWWForZCoord(&fmflds[n]);
        f77name(aminmax)(&fmflds[n].coupe.WWmin3d[NO_OP], &fmflds[n].coupe.WWmax3d[NO_OP],
            fmflds[n].coupe.ww3d[0], &fmflds[n].dst.ni,&fmflds[n].dst.nj);

      for (j=0; j < fmflds[n].coupe.nbNiveauxCoupe;j++)
        {
        f77name(aminmax)(&opmin[NO_OP], &opmax[NO_OP],fmflds[n].coupe.ww3d[j], &fmflds[n].dst.ni,&fmflds[n].dst.nj);
        fmflds[n].coupe.WWmin3d[0] = (fmflds[n].coupe.WWmin3d[0]  < opmin[0]) ? fmflds[n].coupe.WWmin3d[0] : opmin[0];
        fmflds[n].coupe.WWmax3d[0] = (fmflds[n].coupe.WWmax3d[0]  > opmax[0]) ? fmflds[n].coupe.WWmax3d[0] : opmax[0];
        }
      }

      if (n == (nbChampsActifs-1))
        {
        RedessinerFenetreAffichage();
        }
      fmflds[n].coupe.coupeValide = 1;
      }

    if (fmflds[n].natureTensorielle != SCALAIRE)
      {
      if (champsACharger[n] == 1)
        {
        FldMgrRescaleWW(n,WindMgrGetEchelleWW(), 1);
        FldMgrCalcMinMaxUVW(n);
        FldMgrSetCoupeMinMax(n);
        FldMgrSetDiffCoupeMinMax(n);
        }
      }

    }
  return 0;
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrMessageChargementFichiers(char texte[], int i, int n)
{
  char tmptexte[128];
  int largeurFenetre, hauteurFenetre;
  int largeurRectangle, hauteurRectangle, largeurTexte, hauteurTexte;
  int idebut, jdebut, ifin, jfin, imilieu;
  float niveauReservoir;
  int lng;

  lng = c_getulng();
  niveauReservoir = (float)(i+1)/(float)n;

  c_wglgwz(&largeurFenetre, &hauteurFenetre);
  c_wglfsz(17);
  largeurRectangle = c_wglwsi(texte, strlen(texte));
  largeurTexte = largeurRectangle;
  hauteurTexte = c_wglhsi(texte, strlen(texte));
  hauteurRectangle = 17 * hauteurTexte;

  idebut = (largeurFenetre - largeurRectangle) / 2;
  ifin   = idebut + largeurTexte;
  imilieu = (idebut + ifin) / 2;
  jdebut = (hauteurFenetre - hauteurRectangle) / 2;
  jfin   = jdebut + hauteurRectangle;

  c_wgllwi(2);

  if (i==0)
      {
      c_wglcol(NOIR);
      c_wglrfi(idebut-15, jdebut-15, ifin+15, jfin+15);

      c_wglcol(BLANC);
      c_wglrli(idebut-10, jdebut-10, ifin+10, jfin+10);
      c_wglrli(idebut-15, jdebut-15, ifin+15, jfin+15);

      if (lng == 0)
  strcpy(tmptexte, "Chargement des donnees...");
      else
  strcpy(tmptexte, "Loading data...");

      largeurTexte = c_wglwsi(tmptexte, strlen(tmptexte));
      c_wglpsi(imilieu - largeurTexte / 2, jfin - hauteurTexte, tmptexte, strlen(tmptexte), 17, 0, 0);

      if (lng == 0)
  strcpy(tmptexte, "Un instant...");
      else
  strcpy(tmptexte, "Please wait...");

      largeurTexte = c_wglwsi(tmptexte, strlen(tmptexte));
      c_wglpsi(imilieu - largeurTexte / 2, jfin - 3  * hauteurTexte, tmptexte, strlen(tmptexte), 17, 0, 0);
      }
  else
      {
      c_wglcol(NOIR);
      c_wglrfi(idebut-9, jfin - 11  * hauteurTexte,
        ifin  +9, jfin - 3   * hauteurTexte);
      c_wglcol(BLANC);
      }

  largeurTexte = c_wglwsi(texte, strlen(texte));
  c_wglpsi(imilieu - largeurTexte / 2, jfin - 5  * hauteurTexte, texte, strlen(texte), 17, 0, 0);

  c_wglrfi(imilieu - largeurTexte / 2, jfin - 10  * hauteurTexte,
      imilieu + largeurTexte / 2, jfin - 6   * hauteurTexte);

  c_wglcol(NOIR);
  c_wglrfi(imilieu - largeurTexte / 2, jfin - 10  * hauteurTexte,
      imilieu - largeurTexte / 2 + (int)(niveauReservoir * largeurTexte), jfin - 6   * hauteurTexte);

  c_wglcol(BLANC);
  c_wgllwi(3);
  c_wglrli(imilieu - largeurTexte / 2, jfin - 10  * hauteurTexte,
      imilieu + largeurTexte / 2, jfin - 6   * hauteurTexte);

  if (lng == 0)
      sprintf(tmptexte, "%4d sur %4d", i+1, n);
  else
      sprintf(tmptexte, "%4d of %4d loaded", i+1, n);

  largeurTexte = c_wglwsi(tmptexte, strlen(tmptexte));
  c_wglpsi(imilieu - largeurTexte / 2, jfin - 11  * hauteurTexte, tmptexte, strlen(tmptexte), 17, 0, 0);

  if (lng == 0)
      strcpy(tmptexte, "Pour annuler le chargement,");
  else
      strcpy(tmptexte, "To cancel data load,");
  largeurTexte = c_wglwsi(tmptexte, strlen(tmptexte));
  c_wglpsi(imilieu - largeurTexte / 2, jfin - 14  * hauteurTexte, tmptexte, strlen(tmptexte), 17, 0, 0);

  if (lng == 0)
      strcpy(tmptexte, "allez dans le \"Tableau de bord\" et");
  else
      strcpy(tmptexte, "go in the \"Control Panel\" and");

  largeurTexte = c_wglwsi(tmptexte, strlen(tmptexte));
  c_wglpsi(imilieu - largeurTexte / 2, jfin - 15  * hauteurTexte, tmptexte, strlen(tmptexte), 17, 0, 0);

  if (lng == 0)
      strcpy(tmptexte, "cliquez sur le bouton \"STOP\"");
  else
      strcpy(tmptexte, "clic the \"STOP\" button");
  largeurTexte = c_wglwsi(tmptexte, strlen(tmptexte));
  c_wglpsi(imilieu - largeurTexte / 2, jfin - 16  * hauteurTexte, tmptexte, strlen(tmptexte), 17, 0, 0);

  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrPreparerCoupe(_Champ *champ, float *rx1, float *ry1, float *rx2, float *ry2)
{
  int i,j;
  int i1, j1, i2, j2;
  float dx,dy;
  float fx1, fx2, fy1, fy2;
  float *vals, *uuvals,*vvvals,*wwvals;
  float x1, y1, x2, y2, module;
  int ind;
  float angleCoupe,dirvent;
  int recInterpolationLevel;


  x1 = *rx1; x2 = *rx2; y1 = *ry1; y2 = *ry2;

  if (champ->natureTensorielle == SCALAIRE)
    {
    if (champ->coupe.fld3d == NULL)
      {
      return 0;
      }
    }
  else
    {
    if (champ->coupe.uu3d == NULL)
      {
      return 0;
      }
    }

  recInterpolationLevel = AttrMgrGetInterpolationLevel();
  x1 = (x1 < 1.0) ? 1.0 : (x1 > (float)champ->dst.ni ) ? (float) champ->dst.ni : x1;
  x2 = (x2 < 1.0) ? 1.0 : (x2 > (float)champ->dst.ni ) ? (float) champ->dst.ni : x2;
  y1 = (y1 < 1.0) ? 1.0 : (y1 > (float)champ->dst.nj ) ? (float) champ->dst.nj : y1;
  y2 = (y2 < 1.0) ? 1.0 : (y2 > (float)champ->dst.nj ) ? (float) champ->dst.nj : y2;

  i1 = 1; j1 = 1;
  i2 = champ->dst.ni; j2 = champ->dst.nj;
  champ->coupe.xmin = x1;
  champ->coupe.xmax = x2;
  champ->coupe.ymin = y1;
  champ->coupe.ymax = y2;


  champ->coupe.njCoupe = champ->coupe.nbNiveauxCoupe;
  if (x1 == x2 && y1 == y2)
    {
    champ->coupe.niCoupe = 1;
    champ->coupe.x[0] = x1;
    champ->coupe.y[0] = y1;

    if (champ->natureTensorielle == SCALAIRE)
      {
      switch(recInterpolationLevel)
         {
         case 0:
         for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
           {
           f77name(ez_rgdint_0)(&champ->coupe.fld2d[i],&x1, &y1, &champ->coupe.niCoupe,champ->coupe.fld3d[i], &i2,&j1,&j2);
           }
         break;

         case 1:
         for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
           {
           f77name(ez_rgdint_1_nw)(&champ->coupe.fld2d[i],&x1, &y1, &champ->coupe.niCoupe,champ->coupe.fld3d[i], &i2,&j1,&j2);
           }
         break;

         case 3:
         for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
           {
           f77name(ez_rgdint_3_nw)(&champ->coupe.fld2d[i],&x1, &y1, &champ->coupe.niCoupe,champ->coupe.fld3d[i], &i2,&j1,&j2);
           }
         break;
         }
      }
    else
      {
      switch(recInterpolationLevel)
         {
         case 0:
         for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
           {
           f77name(ez_rgdint_0)(&champ->coupe.uu2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.uu3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_0)(&champ->coupe.vv2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.vv3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_0)(&champ->coupe.ww2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.ww3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_0)(&champ->coupe.uvwtang2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.uu3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_0)(&champ->coupe.uvwnorm2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.vv3d[i],&i2,&j1,&j2);
           }
         break;

         case 1:
         for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
           {
           f77name(ez_rgdint_1_nw)(&champ->coupe.uu2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.uu3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_1_nw)(&champ->coupe.vv2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.vv3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_1_nw)(&champ->coupe.ww2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.ww3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_1_nw)(&champ->coupe.uvwtang2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.uu3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_1_nw)(&champ->coupe.uvwnorm2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.vv3d[i],&i2,&j1,&j2);
           }
         break;

         case 3:
         for (i=0; i < champ->coupe.nbNiveauxCoupe; i++)
           {
           f77name(ez_rgdint_3_nw)(&champ->coupe.uu2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.uu3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_3_nw)(&champ->coupe.vv2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.vv3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_3_nw)(&champ->coupe.ww2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.ww3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_3_nw)(&champ->coupe.uvwtang2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.uu3d[i],&i2,&j1,&j2);
           f77name(ez_rgdint_3_nw)(&champ->coupe.uvwnorm2d[i],&x1, &y1,&champ->coupe.niCoupe,champ->coupe.vv3d[i],&i2,&j1,&j2);
           }
         break;
         }

      f77name(modulus3d)(champ->coupe.uvw2d,champ->coupe.uvwtang2d,champ->coupe.uvwnorm2d,
            champ->coupe.ww2d,&champ->coupe.nbNiveauxCoupe);

      }
    return 0;
    }

  c_xy2fxfy(&fx1, &fy1, x1, y1);
  c_xy2fxfy(&fx2, &fy2, x2, y2);
  dx = fx2 - fx1;
  dy = fy2 - fy1;

  champ->coupe.niCoupe = (int)(sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
  if (champ->coupe.niCoupe < 25)
    {
    champ->coupe.niCoupe = 25;
    }

  for (i=0; i < champ->coupe.niCoupe; i++)
      {
      champ->coupe.x[i] = (float)(i) / (float)(champ->coupe.niCoupe - 1) * dx + fx1;
      champ->coupe.y[i] = (float)(i) / (float)(champ->coupe.niCoupe - 1) * dy + fy1;
      c_fxfy2xy(&champ->coupe.x[i], &champ->coupe.y[i],champ->coupe.x[i], champ->coupe.y[i]);
      }

  angleCoupe = atan2(dy,dx);

  if (champ->natureTensorielle == SCALAIRE)
     {
     vals = (float *) malloc(champ->coupe.niMaxCoupe*sizeof(float));
        for (j=0; j < champ->coupe.nbNiveauxCoupe; j++)
          {
          switch(recInterpolationLevel)
            {
            case 0:
/*            fprintf(stderr, "XY-Voisin!\n");*/
            f77name(ez_rgdint_0)(vals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.fld3d[j],&i2,&j1,&j2);
            break;

            case 1:
/*            fprintf(stderr, "XY-Bilineaire!\n");*/
            f77name(ez_rgdint_1_nw)(vals,champ->coupe.x,champ->coupe.y,&champ->coupe.niCoupe,champ->coupe.fld3d[j],&i2,&j1,&j2);
            break;

            case 3:
/*            fprintf(stderr, "XY-Bicubique!\n");*/
              f77name(ez_rgdint_3_nw)(vals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe, champ->coupe.fld3d[j], &i2, &j1, &j2);
            break;
            }

         for (i=0; i < champ->coupe.niCoupe; i++)
             {
             ind = FTN2C(i,j,champ->coupe.niCoupe);
             champ->coupe.fld2d[ind] = vals[i];
             }
         }
      free(vals);
     }
  else
     {
    uuvals = (float *) malloc(champ->coupe.niCoupe * sizeof(float));
    vvvals = (float *) malloc(champ->coupe.niCoupe * sizeof(float));
    wwvals = (float *) malloc(champ->coupe.niCoupe * sizeof(float));

    for (j=0; j < champ->coupe.nbNiveauxCoupe; j++)
      {
      switch(recInterpolationLevel)
         {
         case 0:
         f77name(ez_rgdint_0)(uuvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.uu3d[j],&i2,&j1,&j2);
         f77name(ez_rgdint_0)(vvvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.vv3d[j],&i2,&j1,&j2);
         f77name(ez_rgdint_0)(wwvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.ww3d[j],&i2,&j1,&j2);
         break;

         case 1:
         f77name(ez_rgdint_1_nw)(uuvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.uu3d[j],&i2,&j1,&j2);
         f77name(ez_rgdint_1_nw)(vvvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.vv3d[j],&i2,&j1,&j2);
         f77name(ez_rgdint_1_nw)(wwvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.ww3d[j],&i2,&j1,&j2);
         break;

         case 3:
         f77name(ez_rgdint_3_nw)(uuvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.uu3d[j],&i2,&j1,&j2);
         f77name(ez_rgdint_3_nw)(vvvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.vv3d[j],&i2,&j1,&j2);
         f77name(ez_rgdint_3_nw)(wwvals,champ->coupe.x,champ->coupe.y, &champ->coupe.niCoupe,champ->coupe.ww3d[j],&i2,&j1,&j2);
         break;
         }

      for (i=0; i < champ->coupe.niCoupe; i++)
        {
        ind = FTN2C(i,j,champ->coupe.niCoupe);
        dirvent  = atan2(vvvals[i],uuvals[i]);
        module = sqrt(uuvals[i]*uuvals[i]+vvvals[i]*vvvals[i]);

        champ->coupe.uvwtang2d[ind] = module*cos(angleCoupe-dirvent);
        champ->coupe.uvwnorm2d[ind] = module*sin(angleCoupe-dirvent);
        champ->coupe.uu2d[ind] = uuvals[i];
        champ->coupe.vv2d[ind] = vvvals[i];
        champ->coupe.ww2d[ind] = wwvals[i];
        champ->coupe.uvw2d[ind] = sqrt(champ->coupe.uvwtang2d[ind] * champ->coupe.uvwtang2d[ind]+wwvals[i]*wwvals[i]);
        }
      }

    free(uuvals);
    free(vvvals);
    free(wwvals);
    }


  *rx1 = x1;
  *rx2 = x2;
  *ry1 = y1;
  *ry2 = y2;
  return 0;
  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrPreparerSerie(_Champ *champ, float *rx1, float *ry1, float *rx2, float *ry2)
{
  int i,j;
  int i1, j1, i2, j2;
  int un = 1;
  float dx,dy;
  float fx1, fx2, fy1, fy2;
  float *posx, *posy, *vals;
  float x1, y1, x2, y2;
  int ind;
  float *tmpvec;

  x1 = *rx1; x2 = *rx2; y1 = *ry1; y2 = *ry2;

  x1 = (x1 < 1.0) ? 1.0 : (x1 > (float)champ->dst.ni ) ? (float) champ->dst.ni : x1;
  x2 = (x2 < 1.0) ? 1.0 : (x2 > (float)champ->dst.ni ) ? (float) champ->dst.ni : x2;
  y1 = (y1 < 1.0) ? 1.0 : (y1 > (float)champ->dst.nj ) ? (float) champ->dst.nj : y1;
  y2 = (y2 < 1.0) ? 1.0 : (y2 > (float)champ->dst.nj ) ? (float) champ->dst.nj : y2;

  i1 = 1; j1 = 1;
  i2 = champ->dst.ni; j2 = champ->dst.nj;
  champ->seqanim.xmin = x1;
  champ->seqanim.xmax = x2;
  champ->seqanim.ymin = y1;
  champ->seqanim.ymax = y2;


  champ->seqanim.njSerie = champ->seqanim.nbFldsAnim;
  if (x1 == x2 && y1 == y2)
    {
    champ->seqanim.niSerie = 1;

    if (champ->natureTensorielle == SCALAIRE)
      {
      if (champ->seqanim.valeursSeries)
        {
        free(champ->seqanim.valeursSeries);
        }
            champ->seqanim.valeursSeries = (float *) calloc(champ->seqanim.nbFldsAnim, sizeof(float));
      for (i=0; i < champ->seqanim.nbFldsAnim; i++)
        {
        f77name(ez_rgdint_0)(&(champ->seqanim.valeursSeries[i]),
                  &x1,&y1,&un,champ->seqanim.animFLDs[i],&i2,&j1,&j2);

        }


      }

    return 0;
    }

  c_xy2fxfy(&fx1, &fy1, x1, y1);
  c_xy2fxfy(&fx2, &fy2, x2, y2);
  dx = fx2 - fx1;
  dy = fy2 - fy1;

  champ->seqanim.niSerie = (int)(sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
  if (champ->seqanim.niSerie < 25)
    champ->seqanim.niSerie = 25;


  posx = (float *) calloc(champ->seqanim.niSerie, sizeof(float));
  posy = (float *) calloc(champ->seqanim.niSerie, sizeof(float));


  for (i=0; i < champ->seqanim.niSerie; i++)
    {
    posx[i] = (float)(i) / (float)(champ->seqanim.niSerie - 1) * dx + fx1;
    posy[i] = (float)(i) / (float)(champ->seqanim.niSerie - 1) * dy + fy1;
    c_fxfy2xy(&posx[i], &posy[i],posx[i], posy[i]);
    }

  if (champ->natureTensorielle == SCALAIRE)
    {
    tmpvec =  (float *) calloc(champ->dst.ni*champ->dst.nj, sizeof(float));
    vals = (float *) calloc(champ->seqanim.niSerie, sizeof(float));
    if (champ->seqanim.valeursSeries)
      {
      free(champ->seqanim.valeursSeries);
      }
    champ->seqanim.valeursSeries = (float *) calloc(champ->seqanim.niSerie*champ->seqanim.njSerie, sizeof(float));

    for (j=0; j < champ->seqanim.nbFldsAnim; j++)
      {
      f77name(ez_rgdint_3_nw)(vals,posx,posy,&champ->seqanim.niSerie,champ->seqanim.animFLDs[j],&champ->dst.ni,&un,&champ->dst.nj);
      for (i=0; i < champ->seqanim.niSerie; i++)
        {
        ind = FTN2C(i,j,champ->seqanim.niSerie);
        champ->seqanim.valeursSeries[ind] = vals[i];
        }
      }
    free(tmpvec);
    free(vals);
    }

  free(posx);
  free(posy);

  *rx1 = x1;
  *rx2 = x2;
  *ry1 = y1;
  *ry2 = y2;
  return 0;
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrProcessChamp(_Champ *champ)
{
  int ier;
  float *tmpfld, *tmpuufld, *tmpvvfld, *tmpmodule;
  int *tmpintfld;
  char *charfld;
  static char texteLecture[512];
  int langue;
  int ni,nj,nk,npts;
  int gdin, gdout;

  langue = c_getulng();

  FldMgrCalcPDFDatev(champ->pdfdatev,&(champ->datev),champ->dateo,champ->deet,champ->npas,champ->ip2);

  if (champ->natureTensorielle == VECTEUR)
      {
      sprintf(texteLecture, lectureVents[langue],
        champ->typvar, champ->ip1,
        champ->ip2, champ->pdfdatev, champ->etiket);
      }
  else
      {
      sprintf(texteLecture, lecture[langue],
        champ->nomvar, champ->typvar, champ->ip1,
        champ->ip2, champ->pdfdatev, champ->etiket);
      }

  MessageInfo(texteLecture, 1);

  champ->fld =     (float *) (calloc(champ->src.ni * champ->src.nj * champ->src.nk, sizeof(float)));
  champ->fld_orig = (float *) (calloc(champ->src.ni * champ->src.nj * champ->src.nk, sizeof(float)));
  ni = champ->src.ni;
  nj = champ->src.nj;
  nk = champ->src.nk;
  ier = c_fstluk(champ->fld, champ->cle, &ni, &nj, &nk);

  champ->champModifie = False;

  if (champ->datyp == 0)
      {
      charfld = (char *)champ->fld;
      if (0 == strncmp("RLE8",(char *)charfld, 4))
        {
        npts = champ->src.ni * champ->src.nj;
        f77name(decrle)(champ->fld,&npts);
        }
      }

  if (champ->datyp == 2 || champ->datyp == 4 || champ->datyp == 130 || champ->datyp == 132)
      {
      for (tmpfld = champ->fld; tmpfld < (champ->fld + champ->src.ni * champ->src.nj); tmpfld++)
        {
        tmpintfld = (int *) tmpfld;
        *tmpfld = (float) *tmpintfld;
        }
      }
  memcpy(champ->fld_orig, champ->fld, champ->src.ni*champ->src.nj*sizeof(float));

  champ->indDict = ChercherNomVar(champ->nomvar);
  if (champ->indDict < 0)
      {
      champ->indDict = AjouterNomVar(champ->nomvar);
      FldMgrDefineDefaultIntervals(champ);
      }

  FldMgrUpdateGridParams(champ);
  FldMgrUpdateFldParams(champ);

  champ->natureTensorielle = SCALAIRE;
  CheckForUUandVV(champ);

  champ->stats_src.to_be_updated = 1;
  champ->stats_dst.to_be_updated = 0;
  champ->stats_zoom.to_be_updated = 0;
  MettreAJourStats();

  switch (champ->domaine)
    {
    case XY:
      npts = champ->dst.ni*champ->dst.nj;
      if (champ->natureTensorielle == VECTEUR)
        {
	/*gdin = c_ezgetgdin();
	gdout = c_ezgetgdout();
	c_ezdefset(gdout, gdin);*/
	tmpuufld = (float *) calloc(npts,  sizeof(float));
        tmpvvfld = (float *) calloc(npts,  sizeof(float));
        tmpmodule = (float *) calloc(npts, sizeof(float));

        c_ezuvint(tmpuufld, tmpvvfld, champ->uu, champ->vv);
        f77name(modulus)(tmpmodule,tmpuufld,tmpvvfld,&npts);
        free(champ->uu);
        free(champ->vv);

        champ->uu = tmpuufld;
        champ->vv = tmpvvfld;
        champ->module = tmpmodule;
        }
      else
        {
        tmpfld = (float *) calloc(champ->dst.ni*champ->dst.nj, sizeof(float));
        c_ezsint(tmpfld, champ->fld);

        free(champ->fld);
        champ->fld = tmpfld;
        }
      break;

    case YZ:
    case XZ:
      npts = champ->dst.ni*champ->dst.nj;
      if (champ->natureTensorielle == VECTEUR)
        {
        tmpmodule = (float *) calloc(npts, sizeof(float));

        f77name(modulus)(tmpmodule,champ->uu,champ->vv,&npts);
        champ->module = tmpmodule;
        }
      break;

    case VALEURS_PONCTUELLES:
      npts = champ->src.ni*champ->src.nj*champ->src.nk;
      if (champ->natureTensorielle == VECTEUR)
        {
        tmpmodule = (float *) calloc(npts, sizeof(float));

        f77name(modulus)(tmpmodule,champ->uu,champ->vv,&npts);
        champ->module = tmpmodule;
        }
      break;
    }

  switch(champ->domaine)
    {
    case XY:
    case XZ:
    case YZ:
      ni = champ->dst.ni;
      nj = champ->dst.nj;
      champ->stats_src.to_be_updated = 0;
      champ->stats_dst.to_be_updated = 1;
      champ->stats_zoom.to_be_updated = 1;
      MettreAJourStats();
      break;

    case VALEURS_PONCTUELLES:
      ni = champ->src.ni;
      nj = champ->src.nj;
      break;
    }

  if (champ->natureTensorielle == VECTEUR)
    {
    f77name(aminmax)(&champ->uumin[NO_OP],&champ->uumax[NO_OP],champ->uu,&ni,&nj);
    f77name(aminmax)(&champ->vvmin[NO_OP],&champ->vvmax[NO_OP],champ->vv,&ni,&nj);
    f77name(aminmax)(&champ->uvmin[NO_OP],&champ->uvmax[NO_OP],champ->module,&ni,&nj);
    }
  else
    {
    f77name(aminmax)(&champ->fldmin[NO_OP],&champ->fldmax[NO_OP],champ->fld,&ni,&nj);
    champ->min = champ->fldmin[NO_OP];
    champ->max = champ->fldmax[NO_OP];
    }

  FldMgrFlagMissingValues(champ);

  champ->min = champ->fldmin[NO_OP];
  champ->max = champ->fldmax[NO_OP];

  if (champ->missingFlag && GetValeursManquantesToggle())
    {
    champ->max = champ->max2;
    champ->fldmax[NO_OP] = champ->max;
    }


  MessageInfo(texteLecture, 0);
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrRemoveChamp(int indChamp)
{
  FldMgrFreeAllFlds(&fmflds[indChamp]);
  memset((char *) &fmflds[indChamp], '\0', sizeof(_Champ));
  nbChampsActifs--;
  indChampCourant--;
}

/* -------------------------------------------------------------------------------------------------- */

int FldMgrReorgAnimCles(_Champ *champ, char dates[][16], int nbDates)
{
   return 0;
  /*  int i, j, trouve;
      _Champ bidon;

      float **newanimflds;
      float *newAnimMin, *newAnimMax;
      int *newIp1s, *newIp2s, *newIp3s;
      int *newDates, *newClesAnim;
      int deltaT;

      if (nbDates == champ->seqanim.nbFldsAnim)
      return 0;

      newanimflds = (float **) calloc(nbDates, sizeof(float *));
      newAnimMin  = (float *) calloc(nbDates, sizeof(float));
      newAnimMax  = (float *) calloc(nbDates, sizeof(float));
      newIp1s     = (int *) calloc(nbDates, sizeof(int));
      newIp2s     = (int *) calloc(nbDates, sizeof(int));
      newIp3s     = (int *) calloc(nbDates, sizeof(int));
      newDates    = (int *) calloc(nbDates, sizeof(char *));
      newClesAnim = (int *) calloc(nbDates, sizeof(int));
      newDates = (char *) calloc(nbDates, sizeof(int));

  for (i=0; i < nbDates; i++)
  {
      champ->seqanim.pdfdates[i] = (char *) calloc(16,sizeof(char));
      newClesAnim[i] = -1;
    }

  for (i=0; i < champ->seqanim.nbFldsAnim; i++)
    {
      j = 0;
      trouve = False;
      bidon.cle = champ->seqanim.clesAnim[i];
      FldMgrGetFstPrm(&bidon);
      FldMgrCalcPDFDatev(bidon.pdfdatev,bidon.datev,bidon.date,bidon.deet,bidon.npas,bidon.ip2);

      while (!trouve && j < nbDates)
  {
    if (0 == strcmp(dates[j],bidon.pdfdatev))
      trouve = True;
    j++;
  }

      if (trouve)
  {
  j--;
  newanimflds[j] = champ->seqanim.animFLDs[i];
  newAnimMin[j]  = champ->seqanim.animFLDmin[i];
  newAnimMax[j]  = champ->seqanim.animFLDmax[i];
  newIp1s[j]     = champ->seqanim.ip1s[i];
  newIp2s[j]     = champ->seqanim.ip2s[i];
  newIp3s[j]     = champ->seqanim.ip3s[i];
  newDates[j]    = bidon.date;
  newClesAnim[j] = champ->seqanim.clesAnim[i];
  }

      }

  for (i=0; i < champ->seqanim.nbFldsAnim; i++)
      {
      free(champ->seqanim.dates[i]);
      }


  free(champ->seqanim.animFLDs);
  free(champ->seqanim.animFLDmin);
  free(champ->seqanim.animFLDmax);
  free(champ->seqanim.animUUmin);
  free(champ->seqanim.animUUmax);
  free(champ->seqanim.animVVmin);
  free(champ->seqanim.animVVmax);
  free(champ->seqanim.animUVmin);
  free(champ->seqanim.animUVmax);
  free(champ->seqanim.ip1s);
  free(champ->seqanim.ip2s);
  free(champ->seqanim.ip3s);
  free(champ->seqanim.dates);
  free(champ->seqanim.clesAnim);


  champ->seqanim.nbFldsAnim = nbDates;
  champ->seqanim.animFLDs = newanimflds;
  champ->seqanim.animFLDmin = newAnimMin;
  champ->seqanim.animFLDmax = newAnimMax;
  champ->seqanim.ip1s  = newIp1s;
  champ->seqanim.ip2s  = newIp2s;
  champ->seqanim.ip3s  = newIp3s;
  champ->seqanim.dates = newDates;
  champ->seqanim.clesAnim = newClesAnim;
  */
  }


/* -------------------------------------------------------------------------------------------------- */

int FldMgrReplaceChamp(int cle, int iun)
{
  int i, ier;
  int cleTrouvee = False;
  int ind;

  nbChampsActifs  = 1;
  indChampCourant = 0;
  ind = 0;

  if (fmflds[ind].champModifie == TRUE)
      {
      return CHAMP_MODIFIE;
      }

  fmflds[ind].iun = iun;
  fmflds[ind].cle = cle;
  ier = FldMgrGetFstPrm(&fmflds[ind]);

  if (fmflds[ind].seqanim.clesAnim != NULL)
      {
      for (i=0; i < fmflds[ind].seqanim.nbFldsAnim; i++)
        {
        if (fmflds[ind].seqanim.clesAnim[i] == cle)
            cleTrouvee = True;
        }
      }

  FldMgrFreeMainFlds(&fmflds[ind]);
  if (!cleTrouvee)
      FldMgrFreeAnimFlds(&fmflds[ind]);

  cleTrouvee = False;
  if (fmflds[ind].coupe.clesNiveaux != NULL)
      {
      for (i=0; i < fmflds[ind].coupe.nbNiveauxCoupe; i++)
        {
        if (fmflds[ind].coupe.clesNiveaux[i] == cle)
            cleTrouvee = True;
        }
      }

  if (!cleTrouvee)
      FldMgrFreeCoupeFlds(&fmflds[ind]);

  FldMgrProcessChamp(&fmflds[ind]);
  return OK;
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrTrierClesSelonIP1(int listeCles[], int *nbCles)
{
  _Champ *tmp, tmpswap;
  int i,j;
  int indmax;
  float nivmax;

  int kind;
  int versPression = -1;


  tmp = (_Champ *) calloc(*nbCles, sizeof(_Champ));
  for (i=0; i < *nbCles; i++)
      {
      tmp[i].cle = listeCles[i];
      FldMgrGetFstPrm(&tmp[i]);
      f77name(convip_plus) (&(tmp[i].ip1), &(tmp[i].niveau), &kind, &versPression, NULL, &faux,(F2Cl) 0);
      }

  for (i=0; i < *nbCles; i++)
      {
      nivmax = tmp[i].niveau;
      indmax = i;
      for (j=i; j < *nbCles; j++)
        {
        if (tmp[j].niveau > nivmax)
          {
          nivmax = tmp[j].niveau;
          indmax = j;
          }
        }

      tmpswap = tmp[i];
      tmp[i] =  tmp[indmax];
      tmp[indmax] = tmpswap;
      }

  for (i=0; i < *nbCles; i++)
      {
      listeCles[i] = tmp[i].cle;
      }

  for (i=0; i < *nbCles; i++)
      {
      if (tmp[i].ip1 == 0)
        listeCles[i] = -1;

      }

  for (i=1; i < *nbCles; i++)
      {
      if (tmp[i].ip1 == tmp[i-1].ip1)
        {
        listeCles[i] = -1;
        }
      }

  j = 0;
  i = 0;
  while (j < *nbCles)
      {
      if (listeCles[j] != -1)
        {
        listeCles[i] = listeCles[j];
        i++;
        }
      j++;
      }

  *nbCles = i;

/*  for (i=0; i < *nbCles; i++)
    {
    fprintf(stderr, "%d %f\n", i, tmp[i].niveau);
    }*/
  free(tmp);
  }



/* -------------------------------------------------------------------------------------------------- */

void FldMgrUpdateFldParams(_Champ *champ)
{
  int lng;
  static char *Mois[] = {"bid", "janvier", "fevrier", "mars", "avril", "mai", "juin", "juillet", "aout", "septembre", "octobre", "novembre", "decembre"};
  static char *Month[] = {"bid", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  int tempDate;

  int ind, indMois;
  int i;
  int yymmdd,hhmmss;
  double dddeltaT;
  int date2;
  int kind, mode;
  int versPression = -1;
  char stringNiveau[16];

  lng = c_getulng();

  tempDate = champ->dateo;
  dddeltaT = (double)(champ->rnpas*champ->deet)/3600.0;

  f77name(incdatr)(&date2, &tempDate, &dddeltaT);
  mode = -3;
  f77name(newdate)(&date2, &yymmdd, &hhmmss, &mode);

  sprintf(champ->annee, "%04d", yymmdd/10000);
  sprintf(champ->mois, "%02d", (yymmdd % 10000) / 100);
  sprintf(champ->jour, "%02d", yymmdd % 100);
  sprintf(champ->heureInit, "%02d", hhmmss / 1000000);
  sprintf(champ->minute, "%02d", (hhmmss / 10000) % 100);

  sscanf(champ->mois, "%d", &indMois);

  if (lng)
      sprintf(champ->mois, "%s", Month[indMois]);
  else
      sprintf(champ->mois, "%s", Mois[indMois]);


  for (i=0; i < 16; i++)
    {
    stringNiveau[i] = '\0';
    }
  f77name(f_convip)(&champ->ip1, &champ->niveau, &kind, &versPression, stringNiveau, &vrai, (F2Cl) 15);
  champ->coordonneeVerticale = kind;
  nettoyer(stringNiveau);

  champ->heure  = (float)(ROUND((float)dddeltaT));

  switch (lng)
      {
      case 0:
      switch(champ->typvar[0])
        {
        case 'A':
        sprintf(champ->titreTemps, labelAnal[lng],
          champ->heureInit, champ->minute,
          champ->jour, champ->mois, champ->annee);
        break;

        case 'P':
        sprintf(champ->titreTemps, labelPrev[lng],
          (int)(champ->heure), champ->heureInit, champ->minute,
          champ->jour, champ->mois, champ->annee);
        break;

        case 'E':
        sprintf(champ->titreTemps, labelDiff[lng],
          (int)(champ->heure), champ->heureInit, champ->minute,
          champ->jour, champ->mois, champ->annee);
        break;

        case 'C':
        sprintf(champ->titreTemps, labelClim[lng],
          champ->heureInit, champ->minute,
          champ->jour, champ->mois, champ->annee);
        break;

        case 'R':
        sprintf(champ->titreTemps, labelResidus[lng],
          champ->heureInit, champ->minute,
          champ->jour, champ->mois, champ->annee);
        break;

        default:
        sprintf(champ->titreTemps, labelDefaut[lng],
          champ->heureInit, champ->minute,
          champ->jour, champ->mois, champ->annee);
        break;
        }
      break;

      case 1:
      switch(champ->typvar[0])
        {
        case 'A':
        sprintf(champ->titreTemps, labelAnal[lng],
          champ->heureInit, champ->minute,
          champ->mois, champ->jour, champ->annee);
        break;

        case 'P':
        sprintf(champ->titreTemps, labelPrev[lng],
          (int)(champ->heure), champ->heureInit, champ->minute,
          champ->mois, champ->jour, champ->annee);
        break;

        case 'E':
        sprintf(champ->titreTemps, labelDiff[lng],
          (int)(champ->heure), champ->heureInit, champ->minute,
          champ->mois, champ->jour, champ->annee);
        break;

        case 'C':
        sprintf(champ->titreTemps, labelClim[lng],
          champ->heureInit, champ->minute,
          champ->mois, champ->jour, champ->annee);
        break;

        case 'R':
        sprintf(champ->titreTemps, labelResidus[lng],
          champ->heureInit, champ->minute,
          champ->mois, champ->jour, champ->annee);
        break;

        default:
        sprintf(champ->titreTemps, labelDefaut[lng],
          champ->heureInit, champ->minute,
          champ->mois, champ->jour, champ->annee);
        break;
        }
      break;
      }

  sprintf(champ->titreEtiquette, labelEtiquette[lng], champ->etiket);

  if (0 == strcmp(champ->nomvar, "DZ"))
      {
      if (champ->ip1 == 0 || champ->ip1 == 12000)
  {
        sprintf(champ->titreNiveau, labelSurface[lng]);
  }
      }
  else
    {
    if (lng == 0)
      {
      strcpy(champ->titreNiveau, "Niveau: ");
      }
    else
      {
      strcpy(champ->titreNiveau, "Level: ");
      }
      strcat(champ->titreNiveau, stringNiveau);
    }

  DictMgrGetFacteurDeConversion(&champ->facteur, champ->indDict);
  DictMgrGetIdentifVar(champ->titreVariable, champ->indDict);
  DictMgrGetUnitesVar(champ->titreUnites, champ->indDict);
  DictMgrGetIndIntervalleDeDefaut(&ind, champ->indDict);
  DictMgrGetIntervallesDeContours(champ->intervalles, &champ->nbIntervalles, champ->indDict, ind);

  UpdateTitreIntervalle(champ->titreIntervalle, champ->intervalles, champ->nbIntervalles, champ->facteur, champ->titreUnites);
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrUpdateGridParams(_Champ *champ)
{
   int gdin, gdout;
   float *lat, *lon;
   int npts;
   char grtyp, grref;
   int ni, nj, ig1, ig2, ig3, ig4, ig1ref, ig2ref, ig3ref, ig4ref;

   FldMgrAjusterDimensionsGrille(champ);
    gdin = c_ezgetgdin();
    if (gdin == -1)
	{
	champ->src.gdid = c_ezqkdef(champ->src.ni, champ->src.nj, champ->src.grtyp, champ->src.ig1, champ->src.ig2, champ->src.ig3, champ->src.ig4, champ->iun);
	gdin = champ->src.gdid;
	}

    gdout = FldMgrDefinirGrille();
   if (champ->src.grtyp[0] == 'Y')
      {

      champ->domaine = VALEURS_PONCTUELLES;
      c_ezgxprm(gdin, &ni, &nj, &grtyp, &ig1, &ig2, &ig3, &ig4, &grref, &ig1ref, &ig2ref, &ig3ref, &ig4ref);

/*    if (ni > 1 && nj > 1)
         {
         champ->domaine == XY;
         } */

/*      if (grref == 'O')
         {
         champ->domaine = XY;
         champ->src.grref[0] = 'O';
         champ->src.ig1ref = ig1ref;
         champ->src.ig2ref = ig2ref;
         champ->src.ig3ref = ig3ref;
         champ->src.ig4ref = ig4ref;
         }*/
      }

  /*  CheckForPointValues(champ, nbChampsActifs-1); */

   switch(champ->domaine)
      {
      case XY:
      f77name(chkgrid)(champ->fld, &champ->src.ni, &champ->src.nj, &champ->src.grtyp[0], &champ->src.ig2, (F2Cl) 1);
      if (GetGrilleSelectionnee() == 0)
         {
         if (HasGridChanged(champ->src.grtyp[0],champ->src.ni,champ->src.nj,champ->src.nk,
             champ->src.ig1, champ->src.ig2, champ->src.ig3,champ->src.ig4))
            {
            gdout = FldMgrDefinirGrille();
            if (gdout == -1) gdout = gdin;
            c_ezdefset(gdout,gdout);
            }
         }
      gdout = c_ezgetgdout();
      gdin  = c_ezgdefrec(champ->src.ni,champ->src.nj,
        champ->src.grtyp,champ->src.ig1,champ->src.ig2,champ->src.ig3,champ->src.ig4);
      if (gdout != gdin && gdout != -1)
        {
        c_ezdefset(gdout, gdin);
        }
      else
        {
        c_ezdefset(gdin, gdin);
        gdout = c_ezgetgdout();
        }

      c_ezgprm(gdout, champ->dst.grtyp, &champ->dst.ni, &champ->dst.nj,
        &champ->dst.ig1, &champ->dst.ig2, &champ->dst.ig3, &champ->dst.ig4);
      champ->dst.nk = 1;
      if (champ->src.grtyp[0] == 'Y')
         {
         npts = champ->src.ni*champ->src.nj*champ->src.nk;
         lat = (float *) calloc(npts, sizeof(float));
         lon = (float *) calloc(npts, sizeof(float));
         if (champ->x != NULL) free(champ->x);
         if (champ->y != NULL) free(champ->y);
         champ->x = (float *) calloc(npts, sizeof(float));
         champ->y = (float *) calloc(npts, sizeof(float));
         c_gdll(gdin, lat, lon);
         c_gdxyfll(gdout, champ->x, champ->y, lat, lon, npts);
         champ->dst.nk = 1;
         free(lat);
         free(lon);
         }
      break;

    case XZ:
    case YZ:
      strcpy(champ->dst.grtyp,"X");
      champ->dst.ig1 = 0;
      champ->dst.ig2 = 0;
      champ->dst.ig3 = 0;
      champ->dst.ig4 = 0;
      InitMapInfo(champ->dst.grtyp[0],
      champ->dst.ni, champ->dst.nj,
      champ->dst.ig1, champ->dst.ig2, champ->dst.ig3, champ->dst.ig4);
      ThisIsTheCurrentGrid(champ->dst.grtyp[0],
        champ->dst.ni, champ->dst.nj, champ->dst.nk,
        champ->dst.ig1, champ->dst.ig2, champ->dst.ig3, champ->dst.ig4);

      break;

    case VALEURS_PONCTUELLES:
      if (GetGrilleSelectionnee() == 0)
        {
        if (HasGridChanged(champ->src.grtyp[0],champ->src.ni,champ->src.nj,champ->src.nk,
              champ->src.ig1, champ->src.ig2, champ->src.ig3,champ->src.ig4))
          {
          gdout = FldMgrDefinirGrille();
          c_ezdefset(gdout,gdout);
          }
        }
      gdout = c_ezgetgdout();
      gdin  = c_ezgdefrec(champ->src.ni,champ->src.nj,
        champ->src.grtyp,champ->src.ig1,champ->src.ig2,champ->src.ig3,champ->src.ig4);
      if (gdout != gdin)
        {
        c_ezdefset(gdout, gdin);
        }
      else
        {
        c_ezdefset(gdin, gdin);
        gdout = c_ezgetgdout();
        }

      npts = champ->src.ni*champ->src.nj*champ->src.nk;
      lat = (float *) calloc(npts, sizeof(float));
      lon = (float *) calloc(npts, sizeof(float));
      champ->x = (float *) calloc(npts, sizeof(float));
      champ->y = (float *) calloc(npts, sizeof(float));
      c_gdll(gdin, lat, lon);
      c_gdxyfll(gdout, champ->x, champ->y, lat, lon, npts);
      c_ezgprm(gdout, champ->dst.grtyp, &champ->dst.ni, &champ->dst.nj,
        &champ->dst.ig1, &champ->dst.ig2, &champ->dst.ig3, &champ->dst.ig4);
      champ->dst.nk = 1;
      free(lat);
      free(lon);
      break;
    }
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrVerConsistanceGrilles(_Champ champ, int listeCles[], int *nbCles)
{
  int i, ivalide;
  _Champ bidon;

  ivalide = 0;

  for (i=0; i < *nbCles; i++)
      {
      bidon.cle = listeCles[i];
      FldMgrGetFstPrm(&bidon);
      if (bidon.src.ni == champ.src.ni && bidon.src.nj == champ.src.nj && bidon.src.nk == champ.src.nk && bidon.src.grtyp[0] == champ.src.grtyp[0] &&
          bidon.src.ig1 == champ.src.ig1 && bidon.src.ig2 == champ.src.ig2 && bidon.src.ig3 == champ.src.ig3 && bidon.src.ig3 == champ.src.ig3 &&
          bidon.src.ig4 == champ.src.ig4)
        {
        listeCles[ivalide] = listeCles[i];
        ivalide++;
        }

      }

  *nbCles = ivalide;

  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrVerConsistanceNiveauxChamps(int *indiceFautif)
{
  int  n, ichamps[NB_MAX_CHAMPS_ACTIFS], ichampsActifs;

  *indiceFautif = nbChampsActifs;

  ichampsActifs = 0;
  for (n=0; n < nbChampsActifs; n++)
      {
      if (fmflds[n].coupe.nbNiveauxCoupe > 1)
        {
        ichamps[ichampsActifs] = n;
        ichampsActifs++;
        }
      }
/*
   for (n=1; n < ichampsActifs; n++)
       {
       if (fmflds[ichamps[n]].coupe.nbNiveauxCoupe != fmflds[ichamps[n-1]].coupe.nbNiveauxCoupe)
         {
         *indiceFautif = n;
         return NB_NIVEAUX_INCONSISTANTS;
         }

       for (i=0; i < fmflds[ichamps[n]].coupe.nbNiveauxCoupe; i++)
         {
         if (fmflds[ichamps[n]].coupe.niveauxCoupe[i] != fmflds[ichamps[n-1]].coupe.niveauxCoupe[i])
             {
             *indiceFautif = n;
             return NIVEAUX_INCONSISTANTS;
             }
         }
       }
*/

  return 0;
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrVerConsistanceNiveaux(_Champ champ, int listeCles[], int *nbCles)
{
  int i, ivalide;
  float niveau;
  int kind, kindref;
  int versPression = -1;
  _Champ bidon;

  ivalide = 0;
  f77name(convip_plus)(&champ.ip1, &niveau, &kindref, &versPression, NULL, &faux, (F2Cl) 0);

  for (i=0; i < *nbCles; i++)
    {
    bidon.cle = listeCles[i];
    FldMgrGetFstPrm(&bidon);
    if (champ.deet*champ.npas == bidon.deet*bidon.npas)
      {
      f77name(convip_plus)(&bidon.ip1, &niveau, &kind, &versPression, NULL, &faux, (F2Cl) 0);
      if (kindref == kind)
        {
        listeCles[ivalide] = listeCles[i];
        ivalide++;
        }
      }
    }

  *nbCles = ivalide;

}


/* -------------------------------------------------------------------------------------------------- */

int FldMgrPreparerTopo()
{
  int i,n,valtoggle;
  int ier;
  int i1,j1,i2,j2,npts,selectedVertCoord;
  _Champ *champ;


  ier = 0;
  valtoggle = CoupeMgrGetToggleTopo();
  selectedVertCoord = GetSelectedVertCoord();

  if (!valtoggle && selectedVertCoord != PRES_VCOORD)
        return -1;

  for (n=0; n < nbChampsActifs; n++)
    {
    FldMgrGetChamp(&champ,n);
    FldMgrGetFstPrm(champ);
    ier = GetSurfacePressure(champ);

    if (ier < 0) return -1;

    if (champ->coupe.niCoupe == 1)
      npts = 25;
    else
      npts = champ->coupe.niCoupe;

    i1 = 1;
    j1 = 1;
    i2 = champ->dst.ni;
    j2 = champ->dst.nj;

    f77name(ez_rgdint_1_nw)(champ->coupe.ligneMontagnes, champ->coupe.x, champ->coupe.y, &champ->coupe.niCoupe, champ->coupe.montagnes, &i2,&j1,&j2);

    if (c_wglgdbg())
      {
      printf("Debug PreparerTopo: npts = %d\n",champ->coupe.niCoupe);

      for (i=0; i < champ->coupe.niCoupe; i++)
        {
        printf("%d %f %f %f\n",i,champ->coupe.x[i], champ->coupe.y[i],champ->coupe.ligneMontagnes[i]);
        }
      }
    }

/*  free(posx);
  free(posy);*/
  return 0;
  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrSetDiffMinMax(int ind)
{
  int npts;

  fmflds[ind].fldmin[NO_OP]= fmflds[ind].min;
  fmflds[ind].fldmax[NO_OP]= fmflds[ind].max;

  if (ind == 0)
    return 0;

  if ((fmflds[ind].natureTensorielle == fmflds[ind-1].natureTensorielle) &&
      (fmflds[ind].domaine == fmflds[ind-1].domaine))
    {
    fmflds[ind-1].diffValide = 1;
    }
  else
    {
    fmflds[ind-1].diffValide = 0;
    }

  if (1 == fmflds[ind-1].diffValide)
    {
    npts =  fmflds[ind].dst.ni*fmflds[ind].dst.nj;
    if (fmflds[ind-1].natureTensorielle == SCALAIRE)
      {
      DiffMgrSetDiffs(fmflds[ind-1].fld,fmflds[ind].fld,
          fmflds[ind-1].fldmin,fmflds[ind-1].fldmax,npts);
      }
    else
      {
      DiffMgrSetDiffs(fmflds[ind-1].uu,fmflds[ind].uu,
          fmflds[ind-1].uumin,fmflds[ind-1].uumax,npts);
      DiffMgrSetDiffs(fmflds[ind-1].vv,fmflds[ind].vv,
          fmflds[ind-1].vvmin,fmflds[ind-1].vvmax,npts);
      DiffMgrSetVDiffs2D(fmflds[ind-1].uu,fmflds[ind-1].vv,
      fmflds[ind].uu,fmflds[ind].vv,
      fmflds[ind-1].uvmin,fmflds[ind-1].uvmax,npts);
      }
    }
 return 0;
}

/* -------------------------------------------------------------------------------------------------- */

int FldMgrSetAnimDiffMinMax(int indChamp)
{
  _Champ *champ1, *champ2;
  int nbChampsActifs;
  float *fld1,*fld2;
  int i,n,npts;
  float fldmin[5],fldmax[5],uvmin[5],uvmax[5];
  float *uu1,*vv1,*uu2,*vv2,*uv1,*uv2;

  nbChampsActifs = FldMgrGetNbChampsActifs();
  if (0 == indChamp%2)
    {
    return 0;
    }
  return 0;
  FldMgrGetChamp(&champ1,indChamp-1);
  FldMgrGetChamp(&champ2,indChamp);

  npts = champ1->dst.ni*champ1->dst.nj;;
  if (champ1->natureTensorielle == SCALAIRE)
    {
    fld1 = calloc(npts,sizeof(float));
    fld2 = calloc(npts,sizeof(float));
    }
  else
    {
    uu1 = calloc(npts,sizeof(float));
    vv1 = calloc(npts,sizeof(float));
    uu2 = calloc(npts,sizeof(float));
    vv2 = calloc(npts,sizeof(float));
    uv1 = calloc(npts,sizeof(float));
    uv2 = calloc(npts,sizeof(float));
    }

  if ((*champ1).seqanim.nbFldsAnim > 0)
    {
    for (n=0; n < (*champ1).seqanim.nbFldsAnim; n++)
      {
      if (champ1->natureTensorielle == SCALAIRE)
        {
        fld1 = (float *)(*champ1).seqanim.animFLDs[n];
        fld2 = (float *)(*champ2).seqanim.animFLDs[n];

        DiffMgrSetDiffs(fld1,fld2,fldmin,fldmax,npts);
        for (i=0; i < 5; i++)
          {
          champ1->fldmin[i] = fldmin[i] < champ1->fldmin[i] ? fldmin[i] : champ1->fldmin[i];
          champ1->fldmax[i] = fldmax[i] > champ1->fldmax[i] ? fldmax[i] : champ1->fldmax[i];
          }
        }
      else
        {
        uu1 = (float *)(*champ1).seqanim.animUUs[n];
        uu2 = (float *)(*champ2).seqanim.animUUs[n];

        vv1 = (float *)(*champ1).seqanim.animVVs[n];
        vv2 = (float *)(*champ2).seqanim.animVVs[n];

        DiffMgrSetVDiffs2D(uu1,vv1,uu2,vv2,uvmin,uvmax,npts);
        for (i=0; i < 5; i++)
          {
          champ1->uvmin[i] = uvmin[i] < champ1->uvmin[i] ? uvmin[i] : champ1->uvmin[i];
          champ1->uvmax[i] = uvmax[i] > champ1->uvmax[i] ? uvmax[i] : champ1->uvmax[i];
          }
        }
      }
    }

  if (champ1->natureTensorielle == SCALAIRE)
    {
    free(fld1);
    free(fld2);
    }
  else
    {
    free(uu1);
    free(vv1);
    free(uu2);
    free(vv2);
    free(uv1);
    free(uv2);
    }
}

/* -------------------------------------------------------------------------------------------------- */

void FldMgrCalcDiffMinMax(float *min, float *max, float *fld1, float *fld2, int npts)
{
  float diff;
  int i;

  *min = fld2[0] - fld1[0];
  *max = *min;

  for (i=1; i < npts; i++)
      {
      diff = fld2[i] - fld1[i];
      *min = *min > diff ? diff : *min;
      *max = *max < diff ? diff : *max;
      }
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrCalcVDiffMinMax(float *min, float *max, float *fld1, float *fld2, int npts)
{
  float diff,diffuu,diffvv;
  int i;

  diffuu =  fld2[0]-fld1[0];
  diffvv =  fld2[npts]-fld1[npts];
  diff =  sqrt(diffuu*diffuu+diffvv*diffvv);
  *min = diff;
  *max = diff;

  for (i=1; i < npts; i++)
      {
      diffuu =  fld2[i]-fld1[i];
      diffvv =  fld2[i+npts]-fld1[i+npts];
      diff =  sqrt(diffuu*diffuu+diffvv*diffvv);
      *min =  *min > diff ? diff : *min;
      *max =  *max < diff ? diff : *max;
      }
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrSetCoupeMinMax(int ind)
{
  int i,j,n, npts;
  float *tmpmod = NULL;
  float *tmpuu, *tmpvv, module;
  float opmin[5],opmax[5];

  n = ind;
  if (fmflds[ind].natureTensorielle == SCALAIRE)
      {
      f77name(aminmax)(&fmflds[ind].coupe.FLDmin3d[NO_OP], &fmflds[ind].coupe.FLDmax3d[NO_OP],
          fmflds[ind].coupe.fld3d[0], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);

      for (j=0; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
        {
        f77name(aminmax)(&opmin[NO_OP], &opmax[NO_OP],
              fmflds[ind].coupe.fld3d[j], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);
        fmflds[n].coupe.FLDmin3d[0] = (fmflds[n].coupe.FLDmin3d[0]  < opmin[0]) ? fmflds[n].coupe.FLDmin3d[0] : opmin[0];
        fmflds[n].coupe.FLDmax3d[0] = (fmflds[n].coupe.FLDmax3d[0]  > opmax[0]) ? fmflds[n].coupe.FLDmax3d[0] : opmax[0];
        }

      }
  else
      {
      f77name(aminmax)(&fmflds[ind].coupe.UUmin3d[NO_OP], &fmflds[ind].coupe.UUmax3d[NO_OP],
          fmflds[ind].coupe.uu3d[0], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);

      for (j=0; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
        {
        f77name(aminmax)(&opmin[NO_OP], &opmax[NO_OP],
              fmflds[ind].coupe.uu3d[j], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);
        fmflds[n].coupe.UUmin3d[0] = (fmflds[n].coupe.UUmin3d[0]  < opmin[0]) ? fmflds[n].coupe.UUmin3d[0] : opmin[0];
        fmflds[n].coupe.UUmax3d[0] = (fmflds[n].coupe.UUmax3d[0]  > opmax[0]) ? fmflds[n].coupe.UUmax3d[0] : opmax[0];

        }

      f77name(aminmax)(&fmflds[ind].coupe.VVmin3d[NO_OP], &fmflds[ind].coupe.VVmax3d[NO_OP],
          fmflds[ind].coupe.vv3d[0], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);

      for (j=0; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
        {
        f77name(aminmax)(&opmin[NO_OP], &opmax[NO_OP],
              fmflds[ind].coupe.vv3d[j], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);
        fmflds[n].coupe.VVmin3d[0] = (fmflds[n].coupe.VVmin3d[0]  < opmin[0]) ? fmflds[n].coupe.VVmin3d[0] : opmin[0];
        fmflds[n].coupe.VVmax3d[0] = (fmflds[n].coupe.VVmax3d[0]  > opmax[0]) ? fmflds[n].coupe.VVmax3d[0] : opmax[0];
        }


      f77name(aminmax)(&fmflds[ind].coupe.WWmin3d[NO_OP], &fmflds[ind].coupe.WWmax3d[NO_OP],
          fmflds[ind].coupe.ww3d[0], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);

      for (j=0; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
        {
        f77name(aminmax)(&opmin[NO_OP], &opmax[NO_OP],
              fmflds[ind].coupe.ww3d[j], &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);
        fmflds[n].coupe.WWmin3d[0] = (fmflds[n].coupe.WWmin3d[0]  < opmin[0]) ? fmflds[n].coupe.WWmin3d[0] : opmin[0];
        fmflds[n].coupe.WWmax3d[0] = (fmflds[n].coupe.WWmax3d[0]  > opmax[0]) ? fmflds[n].coupe.WWmax3d[0] : opmax[0];

        }

      npts = fmflds[ind].dst.ni * fmflds[ind].dst.nj;
      tmpmod = (float *)  calloc(npts, sizeof(float));
      f77name(modulus3d)(tmpmod,fmflds[n].coupe.uu3d[0],fmflds[n].coupe.vv3d[0],fmflds[n].coupe.ww3d[0],&npts);
      f77name(aminmax)(&fmflds[ind].coupe.UVWmin3d[NO_OP], &fmflds[ind].coupe.UVWmax3d[NO_OP],
          tmpmod, &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);

      for (j=0; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
        {
        f77name(modulus3d)(tmpmod,fmflds[n].coupe.uu3d[j],fmflds[n].coupe.vv3d[j],fmflds[n].coupe.ww3d[j],&npts);
        f77name(aminmax)(&opmin[NO_OP], &opmax[NO_OP],
              tmpmod, &fmflds[ind].dst.ni,&fmflds[ind].dst.nj);
        fmflds[n].coupe.UVWmin3d[0] = (fmflds[n].coupe.UVWmin3d[0]  < opmin[0]) ? fmflds[n].coupe.UVWmin3d[0] : opmin[0];
        fmflds[n].coupe.UVWmax3d[0] = (fmflds[n].coupe.UVWmax3d[0]  > opmax[0]) ? fmflds[n].coupe.UVWmax3d[0] : opmax[0];
        }

      module = fmflds[n].coupe.uu3d[0][0]*fmflds[n].coupe.uu3d[0][0]+ fmflds[n].coupe.vv3d[0][0]* fmflds[n].coupe.vv3d[0][0];
      fmflds[n].coupe.UVmin3d[0] = module;
      fmflds[n].coupe.UVmax3d[0] = module;
      for (j=0; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
        {
        tmpuu = fmflds[n].coupe.uu3d[j];
        tmpvv = fmflds[n].coupe.vv3d[j];
        for (i=0; i < npts; i++)
          {
          module = tmpuu[i]*tmpuu[i]+tmpvv[i]*tmpvv[i];
          if (fmflds[n].coupe.UVmin3d[0] > module) fmflds[n].coupe.UVmin3d[0] = module;
          if (fmflds[n].coupe.UVmax3d[0] < module) fmflds[n].coupe.UVmax3d[0] = module;
          }
        }
      fmflds[n].coupe.UVmin3d[0] = sqrt(fmflds[n].coupe.UVmin3d[0]);
      fmflds[n].coupe.UVmax3d[0] = sqrt(fmflds[n].coupe.UVmax3d[0]);

      }
  if (tmpmod)
      {
      free(tmpmod);
      }
  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrSetDiffCoupeMinMax(int ind)
{
  int i,j,npts;
  float opmin[5],opmax[5];

  if (0 == ind%2)
      {
      return 0;
      }

  if (fmflds[ind].natureTensorielle == fmflds[ind-1].natureTensorielle)
      {
      fmflds[ind-1].diffValide = 1;
      }
  else
      {
      fmflds[ind-1].diffValide = 0;
      }

  npts =  fmflds[ind].dst.ni*fmflds[ind].dst.nj;

  if (fmflds[ind].natureTensorielle == SCALAIRE)
      {
      if (fmflds[ind-1].natureTensorielle == SCALAIRE)
        {
        DiffMgrSetDiffs(fmflds[ind-1].coupe.fld3d[0],fmflds[ind].coupe.fld3d[0],
                        fmflds[ind-1].coupe.FLDmin3d,fmflds[ind-1].coupe.FLDmax3d,npts);

        for (j=1; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
          {
          DiffMgrSetDiffs(fmflds[ind-1].coupe.fld3d[j],fmflds[ind].coupe.fld3d[j], opmin,opmax,npts);

          for (i=0; i < 5; i++)
              {
              fmflds[ind-1].coupe.FLDmin3d[i] = opmin[i] < fmflds[ind-1].coupe.FLDmin3d[i] ? opmin[i] : fmflds[ind-1].coupe.FLDmin3d[i];
              fmflds[ind-1].coupe.FLDmax3d[i] = opmax[i] > fmflds[ind-1].coupe.FLDmax3d[i] ? opmax[i] : fmflds[ind-1].coupe.FLDmax3d[i];
              }
          }

        }
      }
  else
      {
      if (fmflds[ind-1].natureTensorielle == VECTEUR)
        {
        DiffMgrSetDiffs(fmflds[ind-1].coupe.uu3d[0],fmflds[ind].coupe.uu3d[0],
            fmflds[ind-1].coupe.UUmin3d,fmflds[ind-1].coupe.UUmax3d,npts);

        for (j=1; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
          {
          DiffMgrSetDiffs(fmflds[ind-1].coupe.uu3d[j],fmflds[ind].coupe.uu3d[j], opmin,opmax,npts);

          for (i=0; i < 5; i++)
              {
              fmflds[ind-1].coupe.UUmin3d[i] = opmin[i] < fmflds[ind-1].coupe.UUmin3d[i] ? opmin[i] : fmflds[ind-1].coupe.UUmin3d[i];
              fmflds[ind-1].coupe.UUmax3d[i] = opmax[i] > fmflds[ind-1].coupe.UUmax3d[i] ? opmax[i] : fmflds[ind-1].coupe.UUmax3d[i];
              }
          }

        DiffMgrSetDiffs(fmflds[ind-1].coupe.vv3d[0],fmflds[ind].coupe.vv3d[0],
            fmflds[ind-1].coupe.VVmin3d,fmflds[ind-1].coupe.VVmax3d,npts);

        for (j=1; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
          {
          DiffMgrSetDiffs(fmflds[ind-1].coupe.vv3d[j],fmflds[ind].coupe.vv3d[j], opmin,opmax,npts);

          for (i=0; i < 5; i++)
              {
              fmflds[ind-1].coupe.VVmin3d[i] = opmin[i] < fmflds[ind-1].coupe.VVmin3d[i] ? opmin[i] : fmflds[ind-1].coupe.VVmin3d[i];
              fmflds[ind-1].coupe.VVmax3d[i] = opmax[i] > fmflds[ind-1].coupe.VVmax3d[i] ? opmax[i] : fmflds[ind-1].coupe.VVmax3d[i];
              }
          }
        DiffMgrSetDiffs(fmflds[ind-1].coupe.ww3d[0],fmflds[ind].coupe.ww3d[0], fmflds[ind-1].coupe.WWmin3d,fmflds[ind-1].coupe.WWmax3d,npts);

        for (j=1; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
          {
          DiffMgrSetDiffs(fmflds[ind-1].coupe.ww3d[j],fmflds[ind].coupe.ww3d[j], opmin,opmax,npts);

          for (i=0; i < 5; i++)
              {
              fmflds[ind-1].coupe.WWmin3d[i] = opmin[i] < fmflds[ind-1].coupe.WWmin3d[i] ? opmin[i] : fmflds[ind-1].coupe.WWmin3d[i];
              fmflds[ind-1].coupe.WWmax3d[i] = opmax[i] > fmflds[ind-1].coupe.WWmax3d[i] ? opmax[i] : fmflds[ind-1].coupe.WWmax3d[i];
              }
          }

        DiffMgrSetVDiffs3D(fmflds[ind-1].coupe.uu3d[0], fmflds[ind-1].coupe.vv3d[0],fmflds[ind-1].coupe.ww3d[0],
        fmflds[ind].coupe.uu3d[0], fmflds[ind].coupe.vv3d[0],fmflds[ind].coupe.ww3d[0],
        fmflds[ind-1].coupe.UVWmin3d,  fmflds[ind-1].coupe.UVWmax3d, npts);

        for (j=1; j < fmflds[ind].coupe.nbNiveauxCoupe;j++)
          {
          DiffMgrSetVDiffs3D(fmflds[ind-1].coupe.uu3d[j], fmflds[ind-1].coupe.vv3d[j],fmflds[ind-1].coupe.ww3d[j],
                  fmflds[ind].coupe.uu3d[j], fmflds[ind].coupe.vv3d[j],fmflds[ind].coupe.ww3d[j],
                  opmin,  opmax, npts);

          for (i=0; i < 5; i++)
              {
              fmflds[ind-1].coupe.UVWmin3d[i] = opmin[i] < fmflds[ind-1].coupe.UVWmin3d[i] ? opmin[i] : fmflds[ind-1].coupe.UVWmin3d[i];
              fmflds[ind-1].coupe.UVWmax3d[i] = opmax[i] > fmflds[ind-1].coupe.UVWmax3d[i] ? opmax[i] : fmflds[ind-1].coupe.UVWmax3d[i];
              }
          }
      }
    }
  return 0;
  }


/* -------------------------------------------------------------------------------------------------- */

void FldMgrCalcPDFDatev(char pdfdatev[], int *datev, int dateo, int deet,int npas,int ip2)
{
  double deltat;
  int idateo,idatev00,idatev01,mode;

/*   if (deet == 0 && npas == 0)
//     {
//     deet = 3600;
//     npas = ip2;
     } */

  deltat = (double)((deet*npas)/3600.0);
  idateo = dateo;
  f77name(incdatr)(datev, &idateo, &deltat);
  mode = -3;
  f77name(newdate)(datev,&idatev00,&idatev01,&mode);
  sprintf(pdfdatev,"%08d.%06d",idatev00,idatev01/100);
  }


/* -------------------------------------------------------------------------------------------------- */

void FldMgrRescaleWW(int n, int echelleWW, int oldEchelleWW)
{
  int i,j,npts;
  float *ww,newscale;

  newscale = (float)echelleWW/(float)oldEchelleWW;
  if (fmflds[n].natureTensorielle > SCALAIRE)
      {
      if (fmflds[n].coupe.ww3d)
        {
        npts = fmflds[n].dst.ni*fmflds[n].dst.nj;
        for (j=0; j < fmflds[n].coupe.nbNiveauxCoupe; j++)
            {
            ww = fmflds[n].coupe.ww3d[j];
            for (i=0; i < npts; i++)
              {
              ww[i] *= newscale;
              }
            }

        npts = fmflds[n].coupe.niCoupe*fmflds[n].coupe.njCoupe;
        for (i=0; i < npts; i++)
            {
            ww = fmflds[n].coupe.ww2d;
            for (i=0; i < npts; i++)
              {
              ww[i] *= newscale;
              }
            }
        }
      }
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrCalcMinMaxUVW(int indChamp)
{
  int n,npts,un,zero,i,j;

  float *uu,*vv,*ww;
  float tmpww,tmpuvw;
  int amplifWW;

  n = indChamp;
  npts = fmflds[n].dst.ni*fmflds[n].dst.nj;
  un = 1;
  zero = 0;

  amplifWW = WindMgrGetEchelleWW();

  for (j=0; j < fmflds[n].coupe.nbNiveauxCoupe; j++)
      {
      uu = fmflds[n].coupe.uu3d[j];
      vv = fmflds[n].coupe.vv3d[j];
      ww = fmflds[n].coupe.ww3d[j];

      tmpww = ww[0]*(float)amplifWW;
      fmflds[n].coupe.UVWmin3d[NO_OP]=sqrt(uu[0]*uu[0]+vv[0]*vv[0]+tmpww*tmpww);
      fmflds[n].coupe.UVWmax3d[NO_OP]=fmflds[n].coupe.UVWmin3d[NO_OP];

      for (i=1; i < npts; i++)
        {
        tmpww = ww[i]*(float)amplifWW;
        tmpuvw = sqrt(uu[i]*uu[i]+vv[i]*vv[i]+tmpww*tmpww);

        fmflds[n].coupe.UVWmin3d[NO_OP] = fmflds[n].coupe.UVWmin3d[NO_OP] < tmpuvw ? fmflds[n].coupe.UVWmin3d[NO_OP] : tmpuvw;
        fmflds[n].coupe.UVWmax3d[NO_OP] = fmflds[n].coupe.UVWmax3d[NO_OP] > tmpuvw ? fmflds[n].coupe.UVWmax3d[NO_OP] : tmpuvw;
        }
      }
  }

/* -------------------------------------------------------------------------------------------------- */

void FldMgrRescaleUUVV(int indChamp)
{
  int i,k;

  for (k=0; k < fmflds[indChamp].coupe.nbNiveauxCoupe; k++)
      {
      for (i=0; i < fmflds[indChamp].dst.ni*fmflds[indChamp].dst.nj; i++)
        {
        fmflds[indChamp].coupe.uu3d[k][i] *= 1853.0/3600.0;
        fmflds[indChamp].coupe.vv3d[k][i] *= 1853.0/3600.0;
        }
      }
  }

/* -------------------------------------------------------------------------------------------------- */

int FldMgrDefinirGrille()
{
  int gdin,gdout;

  char grtyp, grref,grtyp_a;
  int ig1ref, ig2ref, ig3ref, ig4ref;
  int ni, nj, ig1, ig2, ig3, ig4;

  fmflds[0].src.gdid = c_ezgdefrec(fmflds[0].src.ni, fmflds[0].src.nj, fmflds[0].src.grtyp,
                      fmflds[0].src.ig1, fmflds[0].src.ig2, fmflds[0].src.ig3, fmflds[0].src.ig4);
  gdin = fmflds[0].src.gdid;
  gdout = c_ezgetgdout();
  c_ezgprm(gdin,&grtyp, &ni, &nj, &ig1, &ig2, &ig3, &ig4);

  switch (grtyp)
    {
    case 'Y':
      grtyp = 'L';
      c_ezgxprm(gdin, &ni, &nj, &grtyp, &ig1, &ig2, &ig3, &ig4, &grref, &ig1ref, &ig2ref, &ig3ref, &ig4ref);
      if (gdout == -1)
         {
         if (grref == 'L')
           {
           grtyp_a = 'B';
           gdout = c_ezgdefrec(361, 181, &grtyp_a, 0, 0, 0, 0);
           InitMapInfo(grtyp_a, 361, 181, 0,0,0,0);
           ThisIsTheCurrentGrid(grtyp_a, 361, 181, 1, 0,0,0,0);
           c_wglssp(1.0, 1.0, 361.0, 181.0, 0, 0, 0, 0, 1);
           }
         }
/*      if (grref == 'O')
         {
         InitMapInfo(grtyp, ni, nj, ig1, ig2, ig3, ig4);
         ThisIsTheCurrentGrid(grtyp, ni, nj, 1, ig1, ig2, ig3, ig4);
         }*/
      break;

    default:
       if (gdout == -1)
          {
          c_ezgprm(gdin,&grtyp, &ni, &nj, &ig1, &ig2, &ig3, &ig4);
          }
       else
          {
          c_ezgprm(gdout,&grtyp, &ni, &nj, &ig1, &ig2, &ig3, &ig4);
          }
       InitMapInfo(grtyp, ni, nj, ig1, ig2, ig3, ig4);
      ThisIsTheCurrentGrid(grtyp, ni, nj, 1, ig1, ig2, ig3, ig4);
      break;
    }

  return gdout;
}


int FldMgrFlagMissingValues(_Champ *champ)
{
  int un = 1;
  int zero = 0;
  unsigned int k, bitpos,mask_ni,mask_nj,mask_nk;
  int gdin, gdout, npts_src, npts_dst,cleMasque,ier;
  float *masque_src;
  unsigned int *src_missing;
  float *flt_src_missing;
  char typvar_masque[4];
  float huge;
  int i;
  int dateo, deet, npas, ni,nj,nk,nbits,datyp,ip1,ip2,ip3,ig1,ig2,ig3,ig4,swa,lng,dltf,ubc,extra1,extra2,extra3;
  char nomvar[8],etiket[16],typvar[4],grtyp[2];

  huge = HUGE_VAL;

  /*
  if (champ->src.grtyp[0] == 'X' || champ->src.grtyp[0] == 'Y')
    {
    return 0;
    }
    */

  if (champ->domaine != XY)
    {
    return 0;
    }

  if (champ->src.grtyp[0] == 'Y')
    {
    return 0;
    }

  if (champ->natureTensorielle == VECTEUR)
    {
    return 0;
    }

  npts_src = champ->src.ni * champ->src.nj;
  npts_dst = champ->dst.ni * champ->dst.nj;
  f77name(sminmax2)(&champ->min, &champ->max, &champ->min2, &champ->max2,
        champ->fld_orig, &champ->src.ni, &champ->src.nj, &un, &un, &champ->src.ni, &champ->src.nj);
  if (champ->typvar[1] == '@' && champ->typvar[0] != '@')
    {
    strcpy(typvar_masque, "@@");
    ier = f77name(fst_get_mask_key)(&cleMasque, &champ->cle, &zero, &champ->iun);
/*    cleMasque = c_fstinf(champ->iun, &mask_ni, &mask_nj, &mask_nk,champ->datev,champ->etiket,champ->ip1,champ->ip2, champ->ip3,typvar_masque, champ->nomvar);*/
     if (cleMasque >= 0)
      {
      if (champ->src.missing != NULL)
        {
        free(champ->src.missing);
        }

      ier = c_fstprm(cleMasque, &dateo, &deet, &npas, &ni, &nj, &nk, &nbits,
               &datyp, &ip1, &ip2, &ip3, typvar, nomvar, etiket, grtyp,
               &ig1, &ig2, &ig3, &ig4,&swa, &lng, &dltf, &ubc,&extra1, &extra2, &extra3);

      src_missing = (unsigned int *) malloc(npts_src*sizeof(int));

      if (datyp == 1 || datyp == 134)
         {
         flt_src_missing = (float *) malloc(npts_src*sizeof(float));
         ier = c_fstluk(flt_src_missing, cleMasque, &mask_ni, &mask_nj, &mask_nk);
         champ->src.missing = calloc((1+npts_src/32), sizeof(int));
         for (i=0; i < npts_src; i++)
            {
            src_missing[i] = (int)(flt_src_missing[i]+0.5);
            }
         free(flt_src_missing);
         }
      else
         {
         ier = c_fstluk(src_missing, cleMasque, &mask_ni, &mask_nj, &mask_nk);
         }

      champ->src.missing = calloc((1+npts_src/32), sizeof(int));

      ier = compact_mask(champ->src.missing, src_missing, npts_src);
      free(src_missing);
      champ->missingVal = 1.01*champ->max;
      f77name(drl_set_spval)(&(champ->missingVal));
      champ->missingFlag = FROM_FSTD;
        champ->stats_src.to_be_updated = 1;
        champ->stats_dst.to_be_updated = 0;
        champ->stats_zoom.to_be_updated = 0;
        MettreAJourStats();

      }
    }
  else
    {
    /* On recherche un flag de valeurs manquantes selon la convention
       missing_code = max + 0.1 * (max-min)
    */
    if ((champ->max - champ->max2) >= 0.09*(champ->max2-champ->min) && champ->max2 != champ->min)
      {
      champ->missingFlag = FROM_MINMAX;
      champ->missingVal = champ->max;
      f77name(drl_set_spval)(&(champ->missingVal));
      if (champ->src.missing != NULL)
        {
        free(champ->src.missing);
        }
      champ->src.missing = calloc((1+npts_src/32), sizeof(int));
      masque_src = calloc(npts_src,sizeof(float));
      for (k=0; k < npts_src; k++)
        {
        bitpos = k - ((k >> 5) << 5);

        if (champ->fld_orig[k] == champ->max)
          {
          champ->src.missing[k >> 5] |= ((unsigned int)0 << bitpos);
          masque_src[k] = 0.0;
          }
        else
          {
          champ->src.missing[k >> 5] |= ((unsigned int)1 << bitpos);
          masque_src[k] = 1.0;
         }
        }
      }
    else
      {
      champ->missingFlag = NOT_MISSING;
      }
    }

  if (champ->missingFlag != NOT_MISSING)
    {
    if (champ->dst.missing != NULL)
      {
      free(champ->dst.missing);
      }

    champ->dst.missing = calloc((1+npts_dst/32), sizeof(int));
    gdin  = c_ezgetgdin();
    gdout = c_ezgetgdout();
    c_rec_ezsint_mask(champ->dst.missing, champ->src.missing, gdin, gdout);
    champ->stats_src.to_be_updated = 0;
    champ->stats_dst.to_be_updated = 1;
    champ->stats_zoom.to_be_updated = 1;
    MettreAJourStats();

/*    for (i=0; i < npts_dst; i++)
      {
      if (!GETMSK(champ->dst.missing, i))
        {
        champ->fld[i] = champ->missingVal;
        }
      }*/
    }
    return 0;
}

void FldMgrLoad_HY(int indChamp)
  {
  _Champ *champ;
  int ier, ier2;
  char nomvar[8] = "HY     ";
  int moins1 = -1;

  FldMgrGetChamp(&champ, indChamp);
  ier = f77name(read_decode_hyb)(&(champ->iun), nomvar, &(champ->ip2),&(champ->ip3) ,champ->etiket, &(champ->datev), &(champ->coupe.ptop), &(champ->coupe.pref), &(champ->coupe.rcoef), (F2Cl) 8, (F2Cl) 16);
  if (ier < 0)
    {
    ier2 = f77name(read_decode_hyb)(&(champ->iun), nomvar, &moins1,&moins1 ,champ->etiket, &(champ->datev), &(champ->coupe.ptop), &(champ->coupe.pref), &(champ->coupe.rcoef), (F2Cl) 8, (F2Cl) 16);
    if (ier2 < 0)
      {
      champ->coupe.ptop = 0.;
      champ->coupe.pref = 1.0;
      champ->coupe.rcoef = 1.0;
      }
    }

   fprintf(stderr, "HY: ptop: %f pref: %f rcoef:%f\n", champ->coupe.ptop, champ->coupe.pref,champ->coupe.rcoef);
  }
