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

#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

#include <wgl_x.h>
#include <rec.h>
#include <rec_functions.h>
#include <gd.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget pdTopLevel = NULL;
Widget pdForme, pdFrame, pdFormeDiapo, pdFrameDiapo, pdFrameProduire, pdProduire, pdProduireCoupe, pdAfficher, pdOk;
Widget pdFrame1, pdForme1, pdFrameFenetre, pdLabelFenetre,pdFrameChamp2D, pdFrameCoupe;
Widget pdChamp2D, pdCoupe, pdChoixFenetre;
Widget pdRCmain;
Widget    pdRCcontrole;
Widget       pdPanneauFormat;
Widget          pdOptionsFormat;
Widget             pdOptionsFormatItems[4];
Widget       pdPanneauResolution;
Widget          pdOptionsResolution;
Widget             pdOptionsResolutionItems[6];
Widget       pdRCsliders;
Widget            pdRCTaille,pdRCTaillePixel,pdRCTaillePhysique,pdRCAmplification,pdRCMemoire;
Widget            pdTaille,pdTaillePixel,pdTaillePhysique,pdAmplification,pdMemoire;
Widget            pdFrameTaille,pdFrameTaillePixel,pdFrameTaillePhysique,pdFrameAmplification,pdFrameMemoire;
Widget            pdInfoTaille,pdInfoTaillePixel,pdInfoTaillePhysique,pdInfoAmplification,pdInfoMemoire;
Widget    pdRCNomFichier,pdLabelNomFichier,pdFrameNomFichier,pdNomFichier;
Widget       pdRCinfo;


static char *nomPanneauDiapo[] = {"PanneauDiapos", "SlidePanel"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelChamp2D[] = {"Champ2D", "2DField"};
static char *labelFenetre[] = {"Fenetre:", "Window:"};
static char *labelCoupe[] = {"Coupe", "XSection"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};
static char *labelTaille[] = {"Agrandissement","Enlarge"};
static char *labelTaillePixels[]   = {"Taille (pixels)  ","Size (pixels)   "};
static char *labelNomFichier[] =     {"Fichier (PNG):","File (PNG):"};
static char *labelProduire[] = {"Generer Image","Export Image"};
static char *captureCompletee[] = {"Capture complétée", "Capture complete"};


char panneauDiapoGeometrie[32];
static char nomFichierSortie[256];

int pdSelectionTerminee;

int largeurFenetre, hauteurFenetre;
int largeurPNG,hauteurPNG;
int resolution= 100;
static float amplif = 1.0;

static int idFenetreCourante = 0;
static int nplanes;

/***
******************
******************
***/

void DesactiverPanneauDiapo()
{
   XtUnrealizeWidget(pdTopLevel);

   }


/***
******************
******************
***/

void PdAdjustLargeur(Widget w, caddr_t client_data, caddr_t call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;

   AjusterLargeur(donnees->value);
   }

void AjusterLargeur(int taille)
{
   
  amplif = 1.0*taille;
  largeurPNG = ROUND(largeurFenetre*amplif);
  hauteurPNG = ROUND(amplif*hauteurFenetre);
  
  AjusterLabelsTaille();
   }

void AjusterLabelsTaille()
{
  XmString xmstr;
  char str[255];
  Arg args[2];
  sprintf(str,"%04d x %04d",largeurPNG,hauteurPNG);
  xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[0],XmNlabelString,xmstr);
  XtSetValues(pdInfoTaillePixel,args,1);
  XmStringFree(xmstr);
}

extern unsigned int colorbitrange[3][3];

void ChoixFenetre(Widget w, caddr_t client_data, caddr_t call_data)
{
  
  int win;
  
  
  idFenetreCourante = (int)client_data;
  
  switch (idFenetreCourante)
    {
    case 0:
      GetFenetreAffichageID(&win);
      break;
      
    case 1:
      if (VerifierExistenceCoupeValide())
	{
	CoupeMgrGetFenetreCoupeID(&win);
	}
      else
	{
	idFenetreCourante = 0;
	GetFenetreAffichageID(&win);
	}
      break;
    }

  c_wglsetw(win);
  wglDrawable = win;
  c_wglgwz(&largeurPNG,&hauteurPNG);
  largeurPNG = ROUND(largeurPNG*amplif);
  hauteurPNG = ROUND(hauteurPNG*amplif);
  AjusterLabelsTaille();
}


