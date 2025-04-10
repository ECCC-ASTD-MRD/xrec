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
#include <Xm/CascadeBG.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>



#include <xinit.h>
#include <wgl.h>
#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>

extern SuperWidgetStruct SuperWidget;
extern _XContour xc;

int facteurDensite;
int echelleWW = 100;

Widget pventTopLevel = NULL;
Widget pventRootRC;
Widget pventForme, pventFormeMenus, pventFrameVents, pventRc, pventAfficher, pventOk;
Widget pventPanneauDensite,pventOptionsDensite,pventOptionsDensiteItems[11];
Widget pventPanneauLongueur,pventOptionsLongueur,pventOptionsLongueurItems[8];
Widget pventPanneauEpaisseurF,pventOptionsEpaisseurF,pventOptionsEpaisseurFItems[8];
Widget pventPanneauCroissance,pventOptionsCroissance,pventOptionsCroissanceItems[5];
Widget pventPanneauEchelleWW,pventOptionsEchelleWW,pventOptionsEchelleWWItems[11];
Widget pventPanneauEchelleFleches,pventOptionsEchelleFleches,pventOptionsEchelleFlechesItems[11];
Widget pventFormeToggles,pventFormeToggles2,pventFrameToggles;
Widget pventFormeBarbules,pventLabelBarbules,pventFrameBarbules;
Widget pventSelectionBarbules,pventAucun,pventBarbules,pventFleches;
Widget pventFormeModule,pventLabelModule,pventFrameModule;
Widget pventSelectionModule,pventModuleOn,pventModuleOff;
Widget pventFormeLIC,pventLabelLIC,pventFrameLIC;
Widget pventSelectionLIC,pventLICOn,pventLICOff;

Widget pventSeparateur1;
Widget pventPanneauXStream;
Widget pventFormeXStream,pventLabelXStream,pventFrameXStream, pventXStreamRC, pventXStreamScalesRC;
Widget pventXStreamBoutonsRc, pventBoutonXStreamStart, pventBoutonXStreamStop;
Widget pventScaleNpts, pventScaleSegstep, pventScaleSeglen, pventScaleDeet, pventScaleNpas, pventScaleDelai;

static char *nomPanneauVents[] = {"PanneauVents", "VectorPanel"};
static char *labelTopLevel[]   = {"Champs vectoriels", "Vector fields"};
static char *labelOk[]         = {"Fermer", "Close"};
static char *labelAfficher[]   = {"Redessiner", "Refresh"};

static char *labelDensite[]    = {"Increment  ", "Increment"};
static char *labelLongueur[]   = {"Longueur   ", "Length "};
static char *labelEpaisseurF[]   = {"Epaisseur \ndes fleches", "Arrow\nThickness"};
static char *labelCroissance[] = {  "Croissance\ndes fleches", "Arrow\nGrowth"};
static char *labelEchelleWW[]  = {"Amplif.   \nWW",          "Amplif.  \nWW"};
static char *labelEchelleFleches[]  = {"Legende   \nFleches",          "Arrows  \nLegend"};
static char *labelAucun[]      = {"Aucun"               , "None"};
static char *labelFleches[]    = {"Fleches"               , "Arrows"};
static char *labelBarbules[]   = {"Barbules"              , "Wind Barbs"};
static char *labelSelectionBarbules[]  = {"Mode d'affichage","Display Mode"};
static char *labelModule[]             = {"Module du vent  ","Wind Modulus"};
static char *labelLIC[]             = {"Int. de convol.","Line Integral Conv."};

static char *labelOui[]         = {"Oui", "Yes"};
static char *labelNon[]        = {"Non", "No"};

static char *labelXStream[]    = {"Lignes de courant", "Streamlines"};


static char
*pventLabelOptionsDensite[][11] = {{"Auto         ","20","10","5","2","1","0.50","0.25","0.20","0.10","0.05"},
           {"Auto         ","20","10","5","2","1","0.50","0.25","0.20","0.10","0.05"}};

static char
*pventLabelOptionsLongueur[][8] = {{"8", "16", "24", "32           ", "48","64","96","128"},
           {"8", "16", "24", "32           ", "48","64","96","128"}};

static char
*pventLabelOptionsEpaisseurF[][6] = {{"1", "2", "3", "4           ", "5","6"},
             {"1", "2", "3", "4           ", "5","6"}};

static char
*pventLabelOptionsEchelleWW[][11] = {{"1", "5", "10", "25           ", "50","100","150","200","400","500","1000"},
             {"1", "5", "10", "25           ", "50","100","150","200","400","500","1000"}};

static char *pventLabelOptionsEchelleFleches[][2] = {{"Oui", "Non           "},{ "Yes", "No           "}};

