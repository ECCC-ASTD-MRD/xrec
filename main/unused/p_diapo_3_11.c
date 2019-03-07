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

#include <rpnmacros.h>
#include <rpnmacros.h>
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
static char *labelOptionsFormat[] = {"gif", "png", "jpg", "tif"};
static char *labelRecalculer[] = {"Recalculer parametres", "Update parameters"};
static char *labelTaille[] = {"Taille","Size"};
static char *labelTaillePixels[]   = {"Taille (pixels)  ","Size (pixels)   "};
static char *labelTaillePhysique[] = {"Taille (pouces)  ","Size (inches)   "};
static char *labelMemoire[] =        {"Memoire (Kbytes):","Memory (Kbytes):"};
static char *labelNomFichier[] =     {"Nom fichier:     ","File name:      "};
static char *labelHauteurPhysique[] = {"Hauteur (pouces)","Hauteur (pouces)"};
static char *labelAmplification[] = {"Amplification","Amplification"};
static char *labelFormat[]     = {"Format:         ","Format:         "};

char suffixe[5] = ".gif";

static int currentItem;
char panneauDiapoGeometrie[32];
static char pdNomVar[256][3];
static char nomFichierSortie[256];

int pdSelectionTerminee;

static int nDiapos = 0;
static int itemDict = 0;
int largeurFenetre, hauteurFenetre;
int largeurPNG,hauteurPNG;
int resolution= 100;
float largeurPHYS,hauteurPHYS,amplif;


void SetFormatToggle(w, client_data, call_data)
Widget  w;          
caddr_t client_data;
caddr_t call_data;  
{
  int formatGraphique;
  char *nomfichier;
  char nouveauNom[256];
  int lngfichier;

  formatGraphique = (int)client_data;

#define GIF 0
#define PNG 1
#define JPG 2
#define TIF 3

  
  nomfichier = (char *) XmTextFieldGetString(pdNomFichier);
  
  strcpy(nouveauNom, nomfichier);
  lngfichier = strlen(nouveauNom);
  nouveauNom[lngfichier-4] = '\0';
  
  switch (formatGraphique)
    {
    case PNG:
      strcpy(suffixe, ".png");
      break;
      
    case GIF:
      strcpy(suffixe, ".gif");
      break;
      
    case JPG:
      strcpy(suffixe, ".jpg");
      break;
      
    case TIF:
      strcpy(suffixe, ".tif");
      break;
      
    }
  
  strcat(nouveauNom, suffixe);
  XmTextFieldSetString(pdNomFichier, nouveauNom);
};

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
   
   largeurPNG = ROUND(largeurFenetre*taille*.01);
   amplif = (float)largeurPNG/(float)largeurFenetre;
   hauteurPNG = ROUND(amplif*(float)hauteurFenetre);
   largeurPHYS = (float)largeurPNG/(float)resolution;
   hauteurPHYS = (float)hauteurPNG/(float)resolution;
   
   AjusterLabelsTaille();
   }

AjusterLabelsTaille()
{
   XmString xmstr;
   char str[255];
   Arg args[2];
   
   sprintf(str,"%04d x %04d",largeurPNG,hauteurPNG);
   xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0],XmNlabelString,xmstr);
   XtSetValues(pdInfoTaillePixel,args,1);
   XmStringFree(xmstr);
   
   sprintf(str,"%6d",ROUND(largeurPNG*hauteurPNG/1024.0));
   xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0],XmNlabelString,xmstr);
   XtSetValues(pdInfoMemoire,args,1);
   }


