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
#include <c_wgl.h>

static char *lblAvrtRecordsManquants[]={"\nLes descripteurs '^^' et '>>'\nsont manquants. Le champ sera affiche\nsans geographie\n", 
                               "\nThe records '^^' and '>>' are missing.\nThe field will be displayed without geography\n"};

extern GeoMapInfoStruct mapInfo;
extern _Viewport   viewp;   

void CheckForPointValues(champ, indChamp)
     _Champ *champ;
     int indChamp;
{
}



DrawPointValues(champ, indChamp, fontSize, lineThickness, fore, back)
     _Champ *champ;
     int indChamp, fontSize, lineThickness, fore, back;
{
  int i, ii,npts;
  int i1, j1;
  float x1, y1, xmin, xmax, ymin, ymax;
  int hauteurTexte, largeurTexte;
  int offsetX, offsetY;
  int indChampRel;
  char strVal[16], tempVal[16];
  float vitesse, direction, u, v, lat, lon;
  int rayon,rayon2;
  int un,op;
  
  _Champ *champRef;
  
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
  
  hauteurTexte = c_wglasi("1234",4) + c_wgldsi("1234", 4);
  
  switch(indChampRel)
    {
    case 3:
      offsetY = (int)(3.6 * hauteurTexte) + fontSize;
      break;
      
    case 2:
      offsetY = (int)(2.4 * hauteurTexte) + fontSize;
      break;
      
    case 1:
      offsetY = (int)(1.2 * hauteurTexte) + fontSize;
      break;
      
    case 0:
      offsetY = fontSize;
      break;
    }
  
  un = 1;
  c_wglgvx(&xmin, &ymin, &xmax, &ymax);
  npts = champ->src.ni*champ->src.nj*champ->src.nk;
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
	  
	  if (champ->intervalles[0] != 0.0 && champ->nbIntervalles == 1)
	    {
	      DrawBoxedStr(strVal, i1 - largeurTexte / 2, j1 + offsetY, fontSize,back, fore);
	    }
	  c_wglcol(fore);
	  rayon = ROUND(0.5*fontSize);
	  rayon2 = ROUND(0.67*fontSize);
	  c_wglcli(i1, j1, rayon);
	  c_wglrli(i1-lineThickness-rayon2, j1, i1+lineThickness+rayon2, j1);
	  c_wglrli(i1, j1-lineThickness-rayon2, i1, j1+lineThickness+rayon2);
	}
    }
}











