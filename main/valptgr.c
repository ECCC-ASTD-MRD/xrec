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
#include <gmp.h>
#include <wgl.h>
#include <souris.h>

extern _XContour xc;
extern GeoMapInfoStruct    mapInfo;
extern int fenetreAffichage;



/** ARGSUSED **/
XtCallbackProc AfficherValeursAuxPtsDeGrille(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int event;
   float val, val3, val2, val1, uu2, uu1, vv2, vv1;
   float uus[3],vvs[3];
   float vals[4],diffs[4],uudiffs[4],vvdiffs[4],uu[4],vv[4],dirs[4],vits[4];
   int i,l;
   int ix1, iy1;
   int largeurTexte[4], hauteurTexte;
   char strVal[4][64], tempVal[64];
   float x, y, xgrid,ygrid,direction, vitesse, u, v, tmplat, tmplon;
   int offsetX[4], offsetY[4];
   int un = 1;
   int nbChampsActifs;
   int op,gdid,ier;
   _Champ *champs[4];

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }
   
   c_wglsetw(fenetreAffichage);
   nbChampsActifs = FldMgrGetNbChampsActifs();
   nbChampsActifs = nbChampsActifs > 4 ? 4 : nbChampsActifs;

   for (i=0; i < nbChampsActifs; i++)
      {
      FldMgrGetChamp(&champs[i], i);
      }

   InvertWidget(w);
   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   c_wglfsz(18);
   hauteurTexte = c_wglasi("1234",4) + c_wgldsi("1234", 4);
   SetClipMask();
   op = CtrlMgrGetMathOp();

   if (nbChampsActifs == 1)
      {
      op = NO_OP;
      }
   
   event = c_wglbtx(&x, &y);
   c_fxfy2xy(&xgrid, &ygrid, x, y);
   
   while (event != BDROIT && event != NIL)
      {
      if ((champs[0]->fld != NULL || champs[0]->uu != NULL) && xgrid >= 1.0 && xgrid <= (float)mapInfo.ni && ygrid >= 1.0 && ygrid <= (float)mapInfo.nj)
         {
         switch(event)
            {
            case BGAUCH:
            ix1 = (int)(xgrid + 0.5);
            iy1 = (int)(ygrid + 0.5);
	    xgrid = (float) ix1;
	    ygrid = (float) iy1;
            
	    for (i=0; i < 4; i++)
	       {
	       vals[i] = 0.0;
	       uu[i] = 0.0;
	       vv[i] = 0.0;
	       }
	    
	    for (i=0; i < nbChampsActifs; i++)
	       {
	       if (champs[i]->domaine != VALEURS_PONCTUELLES)
		  {
		  switch (champs[i]->natureTensorielle)
		     {
		     case SCALAIRE:
                       f77name(grdval)(&vals[i], champs[i]->fld, &ix1, &iy1, &mapInfo.ni, &mapInfo.nj);
                       vals[i] /= champs[i]->facteur;
                       break;
                       
		     default:
                       if (!xc.statuts[BARBULES])
                          {
                          if (0 == strcmp(champs[i]->nomvar, "UU"))
                             {
                             f77name(grdval)(&vals[i], champs[i]->uu, &ix1, &iy1, &mapInfo.ni, &mapInfo.nj);
                             }
                          else
                             {
                             f77name(grdval)(&vals[i], champs[i]->vv, &ix1, &iy1, &mapInfo.ni, &mapInfo.nj);
                             }
                          }
                       else
                          {
                          c_xy2fxfy(&x, &y, (float)ix1, (float)iy1);
                          f77name(grdval)(&uu[i], champs[i]->uu, &ix1, &iy1, &mapInfo.ni, &mapInfo.nj);
                          f77name(grdval)(&vv[i], champs[i]->vv, &ix1, &iy1, &mapInfo.ni, &mapInfo.nj);
                          gdid = c_ezgetgdout();
			  ier = c_gdllfxy(gdid, &tmplat, &tmplon, &xgrid, &ygrid, 1);
			  ier = c_gdwdfuv(gdid, &vits[i], &dirs[i], &uu[i], &vv[i], &tmplat, &tmplon, 1);
                          break;
                          }
                     }
                  }
               }
            
            switch(op)
               {
               case NO_OP:
                 for (i=0; i < nbChampsActifs; i++)
                    {
                    if (champs[i]->domaine != VALEURS_PONCTUELLES)
                       {
                       if (champs[i]->natureTensorielle == SCALAIRE || !xc.statuts[BARBULES])
                          {
                          ConvertFloatToString(strVal[i], vals[i]);
                          }
                       else
                          {
                          sprintf(strVal[i], "%03.0f",(float)(int)(dirs[i] + 0.5));
                          sprintf(tempVal, "%03.0f",(float)(int)(vits[i] + 0.5));
                          
                          strcat(strVal[i], "@");
                          strcat(strVal[i], tempVal);
                          largeurTexte[i] = c_wglwsi(strVal[i], strlen(strVal[i])); 
                          }
                       }
                    largeurTexte[i] = c_wglwsi(strVal[i], strlen(strVal[i])); 
                    }
                 break;
                 
               case SUBTRACT:
                 for (i=0; i < nbChampsActifs; i+=2)
                    {
                    if (champs[i]->natureTensorielle == SCALAIRE || !xc.statuts[BARBULES])
                       {
                       diffs[i]=vals[i] - vals[i+1];
                       sprintf(strVal[i], "%12.6g - %12.6g = %12.6g", vals[i], vals[i+1], diffs[i]);
                       }
                    else
                       {
                       uudiffs[i] = uu[i] - uu[i+1];
                       vvdiffs[i] = vv[i] - vv[i+1];
                       sprintf(strVal[i], "<%03.0f,%03.0f> - <%03.0f,%03.0f> = <%03.0f,%03.0f>",
                               uu[i],vv[i],uu[i+1],vv[i+1],uudiffs[i],vvdiffs[i]);
                       }
                    largeurTexte[i] = c_wglwsi(strVal[i], strlen(strVal[i])); 
                    }
                 break;
                 
               case ABS_SUBTRACT:
                 for (i=0; i < nbChampsActifs; i+=2)
                    {
                    if (champs[i]->natureTensorielle == SCALAIRE || !xc.statuts[BARBULES])
                       {
                       diffs[i]=fabs(vals[i] - vals[i+1]);
                       sprintf(strVal[i], "ABS(%12.6g - %12.6g) = %12.6g", vals[i], vals[i+1], diffs[i]);
                       }
                    else
                       {
                       uudiffs[i] = fabs(uu[i] - uu[i+1]);
                       vvdiffs[i] = fabs(vv[i] - vv[i+1]);
                       sprintf(strVal[i], "ABS(<%03.0f,%03.0f> - <%03.0f,%03.0f>) = <%03.0f,%03.0f>",
                               uu[i],vv[i],uu[i+1],vv[i+1],uudiffs[i],vvdiffs[i]);
                       }
                    largeurTexte[i] = c_wglwsi(strVal[i], strlen(strVal[i])); 
                    }
                 break;
               }
            
            switch (op)
               {
               case NO_OP:
                 switch(nbChampsActifs)
                    {
                    case 4:
                      offsetX[3] = 4;
                      offsetY[3] = hauteurTexte + 8 + 4;
                      
                    case 3:
                      offsetX[2] =  - largeurTexte[2] - 4;
                      offsetY[2] = hauteurTexte + 8 + 4;
                      
                    case 2:
                      offsetX[1] = 4;
                      offsetY[1] = 8;
                      
                      offsetX[0] = - largeurTexte[0] - 4;
                      offsetY[0] = 8;
                      break;
                      
                    case 1:
                      offsetX[0] = - largeurTexte[0] / 2;
                      offsetY[0] = 8;
                      break;
                    }
                 break;
                 
               default:
                 switch(nbChampsActifs)
                    {
                    case 4:
                    case 3:
                      offsetX[2] = - largeurTexte[2] / 2;
                      offsetY[2] = hauteurTexte + 8 + 4;
                      
                    case 2:
                    case 1:
                      offsetX[0] = - largeurTexte[0] / 2;
                      offsetY[0] = 8;
                      break;
                    }
               }
            
            c_xy2fxfy(&x,&y,(float)(int)(xgrid+0.5),(float)(int)(ygrid+0.5));
            c_wglxai(&ix1, &iy1, x, y);
            
            
            switch (op)
               {
               case NO_OP:
                 for (i=0; i < nbChampsActifs; i++)
                    {
                    if (champs[i]->domaine != VALEURS_PONCTUELLES)
                       {
                       DrawBoxedStr(strVal[i], ix1 + offsetX[i], iy1 + offsetY[i], 
                                    18,xc.attributs[i].indCouleurBack, xc.attributs[i].indCouleurFore);
                       }
                    }
                 break;
                 
               default:
                 for (i=0; i < nbChampsActifs; i+=2)
                    {
                    DrawBoxedStr(strVal[i], ix1 + offsetX[i], iy1 + offsetY[i], 
                                 18,xc.attributs[i].indCouleurBack, xc.attributs[i].indCouleurFore);
                    }
                 break;
               }
            
            c_wglcol(NOIR);
            c_wglrli(ix1 - 2, iy1 - 2, ix1 + 2, iy1 +2);
            
            if (xgrid < 10.0 && ygrid < 10.0)
              sprintf(strVal[0], "(%1d,%1d)", (int)(xgrid + 0.5), (int)(ygrid + 0.5));
            else
              if (xgrid < 100.0 && ygrid < 100.0)
                sprintf(strVal[0], "(%2d,%2d)", (int)(xgrid + 0.5), (int)(ygrid + 0.5));
              else
                if (xgrid < 1000.0 && ygrid < 1000.0)
                  sprintf(strVal[0], "(%3d,%3d)", (int)(xgrid + 0.5), (int)(ygrid + 0.5));
                else
                  sprintf(strVal[0], "(%4d,%4d)", (int)(xgrid + 0.5), (int)(ygrid + 0.5));
            
            largeurTexte[0] = c_wglwsi(strVal[0], strlen(strVal[0])); 
            DrawBoxedStr(strVal[0], ix1 - largeurTexte[0] / 2, iy1 - hauteurTexte - 6,
                         18,BLANC, NOIR);
            break;
            
            case BMLIEU:
              xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
              RafraichirFenetre(xc.rafraichir, NULL, NULL);
              c_wglfsz(18);
              xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
              break;
              
            case BDROIT:
              break;
              
            case NIL:
              break;
              
            default:
              break;
            }
         }
      
      event = c_wglbtx(&x, &y);
      c_fxfy2xy(&xgrid, &ygrid, x, y);
      
      }
   
   InvertWidget(w);
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
}


int	 bidonprint(float px, float pos, char *nombre)
{
  printf("bidonprint! : %f %f %s\n", px, pos, nombre);
}