void ExportImage()
{
   int i,j,ind;
   int lng;
   int lngfichier;
   
   char *tmpdir, *returnedStr;
   char commande[1024];
   static char *nomFenetre[] = {"xrecAffichage", "xrecDisplay"};


   lng = c_getulng();
   printf("ExportImage...\n");
   returnedStr = (char *) XmTextFieldGetString(pdNomFichier);
   
   strcpy(nomFichierSortie, returnedStr);
   lngfichier = strlen(nomFichierSortie);
   nomFichierSortie[lngfichier-4] = '\0';
   strcat(nomFichierSortie, suffixe);

   RedessinerFenetreAffichage();

   sprintf(commande,"r.import -window %s %s", nomFenetre[lng], nomFichierSortie);
   
   printf("generating image...\n");
   printf("%s\n",commande);
   system(commande);
   printf("generating image...done\n");
   
   RedessinerFenetreAffichage();
   }





void ExportMovie()
{
   int n,lng;
   char commande[1024];
   int fenetreAffichage;
   _Champ *champ;
   static char *nomFenetre[] = {"xrecAffichage", "xrecDisplay"};

   lng = c_getulng();
   
   printf("ExportMovie...\n");
   FldMgrGetChamp(&champ,0);
   FldMgrLoadTimeAnimationSeq();
   RedessinerFenetreAffichage();

   RemettreChampsAJour(0);

   for (n=0; n < champ->seqanim.nbFldsAnim; n++)
     {
     sprintf(commande,"r.import -window %s  %04d.gif", nomFenetre[lng],(XtPointer) n);
     
     printf("generating gif...\n");
     printf("%s\n",commande);
     system(commande);
     printf("generating gif...done\n");
     
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
  DesactiverPanneauDiapo();
   }

static XtCallbackProc PdAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;
   int taille;

   RedessinerFenetres(); 
   c_wglgwz(&largeurFenetre,&hauteurFenetre);
   AjusterLargeur(100);
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
   XtAddCallback(pdOk, XmNactivateCallback, (XtCallbackProc)  PdOk, NULL);
   XtManageChild(pdOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pdOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pdAfficher = (Widget)XmCreatePushButton(pdForme, labelAfficher[lng], args, i);
   XtAddCallback(pdAfficher, XmNactivateCallback, (XtCallbackProc)  PdAfficher, NULL);
   XtManageChild(pdAfficher);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pdAfficher); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pdProduire = (Widget)XmCreatePushButton(pdForme, labelProduire[lng], args, i);
   XtAddCallback(pdProduire, XmNactivateCallback, (XtCallbackProc)  ExportImage, NULL);
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
	pdOptionsFormatItems[n] = (Widget) XmCreatePushButtonGadget(pdOptionsFormat, labelOptionsFormat[n], args, i);
	XmStringFree(string);   
	XtAddCallback(pdOptionsFormatItems[n], XmNactivateCallback, (XtCallbackProc)  SetFormatToggle,(XtPointer) n);
	}

   XtManageChildren(pdOptionsFormatItems, XtNumber(labelOptionsFormat));

   i = 0;
   string = XmStringCreateLtoR(labelFormat[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, pdOptionsFormat); i++;
   XtSetArg(args[i], XmNmenuHistory, pdOptionsFormatItems[0]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   pdPanneauFormat = (Widget) XmCreateOptionMenu(pdRCcontrole, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(pdPanneauFormat);

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   resolution = 100;
   largeurPNG = largeurFenetre;
   hauteurPNG = hauteurFenetre;
   largeurPHYS = (float)(largeurPNG)/(float)resolution;
   hauteurPHYS = (float)(hauteurPNG)/(float)resolution;
   amplif = (float)largeurPNG/(float)largeurFenetre;


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

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   pdRCMemoire = (Widget)XmCreateRowColumn(pdRCcontrole, "rc", args, i);
   XtManageChild(pdRCMemoire);

   pdMemoire = (Widget)XmCreateLabel(pdRCMemoire,labelMemoire[lng],args, 0);
   XtManageChild(pdMemoire);

   pdFrameMemoire = (Widget)XmCreateFrame(pdRCMemoire,"frame",args,0);
   XtManageChild(pdFrameMemoire);

   sprintf(dimstr,"%6d",ROUND(largeurPNG*hauteurPNG/1024.0));
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
