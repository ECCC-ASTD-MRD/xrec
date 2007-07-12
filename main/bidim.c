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

#include <wgl.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>

extern GeoMapInfoStruct    mapInfo;
extern _Viewport viewp;
extern _XContour xc;
extern int recColorTable[];
extern int sizeRecColorTable;
extern int facteurLissage;
extern float labelPos[][4];


void AfficherChampBiDimensionnel(int indChamp,int nbChampsActifs,float *fld,float min,float max,float *uu,float * vv)
{
   int lissfac,fontSize;
   float xdebut, ydebut, xfin, yfin;
   int idebut, jdebut, ifin, jfin;
   int largeurFenetre, hauteurFenetre;
   int fond;
   int mdeb,ndeb,mfin,nfin;
   Hilo hilo[256];
   int hlcount;
   int hlnmax = 256;

   _Champ *champ;

   nbChampsActifs = FldMgrGetNbChampsActifs();
   FldMgrGetChamp(&champ, indChamp);

   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglgsx(&xdebut, &ydebut, &xfin, &yfin);

   DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, mapInfo.ni, mapInfo.nj);

   if (xc.statuts[ZOOM_LOCAL] && AUTO == DictMgrGetMinMaxMode(champ->nomvar))
      {
      DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, mapInfo.ni, mapInfo.nj);
      {
      if (champ->natureTensorielle == SCALAIRE)
        {
        f77name(sminmax)(&champ->localMin, &champ->localMax, fld,  &mapInfo.ni, &mapInfo.nj, &mdeb, &ndeb, &mfin, &nfin);
        min = champ->localMin;
        max = champ->localMax;
        }
      else
        {
        f77name(vsminmax)(&champ->localMin, &champ->localMax, uu, vv,  &mapInfo.ni, &mapInfo.nj, &mdeb, &ndeb, &mfin, &nfin);
        min = champ->localMin;
        max = champ->localMax;
        }
      }
   }


   lissfac = facteurLissage;
   if (xc.statuts[LISSAGE])
      {
      if (lissfac == -32767)
       {
       lissfac = CalculerFacteurLissage(mapInfo.ni, mapInfo.nj);
       }
      else
       {
       lissfac = facteurLissage;
       }
      }
   else
      {
      lissfac = 1;
      }

   c_wglssp(xdebut, ydebut, xfin, yfin, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
       {
       xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
       return;
       }
      }

   if (nbChampsActifs > 1 && indChamp == 0)
      EffacerLegende2();

   c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);
   if (!xc.statuts[COULEURS])
      AfficherPerimetreFenetre();

   c_wglgsp(&xdebut, &ydebut, &xfin, &yfin, &idebut, &jdebut, &ifin, &jfin);

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
   {
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   return;
   }
      }

   if (AfficherItem(indChamp, COULEURS))
      {
      c_wglssp(xdebut, ydebut, xfin, yfin, idebut, jdebut, ifin, jfin, 1);
      SetClipMask();
      if (xc.statuts[VALEURS_MANQUANTES] && champ->missingFlag)
        {
        c_wglcol(xc.attributs[FOND].indCouleurFore);
        c_wglrfi(viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2);
        c_wglfton_m(fld, champ->dst.missing, mapInfo.ni, mapInfo.nj, champ->intervalles,
              champ->nbIntervalles,
              champ->facteur, min, max,
            recColorTable, sizeRecColorTable,xc.flagInterrupt, lissfac);
        }
      else
        {
        c_wglfton(fld, mapInfo.ni, mapInfo.nj, champ->intervalles,
            champ->nbIntervalles,
            champ->facteur, min, max,
            recColorTable, sizeRecColorTable, xc.flagInterrupt, lissfac);
        }
      }
   else
      {
      if (indChamp == 0)
         {
         c_wglcol(xc.attributs[FOND].indCouleurFore);
         c_wglrfi(viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2);
         }
      }

   if (champ->natureTensorielle == VECTEUR && champ->cle >= 0 && xc.statuts[BARBULES])
     {
     if (0 != WindMgrGetLICState())
       {
       SetClipMask();
       AfficherLIC(uu, vv, mapInfo.ni, mapInfo.nj, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].epaisseur,max);
       }
     }


   if (xc.flagInterrupt)
      {
      if (c_wglanul())
         {
         xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
         return;
         }
      }

   if (indChamp == 0)
      {
      switch(xc.statuts[GEOGRAPHIE])
        {
        case TRUE:
                SetClipMask();
                c_gmpdrw();
                break;
      
        case FALSE:
                break;
        }
      }   /** if (indChamp > 0) **/

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

   if (xc.statuts[GRILLE_SOURCE])
      {
      SetClipMask();
      AfficherGrilleSource(indChamp);
      }

   if (xc.statuts[GRILLE] && (indChamp == 0))
      {
      SetClipMask();
      AfficherGrille();
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

   if (AfficherItem(indChamp, CONTOURS))
      {
      SetClipMask();
      c_wglcont(fld, champ->dst.ni, champ->dst.nj, champ->intervalles,
                champ->nbIntervalles, champ->facteur, min, max,
                xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].epaisseur,
                xc.attributs[indChamp].codeDash, xc.attributs[indChamp].style, lissfac, xc.flagInterrupt);
      }


   c_wgllwi(1);
   c_wglsld(0);

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }


   if (AfficherItem(indChamp, LABELS))
    {
    switch(xc.attributs[indChamp].couleurFore)
        {
        case BLANC:
        case JAUNE:
        case CYAN:
        fond = NOIR;
        break;
    
    default:
    fond = BLANC;
    break;
    }


    if (xc.attributs[indChamp].labelSize == 0)
      {
      fontSize = AttrMgrGetFontSizeLabels();
      }
    else
      {
      fontSize = xc.attributs[indChamp].labelSize;
      }

      SetClipMask();
      SetLabFontSize(fontSize);
      c_wgllab(fld, mapInfo.ni, mapInfo.nj, champ->intervalles, champ->nbIntervalles,
               champ->facteur, min, max, labelPos[indChamp%4], 4, xc.attributs[indChamp].indCouleurFore,
               xc.attributs[indChamp].indCouleurBack, indChamp, lissfac);
      }


   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        return;
        }
      }

   if (champ->natureTensorielle == VECTEUR && champ->cle >= 0 && xc.statuts[BARBULES])
      {
      SetClipMask();
      c_wgllwi(1);
      if (-1 != WindMgrGetDisplayMode())
        {
        AfficherFleches(uu,vv, mapInfo.ni, mapInfo.nj, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].epaisseur,max);
        if (0 != WindMgrGetDisplayMode())
            AfficherLegendeVent(max,10,10,0);
        }
      }

   if (AfficherItem(indChamp, VALEURS_CENTRALES))
      {
      hl_setFontSize(xc.attributs[indChamp].centralValSize);
      if (champ->natureTensorielle == SCALAIRE || !xc.statuts[BARBULES])
         {
         hl_find (fld, mapInfo.ni, mapInfo.nj, 1.0, hilo,&hlcount,hlnmax);
         hl_print (hilo,hlcount,champ->facteur, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].indCouleurBack);
         }
      else
         {
         if (WindMgrGetModulusState())
            {
            hl_find (fld, mapInfo.ni, mapInfo.nj, 1.0, hilo,&hlcount,hlnmax);
            hl_print (hilo,hlcount,champ->facteur, xc.attributs[indChamp].indCouleurFore,xc.attributs[indChamp].indCouleurBack);
            }
         }
      }

   if (xc.flagInterrupt)
      {
      if (c_wglanul())
        {
        xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
        f77name(xpanstatact)();
        f77name(xpanstatact)();
        f77name(xpanstatact)();
        f77name(xpanstatact)();
        f77name(xpanstatact)();
        return;
        }
      }
}


