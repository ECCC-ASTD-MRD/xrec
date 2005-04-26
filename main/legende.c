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
#include <rec.h>
#include <math.h>
#include <rpnmacros.h>

extern float c_wglhsx();
extern float c_wgldsx();
extern _Viewport viewp;
extern _XContour xc;
extern _InfoChamps *infoChamps;

AfficherLegendeCouleur(colorTable, contourMin, contourMax, intervalles, nbIntervalles, facteur, x1, y1, x2, y2)
int colorTable[];
float contourMin, contourMax, intervalles[];
int nbIntervalles;
float facteur;
float x1, y1, x2, y2;
{
  float x;
  float contourCourant, contourLimite;
  int i,j, k, n,increment, milieu, imax;
  char nombre[32], format[32];
  float pos, hauteurTexte;
  float cmin, cmax, inter,cmin2;
  float xa, ya, xb, yb;
  int   ia, ja, ib, jb;
  int oldAspectRatioFlag;
  int transformationFenetre,annulationDemandee,fz;
  
  int i1, j1, i2, j2, largeur, hauteur,indcol;
  float fraction, px, py;
  int largeurLegende, hauteurLegende, longueurString;
  int variation = PaletteMgrGetVariation();
  
  if (contourMax == contourMin)
    return;
  
  annulationDemandee = c_wglanul();
  if (annulationDemandee) return;
  cmin = contourMin;
  cmax = contourMax;
  transformationFenetre = c_wglgmod();
  c_wglgsp(&xa, &ya, &xb, &yb, &ia, &ja, &ib, &jb); 
  
  if (intervalles[0] != 0.0 && nbIntervalles == 1)
    AjusterMinMax(&cmin, &cmax, facteur, intervalles[0]);
  
  GetFormat(format, intervalles, nbIntervalles, facteur);
  oldAspectRatioFlag = c_wglgaf();
  c_wglias(TRUE);
  c_wglgwz(&largeur, &hauteur);
  i1 = x1 * largeur;
  i2 = x2 * largeur;
  j1 = y1 * hauteur;
  j2 = y2 * largeur;
  
  c_wglfsz(14);
  largeurLegende = CalculerLargeurLegendeCouleur();
  hauteurLegende = (int)(0.8 * hauteur);
  
  c_wglssp(0.0, 0.0, 100.0, 256.0, 
     largeur - 5 - largeurLegende, (int)(hauteur * 0.15), largeur - 5, (int)(hauteur * 0.85), 1);
  c_wglcol(xc.attributs[FOND].indCouleurFore);
  c_wglrfx(-1.0, -10.0, 101.0, 266.0);
  
  c_wglcmx(0.0, 0.0, 100.0, 256.0);
  
  if (nbIntervalles == 1)
    {
    if (intervalles[0] == 0.0 && nbIntervalles == 1)
      {
      inter = (cmax - cmin) / (0.7*hauteurLegende);
      cmin2 = cmin + inter;
      if (cmin2 == cmin)
        {
        inter = cmax - cmin;
        }
      }
    else
      {
      inter = intervalles[0] * facteur;
      }
    }
  
  if (nbIntervalles == 1)
    {
    contourCourant = cmin;
    n = 0;
    while (contourCourant <= cmax && !annulationDemandee)
      {
      fraction = contourCourant;
      enhancefracs(&fraction,1,cmin,cmax-cmin,variation);
      
      i = ROUND(255 * (contourCourant - cmin) / (cmax - cmin));
      j = ROUND(255 * ((contourCourant+inter) - cmin) / (cmax - cmin));
      
      if (8 < c_wglgpl())
        {
        c_wglcolf(colorTable[0] + fraction*(colorTable[255]-colorTable[0]));
        }
      else
        {
        c_wglcol(colorTable[ROUND(255*fraction)]);
        }
      c_wglrfx(10.0/largeurLegende*100.0, (float)(i), 40.0/largeurLegende*100.0, (float)(j));
      contourCourant += inter;
      n++;
      if (0 == (n% 16))
  annulationDemandee = c_wglanul();
      }
    }
  else
    {
    if (cmin < intervalles[0])
      contourCourant = 0.0;
    else
      contourCourant = 1.0;
    
    if (cmax > intervalles[nbIntervalles - 1])
      contourLimite = (float)nbIntervalles;
    else
      contourLimite = (float)(nbIntervalles-1);
    
    while (contourCourant <= (float)(nbIntervalles))
      {
      i = ROUND(255 * (contourCourant ) / (float)(nbIntervalles+1));
      j = ROUND(255 * (contourCourant+1.0) / (float)(nbIntervalles+1));
      
      c_wglcol(colorTable[ROUND(255 * contourCourant / (float)nbIntervalles)]);
      c_wglrfx(10.0/largeurLegende*100.0, (float)(i), 40.0/largeurLegende*100.0, (float)(j));
      contourCourant += 1.0;
      }
    }
  
  if (annulationDemandee) return;
  contourCourant = cmin;
  sprintf(nombre, "%6.0f", (contourCourant/facteur));
  longueurString = strlen(nombre);
  hauteurTexte =  c_wglhsx(nombre, longueurString) - c_wgldsx(nombre, longueurString);
  
  if (nbIntervalles == 1)
    x = (255.0 * inter) / (cmax - cmin);
  else
    x = 255.0 / (float)nbIntervalles;
  
  increment = 1;
  if ((3 * hauteurTexte) >= x)
    {
    while ((float)(increment*x) < (3.0 * hauteurTexte))
      increment++;
    }
  
  
  c_wglcmx(0.0, 0.0, 100.0, 266.0);
  c_wglcol(xc.attributs[FOND].indCouleurBack);
  fz = AttrMgrGetFontSizeColorTable();
  k = 0;
  if (nbIntervalles == 1)
    {
    while (contourCourant <= cmax)
      {
      pos = (255.0 * (contourCourant - cmin) / (cmax - cmin));
      sprintf(nombre, format, contourCourant/facteur);
      longueurString=strlen(nombre);
      px = 45.0/largeurLegende*100.0;
      c_wglpsx((float)px, (float)pos, (char *)nombre, (int)longueurString, fz, (int)0, (int)0);
      contourCourant += (inter*increment);
      }
    }
  else
    {
    if (cmin < intervalles[0])
      i = 0;
    else
      i = 1;
    
    if (cmax > intervalles[nbIntervalles-1])
      imax = nbIntervalles+1;
    else
      imax = nbIntervalles;
    
    while (i <= imax)
      {
      if (i == 0)
        {
        contourCourant = cmin/facteur;
        }
      else
        {
        if (i == (nbIntervalles+1))
          contourCourant = cmax/facteur;
        else
          contourCourant = intervalles[i-1];
        }
      
      pos = (255.0 * i / (nbIntervalles+1));
      sprintf(nombre, format, contourCourant);
      longueurString = strlen(nombre);
      px = 45.0/largeurLegende*100.0;
      /*       printf("<avant wglpsx>: %f %f %s %d %d %d %d\n", px, pos, nombre, longueurString, fz, 0,0);    */
      c_wglpsx(px, pos, nombre, longueurString, fz, 0, 0);
      i++;
      }
    }
  
  c_wglias(oldAspectRatioFlag);
  c_wglssp(xa, ya, xb, yb, ia, ja, ib, jb, transformationFenetre); 
  
  
}

