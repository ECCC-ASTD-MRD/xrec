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
#include <Xm/List.h>
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

#include <rmn/rpnmacros.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl.h>
#include <xinit.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

Widget pmTopLevel = NULL;
Widget pmForme, pmFrame, pmFormeMinMax, pmFrameMinMax,pmRc, pmAfficher, pmOk;
Widget pmFormePal, pmFormeMin, pmFormeMax;
Widget pmLabelMinMax, pmLabelMin, pmLabelMax;
Widget pmPanneauMinMax, pmPanneauMin, pmPanneauMax;
Widget pmFrameValeurs,pmRCValeurs,pmFrameChoix,pmChoixMinMax,pmAuto,pmFixe;
Widget pmFrameLimiteX,pmFormeLimiteX,pmLabelMinX,pmTextMinX,pmLabelMaxX,pmTextMaxX;

static char *nomPanneauMinMax[] = {"PanneauMinMax", "MinMaxPanel"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};

static char *labelMin[] = {"Min", "Min"};
static char *labelMax[] = {"Max", "Max"};
static char *labelMinMax[] = {"Code", "Code"};

static char *labelAuto[] = {"Auto", "Auto"};
static char *labelFixe[] = {"Fixes", "Fixed"};


char *minStr = NULL;
char *maxStr = NULL;

char panneauMinMaxGeometrie[32];
static char pmNomVar[256][5];

int pmSelectionTerminee;

static int prev_nMinMaxs = 0;
static int nMinMaxs = 0;
static int itemDict = 0;


void PmReadMinMax(Widget w, caddr_t client_data, caddr_t call_data) 
{
   if (minStr != NULL)
      {
      XtFree(minStr);
      XtFree(maxStr);
      }
   minStr = (char *) XmTextFieldGetString(pmTextMinX);
   maxStr = (char *) XmTextFieldGetString(pmTextMaxX);
   printf("%s\n%s\n\n",minStr,maxStr);
   }


void PmSelectNomvar(Widget w, caddr_t client_data, caddr_t call_data)
{
   int i;
   char tempStr[16];
   float min,max;
   Arg args[16];
   int varMode;

   XmListCallbackStruct *info = (XmListCallbackStruct *) call_data;

   itemDict  = (int)(info->item_position - 1);

   varMode = DictMgrGetMinMaxMode(pmNomVar[itemDict]);

   i = 0;
   if (varMode == AUTO)
      {
      sprintf(tempStr, "Auto");
      XmTextFieldSetString(pmTextMinX, tempStr);
      XmTextFieldSetString(pmTextMaxX, tempStr);

      i = 0;
      XtSetArg(args[i], XmNset, True); i++;
      XtSetValues(pmAuto, args, i);

      i = 0;
      XtSetArg(args[i], XmNset, False); i++;
      XtSetValues(pmFixe, args, i);
      }
   else
      {
      DictMgrGetMinMaxValues(pmNomVar[itemDict],&min,&max);
      sprintf(tempStr, "%-10.4g",min);
      XmTextFieldSetString(pmTextMinX, tempStr);

      sprintf(tempStr, "%-10.4g",max);
      XmTextFieldSetString(pmTextMaxX, tempStr);

      i = 0;
      XtSetArg(args[i], XmNset, False); i++;
      XtSetValues(pmAuto, args, i);

      i = 0;
      XtSetArg(args[i], XmNset, True); i++;
      XtSetValues(pmFixe, args, i);

      }
      
   }


void PmSetMinMaxAuto(Widget w, caddr_t client_data, caddr_t call_data)
{
   char tempStr[16];

   DictMgrSetMinMaxMode(pmNomVar[itemDict],itemDict,AUTO);
   
   sprintf(tempStr, "Auto");
   XmTextFieldSetString(pmTextMinX, tempStr);
   XmTextFieldSetString(pmTextMaxX, tempStr);
   RedessinerFenetres();
   }

/**
*************
*************
**/

void PmSetMinMaxFixe(Widget w, caddr_t client_data, caddr_t call_data)
{
   char tempStr[16],*returnedStr;
   float min,max;

   returnedStr = (char *) XmTextFieldGetString(pmTextMinX);
   sscanf(returnedStr, "%g", &min);
   XtFree(returnedStr);
   
   returnedStr = (char *) XmTextFieldGetString(pmTextMaxX);
   sscanf(returnedStr, "%g", &max);
   XtFree(returnedStr);

   DictMgrSetMinMaxMode(pmNomVar[itemDict],itemDict,CUSTOM);
   DictMgrSetMinMaxValues(pmNomVar[itemDict],itemDict,min,max);

   sprintf(tempStr, "%-10.4f",min);
   XmTextFieldSetString(pmTextMinX, tempStr);

   sprintf(tempStr, "%-10.4f",max);
   XmTextFieldSetString(pmTextMaxX, tempStr);
   RedessinerFenetres();
   }

