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

#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <rmn/rpnmacros.h>

#define MODULE2D(a,b)   (sqrt(a*a+b*b))
#define MODULE3D(a,b,c) (sqrt(a*a+b*b+c*c))

void DiffMgrSetDiffs(float *fld1,float *fld2,float *opmin,float *opmax, int npts)
{
   int i,n;

   float diff,somme,abs_diff,abs_somme;

   opmin[NO_OP] = fld1[0];
   opmin[SUBTRACT] = fld1[0] - fld2[0];
   opmin[SOMME]    = fld1[0] + fld2[0];
   opmin[ABS_SUBTRACT] = fabs(fld1[0] - fld2[0]);
   opmin[ABS_SOMME] = fabs(fld1[0] + fld2[0]);

   for (n=0; n < 5; n++)
      {
      opmax[n] = opmin[n];
      }

   for (i=1; i < npts; i++)
      {
      diff = fld1[i] - fld2[i];
      somme = fld1[i] + fld2[i];
      abs_diff = fabs(diff);
      abs_somme = fabs(somme);
      
      opmin[NO_OP] = opmin[NO_OP] > fld1[i] ? fld1[i] : opmin[NO_OP];
      opmax[NO_OP] = opmax[NO_OP] < fld1[i] ? fld1[i] : opmax[NO_OP];
      
      opmin[SUBTRACT] = opmin[SUBTRACT] > diff ? diff : opmin[SUBTRACT];
      opmax[SUBTRACT] = opmax[SUBTRACT] < diff ? diff : opmax[SUBTRACT];
      
      opmin[SOMME] = opmin[SOMME] > somme ? somme : opmin[SOMME];
      opmax[SOMME] = opmax[SOMME] < somme ? somme : opmax[SOMME];
      
      opmin[ABS_SUBTRACT] = opmin[ABS_SUBTRACT] > abs_diff ? abs_diff : opmin[ABS_SUBTRACT];
      opmax[ABS_SUBTRACT] = opmax[ABS_SUBTRACT] < abs_diff ? abs_diff : opmax[ABS_SUBTRACT];
      
      opmin[ABS_SOMME] = opmin[ABS_SOMME] > abs_somme ? abs_somme : opmin[ABS_SOMME];
      opmax[ABS_SOMME] = opmax[ABS_SOMME] < abs_somme ? abs_somme : opmax[ABS_SOMME];
      
      }
   }

void DiffMgrCalcDiffs(float *out,float *fld1,float *fld2, int npts, int op)
{
   int i;

   switch (op)
      {
      case NO_OP:
      case MODULE:
      for (i=0; i < npts; i++)
        out[i] = fld1[i];
      break;
      
      case SUBTRACT:
      case MODULE_SUB:
      for (i=0; i < npts; i++)
        out[i] = fld1[i] - fld2[i];
      break;
      
      case SOMME:
      case MODULE_ADD:
      for (i=0; i < npts; i++)
        out[i] = fld1[i] + fld2[i];
      break;
      
      case ABS_SUBTRACT:
      for (i=0; i < npts; i++)
        out[i] = fabs(fld1[i] - fld2[i]);
      break;
      
      case ABS_SOMME:
      for (i=0; i < npts; i++)
        out[i] = fabs(fld1[i] + fld2[i]);
      break;
      }
   }


void DiffMgrCalcVDiffs2D(float *out,float *uu1,float *vv1,float *uu2,float *vv2,int npts,int op)
{
   int i;

   switch (op)
      {
      case NO_OP:
      for (i=0; i < npts; i++)
        out[i] = MODULE2D(uu1[i],vv1[i]);
      break;
      
      case SUBTRACT:
      case ABS_SUBTRACT:
      for (i=0; i < npts; i++)
        out[i] = MODULE2D(uu1[i] - uu2[i],vv1[i] - vv2[i]);
      break;
      
      case SOMME:
      case ABS_SOMME:
      for (i=0; i < npts; i++)
        out[i] = MODULE2D(uu1[i] + uu2[i],vv1[i] + vv2[i]);
      break;
      }
   }


void DiffMgrSetVDiffs2D(float *uu1,float *vv1,float *uu2,float *vv2,float *opmin,float *opmax,int npts)
{
   int i,n;

   float diffuu,diffvv,somme_uu,somme_vv;
   float diff,somme,module;

   diffuu = uu1[0] - uu2[0];
   diffvv = vv1[0] - vv2[0];

   somme_uu = uu1[0] + uu2[0];
   somme_vv = vv1[0] + vv2[0];

   opmin[NO_OP] = MODULE2D(uu1[0],vv1[0]);
   opmin[SUBTRACT] = MODULE2D(diffuu,diffvv);
   opmin[SOMME]    = MODULE2D(somme_uu,somme_vv);

   for (n=0; n < 5; n++)
      {
      opmax[n] = opmin[n];
      }

   for (i=1; i < npts; i++)
      {
      diffuu = uu1[i] - uu2[i];
      diffvv = vv1[i] - vv2[i];
      somme_uu = uu1[i] + uu2[i];
      somme_vv = vv1[i] + vv2[i];

      diff   = sqrt(diffuu*diffuu+diffvv*diffvv);
      somme   = sqrt(somme_uu*somme_uu+somme_vv*somme_vv);

      module = sqrt(uu1[i]*uu1[i]+vv1[i]*vv1[i]);
      opmin[NO_OP] = opmin[NO_OP] > module ? module : opmin[NO_OP];
      opmax[NO_OP] = opmax[NO_OP] < module ? module : opmax[NO_OP];
      
      opmin[SUBTRACT] = opmin[SUBTRACT] > diff ? diff : opmin[SUBTRACT];
      opmax[SUBTRACT] = opmax[SUBTRACT] < diff ? diff : opmax[SUBTRACT];
      
      opmin[SOMME] = opmin[SOMME] > somme ? somme : opmin[SOMME];
      opmax[SOMME] = opmax[SOMME] < somme ? somme : opmax[SOMME];
      }

   opmin[ABS_SUBTRACT] = opmin[SUBTRACT];
   opmax[ABS_SUBTRACT] = opmax[SUBTRACT];

   opmin[ABS_SOMME] = opmin[SOMME];
   opmax[ABS_SOMME] = opmax[SOMME];
   }

