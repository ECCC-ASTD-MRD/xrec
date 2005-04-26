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
#include <memory.h>
#include <rpnmacros.h>
#include <math.h>
#include <gmp.h>
#include <wgl.h>

int DrawPointValues(_Champ *, int, int, int, int, int, int [], int);
void PointerFleche(float xdepart, float ydepart, float dirVent, float vitVent, int rayon, int width);
void PointerVent(float xdepart, float ydepart, float dirVent, float vitVent, int rayon);



static char *lblAvrtRecordsManquants[]={"\nLes descripteurs '^^' et '>>'\nsont manquants. Le champ sera affiche\nsans geographie\n",
                               "\nThe records '^^' and '>>' are missing.\nThe field will be displayed without geography\n"};

extern GeoMapInfoStruct mapInfo;
extern _Viewport   viewp;

void CheckForPointValues(_Champ *champ, int indChamp)
{
}

extern float PtValMgrGetSymbolSize();

DrawPointValues(_Champ *champ, int indChamp, int fontSize, int lineThickness, int fore, int back, int cols[], int nbcols)
{
  
  int i, ii,npts;
  int i1, j1, i2, j2;
  float x1, y1, x2, y2, fx1, fx2, fy1, fy2, xmin, xmax, ymin, ymax;
  int hauteurTexte, largeurTexte;
  int offsetX, offsetY;
  int indChampRel;
  char strVal[64], tempVal[64];
  float vitesse, direction, u, v, lat, lon;
  int rayon,rayon2;
  float rayon_orig;
  float delta, rayonDeg;
  int un, op, grsrc, grdst,ier; 
  int colorStatus, latlonStatus, valueStatus, typeSymbole, couleur, unitesRayon, displayMode;
  int *tmpInds, *tmpRayons;
  float *tmpVals, *lats, *lons;
  float rmin, rmax, tmplat,tmpuu,tmpvv;
  int longueurFleche = WindMgrGetLongueur();

  _Champ *champRef, *tmpchamp;
  float uvmin, uvmax;

  un = 1;
  npts = champ->src.ni*champ->src.nj*champ->src.nk;
  colorStatus = PtValMgrGetColorStatus();
  valueStatus = PtValMgrGetAffichageValeurs();
  latlonStatus = PtValMgrGetAffichageLatlon();
  typeSymbole  = PtValMgrGetSymboleType();
  rayon_orig = PtValMgrGetSymbolSize();
  unitesRayon = PtValMgrGetUnitesSymbole();

  tmpInds = (int *) malloc(npts*sizeof(int));
  tmpRayons = (int *) malloc(npts*sizeof(int));
  tmpVals = (float *) malloc(npts*sizeof(float));
  lats = (float *) malloc(npts*sizeof(float));
  lons = (float *) malloc(npts*sizeof(float));
  
  grsrc = c_ezqkdef(champ->src.ni, champ->src.nj, champ->src.grtyp, champ->src.ig1, champ->src.ig2, champ->src.ig3, champ->src.ig4, 1);
  grdst = c_ezgetgdout();
  ier = c_gdll(grsrc, lats, lons);
  c_wglfsz(fontSize);
  c_wgllwi(lineThickness);
  FldMgrGetChamp(&champRef, 0);
  if (champRef->domaine == VALEURS_PONCTUELLES)
    indChampRel = indChamp;
  else
    indChampRel = indChamp - 1;

  op = CtrlMgrGetMathOp();

  /* start patch */
  if (op != NO_OP)
    {
      indChampRel--;
    }
  /* end patch */

  if (champ->natureTensorielle == SCALAIRE)
    {
    f77name(sminmax)(&champ->localMin, &champ->localMax, champ->fld,  &champ->src.ni, &champ->src.nj, &un, &un, &champ->src.ni, &champ->src.nj);
    }
  else
    {
    f77name(sminmax)(&champ->localMin, &champ->localMax, champ->module,  &champ->src.ni, &champ->src.nj, &un, &un, &champ->src.ni, &champ->src.nj);
    }

  hauteurTexte = c_wglasi("1234",4) + c_wgldsi("1234", 4);

  offsetY = ROUND(rayon_orig);
  switch(indChampRel)
    {
    case 3:
      offsetY += (int)(3.6 * hauteurTexte) + fontSize/2;
      break;

    case 2:
      offsetY += (int)(2.4 * hauteurTexte) + fontSize/2;
      break;

    case 1:
      offsetY += (int)(1.2 * hauteurTexte) + fontSize/2;
      break;

    case 0:
      offsetY += fontSize/2;
      break;
    }

  c_wglgvx(&xmin, &ymin, &xmax, &ymax);
  switch(unitesRayon)
    {
    case PIXELS:
    break;

    case DEGRES:
    rayonDeg = rayon_orig;
    break;

    case KM:
    rayonDeg = (float) rayon_orig / 111.11;
    break;

    case NAUTICAL_MILES:
    rayonDeg = (float) rayon_orig / 60.0;
    break;

    default:
    break;
    }
    
    switch(unitesRayon)
      {
      case PIXELS:
      break;
      
      case DEGRES:
      case KM:
      case NAUTICAL_MILES:
      for (i=0; i < npts; i++)
        {
        tmplat = lats[i] - (float)rayonDeg;
        ier = c_gdxyfll(grdst,&x1, &y1, &lats[i], &lons[i], 1);
        ier = c_gdxyfll(grdst,&x2, &y2, &tmplat, &lons[i], 1);
        c_xy2fxfy(&fx1, &fy1, x1, y1);
        c_xy2fxfy(&fx2, &fy2, x2, y2);
        c_wglxai(&i1, &j1, fx1, fy1);
        c_wglxai(&i2, &j2, fx2, fy2);
        tmpRayons[i] = ROUND(sqrt((j2-j1)*(j2-j1)+(i2-i1)*(i2-i1)));
        tmpRayons[i] = tmpRayons[i] > 0 ? tmpRayons[i] : 1;
        }
      break;
            
      default:
      break;
      }
      
      
    
    rmin = champRef->min;
    rmax = champRef->max;
    uvmax = 0.0;
    for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&tmpchamp, i);
      if (tmpchamp->natureTensorielle == VECTEUR)
        {
        if (tmpchamp->uvmax[NO_OP] > uvmax) uvmax = tmpchamp->uvmax[NO_OP];
        }
      }
      
     uvmin = uvmax;
    for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&tmpchamp, i);
      if (tmpchamp->natureTensorielle == VECTEUR)
        {
        if (tmpchamp->uvmin[NO_OP] < uvmax) uvmin = tmpchamp->uvmin[NO_OP];
        }
      }
      
    if (champ->natureTensorielle == SCALAIRE)
      {
    if (champRef->intervalles[0] != 0.0)
      {
      AjusterMinMax(&rmin, &rmax, champRef->facteur, champRef->intervalles[0]);
      }
      delta = rmax - rmin;
      memcpy(tmpVals, champ->fld, npts*sizeof(float));
    c_wglcalcols(tmpInds, tmpVals, npts, rmin, delta, champ->intervalles, champ->nbIntervalles, champ->facteur, nbcols);
      }
    else
      {
    if (champRef->intervalles[0] != 0.0)
      {
      AjusterMinMax(&uvmin, &uvmax, champRef->facteur, champRef->intervalles[0]);
      }
      delta = uvmax - uvmin;
      memcpy(tmpVals, champ->module, npts*sizeof(float));
      c_wglcalcols(tmpInds, tmpVals, npts, uvmin, delta, champ->intervalles, champ->nbIntervalles, champ->facteur, nbcols);
      }
    for (i=0; i < npts; i++)
      {
      c_xy2fxfy(&x1, &y1, champ->x[i], champ->y[i]);
      if (xmin < x1 && x1 < xmax && ymin < y1 && y1 < ymax)
        {
        if (unitesRayon == PIXELS)
          {
          rayon = ROUND(rayon_orig);
          rayon = rayon > 0 ? rayon: 1;
          }
        else
          {
          rayon = tmpRayons[i];
          }
        c_wglxai(&i1, &j1, x1, y1);
        couleur = ROUND(tmpInds[i]); 
        couleur = couleur < 0 ? 0 : (couleur > (nbcols - 1) ? (nbcols - 1): couleur);
        rayon2 = ROUND(1.2 * rayon);
        switch(typeSymbole)
          {
          case CROIX:
          switch(colorStatus)
            {
            case 1:
            c_wglcol(cols[couleur]);
            c_wglcfi(i1, j1, rayon);
            c_wglcol(fore);
            c_wglcli(i1, j1, rayon);
            c_wglrli(i1-lineThickness-rayon2, j1, i1+lineThickness+rayon2, j1);
            c_wglrli(i1, j1-lineThickness-rayon2, i1, j1+lineThickness+rayon2);
            break;
            
            case 0:
            c_wglcol(fore);
            c_wglcli(i1, j1, rayon);
            c_wglrli(i1-lineThickness-rayon2, j1, i1+lineThickness+rayon2, j1);
            c_wglrli(i1, j1-lineThickness-rayon2, i1, j1+lineThickness+rayon2);
            break;
            }
          break;

          case CERCLE:
          switch (colorStatus)
            {
            case 1:
            c_wglcol(cols[couleur]);
            c_wglcfi(i1, j1, rayon);
            break;
            
            case 0:
            c_wglcol(fore);
            c_wglcli(i1, j1, rayon);
            break;
            }
          break;

          case RECTANGLE:
          switch (colorStatus)
            {
            case 1:
            c_wglcol(cols[couleur]);
            c_wglrfi(i1-rayon, j1-rayon, i1+rayon, j1+rayon);
            break;
            
            case 0:
            c_wglcol(fore);
            c_wglrli(i1-rayon, j1-rayon, i1+rayon, j1+rayon);
            break;
            }
          break;
          }
        }
      switch (champ->natureTensorielle)
        {
        case SCALAIRE:
        break;
        
        case VECTEUR:
              grdst = c_ezgetgdout();
              vitesse = sqrt(champ->uu[i]*champ->uu[i] + champ->vv[i]*champ->vv[i]);
              if (vitesse != 0.0)
                direction = 270.0 - 57.29577951 * atan2(champ->vv[i],champ->uu[i]);
              else
                direction = 0.0;

              if (direction < 0.0)
                direction += 360.0;

              if (direction > 360.0)
                direction -= 360.0;
              c_gduvfwd(grdst, &tmpuu, &tmpvv, &vitesse, &direction, &lats[i], &lons[i], 1);
              vitesse = sqrt(tmpuu*tmpuu + tmpvv*tmpvv);
              if (vitesse != 0.0)
                direction = 270.0 - 57.29577951 * atan2(tmpvv,tmpuu);
              else
                direction = 0.0;

              if (direction < 0.0)
                direction += 360.0;

              if (direction > 360.0)
                direction -= 360.0;
              
              displayMode = WindMgrGetDisplayMode();
              if (displayMode == 0)
                {
                PointerVent(x1, y1, direction, vitesse, longueurFleche);
                }
              else
                {
                PointerFleche(x1, y1, direction, vitesse, (int)(vitesse/uvmax*longueurFleche), 1);
                }
/*                  temp = vit;
                  enhancefracs(&temp,1,0.0,vm,variation);
                  largeurFleche  = ROUND(1.0*epaisseur*temp);
                  longueurFleche = ROUND((float)longueur*temp);
                  longueurFleche = longueurFleche < 1 ? 1 : longueurFleche;
 */                                    
        break;
        }
       
      }

  if (valueStatus == 1)
    {
    for (i=0; i < npts; i++)
        {
        c_xy2fxfy(&x1, &y1, champ->x[i], champ->y[i]);
          if (xmin < x1 && x1 < xmax && ymin < y1 && y1 < ymax)
          {
            c_wglxai(&i1, &j1, x1, y1);
            if (champ->natureTensorielle == SCALAIRE)
              {
              sprintf(strVal, "%6.2f", champ->fld[i]/champ->facteur);
              largeurTexte =  c_wglwsi(strVal, strlen(strVal));
              }
            else
              {
              vitesse = sqrt(champ->uu[i]*champ->uu[i]+champ->vv[i]*champ->vv[i]);
              if (vitesse != 0.0)
                direction = 270.0 - 57.29577951 * atan2(champ->vv[i],champ->uu[i]);
              else
                direction = 0.0;

              if (direction < 0.0)
                direction += 360.0;

              if (direction > 360.0)
                direction -= 360.0;

              sprintf(strVal, "%03.0f",(float)(int)(direction + 0.5));
              sprintf(tempVal, "%03.0f",(float)(int)(vitesse + 0.5));

              strcat(strVal, "@");
              strcat(strVal, tempVal);
              largeurTexte = c_wglwsi(strVal, strlen(strVal));
              }

          DrawBoxedStr(strVal, i1 - largeurTexte / 2, j1 + offsetY, fontSize,back, fore);
          }
        }
      }

  if (latlonStatus == 1 && indChamp <= 1)
    {
    grsrc = c_ezqkdef(champ->src.ni, champ->src.nj, champ->src.grtyp, champ->src.ig1, champ->src.ig2, champ->src.ig3, champ->src.ig4, 1);
    ier = c_gdll(grsrc, lats, lons);
    for (i=0; i < npts; i++)
      {
      c_xy2fxfy(&x1, &y1, champ->x[i], champ->y[i]);
      if (xmin < x1 && x1 < xmax && ymin < y1 && y1 < ymax)
        {
        c_wglxai(&i1, &j1, x1, y1);
        sprintf(strVal, "(%04d) <%6.2f,%6.2f>", i,lats[i], lons[i]);
        largeurTexte =  c_wglwsi(strVal, strlen(strVal));
        

        DrawBoxedStr(strVal, i1 - largeurTexte / 2, j1 - offsetY - fontSize/2 - 3, fontSize,back, fore);
        }
      }
    }
    
  free(tmpInds);
  free(tmpVals);
  free(tmpRayons);
  free(lats);
  free(lons);


}