void ExportImage()
{
   int i;
   
   char *returnedStr;
   char nomFichierPNG[256];
   int fenetreCoupe;
   Window win;


   switch (idFenetreCourante)
     {
     case 0:
       printf("ExportImage2D...\n");
       returnedStr = (char *) XmTextFieldGetString(pdNomFichier);
       
       strcpy(nomFichierSortie, "");
       i = 0;
       while(returnedStr[i] != '.' && i < strlen(returnedStr))
	 {
	 nomFichierSortie[i] = returnedStr[i];
	 i++;
	 }
       nomFichierSortie[i] = '\0';
       
       win = c_wglgacw();
       break;

     case 1:
       printf("ExportImage Coupe...\n");
       CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
       returnedStr = (char *) XmTextFieldGetString(pdNomFichier);
       
       strcpy(nomFichierSortie, (char *)getenv("TMPDIR"));
       i = 0;
       while(returnedStr[i] != '.' && i < strlen(returnedStr))
	 {
	 nomFichierSortie[i] = returnedStr[i];
	 i++;
	 }
       nomFichierSortie[i] = '\0';
       
       c_wglsetw(fenetreCoupe);
       win = fenetreCoupe;
       break;
     }



   PdAllouerRessourcesImage();
   wglDrawable = bgPix;
   
   switch(idFenetreCourante)
     {
     case 0:
       RedessinerFenetreAffichage();
       break;

     case 1:
       RedessinerFenetreCoupe();
       break;
     }
   
   strcpy(nomFichierPNG, nomFichierSortie);
   strcat(nomFichierPNG,".png");
   PdCreatePNGimage(nomFichierPNG);

   PdDesallouerRessourcesImage();
   wglDrawable =  win;
   c_wglsetw(win);
   c_wglsbf();
   c_wgldbf();
   c_wglfbf();

   MessageAvertissement(captureCompletee[lng], AVERTISSEMENT);
}


void ExportMovie()
{
  int n;
   
   int win;
   char nomFichierPNG[256];
   _Champ *champ;

   printf("ExportMovie...\n");

   FldMgrGetChamp(&champ,0);
   FldMgrLoadTimeAnimationSeq();
   
   RemettreChampsAJour(0);
   
   
   largeurPNG = amplif * w;
   hauteurPNG = amplif * h;
   GetFenetreAffichageID(&win);
   PdAllouerRessourcesImage();
   for (n=0; n < champ->seqanim.nbFldsAnim; n++)
     {
     sprintf(nomFichierPNG,"%03d.png",n);
     printf("%s\n", nomFichierPNG);
     wglDrawable = bgPix;
     AnimerFrames(1);
     PdCreatePNGimage(nomFichierPNG);
     }
   wglDrawable = win;
   PdDesallouerRessourcesImage();
   MessageAvertissement(captureCompletee[lng], AVERTISSEMENT);
}




void PdReadDiapo(Widget w, caddr_t client_data, caddr_t call_data) 
{

   }


void PdSelectNomvar(Widget w, caddr_t client_data, caddr_t call_data) 
{

   }


void PdSetDiapoAuto(Widget w, caddr_t client_data, caddr_t call_data)
{

   }

/**
*************
*************
**/

void PdSetDiapoFixe(Widget w, caddr_t client_data, caddr_t call_data) 
{

   }

/**
*************
*************
**/

XtCallbackProc PdOk(Widget w, caddr_t client_data, caddr_t call_data)
{
   return 0;
   }

XtCallbackProc PdAfficher(Widget w, caddr_t client_data, caddr_t call_data)
{
   int taille;

   RedessinerFenetres(); 
   c_wglgwz(&largeurFenetre,&hauteurFenetre);
   XmScaleGetValue(pdTaille,&taille);
   AjusterLargeur(taille);
   return 0;
   }


