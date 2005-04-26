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

#include <stdio.h>
#include <math.h>
#include <wgl.h>
#include <rec.h>
#include <rpnmacros.h>

extern _XContour    xc;

void PointerVent(float xdepart, float ydepart, float dirVent, float vitVent, int rayon);

#define C2FTN(i,j,ni)  (int)((ni) * (j) + i)

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

CheckForUUandVV(champ)
_Champ *champ;
{
   int i,j,ier;
   int ni, nj, nk;
   char autreChamp[3],x[3],y[3],z[3];
   _Champ bidon;

   int datev, deltaT, tempDate,date2, mode;
   int lng;
   int nchamps;
   int listeCles[1000];
   int nbMaxCles = 1000;
   int found,nbCles;

   static char uu[] = "UU";
   static char vv[] = "VV";

   static char *uumanquant[] = {"Champ UU associe manquant: VV sera affiche comme un scalaire", 
          "Associated UU field missing: VV will be processed as a scalar" };

   static char *vvmanquant[] = {"Champ VV associe manquant: UU sera affiche comme un scalaire", 
          "Associated VV field missing: UU will be processed as a scalar" };

   int variation = WindMgrGetCroissance();

   int yymmdd,hhmmss;
   double dddeltaT;

   lng = c_getulng();

   if (!xc.statuts[TRAITEMENT_VECTORIEL])
     {
     champ->natureTensorielle = SCALAIRE;
     return;
     }

   if (((0 == strcmp(champ->nomvar, uu)) || (0 == strcmp(champ->nomvar, vv))))
      {
      tempDate = champ->dateo;
      dddeltaT = (double)(champ->deet*champ->npas)/3600.0;
      
      f77name(incdatr)(&date2, &tempDate, &dddeltaT);
      mode = -3;
      f77name(newdate)(&date2, &yymmdd, &hhmmss, &mode);
      
      bidon.cle = -1;

      if (0 == strcmp(champ->nomvar,"UU"))
  {
    ier = c_fstinl(champ->iun, &ni, &nj, &nk,
       -1, champ->etiket, champ->ip1,
       champ->ip2, champ->ip3, champ->typvar, vv,
       listeCles, &nbCles, nbMaxCles);

    if (nbCles == 0)
      {
        if (champ->domaine == XY)
    {
      MessageAvertissement(vvmanquant[lng],AVERTISSEMENT);
    }
        return;
      }
   champ->uu = champ->fld;
   champ->fld = NULL;

   champ->vv     = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
   FldMgrVerConsistanceGrilles(*champ, listeCles, &nbCles);

   i = 0;
   found = 0;
   while (i < nbCles && !found)
      {
      bidon.cle = listeCles[i];
      FldMgrGetFstPrm(&bidon);
      if (bidon.dateo == champ->dateo && bidon.npas == champ->npas  && bidon.deet == champ->deet)
         {
         found = 1;
         }
      i++;
      }

         if (bidon.cle >= 0)
            {
            ier = c_fstluk(champ->vv,bidon.cle, &ni, &nj, &nk);
            }
         else
            {
            for (i=0; i < champ->src.ni*champ->src.nj; i++)
               {
               champ->vv[0] = 0.0;
               }
            }
   }

      if (0 == strcmp(champ->nomvar,"VV"))
   {
   ier = c_fstinl(champ->iun, &ni, &nj, &nk,
       -1, champ->etiket, champ->ip1,
       champ->ip2, champ->ip3, champ->typvar, uu,
       listeCles, &nbCles, nbMaxCles);
    if (nbCles ==  0)
      {
        if (champ->domaine == XY)
    {
      MessageAvertissement(uumanquant[lng],AVERTISSEMENT);
    }
        return;
      }
   champ->vv = champ->fld;
   champ->fld = NULL;
   champ->uu     = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
   FldMgrVerConsistanceGrilles(*champ, listeCles, &nbCles);

   i = 0;
   found = 0;
   while (i < nbCles && !found)
      {
      bidon.cle = listeCles[i];
      FldMgrGetFstPrm(&bidon);
      if (bidon.dateo == champ->dateo && bidon.npas == champ->npas && bidon.deet == champ->deet)
         {
         found = 1;
         }
      i++;
      }

         if (bidon.cle >= 0)
            {
            ier = c_fstluk(champ->uu,bidon.cle, &ni, &nj, &nk);
            }
         else
            {
            for (i=0; i < champ->src.ni*champ->src.nj; i++)
               {
               champ->uu[0] = 0.0;
               }
            }
   }
      
      champ->natureTensorielle = VECTEUR;
      }
   
   }