AjusterViewport(_Viewport *viewp)
{
  static int oldLargeurFenetre = 0;
  static int oldHauteurFenetre = 0;
  static int oldFlagLegende = 0;
  static int lastInd = -1;
  static char lastnomvar[5] = {"\0\0\0\0\0"};
  _Champ *champ;
  
  int largeurFenetre, hauteurFenetre, largeurLegendeCouleur, hauteurLegende, flagLegende;
  int largeurAxeY;
  
  FldMgrGetChamp(&champ, 0);
  c_wglgwz(&largeurFenetre, &hauteurFenetre);
  
  if (!xc.statuts[LEGENDE] && !xc.statuts[LEGENDE_COULEUR])
    {
    viewp->vlargeur = largeurFenetre;
    viewp->vhauteur = hauteurFenetre;
    viewp->vi1 = 0;
    viewp->vi2 = largeurFenetre - 1;
    viewp->vj1 = 0;
    viewp->vj2 = hauteurFenetre - 1;
    return;
    }
  
  if (AfficherItem(0, LEGENDE_COULEUR))
    flagLegende = TRUE;
  else
    flagLegende = FALSE;
  
  /**
     if (largeurFenetre == oldLargeurFenetre && hauteurFenetre == oldHauteurFenetre && flagLegende == oldFlagLegende &&
     0 == strcmp(lastnomvar, champ->nomvar) && lastInd == infoChamps[champ->indDict].indIntervalleDeDefaut)
     {
     strcpy(lastnomvar, champ->nomvar);
     lastInd = infoChamps[champ->indDict].indIntervalleDeDefaut;
     return;
     }
  **/
  
  strcpy(lastnomvar, champ->nomvar);
  lastInd = infoChamps[champ->indDict].indIntervalleDeDefaut;
  
  largeurLegendeCouleur = CalculerLargeurLegendeCouleur();
  hauteurLegende = CalculerHauteurLegende();
  
  if (xc.statuts[AXE_Y] == TRUE)
    {
    largeurAxeY = c_wglwsi("123456", 6);
    }
  else
    {
    largeurAxeY = 0;
    }
  
  viewp->vlargeur = largeurFenetre - largeurLegendeCouleur - largeurAxeY - 20;
  viewp->vratio   = (float)(viewp->vlargeur) / (float)(largeurFenetre);
  viewp->vhauteur = ROUND(viewp->vratio * hauteurFenetre);
  
  if ((hauteurFenetre - viewp->vhauteur) < 2 * hauteurLegende)
    {
    viewp->vhauteur = hauteurFenetre - 2 * hauteurLegende - 20;
    viewp->vratio   = (float)(viewp->vhauteur) / (float)(hauteurFenetre);
    viewp->vlargeur = ROUND(viewp->vratio * (largeurFenetre - largeurLegendeCouleur - largeurAxeY - 20));
    viewp->vratio   = (float)(viewp->vlargeur) / (float)(largeurFenetre);
    viewp->vhauteur = ROUND(viewp->vratio * hauteurFenetre);
    }
  
  viewp->vi1      = 10+largeurAxeY+(largeurFenetre-viewp->vlargeur-largeurLegendeCouleur-largeurAxeY-20)/2;
  viewp->vi2      = viewp->vi1 + viewp->vlargeur;
  viewp->vj1      = ROUND((hauteurFenetre - viewp->vhauteur) * 0.5);
  viewp->vj2      = viewp->vj1 + viewp->vhauteur;
  
  oldLargeurFenetre = largeurFenetre;
  oldHauteurFenetre = hauteurFenetre;
  oldFlagLegende = flagLegende;
}

CalculerLargeurLegendeCouleur()
{
  int i, largeurLegende;
  int exposant = 0;
  float rmint;
  _Champ *champ;
  
  FldMgrGetChamp(&champ, 0);
  
  if (AfficherItem(0, LEGENDE_COULEUR))
    {
    c_wglfsz(AttrMgrGetFontSizeColorTable());
    for (i=0; i < champ->nbIntervalles; i++)
      {
      rmint = champ->intervalles[i];
      if (rmint != 0.0 && (fabs(rmint) < 1.0e-3 || fabs(rmint) > 1.0e5))
  exposant = 1;
      }
    
    if (exposant)
      largeurLegende = 15 + 35 + c_wglwsi("1.00e+05", 8);
    else
      largeurLegende = 15 + 35 + c_wglwsi("123456", 6);
    return largeurLegende;
    }
  else
    return 0;
}

CalculerHauteurLegende()
{
  int hauteurLegende,fz;
  
  
  c_wglfsz(AttrMgrGetFontSizeLegend());
  hauteurLegende = ROUND(2.5 * c_wglasi("123456", 6));
  return hauteurLegende;
}



