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

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

#include <rec.h>
#include <wgl.h>
#include <xinit.h>
#include <memory.h>
#include <gmp.h>
#include <souris.h>
#include <rpnmacros.h>
#include <ctype.h>

#define C2FTN(i,j,ni)  (int)((ni) * (j) + (i))

#define MODIFIER 0
#define REMETTRE 1

extern SuperWidgetStruct SuperWidget;
extern _XContour    xc;
extern GeoMapInfoStruct  mapInfo;
extern int recColorTable[];

Widget peTopLevel = NULL;
Widget peForme;
Widget peFermer, peAfficher, peFrame;
Widget peRC;
Widget    peRCBoutons;
Widget       peEditerValeurs, 
             peRemettreValeurs,
             peAnnulerToutesModifs, 
             peAnnulerEdition, 
             peRefaireEdition, 
             peEnregistrer,
             peRafraichirFenetre;
Widget    peRCValRemplacement;
Widget        peLabelValRemplacement, 
              peTextValRemplacement;
Widget    peRCEtikRemplacement;
Widget        peLabelEtikRemplacement, 
              peTextEtikRemplacement;
Widget    peSeparateurs[3];
Widget    peInfo, peWarning, peWarningWithCancel;
/*<----------------------------------------------------------------------------------------->*/

typedef struct
{
   int ind;
   float val;
   } PointEditionStruct;

typedef struct 
{
   int type;
   int x1, y1, x2, y2;
   float valeur, ancienneValeur;
   PointEditionStruct *anciennesValeurs;
   } EditionStruct;

EditionStruct *pileEd = NULL; 
int nbCellulesPile = 64;
int nbTotalPile = 0;
int indCourantPile = -1;
_Champ champOriginal;

/*<----------------------------------------------------------------------------------------->*/

static char *nomPanneauEdition[] = {"PanneauEdition", "EditingPanel"};
static char *labelFermer[] = {"Fermer", "Close"};
static char *labelAfficher[] = {"Redessiner", "Redraw"};

static char *EditerValeurs[] = {"Modifier\nvaleurs aux\npoints de grille", "Modify\ngrid point\nvalues"};
static char *RemettreValeurs[] = {"Remettre valeurs\noriginales aux\npoints de grille", "Put back\noriginal grid point\nvalues"};
static char *AnnulerEdition[] = {"Annuler\nedition # 0", "Undo\nEdition # 0"};
static char *RefaireEdition[] = {"Refaire\nedition # 0", "Redo\nEdition # 0"};
static char *AnnulerToutesModifs[] = {"Annuler toutes\nles modifications", "Cancel all\nmodifications"};
static char *Enregistrer[] = {"Enregistrer\nmodifications", "Save\nmodifications"};
static char *NouvelleValeur[] = {"Nouvelle valeur", "New value"};
static char *NouvelleEtiquette[] = {"Nouvelle etiquette", "New stamp"};
static char *EtiquetteDefaut[] = {"EXPERIMENTAL", "EXPERIMENTAL"};

/*<----------------------------------------------------------------------------------------->*/

