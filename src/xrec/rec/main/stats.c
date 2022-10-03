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
#include <Xm/Text.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

#include <rmn/rpnmacros.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <wgl.h>
#include <xinit.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;
extern GeoMapInfoStruct    mapInfo;


Widget pstatTopLevel = NULL;
Widget pstatForme, pstatFrame, pstatFormestats, pstatFramestats,pstatRc, pstatAfficher, pstatOk;
Widget pstatFormePal, pstatFormeMin, pstatFormeMax;
Widget pstatLabelstats, pstatLabelMin, pstatLabelMax;
Widget pstatPanneaustats, pstatPanneauMin, pstatPanneauMax;
Widget pstatFrameValeurs,pstatRCValeurs,pstatFrameChoix,pstatChoixstats,pstatAuto,pstatFixe;
Widget pstatFrameLimiteX,pstatFormeLimiteX,pstatLabelMinX,pstatTextMinX,pstatLabelMaxX,pstatTextMaxX;

static char *nomPanneauStats[] = {"PanneauStats", "StatsPanel"};
static char *labelOk[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};

char panneaustatsGeometrie[32];

int pstatSelectionTerminee;

XtCallbackProc PStatOk(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   pstatSelectionTerminee = TRUE;
   DesactiverPanneauStats();
   return 0;
   }
 
XtCallbackProc PStatAfficher(Widget w, caddr_t unused1, caddr_t unused2)
{
   RafraichirStats();
   return 0;
   }


void InitPanneauStats()
{

   int i;
   Arg args[16];
   char nomShell[128];

   int lng;
   char labelText[128];


/* Initialize toolkit */
   Xinit("xregarder");
   lng = c_getulng();

   i = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauStats[lng]);
   i = 0;
   if (0 < strlen(panneaustatsGeometrie)) 
      {
      XtSetArg(args[i],XmNgeometry,panneaustatsGeometrie);
      i++;
      }
   
   XtSetArg(args[i], XmNallowShellResize, True); i++;
   pstatTopLevel = XtAppCreateShell(nomShell, nomShell, 
                                 applicationShellWidgetClass,
                                 XtDisplay(SuperWidget.topLevel), args, i);
   
   i = 0;
   XtSetArg(args[i], XmNwidth, 540); i++;
   pstatForme = (Widget) XmCreateForm(pstatTopLevel, "form", args, i);
   XtManageChild(pstatForme);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pstatOk = (Widget)XmCreatePushButton(pstatForme, labelOk[lng], args, i);
   XtAddCallback(pstatOk, XmNactivateCallback, (XtCallbackProc) PStatOk, NULL);
   XtManageChild(pstatOk);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNrightWidget, pstatOk); i++;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   pstatAfficher = (Widget)XmCreatePushButton(pstatForme, labelAfficher[lng], args, i);
   XtAddCallback(pstatAfficher, XmNactivateCallback, (XtCallbackProc) PStatAfficher, NULL);
   XtManageChild(pstatAfficher);

   i=0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, pstatOk); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
/*   XtSetArg(args[i], XmNwidth, 400); i++;*/
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pstatFrame = (Widget) XmCreateFrame(pstatForme, "form", args, i);
   XtManageChild(pstatFrame);

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pstatRc = (Widget)XmCreateRowColumn(pstatFrame, "pstatRc", args, i);
   XtManageChild(pstatRc);

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   sprintf(labelText, "Stats...");
   XtSetArg(args[i], XmNvalue, labelText); i++;
   XtSetArg(args[i], XmNrows, 30); i++;
   XtSetArg(args[i], XmNheight, 200); i++;
   XtSetArg(args[i], XmNcolumns, 80); i++;
   XtSetArg(args[i], XmNeditMode, XmMULTI_LINE_EDIT); i++;
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pstatLabelstats = (Widget)XmCreateScrolledText(pstatRc, labelText, args, i);
   XtManageChild(pstatLabelstats);
   }



void ActiverPanneauStats()
{
   
   if (!pstatTopLevel)
      InitPanneauStats();
   
   if (!XtIsRealized(pstatTopLevel))
      {
      XtRealizeWidget(pstatTopLevel);
      }
   

   f77name(xpanstatact)();
   
   }