void AfficherLegende2(_Champ champ)
{
  int i, couleur;
  
  int LargeurTitre[6], Largeur; 
  int HauteurRectangle, hauteurTitre[6];
  int hauteurTexte, descent;
  int largeurFenetre, hauteurFenetre;
  int fontSize;
  char tmpStr[132];
  
  c_wglgwz(&largeurFenetre, &hauteurFenetre);
  c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);
  
  EffacerLegende2();
  
  c_wglcol(xc.attributs[FOND].indCouleurBack);
  
  if (champ.natureTensorielle == VECTEUR && xc.statuts[BARBULES])
    {
    strcpy(tmpStr, champ.titreVariable);
    strcpy(champ.titreVariable,"UU-VV ");
    }
  
  strcpy(tmpStr, champ.titreNiveau);
  strcat(tmpStr, " - ");
  strcat(tmpStr, champ.titreEtiquette);
  strcat(tmpStr, " - ");
  strcat(tmpStr, champ.titreIntervalle);
  
  fontSize = SelectFontSize(tmpStr, viewp.vlargeur, (int)((hauteurFenetre - viewp.vj2) * 0.40));
  c_wglfsz(fontSize);
  descent = c_wgldsi("1234", 4);
  hauteurTexte = c_wglasi("1234",  4); /**  + descent; **/
  
  
  LargeurTitre[1] = c_wglwsi(tmpStr, strlen(tmpStr)); 
  
  LargeurTitre[0] = c_wglwsi(champ.titreVariable, strlen(champ.titreVariable)); 
  LargeurTitre[2] = c_wglwsi(champ.titreNiveau, strlen(champ.titreNiveau)); 
  LargeurTitre[3] = c_wglwsi(champ.titreIntervalle, strlen(champ.titreIntervalle)); 
  LargeurTitre[4] = c_wglwsi(champ.titreUnites, strlen(champ.titreUnites)); 
  LargeurTitre[5] = c_wglwsi(champ.titreEtiquette, strlen(champ.titreEtiquette)); 
  
  LargeurTitre[1] = c_wglwsi(tmpStr, strlen(tmpStr)); 
  c_wglpsi(viewp.vi1 + (int)(0.5 *(viewp.vlargeur - LargeurTitre[0])), 
     viewp.vj2+(int)(0.60 * (hauteurFenetre-viewp.vj2-2*hauteurTexte) + hauteurTexte),
     champ.titreVariable, strlen(champ.titreVariable), fontSize, 0, 0);
  
  c_wglpsi(viewp.vi1 + (int)(0.5 *(viewp.vlargeur - LargeurTitre[1])), 
     viewp.vj2+(int)(0.30 * (hauteurFenetre-viewp.vj2-2*hauteurTexte)),
     tmpStr, strlen(tmpStr), fontSize, 0, 0);
  
  LargeurTitre[1] = c_wglwsi(champ.titreTemps, strlen(champ.titreTemps));
  c_wglpsi(viewp.vi1 + (int)(0.5 *(viewp.vlargeur - LargeurTitre[1])), 
     (int)(0.50 * (viewp.vj1) - hauteurTexte * 0.5),
     champ.titreTemps, strlen(champ.titreTemps), fontSize, 0, 0);
  
  Largeur = LargeurTitre[0];
  for (i=1; i < 6; i++)
    if (LargeurTitre[i] > Largeur)
      Largeur = LargeurTitre[i];
  
  hauteurTitre[5] = 5;
  
  i = 4;
  while (i >= 0)
    {
    hauteurTitre[i] = hauteurTitre[i+1] + hauteurTexte + 2;
    i--;
    }
  
  HauteurRectangle = hauteurTitre[0]  + hauteurTexte + 2;
  
  if (champ.natureTensorielle == VECTEUR)
    {
    strcpy(champ.titreVariable,tmpStr);
    }
  
}


/**
***********************************************************************
***********************************************************************
**/


EffacerLegende2()
{
   int largeurFenetre, hauteurFenetre;
   int largeurPinceau;

   largeurPinceau = xc.attributs[FOND].epaisseur;

/**
   if (xc.statuts[COULEURS])
      c_wglcol(xc.attributs[FOND].indCouleurFore);
   else
      c_wglcol(BLANC);
**/

   c_wglcol(xc.attributs[FOND].indCouleurFore);
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);

   c_wglrfi(0, 0, largeurFenetre, viewp.vj1 - largeurPinceau);
   c_wglrfi(0, 0, viewp.vi1 - largeurPinceau, hauteurFenetre);
   c_wglrfi(0, viewp.vj2+largeurPinceau, largeurFenetre, hauteurFenetre);
   c_wglrfi(viewp.vi2+largeurPinceau, 0, largeurFenetre, hauteurFenetre);

   }