/**
*************
*************
**/

XtCallbackProc PmOk(Widget w, caddr_t client_data, caddr_t call_data)
{
   pmSelectionTerminee = TRUE;
   DesactiverPanneauMinMax();
   return 0;
   }

/**
*************
*************
**/

XtCallbackProc PmAfficher(Widget w, caddr_t client_data, caddr_t call_data)
{
   RedessinerFenetres();
   return 0;
   }

/**
*************
*************
**/

void InitPanneauMinMax()
{

   int i,j;
   Arg args[16];
   XmStringTable table;
   char nomShell[128];

   int lng;

/* Initialize toolkit */
   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauMinMax[lng]);
   i = 0;
   if (0 < strlen(panneauMinMaxGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneauMinMaxGeometrie);
      i++;
      }
   
   pmTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                 applicationShellWidgetClass,
                                 XtDisplay(SuperWidget.topLevel), args, i);
   
   i = 0;
   pmForme = (Widget) XmCreateForm(pmTopLevel, "form", args, i);
   XtManageChild(pmForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pmOk = (Widget)XmCreatePushButton(pmForme, labelOk[lng], args, i);
   XtAddCallback(pmOk, XmNactivateCallback, (XtCallbackProc) PmOk, NULL);
   XtManageChild(pmOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pmOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pmAfficher = (Widget)XmCreatePushButton(pmForme, labelAfficher[lng], args, i);
   XtAddCallback(pmAfficher, XmNactivateCallback, (XtCallbackProc) PmAfficher, NULL);
   XtManageChild(pmAfficher);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pmOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   pmFrame = (Widget) XmCreateFrame(pmForme, "form", args, i);
   XtManageChild(pmFrame);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pmRc = (Widget)XmCreateRowColumn(pmFrame, "pmRc", args, i);
   XtManageChild(pmRc);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   pmFormePal = (Widget)XmCreateRowColumn(pmRc, labelMinMax[lng], args, i);
   XtManageChild(pmFormePal);

   i = 0;
   pmLabelMinMax = (Widget)XmCreateLabel(pmFormePal, labelMinMax[lng], args, i);
   XtManageChild(pmLabelMinMax);

   DictMgrGetVariableList(pmNomVar,&nMinMaxs,256);
   prev_nMinMaxs = nMinMaxs;
   
   table = (XmString *) calloc(nMinMaxs, sizeof(XmString));

   for (j=0; j < nMinMaxs; j++)
      {
      table[j] = XmStringCreate(pmNomVar[j], XmSTRING_DEFAULT_CHARSET); 
      }

   i = 0;
   XtSetArg(args[i], XmNitemCount, nMinMaxs); i++;
   XtSetArg(args[i], XmNvisibleItemCount, 9); i++; 
   XtSetArg(args[i], XmNitems, table); i++;
   XtSetArg(args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
   pmPanneauMinMax = (Widget) XmCreateScrolledList(pmFormePal, "option_menu1", args, i);

   XtAddCallback(pmPanneauMinMax, XmNsingleSelectionCallback, (XtCallbackProc) PmSelectNomvar, NULL);
   XtManageChild(pmPanneauMinMax);

   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pmFrame);i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pmOk);i++;

   pmFrameValeurs = (Widget) XmCreateFrame(pmForme, "frame", args, i);
   XtManageChild(pmFrameValeurs);

   pmRCValeurs = (Widget) XmCreateRowColumn(pmFrameValeurs, "rc", NULL, 0);
   XtManageChild(pmRCValeurs);

   pmFrameChoix = (Widget) XmCreateFrame(pmRCValeurs, "frame", NULL, 0);
   XtManageChild(pmFrameChoix);

   pmChoixMinMax = (Widget) XmCreateRadioBox(pmFrameChoix, "radiobox", NULL, 0);
   XtManageChild(pmChoixMinMax);

/**
****
**/

   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNset, True); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pmAuto = (Widget) XmCreateToggleButton(pmChoixMinMax, labelAuto[lng], args, i);
   XtAddCallback(pmAuto, XmNarmCallback, (XtCallbackProc) PmSetMinMaxAuto, NULL);
   XtManageChild(pmAuto);
   
   i = 0;
   XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
   XtSetArg(args[i], XmNmarginHeight, 0); i++;
   XtSetArg(args[i], XmNmarginBottom, 0); i++;
   XtSetArg(args[i], XmNmarginTop, 0); i++;
   pmFixe = (Widget)XmCreateToggleButton(pmChoixMinMax, labelFixe[lng], args, i);
   XtAddCallback(pmFixe, XmNarmCallback, (XtCallbackProc) PmSetMinMaxFixe, NULL);
   XtManageChild(pmFixe);
   