static XtCallbackProc PeFermer();
static XtCallbackProc PeAfficher();
static XtCallbackProc PeEditerValeurs();
static XtCallbackProc PeAnnulerToutesModifs();
static XtCallbackProc PeAnnulerEdition();
static XtCallbackProc PeRefaireEdition();
static XtCallbackProc PeEnregistrer();
/*<----------------------------------------------------------------------------------------->*/
void InitPanneauEdition()
{
   Arg	args[10];
   XmString label;
   XmStringTable labelTable;
   register int n;	
   char nomShell[128];
   int lng = 0;
   Colormap cmap;
   
   Xinit("xrec");
   lng = c_getulng();
   
   memset(&champOriginal, NULL, sizeof(_Champ));

   n = 0;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, nomPanneauEdition[lng]);
   peTopLevel = XtAppCreateShell(nomShell, nomShell, 
				 applicationShellWidgetClass,
				 XtDisplay(SuperWidget.topLevel), args, n);

   peForme = (Widget) XmCreateForm(peTopLevel, "form", NULL, 0);
   XtManageChild(peForme);

   n = 0;
   XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
   XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
   peFermer = (Widget)XmCreatePushButton(peForme, labelFermer[lng], args, n);
   XtAddCallback(peFermer, XmNactivateCallback, (XtCallbackProc)  PeFermer, NULL);
   XtManageChild(peFermer);

   n = 0;
   XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
   XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
   XtSetArg(args[n], XmNrightWidget, peFermer); n++;
   peAfficher = (Widget)XmCreatePushButton(peForme, labelAfficher[lng], args, n);
   XtAddCallback(peAfficher, XmNactivateCallback, (XtCallbackProc)  PeAfficher, NULL);
   XtManageChild(peAfficher);

   n = 0;
   XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
   XtSetArg(args[n], XmNtopWidget, peFermer); n++;
   XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
   peFrame = (Widget) XmCreateFrame(peForme, "form", args, n);
   XtManageChild(peFrame);

   n = 0;
   XtSetArg (args[n], XmNorientation, XmVERTICAL);  n++;
   XtSetArg (args[n], XmNnumColumns, 1);  n++;
   peRC = (Widget) XmCreateRowColumn(peFrame, "RC", args, n);
   XtManageChild(peRC);
   
   n=0;
   XtSetArg (args[n], XmNorientation, XmVERTICAL);  n++;
   XtSetArg (args[n], XmNpacking, XmPACK_COLUMN);  n++;
   XtSetArg (args[n], XmNnumColumns, 2);  n++;
   peRCBoutons = (Widget) XmCreateRowColumn(peRC, "RCBoutons", args, n);
   XtManageChild(peRCBoutons);
   
   label = XmStringCreateLtoR(EditerValeurs[lng], XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(args[n], XmNlabelString, label); n++;
   XtSetArg (args[n], XmNsensitive, False);  n++;
   peEditerValeurs = (Widget) XmCreatePushButton(peRCBoutons, EditerValeurs[lng], args, n);
   XtAddCallback(peEditerValeurs, XmNactivateCallback, (XtCallbackProc)  PeEditerValeurs, MODIFIER);
   XtManageChild(peEditerValeurs);
   XmStringFree(label);
   
   label = XmStringCreateLtoR(AnnulerEdition[lng], XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(args[n], XmNlabelString, label); n++;
   XtSetArg (args[n], XmNsensitive, False);  n++;
   peAnnulerEdition = (Widget) XmCreatePushButton(peRCBoutons, AnnulerEdition[lng], args, n);
   XtAddCallback(peAnnulerEdition, XmNactivateCallback, (XtCallbackProc)  PeAnnulerEdition, NULL);
   XtManageChild(peAnnulerEdition);
   XmStringFree(label);
   
   label = XmStringCreateLtoR(Enregistrer[lng], XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(args[n], XmNlabelString, label); n++;
   XtSetArg (args[n], XmNsensitive, False);  n++;
   peEnregistrer = (Widget) XmCreatePushButton(peRCBoutons, Enregistrer[lng], args, n);
   XtAddCallback(peEnregistrer, XmNactivateCallback, (XtCallbackProc)  PeEnregistrer, NULL);
   XtManageChild(peEnregistrer);
   XmStringFree(label);
   
   label = XmStringCreateLtoR(RemettreValeurs[lng], XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(args[n], XmNlabelString, label); n++;
   XtSetArg(args[n], XmNsensitive, False); n++;
   peRemettreValeurs = (Widget) XmCreatePushButton(peRCBoutons, RemettreValeurs[lng], args, n);
   XtAddCallback(peRemettreValeurs, XmNactivateCallback, (XtCallbackProc)  PeEditerValeurs, (XtPointer) REMETTRE);
   XtManageChild(peRemettreValeurs);
   XmStringFree(label);
   
   label = XmStringCreateLtoR(RefaireEdition[lng], XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(args[n], XmNlabelString, label); n++;
   XtSetArg (args[n], XmNsensitive, False);  n++;
   peRefaireEdition = (Widget) XmCreatePushButton(peRCBoutons, RefaireEdition[lng], args, n);
   XtAddCallback(peRefaireEdition, XmNactivateCallback, (XtCallbackProc)  PeRefaireEdition, NULL);
   XtManageChild(peRefaireEdition);
   XmStringFree(label);
   
   label = XmStringCreateLtoR(AnnulerToutesModifs[lng], XmSTRING_DEFAULT_CHARSET);
   n = 0;
   XtSetArg(args[n], XmNlabelString, label); n++;
   XtSetArg (args[n], XmNsensitive, False);  n++;
   peAnnulerToutesModifs = (Widget) XmCreatePushButton(peRCBoutons, AnnulerToutesModifs[lng], args, n);
   XtAddCallback(peAnnulerToutesModifs, XmNactivateCallback, (XtCallbackProc)  PeAnnulerToutesModifs, NULL);
   XtManageChild(peAnnulerToutesModifs);
   XmStringFree(label);
   
   n = 0;
   peSeparateurs[0] =  (Widget) XmCreateSeparator(peRC, "sep 1", args, n);
   XtManageChild(peSeparateurs[0]);
   
   labelTable = (XmString *)calloc(1, sizeof(XmString *));
   labelTable[0] = label;
   
   n=0;
   XtSetArg (args[n], XmNorientation, XmVERTICAL);  n++;
   XtSetArg (args[n], XmNnumColumns, 1);  n++;
   peRCValRemplacement = (Widget) XmCreateRowColumn(peRC, "Val", args, n);
   XtManageChild(peRCValRemplacement);
   
   label = XmStringCreateLtoR(NouvelleValeur[lng], XmSTRING_DEFAULT_CHARSET);
   n=0;
   peLabelValRemplacement = (Widget) XmCreateLabel(peRCValRemplacement, NouvelleValeur[lng], args, n);
   XtManageChild(peLabelValRemplacement);
   XmStringFree(label);
   
   n = 0;
   XtSetArg (args[n], XmNvalue, "0.0000");  n++;
   XtSetArg (args[n], XmNblinkRate, 0);  n++;
   peTextValRemplacement = (Widget) XmCreateTextField(peRCValRemplacement, "text", args, n);
   XtManageChild(peTextValRemplacement);
   
   n = 0;
   peSeparateurs[2] =  (Widget) XmCreateSeparator(peRC, "sep 2", args, n);
   XtManageChild(peSeparateurs[2]);
   
   n=0;
   XtSetArg (args[n], XmNorientation, XmVERTICAL);  n++;
   XtSetArg (args[n], XmNnumColumns, 1);  n++;
   peRCEtikRemplacement = (Widget) XmCreateRowColumn(peRC, "Etik", args, n);
   XtManageChild(peRCEtikRemplacement);
   
   label = XmStringCreateLtoR(NouvelleEtiquette[lng], XmSTRING_DEFAULT_CHARSET);
   n=0;
   peLabelEtikRemplacement = (Widget) XmCreateLabel(peRCEtikRemplacement, NouvelleEtiquette[lng], args, n);
   XtManageChild(peLabelEtikRemplacement);
   XmStringFree(label);
   
   n = 0;
   XtSetArg (args[n], XmNmaxLength, 8);  n++;
   XtSetArg (args[n], XmNvalue, EtiquetteDefaut[lng]);  n++;
   XtSetArg (args[n], XmNblinkRate, 0);  n++;
   peTextEtikRemplacement = (Widget) XmCreateTextField(peRCEtikRemplacement, "text", args, n);
   XtManageChild(peTextEtikRemplacement);

   peInfo = (Widget) CreateInfoDialog(peTopLevel);
   peWarning = (Widget) CreateWarningDialog(peTopLevel);
   peWarningWithCancel = (Widget) CreateWarningDialogWithCancelBox(peTopLevel);

   }


/*<----------------------------------------------------------------------------------------->*/
ActiverPanneauEdition()
{
   XEvent peEvent;
   Widget peWidgetParent;
   

   Colormap cmap;
   Arg args[2];
   int i;

   if (!peTopLevel)
      InitPanneauEdition();
   
   if (!XtIsRealized(peTopLevel))
      {
      XtRealizeWidget(peTopLevel);
      CheckColormap(peTopLevel);
      }

   f77name(xpaneact)();
   
   }

/*<----------------------------------------------------------------------------------------->*/
f77name(xpaneact)()
{
   _Champ *champ;

   if (peTopLevel)
      {
      if (!XtIsRealized(peTopLevel))
	 {
	 return;
	 }
      }
   else
      {
      return;
      }
   
   FldMgrGetChamp(&champ, 0);
   if (champ->fld)
      {
      ActiverWidget(peEditerValeurs);
      if (!champ->champModifie)
	 {
	 if (champOriginal.fld)
	    {
	    free(champOriginal.fld);
	    }

	 champOriginal = *champ;
	 champOriginal.fld = (float *) calloc(mapInfo.ni * mapInfo.nj, sizeof(float));
	 memcpy((char *)champOriginal.fld,(char *)champ->fld, mapInfo.ni * mapInfo.nj * sizeof(float));
	 }

      if (!champ->champModifie && nbTotalPile == 0)
	 {
	 DesactiverAuxPeWidgets();
	 }
      else
	 {
	 ActiverAuxPeWidgets();
	 }
      }
   else
      {
      DesactiverWidget(peEditerValeurs);
      DesactiverAuxPeWidgets();
      }
   
   LocalEventLoop(peTopLevel);
   }


/*<----------------------------------------------------------------------------------------->*/
DesactiverPanneauEdition()
{
   XtUnrealizeWidget(peTopLevel);
   }

/*<----------------------------------------------------------------------------------------->*/
static XtCallbackProc PeFermer()
{
   DesactiverPanneauEdition();
   }

/*<----------------------------------------------------------------------------------------->*/
static XtCallbackProc PeAfficher(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   int i;
   
   xc.annulationDemandee = FALSE;
   AfficherBoutonAnnulation();
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      if (!c_wglanul())
	 AfficherCarte(i);
      }
   EnleverBoutonAnnulation();

   for (i=0; i <= indCourantPile; i++)
      {
      AfficherPixelBarre((float)pileEd[i].x1, (float)pileEd[i].y1, (float)pileEd[i].x2, (float)pileEd[i].y2);
      }
   c_wglfsh();
   FlusherTousLesEvenements();

   }

/*<----------------------------------------------------------------------------------------->*/

/** ARGSUSED **/
static XtCallbackProc PeEditerValeurs(w, clientData, callData)
Widget w;
caddr_t clientData, callData;
{
   int lng;
   static char *pasAvecDesChampsVectoriels[] = {"\nCette fonction ne s'applique pas\naux champs vectoriels\n", 
						   "\nThis function does not work\nwith vector fields\n"};
   
   int bouton, event;
   int x0, y0, xsize, ysize, x1, y1, x2, y2;
   int nbMenus;
   char TitreMenu[3][80];
   int temp;
   int i,j,k,n;
   float newX1, newY1, newX2, newY2;
   float ancienneValeur, valeurPloguee, val;
   PointEditionStruct *anciennesValeurs;
   char  *txtValeurPloguee;
   int mode;
   Arg args[1];
   int oldIndCourantPile;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);
   mode = (int)clientData;

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }
   
   if (champ->natureTensorielle == VECTEUR)
      {
      lng = c_getulng();
      DesactiverTousPeWidgets();
      MessageAvertissementAux(pasAvecDesChampsVectoriels[lng], AVERTISSEMENT, peWarning, peTopLevel);
      ActiverTousPeWidgets();
      return;
      }

   oldIndCourantPile = indCourantPile;
   DesactiverTousPeWidgets();

   i = 0;
   XtSetArg(args[i], XmNvalue, &txtValeurPloguee); i++;
   XtGetValues(peTextValRemplacement, args, i);

   sscanf(txtValeurPloguee, "%e", &valeurPloguee);

   InvertWidget(w);

   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   nbMenus = 0;
   
   event = NULL;
   while (event != RESET)
      {
      f77name(souris)(&bouton, &event, 
	      &x0, &y0, &xsize, &ysize, 
	      &x1, &y1, &x2, &y2, TitreMenu, &nbMenus, 80);
      
      if (x2 < x1)
	 {
	 temp = x1;
	 x1 = x2;
	 x2 = temp;
	 }
      
      if (y2 < y1)
	 {
	 temp = y1;
	 y1 = y2;
	 y2 = temp;
	 }
      
      x1 -= x0; x2 -= x0;
      y1 -= y0; y2 -= y0;
      
      c_wgliax(&newX1, &newY1, x1, y1);
      c_wgliax(&newX2, &newY2, x2, y2);
      c_fxfy2xy(&newX1, &newY1, newX1, newY1);
      c_fxfy2xy(&newX2, &newY2, newX2, newY2);
      
      switch(event)
	 {
	 case DRAG:
	 champ->champModifie = True;
	 newX1 = (float)ROUND(newX1+0.5);
	 newY1 = (float)ROUND(newY1+0.5);
	 newX2 = (float)ROUND(newX2-0.5);
	 newY2 = (float)ROUND(newY2-0.5);


	 if (newX1 < 1.0 || newX2 > (float)mapInfo.ni || newY1 < 1.0 || newY2 > (float)mapInfo.nj)
	    {
	    Beeper();
	    }
	 else
	    {
	    anciennesValeurs = (PointEditionStruct *) calloc(((int)(newY2 - newY1)+1)*((int)(newX2 - newX1)+1), 
							     sizeof(PointEditionStruct));
	    n = 0;
	    for (i=(int)newX1; i <= (int)newX2; i++)
	       {
	       for (j=(int)newY1; j <= (int)newY2; j++)
		  {
		  k=C2FTN(i-1,j-1,mapInfo.ni);
		  anciennesValeurs[n].ind = k;
		  anciennesValeurs[n].val = champ->fld[k];
		  if (mode == MODIFIER)
		     val = valeurPloguee;
		  else
		     val = champOriginal.fld[k];
		  
		  champ->fld[k] = val;
		  n++;
		  }
	       }
	    
	    AfficherPixelBarre(newX1, newY1, newX2, newY2);
	    AjouterItemDansLaPile(DRAG, valeurPloguee, ancienneValeur,anciennesValeurs,(int)newX1,(int)newY1,(int)newX2,(int)newY2); 
	    }
	 break;
	 
	 case CLIC:
	 if (newX1 < 1.0 || newX2 > (float)mapInfo.ni || newY1 < 1.0 || newY2 > (float)mapInfo.nj)
	    {
	    Beeper();
	    }
	 else
	    {
	    champ->champModifie = True;
	    newX1 = (float)ROUND(newX1);
	    newY1 = (float)ROUND(newY1);
	    newX2 = (float)ROUND(newX2);
	    newY2 = (float)ROUND(newY2);
	    
	    k=C2FTN(ROUND(newX1)-1,ROUND(newY1)-1,mapInfo.ni);
	    ancienneValeur = champ->fld[k];
	    anciennesValeurs = NULL;
	    newX2 = newX1;
	    newY2 = newY1;
	    if (mode == MODIFIER)
	       val = valeurPloguee;
	    else
	       val = champOriginal.fld[k];
	    champ->fld[k] = val;
	    AfficherPixelBarre(newX1, newY1, newX2, newY2);
	    AjouterItemDansLaPile(CLIC, val, ancienneValeur, anciennesValeurs, (int)newX1, (int)newY1, (int)newX2, (int)newY2);
	    }
	 break;

	 case BACK_CLIC:
	 PeAfficher();
	 break;

	 case NIL:
	 event = RESET;
	 break;

	 default:
	 break;
	 }

      }

   if (champ->champModifie)
      {
      ActiverWidget(peRemettreValeurs);
      ActiverAuxPeWidgets();
      }

   ActiverWidget(peEditerValeurs);
   ActiverTextFields();
   PeActiverBoutons();

   if (champ->champModifie && indCourantPile > oldIndCourantPile)
      {
      PeAfficher();
      }
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   InvertWidget(w);
   }

/*<----------------------------------------------------------------------------------------->*/

static XtCallbackProc PeAnnulerToutesModifs(w, unused1, unused2)
Widget w;
caddr_t unused1, unused2;
{
   static char *annulModif[] = {"Annuler toutes\nles modifications ?", "Cancel all\nmodifications ?"};
   int lng;
   int reponse;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);
   
   lng = c_getulng();
   
   DesactiverTousPeWidgets();
   reponse = MessageAvertissementAux(annulModif[lng], OK_CANCEL, peWarningWithCancel, peTopLevel);
   ActiverTousPeWidgets();

   if (reponse == BTN_OK)
      {
      memcpy((char *)champ->fld,(char *)champOriginal.fld, mapInfo.ni * mapInfo.nj * sizeof(float));
      indCourantPile = -1;
      nbTotalPile = 0;
      MAJLabelsPile(1, 1);
      champ->champModifie = False;
      
      DesactiverAuxPeWidgets();
      PeAfficher(NULL, NULL, NULL);
      }
   }