AfficherLegendeSup2()
{
   int i;
   
   int largeurTexte, largeurMax; 
   int hauteurRectangle;
   int hauteurTexte, descent;
   int tempDate,tempDate0,tempDate2;
   char buffer[72];
   char texte[4][72];
   int rect[4][4];
   int h1, h2;
   char pdfdatev[16];
   int largeurFenetre, hauteurFenetre;
   int fontSize;
   int nbChampsActifs, lng;
   _Champ *champ, *champ0;
   double deltaT;
   int ind,op;
   char mo[4];
   char kindstring[16];
   int kind;
   int mode = -1;
   int un = 1;
   int rip1;
   float rniveau;
   
   static char *Mois[] = {"bid", "Jan", "Fev", "Mars", "Avr", "Mai", "Juin", "Juil", "Aout", "Sep", "Oct", "Nov", "Dec"};
   static char *Month[] = {"bid", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
   
   char dateMess[32],dateMess0[32];
   char **leMois;
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);
   EffacerLegende2();
   
   lng = c_getulng();
   if (lng == 0)
      {
      leMois = Mois;
      }
   else
      {
      leMois = Month;
      }
   
   op = CtrlMgrGetMathOp();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   nbChampsActifs = nbChampsActifs > 4 ? 4 : nbChampsActifs;
   if (op == NO_OP || nbChampsActifs < 2)
      {
      for (i=0; i < nbChampsActifs; i++)
   {
   ind = i%4;
   strcpy(texte[ind], "");
   FldMgrGetChamp(&champ, i);
   deltaT =  (champ->npas*champ->deet)/3600.0;
   tempDate = champ->dateo;
   tempDate2 = champ->dateo;
   f77name(incdatr)(&tempDate, &tempDate, &deltaT);
   
   FldMgrCalcPDFDatev(pdfdatev,&(champ->datev),champ->dateo,champ->deet,champ->npas,champ->ip2);
   mo[3] = '\0';
   strncpy(mo,champ->mois,3);
   sprintf(dateMess, "V%s", pdfdatev);
   
   rip1 = champ->ip1;
         f77name(convip)(&rip1, &rniveau, &kind, &mode, kindstring, &un, 15);
         kindstring[15] = '\0';
   nettoyer(kindstring);
   if (champ->cle >= 0)
      {
      if (champ->natureTensorielle == VECTEUR && !xc.statuts[BARBULES])
         {
         sprintf(texte[ind], "%s-%s-%s-%3d-%3d-%s-%s", "UU-VV", champ->typvar, 
           kindstring, champ->ip2, champ->ip3, dateMess, champ->etiket);
         }
      else
         {
         sprintf(texte[ind], "%s-%s-%s-%3d-%3d-%s-%s", champ->nomvar, champ->typvar, 
           kindstring, (int)(champ->ip2), champ->ip3, dateMess, champ->etiket);
         }
      }
   }
      }
   else
      { 
      for (ind=0; ind < nbChampsActifs;ind+=2)
   {
   strcpy(texte[ind],"");
   strcpy(texte[ind+1],"");
   FldMgrGetChamp(&champ, ind+1);
   FldMgrGetChamp(&champ0, ind);
   CombinerLegendeDiffChamps(texte[ind], champ0, champ);
   }
      }
   
   fontSize = SelectFontSize(texte[ind], (int)(0.5 * viewp.vlargeur - 10.0), viewp.vj1);
   c_wglfsz(fontSize);
   descent = c_wgldsi("1234", 4);
   hauteurTexte = c_wglasi("1234", 4);
   hauteurRectangle = hauteurTexte + 5;
   
   largeurMax = 0;
   for (ind=0; ind < nbChampsActifs; ind++)
      {
      largeurTexte = c_wglwsi(texte[ind], strlen(texte[ind]));
      if (largeurMax < largeurTexte)
   largeurMax = largeurTexte;
      }
   
   largeurMax += 6;
   
   c_wgllwi(2);

   switch(op)
     {
     case NO_OP:
       switch(nbChampsActifs)
   {
   case 1:
     rect[0][0] = viewp.vi1 + 0.5 * (viewp.vlargeur - largeurMax);
     rect[0][1] = (int)(0.50 * (viewp.vj1) - hauteurTexte * 0.5);
     
     c_wglcol(xc.attributs[0].indCouleurFore);
     c_wglrfi(rect[0][0], rect[0][1], rect[0][0]+largeurMax, rect[0][1]+hauteurRectangle);
     c_wglcol(xc.attributs[0].indCouleurBack);
     c_wglrli(rect[0][0], rect[0][1], rect[0][0]+largeurMax, rect[0][1]+hauteurRectangle);
     c_wglcol(xc.attributs[0].indCouleurBack);
     c_wglpsi(rect[0][0]+5, rect[0][1] + 4, texte[0], strlen(texte[0]), fontSize, 0, 0);
     break;
     
   default:
     rect[0][0] = viewp.vi1 + 0.5 * (0.5 * viewp.vlargeur - largeurMax) - 10;
     rect[1][0] = viewp.vi1 + 0.5 * viewp.vlargeur + 0.5 * (0.5 * viewp.vlargeur - largeurMax) + 10;
     rect[2][0] = rect[0][0];
     rect[3][0] = rect[1][0];
     
     rect[0][1] = (int)(0.50 * (viewp.vj1) - hauteurTexte * 0.5);
     rect[1][1] = rect[0][1];
     rect[2][1] = viewp.vj2+(int)(0.50 * (hauteurFenetre-viewp.vj2-0.5*hauteurTexte));
     rect[3][1] = rect[2][1];
     for (i=0; i < nbChampsActifs; i++)
       {
       if (0 < strlen(texte[i]))
         {
         c_wglcol(xc.attributs[i].indCouleurFore);
         c_wglrfi(rect[i][0], rect[i][1], rect[i][0]+largeurMax, rect[i][1]+hauteurRectangle);
         c_wglcol(xc.attributs[i].indCouleurBack);
         c_wglrli(rect[i][0], rect[i][1], rect[i][0]+largeurMax, rect[i][1]+hauteurRectangle);
         c_wglcol(xc.attributs[i].indCouleurBack);
         c_wglpsi(rect[i][0]+5, rect[i][1] + 4, texte[i], strlen(texte[i]), fontSize, 0, 0);
         }
       }
     break;
     
   }
       break;
       
       
     default:
       switch (nbChampsActifs)
   {
   case 1:
   case 2:
     rect[0][0] = viewp.vi1 + 0.5 * (viewp.vlargeur - largeurMax);
     rect[0][1] = (int)(0.50 * (viewp.vj1) - hauteurTexte * 0.5);
     
     c_wglcol(xc.attributs[0].indCouleurFore);
     c_wglrfi(rect[0][0], rect[0][1], rect[0][0]+largeurMax, rect[0][1]+hauteurRectangle);
     c_wglcol(xc.attributs[0].indCouleurBack);
     c_wglrli(rect[0][0], rect[0][1], rect[0][0]+largeurMax, rect[0][1]+hauteurRectangle);
     c_wglcol(xc.attributs[0].indCouleurBack);
     c_wglpsi(rect[0][0]+5, rect[0][1] + 4, texte[0], strlen(texte[0]), fontSize, 0, 0); 
     
     break;
     
   case 3:
   case 4:  
     rect[0][0] = viewp.vi1 + 0.5 * (viewp.vlargeur - largeurMax);
     rect[0][1] = (int)(0.50 * (viewp.vj1) - hauteurTexte * 0.5);
     
     rect[2][0] = rect[0][0];
     rect[2][1] = viewp.vj2+(int)(0.50 * (hauteurFenetre-viewp.vj2-0.5*hauteurTexte));
     for (i=0; i < nbChampsActifs; i+=2)
       {
       ind = i%4;
       if (0 < strlen(texte[ind]))
         {
         c_wglcol(xc.attributs[ind].indCouleurFore);
         c_wglrfi(rect[ind][0], rect[ind][1], rect[ind][0]+largeurMax, rect[ind][1]+hauteurRectangle);
         c_wglcol(xc.attributs[ind].indCouleurBack);
         c_wglrli(rect[ind][0], rect[ind][1], rect[ind][0]+largeurMax, rect[ind][1]+hauteurRectangle);
         c_wglcol(xc.attributs[ind].indCouleurBack);
         c_wglpsi(rect[ind][0]+5, rect[ind][1] + 4, texte[ind], strlen(texte[ind]), fontSize, 0, 0);
         }
       }
     break;
   }
      }  
   }

   
/**
 **************************************************************************
 **************************************************************************
 **/
   