CheckForWW(_Champ *champ)
  {
  int i,j,ier;
  int ni, nj, nk;
  char autreChamp[3],x[3],y[3],z[3];
  
  int datev, deltaT;
  int nchamps;
  int echelleWW;
  
  static char ww[] = "WW";
  static char tt[] = "TT";
  
  float niveau,rho;
  float empirique[3];
  
  float *wwptr,*ttptr, *ww_interpole;
  int tempDate;
  double dddeltaT;
  
  empirique[0] =  0.015344;
  empirique[1] =  0.00152572;
  empirique[2] = -3.091325e-7;
  
  tempDate = champ->dateo;
  dddeltaT = (double)(champ->deet*champ->npas)/3600.0;
  
  f77name(incdatr)(&datev, &tempDate, &dddeltaT);
  ttptr         = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
  champ->ww     = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
  wwptr = champ->ww;
  
  ier = c_fstlir(champ->ww, champ->iun, &ni, &nj, &nk, datev,
                champ->etiket, champ->ip1, champ->ip2, champ->ip3, champ->typvar, ww);
  
  
  if (champ->coordonneeVerticale == SIGMA)
    {
    niveau = champ->niveau * 1000.0;
    }
  else
    {
    niveau = champ->niveau;
    }
  
  ier = c_fstlir(ttptr, champ->iun, &ni, &nj, &nk, datev, champ->etiket, champ->ip1, champ->ip2, champ->ip3, champ->typvar, tt);
  if (ier < 0)
    {
    fprintf(stderr,"%s-%f-%s\n","<LoadWW> Level: ", champ->ip1, 
      "Cannot read associated temperature field\nApproximating the density\n\n");
    rho = empirique[0] + niveau * empirique[1] + niveau * niveau * empirique[2];
    for (i=0; i < champ->src.ni*champ->src.nj; i++)
      {
      wwptr[i] /= (-9.81*rho);
    /*   wwptr[i] *= 0.5147; */ /* Ancienne version */
      wwptr[i] /= 0.5147;
      }
    }
  else
    {
    for (i=0; i < champ->src.ni*champ->src.nj; i++)
      {
      rho = niveau*100.0/(287.0*(ttptr[i]+273.));
      if (rho != 0.0)
        {
        wwptr[i] /= (-9.81*rho);
        wwptr[i] /= 0.5147; /* Conversion en noeuds  - 1 noeud = 0.5147 ms-1 */
        }
      else
        {
        wwptr[i] = 0.0;
        }
      }
    }
  free(ttptr);
 
   
   ww_interpole = (float *) calloc(champ->dst.ni*champ->dst.nj, sizeof(float));
   c_ezsint(ww_interpole, champ->ww);
   free(champ->ww);
   champ->ww = ww_interpole;
   f77name(aminmax)(&champ->wwmin[NO_OP],&champ->wwmax[NO_OP],champ->ww,&champ->dst.ni,&champ->dst.nj);
   }

/**
************************************************************* 
************************************************************* 
**/

CalcWWForZCoord(champ)
_Champ *champ;
{
   int i,j,n,ier;
   int ni, nj, nk;
   
   int datev, ip1, coord, deltaT;
   int nchamps;
   float pression;
   
   static char tt[] = "TT";
   static char p0[] = "P0";
   static char gz[] = "GZ";
   
   float niveau,rho;
   float *gzptr,*ttptr,*ttcumul,*p0ptr;
   int tempDate;
   double dddeltaT;
   
   
   gzptr         = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
   ttptr         = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
   ttcumul       = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));
   p0ptr         = (float *) calloc(champ->src.ni*champ->src.nj, sizeof(float));

   tempDate = champ->dateo;
   dddeltaT = (double)(champ->npas*champ->deet)/3600.0;
   
   f77name(incdatr)(&datev, &tempDate, &dddeltaT);
   ier = c_fstlir(p0ptr, champ->iun, &ni, &nj, &nk, datev,
      champ->etiket, 0, champ->ip2, champ->ip3, champ->typvar, p0);

   for (n=0; n < champ->coupe.nbNiveauxCoupe; n++)
      {
      coord = ip12coord(champ->ip1);
      ip1 = lvl2ip1(champ->coupe.niveauxCoupe[n],coord);
      ier = c_fstlir(ttptr, champ->iun, &ni, &nj, &nk, datev,
                      champ->etiket, ip1, champ->ip2, champ->ip3, champ->typvar, tt);
      for (i=0; i < champ->src.ni*champ->src.nj; i++)
        {
        ttcumul[i] += ttptr[i];
        }
      }

   for (i=0; i < champ->src.ni*champ->src.nj; i++)
      {
      ttcumul[i] /= champ->coupe.nbNiveauxCoupe;
      ttcumul[i] += 273.0;
      }

   for (n=0; n < champ->coupe.nbNiveauxCoupe; n++)
      {
      coord = ip12coord(champ->ip1);
      ip1 = lvl2ip1(champ->coupe.niveauxCoupe[n],coord);
      ier = c_fstlir(ttptr, champ->iun, &ni, &nj, &nk, datev,
         champ->etiket, ip1, champ->ip2, champ->ip3, champ->typvar, tt);
      ier = c_fstlir(gzptr, champ->iun, &ni, &nj, &nk, datev,
         champ->etiket, ip1, champ->ip2, champ->ip3, champ->typvar, gz);

      for (i=0; i < champ->src.ni*champ->src.nj; i++)
   {
   pression = 100.0*p0ptr[i]*exp(-gzptr[i]*10.0*9.81/(287.0*ttcumul[i]));
   champ->coupe.ww3d[n][i] = -(287.0*(273.0+ttptr[i])*champ->coupe.ww3d[n][i])/(9.81*pression);
   champ->coupe.ww3d[n][i] *= 0.5147;
   }
      }
   
   free(gzptr);
   free(ttptr);
   free(ttcumul);
   free(p0ptr);
   }


