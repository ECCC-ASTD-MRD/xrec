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
#include <wgl.h>
#include <rpnmacros.h>
#include <rec.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget poTopLevel = NULL;
Widget poForme, poFrame, poFrameVariation,poFormePalette, poFramePalette,poRc, poAfficher, poOk;
Widget poFormePal, poFormeMin, poFormeMax;
Widget poLabelPalette, poLabelMin, poLabelMax;
Widget poPanneauPalette, poPanneauMin, poPanneauMax;
Widget poOptionsMax, poOptionsPalette, poOptionsMin;
Widget poPanneauVariation,poOptionsVariation,poOptionsVariationItems[7];
Widget poOptionsMaxItems[10], poOptionsPaletteItems[6], poOptionsMinItems[10];


static char *nomPanneauPalette[] = {"PanneauOptions", "OptionsPanel"};
static char *labelTopLevel[] = {"Options", "Options"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};

static char *labelMin[] = {"Min (%)", "Min (%)"};
static char *labelMax[] = {"Max (%)", "Max (%)"};
static char *labelPalette[] = {"Palette", "Palette"};
static char *labelVariation[] = {"Rehaussement", "Enhancement"};

static int currentItem;
static int variation = LINEAIRE;
char panneauPaletteGeometrie[32];
static char *poLabelOptionsPalette[90][13];

static char   *poLabelOptionsVariation[][7] = {{"Cubique", "Quadratique", "Lineaire", "Racine carree", "Racine cubique","Extremes","Moyenne"},
                                        {"Cubic","Quadratic","Linear","Square root","Cubic root","Extrema","Mean"}};

void SetVariationToggle ();

int poSelectionTerminee;

extern _ColormapInfo recCmap;
extern int recColorTable[];
static int npalettes = 0;
extern int sizeRecColorTable;

static XtCallbackProc PoOk(w, unused1, unused2)
static XtCallbackProc PoAfficher(w, unused1, unused2)

static XtCallbackProc PoOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   poSelectionTerminee = TRUE;
   DesactiverPanneauPalette();
   }

static XtCallbackProc PoAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;

   RedessinerFenetres();
   }


InitPanneauOptions()
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
   
   char grtyp[2], typvar[2], nomvar[5], etiket[13],options[9];