int SelectFontSize(str, largeur, hauteur)
char *str;
int largeur, hauteur;
{
   int fontSizes[5];
   int hauteurTexte, largeurTexte;
   int i, ok,fz;

   fontSizes[0] = 24;
   fontSizes[1] = 18;
   fontSizes[2] = 17;
   fontSizes[3] = 14;
   fontSizes[4] = 12;
   
   fz = AttrMgrGetFontSizeLegend();
   if (fz == -1)
      {
      i = 0;
      ok = 0;

      while (i < 5 && !ok)
         {
         c_wglfsz(fontSizes[i]);
         largeurTexte = c_wglwsi(str, strlen(str));
         if (largeur < largeurTexte)
      i++;
         else
      ok = 1;
         }
   
      while (i < 4 && !ok)
         {
         c_wglfsz(fontSizes[i]);
         hauteurTexte = c_wglhsi(str, strlen(str));
         if (hauteur < hauteurTexte)
      i++;
         else
        ok = 1;
         }
   
      if (i > 4)
      i = 4;
      fz = fontSizes[i];
      } 
    return fz;

   }


void AfficherLegendeCoupe2(champ)
_Champ champ;
{
   static char *legendeCoupe[] = {"Coupe verticale","Vertical cross-section"};
   static char *legendeSerie[] = {"Serie temporelle","Time series"};
   int i, couleur;

   int LargeurTitre[6], Largeur; 
   int HauteurRectangle, hauteurTitre[6];
   int hauteurTexte, descent;
   int largeurFenetre, hauteurFenetre;
   int fontSize;
   int lng;
   char tmpStr[132];

   lng =  c_getulng();
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);

   EffacerLegende2();

   c_wglcol(xc.attributs[FOND].indCouleurBack);
   /**
     if (xc.statuts[COULEURS])
     c_wglcol(xc.attributs[FOND].indCouleurBack);
     else
     c_wglcol(NOIR);
     **/
   
   if (champ.natureTensorielle == VECTEUR)
      {
      strcpy(tmpStr, champ.titreVariable);
      strcpy(champ.titreVariable,"UU-VV ");
      }

   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      strcpy(tmpStr, legendeCoupe[lng]);
      }
   else
      {
      strcpy(tmpStr, legendeSerie[lng]);
      }

   strcat(tmpStr, " - ");
   strcat(tmpStr, champ.titreEtiquette);
   strcat(tmpStr, " - ");
   strcat(tmpStr, champ.titreIntervalle);
   
   fontSize = SelectFontSize(tmpStr, viewp.vlargeur, (int)((hauteurFenetre - viewp.vj2) * 0.40));
   c_wglfsz(fontSize);
   descent = c_wgldsi("1234", 4);
   hauteurTexte = c_wglasi("1234",  4); /**  + descent; **/
   

   LargeurTitre[1] = c_wglwsi(tmpStr, strlen(tmpStr)); 

   LargeurTitre[0] = c_wglwsi(champ.titreVariable, strlen(champ.titreVariable)); 
   LargeurTitre[2] = c_wglwsi(champ.titreNiveau, strlen(champ.titreNiveau)); 
   LargeurTitre[3] = c_wglwsi(champ.titreIntervalle, strlen(champ.titreIntervalle)); 
   LargeurTitre[4] = c_wglwsi(champ.titreUnites, strlen(champ.titreUnites)); 
   LargeurTitre[5] = c_wglwsi(champ.titreEtiquette, strlen(champ.titreEtiquette)); 

   LargeurTitre[1] = c_wglwsi(tmpStr, strlen(tmpStr)); 
   c_wglpsi(viewp.vi1 + (int)(0.5 *(viewp.vlargeur - LargeurTitre[0])), 
      viewp.vj2+(int)(0.60 * (hauteurFenetre-viewp.vj2-2*hauteurTexte) + hauteurTexte),
      champ.titreVariable, strlen(champ.titreVariable), fontSize, 0, 0);

   c_wglpsi(viewp.vi1 + (int)(0.5 *(viewp.vlargeur - LargeurTitre[1])), 
      viewp.vj2+(int)(0.30 * (hauteurFenetre-viewp.vj2-2*hauteurTexte)),
      tmpStr, strlen(tmpStr), fontSize, 0, 0);

   LargeurTitre[1] = c_wglwsi(champ.titreTemps, strlen(champ.titreTemps));
   c_wglpsi(viewp.vi1 + (int)(0.5 *(viewp.vlargeur - LargeurTitre[1])), 
      (int)(0.50 * (viewp.vj1) - hauteurTexte),
      champ.titreTemps, strlen(champ.titreTemps), fontSize, 0, 0);

   Largeur = LargeurTitre[0];
   for (i=1; i < 6; i++)
      if (LargeurTitre[i] > Largeur)
   Largeur = LargeurTitre[i];
   
   hauteurTitre[5] = 5;
   
   i = 4;
   while (i >= 0)
      {
      hauteurTitre[i] = hauteurTitre[i+1] + hauteurTexte + 2;
      i--;
      }
   
   HauteurRectangle = hauteurTitre[0]  + hauteurTexte + 2;
   
   if (champ.natureTensorielle == VECTEUR)
      {
      strcpy(champ.titreVariable,tmpStr);
      }

   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      sprintf(tmpStr, "(%5.1f, %5.1f)", champ.coupe.xmin, champ.coupe.ymin);
      LargeurTitre[0] = c_wglwsi(tmpStr, strlen(tmpStr)); 
      c_wglpsi(viewp.vi1, viewp.vj1 - hauteurTexte - 5,
         tmpStr, strlen(tmpStr), 12, 0, 0);
      
      sprintf(tmpStr, "(%5.1f, %5.1f)", champ.coupe.xmax, champ.coupe.ymax);
      LargeurTitre[0] = c_wglwsi(tmpStr, strlen(tmpStr)); 
      c_wglpsi(viewp.vi2 - LargeurTitre[0], viewp.vj1 - hauteurTexte - 5,
         tmpStr, strlen(tmpStr), 12, 0, 0);
      }
   else
      {
      sprintf(tmpStr, "(%5.1f, %5.1f)", champ.seqanim.xmin, champ.seqanim.ymin);
      LargeurTitre[0] = c_wglwsi(tmpStr, strlen(tmpStr)); 
      c_wglpsi(viewp.vi1, viewp.vj1 - hauteurTexte - 5,
         tmpStr, strlen(tmpStr), 12, 0, 0);
      
      sprintf(tmpStr, "(%5.1f, %5.1f)", champ.seqanim.xmax, champ.seqanim.ymax);
      LargeurTitre[0] = c_wglwsi(tmpStr, strlen(tmpStr)); 
      c_wglpsi(viewp.vi2 - LargeurTitre[0], viewp.vj1 - hauteurTexte - 5,
         tmpStr, strlen(tmpStr), 12, 0, 0);
      }
      
   }


/**
***********************************************************************
***********************************************************************
**/