/*<----------------------------------------------------------------------------------------->*/
static XtCallbackProc PeAnnulerEdition()
{
   int i,j, k, n;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);

   if (indCourantPile < 0)
      return;

   switch(pileEd[indCourantPile].type)
      {
      case DRAG:
      n = 0;
      for (i=pileEd[indCourantPile].x1; i <= pileEd[indCourantPile].x2; i++)
	 {
	 for (j=pileEd[indCourantPile].y1; j <= pileEd[indCourantPile].y2; j++)
	    {
	    k = pileEd[indCourantPile].anciennesValeurs[n].ind;
	    champ->fld[k] = pileEd[indCourantPile].anciennesValeurs[n].val;
	    n++;
	    }
	 }
      break;
      
      case CLIC:
      case BACK_CLIC:
      k = C2FTN(pileEd[indCourantPile].x1-1, pileEd[indCourantPile].y1-1, mapInfo.ni);
      champ->fld[k] = pileEd[indCourantPile].ancienneValeur;
      break;
      
      }
   
   indCourantPile--;
   if (indCourantPile < 0)
      {
      MAJLabelsPile(1, 1);
      DesactiverWidget(peRemettreValeurs);
      DesactiverWidget(peAnnulerToutesModifs);
      DesactiverWidget(peAnnulerEdition);
      DesactiverWidget(peEnregistrer);
      champ->champModifie = False;
      }
   else
      {
      MAJLabelsPile(indCourantPile+1, indCourantPile+2);
      }

   ActiverWidget(peRefaireEdition);
   PeAfficher(NULL, NULL, NULL);
   }
   
