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

#include <rec.h>
#include <gmp.h>
#include <wgl.h>
#include <rpnmacros.h>

extern GeoMapInfoStruct    mapInfo;
extern _Viewport viewp;
extern _XContour xc;
extern int recColorTable[];
extern int facteurLissage;
extern float labelPos[][4];

static char *labelOperationInvalide[]  = { "\
On ne peut effectuer d'operations arithmetiques entre\n\
des champs scalaires et vectoriels", "\
It is not possible to perform arithmetic operations\n\
between scalar and vector fields"};

ManipulerChampBiDimensionnel(indChamp)
int indChamp;
{
   float xdebut, ydebut, xfin, yfin;
   int idebut, jdebut, ifin, jfin;
   int largeurFenetre, hauteurFenetre;
   int lng,fond;
   float min,max,vmin,vmax,tmpmin,tmpmax;
   float *fld,*uu,*vv,*module;
   float *tmpfld,*tmpuu,*tmpvv,*tmpmodule;
   float *pfld;
   unsigned int *maskfld, *maskfld2;
   int statusMask, statusMask2;
   float missingVal, missingVal2;
   float *origFld;

   int i,npts;
   Hilo hilo[64];
   int hlcount;
   int hlnmax = 64;
   
   _Champ *champ,*champ2,*tmpchamp;
   int nbChampsActifs;
   int op,customFld;
   float *fld1, *fld2;

   customFld = False;

   tmpfld    = NULL;
   tmpmodule = NULL;

   op = CtrlMgrGetContextualMathOp(indChamp);
   npts = mapInfo.ni*mapInfo.nj;
   nbChampsActifs = FldMgrGetNbChampsActifs();
   FldMgrGetChamp(&champ, indChamp);
   FldMgrGetChamp(&champ2, indChamp+1);


   if (champ->natureTensorielle == SCALAIRE)
      {
      min = champ->fldmin[op];
      max = champ->fldmax[op];
      }
   else
      {
      if (xc.statuts[BARBULES])
        {
        GetMinMaxUV(&min, &max);
        }
      else
        {
        if (0 == strcmp(champ->nomvar,"UU"))
          {
          min = champ->uumin[op];
          max = champ->uumax[op];
          }
        else
          {
          min = champ->vvmin[op];
          max = champ->vvmax[op];
          }
        }
      }
   
   if (xc.statuts[TOPO])
     {
     if (champ->coupe.montagnes == NULL)
       {
       GetSurfacePressure(champ);
       }
     }
   
   if (op != NO_OP || xc.statuts[TOPO])
      {
      if (op != NO_OP)
        {
        if (0 != indChamp%2)
          {
          return;
          }
        }
      
      if (op != NO_OP && 0 == indChamp%2  && indChamp != (nbChampsActifs-1))
        {
        if (champ->diffValide == 0)
          {
          lng = c_getulng();
          MessageAvertissement(labelOperationInvalide[lng],AVERTISSEMENT);
          return;
          }
        }
      
      customFld = True;
      if (champ->natureTensorielle == SCALAIRE)
        {
        tmpfld = (float *)calloc(npts,sizeof(float));
        min = champ->fldmin[op];
        max = champ->fldmax[op];
        if (op != NO_OP)
          {
          DiffMgrCalcDiffs(tmpfld,champ->fld,champ2->fld,npts,op);
          if (GetValeursManquantesToggle())
            {
            statusMask = champ->missingFlag;
            statusMask2 = champ2->missingFlag;
            if (champ->missingFlag != NOT_MISSING)
              {
              maskfld = champ->dst.missing;
              missingVal = champ->missingVal;
              }
            
            if (champ2->missingFlag != NOT_MISSING)
              {
              maskfld2 = champ2->dst.missing;
              missingVal2 = champ2->missingVal;
              }
            
            origFld = (float *)malloc(npts*sizeof(float));
            memcpy(origFld, champ->fld, npts*sizeof(float));
            DiffMgrMergeMasks(indChamp, indChamp+1);
            for (i=0; i < npts; i++)
              {
              if (!GETMSK(champ->dst.missing,i))
                {
                tmpfld[i] = champ->missingVal;
                }
              }

            }
          }
        else
          {
          pfld = champ->fld;
          }
        }
      else
        {
        tmpuu = (float *)calloc(npts,sizeof(float));
        tmpvv = (float *)calloc(npts,sizeof(float));
        tmpmodule = (float *)calloc(npts,sizeof(float));
        if (op != NO_OP)
            {
            DiffMgrCalcDiffs(tmpuu,champ->uu,champ2->uu,npts,op);
            DiffMgrCalcDiffs(tmpvv,champ->vv,champ2->vv,npts,op);
            f77name(modulus)(tmpmodule,tmpuu,tmpvv,&npts);
            }
        else
            {
            for (i=0; i < npts; i++)
              {
              tmpuu[i] = champ->uu[i];
              tmpvv[i] = champ->vv[i];
              tmpmodule[i] = champ->module[i];
              }
            }
         } 
      }
   
   if (champ->natureTensorielle == SCALAIRE)
      {
      if (CUSTOM == DictMgrGetMinMaxMode(champ->nomvar))
        {
        DictMgrGetMinMaxValues(champ->nomvar,&min,&max);
        }
      }
   else
      {
      if (CUSTOM == DictMgrGetMinMaxMode("UV"))
        {
        DictMgrGetMinMaxValues("UV",&min,&max);
        }
      }
   
   if (champ->coupe.montagnes && xc.statuts[TOPO])
      {
      if (champ->coordonneeVerticale == PRESSION) 
        {
        if (champ->natureTensorielle == SCALAIRE)
          {
          for (i=0; i < npts; i++)
            {
            if (champ->coupe.montagnes[i] < champ->niveau)
              {
              tmpfld[i] = min;
              }
            else
              {
              tmpfld[i] = champ->fld[i];
              }
            }
          }
        else
          {
          for (i=0; i < npts; i++)
            {
            if (champ->coupe.montagnes[i] < champ->niveau)
              {
              tmpuu[i] = 0.0;
              tmpvv[i] = 0.0;
              tmpmodule[i] = 0.0;
              }
            }
          }
        }
      else
        {
        if (champ->natureTensorielle == SCALAIRE)
          {
          for (i=0; i < npts; i++)
            {
            if (champ->coupe.montagnes[i] > champ->niveau)
              {
              tmpfld[i] = min;
              }
            else
              {
              tmpfld[i] = champ->fld[i];
              }
            }
          }
        }
      }
   

   if (customFld)
      {
      if (champ->natureTensorielle == SCALAIRE)
        {
        pfld = tmpfld;
        uu = NULL;
        vv = NULL;
        }
      else
	       {
         if (WindMgrGetModulusState())
            {
            pfld = tmpmodule;
            uu = tmpuu;
            vv = tmpvv;
            }
         else
            {
            if (0 == strcmp(champ->nomvar, "UU"))
               {
               pfld = tmpuu;
               uu = tmpuu;
               vv = tmpvv;
               }
            else
               {
               pfld = tmpvv;
               uu = tmpuu;
               vv = tmpvv;
               }
            }
          }
      }
   else
      {
      if (champ->natureTensorielle == SCALAIRE)
        {
        pfld = champ->fld;
        uu = NULL;
        vv = NULL;
        }
      else
         {
         if (WindMgrGetModulusState())
            {
            pfld = champ->module;
            uu = champ->uu;
            vv = champ->vv;
            }
         else
            {
            if (0 == strcmp(champ->nomvar, "UU"))
               {
               pfld = champ->uu;
               uu = champ->uu;
               vv = champ->vv;
               }
            else
               {
               pfld = champ->vv;
               uu = champ->uu;
               vv = champ->vv;
               }
            }
         }
      }
   
   AfficherChampBiDimensionnel(indChamp,nbChampsActifs,pfld,min,max,uu,vv);

   if (customFld)
      {
      if (tmpfld)
        {
        free(tmpfld);
        }

      if (tmpmodule)
        {
        free(tmpuu);
        free(tmpvv);
        free(tmpmodule);
        }
      }
      
    if (op != NO_OP)
      {
      if (GetValeursManquantesToggle())
        {
        if (statusMask != NOT_MISSING)
          {
          champ->dst.missing =  maskfld;
          champ->missingFlag = champ->missingFlag;
          champ->missingVal = missingVal;
          }
        else
          {
          champ->dst.missing = NULL;
          champ->missingFlag = NOT_MISSING;
          champ->missingVal = missingVal;
          }

        memcpy(champ->fld, origFld, npts*sizeof(float));
        free(origFld);
        if (statusMask2 != NOT_MISSING)
          {
          champ2->dst.missing = maskfld2;
          champ2->missingFlag = statusMask2;
          champ2->missingVal = missingVal2;
          }
        }
      }
   }