AfficherAxeY(champ)
_Champ champ;
{
   float x1, y1;
   int   i, i1, j1;
   char  strNiveaux[16], format[16];
   int largeurStr, hauteurStr;
   int largeurFenetre, hauteurFenetre;
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0,0,largeurFenetre-1, hauteurFenetre-1);
   c_wglcol(xc.attributs[FOND].indCouleurBack);

   if (xc.statuts[COULEURS])
      {
      c_wglcol(xc.attributs[FOND].indCouleurBack);
      }
   else
      {
      c_wglcol(NOIR);
      }
   
   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      hauteurStr = c_wglhsi("1234", 4);
      if (champ.coupe.niveauMin <= 10.0)
   {
   strcpy(format, "%5.3f");
   largeurStr = c_wglwsi("12345", 5);
   }
      else
   {
   strcpy(format, "%4.0f");
   largeurStr = c_wglwsi("1234", 4);
   }
      
      c_wglfsz(14);
      c_wglfsz(AttrMgrGetFontSizeLegend());
      for (i=0; i < champ.coupe.nbNiveauxCoupe; i++)
   {
   sprintf(strNiveaux, format, champ.coupe.niveauxCoupe[i]);
   x1 = 1.0;
   y1 = (float)(i+1);
   c_wglxai(&i1, &j1, 1.0, champ.coupe.niveauxCoupe[i]);
   if (j1 > (viewp.vj1-10) && j1 < (viewp.vj2+10))
      {
      c_wglpsi(i1 - largeurStr - 8, j1 - hauteurStr/2, strNiveaux, strlen(strNiveaux), AttrMgrGetFontSizeLegend(), 0, 0);
      c_wglmvi(i1,j1);
      c_wgldri(i1+10, j1);
      c_wglxai(&i1, &j1, (float)champ.coupe.niCoupe, champ.coupe.niveauxCoupe[i]);
      c_wglmvi(i1-10,j1);
      c_wgldri(i1,j1);
      }
   }
      }
   else
      {
      strcpy(format, "%4.0f");
      hauteurStr = c_wglhsi("1234", 4);
      largeurStr = c_wglwsi("1234", 4);
      
      c_wglfsz(14);
      c_wglfsz(AttrMgrGetFontSizeLegend());
      for (i=0; i < champ.seqanim.nbFldsAnim; i++)
   {
   sprintf(strNiveaux, format, (float)champ.seqanim.ip2s[i]);
   x1 = 1.0;
   y1 = (float)(i+1);
   c_wglxai(&i1, &j1, 1.0, (float)champ.seqanim.ip2s[i]);
   if (j1 > (viewp.vj1-10) && j1 < (viewp.vj2+10))
      {
      c_wglpsi(i1 - largeurStr - 8, j1 - hauteurStr/2, strNiveaux, strlen(strNiveaux), 14, 0, 0);
      c_wglmvi(i1,j1);
      c_wgldri(i1+10, j1);
      c_wglxai(&i1, &j1, (float)champ.seqanim.niSerie, (float)champ.seqanim.ip2s[i]);
      c_wglmvi(i1-10,j1);
      c_wgldri(i1,j1);
      }
   }
      }
   }