/*<----------------------------------------------------------------------------------------->*/
static XtCallbackProc PeRefaireEdition()
{
   int i,j, k, n;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);

   indCourantPile++;
   champ->champModifie = True;

   ActiverAuxPeWidgets();
   if (indCourantPile == (nbTotalPile-1))
      {
      MAJLabelsPile(indCourantPile+1, indCourantPile+1);
      DesactiverWidget(peRefaireEdition);
      }
   else
      {
      MAJLabelsPile(indCourantPile+1, indCourantPile+2);
      }
   
   switch(pileEd[indCourantPile].type)
      {
      case DRAG:
      for (i=pileEd[indCourantPile].x1; i <= pileEd[indCourantPile].x2; i++)
	 {
	 for (j=pileEd[indCourantPile].y1; j <= pileEd[indCourantPile].y2; j++)
	    {
	    k = C2FTN(i-1, j-1, mapInfo.ni);
	    champ->fld[k] = pileEd[indCourantPile].valeur;
	    }
	 }
      break;
      
      case CLIC:
      case BACK_CLIC:
      k = C2FTN(pileEd[indCourantPile].x1-1, pileEd[indCourantPile].y1-1, mapInfo.ni);
      champ->fld[k] = pileEd[indCourantPile].valeur;
      break;
      
      }
   
   PeAfficher(NULL, NULL, NULL);

   }