void f77name(xpanstatact)()
{
   if (pstatTopLevel)
      {
      if (XtIsRealized(pstatTopLevel))
         {
         RafraichirStats();
         LocalEventLoop(pstatTopLevel);
         }
      }
   }



void DesactiverPanneauStats()
{
  XtUnrealizeWidget(pstatTopLevel);

   }


XtCallbackProc AfficherStats(Widget w, caddr_t client_data, caddr_t call_data)
{
   ActiverPanneauStats();
   AfficherMessageInfoStandard();

   }
   
void RafraichirStats()
   {
   char labelText[131072];
   if (pstatTopLevel)
      {
      if (XtIsRealized(pstatTopLevel))
         {
         strcpy(labelText, "");
         CalculerStats(labelText);
         XmTextSetString(pstatLabelstats, labelText);
         }
      }
  }

void CalculerStats(char texte[])
   {
   int i;
   _Champ *champ;
   
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&champ, i);
      CalculerStats2(texte, i+1, champ, champ->stats_src);
      CalculerStats2(texte, i+1, champ, champ->stats_dst);
      CalculerStats2(texte, i+1, champ, champ->stats_zoom);
      }
   }  

void CalculerStats2(char texte[], int index, _Champ *champ, _Stats stats)
   {
   char tmpStr[512];
   
      
   switch (stats.couverture)
      {
      case SRC_GRID:
      sprintf(tmpStr, "Champ #%02d: NV:%s TV:%s ET:%s\n", index, champ->nomvar, champ->typvar, champ->etiket);
      strcat(texte, tmpStr);
      sprintf(tmpStr, "Niveau:%d Heure:%d IP3:%d\n", champ->ip1, champ->ip2, champ->ip3);
      strcat(texte, tmpStr);
      if (champ->missingFlag && GetValeursManquantesToggle())
        {
        sprintf(tmpStr, "Champ source: (%d x %d) - Npts valides: %d sur %d (%4.2f)\n",champ->src.ni, champ->src.nj,stats.npts_active, stats.npts_total, 100.0*(float)(stats.npts_active)/(float)(stats.npts_total));
        strcat(texte, tmpStr);
        }
      else
        {
        sprintf(tmpStr, "Champ source: (%d x %d)\n", champ->src.ni, champ->src.nj);
        strcat(texte, tmpStr);
        }
      break;
      
      case DST_GRID:
      if (champ->missingFlag && GetValeursManquantesToggle())
        {
        sprintf(tmpStr, "Champ interpole: (%d x %d) - Npts valides: %d sur %d (%4.2f)\n",champ->dst.ni, champ->dst.nj,stats.npts_active, stats.npts_total, 100.0*(float)(stats.npts_active)/(float)(stats.npts_total));
        strcat(texte, tmpStr);
        }
      else
        {
        sprintf(tmpStr, "Champ interpole: (%d x %d)\n", champ->dst.ni, champ->dst.nj);
        strcat(texte, tmpStr);
        }
      break;
      
      case ZOOM_GRID:
      if (champ->missingFlag && GetValeursManquantesToggle())
        {
        sprintf(tmpStr, "Champ zoom: (%d,%d)--(%d,%d) - Npts valides: %d sur %d (%4.2f)\n",champ->stats_zoom.iz1, champ->stats_zoom.jz1, champ->stats_zoom.iz2, champ->stats_zoom.jz2 ,stats.npts_active, stats.npts_total, 100.0*(float)(stats.npts_active)/(float)(stats.npts_total));
        strcat(texte, tmpStr);
        }
      else
        {
        sprintf(tmpStr, "Champ zoom: (%d,%d)--(%d,%d)\n", champ->stats_zoom.iz1, champ->stats_zoom.jz1, champ->stats_zoom.iz2, champ->stats_zoom.jz2); 
        strcat(texte, tmpStr);
        }
      break;
      }
     
     switch(champ->natureTensorielle)
        {
        case SCALAIRE:
          sprintf(tmpStr, "   Min: %g au point(%d, %d)\n", stats.min, stats.imin, stats.jmin);
          strcat(texte, tmpStr);
          sprintf(tmpStr, "   Max: %g au point(%d, %d)\n", stats.max, stats.imax, stats.jmax);
          strcat(texte, tmpStr);
          sprintf(tmpStr, "   Moy.: %g Ecart: %g \n",stats.mean, stats.stddev);
          strcat(texte, tmpStr);
          sprintf(tmpStr, "--------------\n");
          strcat(texte, tmpStr);
        break;
        
        case VECTEUR:
        sprintf(tmpStr, "   UU Min: %g au point(%d, %d)\n", stats.uumin, stats.iuumin, stats.juumin);
        strcat(texte, tmpStr);
        sprintf(tmpStr, "   UU Max: %g au point(%d, %d)\n", stats.uumax, stats.iuumax, stats.juumax);
        strcat(texte, tmpStr);
          sprintf(tmpStr, "   Moy.: %g Ecart: %g \n",stats.uumean, stats.uustddev);
          strcat(texte, tmpStr);
        sprintf(tmpStr, "---\n");
        strcat(texte, tmpStr);   
        sprintf(tmpStr, "   VV Min: %g au point(%d, %d)\n", stats.vvmin, stats.ivvmin, stats.jvvmin);
        strcat(texte, tmpStr);
        sprintf(tmpStr, "   VV Max: %g au point(%d, %d)\n", stats.vvmax, stats.ivvmax, stats.jvvmax);
        strcat(texte, tmpStr);
          sprintf(tmpStr, "   Moy.: %g Ecart: %g \n",stats.vvmean, stats.vvstddev);
          strcat(texte, tmpStr);
        sprintf(tmpStr, "---\n");
        strcat(texte, tmpStr);   
        sprintf(tmpStr, "   UV Min: %g au point(%d, %d)\n", stats.uvmin, stats.iuvmin, stats.juvmin);
        strcat(texte, tmpStr);
        sprintf(tmpStr, "   UV Max: %g au point(%d, %d)\n", stats.uvmax, stats.iuvmax, stats.juvmax);
        strcat(texte, tmpStr);
          sprintf(tmpStr, "   Moy.: %g Ecart: %g \n",stats.uvmean, stats.uvstddev);
          strcat(texte, tmpStr);
        sprintf(tmpStr, "---\n");
        strcat(texte, tmpStr);   
        break;
        }
        
      }
  