AfficherLegendeSupProfil(xx, yy, xmin, ymin, xmax, ymax)
float xx, yy;
float xmin, ymin, xmax, ymax;
{
   int i;
   int i1, j1, i2, j2,ix1,iy1;
   int largeurTexte, largeurMax; 
   int hauteurRectangle;
   int hauteurTexte, descent;
   int tempDate, nbChampsActifs;
   char texte[4][72];
   char texteProfil[64];
   int rect[4][4];
   int h1, h2;
   int largeurFenetre, hauteurFenetre;
   int largeur, hauteur;
   int fontSize;
   int lng;
   _Champ *champ, *champ2;
   int op,ok;
   double deltaT;
   char pdfdatev[16];
   
   static char *Mois[] = {"bid", "Jan", "Fev", "Mars", "Avr", "Mai", "Juin", "Juil", "Aout", "Sep", "Oct", "Nov", "Dec"};
   static char *Month[] = {"bid", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
   static char *lettres[] = {"A", "B", "C", "D"};

   char dateMess[16];
   char mo[4];
   char **leMois;
   
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglxai(&i1, &j1, xmin, ymin);
   c_wglxai(&i2, &j2, xmax, ymax);
   largeur = i2 - i1;
   hauteur = j2 - j1;
   c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);
   
   lng = c_getulng();
   if (lng == 0)
      {
      leMois = Mois;
      }
   else
      {
      leMois = Month;
      }
   
   c_wglfsz(18);
   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      if (lng == 0)
   {
   sprintf(texteProfil, "Profil vertical - pt (%5.2f, %5.2f)", xx, yy);
   }
      else
   {
   sprintf(texteProfil, "Vertical profile - pt (%5.2f, %5.2f)", xx, yy);
   }
      }
   else
      {
      if (lng == 0)
   {
   sprintf(texteProfil, "Serie temporelle - pt (%5.2f, %5.2f)", xx, yy);
   }
      else
   {
   sprintf(texteProfil, "Time serie - pt (%5.2f, %5.2f)", xx, yy);
   }
      }

   ix1 = (largeurFenetre - c_wglwsi(texteProfil,strlen(texteProfil))) / 2;
   iy1 = (j1 - c_wglhsi(texteProfil,strlen(texteProfil))) / 2;
   c_wglpsi(ix1, iy1, texteProfil, strlen(texteProfil), 18, 0, 0);

   op = CtrlMgrGetMathOp();
   nbChampsActifs = FldMgrGetNbChampsActifs();
   nbChampsActifs = nbChampsActifs > 4 ? 4 : nbChampsActifs;
   if (op == NO_OP || nbChampsActifs < 2)
      {
      for (i=0; i < nbChampsActifs; i++)
   {
   strcpy(texte[i], "");
   FldMgrGetChamp(&champ, i);
   if (ZP == CoupeMgrGetDimensionCoupe())
      {
      ok = champ->cle >= 0 && champ->coupe.coupeValide;
      }
   else
      {
      ok = champ->cle >= 0 && champ->seqanim.nbFldsAnim > 0;
      }

   if (ok)
      {
      tempDate = champ->dateo;
      deltaT =  (champ->npas*champ->deet)/3600.0;
      f77name(incdatr)(&tempDate, &tempDate, &deltaT);
      
      FldMgrCalcPDFDatev(pdfdatev,&(champ->datev),champ->dateo,champ->deet,champ->npas,champ->ip2);
      mo[3] = '\0';
      strncpy(mo,champ->mois,3);
      sprintf(dateMess, "V%s", pdfdatev);
      
      if (champ->natureTensorielle == VECTEUR)
         {
         sprintf(texte[i], "%s-%s-%3d-%3d-%s-%s", "UU-VV", champ->typvar, 
           champ->ip2, champ->ip3, dateMess, champ->etiket);
         }
      else
         {
         sprintf(texte[i], "%s-%s-%3d-%3d-%s-%s", champ->nomvar, champ->typvar, 
           champ->ip2, champ->ip3, dateMess, champ->etiket);
         }
      }
   }
      }
   else
      {
      for (i=0; i < nbChampsActifs;i+=2)
   {
   strcpy(texte[i],"");
   strcpy(texte[i+1],"");
   FldMgrGetChamp(&champ, i+1);
   FldMgrGetChamp(&champ2, i);
   CombinerLegendeDiffChamps(texte[i], champ, champ2);
   }
      }

   i = 0;
   while (strlen(texte[i]) == 0)
      i++;

   switch(nbChampsActifs)
      {
      case 1:
      fontSize = 18;
      break;
      
      case 2:
      fontSize = 17;
      break;
      
      case 3:
      case 4:
      fontSize = 14;
      break;
      }
   
   c_wglfsz(fontSize);
   descent = c_wgldsi("1234", 4);
   hauteurTexte = c_wglasi("1234", 4);
   hauteurRectangle = hauteurTexte + 5;
   largeurMax = 0;
   for (i=0; i < nbChampsActifs; i++)
      {
      largeurTexte = c_wglwsi(texte[i], strlen(texte[i]));
      if (largeurMax < largeurTexte)
   largeurMax = largeurTexte;
      }
   
   largeurMax += 6;
   largeurMax -= 6;
   
   c_wgllwi(1);
   
   if (nbChampsActifs < 2)
      {
      op = NO_OP;
      }
   
   if (op == NO_OP || nbChampsActifs < 2)
      {
      switch (nbChampsActifs)
   {
   case 1:
   rect[0][0] = i1 + 0.5 * (largeur - largeurMax);
   rect[0][1] = (int)(((int)(j2+0.5*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
   break;
   
   case 2:
   rect[0][0] = i1 + 0.5 * (largeur - largeurMax);
   rect[1][0] = rect[0][0];
   rect[0][1] = (int)(((int)(j2+0.66*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
   rect[1][1] = (int)(((int)(j2+0.33*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
   break;
   
   case 3:
   case 4:
   rect[0][0] = largeurFenetre/2 - largeurTexte - 20;
   rect[1][0] = largeurFenetre/2 + 20;
   rect[2][0] = rect[0][0];
   rect[3][0] = rect[1][0];
   
   rect[0][1] = (int)(((int)(j2+0.66*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
   rect[1][1] = rect[0][1];
   rect[2][1] = (int)(((int)(j2+0.33*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
   rect[3][1] = rect[2][1];
   break;
   }
      for (i=0; i < nbChampsActifs; i++)
   {
   c_wglcol(xc.attributs[i].indCouleurFore);
   AfficherSymbole(rect[i][0]-10, rect[i][1]+hauteurTexte/2, i);
   c_wglrli(rect[i][0]-18,rect[i][1]-8+hauteurTexte/2, rect[i][0]-2,rect[i][1]+8+hauteurTexte/2);
   c_wglpsi(rect[i][0]+5, rect[i][1], texte[i], strlen(texte[i]), fontSize, 0, 0);
   }
      }
   else
      {
      rect[0][0] = i1 + 0.5 * (largeur - largeurMax);
      rect[2][0] = rect[0][0];
      
      rect[0][1] = (int)(((int)(j2+0.66*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
      rect[2][1] = (int)(((int)(j2+0.33*(hauteurFenetre-j2))) - hauteurTexte * 0.5);
      }

   for (i=0; i < nbChampsActifs; i+=2)
      {
      c_wglcol(xc.attributs[i].indCouleurFore);
      AfficherSymbole(rect[i][0]-10, rect[i][1]+hauteurTexte/2, i);
      c_wglrli(rect[i][0]-18,rect[i][1]-8+hauteurTexte/2, rect[i][0]-2,rect[i][1]+8+hauteurTexte/2);
      c_wglpsi(rect[i][0]+5, rect[i][1], texte[i], strlen(texte[i]), fontSize, 0, 0);
      }
   }   

/**
***************
***************
**/


AfficherLegendeVentUVW(ventUVWmax)
float ventUVWmax;
{
   int largeurLegende,hauteurLegende;
   
   GetDimensionsLegendeVent(&largeurLegende,&hauteurLegende);
   AfficherLegendeVent(ventUVWmax,10,10,0);
   
   }

GetDimensionsLegendeVent(largeur,hauteur)
int *largeur, *hauteur;
{
   int hauteurTexte,hauteurLegende;
   int largeurLegende, largeurTexte;
   float x,y;
   int i,j,rayon,longueur,lwi;
   char nombre[16];

   longueur = WindMgrGetLongueur();
   rayon = longueur;

   hauteurTexte   =  c_wglasi("123456", 6);
   largeurTexte   =  c_wglwsi("123", 3);
   hauteurLegende = 8 * hauteurTexte;
   largeurLegende = rayon<largeurTexte ? largeurTexte+20 : rayon+20;

   *largeur = largeurLegende;
   *hauteur = hauteurLegende;
   
   }

extern void PointerFleche(float xdepart, float ydepart, float dirVent, float vitVent, int rayon, int width);

extern int flagLegendeFleches;

AfficherLegendeVent(ventMax, offsetX, offsetY, angleFleche)
float ventMax;
int offsetX, offsetY,angleFleche;
{
   int hauteurTexte,hauteurLegende;
   int largeurLegende, largeurTexte;
   float x,y;
   int i,j,rayon,longueur,lwi,linewidth, tmp;
   float fraction;
   char nombre[16];
   int txtOffX,txtOffY;
   char format[8];
   
   int variation = WindMgrGetCroissance();

   if (flagLegendeFleches == 0) return;

   tmp = variation;
   switch(tmp)
      {
      case CUBIQUE:
      variation = RACINE_CUBIQUE;
      break;
      
      case QUADRATIQUE:
      variation = RACINE_CARREE;
      break;
      
      case RACINE_CUBIQUE:
      variation = CUBIQUE;
      break;
      
      case RACINE_CARREE:
      variation = QUADRATIQUE;
      break;
      }

   if (ventMax < 1.0)
      {
      strcpy(format,"%4.3f");
      }
   else
      {
      strcpy(format,"%03.0f");
      }

   longueur = WindMgrGetLongueur();
   rayon = longueur;
   txtOffX = ROUND(5*sin(angleFleche*0.01745));
   txtOffY = ROUND(5*cos(angleFleche*0.01745));

   hauteurTexte   =  c_wglasi("123456", 6);
   largeurTexte   =  c_wglwsi("123", 3);
   hauteurLegende = 8 * hauteurTexte+5;
   largeurLegende = rayon<largeurTexte ? largeurTexte+20 : rayon+20;
   
   c_wglcol(BLANC);
   
   i = viewp.vi1+offsetX;
   j = viewp.vj1+offsetY;
   c_wglrfi(i,j,i+largeurLegende,j+hauteurLegende);
   c_wglcol(NOIR);
   c_wglrli(i,j,i+largeurLegende,j+hauteurLegende);

   i = viewp.vi1+10+offsetX;
   j = viewp.vj1+offsetY+ROUND(6.5*hauteurTexte);
   c_wgliax(&x,&y, i,j);
   sprintf(nombre,format, ventMax);
   c_wglpsi(i+txtOffX,j+txtOffY, nombre, strlen(nombre), 17, 0, 0);
   linewidth = WindMgrGetEpaisseur();
   PointerFleche(x,y,270.-(float)angleFleche,ventMax,rayon, linewidth);
   
   j = viewp.vj1+offsetY+ROUND(4.5*hauteurTexte);
   c_wgliax(&x,&y, i,j);
   fraction = 0.75;
   enhancefracs(&fraction,1,0.0,1.0,variation);
   sprintf(nombre,format, fraction*ventMax);
   c_wglpsi(i+txtOffX,j+txtOffY, nombre, strlen(nombre), 17, 0, 0);
   lwi = ROUND(0.75 * linewidth);
   rayon = longueur * 0.75;
   PointerFleche(x,y,270.-(float)angleFleche,0.75*ventMax,rayon, lwi);
   
   j = viewp.vj1+offsetY+ROUND(2.5*hauteurTexte);
   c_wgliax(&x,&y, i,j);
   fraction = 0.50;
   enhancefracs(&fraction,1,0.0,1.0,variation);
   sprintf(nombre,format, fraction*ventMax);
   c_wglpsi(i+txtOffX,j+txtOffY, nombre, strlen(nombre), 17, 0, 0);
   lwi = ROUND(0.5*linewidth);
   rayon = longueur * 0.5;
   PointerFleche(x,y,270.-(float)angleFleche,0.50*ventMax,rayon, lwi);
   
   j = viewp.vj1+offsetY+ROUND(0.5*hauteurTexte);
   c_wgliax(&x,&y, i,j);
   fraction = 0.25;
   enhancefracs(&fraction,1,0.0,1.0,variation);
   sprintf(nombre,format, fraction*ventMax);
   c_wglpsi(i+txtOffX,j+txtOffY, nombre, strlen(nombre), 17, 0, 0);
   lwi = ROUND(0.25*linewidth);
   rayon = longueur * 0.25;
   PointerFleche(x,y,270.-(float)angleFleche,0.25*ventMax,rayon, lwi);
   }

CombinerLegendeDiffChamps(texte, champ1, champ2)
char texte[];
_Champ *champ1, *champ2;
{
   int tempDate1,tempDate2,deltaT,lng;
   char buffer[72];
   static char *Mois[] = {"bid", "Jan", "Fev", "Mars", "Avr", "Mai", "Juin", "Juil", "Aout", "Sep", "Oct", "Nov", "Dec"};
   static char *Month[] = {"bid", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
   
   char dateMess1[24],dateMess2[24];
   char **leMois;
   char mo[4];
   
   strcpy(texte,"");

   lng = c_getulng();
   if (lng == 0)
      {
      leMois = Mois;
      }
   else
      {
      leMois = Month;
      }

   if (0 == strcmp(champ1->nomvar, champ2->nomvar))
      sprintf(buffer, "%s*", champ1->nomvar);
   else
      sprintf(buffer, "[%s-%s]*", champ1->nomvar, champ2->nomvar);
   strcat(texte,buffer);
   
   if (0 == strcmp(champ1->typvar, champ2->typvar))
      sprintf(buffer, "%s*", champ1->typvar);
   else
      sprintf(buffer, "[%s-%s]*", champ1->typvar, champ2->typvar);
   strcat(texte,buffer);
   
   if (champ1->ip1 == champ2->ip1)
     {
     strcat(texte, &(champ1->titreNiveau[7]));
     }
   else
     {
     sprintf(buffer, "[%s-%s]*", &(champ1->titreNiveau[7]), &(champ2->titreNiveau[7]));
     strcat(texte,buffer);
     }
   
   if (champ1->ip2 == champ2->ip2)
      sprintf(buffer, "%3d*", champ1->ip2);
   else
      sprintf(buffer, "[%3d-%3d]*", champ1->ip2, champ2->ip2);
   strcat(texte,buffer);
   
   if (champ1->ip3 == champ2->ip3)
      sprintf(buffer, "%3d*", champ1->ip3);
   else
      sprintf(buffer, "[%3d-%3d]*", champ1->ip3, champ2->ip3);
   strcat(texte,buffer);
   
   mo[3] = '\0';
   mo[4] = '\0';
   strncpy(mo,champ2->mois,3);
   sprintf(dateMess2, "V%s:%sZ %s%s%s", 
     champ2->heureInit,champ2->minute,champ2->jour,mo,champ2->annee);
   
   mo[3] = '\0';
   mo[4] = '\0';
   strncpy(mo,champ1->mois,3);
   sprintf(dateMess1, "V%s:%sZ %s%s%2s", 
     champ1->heureInit,champ1->minute,champ1->jour,mo,champ1->annee);
   
   if (0 == strcmp(dateMess2,dateMess1))
      sprintf(buffer, "%s*", dateMess1);
   else
      sprintf(buffer, "[%s-%s]*", dateMess1,dateMess2);
   strcat(texte,buffer);
   
   if (0 == strcmp(champ1->etiket,champ2->etiket))
      sprintf(buffer, "%s", champ1->etiket);
   else
      sprintf(buffer, "[%s-%s]", champ1->etiket, champ2->etiket);
   strcat(texte,buffer);
   }