/*<----------------------------------------------------------------------------------------->*/
static XtCallbackProc PeEnregistrer(w, clientData, callData)
Widget w;
caddr_t clientData, callData;
{
   char nomFich[1024];
   char *nouvelleEtiquette;
   char *typFich = "STD+RND";
   int lng;
   static char *nombidon[] = {"champ_edite", "modified_field"};
   static char *succes[] = {"\nOperation completee.\n Le champ a ete ecrit dans le fichier:\n", 
			       "\nOperation successfull.\n The field has been saved in the file:\n"};
   static char *problemeFnom[] = {"\nProbleme avec l'ouverture du fichier\nOperation annulee\n", 
				     "\nCan't open file.\nOperation cancelled\n"};

   int iunSortie = 132;
   char succesStr[256];
   Arg args[2];
   int i;
   float *work;
   float tmp;
   int ni, nj, nk;
   int ier, key, dateo, deet, npas, nbits, datyp, ip1, ip2, ip3;
   int ig1,ig2,ig3,ig4,swa,fstlng,dltf,ubc,extra1,extra2,extra3;
   char grtyp[2], typvar[3], nomvar[5], etiket[13];
   float *tmpfld;
   int   *itmpfld;
   unsigned int *iunstmpfld;

   unsigned int tmpunsfld;
   int tmpintfld;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);
   
   lng = c_getulng();
   f77name(rtnozsrt)(nomFich, 1024);

   i = 0;
   while (!(isspace((int)nomFich[i])))
      i++;
   nomFich[i] = '\0';
   for (i=0; i < strlen(nomFich); i++)
      nomFich[i] = tolower((int)nomFich[i]);


   if (0 == strcmp(nomFich, nombidon[lng]))
      {
      strcpy(nomFich, (char *) getenv("HOME"));
      strcat(nomFich, "/");
      strcat(nomFich, nombidon[lng]);
      }

   ier  = c_fnom(iunSortie, nomFich, typFich, 0);
   if (ier != 0)
      {
      DesactiverTousPeWidgets();
      MessageAvertissementAux(problemeFnom[lng], AVERTISSEMENT, peWarning, peTopLevel);
      ActiverTousPeWidgets();
      return;
      }
   
   i = 0;
   XtSetArg(args[i], XmNvalue, &nouvelleEtiquette); i++;
   XtGetValues(peTextEtikRemplacement, args, i);

   for (i=strlen(nouvelleEtiquette); i < 8; i++)
      {
      nouvelleEtiquette[i] = ' ';
      }
	
   ier = c_fstouv(iunSortie, "RND+R/W");
   if (ier < 0)
      {
      DesactiverTousPeWidgets();
      MessageAvertissementAux(problemeFnom[lng], AVERTISSEMENT, peWarning, peTopLevel);
      ActiverTousPeWidgets();
      return;
      }

   ier = c_fstprm(champ->cle, &dateo, &deet, &npas, &ni, &nj, &nk, &nbits,
		  &datyp, &ip1, &ip2, &ip3, typvar, nomvar, etiket,
		  grtyp, &ig1, &ig2, &ig3, &ig4, &swa, &fstlng, &dltf,
		  &ubc, &extra1, &extra2, &extra3);
       
   if ((grtyp[0] == 'G' || grtyp[0] == 'A' || grtyp[0] == 'B') && ig2 == 1)
      f77name(permut)(champ->fld, &ni, &nj);
   
   if (datyp == 2)
      {
      for (tmpfld = champ->fld; tmpfld < (champ->fld + ni * nj); tmpfld++)
	 {
	 tmpintfld = (int) *tmpfld;
	 memcpy(tmpfld, &tmpintfld, sizeof(float));
	 }
      }
   
   if (datyp == 4)
      {
      for (tmpfld = champ->fld; tmpfld < (champ->fld + ni * nj); tmpfld++)
	 {
	 tmpunsfld = (unsigned int) *tmpfld;
	 memcpy(tmpfld, &tmpunsfld, sizeof(float));
	 }
      }

   ier = c_fstecr(champ->fld, work, -nbits, iunSortie, 
		  champ->date, deet, npas, mapInfo.ni, mapInfo.nj, 1,
		  ip1, ip2, ip3, champ->typvar, 
		  champ->nomvar, nouvelleEtiquette, &mapInfo.type,
		  mapInfo.ig1, mapInfo.ig2, mapInfo.ig3, mapInfo.ig4, datyp, 1);

   ier = c_fstfrm(iunSortie);
   ier = c_fclos(iunSortie);

   strcpy(succesStr, succes[lng]);
   strcat(succesStr, nomFich);
   strcat(succesStr, "\n");

   DesactiverTousPeWidgets();
   MessageAvertissementAux(succesStr, INFO, peInfo, peTopLevel);
   ActiverTousPeWidgets();
   
   if ((grtyp[0] == 'G' || grtyp[0] == 'A' || grtyp[0] == 'B') && ig2 == 1)
      f77name(permut)(champ->fld, &ni, &nj);
   
   if (datyp == 2)
      {
      for (itmpfld = (int *)champ->fld; itmpfld < (int *)(champ->fld + ni * nj); itmpfld++)
	 {
	 tmp = (float) *itmpfld;
	 memcpy(itmpfld, &tmp, sizeof(int));
	 }
      }
   
   if (datyp == 4)
      {
      for (iunstmpfld = (unsigned int *)champ->fld; iunstmpfld < (unsigned int *)(champ->fld + ni * nj); iunstmpfld++)
	 {
	 tmp = (float) *iunstmpfld;
	 memcpy(iunstmpfld, &tmp, sizeof(unsigned int));
	 }
      }

   indCourantPile = -1;
   nbTotalPile = 0;
   MAJLabelsPile(nbTotalPile, nbTotalPile);
   DesactiverAuxPeWidgets();
   champ->champModifie = False;
   }