int AfficherItem(int indChamp,  int itemAAfficher)
{
   _Champ *champ;

   FldMgrGetChamp(&champ, indChamp);

   switch (itemAAfficher)
      {
      case COULEURS:
        if (indChamp != 0) return 0;
        if (!xc.statuts[COULEURS]) return 0;
        if (champ->natureTensorielle == SCALAIRE) return 1;

        if (xc.statuts[BARBULES])
           {
           if (WindMgrGetModulusState()) return 1;
           else return 0;
           }
        else return 1;
        break;

      case CONTOURS:
        if (xc.attributs[indChamp].displayContours == JAMAIS) return 0;
        if (xc.attributs[indChamp].displayContours == TOUJOURS) return 1;
        if (!xc.statuts[CONTOURS]) return 0;
        if (champ->natureTensorielle == SCALAIRE) return 1;

        if (xc.statuts[BARBULES])
           {
           if (WindMgrGetModulusState()) return 1;
           else return 0;
           }
        else return 1;
        break;

      case LABELS:
        if (xc.attributs[indChamp].displayLabels == JAMAIS) return 0;
        if (xc.attributs[indChamp].displayLabels == TOUJOURS) return 1;
        if (!xc.statuts[LABELS]) return 0;
        if (champ->natureTensorielle == SCALAIRE) return 1;

        if (xc.statuts[BARBULES])
           {
           if (WindMgrGetModulusState()) return 1;
           else return 0;
           }
        else return 1;
        break;

      case LEGENDE:
        if (xc.statuts[LEGENDE] == 0) return 0;
        if (xc.statutSuperposition  == 0)
          {
          if (indChamp == (FldMgrGetNbChampsActifs() -1))
            return 1;
          else
            return 0;
          }
        return 1;
        break;

      case LEGENDE_COULEUR:
        if (!xc.statuts[COULEURS]) return 0;
        if (!xc.statuts[LEGENDE_COULEUR]) return 0;
        if (champ->natureTensorielle == SCALAIRE) return 1;

        if (xc.statuts[BARBULES])
           {
           if (WindMgrGetModulusState()) return 1;
        if (indChamp == 0) return 0;
           else return 1;
           }
        else return 1;
        break;

      case VALEURS_CENTRALES:
        if (xc.attributs[indChamp].displayValCentrales == JAMAIS) return 0;
        if (xc.attributs[indChamp].displayValCentrales == TOUJOURS) return 1;
        if (!xc.statuts[VALEURS_CENTRALES]) return 0;
        if (champ->natureTensorielle == SCALAIRE) return 1;

        if (xc.statuts[BARBULES])
           {
           if (WindMgrGetModulusState()) return 1;
           else return 0;
           }
        else
           {
           return 1;
           }
        break;
      }
}