void MettreAJourStats()
  {
  MettreAJourSrcStats();
  MettreAJourDstStats();
  MettreAJourZoomStats();
  }
        
void MettreAJourSrcStats()
   {
   int i;
   int un = 1;
   float *uv;
   int npts;

   _Champ *champ;
   
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
    {
    FldMgrGetChamp(&champ, i);
    champ->stats_src.couverture = SRC_GRID;
    if (champ->stats_src.to_be_updated == 1)
      {
      if (champ->natureTensorielle == SCALAIRE)
        {
        if (champ->missingFlag && GetValeursManquantesToggle())
         {
         champ->stats_src.npts_total = champ->src.ni*champ->src.nj;
         sminmax_missing_ij(&(champ->stats_src.min), &(champ->stats_src.max), &(champ->stats_src.imin), &(champ->stats_src.jmin), 
                &(champ->stats_src.imax), &(champ->stats_src.jmax), champ->fld, champ->src.missing, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
         smeanvar_missing(&(champ->stats_src.mean), &(champ->stats_src.stddev),  champ->fld, champ->src.missing, &champ->stats_src.npts_active, &champ->stats_src.npts_total, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
         }
        else
         {
         f77name(sminmax_ij)(&(champ->stats_src.min), &(champ->stats_src.max), &(champ->stats_src.imin), &(champ->stats_src.jmin), 
                &(champ->stats_src.imax), &(champ->stats_src.jmax), champ->fld, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
         f77name(smeanvar)(&(champ->stats_src.mean), &(champ->stats_src.stddev),  champ->fld, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
         }
        }
      else
        {
          npts = champ->src.ni*champ->src.nj;
          uv = (float *)malloc(sizeof(float)*npts);
          f77name(modulus)(uv, champ->uu, champ->vv, &npts);
          if (champ->missingFlag && GetValeursManquantesToggle())
            {
            champ->stats_src.npts_total = champ->src.ni*champ->src.nj;
            sminmax_missing_ij(&(champ->stats_src.uumin), &(champ->stats_src.uumax), &(champ->stats_src.iuumin), &(champ->stats_src.juumin), 
                    &(champ->stats_src.iuumax), &(champ->stats_src.juumax), champ->uu, champ->src.missing, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
            smeanvar_missing(&(champ->stats_src.uumean), &(champ->stats_src.uustddev),  champ->uu, champ->src.missing, &champ->stats_src.npts_active, &champ->stats_src.npts_total, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
            
            sminmax_missing_ij(&(champ->stats_src.vvmin), &(champ->stats_src.vvmax), &(champ->stats_src.ivvmin), &(champ->stats_src.jvvmin), 
                    &(champ->stats_src.ivvmax), &(champ->stats_src.jvvmax), champ->vv, champ->src.missing, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
            smeanvar_missing(&(champ->stats_src.vvmean), &(champ->stats_src.vvstddev),  champ->vv, champ->src.missing, &champ->stats_src.npts_active, &champ->stats_src.npts_total, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
            
            sminmax_missing_ij(&(champ->stats_src.uvmin), &(champ->stats_src.uvmax), &(champ->stats_src.iuvmin), &(champ->stats_src.juvmin), 
                    &(champ->stats_src.iuvmax), &(champ->stats_src.juvmax), uv, champ->src.missing, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
            smeanvar_missing(&(champ->stats_src.uvmean), &(champ->stats_src.uvstddev),  uv, champ->src.missing, &champ->stats_src.npts_active, &champ->stats_src.npts_total, champ->src.ni, champ->src.nj, un, un, champ->src.ni, champ->src.nj);
            }
          else
            {
            f77name(sminmax_ij)(&(champ->stats_src.uumin), &(champ->stats_src.uumax), &(champ->stats_src.iuumin), &(champ->stats_src.juumin), 
              &(champ->stats_src.iuumax), &(champ->stats_src.juumax), champ->uu, &(champ->src.ni), &(champ->src.nj), &un, &un, &champ->src.ni, &champ->src.nj);
            f77name(sminmax_ij)(&(champ->stats_src.vvmin), &(champ->stats_src.vvmax), &(champ->stats_src.ivvmin), &(champ->stats_src.jvvmin), 
              &(champ->stats_src.ivvmax), &(champ->stats_src.jvvmax), champ->vv, &(champ->src.ni), &(champ->src.nj), &un, &un, &champ->src.ni, &champ->src.nj);
            f77name(vsminmax_ij)(&(champ->stats_src.uvmin), &(champ->stats_src.uvmax), &(champ->stats_src.iuvmin), &(champ->stats_src.juvmin), 
              &(champ->stats_src.iuvmax), &(champ->stats_src.juvmax), champ->uu, champ->vv,  &(champ->src.ni), &(champ->src.nj), &un, &un, &champ->src.ni, &champ->src.nj);
            f77name(smeanvar)(&(champ->stats_src.uumean), &(champ->stats_src.uustddev),  champ->uu, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
            f77name(smeanvar)(&(champ->stats_src.vvmean), &(champ->stats_src.vvstddev),  champ->vv, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
            f77name(smeanvar)(&(champ->stats_src.uvmean), &(champ->stats_src.uvstddev),  uv, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
            }
            free(uv);
        }
      champ->stats_src.to_be_updated = 0;
      }
    }
   }
   
void MettreAJourDstStats()
   {
   int i;
   int un = 1;

   _Champ *champ;
   float *uv;
   int npts;
   
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
    {
    FldMgrGetChamp(&champ, i);
    champ->stats_dst.couverture = DST_GRID;
    if (champ->stats_dst.to_be_updated == 1)
      {
      if (champ->natureTensorielle == SCALAIRE)
        {
        if (champ->missingFlag && GetValeursManquantesToggle())
         {
         champ->stats_dst.npts_total = champ->dst.ni*champ->dst.nj;
         sminmax_missing_ij(&(champ->stats_dst.min), &(champ->stats_dst.max), &(champ->stats_dst.imin), &(champ->stats_dst.jmin), 
                &(champ->stats_dst.imax), &(champ->stats_dst.jmax), champ->fld, champ->dst.missing, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
         smeanvar_missing(&(champ->stats_dst.mean), &(champ->stats_dst.stddev),  champ->fld, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
         }
        else
         {
          f77name(sminmax_ij)(&(champ->stats_dst.min), &(champ->stats_dst.max), &(champ->stats_dst.imin), &(champ->stats_dst.jmin), 
                  &(champ->stats_dst.imax), &(champ->stats_dst.jmax), champ->fld, &champ->dst.ni, &champ->dst.nj, &un, &un, &champ->dst.ni, &champ->dst.nj);
          f77name(smeanvar)(&(champ->stats_dst.mean), &(champ->stats_dst.stddev),  champ->fld, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &(champ->dst.ni), &(champ->dst.nj));
         }
        }
      else
        {
          npts = champ->dst.ni*champ->dst.nj;
          uv = (float *)malloc(sizeof(float)*npts);
          if (champ->missingFlag && GetValeursManquantesToggle())
            {
            champ->stats_dst.npts_total = champ->dst.ni*champ->dst.nj;
            sminmax_missing_ij(&(champ->stats_dst.uumin), &(champ->stats_dst.uumax), &(champ->stats_dst.iuumin), &(champ->stats_dst.juumin), 
                    &(champ->stats_dst.iuumax), &(champ->stats_dst.juumax), champ->uu, champ->dst.missing, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
            smeanvar_missing(&(champ->stats_dst.uumean), &(champ->stats_dst.uustddev),  champ->uu, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
            
            sminmax_missing_ij(&(champ->stats_dst.vvmin), &(champ->stats_dst.vvmax), &(champ->stats_dst.ivvmin), &(champ->stats_dst.jvvmin), 
                    &(champ->stats_dst.ivvmax), &(champ->stats_dst.jvvmax), champ->vv, champ->dst.missing, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
            smeanvar_missing(&(champ->stats_dst.vvmean), &(champ->stats_dst.vvstddev),  champ->vv, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
            
            sminmax_missing_ij(&(champ->stats_dst.uvmin), &(champ->stats_dst.uvmax), &(champ->stats_dst.iuvmin), &(champ->stats_dst.juvmin), 
                    &(champ->stats_dst.iuvmax), &(champ->stats_dst.juvmax), uv, champ->dst.missing, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
            smeanvar_missing(&(champ->stats_dst.uvmean), &(champ->stats_dst.uvstddev),  uv, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj, un, un, champ->dst.ni, champ->dst.nj);
            }
          else
            {
          f77name(modulus)(uv, champ->uu, champ->vv, &npts);
          f77name(sminmax_ij)(&(champ->stats_dst.uumin), &(champ->stats_dst.uumax), &(champ->stats_dst.iuumin), &(champ->stats_dst.juumin), 
            &(champ->stats_dst.iuumax), &(champ->stats_dst.juumax), champ->uu, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &champ->dst.ni, &champ->dst.nj);
          f77name(sminmax_ij)(&(champ->stats_dst.vvmin), &(champ->stats_dst.vvmax), &(champ->stats_dst.ivvmin), &(champ->stats_dst.jvvmin), 
            &(champ->stats_dst.ivvmax), &(champ->stats_dst.jvvmax), champ->vv, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &champ->dst.ni, &champ->dst.nj);
          f77name(sminmax_ij)(&(champ->stats_dst.uvmin), &(champ->stats_dst.uvmax), &(champ->stats_dst.iuvmin), &(champ->stats_dst.juvmin), 
            &(champ->stats_dst.iuvmax), &(champ->stats_dst.juvmax), uv, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &champ->dst.ni, &champ->dst.nj);
/*          f77name(vsminmax_ij)(&(champ->stats_dst.uvmin), &(champ->stats_dst.uvmax), &(champ->stats_dst.iuvmin), &(champ->stats_dst.juvmin), 
            &(champ->stats_dst.iuvmax), &(champ->stats_dst.juvmax), champ->uu, champ->vv,  &(champ->dst.ni), &(champ->dst.nj), &un, &un, &champ->dst.ni, &champ->dst.nj);*/
          f77name(smeanvar)(&(champ->stats_dst.uumean), &(champ->stats_dst.uustddev),  champ->uu, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &(champ->dst.ni), &(champ->dst.nj));
          f77name(smeanvar)(&(champ->stats_dst.vvmean), &(champ->stats_dst.vvstddev),  champ->vv, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &(champ->dst.ni), &(champ->dst.nj));
          f77name(smeanvar)(&(champ->stats_dst.uvmean), &(champ->stats_dst.uvstddev),  uv, &(champ->dst.ni), &(champ->dst.nj), &un, &un, &(champ->dst.ni), &(champ->dst.nj));
            free(uv);
             }
        }
      champ->stats_dst.to_be_updated = 0;
      }
    }
   }


void MettreAJourZoomStats(void)
   {
   int i;
   int ix1, iy1, ix2, iy2;
   int un =  1;

   float x0, y0, x1, y1;
   int fenetreAffichage;
   _Champ *champ;
   float *uv;
   int npts;

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   c_wglgvx(&x0, &y0, &x1, &y1);
   c_fxfy2xy(&x0, &y0, x0, y0);
   c_fxfy2xy(&x1, &y1, x1, y1);
   ix1 = (int) x0;
   iy1 = (int) y0;
   ix2 = (int) x1;
   iy2 = (int) y1;

   
  for (i=0; i < FldMgrGetNbChampsActifs(); i++)
    { 
    FldMgrGetChamp(&champ, i);
    champ->stats_zoom.couverture = ZOOM_GRID;
    champ->stats_zoom.iz1 = ix1;
    champ->stats_zoom.jz1 = iy1;
    champ->stats_zoom.iz2 = ix2;
    champ->stats_zoom.jz2 = iy2;
    switch (champ->domaine)
      {
      case VALEURS_PONCTUELLES:
      ix1 = un;
      ix2 = champ->src.ni;
      iy1 = un;
      iy2 = un;
      if (champ->stats_zoom.to_be_updated == 1)
        {
        switch (champ->natureTensorielle)
          {
          case SCALAIRE:
          f77name(sminmax_ij)(&(champ->stats_src.min), &(champ->stats_src.max), &(champ->stats_src.imin), &(champ->stats_src.jmin),
                 &(champ->stats_src.imax), &(champ->stats_src.jmax), champ->fld, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
          f77name(smeanvar)(&(champ->stats_src.mean), &(champ->stats_src.stddev),  champ->fld, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
          break;

          case VECTEUR:
          npts = champ->dst.ni*champ->dst.nj;
          uv = (float *)malloc(sizeof(float)*npts);
          f77name(modulus)(uv, champ->uu, champ->vv, &npts);
          f77name(sminmax_ij)(&(champ->stats_src.uumin), &(champ->stats_src.uumax), &(champ->stats_src.iuumin), &(champ->stats_src.juumin),
            &(champ->stats_src.iuumax), &(champ->stats_src.juumax), champ->uu, &(champ->src.ni), &(champ->src.nj), &un, &un, &champ->src.ni, &champ->src.nj);
          f77name(sminmax_ij)(&(champ->stats_src.vvmin), &(champ->stats_src.vvmax), &(champ->stats_src.ivvmin), &(champ->stats_src.jvvmin),
            &(champ->stats_src.ivvmax), &(champ->stats_src.jvvmax), champ->vv, &(champ->src.ni), &(champ->src.nj), &un, &un, &champ->src.ni, &champ->src.nj);
          f77name(vsminmax_ij)(&(champ->stats_src.uvmin), &(champ->stats_src.uvmax), &(champ->stats_src.iuvmin), &(champ->stats_src.juvmin),
            &(champ->stats_src.iuvmax), &(champ->stats_src.juvmax), champ->uu, champ->vv,  &(champ->src.ni), &(champ->src.nj), &un, &un, &champ->src.ni, &champ->src.nj);
          f77name(smeanvar)(&(champ->stats_src.uumean), &(champ->stats_src.uustddev),  champ->uu, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
          f77name(smeanvar)(&(champ->stats_src.vvmean), &(champ->stats_src.vvstddev),  champ->vv, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
          f77name(smeanvar)(&(champ->stats_src.uvmean), &(champ->stats_src.uvstddev),  uv, &(champ->src.ni), &(champ->src.nj), &un, &un, &(champ->src.ni), &(champ->src.nj));
          free(uv);
          break;
          }
        }
    break;

    default:
        if (champ->stats_zoom.to_be_updated == 1)
          {
          if (champ->natureTensorielle == SCALAIRE)
            {
            if (champ->missingFlag && GetValeursManquantesToggle())
              {
              sminmax_missing_ij(&(champ->stats_zoom.min), &(champ->stats_zoom.max), &(champ->stats_zoom.imin), &(champ->stats_zoom.jmin),
                      &(champ->stats_zoom.imax), &(champ->stats_zoom.jmax), champ->fld, champ->dst.missing, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);
              smeanvar_missing(&(champ->stats_zoom.mean), &(champ->stats_zoom.stddev),  champ->fld, champ->dst.missing, &champ->stats_zoom.npts_active, &champ->stats_zoom.npts_total, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);
              }
            else
              {
              f77name(sminmax_ij)(&(champ->stats_zoom.min), &(champ->stats_zoom.max), &(champ->stats_zoom.imin), &(champ->stats_zoom.jmin),
                &(champ->stats_zoom.imax), &(champ->stats_zoom.jmax), champ->fld, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
            f77name(smeanvar)(&(champ->stats_zoom.mean), &(champ->stats_zoom.stddev),  champ->fld, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
              }
            }
          else
            {
            npts = champ->dst.ni*champ->dst.nj;
            uv = (float *)malloc(sizeof(float)*npts);
            if (champ->missingFlag && GetValeursManquantesToggle())
              {
              champ->stats_zoom.npts_total = champ->dst.ni*champ->dst.nj;
              sminmax_missing_ij(&(champ->stats_zoom.uumin), &(champ->stats_zoom.uumax), &(champ->stats_zoom.iuumin), &(champ->stats_zoom.juumin),
                      &(champ->stats_zoom.iuumax), &(champ->stats_zoom.juumax), champ->uu, champ->dst.missing, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);
              smeanvar_missing(&(champ->stats_zoom.uumean), &(champ->stats_zoom.uustddev),  champ->uu, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);

              sminmax_missing_ij(&(champ->stats_zoom.vvmin), &(champ->stats_zoom.vvmax), &(champ->stats_zoom.ivvmin), &(champ->stats_zoom.jvvmin),
                      &(champ->stats_zoom.ivvmax), &(champ->stats_zoom.jvvmax), champ->vv, champ->dst.missing, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);
              smeanvar_missing(&(champ->stats_zoom.vvmean), &(champ->stats_zoom.vvstddev),  champ->vv, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj,ix1, iy1, ix2, iy2);

              sminmax_missing_ij(&(champ->stats_zoom.uvmin), &(champ->stats_zoom.uvmax), &(champ->stats_zoom.iuvmin), &(champ->stats_zoom.juvmin),
                      &(champ->stats_zoom.iuvmax), &(champ->stats_zoom.juvmax), uv, champ->dst.missing, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);
              smeanvar_missing(&(champ->stats_zoom.uvmean), &(champ->stats_zoom.uvstddev),  uv, champ->dst.missing, &champ->stats_dst.npts_active, &champ->stats_dst.npts_total, champ->dst.ni, champ->dst.nj, ix1, iy1, ix2, iy2);
              }
            else
              {
            f77name(modulus)(uv, champ->uu, champ->vv, &npts);
              f77name(sminmax_ij)(&(champ->stats_zoom.uumin), &(champ->stats_zoom.uumax), &(champ->stats_zoom.iuumin), &(champ->stats_zoom.juumin),
                &(champ->stats_zoom.iuumax), &(champ->stats_zoom.juumax), champ->uu, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
              f77name(sminmax_ij)(&(champ->stats_zoom.vvmin), &(champ->stats_zoom.vvmax), &(champ->stats_zoom.ivvmin), &(champ->stats_zoom.jvvmin),
                &(champ->stats_zoom.ivvmax), &(champ->stats_zoom.jvvmax), champ->vv, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
              f77name(vsminmax_ij)(&(champ->stats_zoom.uvmin), &(champ->stats_zoom.uvmax), &(champ->stats_zoom.iuvmin), &(champ->stats_zoom.juvmin),
                &(champ->stats_zoom.iuvmax), &(champ->stats_zoom.juvmax), champ->uu, champ->vv,  &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
            f77name(smeanvar)(&(champ->stats_zoom.uumean), &(champ->stats_zoom.uustddev),  champ->uu, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
            f77name(smeanvar)(&(champ->stats_zoom.vvmean), &(champ->stats_zoom.vvstddev),  champ->vv, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
            f77name(smeanvar)(&(champ->stats_zoom.uvmean), &(champ->stats_zoom.uvstddev),  uv, &(champ->dst.ni), &(champ->dst.nj), &ix1, &iy1, &ix2, &iy2);
              free(uv);
              }
             }
          champ->stats_zoom.to_be_updated = 0;
          }
        }
    break;
    }
   }

void sminmax_missing_ij(float *rmin, float *rmax, int *imin, int *jmin, int *imax, int *jmax, float *fld, unsigned int *mask,
                         int ni, int nj, int idebut, int jdebut, int ifin, int jfin)
   {
   int i,j,k;
   int i1,i2,j1,j2;
   
   i1 = idebut - 1;
   j1 = jdebut - 1;
   i2 = ifin - 1;
   j2 = jfin - 1;
   
   i1 = i1 > 0 ? i1 : 0;
   i2 = i2 < (ni-1) ? i2 : (ni-1);
   j1 = j1 > 0 ? j1 : 0;
   j2 = j2 < (nj-1) ? j2 : (nj-1);
   
   k = FTN2C(i1, j1, ni);
   if (0 == GETMSK(mask,k))
      {
      k++;
      while ((!GETMSK(mask, k)) && k < (ni*nj-1))
         k++;
      }
      
   *jmin = k/ni; 
   *imin = k-(*jmin*ni);
   
   *imax = *imin;
   *jmax = *jmin;
   
   *rmin = fld[FTN2C(*imin, *jmin, ni)];
   *rmax = *rmin;
   
   for (j=j1; j <= j2; j++)
      {
      for (i=i1; i <= i2; i++)
         {
         k = FTN2C(i,j,ni);
         if (GETMSK(mask, k))
            {
            if (*rmin > fld[k])
               {
               *rmin = fld[k];
               *imin = i;
               *jmin = j;
               }
            if (*rmax < fld[k])
               {
               *rmax = fld[k];
               *imax = i;
               *jmax = j;
               }
            }
         }
      }
   *imin = *imin + 1;
   *imax = *imax + 1;
   *jmin = *jmin + 1;
   *jmax = *jmax + 1;
   }

  
void smeanvar_missing(float *mean, float *stddev, float *fld, unsigned int *mask, int *npts, int *npts_total, 
                         int ni, int nj, int idebut, int jdebut, int ifin, int jfin)
   {
   int i,j,k;
   int i1,i2,j1,j2;
   float somme;
   
   i1 = idebut - 1;
   j1 = jdebut - 1;
   i2 = ifin - 1;
   j2 = jfin - 1;
   
   i1 = i1 > 0 ? i1 : 0;
   i2 = i2 < (ni-1) ? i2 : (ni-1);
   j1 = j1 > 0 ? j1 : 0;
   j2 = j2 < (nj-1) ? j2 : (nj-1);
   
   *npts = 0;
   *npts_total = 0;
   somme = 0.0;
   for (j=j1; j <= j2; j++)
      {
      for (i=i1; i <= i2; i++)
         {
         *npts_total = *npts_total+1;
         k = FTN2C(i,j,ni);
         if (GETMSK(mask, k))
            {
            *npts = *npts + 1;
            somme=somme+fld[k];
            }
         }
      }
   *mean = somme / (float)(*npts);   
   
   *stddev = 0.0;
   for (j=j1; j <= j2; j++)
      {
      for (i=i1; i <= i2; i++)
         {
         k = FTN2C(i,j,ni);
         if (GETMSK(mask, k))
            {
            *stddev= *stddev+(fld[k]-*mean)*(fld[k]-*mean);
            }
         }
      }
      
   *stddev = sqrt(*stddev/((float)*npts));
   
  
   }
 