/*<----------------------------------------------------------------------------------------->*/

AfficherPixelBarre(rx1, ry1, rx2, ry2)
float rx1,ry1,rx2,ry2;
{

   float x1, y1, x2, y2;
   
   x1 = rx1; x2 = rx2; 
   y1 = ry1; y2 = ry2;

   c_wglcol(NOIR);
   c_wgllwi(1);

   x1 -=  0.5;
   y1 -=  0.5;
   x2 +=  0.5;
   y2 +=  0.5;

   c_xy2fxfy(&x1, &y1, x1, y1);
   c_xy2fxfy(&x2, &y2, x2, y2);

   c_wglrlx(x1,y1,x2,y2);

   c_wglmvx(x1,y1);
   c_wgldrx(x2,y2);

   c_wglmvx(x1,y2);
   c_wgldrx(x2,y1);
   }

/*<----------------------------------------------------------------------------------------->*/

ActiverAuxPeWidgets()
{
   if (indCourantPile >= 0)
      {
      ActiverWidget(peAnnulerToutesModifs);
      ActiverWidget(peAnnulerEdition);
      ActiverWidget(peEnregistrer);
      ActiverWidget(peRemettreValeurs);
      
      if (indCourantPile == (nbTotalPile - 1))
	 {
	 DesactiverWidget(peRefaireEdition);
	 }
      else
	 {
	 ActiverWidget(peRefaireEdition);
	 }
      }
   
   }

