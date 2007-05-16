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

#ifdef X_WGL
#include <Xm/Xm.h>

#include <rpnmacros.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl_x.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget pdTopLevel = NULL;
Widget pdForme, pdFrame, pdFormeDiapo, pdFrameDiapo, pdProduire, pdAfficher, pdOk;
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
static char *labelTopLevel[] = {"Diapo", "Diapo"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelProduire[] = {"Generer Image", "Write Image"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};
static char *labelOptionsFormat[] = {"gif","pict","jpeg","eps"};
static char *labelOptionsResolution[] = {"75  ","100 ","150 ","200 ","300 ","600 "};
static char *labelRecalculer[] = {"Recalculer parametres", "Update parameters"};
static char *labelTaille[] = {"Taille","Size"};
static char *labelTaillePixels[]   = {"Taille (pixels)  ","Size (pixels)   "};
static char *labelTaillePhysique[] = {"Taille (pouces)  ","Size (inches)   "};
static char *labelMemoire[] =        {"Memoire (Kbytes):","Memory (Kbytes):"};
static char *labelNomFichier[] =     {"Nom fichier:     ","File name:      "};
static char *labelHauteurPhysique[] = {"Hauteur (pouces)","Hauteur (pouces)"};
static char *labelAmplification[] = {"Amplification","Amplification"};
static char *labelFormat[]     = {"Format:         ","Format:         "};
static char *labelResolution[] = {"Resolution (ppp)","Resolution (dpi)"};


static int currentItem;
char panneauDiapoGeometrie[32];
static char pdNomVar[256][3];
static char nomFichierSortie[256];

int pdSelectionTerminee;

static int nDiapos = 0;
static int itemDict = 0;
int largeurFenetre, hauteurFenetre;
int largeurGIF,hauteurGIF;
int resolution= 100;
float largeurPHYS,hauteurPHYS,amplif;


void SetFormatToggle(w, client_data, call_data)
Widget  w;          
caddr_t client_data;
caddr_t call_data;  
{
   };



/***
******************
******************
***/

void PdRes(w, client_data, call_data)
Widget  w;              /*  widget id           */
caddr_t client_data;    /*  data from application   */
caddr_t call_data;      /*  data from widget class  */
{
   int i;
   Arg args[2];
   float newres;

   resolution = atoi(labelOptionsResolution[(int)client_data]);

   largeurPHYS = (float)largeurGIF/(float)resolution;
   hauteurPHYS = (float)hauteurGIF/(float)resolution;

   AjusterLabelsTaille();
   }

/***
******************
******************
***/

void PdAdjustLargeur(w, client_data, call_data)
Widget  w;              /*  widget id           */
caddr_t client_data;    /*  data from application   */
caddr_t call_data;      /*  data from widget class  */
{
   int i;
   Arg args[2];
   float ratio;
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;

   AjusterLargeur(donnees->value);
   }

AjusterLargeur(taille)
int taille;
{
   
   largeurGIF = ROUND(largeurFenetre*taille*.01);
   amplif = (float)largeurGIF/(float)largeurFenetre;
   hauteurGIF = ROUND(amplif*(float)hauteurFenetre);
   largeurPHYS = (float)largeurGIF/(float)resolution;
   hauteurPHYS = (float)hauteurGIF/(float)resolution;
   
   AjusterLabelsTaille();
   }

AjusterLabelsTaille()
{
   XmString xmstr;
   char str[255];
   Arg args[2];
   
   sprintf(str,"%04d x %04d",largeurGIF,hauteurGIF);
   xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0],XmNlabelString,xmstr);
   XtSetValues(pdInfoTaillePixel,args,1);
   XmStringFree(xmstr);
   
   sprintf(str,"%05.2f x %05.2f",largeurPHYS,hauteurPHYS);
   xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0],XmNlabelString,xmstr);
   XtSetValues(pdInfoTaillePhysique,args,1);
   XmStringFree(xmstr);

   sprintf(str,"%6d",ROUND(largeurGIF*hauteurGIF/1024.0));
   xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0],XmNlabelString,xmstr);
   XtSetValues(pdInfoMemoire,args,1);
   }