/* Initialize toolkit */
   Xinit("xrec");
   lng = c_getulng();

   strcpy(poLabelOptionsPalette[0],"Col.std1");   
   strcpy(poLabelOptionsPalette[1],"Col.std2");
   strcpy(poLabelOptionsPalette[2],"N&B1");
   strcpy(poLabelOptionsPalette[3],"N&B2");
   strcpy(poLabelOptionsPalette[4],"B&B1");
   strcpy(poLabelOptionsPalette[5],"B&B2");
   strcpy(poLabelOptionsPalette[6],"R&B1");
   strcpy(poLabelOptionsPalette[7],"R&B2");
   npalettes = 8;
   
   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauPalette[lng]);
   i = 0;
   if (0 < strlen(panneauPaletteGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneauPaletteGeometrie);
      i++;
      }
   
   poTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                 apolicationShellWidgetClass,
                                 XtDisplay(SuperWidget.topLevel), args, i);
   
   i = 0;
   poForme = (Widget) XmCreateForm(poTopLevel, "form", args, (XtPointer) i);
   XtManageChild(poForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   poOk = (Widget)XmCreatePushButton(poForme, labelOk[lng], args, (XtPointer) i);
   XtAddCallback(poOk, XmNactivateCallback, (XtCallbackProc) PoOk, NULL);
   XtManageChild(poOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, poOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   poAfficher = (Widget)XmCreatePushButton(poForme, labelAfficher[lng], args, (XtPointer) i);
   XtAddCallback(poAfficher, XmNactivateCallback, (XtCallbackProc) PoAfficher, NULL);
   XtManageChild(poAfficher);

   poOptionsVariation = (Widget)XmCreatePulldownMenu(poForme, labelVariation[lng], NULL, 0);

   for (n=0; n < XtNumber(poLabelOptionsVariation[lng]); n++)
	{
	i = 0;
	string = XmStringCreateLtoR(poLabelOptionsVariation[lng][n], XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, string); i++;
	poOptionsVariationItems[n] = (Widget) XmCreatePushButtonGadget(poOptionsVariation, poLabelOptionsVariation[lng][n], args, (XtPointer) i);
	XmStringFree(string);   
	XtAddCallback(poOptionsVariationItems[n], XmNactivateCallback, (XtCallbackProc) SetVariationToggle, (XtPointer) n);
	}

   XtManageChildren(poOptionsVariationItems, XtNumber(poLabelOptionsVariation[lng]));

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, poOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   poFrameVariation = (Widget) XmCreateFrame(poForme, "form", args, (XtPointer) i);
   XtManageChild(poFrameVariation);

   i = 0;
   currentItem = 2;
   string = XmStringCreateLtoR(labelVariation[lng], XmSTRING_DEFAULT_CHARSET); 
   XtSetArg(args[i], XmNlabelString, string); i++;
   XtSetArg(args[i], XmNsubMenuId, poOptionsVariation); i++;
   XtSetArg(args[i], XmNmenuHistory, poOptionsVariationItems[currentItem]); i++;
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   poPanneauVariation = XmCreateOptionMenu(poFrameVariation, "option_menu1", args, (XtPointer) i);
   XmStringFree(string);   

   XtManageChild(poPanneauVariation);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, poFrameVariation); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   poFrame = (Widget) XmCreateFrame(poForme, "form", args, (XtPointer) i);
   XtManageChild(poFrame);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   poRc = (Widget)XmCreateRowColumn(poFrame, "poRc", args, (XtPointer) i);
   XtManageChild(poRc);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   poFormePal = (Widget)XmCreateRowColumn(poRc, labelPalette[lng], args, (XtPointer) i);
   XtManageChild(poFormePal);

   i = 0;
   poLabelPalette = (Widget)XmCreateLabel(poFormePal, labelPalette[lng], args, (XtPointer) i);
   XtManageChild(poLabelPalette);

   table = (XmString *) calloc(npalettes, sizeof(XmString));

   for (j=0; j < npalettes; j++)
      {
      table[j] = (XmString *) XmStringCreate(poLabelOptionsPalette[j], XmSTRING_DEFAULT_CHARSET); 
      }

   i = 0;
   XtSetArg(args[i], XmNitemCount, npalettes); i++;
   XtSetArg(args[i], XmNvisibleItemCount, 8); i++; 
   XtSetArg(args[i], XmNitems, table); i++;
   XtSetArg(args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
   poPanneauPalette = (Widget) XmCreateScrolledList(poFormePal, "option_menu1", args, (XtPointer) i);

   XmListSelectPos(poPanneauPalette, recCmap.noPalette+1, True);
   XtAddCallback(poPanneauPalette, XmNsingleSelectionCallback, (XtCallbackProc) SetPaletteToggle, NULL);
   XtManageChild(poPanneauPalette);

   i = 0;
   XtSetArg(args[i], XmNheight, &height); i++;
   XtGetValues(poPanneauPalette, args, (XtPointer) i);


   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   poFormeMin = (Widget)XmCreateRowColumn(poRc, labelPalette[lng], args, (XtPointer) i);
   XtManageChild(poFormeMin);

   i = 0;
   poLabelMin = (Widget)XmCreateLabel(poFormeMin, labelMin[lng], args, (XtPointer) i);
   XtManageChild(poLabelMin);

   i = 0;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNscaleHeight, height); i++;
   XtSetArg(args[i], XmNvalue, (int)(recCmap.amplificationMin*100.0)); i++;
   poPanneauMin = (Widget) XmCreateScale(poFormeMin, "option_menu1", args, (XtPointer) i);

   XtAddCallback(poPanneauMin, XmNdragCallback, (XtCallbackProc) SetMinToggle, NULL);
   XtAddCallback(poPanneauMin, XmNvalueChangedCallback, (XtCallbackProc) SetMinToggle, NULL);
   XtManageChild(poPanneauMin);

/**
**
**/ 
   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   poFormeMax = (Widget)XmCreateRowColumn(poRc, labelMax[lng], args, (XtPointer) i);
   XtManageChild(poFormeMax);

   i = 0;
   poLabelMax = (Widget)XmCreateLabel(poFormeMax, labelMax[lng], args, (XtPointer) i);
   XtManageChild(poLabelMax);

   i = 0;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNvalue, 100-(int)(recCmap.reductionMax*100.0)); i++;
   XtSetArg(args[i], XmNscaleHeight, height); i++;
   poPanneauMax = (Widget) XmCreateScale(poFormeMax, "option_menu1", args, (XtPointer) i);

   XtAddCallback(poPanneauMax, XmNdragCallback, (XtCallbackProc) SetMaxToggle, NULL);
   XtAddCallback(poPanneauMax, XmNvalueChangedCallback, (XtCallbackProc) SetMaxToggle, NULL);
   XtManageChild(poPanneauMax);
   
   }

ActiverPanneauPalette()
{
   XEvent poEVent;
   Widget poWidgetParent;
   
   Arg args[2];
   int i;
   
   if (!poTopLevel)
      InitPanneauPalette();
   
   if (!XtIsRealized(poTopLevel))
      {
      XtRealizeWidget(poTopLevel);
      CheckColormap(poTopLevel);
      }
   
   f77name(xpanpact)();
   
   }

f77name(xpanpact)()
{
   LocalEventLoop(poTopLevel);
   }



DesactiverPanneauPalette()
{
   int i;

   XtUnrealizeWidget(poTopLevel);

   }