/*<----------------------------------------------------------------------------------------->*/
   
DesactiverAuxPeWidgets()
{
   int i;
   Arg args[2];

   DesactiverWidget(peAnnulerToutesModifs);
   DesactiverWidget(peAnnulerEdition);
   DesactiverWidget(peRefaireEdition);
   DesactiverWidget(peRemettreValeurs);
   DesactiverWidget(peEnregistrer);
   }

ActiverTextFields()
{
   int i;
   Arg args[2];
   
   i = 0;
   XtSetArg(args[i], XmNeditable, True); i++;
   XtSetArg(args[i], XmNsensitive, True); i++;

   XtSetValues(peTextValRemplacement, args, i);
   XtSetValues(peTextEtikRemplacement, args, i);
   }

DesactiverTextFields()
{
   int i;
   Arg args[2];

   i = 0;
   XtSetArg(args[i], XmNeditable, False); i++;
   XtSetArg(args[i], XmNsensitive, False); i++;

   XtSetValues(peTextValRemplacement, args, i);
   XtSetValues(peTextEtikRemplacement, args, i);
   }

   
/*<----------------------------------------------------------------------------------------->*/

MAJLabelsPile(noAnnul, noRefaire)
int noAnnul, noRefaire;
{
   int lng;
   static char *AnnulerEditionTmp[] = {"Annuler\nedition # ", "Undo\nEdition # "};
   static char *RefaireEditionTmp[] = {"Refaire\nedition # ", "Redo\nEdition # "};
   char tmp[128];
   XmString label;
   
   int i;
   Arg args[2];

   lng = c_getulng();

   sprintf(tmp, "%s %2d", AnnulerEditionTmp[lng], noAnnul);
   label = XmStringCreateLtoR(tmp, XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetValues(peAnnulerEdition, args, i);
   XmStringFree(label);

   sprintf(tmp, "%s %2d", RefaireEditionTmp[lng], noRefaire);
   label = XmStringCreateLtoR(tmp, XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetValues(peRefaireEdition, args, i);
   XmStringFree(label);

   }

/*<----------------------------------------------------------------------------------------->*/

AjouterItemDansLaPile(type, valeur, ancienneValeur, anciennesValeurs, x1, y1, x2, y2)
int type;
float valeur, ancienneValeur;
PointEditionStruct *anciennesValeurs;
int x1, y1, x2, y2;
{
   int i, n;
   Arg args[2];

   if (!pileEd)
      {
      pileEd = (EditionStruct *) calloc(nbCellulesPile, sizeof(EditionStruct));
      indCourantPile = -1;
      nbTotalPile = 0;
      }
   
   indCourantPile++;
   MAJLabelsPile(indCourantPile+1, indCourantPile+1);
   nbTotalPile = indCourantPile+1;
   
   if (nbTotalPile == nbCellulesPile)
      {
      nbCellulesPile+= 64;
      pileEd = (EditionStruct *)realloc(pileEd, nbCellulesPile * sizeof(EditionStruct));
      for (n=indCourantPile; n < nbCellulesPile; n++)
	 {
	 memset(&pileEd[n], NULL, sizeof(EditionStruct));
	 }
      }
   
   if (pileEd[indCourantPile].anciennesValeurs)
      free(pileEd[indCourantPile].anciennesValeurs);
   
   pileEd[indCourantPile].type = type;
   pileEd[indCourantPile].valeur = valeur;
   pileEd[indCourantPile].ancienneValeur = ancienneValeur;
   pileEd[indCourantPile].anciennesValeurs = anciennesValeurs;
   pileEd[indCourantPile].x1 = x1;
   pileEd[indCourantPile].x2 = x2;
   pileEd[indCourantPile].y1 = y1;
   pileEd[indCourantPile].y2 = y2;
   
   
   }


PeDesactiverBoutons()
{
   DesactiverWidget(peFermer);
   DesactiverWidget(peAfficher);
   }

PeActiverBoutons()
{
   ActiverWidget(peFermer);
   ActiverWidget(peAfficher);
   }

DesactiverTousPeWidgets()
{
      DesactiverAuxPeWidgets();
      DesactiverWidget(peEditerValeurs);
      PeDesactiverBoutons();
      DesactiverTextFields();
   }

ActiverTousPeWidgets()
{
      ActiverAuxPeWidgets();
      ActiverWidget(peEditerValeurs);
      PeActiverBoutons();
      ActiverTextFields();
   }