void InitPanneauDiapo()
{

   int i;
   Arg args[16];
   char nomShell[128];
   char dimstr[128];
   XmString label;
   
   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauDiapo[lng]);
   i = 0;
   if (0 < strlen(panneauDiapoGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneauDiapoGeometrie);
      i++;
      }
   
   pdTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                 applicationShellWidgetClass,
                                 XtDisplay(SuperWidget.topLevel), args, i);
   
   i = 0;
   pdForme = (Widget) XmCreateForm(pdTopLevel, "form", args, i);
   XtManageChild(pdForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pdOk = (Widget)XmCreatePushButton(pdForme, labelOk[lng], args, i);
   XtAddCallback(pdOk, XmNactivateCallback, DesactiverPanneauDiapo, NULL);
   XtManageChild(pdOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pdOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pdAfficher = (Widget)XmCreatePushButton(pdForme, labelAfficher[lng], args, i);
   XtAddCallback(pdAfficher, XmNactivateCallback, (XtCallbackProc) PdAfficher, NULL);
   XtManageChild(pdAfficher);

   /*--------------------------------------------------------------------------------*/

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pdOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pdFrame1 = (Widget) XmCreateFrame(pdForme, "form", args, i);
   XtManageChild(pdFrame1);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdForme1 = (Widget) XmCreateRowColumn(pdFrame1, "form", args, i);
   XtManageChild(pdForme1);

   pdFrameFenetre = (Widget) XmCreateFrame(pdForme1, "form", NULL, 0);
   XtManageChild(pdFrameFenetre);
   pdLabelFenetre = (Widget) XmCreateLabel(pdFrameFenetre, labelFenetre[lng], NULL, 0);
   XtManageChild(pdLabelFenetre);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdChoixFenetre = (Widget) XmCreateRadioBox(pdForme1, "radiobox", args, i);
   XtManageChild(pdChoixFenetre);

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pdChamp2D = (Widget) XmCreateToggleButton(pdChoixFenetre, labelChamp2D[lng], args, i);
   XtAddCallback(pdChamp2D, XmNvalueChangedCallback,(XtCallbackProc) ChoixFenetre, 0);
   XtManageChild(pdChamp2D);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pdCoupe = (Widget) XmCreateToggleButton(pdChoixFenetre, labelCoupe[lng], args, i);
   XtAddCallback(pdCoupe, XmNvalueChangedCallback, (XtCallbackProc) ChoixFenetre,  (XtPointer)1);
   XtManageChild(pdCoupe);
   
   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pdFrame1); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pdFrame = (Widget) XmCreateFrame(pdForme, "form", args, i);
   XtManageChild(pdFrame);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pdFrame1); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pdRCmain = (Widget)XmCreateRowColumn(pdForme, "pdRc", args, i);
   XtManageChild(pdRCmain);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pdRCcontrole = (Widget)XmCreateRowColumn(pdRCmain, "pdRc", args, i);
   XtManageChild(pdRCcontrole);

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   largeurPNG = largeurFenetre;
   hauteurPNG = hauteurFenetre;
   amplif = (float)largeurPNG/(float)largeurFenetre;

   i = 0;
   label = XmStringCreateLtoR(labelTaille[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNscaleMultiple, 1); i++;
   XtSetArg(args[i], XmNminimum, 1); i++;
   XtSetArg(args[i], XmNmaximum, 10); i++;
   XtSetArg(args[i], XmNvalue, 1); i++;
   pdTaille = (Widget)XmCreateScale(pdRCcontrole, labelTaille[lng], args, i);
   XtAddCallback(pdTaille, XmNdragCallback, (XtCallbackProc) PdAdjustLargeur, NULL);
   XtAddCallback(pdTaille, XmNvalueChangedCallback, (XtCallbackProc) PdAdjustLargeur, NULL);
   XtManageChild(pdTaille);
   XmStringFree(label);



   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdRCTaillePixel = (Widget)XmCreateRowColumn(pdRCcontrole, "rc", args, i);
   XtManageChild(pdRCTaillePixel);

   pdTaillePixel = (Widget)XmCreateLabel(pdRCTaillePixel,labelTaillePixels[lng],args, 0);
   XtManageChild(pdTaillePixel);

   pdFrameTaillePixel = (Widget)XmCreateFrame(pdRCTaillePixel,"frame",args,0);
   XtManageChild(pdFrameTaillePixel);

   sprintf(dimstr,"%04d x %04d",largeurPNG,hauteurPNG);
   pdInfoTaillePixel = (Widget)XmCreateLabel(pdFrameTaillePixel,dimstr,args, 0);
   XtManageChild(pdInfoTaillePixel);

   pdFrameNomFichier = (Widget)XmCreateFrame(pdRCcontrole,"frame",args,0);
   XtManageChild(pdFrameNomFichier);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdRCNomFichier = (Widget)XmCreateRowColumn(pdFrameNomFichier, "rc", args, i);
   XtManageChild(pdRCNomFichier);

   pdLabelNomFichier = (Widget)XmCreateLabel(pdRCNomFichier,labelNomFichier[lng],args, 0);
   XtManageChild(pdLabelNomFichier);

   i = 0;
   XtSetArg(args[i], XmNcolumns, 16);  i++;
   XtSetArg(args[i], XmNmaxLength, 256);  i++;
   XtSetArg(args[i], XmNvalue, "image.png"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, True); i++;
   pdNomFichier = (Widget)XmCreateTextField(pdRCNomFichier,"image",args, i);
   XtManageChild(pdNomFichier);

   pdFrameProduire = (Widget)XmCreateFrame(pdRCcontrole,"frame",args,0);
   XtManageChild(pdFrameProduire);

   i = 0;
   pdProduire = (Widget)XmCreatePushButton(pdFrameProduire, labelProduire[lng], args, i);
   XtAddCallback(pdProduire, XmNactivateCallback, (XtCallbackProc) ExportImage, NULL);
   XtManageChild(pdProduire);
  }

   



