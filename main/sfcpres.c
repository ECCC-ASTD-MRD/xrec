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

GetSurfacePressure(champ)
_Champ *champ;
{
   _Champ bidon;
   int dateo,datev,ip2;
   int listeIP1[100];
   int i,ier,n,lngListe,lngMaxListe;
   int ni,nj,nk,npts;
   int cleP0,clePN,cleMT,cleGZtop,cleGZbottom,cleTTtop,cleTTbottom;
   int sommeMasque,coordonnee;
   
   float *pnm,*tmp, *mt,*ztop,*zbottom,*tttop,*ttbottom,ip1top,ip1bot,tmean;
   float *temp;
   int *masque;
   int lng,gdin,gdout;
   double deltaT;
   
   char *P0 = "P0";
   char *PN = "PN";
   char *MT = "MT";
   char *ME = "ME";
   char *GZ = "GZ";
   char *TT = "TT";
   char *nomMT = MT;
   
   static char *pasavecSIGMA[] = {"Cette option n'est pas disponible\nen coordonnees sigma",
				     "This option is not available\nin sigma coordinates"};
   static char *pasdeP0[] = {"Champ de pression a la surface non disponible\nCalcul a vos risques et perils",
				"Surface pressure field not available\nComputation done at your own risk"};
   static char *pasdeMT[] = {"Champ de topographie non disponible.\n Generation de PO impossible",
				 "Topography field not available.\n Cannot generate P0 field"};
   static char *pasdePNM[] = {"Champ de pression au niveau de la mer non disponible.\n Generation de PO impossible",
				 "Sea level pressure field not available.\n Cannot generate P0 field"};
   static char *pasdeGZ[] = {"Champ de geopotentiel non disponible.\n Generation de PO impossible",
				 "Geopotential field not available.\n Cannot generate P0 field"};
   static char *pasdeTT[] = {"Champ de temperature non disponible.\n Generation de PO impossible",
				 "Temperature field not available.\n Cannot generate P0 field"};

   lng = c_getulng();
   dateo = champ->date;
   if (champ->ip1 <= 12000 && champ->ip1 >= 2000)
      {
      coordonnee = SIGMA;
      }
   else
      {
      if (champ->ip1 >= 12001)
	 {
	 coordonnee = METRES;
	 }
      else
	 {
	 coordonnee = PRESSION;
	 }
      }
   
   deltaT = (champ->deet*champ->npas)/3600.0;
   f77name(incdatr)(&datev,&dateo,&deltaT);
   
   switch(coordonnee)
      {
      case SIGMA:
      MessageAvertissement(pasavecSIGMA[lng],AVERTISSEMENT);
      return -1;
      break;

      case METRES:
      cleP0 = c_fstinf(champ->iun, &ni, &nj, &nk,-1,"        ", -1,-1, -1," ", ME);

      if (cleP0 < 0)
         {
         MessageAvertissement(pasdeMT[lng],AVERTISSEMENT);
         return -1;
         }
      else
         {
         champ->coupe.montagnes = (float *) calloc(ni * nj * nk, sizeof(float));
         ier = c_fstluk(champ->coupe.montagnes, cleP0, &ni, &nj, &nk);
         return 0;
         }
      break;
      
      default:
      cleP0 = c_fstinf(champ->iun, &ni, &nj, &nk,
                       datev,champ->etiket, -1,
                       champ->ip2, champ->ip3, champ->typvar, P0);

      if (cleP0 < 0)
         {
         MessageAvertissement(pasdeP0[lng],AVERTISSEMENT);
         }
      else
         {
	 gdin = c_ezgetgdin();
	 gdout = c_ezgetgdout();
	 c_ezdefset(gdout,gdin);

         temp = (float *) calloc(ni * nj * nk, sizeof(float));
         ier = c_fstluk(temp, cleP0, &ni, &nj, &nk);
	 champ->coupe.montagnes = malloc(champ->dst.ni*champ->dst.nj*champ->dst.nk*sizeof(float));
	 c_ezsint(champ->coupe.montagnes,temp);
	 free(temp);

         return 0;
         }
            
      clePN = c_fstinf(champ->iun, &ni, &nj, &nk,
                       datev,champ->etiket, -1,
                       champ->ip2, champ->ip3, champ->typvar, PN);
          
      if (clePN < 0)
         {
         MessageAvertissement(pasdePNM[lng],AVERTISSEMENT);
         return -1;
         }
         
      cleMT = c_fstinf(champ->iun, &ni, &nj, &nk,-1,"        ", -1,-1, -1, " ", MT);
      
      if (cleMT < 0)
         {
         cleMT = c_fstinf(champ->iun, &ni, &nj, &nk,-1,"        ", -1,-1,-1," ", ME);
	 nomMT = ME;
         if (cleMT < 0) 
            {
            MessageAvertissement(pasdeMT[lng], AVERTISSEMENT);
            return -1;
            }
         }
       
      lngMaxListe = 100;
      ier = c_fstinl(champ->iun, &ni, &nj, &nk,
	         	   datev, champ->etiket,-1,
		         champ->ip2, champ->ip3, champ->typvar, GZ,
		         listeIP1, &lngListe, lngMaxListe);

	
      FldMgrVerConsistanceGrilles(*champ, listeIP1, &lngListe);
      FldMgrVerConsistanceNiveaux(*champ, listeIP1, &lngListe);
      FldMgrTrierClesSelonIP1(listeIP1,lngListe);
    
      if (lngListe < 2) 
         {
         MessageAvertissement(pasdeGZ[lng],AVERTISSEMENT);
         return -1;
         }

      npts = ni * nj * nk;        
      gdin = c_ezgetgdin();
      gdout = c_ezgetgdout();
      c_ezdefset(gdout,gdin);
      champ->coupe.montagnes      = (float *) calloc(npts, sizeof(float));
      pnm       =  (float *) calloc(npts, sizeof(float));
      mt       =  (float *) calloc(npts, sizeof(float));
      ztop     =  (float *) calloc(npts, sizeof(float));
      zbottom  =  (float *) calloc(npts, sizeof(float));
      tttop    = (float *) calloc(npts, sizeof(float));
      ttbottom = (float *) calloc(npts, sizeof(float));
    
      masque  = (int *)   calloc(npts, sizeof(int));

      cleGZbottom = 0;
      cleGZtop =  1;
    
      ier = c_fstluk(pnm, clePN, &ni, &nj, &nk);
      ier = c_fstluk(mt, cleMT, &ni, &nj, &nk);
    
      if (nomMT == MT)
	 {
	 for (i=0; i < npts; i++)
	    mt[i] *= 0.1;
	 }

      n = 0;

/* traitement separe de la premiere couche */
    
      sommeMasque = 0;
      bidon.cle = listeIP1[cleGZbottom];
      FldMgrGetFstPrm(&bidon);
      ip1bot = (float)bidon.ip1;
      
      ier = c_fstluk(zbottom,  listeIP1[cleGZbottom], &ni, &nj, &nk); 
      for (i=0; i < npts; i++)
	 zbottom[i] *= 10.0;
      cleTTbottom = c_fstinf(champ->iun, &ni, &nj, &nk,-1,"        ",bidon.ip1,-1,-1," ", TT);
      if (cleTTbottom < 0)
	 {
	 MessageAvertissement(pasdeTT[lng],AVERTISSEMENT);
	 return -1;
	 }
      ier = c_fstluk(ttbottom, cleTTbottom, &ni, &nj, &nk); 
      
      for (i=0; i < npts; i++)
         {
         champ->coupe.montagnes[i] = pnm[i];
         if (mt[i] <= 0.0)
            {
            masque[i] = 1;
            }
         else
            {
            if (mt[i] >= zbottom[i])
               {
               champ->coupe.montagnes[i] = (float) ip1bot;
               }
            else
               {
               champ->coupe.montagnes[i] = pnm[i]*exp((-9.8*mt[i])/(287.0*(273+ttbottom[i])));
               masque[i] = 1;
               }
            }
	 sommeMasque += masque[i];
         }
      
      bidon.cle = listeIP1[cleGZtop];
      FldMgrGetFstPrm(&bidon);
      ip1top = (float)bidon.ip1;
      
      ier = c_fstluk(ztop, listeIP1[cleGZtop], &ni, &nj, &nk); 
      for (i=0; i < npts; i++)
	 ztop[i] *= 10.0;

      cleTTtop = c_fstinf(champ->iun, &ni, &nj, &nk,-1,bidon.etiket,bidon.ip1,-1,-1," ", TT);
      if (cleTTtop < 0)
	 {
	 MessageAvertissement(pasdeTT[lng],AVERTISSEMENT);
	 return -1;
	 }

      ier = c_fstluk(tttop, cleTTtop, &ni, &nj, &nk); 
      while (sommeMasque != npts && cleGZtop < lngListe)
	 {
	 for (i=0; i < npts; i++)
            {
            if (masque[i] == 0)
               {
               if (mt[i] >= ztop[i])
                  {
                  champ->coupe.montagnes[i] = (float) ip1top;
                  }
               else
                  {
		  tmean = (tttop[i]+ttbottom[i])*0.5 + 273.0;
                  champ->coupe.montagnes[i] = ip1bot*(exp((-9.8*(mt[i]-zbottom[i]))/(287.0*tmean)));
                  masque[i] = 1;
                  }
               }
            }
	 
	 sommeMasque = 0;
	 for (i=0; i < npts; i++)
            {
            sommeMasque += masque[i];
            zbottom[i] = ztop[i];
            ttbottom[i] = tttop[i];
	    }
	 
	 ip1bot = ip1top;
	 cleGZtop++;     
	 
	 
	 bidon.cle = listeIP1[cleGZtop];
	 FldMgrGetFstPrm(&bidon);
	 ip1top = (float)bidon.ip1;
	 ier = c_fstluk(ztop, listeIP1[cleGZtop], &ni, &nj, &nk); 
	 for (i=0; i < npts; i++)
	    ztop[i] *= 10.0;
	 cleTTtop = c_fstinf(champ->iun, &ni, &nj, &nk,-1,bidon.etiket,bidon.ip1,-1,-1," ", TT);
	 if (cleTTtop < 0)
	    {
	    MessageAvertissement(pasdeTT[lng],AVERTISSEMENT);
	    return -1;
	    }
	 
	 ier = c_fstluk(tttop, cleTTtop, &ni, &nj, &nk); 
	 }
      
      temp = champ->coupe.montagnes;
      champ->coupe.montagnes = malloc(champ->dst.ni*champ->dst.nj*champ->dst.nk*sizeof(float));
      c_ezsint(champ->coupe.montagnes,temp);
      free(temp);
      free(pnm);
      free(mt);
      free(ztop);
      free(zbottom);
      free(tttop);
      free(ttbottom);
      free(masque);
      
      return 0;
      break;
      }
   }