static char
*pventLabelOptionsCroissance[][5] = {{"Cubique", "Quadratique", "Lineaire", "Racine carree", "Racine cubique"},
             {"Cubic","Quadratic","Linear","Square root","Cubic root"}};

#define LINEAIRE        1
#define RACINE_CARREE   -2
#define RACINE_CUBIQUE  -3
#define CARRE           2
#define CUBIQUE         3

#define BARBULE        0
#define FLECHES         1
#define NONE           -1
#define NO_MODULUS       0
#define MODULUS          1

#define NO_LIC       0
#define LIC          1

float densite = -1.0;
int croissance = 1;
int longueur = 32;
int epaisseurF = 1;
int displayMode = FLECHES;
int flagModule  = 0;
int flagLIC     = 0;
int flagLegendeFleches = 1;
/*------------------------------*/
int npts = 2000;
int segstep = 4;
int nnpas = 64;
int seglen = 3;
int deet = 1800;
float delai = 0.1;
/*------------------------------*/

static char *labelNPTS[]  = {"Nombre de points","Number of points"};
static char *labelNPAS[]  = {"Nombre de pas de temps","Number of time steps"};
static char *labelSEGSTEP[]  = {"Increment de segment","Segment increment"};
static char *labelSEGLEN[]  = {"Longueur de segment","Segment length"};
static char *labelDEET[]  = {"Pas de temps (sec.)","Time step (sec.)"};
static char *labelDELAI[]  = {"Delai entre les images (sec.)","Delay between frames (sec.)"};

static int currentItem;
char panneauVentsGeometrie[32];

int pventSelectionTerminee;