void DiffMgrSetVDiffs3D(float *uu1,float *vv1,float *ww1, float *uu2,float *vv2,float *ww2, float *opmin, float *opmax, int npts)
{
   int i,n;

   float diffuu,diffvv,diffww,somme_uu,somme_vv,somme_ww;
   float diff,somme,module;

   diffuu = uu1[0] - uu2[0];
   diffvv = vv1[0] - vv2[0];
   diffww = ww1[0] - ww2[0];

   somme_uu = uu1[0] + uu2[0];
   somme_vv = vv1[0] + vv2[0];
   somme_ww = ww1[0] + ww2[0];

   opmin[NO_OP] = sqrt(uu1[0]*uu1[0]+vv1[0]*vv1[0]+ww1[0]*ww1[0]);
   opmin[SUBTRACT] = sqrt(diffuu*diffuu+diffvv*diffvv+diffww*diffww);
   opmin[SOMME]    = sqrt(somme_uu*somme_uu+somme_vv*somme_vv+somme_ww*somme_ww);

   for (n=0; n < 5; n++)
      {
      opmax[n] = opmin[n];
      }

   for (i=1; i < npts; i++)
      {
      diffuu = uu1[i] - uu2[i];
      diffvv = vv1[i] - vv2[i];
      diffww = ww1[i] - ww2[i];

      somme_uu = uu1[i] + uu2[i];
      somme_vv = vv1[i] + vv2[i];
      somme_ww = ww1[i] + ww2[i];

      diff   = sqrt(diffuu*diffuu+diffvv*diffvv+diffww*diffww);
      somme   = sqrt(somme_uu*somme_uu+somme_vv*somme_vv+somme_ww*somme_ww);

      module = sqrt(uu1[i]*uu1[i]+vv1[i]*vv1[i]+ww1[i]*ww1[i]);
      opmin[NO_OP] = opmin[NO_OP] > module ? module : opmin[NO_OP];
      opmax[NO_OP] = opmax[NO_OP] < module ? module : opmax[NO_OP];
      
      opmin[SUBTRACT] = opmin[SUBTRACT] > diff ? diff : opmin[SUBTRACT];
      opmax[SUBTRACT] = opmax[SUBTRACT] < diff ? diff : opmax[SUBTRACT];
      
      opmin[SOMME] = opmin[SOMME] > somme ? somme : opmin[SOMME];
      opmax[SOMME] = opmax[SOMME] < somme ? somme : opmax[SOMME];
      }

   opmin[ABS_SUBTRACT] = opmin[SUBTRACT];
   opmax[ABS_SOMME] = opmax[SOMME];
   }

int DiffMgrGetNbChampsAffichables()
{
   int op,nbChampsActifs;

   op = CtrlMgrGetMathOp();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   switch (op)
      {
      case NO_OP:
      return nbChampsActifs;
      break;

      default:
      if (0 != nbChampsActifs%2)
        {
        return 1+nbChampsActifs/2;
        }
      else
        {
        return nbChampsActifs/2;
        }
      }
   }
   
void DiffMgrMergeMasks(int indChamp1, int indChamp2)
{
  _Champ *champ1, *champ2;
  int i, npts;
  unsigned int *new_mask, *new_mask32;
  
  FldMgrGetChamp(&champ1, indChamp1);
  FldMgrGetChamp(&champ2, indChamp2);
  
  npts = champ1->dst.ni * champ1->dst.nj;
  if (champ1->missingFlag != NOT_MISSING)
    {
    if (champ2->missingFlag != NOT_MISSING)
      {
      new_mask = calloc(npts,sizeof(unsigned int));
      new_mask32 = calloc((1+npts/32), sizeof(unsigned int));
      for (i=0; i < npts; i++)
        {
        new_mask[i] = GETMSK(champ1->dst.missing,i) & GETMSK(champ2->dst.missing,i);
        }
      compact_mask(new_mask32, new_mask, npts);
      champ1->dst.missing = new_mask32;
      for (i=0; i < npts; i++)
        {
        if (!GETMSK(champ1->dst.missing,i))
          {
          champ1->fld[i] = champ1->missingVal;
          }
        }

      free(new_mask);
      }
    }
  else
    {
    if (champ2->missingFlag != NOT_MISSING)
      {
      champ1->dst.missing = champ2->dst.missing;
      champ1->missingFlag = champ2->missingFlag;
      champ1->missingVal  = champ2->missingVal;
      for (i=0; i < npts; i++)
        {
        if (!GETMSK(champ1->dst.missing,i))
          {
          champ1->fld[i] = champ1->missingVal;
          }
        }
      }
    }  
}