/**
****
**/

   pmFrameLimiteX = (Widget) XmCreateFrame(pmRCValeurs, "Forme", args, i);
   XtManageChild(pmFrameLimiteX);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   pmFormeLimiteX = (Widget) XmCreateRowColumn(pmFrameLimiteX, "radiobox", NULL, 0);
   XtManageChild(pmFormeLimiteX);

   i = 0;
   pmLabelMaxX = (Widget) XmCreateLabel(pmFormeLimiteX, labelMax[lng], args, i);
   XtManageChild(pmLabelMaxX);
      
   i = 0;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, ""); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   pmTextMaxX = (Widget) XmCreateTextField(pmFormeLimiteX, "Max", args, i);
   XtManageChild(pmTextMaxX);

   i = 0;
   pmLabelMinX = (Widget) XmCreateLabel(pmFormeLimiteX, labelMin[lng], args, i);
   XtManageChild(pmLabelMinX);

   i = 0;
   XtSetArg(args[i], XmNcolumns, 10);  i++;
   XtSetArg(args[i], XmNmaxLength, 10);  i++;
   XtSetArg(args[i], XmNvalue, ""); i++;
   XtSetArg(args[i], XmNblinkRate, 0);  i++;
   pmTextMinX = (Widget) XmCreateTextField(pmFormeLimiteX, "Min:", args, i);
   XtManageChild(pmTextMinX);

   }


/**
 ******************************************************************************
 ******************************************************************************
 **/


void ActiverPanneauMinMax()
{
   XmStringTable extended_table;
   int newItems;

   int i;
   
   if (!pmTopLevel)
      InitPanneauMinMax();
   
   if (!XtIsRealized(pmTopLevel))
      {
      XtRealizeWidget(pmTopLevel);
      }
   
   DictMgrGetVariableList(pmNomVar,&nMinMaxs,256);
   if (nMinMaxs != prev_nMinMaxs)
     {
     newItems = nMinMaxs - prev_nMinMaxs;
     extended_table = (XmString *) calloc(newItems, sizeof(XmString));
     for (i=0; i < newItems; i++)
       {
       extended_table[i] = XmStringCreate(pmNomVar[prev_nMinMaxs + i], XmSTRING_DEFAULT_CHARSET); 
       }
     XmListAddItems(pmPanneauMinMax, extended_table, newItems, 0);
     prev_nMinMaxs = nMinMaxs;

     }

   f77name(xpanmact)();
   
   }

void f77name(xpanmact)()
{
   LocalEventLoop(pmTopLevel);
   }



void DesactiverPanneauMinMax()
{
   XtUnrealizeWidget(pmTopLevel);

   }


void f77name(c_sminmaxatr)(char item[],char valeur[], F2Cl flenItem, F2Cl flenValeur) 
{
   int lenItem=flenItem, lenValeur=flenValeur;
   
   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);

/**
   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauMinMaxGeometrie,valeur);
      }
   
   if (0 == strcmp(item,"MinMax"))
      {
      recCmap.noMinMax = atoi(valeur);
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
   ResetColorMap(recColorTable, 256, recCmap.noMinMax);
**/
   }



void EcrMinMaxAtr(FILE *fichierDemarrage)
{
/**
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

   strcpy(tableau, "MinMax");
   
   strcpy(item,"geometrie");
   if (pmTopLevel)
      {
      disp = XtDisplay(pmTopLevel);
      win  = XtWindow(pmTopLevel);
      i = 0;
      XtSetArg(args[i], XmNx, &x); i++;
      XtSetArg(args[i], XmNy, &y); i++;
      XtGetValues(pmTopLevel, args, i);
      
      sprintf(valeur,"%+d%+d",x,y);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
   else
      {
      if (strlen(panneauMinMaxGeometrie) > 0)
         {
         strcpy(valeur,panneauMinMaxGeometrie);
         sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
         fprintf(fichierDemarrage,"%s\n",ligne);
         }
      }


   strcpy(item, "MinMax");
   sprintf(valeur, "%2d", recCmap.noMinMax);
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
**/
   }