void ExportImage()
{
   int rgb[256][3];
   XImage *xi;

   int x,y,w,h;
   int i,j,ind;
   int ncarFontSize;
   FILE *fout;
   unsigned char c[10000];
   
   Window root;
   unsigned int width, height, border_width, depth, nplanes;
   char *tmpdir, *returnedStr;
   char nomFichierPPM[256],nomFichierPGM[256],commande[1024];
   Window win;
   _Fenetre fen;


   printf("ExportImage...\n");
   fen = fenetre[0];
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
   for (i=0; i < 256; i++)
      {
      rgb[i][0] = couleurs[i].red / 256;
      rgb[i][1] = couleurs[i].green / 256;
      rgb[i][2] = couleurs[i].blue / 256;
      }
   
   for (i=0; i < 256; i++)
      {
      for (j=0; j < 3; j++)
	 {
	 rgb[i][j] = rgb[i][j] > 255 ? 255 : rgb[i][j];
	 }
      }

   c_wglgwz(&w,&h);
   nplanes = c_wglgpl();
   bgPix = XCreatePixmap(wglDisp, RootWindow(wglDisp, wglScrNum), largeurGIF,hauteurGIF, nplanes);
   wglDrawable = bgPix;

   RedessinerFenetreAffichage();

   XGetGeometry(wglDisp, bgPix, &root, &x, &y, &w, &h, &border_width, &depth);  
   printf("GetImage...%dx%d\n",w,h);
   xi = XGetImage(wglDisp,bgPix,0,0,w,h,AllPlanes,ZPixmap);
   printf("GetImage...done\n");

   tmpdir = (char *)getenv("TMPDIR");

   strcpy(nomFichierPPM, nomFichierSortie);
   strcat(nomFichierPPM,".ppm");
   fout = fopen(nomFichierPPM, "w" );
   fprintf(fout,"P3\n");
   fprintf(fout,"256 1\n255\n");
   printf("generating ppm...\n");
   for (i=0; i< 256; i++)
      {
      fprintf(fout,"%d %d %d\n",rgb[i][0],rgb[i][1],rgb[i][2]);
      }
   fclose(fout);
   printf("generating ppm...done\n");

   strcpy(nomFichierPGM, nomFichierSortie);
   strcat(nomFichierPGM,".pgm");
   fout = fopen(nomFichierPGM, "w" );
   fprintf(fout,"P5\n");
   fprintf(fout,"%d %d\n255\n",w,h);
   printf("generating pgm...\n");
   for (j=0; j < h; j++)
      {
      for (i=0; i< w; i++)
	 {
	 c[i] = (unsigned char) XGetPixel(xi, i,j);
	 }
      fwrite(c,sizeof(unsigned char),w,fout);
      }
   fclose(fout);
   printf("generating pgm...done\n");


   sprintf(commande,"r.gcv.gif %s", nomFichierSortie);
   printf("%s\n",commande);
   
   printf("generating gif...\n");
   system(commande);
   printf("generating gif...done\n");
   
   XFreePixmap(wglDisp, bgPix);
   bgPix = NULL;
   XDestroyImage(xi);
   wglDrawable =  win;
   fenetre[0] = fen;
   c_wglsetw(win);
   c_wglsbf();
   c_wgldbf();
   c_wglfbf();
   RedessinerFenetreAffichage();
   }





void ExportMovie()
{
   int fenetre;
   int rgb[256][3];
   XImage *xi;

   int x,y,w,h;
   int i,j,n,ind;
   int ncarFontSize;
   FILE *fout;
   unsigned char c[10000];
   
   Window root;
   unsigned int width, height, border_width, depth, nplanes;
   char *tmpdir;
   char nomFichierPPM[256],nomFichierPGM[256],commande[1024];
   int fenetreAffichage;
   _Champ *champ;

   printf("ExportMovie...\n");
   FldMgrGetChamp(&champ,0);
   FldMgrLoadTimeAnimationSeq();
   RedessinerFenetreAffichage();

   RemettreChampsAJour(0);

   for (i=0; i < 256; i++)
      {
      rgb[i][0] = couleurs[i].red / 256;
      rgb[i][1] = couleurs[i].green / 256;
      rgb[i][2] = couleurs[i].blue / 256;
      }
   
   for (i=0; i < 256; i++)
      {
      for (j=0; j < 3; j++)
	 {
	 rgb[i][j] = rgb[i][j] > 255 ? 255 : rgb[i][j];
	 }
      }

   tmpdir = (char *) getenv("TMPDIR");
   strcpy(nomFichierPPM, tmpdir);
   strcat(nomFichierPPM,"/test.ppm");
   fout = fopen(nomFichierPPM, "w" );
   fprintf(fout,"P3\n");
   fprintf(fout,"256 1\n255\n");
   printf("generating ppm...\n");
   for (i=0; i< 256; i++)
      {
      fprintf(fout,"%d %d %d\n",rgb[i][0],rgb[i][1],rgb[i][2]);
      }
   fclose(fout);
   printf("generating colormap...done\n");

   
   GetFenetreAffichageID(&fenetreAffichage);
   for (n=0; n < champ->seqanim.nbFldsAnim; n++)
      {
      XGetGeometry(wglDisp, fenetreAffichage, &root, &x, &y, &w, &h, &border_width, &depth);  
      printf("GetImage...%dx%d\n",w,h);
      xi = XGetImage(wglDisp,fenetreAffichage,0,0,w,h,AllPlanes,ZPixmap);
      printf("GetImage...done\n");
      
      tmpdir = (char *)getenv("TMPDIR");
      
      sprintf(nomFichierPGM,"%s/%03d.pgm",tmpdir,n);
      fout = fopen(nomFichierPGM, "w" );
      fprintf(fout,"P5\n");
      fprintf(fout,"%d %d\n255\n",w,h);
      printf("generating pgm...\n");
      for (j=0; j < h; j++)
	 {
	 for (i=0; i< w; i++)
	    {
	    c[i] = (unsigned char) XGetPixel(xi, i,j);
	    }
	 fwrite(c,sizeof(unsigned char),w,fout);
	 }
      fclose(fout);
      printf("generating pgm...done\n"); 
      sprintf(commande,"ppmmerge pgmtoppm -map %s %s | ppmmerge ppmtogif > %03d.gif;rm %s",nomFichierPPM,nomFichierPGM,n,nomFichierPGM);
      printf("%s\n",commande);
      
      printf("generating gif...\n");
      system(commande);
      printf("generating gif...done\n");
      XDestroyImage(xi);

      AnimerFrames(1);
     }
   }