void SetModuleOn (w, client_data, call_data)
     Widget w;    /*  widget id   */
     XtPointer  client_data;  /*  data from application   */
     XtPointer  call_data;  /*  data from widget class  */
{
  flagModule = MODULUS;
  flagLIC = NO_LIC;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

/** ARGSUSED **/
void PVentSetNPTS(Widget w, XtPointer client_data, XtPointer call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   npts = donnees->value;
   return ;
   }


/** ARGSUSED **/
void PVentSetNPAS(Widget w, XtPointer client_data, XtPointer call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   nnpas = donnees->value;
   return ;
   }

/** ARGSUSED **/
void PVentSetDEET(Widget w, XtPointer client_data, XtPointer call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   deet = donnees->value;
   return ;
   }

/** ARGSUSED **/
void PVentSetSEGLEN(Widget w, XtPointer client_data, XtPointer call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   seglen = donnees->value;
   return ;
   }

/** ARGSUSED **/
void PVentSetSEGSTEP(Widget w, XtPointer client_data, XtPointer call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   segstep = donnees->value;
   return ;
   }

/** ARGSUSED **/
void PVentSetDELAI(Widget w, XtPointer client_data, XtPointer call_data)
{
   XmScaleCallbackStruct *donnees = (XmScaleCallbackStruct *) call_data;
   
   delai = 0.01 * (float)(donnees->value);
   return ;
   }

/** ARGSUSED **/
void PVentStop(Widget w, XtPointer unused1, XtPointer unused2)
{
   xc.annulationDemandee = TRUE;
   return ;
   }

void c_animvent(float *uu,float *vv,float *mapscl,int ni,int nj, int seglen, float *uut,float *vvt,int npts,int nnpas,int deet,int segstep,float delai);

/** ARGSUSED **/
void PVentStart(Widget w, XtPointer unused1, XtPointer unused2)
{
  _Champ *champ;
  float *mapscl;
  int ni, nj, found;
  float *uut, *vvt, *uu, *vv;
  int ig1, ig2, ig3, ig4,i;
  char grtyp[2];
  float pi,pj,d60,dgrw;
  int nbChampsActifs;
  static char *mauvaiseProjection[]  = {"SVP selectionner une projection PS Nord ou Sud","PLS select a North or South PS Projection"};

  xc.annulationDemandee = FALSE;
  nbChampsActifs = FldMgrGetNbChampsActifs();
  i = 0;
  found = 0;
  while (found == 0 && i < nbChampsActifs)
    {
    FldMgrGetChamp(&champ, i);
    if (champ->natureTensorielle == VECTEUR)
      {
      found = 1;
      }
    else
      {
      i++;
      }
    }

  if (found == 0) return ;
  grtyp[0] = champ->dst.grtyp[0];
  if (champ->dst.grtyp[0] != 'N' && champ->dst.grtyp[0] != 'S')
    {
    MessageAvertissement(mauvaiseProjection[c_getulng()], 0);
    return ;
    }
  ig1 = champ->dst.ig1;
  ig2 = champ->dst.ig2;
  ig3 = champ->dst.ig3;
  ig4 = champ->dst.ig4;
  ni = champ->dst.ni;
  nj = champ->dst.nj;
  uut = (float *) malloc(sizeof(float)*nnpas * npts);
  vvt = (float *) malloc(sizeof(float)*nnpas * npts);
  mapscl = (float *) malloc(sizeof(float)*ni * nj);
  uu = (float *) malloc(sizeof(float)*ni * nj);
  vv = (float *) malloc(sizeof(float)*ni * nj);
  f77name(cigaxg)(grtyp, &pi, &pj, &d60, &dgrw, &ig1, &ig2, &ig3, &ig4,(F2Cl) 1);
  f77name(mscale)(mapscl,&d60,&pi,&pj,&ni,&nj);

  for (i=0; i < ni*nj; i++)
    {
    uu[i] = champ->uu[i]*mapscl[i]*1.853/(d60/1000.);
    vv[i] = champ->vv[i]*mapscl[i]*1.853/(d60/1000.);
    }

  c_animvent(uu,vv,mapscl,ni,nj,seglen,uut,vvt,npts,nnpas,deet,segstep,delai);
  free(mapscl);
  free(uu);
  free(vv);
  free(uut);
  free(vvt);
  return ;
   }

void SetLICOn(Widget w, XtPointer client_data, XtPointer call_data)
{
  flagLIC = LIC;
  flagModule=NO_MODULUS;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

void SetModuleOff(Widget w, XtPointer client_data, XtPointer call_data)
{
  flagModule = NO_MODULUS;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

void SetLICOff (Widget w, XtPointer client_data, XtPointer call_data)
{
  flagLIC = NO_LIC;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

void SetModeNone(Widget w, XtPointer client_data, XtPointer call_data)
{
  displayMode = NONE;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

void SetModeBarbules(Widget w, XtPointer client_data, XtPointer call_data)
{
  displayMode = BARBULE;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

void SetModeFleches(Widget w, XtPointer client_data, XtPointer call_data)
{
  displayMode = FLECHES;
  if (XmToggleButtonGetState(w))
    {
    PventAfficher(NULL,NULL,NULL);
    }
}

void SetDensiteToggle (Widget w, XtPointer client_data, XtPointer call_data)
{
  if (0 == strcmp(XtName(w), pventLabelOptionsDensite[0][0]))
    {
    densite = -1.0;
    }
  else
    {
    sscanf(XtName(w),"%f",&densite);
    }
  PventAfficher(NULL,NULL,NULL);
}

void SetLongueurToggle(Widget w, XtPointer client_data, XtPointer call_data)
{
  sscanf(XtName(w),"%d",&longueur);
  PventAfficher(NULL,NULL,NULL);
}

void SetEpaisseurFToggle (Widget w, XtPointer client_data, XtPointer call_data)
{
  sscanf(XtName(w),"%d",&epaisseurF);
  PventAfficher(NULL,NULL,NULL);
}

void SetEchelleWWToggle (Widget w, XtPointer client_data, XtPointer call_data)
{
  int i;
  int oldEchelleWW = echelleWW;
  float x1,y1,x2,y2;
  sscanf(XtName(w),"%d",&echelleWW);


  for (i=0; i < FldMgrGetNbChampsActifs(); i++)
    {
    FldMgrRescaleWW(i,echelleWW,oldEchelleWW);
    }

  CoupeMgrGetCoupeCoords(&x1, &y1, &x2, &y2);
  PreparerCoupe(x1,y1,x2,y2);
  RedessinerFenetreCoupe();
}

void SetEchelleFlechesToggle(Widget w, XtPointer client_data, XtPointer call_data)
{

  if ((int)client_data == 0)
    {
    flagLegendeFleches = 1;
    }
  else
    {
    flagLegendeFleches = 0;
    }

  RedessinerFenetres();
}

void SetCroissanceToggle (Widget w, XtPointer client_data, XtPointer call_data)
{
  switch((int)client_data)
    {
    case 0:
      croissance = CUBIQUE;
      break;

    case 1:
      croissance = QUADRATIQUE;
      break;

    case 2:
      croissance = LINEAIRE;
      break;

    case 3:
      croissance = RACINE_CARREE;
      break;

    case 4:
      croissance = RACINE_CUBIQUE;
      break;
    }
  PventAfficher(NULL,NULL,NULL);
}

void PventOk(Widget w, XtPointer client_data, XtPointer call_data)
{
  pventSelectionTerminee = TRUE;
  DesactiverPanneauVents();
  return ;
}

void PventAfficher(Widget w, XtPointer client_data, XtPointer call_data)
{
  RedessinerFenetres();
  LibererImages();
  return ;
}


void InitPanneauVents()
{

  int i;
  Arg args[16];
  XmString string, label;

  int n,lng;
  char nomShell[128];

  Xinit("xregarder");
  lng = c_getulng();

  i = 0;
  strcpy(nomShell, XtName(SuperWidget.topLevel));
  strcat(nomShell, nomPanneauVents[lng]);
  i = 0;

  if (0 < strlen(panneauVentsGeometrie))
    {
    XtSetArg(args[i],XmNgeometry,panneauVentsGeometrie);
    i++;
    }

  pventTopLevel = XtAppCreateShell(nomShell, nomShell,
                                   applicationShellWidgetClass,
                                   XtDisplay(SuperWidget.topLevel), args, i);
  i = 0;
  pventForme = (Widget) XmCreateForm(pventTopLevel, "form", args, i);
  XtManageChild(pventForme);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  pventOk = (Widget)XmCreatePushButton(pventForme, labelOk[lng], args, i);
  XtAddCallback(pventOk, XmNactivateCallback, PventOk, NULL);
  XtManageChild(pventOk);

  i = 0;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNrightWidget, pventOk); i++;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  pventAfficher = (Widget)XmCreatePushButton(pventForme, labelAfficher[lng], args, i);
  XtAddCallback(pventAfficher, XmNactivateCallback, PventAfficher, NULL);
  XtManageChild(pventAfficher);

  i = 0;
  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, pventOk); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNnumColumns, 1); i++;
  XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
  XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
  pventRootRC = XmCreateRowColumn(pventForme, "pventRc", args, i);
  XtManageChild(pventRootRC);
  
  
  pventFormeToggles = (Widget) XmCreateForm(pventRootRC, "form", args, i);
  XtManageChild(pventFormeToggles);

  i = 0;
  pventFrameToggles = (Widget) XmCreateFrame(pventFormeToggles, "form", args, i);
  XtManageChild(pventFrameToggles);

  i = 0;
  pventFormeToggles2 = (Widget) XmCreateForm(pventFrameToggles, "form", args, i);
  XtManageChild(pventFormeToggles2);

  pventFormeBarbules = (Widget) XmCreateRowColumn(pventFormeToggles2, "form", NULL, 0);
  XtManageChild(pventFormeBarbules);

  pventLabelBarbules = (Widget) XmCreateLabel(pventFormeBarbules, labelSelectionBarbules[lng], NULL, 0);
  XtManageChild(pventLabelBarbules);

  pventFrameBarbules = (Widget) XmCreateFrame(pventFormeBarbules, "frame", NULL, 0);
  XtManageChild(pventFrameBarbules);

  pventSelectionBarbules = (Widget) XmCreateRadioBox(pventFrameBarbules, "radiobox", NULL, 0);
  XtManageChild(pventSelectionBarbules);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventAucun = (Widget) XmCreateToggleButton(pventSelectionBarbules, labelAucun[lng], args, i);
  XtAddCallback(pventAucun, XmNvalueChangedCallback, SetModeNone, NULL);
  XtManageChild(pventAucun);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventBarbules = (Widget) XmCreateToggleButton(pventSelectionBarbules, labelBarbules[lng], args, i);
  XtAddCallback(pventBarbules, XmNvalueChangedCallback, SetModeBarbules, NULL);
  XtManageChild(pventBarbules);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNset, True); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventFleches = (Widget)XmCreateToggleButton(pventSelectionBarbules, labelFleches[lng], args, i);
  XtAddCallback(pventFleches, XmNvalueChangedCallback, SetModeFleches, NULL);
  XtManageChild(pventFleches);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, pventFormeBarbules); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  pventFormeModule = (Widget) XmCreateRowColumn(pventFormeToggles2, "form", args, i);
  XtManageChild(pventFormeModule);

  pventLabelModule = (Widget) XmCreateLabel(pventFormeModule, labelModule[lng], NULL, 0);
  XtManageChild(pventLabelModule);

  pventFrameModule = (Widget) XmCreateFrame(pventFormeModule, "frame", NULL, 0);
  XtManageChild(pventFrameModule);

  pventSelectionModule = (Widget) XmCreateRadioBox(pventFrameModule, "radiobox", NULL, 0);
  XtManageChild(pventSelectionModule);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNset, False); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventModuleOn = (Widget) XmCreateToggleButton(pventSelectionModule, labelOui[lng], args, i);
  XtAddCallback(pventModuleOn, XmNvalueChangedCallback, SetModuleOn, NULL);
  XtManageChild(pventModuleOn);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNset, True); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventModuleOff = (Widget) XmCreateToggleButton(pventSelectionModule, labelNon[lng], args, i);
  XtAddCallback(pventModuleOff, XmNvalueChangedCallback, SetModuleOff, NULL);
  XtManageChild(pventModuleOff);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, pventFormeModule); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  pventFormeLIC = (Widget) XmCreateRowColumn(pventFormeToggles2, "form", args, i);
  XtManageChild(pventFormeLIC);

  pventLabelLIC = (Widget) XmCreateLabel(pventFormeLIC, labelLIC[lng], NULL, 0);
  XtManageChild(pventLabelLIC);

  pventFrameLIC = (Widget) XmCreateFrame(pventFormeLIC, "frame", NULL, 0);
  XtManageChild(pventFrameLIC);

  pventSelectionLIC = (Widget) XmCreateRadioBox(pventFrameLIC, "radiobox", NULL, 0);
  XtManageChild(pventSelectionLIC);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNset, False); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventLICOn = (Widget) XmCreateToggleButton(pventSelectionLIC, labelOui[lng], args, i);
  XtAddCallback(pventLICOn, XmNvalueChangedCallback, SetLICOn, NULL);
  XtManageChild(pventLICOn);

  i = 0;
  XtSetArg(args[i], XmNvisibleWhenOff, False); i++;
  XtSetArg(args[i], XmNset, True); i++;
  XtSetArg(args[i], XmNmarginHeight, 0); i++;
  XtSetArg(args[i], XmNmarginBottom, 0); i++;
  XtSetArg(args[i], XmNmarginTop, 0); i++;
  pventLICOff = (Widget) XmCreateToggleButton(pventSelectionLIC, labelNon[lng], args, i);
  XtAddCallback(pventLICOff, XmNvalueChangedCallback, SetLICOff, NULL);
  XtManageChild(pventLICOff);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, pventOk); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, pventFormeToggles); i++;
  pventFormeMenus = (Widget) XmCreateForm(pventRootRC, "form", args, i);
  XtManageChild(pventFormeMenus);

  i = 0;
  pventFrameVents = (Widget) XmCreateFrame(pventFormeMenus, "form", args, i);
  XtManageChild(pventFrameVents);

  /* Create RowColumn in pventTopLevel */

  i = 0;
  XtSetArg(args[i], XmNnumColumns, 1); i++;
  XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
  XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
  pventRc = XmCreateRowColumn(pventFrameVents, "pventRc", args, i);
  XtManageChild(pventRc);

  i = 0;
  pventOptionsDensite = (Widget)XmCreatePulldownMenu(pventRc, labelDensite[lng], args, i);

  for (n=0; n < XtNumber(pventLabelOptionsDensite[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(pventLabelOptionsDensite[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    pventOptionsDensiteItems[n] = XmCreatePushButtonGadget(pventOptionsDensite, pventLabelOptionsDensite[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(pventOptionsDensiteItems[n], XmNactivateCallback, SetDensiteToggle, (XtPointer) n);
    }

  XtManageChildren(pventOptionsDensiteItems, XtNumber(pventLabelOptionsDensite[lng]));

  i = 0;
  string = XmStringCreateLtoR(labelDensite[lng], XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[i], XmNlabelString, string); i++;
  XtSetArg(args[i], XmNsubMenuId, pventOptionsDensite); i++;
  XtSetArg(args[i], XmNmenuHistory, pventOptionsDensiteItems[currentItem]); i++;
  XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
  pventPanneauDensite = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
  XmStringFree(string);

  XtManageChild(pventPanneauDensite);

  /**
   **
   **/


  pventOptionsLongueur = (Widget)XmCreatePulldownMenu(pventRc, labelLongueur[lng], NULL, 0);

  for (n=0; n < XtNumber(pventLabelOptionsLongueur[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(pventLabelOptionsLongueur[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    pventOptionsLongueurItems[n] = XmCreatePushButtonGadget(pventOptionsLongueur, pventLabelOptionsLongueur[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(pventOptionsLongueurItems[n], XmNactivateCallback, SetLongueurToggle, (XtPointer) n);
    }

  XtManageChildren(pventOptionsLongueurItems, XtNumber(pventLabelOptionsLongueur[lng]));

  i = 0;
  currentItem = 3;
  string = XmStringCreateLtoR(labelLongueur[lng], XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[i], XmNlabelString, string); i++;
  XtSetArg(args[i], XmNsubMenuId, pventOptionsLongueur); i++;
  XtSetArg(args[i], XmNmenuHistory, pventOptionsLongueurItems[currentItem]); i++;
  XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
  pventPanneauLongueur = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
  XmStringFree(string);

  XtManageChild(pventPanneauLongueur);

  /**
   **
   **/

  pventOptionsEpaisseurF = (Widget)XmCreatePulldownMenu(pventRc, labelEpaisseurF[lng], NULL, 0);

  for (n=0; n < XtNumber(pventLabelOptionsEpaisseurF[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(pventLabelOptionsEpaisseurF[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    pventOptionsEpaisseurFItems[n] = XmCreatePushButtonGadget(pventOptionsEpaisseurF, pventLabelOptionsEpaisseurF[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(pventOptionsEpaisseurFItems[n], XmNactivateCallback, SetEpaisseurFToggle, (XtPointer) n);
    }

  XtManageChildren(pventOptionsEpaisseurFItems, XtNumber(pventLabelOptionsEpaisseurF[lng]));

  i = 0;
  currentItem = 0;
  string = XmStringCreateLtoR(labelEpaisseurF[lng], XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[i], XmNlabelString, string); i++;
  XtSetArg(args[i], XmNsubMenuId, pventOptionsEpaisseurF); i++;
  XtSetArg(args[i], XmNmenuHistory, pventOptionsEpaisseurFItems[currentItem]); i++;
  XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
  pventPanneauEpaisseurF = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
  XmStringFree(string);

  XtManageChild(pventPanneauEpaisseurF);

  /**
   **
   **/

  pventOptionsCroissance = (Widget)XmCreatePulldownMenu(pventRc, labelCroissance[lng], NULL, 0);

  for (n=0; n < XtNumber(pventLabelOptionsCroissance[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(pventLabelOptionsCroissance[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    pventOptionsCroissanceItems[n] = XmCreatePushButtonGadget(pventOptionsCroissance, pventLabelOptionsCroissance[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(pventOptionsCroissanceItems[n], XmNactivateCallback, SetCroissanceToggle, (XtPointer) n);
    }

  XtManageChildren(pventOptionsCroissanceItems, XtNumber(pventLabelOptionsCroissance[lng]));

  i = 0;
  currentItem = 2;
  string = XmStringCreateLtoR(labelCroissance[lng], XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[i], XmNlabelString, string); i++;
  XtSetArg(args[i], XmNsubMenuId, pventOptionsCroissance); i++;
  XtSetArg(args[i], XmNmenuHistory, pventOptionsCroissanceItems[currentItem]); i++;
  XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
  pventPanneauCroissance = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
  XmStringFree(string);

  XtManageChild(pventPanneauCroissance);

  /**
   **
   **/


  pventOptionsEchelleWW = (Widget)XmCreatePulldownMenu(pventRc, labelEchelleWW[lng], NULL, 0);

  for (n=0; n < XtNumber(pventLabelOptionsEchelleWW[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(pventLabelOptionsEchelleWW[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    pventOptionsEchelleWWItems[n] = XmCreatePushButtonGadget(pventOptionsEchelleWW, pventLabelOptionsEchelleWW[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(pventOptionsEchelleWWItems[n], XmNactivateCallback, SetEchelleWWToggle, (XtPointer) n);
    }

  XtManageChildren(pventOptionsEchelleWWItems, XtNumber(pventLabelOptionsEchelleWW[lng]));

  i = 0;
  currentItem = 5;
  string = XmStringCreateLtoR(labelEchelleWW[lng], XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[i], XmNlabelString, string); i++;
  XtSetArg(args[i], XmNsubMenuId, pventOptionsEchelleWW); i++;
  XtSetArg(args[i], XmNmenuHistory, pventOptionsEchelleWWItems[currentItem]); i++;
  XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
  pventPanneauEchelleWW = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
  XmStringFree(string);

  XtManageChild(pventPanneauEchelleWW);

  pventOptionsEchelleFleches = (Widget)XmCreatePulldownMenu(pventRc, labelEchelleFleches[lng], NULL, 0);

  for (n=0; n < XtNumber(pventLabelOptionsEchelleFleches[lng]); n++)
    {
    i = 0;
    string = XmStringCreateLtoR(pventLabelOptionsEchelleFleches[lng][n], XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[i], XmNlabelString, string); i++;
    pventOptionsEchelleFlechesItems[n] = XmCreatePushButtonGadget(pventOptionsEchelleFleches, pventLabelOptionsEchelleFleches[lng][n], args, i);
    XmStringFree(string);
    XtAddCallback(pventOptionsEchelleFlechesItems[n], XmNactivateCallback, SetEchelleFlechesToggle, (XtPointer) n);
    }

  XtManageChildren(pventOptionsEchelleFlechesItems, XtNumber(pventLabelOptionsEchelleFleches[lng]));

  i = 0;
  currentItem = 0;
  string = XmStringCreateLtoR(labelEchelleFleches[lng], XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[i], XmNlabelString, string); i++;
  XtSetArg(args[i], XmNsubMenuId, pventOptionsEchelleFleches); i++;
  XtSetArg(args[i], XmNmenuHistory, pventOptionsEchelleFlechesItems[currentItem]); i++;
  XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
  pventPanneauEchelleFleches = XmCreateOptionMenu(pventRc, "option_menu1", args, i);
  XmStringFree(string);

  XtManageChild(pventPanneauEchelleFleches);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, pventRootRC); i++;
  pventSeparateur1 = (Widget) XmCreateSeparator(pventForme, "separator", args, i);
  XtManageChild(pventSeparateur1);
  
  
  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, pventSeparateur1); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  pventFormeXStream = (Widget) XmCreateForm(pventForme, "form", args, i);
  XtManageChild(pventFormeXStream);

  i = 0;
  pventFrameXStream = (Widget) XmCreateFrame(pventFormeXStream, "form", args, i);
  XtManageChild(pventFrameXStream);

  i = 0;
  XtSetArg(args[i], XmNnumColumns, 1); i++;
  XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
  XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  pventXStreamRC = XmCreateRowColumn(pventFrameXStream, "pventRc", args, i);
  XtManageChild(pventXStreamRC);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   pventLabelXStream = (Widget)XmCreateLabel(pventXStreamRC, labelXStream[lng], args, i);
   XtManageChild(pventLabelXStream);
   
  i = 0;
  XtSetArg(args[i], XmNnumColumns, 2); i++;
  XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
  XtSetArg(args[i], XmNpacking, XmPACK_COLUMN); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  pventXStreamScalesRC = XmCreateRowColumn(pventXStreamRC, "pventRc", args, i);
  XtManageChild(pventXStreamScalesRC);

   i = 0;
   label = XmStringCreateLtoR(labelNPTS[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNminimum, 1000); i++;
   XtSetArg(args[i], XmNmaximum, 20000); i++;
   XtSetArg(args[i], XmNvalue, 2000); i++;
   XtSetArg(args[i], XmNscaleMultiple, 1000); i++;
   pventScaleNpts = (Widget)XmCreateScale(pventXStreamScalesRC, labelNPTS[lng], args, i);
   XtManageChild(pventScaleNpts);
   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelNPAS[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNscaleMultiple, 4); i++;
   XtSetArg(args[i], XmNminimum, 4); i++;
   XtSetArg(args[i], XmNmaximum, 256); i++;
   XtSetArg(args[i], XmNvalue, 64); i++;
   pventScaleNpas = (Widget)XmCreateScale(pventXStreamScalesRC, labelNPAS[lng], args, i);
   XtManageChild(pventScaleNpas);
   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelDEET[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNminimum, 60); i++;
   XtSetArg(args[i], XmNmaximum, 3600); i++;
   XtSetArg(args[i], XmNvalue, 1800); i++;
   XtSetArg(args[i], XmNscaleMultiple, 60); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNleftWidget, pventScaleNpas); i++;
   pventScaleDeet = (Widget)XmCreateScale(pventXStreamScalesRC, labelDEET[lng], args, i);
   XtManageChild(pventScaleDeet);
   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelSEGSTEP[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNminimum, 2); i++;
   XtSetArg(args[i], XmNmaximum, 32); i++;
   XtSetArg(args[i], XmNvalue, 4); i++;
   pventScaleSegstep = (Widget)XmCreateScale(pventXStreamScalesRC, labelSEGSTEP[lng], args, i);
   XtManageChild(pventScaleSegstep);
   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelSEGLEN[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 0); i++;
   XtSetArg(args[i], XmNminimum, 1); i++;
   XtSetArg(args[i], XmNmaximum, 31); i++;
   XtSetArg(args[i], XmNvalue, 3); i++;
   pventScaleSeglen = (Widget)XmCreateScale(pventXStreamScalesRC, labelSEGLEN[lng], args, i);
   XtManageChild(pventScaleSeglen);
   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelDELAI[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNtitleString, label); i++;
   XtSetArg(args[i], XmNshowValue, True); i++;
   XtSetArg(args[i], XmNdecimalPoints, 2); i++;
   XtSetArg(args[i], XmNminimum, 0); i++;
   XtSetArg(args[i], XmNvalue, 12); i++;
   XtSetArg(args[i], XmNmaximum, 100); i++;
   pventScaleDelai = (Widget)XmCreateScale(pventXStreamScalesRC, labelDELAI[lng], args, i);
   XtManageChild(pventScaleDelai);
   XmStringFree(label);
   
   
  i = 0;
  XtSetArg(args[i], XmNnumColumns, 1); i++;
  XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
  XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
  XtSetArg(args[i], XmNtopWidget, pventScaleNpts); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  pventXStreamBoutonsRc = XmCreateRowColumn(pventXStreamRC, "pventRc", args, i);
  XtManageChild(pventXStreamBoutonsRc);
   
   i = 0;
   pventBoutonXStreamStart = (Widget)XmCreatePushButton(pventXStreamBoutonsRc, "start", args, i);
   XtAddCallback(pventBoutonXStreamStart, XmNactivateCallback, PVentStart, NULL);
   XtManageChild(pventBoutonXStreamStart);

   pventBoutonXStreamStop = (Widget)XmCreatePushButton(pventXStreamBoutonsRc, "stop", args, i);
   XtAddCallback(pventBoutonXStreamStop, XmNactivateCallback, PVentStop, NULL);
   XtManageChild(pventBoutonXStreamStop);

   
   XtAddCallback(pventScaleNpts, XmNdragCallback, PVentSetNPTS, NULL);
   XtAddCallback(pventScaleNpts, XmNvalueChangedCallback, PVentSetNPTS, NULL);
   
   XtAddCallback(pventScaleNpas, XmNdragCallback, PVentSetNPAS, NULL);
   XtAddCallback(pventScaleNpas, XmNvalueChangedCallback, PVentSetNPAS, NULL);

   XtAddCallback(pventScaleDeet, XmNdragCallback, PVentSetDEET, NULL);
   XtAddCallback(pventScaleDeet, XmNvalueChangedCallback, PVentSetDEET, NULL);

   XtAddCallback(pventScaleDelai, XmNdragCallback, PVentSetDELAI, NULL);
   XtAddCallback(pventScaleDelai, XmNvalueChangedCallback, PVentSetDELAI, NULL);

   XtAddCallback(pventScaleSeglen, XmNdragCallback, PVentSetSEGLEN, NULL);
   XtAddCallback(pventScaleSeglen, XmNvalueChangedCallback, PVentSetSEGLEN, NULL);

   XtAddCallback(pventScaleSegstep, XmNdragCallback, PVentSetSEGSTEP, NULL);
   XtAddCallback(pventScaleSegstep, XmNvalueChangedCallback, PVentSetSEGSTEP, NULL);
}

void ActiverPanneauVents()
{
  if (!pventTopLevel)
    InitPanneauVents();

  pventSelectionTerminee = False;

  if (!XtIsRealized(pventTopLevel))
    {
    XtRealizeWidget(pventTopLevel);
    CheckColormap(pventTopLevel);
    }

  f77name(xpventact)();

}

void f77name(xpventact)()
{
  LocalEventLoop(pventTopLevel);
}


void DesactiverPanneauVents()
{
  XtUnrealizeWidget(pventTopLevel);
}

float WindMgrGetDensite()
{
  return densite;
}

int WindMgrGetCroissance()
{
  return croissance;
}

int WindMgrGetDisplayMode()
{
  return displayMode;
}

int WindMgrGetLongueur()
{
  return longueur;
}

int WindMgrGetEpaisseur()
{
  return epaisseurF;
}

int WindMgrGetModulusState()
{
  return flagModule;
}

int WindMgrGetLICState()
{
  return flagLIC;
}

int WindMgrGetEchelleWW()
{
  return echelleWW;
}

void f77name(c_swndatr)( char item[], char valeur[], F2Cl flenItem, F2Cl flenValeur)
{
  int lenItem=flenItem, lenValeur=flenValeur;
  
  item[lenItem-1] = '\0';
  valeur[lenValeur-1] = '\0';
  nettoyer(item);
  nettoyer(valeur);

  if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
    {
    strcpy(panneauVentsGeometrie,valeur);
    }

  if (0 == strcmp(item,"niveau_de_densite") || 0 == strcmp(item,"smoothing_factor"))
    {
    if (0 == strncmp("automati",valeur,8))
      {
      densite =-32767.0;
      printf("densite: %f\n", densite);
      }
    else
      {
      if (0 == strcmp(valeur,"maximum"))
  {
  densite = 32767.0;
  printf("densite: %f\n", densite);
  }
      else
  {
  densite = atof(valeur);
  printf("densite: %f\n", densite);
  }
      }
    }

  if (0 == strcmp(item,"taille_police_labels") || 0 == strcmp(item,"labels_font_size"))
    {
    longueur = atoi(valeur);
    }

  if (0 == strcmp(item,"taille_police_legende_couleurs") || 0 == strcmp(item,"color_legend_font_size"))
    {
    croissance = atoi(valeur);
    }

  if (0 == strcmp(item,"taille_police_legende") || 0 == strcmp(item,"legend_font_size"))
    {
    densite = atoi(valeur);
    }
}

void EcrWndAtr(FILE *fichierDemarrage)
{
  char tableau[32];
  char ligne[80];
  char item[32],valeur[32];
  int i;

  Arg  args[10];
  Position x,y;
  Display *disp;
  Window win;

  strcpy(tableau, "Vents_divers");

  strcpy(item,"geometrie");
  if (pventTopLevel)
    {
    disp = XtDisplay(pventTopLevel);
    win  = XtWindow(pventTopLevel);
    i = 0;
    XtSetArg(args[i], XmNx, &x); i++;
    XtSetArg(args[i], XmNy, &y); i++;
    XtGetValues(pventTopLevel, args, i);

    sprintf(valeur,"%+d%+d",x,y);
    sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
    fprintf(fichierDemarrage,"%s\n",ligne);
    }
  else
    {
    if (strlen(panneauVentsGeometrie) > 0)
      {
      strcpy(valeur,panneauVentsGeometrie);
      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }
    }
  sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
  fprintf(fichierDemarrage,"%s\n",ligne);

}
