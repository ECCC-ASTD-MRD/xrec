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

#include <xinit.h>
#include <c_wgl.h>
#include <rpnmacros.h>
#include <rec.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;
int externcmap[768];
int externcmaps[90][768];

Widget ppTopLevel = NULL;
Widget ppForme, ppFrame, ppFrameVariation,ppFormePalette, ppFramePalette,ppRc, ppAfficher, ppOk;
Widget ppFormePal, ppFormeMin, ppFormeMax;
Widget ppLabelPalette, ppLabelMin, ppLabelMax;
Widget ppPanneauPalette, ppPanneauMin, ppPanneauMax;
Widget ppOptionsMax, ppOptionsPalette, ppOptionsMin;
Widget ppPanneauVariation,ppOptionsVariation,ppOptionsVariationItems[7];
Widget ppOptionsMaxItems[10], ppOptionsPaletteItems[6], ppOptionsMinItems[10];


static char *nomPanneauPalette[] = {"PanneauPalette", "PalettePanel"};
static char *labelTopLevel[] = {"Geographie", "Geography"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};

static char *labelMin[] = {"Min (%)", "Min (%)"};
static char *labelMax[] = {"Max (%)", "Max (%)"};
static char *labelPalette[] = {"Palette", "Palette"};
static char *labelVariation[] = {"Rehaussement", "Enhancement"};

static int currentItem;
static int variation = LINEAIRE;
char panneauPaletteGeometrie[32];
static char ppLabelOptionsPalette[90][13];

static char   *ppLabelOptionsVariation[][7] = {{"Cubique", "Quadratique", "Lineaire", "Racine carree", "Racine cubique","Extremes","Moyenne"},
                                        {"Cubic","Quadratic","Linear","Square root","Cubic root","Extrema","Mean"}};

void SetVariationToggle ();

int ppSelectionTerminee;

extern _ColormapInfo recCmap;
extern int recColorTable[];
static int npalettes = 0;
extern int sizeRecColorTable;

void SetPaletteToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int i;
   XmListCallbackStruct *info = (XmListCallbackStruct *) call_data;

   recCmap.noPalette = (int)(info->item_position - 1);
   for (i=0;i<768;i++)
      {
      externcmap[i]=externcmaps[info->item_position-1][i];
      }

   ResetColorMap(recColorTable, sizeRecColorTable, recCmap.noPalette);
    
   }

void SetMinToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int i;
   float facteur;
   Arg args[2];

   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   recCmap.amplificationMin = (float)(donnees->value);
   recCmap.amplificationMin /= 100.0;
   ResetColorMap(recColorTable, sizeRecColorTable, recCmap.noPalette);

   }

void SetMaxToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   int i;
   float facteur;
   Arg args[2];

   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;

   recCmap.reductionMax = (float)(100 - donnees->value);
   recCmap.reductionMax /= 100.0;
   ResetColorMap(recColorTable, sizeRecColorTable, recCmap.noPalette);

   }

static XtCallbackProc PpOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   ppSelectionTerminee = TRUE;
   DesactiverPanneauPalette();
   }

static XtCallbackProc PpAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;

   RedessinerFenetres();
   }