/**
 *************************************************************  
 *************************************************************  
 **/

extern void PointerFleche(float xdepart, float ydepart, float dirVent, float vitVent, int rayon, int width);



AfficherFleches(uu, vv, ni, nj, couleur, linewidth, ventmax)
float *uu, *vv;
int ni, nj;
int couleur, linewidth;
float ventmax;
{
   int i,j, k;
   float x, y;
   float pt, vit, dir;
   int idebut, jdebut, ifin, jfin;
   int i1, i2, j1, j2;
   int largeurFenetre, hauteurFenetre;
   int increment, incrementX, incrementY;
   int idist, jdist, idist1, idist2, jdist1, jdist2;
   float xdist, ydist, x2, y2,vm;
   int longueurFleche,largeurFleche,longueur,longueurMax,epaisseur;
   float densite;
   float temp,uutmp,vvtmp;
   int idensite,annulationDemandee,displayMode;
   int un = 1;
   float ri,rj;
   int fni,fnj;

   int variation = WindMgrGetCroissance();
   
   extern float WindMgrGetDensite();
   extern int WindMgrGetLongueur();
   extern int WindMgrGetEpaisseur();
   extern int WindMgrGetDisplayMode();

   densite = WindMgrGetDensite();
   longueur = WindMgrGetLongueur();
   epaisseur = WindMgrGetEpaisseur();
   displayMode = WindMgrGetDisplayMode();

   annulationDemandee = False;
   fni = ni;
   fnj = nj;

   c_wglcol(couleur);
   c_wgllwi(linewidth);
   DefinirFenetreGrille(&idebut, &jdebut, &ifin, &jfin, ni, nj);

   vm=ventmax;
   if (densite == -1.0)
      {
      densite = 1.0;
      longueurMax = longueur;
      }
   else
      {
      longueurMax = ROUND(0.33*longueur);
      }
   
   ri = (float)idebut;
   while (ri < (float)ifin && !annulationDemandee)
      {
      annulationDemandee = c_wglanul();
      
      rj = (float)jdebut;
      while (rj < (float)jfin)
   {
   if (densite < 1.0)
      {
      f77name(ez_rgdint_3_nw)(&uutmp,&ri,&rj,&un,uu,&fni,&un,&fnj);
      f77name(ez_rgdint_3_nw)(&vvtmp,&ri,&rj,&un,vv,&fni,&un,&fnj);
      }
   else
      {
      k = C2FTN(ROUND(ri)-1,ROUND(rj)-1,ni);
      uutmp = uu[k];
      vvtmp = vv[k];
      }
   vit = sqrt(uutmp*uutmp+vvtmp*vvtmp);
   if (vit != 0.0)
      dir = 270.0 - 57.29577951 * atan2(vvtmp,uutmp);
   else
      dir = 0.0;
   
   if (dir < 0.0)
      dir += 360.0;
   
   if (dir > 360.0)
      dir -= 360.0;
   
   c_xy2fxfy(&x, &y, ri,rj);
   if (displayMode == 0)
      {
      PointerVent(x, y, dir, vit, longueur);
      }
   else
      {
      temp = vit;
      enhancefracs(&temp,1,0.0,vm,variation);
      largeurFleche  = ROUND(1.0*epaisseur*temp);
      longueurFleche = ROUND((float)longueur*temp);
      longueurFleche = longueurFleche < 1 ? 1 : longueurFleche;
      PointerFleche(x, y, dir, vit, longueurFleche, largeurFleche);
      }

   jdist = 0;
   c_wglxai(&idist1, &jdist1, x, y);
   while (jdist < longueurMax && rj <= (float)jfin)
      {
      c_xy2fxfy(&x2, &y2, ri, rj+densite);
      c_wglxai(&idist2, &jdist2, x2, y2);
      jdist = jdist2 - jdist1;
      rj+=densite;
      }
   }
   
      idist = 0;
      c_xy2fxfy(&x, &y, ri,rj);
      while (idist < longueurMax && ri <= (float)ifin)
        {
        c_xy2fxfy(&x2, &y2, ri+densite, rj);
        c_wglxai(&idist2, &jdist2, x2, y2);
        idist = idist2 - idist1;
        ri+=densite;
        }
      }
   c_wgllwi(1);
   }


RestoreScalarWindFields(champ)
_Champ *champ;
{
   int i,j,ier;
   int ni, nj, nk;
   char autreChamp[3];
   static float *buf;
   float temp1, temp2, dir, vit;

   champ->natureTensorielle = SCALAIRE;
   FldMgrFreeAllFlds(champ);
   FldMgrProcessChamp(champ);
   }