void PdReadDiapo(w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{

   }


void PdSelectNomvar(w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int i;
   char tempStr[16];
   float min,max;
   Arg args[16];
   int varMode;

   XmListCallbackStruct *info = (XmListCallbackStruct *) call_data;

   }


void PdSetDiapoAuto(w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int i;
   char tempStr[16];

   }

/**
*************
*************
**/

void PdSetDiapoFixe(w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   Arg args[16];
   int i;
   
   char tempStr[16],*returnedStr;
   float min,max;

   }

/**
*************
*************
**/

static XtCallbackProc PdOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{

   }

static XtCallbackProc PdAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;
   int taille;

   RedessinerFenetres(); 
   c_wglgwz(&largeurFenetre,&hauteurFenetre);
   XmScaleGetValue(pdTaille,&taille);
   AjusterLargeur(taille);
   }


InitPanneauDiapo()
{

   int i,j,n;
   Position height;
   Arg args[16];
   XmString string,label;
   XmStringTable table;
   char nomShell[128];
   char *armnlib;
   char nomFichierDiapo[128];
   char dimstr[128];
   
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
   XtAddCallback(pdOk, XmNactivateCallback, PdOk, NULL);
   XtManageChild(pdOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pdOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pdAfficher = (Widget)XmCreatePushButton(pdForme, labelAfficher[lng], args, i);
   XtAddCallback(pdAfficher, XmNactivateCallback, PdAfficher, NULL);
   XtManageChild(pdAfficher);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pdAfficher); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pdProduire = (Widget)XmCreatePushButton(pdForme, labelProduire[lng], args, i);
   XtAddCallback(pdProduire, XmNactivateCallback, ExportImage, NULL);
   XtManageChild(pdProduire);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pdOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pdFrame = (Widget) XmCreateFrame(pdForme, "form", args, i);
   XtManageChild(pdFrame);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pdOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pdRCmain = (Widget)XmCreateRowColumn(pdForme, "pdRc", args, i);
   XtManageChild(pdRCmain);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pdRCcontrole = (Widget)XmCreateRowColumn(pdRCmain, "pdRc", args, i);
   XtManageChild(pdRCcontrole);

   pdOptionsFormat = (Widget)XmCreatePulldownMenu(pdRCcontrole, labelFormat[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsFormat); n++)
	{
	i = 0;
	if (i > 0)
	   {
	   XtSetArg(args[i], XmNsensitive, False); i++;
	   }
	string = XmStringCreateLtoR(labelOptionsFormat[n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pdOptionsFormatItems[n] = XmCreatePushButtonGadget(pdOptionsFormat, labelOptionsFormat[n], args, i);
	XmStringFree(string);   
	XtAddCallback(pdOptionsFormatItems[n], XmNactivateCallback, SetFormatToggle, n);
	}

   XtManageChildren(pdOptionsFormatItems, XtNumber(labelOptionsFormat));

   i = 0;
   string = XmStringCreateLtoR(labelFormat[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pdOptionsFormat); i++;
   XtSetArg(args[i], XmNmenuHistory, pdOptionsFormatItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pdPanneauFormat = XmCreateOptionMenu(pdRCcontrole, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pdPanneauFormat);

   pdOptionsResolution = (Widget)XmCreatePulldownMenu(pdRCcontrole, labelResolution[lng], NULL, 0);

   for (n=0; n < XtNumber(labelOptionsResolution); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(labelOptionsResolution[n], XmSTRING_DEFAULT_CHARSET); 
	XtSetArg(args[i], XmNlabelString, string); i++;
	pdOptionsResolutionItems[n] = XmCreatePushButtonGadget(pdOptionsResolution, labelOptionsResolution[n], args, i);
	XmStringFree(string);   
	XtAddCallback(pdOptionsResolutionItems[n], XmNactivateCallback, PdRes, n);
	}

   XtManageChildren(pdOptionsResolutionItems, XtNumber(labelOptionsResolution));

   i = 0;
   string = XmStringCreateLtoR(labelResolution[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pdOptionsResolution); i++;
   XtSetArg(args[i], XmNmenuHistory, pdOptionsResolutionItems[1]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pdPanneauResolution = XmCreateOptionMenu(pdRCcontrole, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pdPanneauResolution);

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   resolution = 100;
   largeurGIF = largeurFenetre;
   hauteurGIF = hauteurFenetre;
   largeurPHYS = (float)(largeurGIF)/(float)resolution;
   hauteurPHYS = (float)(hauteurGIF)/(float)resolution;
   amplif = (float)largeurGIF/(float)largeurFenetre;


   i = 0;
   label = XmStringCreateLtoR(labelTaille[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, False); i++;
   XtSetArg(args[i], XmNscaleMultiple, 100); i++;
   XtSetArg(args[i], XmNminimum, 100); i++;
   XtSetArg(args[i], XmNmaximum, 1000); i++;
   XtSetArg(args[i], XmNvalue, 100); i++;
   pdTaille = (Widget)XmCreateScale(pdRCcontrole, labelTaille[lng], args, i);
   XtAddCallback(pdTaille, XmNdragCallback, PdAdjustLargeur, NULL);
   XtAddCallback(pdTaille, XmNvalueChangedCallback, PdAdjustLargeur, NULL);
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

   sprintf(dimstr,"%04d x %04d",largeurGIF,hauteurGIF);
   pdInfoTaillePixel = (Widget)XmCreateLabel(pdFrameTaillePixel,dimstr,args, 0);
   XtManageChild(pdInfoTaillePixel);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdRCTaillePhysique = (Widget)XmCreateRowColumn(pdRCcontrole, "rc", args, i);
   XtManageChild(pdRCTaillePhysique);

   pdTaillePhysique = (Widget)XmCreateLabel(pdRCTaillePhysique,labelTaillePhysique[lng],args, 0);
   XtManageChild(pdTaillePhysique);

   pdFrameTaillePhysique = (Widget)XmCreateFrame(pdRCTaillePhysique,"frame",args,0);
   XtManageChild(pdFrameTaillePhysique);

   sprintf(dimstr,"%05.2f x %05.2f",largeurPHYS,hauteurPHYS);
   pdInfoTaillePhysique = (Widget)XmCreateLabel(pdFrameTaillePhysique,dimstr,args, 0);
   XtManageChild(pdInfoTaillePhysique);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdRCMemoire = (Widget)XmCreateRowColumn(pdRCcontrole, "rc", args, i);
   XtManageChild(pdRCMemoire);

   pdMemoire = (Widget)XmCreateLabel(pdRCMemoire,labelMemoire[lng],args, 0);
   XtManageChild(pdMemoire);

   pdFrameMemoire = (Widget)XmCreateFrame(pdRCMemoire,"frame",args,0);
   XtManageChild(pdFrameMemoire);

   sprintf(dimstr,"%6d",ROUND(largeurGIF*hauteurGIF/1024.0));
   pdInfoMemoire = (Widget)XmCreateLabel(pdFrameMemoire,dimstr,args, 0);
   XtManageChild(pdInfoMemoire);

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
   XtSetArg(args[i], XmNvalue, "image.gif"); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   XtSetArg(args[i], XmNeditable, True); i++;
   pdNomFichier = (Widget)XmCreateTextField(pdRCNomFichier,"image",args, i);
   XtManageChild(pdNomFichier);
  }

   



/**
 ******************************************************************************
 ******************************************************************************
 **/


ActiverPanneauDiapos()
{
   XEvent pdEVent;
   Widget pdWidgetParent;
   
   Arg args[2];
   int i;
   
   if (!pdTopLevel)
      InitPanneauDiapo();
   
   if (!XtIsRealized(pdTopLevel))
      {
      XtRealizeWidget(pdTopLevel);
      }
   
   f77name(xpandact)();
   
   }

f77name(xpandact)()
{
   LocalEventLoop(pdTopLevel);
   }



DesactiverPanneauDiapo()
{
   int i;

   XtUnrealizeWidget(pdTopLevel);

   }
#else
ActiverPanneauDiapos()
{
}

ExportMovie()
{
}
#endif