InitPanneauPalette()
{

   int i,j;
   Position height;
   Arg args[16];
   XmString string;
   XmStringTable table;
   char nomShell[128];
   char *armnlib;
   char nomFichierPalette[128];

   Colormap cmap;
   int n,lng,key,iun,lrec,ierr;
   int ni, nj, nk;
   int ier, dateo, date,deet, npas, nbits, datyp, ip1, ip2, ip3;
   int ig1,ig2,ig3,ig4,bidon;
   
   char grtyp[2], typvar[3], nomvar[5], etiket[13],options[9];
   
   strcpy(nomvar, "    ");
   strcpy(typvar, "  ");
   strcpy(etiket, "            ");
   strcpy(grtyp, " ");
   
   
   /* Initialize toolkit */
   Xinit("xregarder");
   lng = c_getulng();
   
   strcpy(ppLabelOptionsPalette[0],"Col.std1");   
   strcpy(ppLabelOptionsPalette[1],"Col.std2");
   strcpy(ppLabelOptionsPalette[2],"N&B1");
   strcpy(ppLabelOptionsPalette[3],"N&B2");
   strcpy(ppLabelOptionsPalette[4],"B&B1");
   strcpy(ppLabelOptionsPalette[5],"B&B2");
   strcpy(ppLabelOptionsPalette[6],"R&B1");
   strcpy(ppLabelOptionsPalette[7],"R&B2");
   npalettes = 8;
   
   iun=91;
   lrec = 0;
   strcpy(options, "RND+R/O");
   
   armnlib = (char *) getenv("ARMNLIB");
   strcpy(nomFichierPalette,armnlib);
   strcat(nomFichierPalette,"/data/palettes.fst");
   ierr = c_fnom(iun,nomFichierPalette,"STD+RND+R/O",lrec);
   ierr = c_fstouv(iun,options);
   key = c_fstinf(iun,&ni,&nj,&nk,-1,"        ",-1,-1,-1," ","  ");

   do
      {
      ierr=c_fstluk(externcmaps[npalettes],key,&ni,&nj,&nk);
      ier = c_fstprm(key, &bidon, &bidon, &bidon, &ni, &nj, &nk, &bidon,
                     &bidon, &bidon, &bidon, &bidon, typvar, nomvar, etiket,
                     grtyp, &bidon, &bidon, &bidon, &bidon,&bidon, &bidon, &bidon,
                     &bidon, &bidon, &bidon, &bidon);
      etiket[12] = '\0';
      sprintf(ppLabelOptionsPalette[npalettes],"%s",etiket);
      npalettes++;
      key=c_fstsui(iun,&ni,&nj,&nk);
      } while (key >= 0);
   c_fstfrm(iun);

   
   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauPalette[lng]);
   i = 0;
   if (0 < strlen(panneauPaletteGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneauPaletteGeometrie);
      i++;
      }
   
   ppTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                 applicationShellWidgetClass,
                                 XtDisplay(SuperWidget.topLevel), args, i);
   
   i = 0;
   ppForme = (Widget) XmCreateForm(ppTopLevel, "form", args, i);
   XtManageChild(ppForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   ppOk = (Widget)XmCreatePushButton(ppForme, labelOk[lng], args, i);
   XtAddCallback(ppOk, XmNactivateCallback, (XtCallbackProc)  PpOk, NULL);
   XtManageChild(ppOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, ppOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   ppAfficher = (Widget)XmCreatePushButton(ppForme, labelAfficher[lng], args, i);
   XtAddCallback(ppAfficher, XmNactivateCallback, (XtCallbackProc)  PpAfficher, NULL);
   XtManageChild(ppAfficher);

   ppOptionsVariation = (Widget)XmCreatePulldownMenu(ppForme, labelVariation[lng], NULL, 0);

   for (n=0; n < XtNumber(ppLabelOptionsVariation[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(ppLabelOptionsVariation[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	ppOptionsVariationItems[n] = (Widget) XmCreatePushButtonGadget(ppOptionsVariation, ppLabelOptionsVariation[lng][n], args, i);
	XmStringFree(string);   
	XtAddCallback(ppOptionsVariationItems[n], XmNactivateCallback, (XtCallbackProc)  SetVariationToggle, (XtPointer) n);
	}

   XtManageChildren(ppOptionsVariationItems, XtNumber(ppLabelOptionsVariation[lng]));

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, ppOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   ppFrameVariation = (Widget) XmCreateFrame(ppForme, "form", args, i);
   XtManageChild(ppFrameVariation);

   i = 0;
   currentItem = 2;
   string = XmStringCreateLtoR(labelVariation[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, ppOptionsVariation); i++;
   XtSetArg(args[i], XmNmenuHistory, ppOptionsVariationItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   ppPanneauVariation = (Widget) XmCreateOptionMenu(ppFrameVariation, "option_menu1", args, i);
   XmStringFree(string);   

   XtManageChild(ppPanneauVariation);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, ppFrameVariation); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   ppFrame = (Widget) XmCreateFrame(ppForme, "form", args, i);
   XtManageChild(ppFrame);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   ppRc = (Widget)XmCreateRowColumn(ppFrame, "ppRc", args, i);
   XtManageChild(ppRc);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   ppFormePal = (Widget)XmCreateRowColumn(ppRc, labelPalette[lng], args, i);
   XtManageChild(ppFormePal);

   i = 0;
   ppLabelPalette = (Widget)XmCreateLabel(ppFormePal, labelPalette[lng], args, i);
   XtManageChild(ppLabelPalette);

   table = (XmString *) calloc(npalettes, sizeof(XmString));

   for (j=0; j < npalettes; j++)
      {
      table[j] = XmStringCreate(ppLabelOptionsPalette[j], XmSTRING_DEFAULT_CHARSET); 
      }

   i = 0;
   XtSetArg(args[i], XmNitemCount, npalettes); i++;
   XtSetArg(args[i], XmNvisibleItemCount, 8); i++; 
   XtSetArg(args[i], XmNitems, table); i++;
   XtSetArg(args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
   ppPanneauPalette = (Widget) XmCreateScrolledList(ppFormePal, "option_menu1", args, i);

   XmListSelectPos(ppPanneauPalette, recCmap.noPalette+1, True);
   XtAddCallback(ppPanneauPalette, XmNsingleSelectionCallback, (XtCallbackProc)  SetPaletteToggle, NULL);
   XtManageChild(ppPanneauPalette);

   i = 0;
   XtSetArg(args[i], XmNheight, &height); i++;
   XtGetValues(ppPanneauPalette, args, i);


   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   ppFormeMin = (Widget)XmCreateRowColumn(ppRc, labelPalette[lng], args, i);
   XtManageChild(ppFormeMin);

   i = 0;
   ppLabelMin = (Widget)XmCreateLabel(ppFormeMin, labelMin[lng], args, i);
   XtManageChild(ppLabelMin);

   i = 0;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNscaleHeight, height); i++;
   XtSetArg(args[i], XmNvalue, (int)(recCmap.amplificationMin*100.0)); i++;
   ppPanneauMin = (Widget) XmCreateScale(ppFormeMin, "option_menu1", args, i);

   XtAddCallback(ppPanneauMin, XmNdragCallback, (XtCallbackProc)  SetMinToggle, NULL);
   XtAddCallback(ppPanneauMin, XmNvalueChangedCallback, (XtCallbackProc)  SetMinToggle, NULL);
   XtManageChild(ppPanneauMin);

/**
**
**/ 
   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   ppFormeMax = (Widget)XmCreateRowColumn(ppRc, labelMax[lng], args, i);
   XtManageChild(ppFormeMax);

   i = 0;
   ppLabelMax = (Widget)XmCreateLabel(ppFormeMax, labelMax[lng], args, i);
   XtManageChild(ppLabelMax);

   i = 0;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNvalue, 100-(int)(recCmap.reductionMax*100.0)); i++;
   XtSetArg(args[i], XmNscaleHeight, height); i++;
   ppPanneauMax = (Widget) XmCreateScale(ppFormeMax, "option_menu1", args, i);

   XtAddCallback(ppPanneauMax, XmNdragCallback, (XtCallbackProc)  SetMaxToggle, NULL);
   XtAddCallback(ppPanneauMax, XmNvalueChangedCallback, (XtCallbackProc)  SetMaxToggle, NULL);
   XtManageChild(ppPanneauMax);
   
   }

ActiverPanneauPalette()
{
   XEvent ppEVent;
   Widget ppWidgetParent;
   
   Arg args[2];
   int i;
   
   if (!ppTopLevel)
      InitPanneauPalette();
   
   if (!XtIsRealized(ppTopLevel))
      {
      XtRealizeWidget(ppTopLevel);
      CheckColormap(ppTopLevel);
      }
   
   f77name(xpanpact)();
   
   }

f77name(xpanpact)()
{
   LocalEventLoop(ppTopLevel);
   }



DesactiverPanneauPalette()
{
   int i;

   XtUnrealizeWidget(ppTopLevel);

   }



f77name(c_spalatr)(item,valeur,lenItem,lenValeur)
char item[],valeur[];
int lenItem,lenValeur;  
{
   Arg args[10];
   int i;
   int indItem;
   char fakeWidgetName[32];

   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);

   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauPaletteGeometrie,valeur);
      }
   
   if (0 == strcmp(item,"palette"))
      {
      recCmap.noPalette = atoi(valeur);
      }
   
   if (0 == strcmp(item,"max"))
      {
      recCmap.reductionMax = (float)(100 - atoi(valeur));
      recCmap.reductionMax /= 100.0;
      }
   
   if (0 == strcmp(item,"min"))
      {
      recCmap.amplificationMin = (float)atoi(valeur);
      recCmap.amplificationMin /= 100.0;
      }
   ResetColorMap(recColorTable, sizeRecColorTable, recCmap.noPalette);
   }



EcrPalAtr(fichierDemarrage)
FILE *fichierDemarrage;
{
   char tableau[32];
   char ligne[80];
   char item[32],valeur[32];
   int i;

   Arg  args[10];
   XmString label;
   XmFontList fontListe;
   char *geom;
   Window root;
   Position x,y;
   Display *disp;
   Window win;

   strcpy(tableau, "palette");
   
   strcpy(item,"geometrie");
   if (ppTopLevel)
      {
      disp = XtDisplay(ppTopLevel);
      win  = XtWindow(ppTopLevel);
      i = 0;
      XtSetArg(args[i], XmNx, &x); i++;
      XtSetArg(args[i], XmNy, &y); i++;
      XtGetValues(ppTopLevel, args, i);
      
      sprintf(valeur,"%+d%+d",x,y);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   else
      {
      if (strlen(panneauPaletteGeometrie) > 0)
         {
         strcpy(valeur,panneauPaletteGeometrie);
         sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
         fprintf(fichierDemarrage,"%s\n",ligne);
         }
      }


   strcpy(item, "palette");
   sprintf(valeur, "%2d", recCmap.noPalette);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);

   strcpy(item, "min");
   sprintf(valeur, "%2d", (int)(recCmap.amplificationMin*100.0));
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   
   strcpy(item, "max");
   sprintf(valeur, "%2d", 100-(int)(recCmap.reductionMax*100.0));
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);
   }


int PaletteMgrGetVariation()
{
   return variation;
   }

void SetVariationToggle (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   switch((int)client_data)
      {
      case 0:
      variation = CUBIQUE;
      break;
      
      case 1:
      variation = QUADRATIQUE;
      break;
      
      case 2:
      variation = LINEAIRE;
      break;
      
      case 3:
      variation = RACINE_CARREE;
      break;
      
      case 4:
      variation = RACINE_CUBIQUE;
      break;

      case 5:
      variation = EXTREMUM;
      break;

      case 6:
      variation = MOYENNE;
      break;
      }
   PpAfficher(NULL,NULL,NULL);
   }