/**
 ******************************************************************************
 ******************************************************************************
 **/


void ActiverPanneauDiapos()
{
   
   if (!pdTopLevel)
      InitPanneauDiapo();
   
   if (!XtIsRealized(pdTopLevel))
      {
      XtRealizeWidget(pdTopLevel);
      }
   
   f77name(xpandact)();
   
   }

void f77name(xpandact)()
{
   LocalEventLoop(pdTopLevel);
   }


void PdCreatePNGimage(char *nomFichierPNG)
{
  int rgb[256][3];
  XImage *xi;
  
  int x,y;
  int i,j,ic;
  FILE *fout;
  unsigned int pixel;
  unsigned char rouge, vert, bleu;
  
  Window root;
  unsigned int border_width, depth, nplanes;
  gdImagePtr im;
  
  XGetGeometry(wglDisp, bgPix, &root, &x, &y, &largeurPNG, &hauteurPNG, &border_width, &depth);  
  printf("GetImage...%dx%d\n",largeurPNG, hauteurPNG);
  xi = XGetImage(wglDisp,bgPix,0,0,largeurPNG, hauteurPNG, AllPlanes,ZPixmap);
  printf("GetImage...done\n");
  
  fout = fopen(nomFichierPNG, "w" );
  printf("generating png...\n");
  
  nplanes = c_wglgpl();
  switch (nplanes)
    {
    case 8:
      for (i=0; i < 256; i++)
	{
	rgb[i][0] = couleurs[i].r;
	rgb[i][1] = couleurs[i].g;
	rgb[i][2] = couleurs[i].b;
	}
      
      for (i=0; i < 256; i++)
	{
	for (j=0; j < 3; j++)
	  {
	  rgb[i][j] = rgb[i][j] > 255 ? 255 : rgb[i][j];
	  }
	}
      im = gdImageCreate(largeurPNG, hauteurPNG);
      for (j=0; j < hauteurPNG; j++)
	{
	for (i=0; i< largeurPNG; i++)
	  {
	  ic = (unsigned int)  XGetPixel(xi, i,j);
	  pixel = gdImageColorResolve(im, rgb[ic][0], rgb[ic][1], rgb[ic][2]); 
	  gdImageSetPixel(im, i, j , pixel);
	  }
	}
      break;
      
    default:
      im = gdImageCreateTrueColor(largeurPNG, hauteurPNG);
      
      for (j=0; j < hauteurPNG; j++)
	{
	for (i=0; i< largeurPNG; i++)
	  {
	  pixel = (unsigned int)  XGetPixel(xi, i,j);
	  rouge = (unsigned char) (pixel >> colorbitrange[0][0]) * (256/colorbitrange[0][2]);
	  vert  = (unsigned char) (pixel >> colorbitrange[1][0]) * (256/colorbitrange[1][2]);
	  bleu  = (unsigned char) (pixel >> colorbitrange[2][0]) * (256/colorbitrange[2][2]);
	  pixel = gdImageColorAllocate(im, (int)rouge, (int)vert, (int) bleu);
	  gdImageSetPixel(im, i, j , pixel);
	  }
	}
      break;
    }
  
  XDestroyImage(xi);
  gdImagePng(im, fout);
  fclose(fout);
  gdImageDestroy(im);
  printf("generating png...done\n");
  
}


void PdAllouerRessourcesImage()
{
  nplanes = c_wglgpl();
  bgPix = XCreatePixmap(wglDisp, RootWindow(wglDisp, wglScrNum), largeurPNG,hauteurPNG, nplanes);
  wglDrawable = bgPix;
   

}

void PdDesallouerRessourcesImage()
{
  XFreePixmap(wglDisp, bgPix);
  bgPix = (int) NULL;

}
