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


#include <string.h>
#include <fcntl.h>
#include <rmn/rpnmacros.h>
#include "rec.h"
#include "rec_functions.h"
#include <memory.h>
#include "xinit.h"
#include <math.h>
#include "wgl.h"
#include "souris.h"
#include "rec_version.h"
#include "rec_functions.h"
#include "xrec_build_info.h"
#include "souris.h"

#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

/*#include <Xm/AccTextT.h>
#include <Xm/ActivatableT.h>
#include <Xm/ArrowB.h>
#include <Xm/ArrowBG.h>
#include <Xm/ArrowBGP.h>
#include <Xm/ArrowBP.h>
#include <Xm/AtomMgr.h>
#include <Xm/BaseClassP.h>
#include <Xm/BulletinB.h>
#include <Xm/BulletinBP.h>
#include <Xm/CacheP.h>
#include <Xm/CareVisualT.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/CascadeBGP.h>
#include <Xm/CascadeBP.h>
#include <Xm/ColorObjP.h>
#include <Xm/ComboBox.h>
#include <Xm/ComboBoxP.h>
#include <Xm/Command.h>
#include <Xm/CommandP.h>
#include <Xm/ContItemT.h>
#include <Xm/Container.h>
#include <Xm/ContainerP.h>
#include <Xm/ContainerT.h>
#include <Xm/CutPaste.h>
#include <Xm/DesktopP.h>
#include <Xm/DialogS.h>
#include <Xm/DialogSEP.h>
#include <Xm/DialogSP.h>
#include <Xm/DialogSavvyT.h>
#include <Xm/Display.h>
#include <Xm/DisplayP.h>
#include <Xm/DragC.h>
#include <Xm/DragCP.h>
#include <Xm/DragDrop.h>
#include <Xm/DragIcon.h>
#include <Xm/DragIconP.h>
#include <Xm/DragOverS.h>
#include <Xm/DragOverSP.h>
#include <Xm/DrawP.h>
#include <Xm/DrawingA.h>
#include <Xm/DrawingAP.h>
#include <Xm/DrawnB.h>
#include <Xm/DrawnBP.h>
#include <Xm/DropSMgr.h>
#include <Xm/DropSMgrP.h>
#include <Xm/DropTrans.h>
#include <Xm/DropTransP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/FileSB.h>
#include <Xm/FileSBP.h>
#include <Xm/FormP.h>
#include <Xm/Frame.h>
#include <Xm/FrameP.h>
#include <Xm/Gadget.h>
#include <Xm/GadgetP.h>
#include <Xm/GrabShell.h>
#include <Xm/GrabShellP.h>
#include <Xm/IconG.h>
#include <Xm/IconGP.h>
#include <Xm/IconH.h>
#include <Xm/JoinSideT.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/LabelGP.h>
#include <Xm/LabelP.h>
#include <Xm/LayoutT.h>
#include <Xm/List.h>
#include <Xm/ListP.h>
#include <Xm/MainW.h>
#include <Xm/MainWP.h>
#include <Xm/Manager.h>
#include <Xm/ManagerP.h>
#include <Xm/MenuShell.h>
#include <Xm/MenuShellP.h>
#include <Xm/MenuT.h>
#include <Xm/MenuUtilP.h>
#include <Xm/MessageB.h>
#include <Xm/MessageBP.h>
#include <Xm/MwmUtil.h>
#include <Xm/NavigatorT.h>
#include <Xm/Notebook.h>
#include <Xm/NotebookP.h>
#include <Xm/PanedW.h>
#include <Xm/PanedWP.h>
#include <Xm/Primitive.h>
#include <Xm/PrimitiveP.h>
#include <Xm/Print.h>
#include <Xm/PrintSP.h>
#include <Xm/Protocols.h>
#include <Xm/ProtocolsP.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/PushBGP.h>
#include <Xm/PushBP.h>
#include <Xm/RepType.h>
#include <Xm/RowColumn.h>
#include <Xm/RowColumnP.h>
#include <Xm/SSpinB.h>
#include <Xm/SSpinBP.h>
#include <Xm/SashP.h>
#include <Xm/Scale.h>
#include <Xm/ScaleP.h>
#include <Xm/Screen.h>
#include <Xm/ScreenP.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrollBarP.h>
#include <Xm/ScrollFrameT.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrolledWP.h>
#include <Xm/SelectioB.h>
#include <Xm/SelectioBP.h>
#include <Xm/SeparatoG.h>
#include <Xm/SeparatoGP.h>
#include <Xm/Separator.h>
#include <Xm/SeparatorP.h>
#include <Xm/ShellEP.h>
#include <Xm/SpecRenderT.h>
#include <Xm/SpinB.h>
#include <Xm/SpinBP.h>
#include <Xm/TakesDefT.h>
#include <Xm/TearOffBP.h>
#include <Xm/TearOffP.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/TextFP.h>
#include <Xm/TextFSelP.h>
#include <Xm/TextInP.h>
#include <Xm/TextOutP.h>
#include <Xm/TextP.h>
#include <Xm/TextSelP.h>
#include <Xm/TextStrSoP.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/ToggleBGP.h>
#include <Xm/ToggleBP.h>
#include <Xm/TraitP.h>
#include <Xm/Transfer.h>
#include <Xm/TransferP.h>
#include <Xm/TransferT.h>
#include <Xm/TransltnsP.h>
#include <Xm/TxtPropCv.h>
#include <Xm/VaSimpleP.h>
#include <Xm/VendorS.h>
#include <Xm/VendorSEP.h>
#include <Xm/VendorSP.h>
#include <Xm/VirtKeys.h>
#include <Xm/VirtKeysP.h>
#include <Xm/Xm.h>
#include <Xm/XmAll.h>
#include <Xm/XmIm.h>
#include <Xm/XmP.h>
#include <Xm/XmStrDefs.h>

#include <Xm/XmosP.h>
#include <Xm/XpmP.h>
*/

#define  TERMINE         0
#define  OUVRIR          1
#define  FERMER          2
#define  INFO_REC        3
#define  NOUVEAU_CHAMP   4
#define  QUITTER        -1
#define  QUITTER_ET_SAUVER -2

#define  DIAPOS          2000
#define  FILMS           2001

static char statutStr[14][2][48] = {{"Couleurs", "Colors" },
            {"Contours", "Contours"},
                  {"Labels", "Labels"},
                  {"Valeurs centrales","Central values"},
                  {"Geographie", "Geography"},
            {"Grille source", "Source grid"},
                  {"Grille destination","Output Grid"},
                  {"Legende", "Legend"},
                  {"Legende couleurs", "Color Legend"},
                  {"Lissage", "Smoothing"},
            {"Extremes locaux","Local Extrema"},
            {"Topographie","Topography"},
            {"Valeurs manquantes", "Missing values"},
                  {"Redessiner la fenetre apres selection", "Redraw Window after selection"}};

static char VecsStr[3][2][48] = {{"Traitement scalaire, affichage scalaire", "Process as scalar, display as scalar" },
         {"Traitement vectoriel, affichage scalaire", "Process as vector, display as scalar"},
         {"Traitement vectoriel, affichage vectoriel", "Process as vector, display as vector"}};




extern SuperWidgetStruct SuperWidget;

/* Fonctions externes */


/*  Fonctions locales */


/* Declaration des items contenus dans les menus */


static char *menuItemFacteurMult[]  = { "0.00", };
static char *labelSuperpositionOn[] = { "Activer\nsuperposition", "Activate\nSuperposition" };
static char *labelSuperpositionOff[]= { "Desactiver\nsuperposition", "Desactivate\nSuperposition"};
static char *labelZoom[]            = { "Zoom", "Zoom" };
static char *labelValeurs[]         = { "Valeurs aux\npts de grille", "Grid point\nvalues" };
static char *labelStats[]           = { "Statistiques", "Statistics"};
static char *labelRafraichir[]      = { "Redessiner\nfenetre(s)", "Redraw\nwindow(s)" };
static char *labelEffacer[]         = { "Effacer tous\nles champs", "Erase\nall fields" };

static char *labelMenuFichier[]   =   { "Fichier ", "File " };
static char *labelMenuAffichage[]   = { "Affichage ", "Display " };
static char *labelMenuIntervalle[] =  { "Contours", "Contours" };
static char *labelMenuGrille[]      =  { "Grille", "Grid" };
static char *labelMenuCalculs[]      =  { "Calculs", "Calc. " };
static char *labelMenuVecteurs[]     = {"Vecteurs", "Vectors"};

static char *labelQuitter[]       = { "Quitter", "Quit" };
static char *labelQuitterEtSauver[]       = { "Quitter et sauvegarder configuration", "Quit and save configuration" };
static char *labelOuvrir[]        = { "Ouvrir d'autres fichiers standards", "Open other standard files" };
static char *labelFermer[]        = { "Fermer des fichiers standards", "Close standard files" };
static char *labelDiapos[]        = { "Produire une image...", "Produce picture... " };
static char *labelFilms[]        = { "Produire une sequence...", "Produce sequence..." };
static char *labelEdition[]        = { "Edition de champs...", "Field Editing..." };
static char *labelAnimation[]      = { "Animation temporelle...", "Time Animation..."};
static char *labelAnimationVerticale[]      = { "Animation dans la verticale...", "Vertical Animation across vertical..."};
static char *labelGeographie[]     = { "Geographie...", "Geography..." };
static char *labelContours[]       = { "Contours...", "Contours..." };
static char *labelAttributs[]      = { "Legende/Interpolation...", "Legend/Interpolation..." };
static char *labelPalette[]        = { "Palettes...", "Palettes..." };
static char *labelMinMax[]         = { "Min-Max / Valeurs manquantes...", "Min-Max / Missing Values..." };
static char *labelCoupe[]          = { "Coupes verticales...", "Vertical Cross-sections..." };
static char *labelSerie[]          = { "Series temporelles...", "Time Series..." };
static char *labelVent[]           = { "Champs vectoriels...", "Vector fields..."};
static char *labelValeursPonctuelles[] = { "Valeurs ponctuelles...", "Point Values..."};

static char *labelMenuCalculsNoOp[]  = { "Aucune operation", "No calculation"};
static char *labelMenuCalculsSubtract[] = {"Champ1 - Champ2, Champ3 - Champ4, ...",
              "Field1 - Field2, Field3 - Field4, ..."};
static char *labelMenuCalculsAdd[] = {"Champ1 + Champ2, Champ3 + Champ4, ...",
              "Field1 + Field2, Field3 + Field4, ..."};
static char *labelMenuCalculsAbsSubtract[] = {"ABS(Champ1 - Champ2), ABS(Champ3 - Champ4), ...",
              "ABS(Field1 - Field2), ABS(Field3 - Field4), ..."};
static char *labelMenuCalculsAbsAdd[] = {"ABS(Champ1 + Champ2), ABS(Champ3 + Champ4), ...",
              "ABS(Field1 + Field2),  ABS(Field3 + Field4), ..."};

static char *lblAvrtSup1[]={"\nCe champ est defini sur une grille\nde type different de celui affiche.\nImpossible de superposer!\n",
                               "\nThis field is not of the same grid type\nas the one showed. Can't superpose\n"};
static char *lblAvrtSup2[]={"\nCe champ est defini sur une grille\nde type different de ceux affiches.\nImpossible de superposer!\n",
                               "\nThis field is not of the same grid type\nas those showed. Can't superpose\n"};

static char *lblAvrtTrop[]={"\nOn ne peut superposer\nplus de 32 champs a la fois\n",
                               "\nCan't display more than 32 fields\n"};

/* Variables globales */

extern int lng;
extern GeoMapInfoStruct    mapInfo, oldMapInfo;
extern GeoMapFlagsStruct   mapFlags;
extern _InfoChamps *infoChamps;
extern _AnimInfo    animInfo;
char panneauContourGeometrie[32];

_XContour     xc;
_Viewport     viewp;
_ColormapInfo recCmap;

Pixmap check;
Dimension hauteurMenus = 25;
int recColorTable[256];
int sizeRecColorTable;
int facteurLissage = -32767;
int fenetreAffichage = 0;
int mathOp = NO_OP;
static int rpnLogo = 1;
int itemGrilleSelectionnee = 0;

void f77name(initrlx)();


/* ****************************************************************************** */

void ActiverWidgetsControle()
{
   short i;
   Arg  args[10];

   i = 0;
   XtSetArg(args[i], XmNsensitive, True); i++;

   XtSetValues(xc.menuform, args, i);
   XtSetValues(xc.menuFich, args, i);
   XtSetValues(xc.menuAff, args, i);
   XtSetValues(xc.menuSup, args, i);
   XtSetValues(xc.menuInt, args, i);
   XtSetValues(xc.menuOpt, args, i);

   XtSetValues(xc.zoom, args, i);
   XtSetValues(xc.valeursPonctuelles, args, i);
   XtSetValues(xc.rafraichir, args, i);

   XtSetValues(xc.superposition, args, i);
   XtSetValues(xc.effacer, args, i);

   FlusherTousLesEvenements();
}

/* ****************************************************************************** */

void ActiverFlagsAffichage()
{
  Arg args[2];
  int i;

  for (i=0; i < NB_MENU_AFFICHAGE_ITEMS; i++)
    {
    switch(xc.statuts[i])
      {
      case TRUE:
      XtSetArg(args[0], XmNset, True);
      XtSetValues(xc.menuAffichageItems[i], args, 1);
      break;

      case FALSE:
      XtSetArg(args[0], XmNset, False);
      XtSetValues(xc.menuAffichageItems[i], args, 1);
      break;
      }
    }


}

/* ****************************************************************************** */

float labelPos[4][4] = {{0.16, 0.40, 0.64, 0.84},
      {0.20, 0.44, 0.68, 0.98},
      {0.24, 0.48, 0.72, 0.92},
      {0.28, 0.52, 0.76, 0.96}};


/* ****************************************************************************** */

void AfficherCarte(int indChamp)
{
  _Champ *champ;
  int fenetreAffichage;

  GetFenetreAffichageID(&fenetreAffichage);
  c_wglsetw(fenetreAffichage);
  FldMgrGetChamp(&champ, indChamp);

  if (xc.ChampAContourer)
    {
    xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;

    AjusterViewport(&viewp);
    if (indChamp == 0)
      {
      EffacerLaFenetreSiNecessaire();
      }

    switch (champ->domaine)
      {
      case XZ:
      case YZ:
      ManipulerChampBiDimensionnel(indChamp);
      break;

      case XY:
      ManipulerChampBiDimensionnel(indChamp);
      break;

      case VALEURS_PONCTUELLES:
      AfficherValeursPonctuelles(indChamp);
      break;
      }

    AfficherLegende(indChamp);
    AfficherPerimetreFenetre();
    xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
    }
  UnSetCurseur(fenetreAffichage);
}

/* ****************************************************************************** */

void AfficherLegende(int indChamp)
{
  _Champ *champ, *champZero;
  int nbChampsActifs;
  float min,max;
  int op;

  nbChampsActifs = FldMgrGetNbChampsActifs();
  FldMgrGetChamp(&champ, indChamp);
  FldMgrGetChamp(&champZero, 0);
  op = CtrlMgrGetContextualMathOp(indChamp);


  if (AfficherItem(indChamp, LEGENDE))
    {
    if (xc.statutSuperposition)
      {
      if (op != NO_OP || indChamp == (nbChampsActifs - 1))
        {
        AfficherLegendeSup2();
        }
      else
        {
        AfficherLegende2(*champ);
        }
      }
    else
      {
      AfficherLegende2(*champ);
      }
    }

  if (AfficherItem(indChamp, LEGENDE_COULEUR))
    {
    if (xc.statuts[ZOOM_LOCAL] && AUTO == DictMgrGetMinMaxMode(champZero->nomvar))
      {
      min = champZero->localMin;
      max = champZero->localMax;
      }
    else
      {
      switch (champZero->natureTensorielle)
        {
        case SCALAIRE:
          if (AUTO == DictMgrGetMinMaxMode(champZero->nomvar))
            {
            min = champZero->fldmin[op];
            max = champZero->fldmax[op];
            }
          else
            {
            DictMgrGetMinMaxValues(champZero->nomvar,&min,&max);
            }
          break;


      case VECTEUR:
        switch (xc.statuts[BARBULES])
          {
          case FALSE:
            if (0 == strcmp(champZero->nomvar, "UU"))
              {
              if (AUTO == DictMgrGetMinMaxMode(champZero->nomvar))
                {
                min = champZero->uumin[op];
                max = champZero->uumax[op];
                }
                    else
                {
                DictMgrGetMinMaxValues(champZero->nomvar,&min,&max);
                }
              }
            else
              {
              if (AUTO == DictMgrGetMinMaxMode(champZero->nomvar))
                {
                min = champZero->vvmin[op];
                max = champZero->vvmax[op];
                }
              else
                {
                DictMgrGetMinMaxValues(champZero->nomvar,&min,&max);
                }
              }
            break;

          case TRUE:
            if (AUTO == DictMgrGetMinMaxMode("UV"))
              {
              min = champZero->uvmin[op];
              max = champZero->uvmax[op];
              }
            else
              {
              DictMgrGetMinMaxValues("UV",&min,&max);
              }
            break;
          }
        break;
        }

      }
      AfficherLegendeCouleur(recColorTable,
           min,max,
           champZero->intervalles, champZero->nbIntervalles,
           champZero->facteur, 0.85, 0.05, 1.0, 0.95);

    }
}


/* ****************************************************************************** */


void AfficherLegendeCoupe(int indChamp)
{
  _Champ *champ, *champZero;
  int i, nbChampsActifs;
  int dernierNiveau, op;
  float valMin, valMax, bidon1, bidon2;
  float uutanmin, uutanmax, uvwmin, uvwmax, uumin, uumax, vvmin, vvmax, wwmin, wwmax, nivmin, nivmax;

  nbChampsActifs = DiffMgrGetNbChampsAffichables();
  nbChampsActifs = nbChampsActifs > 4 ? 4 : nbChampsActifs;
  op = CtrlMgrGetMathOp();
  dernierNiveau = -1;

  for (i=0; i < nbChampsActifs; i++)
    {
    FldMgrGetChamp(&champ, i);
    if (ZP == CoupeMgrGetDimensionCoupe())
      {
      if (champ->coupe.coupeValide)
        {
        dernierNiveau = i;
        }
      }
    else
      {
      if (champ->seqanim.nbFldsAnim > 0)
        {
        dernierNiveau = i;
        }
      }
    }

  if (indChamp !=  dernierNiveau && op == NO_OP)
    return;

  FldMgrGetChamp(&champ, indChamp);
  if (xc.statuts[LEGENDE])
    {
    if (xc.statutSuperposition)
      {
      AfficherLegendeSup2();
      }
    else
      {
      AfficherLegendeCoupe2(*champ);
      }
    }

  FldMgrGetChamp(&champZero, 0);
  if (AfficherItem(0,LEGENDE_COULEUR))
    {
    if (champZero->natureTensorielle == SCALAIRE)
      {
      CoupeMgrGetLimites(&valMin, &valMax, &bidon1, &bidon2);
      }
    else
      {
      CoupeMgrGetLimitesUVW(&uutanmin,&uutanmax,&uvwmin,&uvwmax,
          &uumin,&uumax,&vvmin,&vvmax,
          &wwmin,&wwmax,&nivmin,&nivmax);
        valMin = uvwmin;
        valMax = uvwmax;
      }

    if (T == CoupeMgrGetDimensionCoupe())
      {
      valMin = bidon1;
      valMax = bidon2;
      }

    valMin *= champZero->facteur;
    valMax *= champZero->facteur;
    if (CUSTOM == DictMgrGetMinMaxMode(champZero->nomvar))
      {
      DictMgrGetMinMaxValues(champZero->nomvar,&valMin,&valMax);
      }
    AfficherLegendeCouleur(recColorTable, valMin, valMax,
         champZero->intervalles, champZero->nbIntervalles,
         champZero->facteur, 0.85, 0.05, 1.0, 0.95);
    }

}


/* ****************************************************************************** */

void AfficherLegendeSerie(int indChamp)
{
  _Champ *champ, *champZero;
  int i, nbChampsActifs;
  int dernierNiveau, op;
  float valMin, valMax, tmin, tmax;
   float uutanmin, uutanmax, uvwmin, uvwmax, uumin, uumax, vvmin, vvmax, wwmin, wwmax, nivmin, nivmax;

  nbChampsActifs = DiffMgrGetNbChampsAffichables();
  nbChampsActifs = nbChampsActifs > 4 ? 4 : nbChampsActifs;
  op = CtrlMgrGetMathOp();
  dernierNiveau = -1;


  for (i=0; i < nbChampsActifs; i++)
    {
    FldMgrGetChamp(&champ, i);
    if (champ->seqanim.nbFldsAnim > 0)
      dernierNiveau = i;
    }

  if (indChamp !=  dernierNiveau && op == NO_OP)
    return;

  FldMgrGetChamp(&champ, indChamp);
  if (xc.statuts[LEGENDE])
    {
    if (xc.statutSuperposition)
      {
      AfficherLegendeSup2();
      }
    else
      {
      AfficherLegendeCoupe2(*champ);
      }
    }

  FldMgrGetChamp(&champZero, 0);
  if (AfficherItem(0,LEGENDE_COULEUR))
    {
    if (champZero->natureTensorielle == SCALAIRE)
      {
      SerieMgrGetLimites(&valMin, &valMax, &tmin, &tmax);
      }
    else
      {
      SerieMgrGetLimitesUVW(&uutanmin,&uutanmax,&uvwmin,&uvwmax,
          &uumin,&uumax,&vvmin,&vvmax,
          &wwmin,&wwmax,&nivmin,&nivmax);
      if (0 == strcmp("UU", champZero->nomvar))
         {
         valMin = uumin;
         valMax = uumax;
         }
      else
         {
         valMin = vvmin;
         valMax = vvmax;
         }
      }

    valMin = tmin;
    valMax = tmax;

    valMin *= champZero->facteur;
    valMax *= champZero->facteur;
    if (CUSTOM == DictMgrGetMinMaxMode(champZero->nomvar))
      {
      DictMgrGetMinMaxValues(champZero->nomvar,&valMin,&valMax);
      }
    AfficherLegendeCouleur(recColorTable, valMin, valMax,
         champZero->intervalles, champZero->nbIntervalles,
         champZero->facteur, 0.85, 0.05, 1.0, 0.95);
    }

}


/* ****************************************************************************** */

void AfficherValeursPonctuelles(int indChamp)
{
   _Champ *champ, *champ_ref;
   int fontSize;

   FldMgrGetChamp(&champ, indChamp);
   FldMgrGetChamp(&champ_ref, 0);

   SetClipMask();
   c_wgllwi(1);

   if (indChamp == 0)
      {
      switch(xc.statuts[GEOGRAPHIE])
        {
        case TRUE:
        c_gmpdrw();
        break;

        case FALSE:
        break;
        }
      }

   if (xc.attributs[indChamp].labelSize == 0)
      {
      fontSize = AttrMgrGetFontSizeLabels();
      }
   else
      {
      fontSize = xc.attributs[indChamp].labelSize;
      }

   c_wglfsz(fontSize);
   DrawPointValues(champ, indChamp, fontSize, xc.attributs[indChamp].epaisseur,
       xc.attributs[indChamp].indCouleurFore, xc.attributs[indChamp].indCouleurBack, recColorTable, sizeRecColorTable);
   }


/* ****************************************************************************** */

int CalculerFacteurLissage(int ni, int nj)
{
   int facteurLissage;
   float densiteX, densiteY, densitePix;
   int largeur, hauteur;

   int   ideb, jdeb, ifin, jfin;

/**
   DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, ni, nj);
   c_wglgvx(&xdeb, &ydeb, &xfin, &yfin);
   c_wglxai(&ideb, &jdeb, xdeb, ydeb);
   c_wglxai(&ifin,   &jfin, xfin, yfin);

   largeur = ifin - ideb + 1;
   hauteur = jfin - jdeb + 1;

   xlissfac = largeur/ni;
   ylissfac = hauteur/nj;

   facteurLissage = xlissfac < ylissfac ? ylissfac : xlissfac;
   return facteurLissage;
**/

   c_wglgvi(&ideb, &jdeb, &ifin, &jfin);

   densiteX = (float)(ifin - ideb)/(float)(ni - 1);
   densiteY = (float)(jfin - jdeb)/(float)(nj - 1);

   c_wglgwz(&largeur, &hauteur);
   densiteX = (float)(largeur)/(float)(ni - 1);
   densiteY = (float)(hauteur)/(float)(nj - 1);

   densitePix = densiteX;
   if (densitePix > densiteY)
      densitePix = densiteY;

   facteurLissage = (int)(densitePix / 2.0);
   if (facteurLissage < 2)
      return 2;
   else
      return facteurLissage;
   }


/* ****************************************************************************** */

/** ARGSUSED **/
void ChangerMenuIntervalles(int indMenu)
{
   int i, j, n, nbInt;
   static char menuCourant[24][128];
   static char tampon[128];
   char format[8];
   Arg args[10];
   _Champ *champ;


   FldMgrGetChamp(&champ, FldMgrGetNbChampsActifs() - 1);


   if (indMenu < 0)
      indMenu = 0;

   for (i=0; i < xc.NbMenusIntervalles; i++)
      {
      XtDestroyWidget(xc.menuIntervalleItems[i]);
      }

   xc.NbMenusIntervalles = infoChamps[champ->indDict].nbMenuItems;

   nbInt = infoChamps[champ->indDict].indIntervalleDeDefaut;
   champ->nbIntervalles = infoChamps[champ->indDict].nbIntervalles[nbInt];
   for (i=0; i < xc.NbMenusIntervalles; i++)
      {
      strcpy(menuCourant[i], "");
      j = 0;
      XtSetArg(args[j], XmNheight, hauteurMenus+5); j++;
      XtSetArg(args[j], XmNrecomputeSize, False); j++;
      XtSetArg(args[j], XmNindicatorType, XmONE_OF_MANY); j++;
      GetFormat(format, infoChamps[champ->indDict].intervallesDeContour[i],
    infoChamps[champ->indDict].nbIntervalles[i], infoChamps[champ->indDict].facteurDeConversion);
      n = 0;
      while (n < infoChamps[champ->indDict].nbIntervalles[i] && strlen(menuCourant[i]) < 64)
   {
   if (n == 0 && infoChamps[champ->indDict].nbIntervalles[i] > 1)
      strcat(menuCourant[i], "[");
   sprintf(tampon,format,infoChamps[champ->indDict].intervallesDeContour[i][n]);
   strcat(menuCourant[i], tampon);
   if (n >= 0 && n < (infoChamps[champ->indDict].nbIntervalles[i]-1))
      strcat(menuCourant[i], ",");
   if (infoChamps[champ->indDict].nbIntervalles[i] > 1 && n == (infoChamps[champ->indDict].nbIntervalles[i]-1))
      strcat(menuCourant[i], "]");
   n++;
   }

      if (n < infoChamps[champ->indDict].nbIntervalles[i]-1)
   {
   strcat(menuCourant[i], "...]");
   }

      xc.menuIntervalleItems[i] = (Widget)XmCreateToggleButton(xc.menuIntervalleDeContour, menuCourant[i], args, j);
      XtAddCallback(xc.menuIntervalleItems[i], XmNvalueChangedCallback, MenuIntervalleSelect, (XtPointer) (long)i);
      }

   XtManageChildren(xc.menuIntervalleItems, xc.NbMenusIntervalles);
   XtManageChild(xc.menuInt);

   for (i=0; i < xc.NbMenusIntervalles; i++)
      {
      XtSetArg(args[0], XmNset, False);
      XtSetValues(xc.menuIntervalleItems[i], args, 1);
      }

   XtSetArg(args[0], XmNset, True);
   XtSetValues(xc.menuIntervalleItems[indMenu], args, 1);
   return ;
   }


/* ****************************************************************************** */

/** ARGSUSED **/
void ChangerStatutSuperposition(Widget w,  XtPointer unused1, XtPointer unused2)
{
   _Champ *champ;

   int i;
   Arg args[10];
   float min,max;
   int fenetreAffichage;

   XmString label = NULL;

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return ;
      }

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   AjusterViewport(&viewp);

   FldMgrGetChamp(&champ, 0);
   if (AUTO == DictMgrGetMinMaxMode(champ->nomvar))
      {
      min = champ->fldmin[NO_OP];
      max = champ->fldmax[NO_OP];
      }
   else
      {
      DictMgrGetMinMaxValues(champ->nomvar,&min,&max);
      }

   switch(xc.statutSuperposition)
      {
      case TRUE:
        i = 0;
        label = XmStringCreateLtoR(labelSuperpositionOn[lng], XmSTRING_DEFAULT_CHARSET);
        XtSetArg(args[i], XmNlabelString, label); i++;
        XtSetValues(xc.superposition, args, i);

        xc.statutSuperposition = FALSE;
        switch (FldMgrGetNbChampsActifs())
           {
           case 0:
             break;

           case 1:
             if (xc.statuts[LEGENDE])
                {
                EffacerLegende2();
                AfficherLegende2(*champ);

                if (AfficherItem(0,LEGENDE_COULEUR))
                   {
                   AfficherLegendeCouleur(recColorTable, min, max, champ->intervalles,
                                          champ->nbIntervalles, champ->facteur, 0.85, 0.05, 1.0, 0.95);
                   }

                AfficherPerimetreFenetre();
                }
             break;

           default:
             i = FldMgrGetNbChampsActifs();
             while (1 < FldMgrGetNbChampsActifs())
                {
                FldMgrRemoveChamp(i-1);
                i--;
                }

             xc.annulationDemandee = FALSE;
             AfficherBoutonAnnulation();
             AfficherCarte(0);
             EnleverBoutonAnnulation();

             CheckMenuIntervalles(infoChamps[champ->indDict].indIntervalleDeDefaut);
             break;
           }
        break;


      case FALSE:
        i = 0;
        label = XmStringCreateLtoR(labelSuperpositionOff[lng], XmSTRING_DEFAULT_CHARSET);
        XtSetArg(args[i], XmNlabelString, label); i++;
        XtSetValues(xc.superposition, args, i);

        xc.statutSuperposition = TRUE;

        if (1 == FldMgrGetNbChampsActifs())
           {
           if (xc.statuts[LEGENDE])
              {
              EffacerLegende2();
              AfficherLegendeSup2();
              if (AfficherItem(0,LEGENDE_COULEUR))
                 {
                 AfficherLegendeCouleur(recColorTable, min, max, champ->intervalles, champ->nbIntervalles,
                                        champ->facteur, 0.85, 0.05, 1.0, 0.95);
                 }
              AfficherPerimetreFenetre();
              }
           }
        break;
      }

   XmStringFree(label);
   RedessinerFenetreCoupe();
   RedessinerFenetreSerie();
   return ;
}

/* ****************************************************************************** */


void CheckMenuIntervalles(int indMenu)
{
   ChangerMenuIntervalles(indMenu);
   }

/* ****************************************************************************** */

void f77name(xcondown)()
{
   DesactiverWidgetsControle();
   }

/* ****************************************************************************** */

void  f77name(xconup)()
{
   ActiverWidgetsControle();
   }


void DesactiverWidgetsControle()
{
   short i;
   Arg  args[10];

   i = 0;
   XtSetArg(args[i], XmNsensitive, False); i++;

   XtSetValues(xc.menuFich, args, i);
   XtSetValues(xc.menuAff, args, i);
   XtSetValues(xc.menuSup, args, i);
   XtSetValues(xc.menuInt, args, i);
   XtSetValues(xc.menuOpt, args, i);

   XtSetValues(xc.zoom, args, i);
   XtSetValues(xc.valeursPonctuelles, args, i);
   XtSetValues(xc.rafraichir, args, i);

   XtSetValues(xc.superposition, args, i);
   XtSetValues(xc.effacer, args, i);

   FlusherTousLesEvenements();
   }

/* ****************************************************************************** */

/** ARGSUSED **/
void Effacer(Widget w, XtPointer unused1, XtPointer unused2)
{
   int i;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return ;
      }

   if (champ->champModifie)
      {
      RefuserOperation();
      }

   EffacerFenetreAffichage();
   EffacerFenetreCoupe();
   EffacerFenetreSerie();
   EffacerCoupe();
   i = FldMgrGetNbChampsActifs();
   while (0 < FldMgrGetNbChampsActifs())
      {
      FldMgrRemoveChamp(i-1);
      i--;
      }

   LibererImages();
   /*   GeoMgrClearMapInfo(); */
   xc.ChampAContourer = FALSE;
   return ;
   }

/* ****************************************************************************** */

void EffacerFenetre()
{
   int col;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);
   /**   if (!xc.statuts[COULEURS] || champ->domaine == VALEURS_PONCTUELLES)**/
   if (champ->domaine == VALEURS_PONCTUELLES)
      {
      col = BLANC;
      }
   else
      {
      if (8 <= c_wglgpl())
   col = xc.attributs[FOND].indCouleurFore;
      else
   col = BLANC;
      }

   c_wglcol(col);
   c_wglclr();
   }

void EffacerFenetres()
{
   EffacerFenetreAffichage();
   EffacerFenetreCoupe();
   EffacerFenetreSerie();
   }

/* ****************************************************************************** */

void EffacerFenetreAffichage()
{
   _Champ *champ;
   int col;

   FldMgrGetChamp(&champ, 0);
   if (champ->domaine != VALEURS_PONCTUELLES)
      {
      if (8 <= c_wglgpl())
   col = xc.attributs[FOND].indCouleurFore;
      else
   col = xc.attributs[FOND].couleurFore;
      }
   else
      {
      col = BLANC;
      }

   c_wglsetw(fenetreAffichage);
   c_wglcol(col);
   c_wglclr();
   }


void EffacerFenetreCoupe()
{
   int fenetreCoupe;
   int col;

   if (CoupeMgrGetStatutCoupe())
      {
      if (8 <= c_wglgpl())
         col = xc.attributs[FOND].indCouleurFore;
      else
         col = BLANC;

      CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
      c_wglsetw(fenetreCoupe);
      c_wglcol(col);
      c_wglclr();
      }
   }

void EffacerFenetreSerie()
{
   int fenetreSerie;
   int col;

   if (SerieMgrGetStatutSerie())
      {
      if (8 <= c_wglgpl())
   col = xc.attributs[FOND].indCouleurFore;
      else
   col = BLANC;

      SerieMgrGetFenetreSerieID(&fenetreSerie);
      c_wglsetw(fenetreSerie);
      c_wglcol(col);
      c_wglclr();
      }
   }


/* ****************************************************************************** */

void InitDictionnaire()
{
   f77name(initrlx)();
   LireDictionnaireRMNLIB(infoChamps);
   LireDictionnaireUsager(infoChamps);
   LireStartRec();

   }


/* ****************************************************************************** */


void InitFormeMessage()
{
   int i;
   Arg args[10];
   XmString label;

   char tempStr[128];

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, xc.formeAnim); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
   xc.formeMessage = (Widget)XmCreateForm(xc.panneauContour, "forme", args, i);
   XtManageChild(xc.formeMessage);

   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 1); i++;
   XtSetArg(args[i], XmNrightPosition, 99); i++;
   xc.formeMessageFrame = (Widget)XmCreateFrame(xc.formeMessage,
                                             "label Frame",
                                             args, i);
   XtManageChild(xc.formeMessageFrame);


   strcpy(tempStr, "Normal");
   i = 0;
   label = XmStringCreateLtoR("Normal", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNalignment, XmALIGNMENT_BEGINNING); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNvalue, (char *)tempStr); i++;
   XtSetArg(args[i], XmNeditable, False); i++;
   xc.labelInfo = (Widget)XmCreateTextField(xc.formeMessageFrame, "TextField", args, i);
   XtManageChild(xc.labelInfo);

   i = 0;
   XtSetArg(args[i], XmNvalue, (char *)tempStr); i++;
   sprintf(tempStr, "%s", "Normal");
   XtSetValues(xc.labelInfo, args, i);

   XmStringFree(label);

   }

void InitFormeAffichage()
{
   int i;
   Arg args[10];

   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNnumColumns, 1); i++;
   XtSetArg(args[i], XmNisHomogeneous, False); i++;
   xc.menuform = (Widget)XmCreateMenuBar(xc.panneauContour,"menuform",args, i);
   XtManageChild(xc.menuform);

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, xc.menuform); i++;
/**   XtSetArg(args[i], XmNwidth, 400); i++;**/
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   xc.formeAff = (Widget)XmCreateForm(xc.panneauContour, "formeAffichage", args, i);
   XtManageChild(xc.formeAff);


   InitMenuFichier();
   CalculerHauteurMenus();
   InitMenuAffichage();
   InitMenuGrille();
   InitMenuVecteurs();
   InitMenuCalculs();
   InitMenuIntervalles();
   InitMenuOptions();

   }

/* ****************************************************************************** */

void InitFormeZoom()
{
   int i;
   Arg args[10];
   XmString label;

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, xc.menuform); i++;
/**   XtSetArg(args[i], XmNwidth, 400); i++;**/
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   xc.formeZoom = (Widget)XmCreateForm(xc.panneauContour, "formeZoom", args, i);
   XtManageChild(xc.formeZoom);


   i = 0;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightPosition, 33); i++;
   xc.zoom = (Widget)XmCreatePushButton(xc.formeZoom, labelZoom[lng], args, i);
   XtAddCallback(xc.zoom, XmNactivateCallback, Zoom, NULL);
   XtManageChild(xc.zoom);

   i = 0;
   label = XmStringCreateLtoR(labelValeurs[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 33); i++;
   XtSetArg(args[i], XmNrightPosition, 67); i++;
   xc.valeursPonctuelles = (Widget)XmCreatePushButton(xc.formeZoom, "pushbutton", args, i);
   XtAddCallback(xc.valeursPonctuelles, XmNactivateCallback, AfficherValeursAuxPtsDeGrille, NULL);
   XtManageChild(xc.valeursPonctuelles);
   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelStats[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 67); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   xc.stats = (Widget)XmCreatePushButton(xc.formeZoom, "pushbutton", args, i);
   XtAddCallback(xc.stats, XmNactivateCallback, AfficherStats, NULL);
   XtManageChild(xc.stats);
   XmStringFree(label);

   }

/* ****************************************************************************** */

void InitFormeAnimation()
{
   int i;
   Arg args[10];
   XmString label;


   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, xc.formeSup); i++;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
/**   XtSetArg(args[i], XmNwidth, 400); i++;**/
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   xc.formeAnim = (Widget)XmCreateForm(xc.panneauContour, "formeAffichage", args, i);
   XtManageChild(xc.formeAnim);

   i = 0;
   label = XmStringCreateLtoR("STOP", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 33); i++;
   XtSetArg(args[i], XmNrightPosition, 67); i++;
   xc.stop = (Widget)XmCreatePushButton(xc.formeAnim, "STOP", args, i);
   XmStringFree(label);
   XtAddCallback(xc.stop, XmNactivateCallback, PaStop, NULL);
   XtManageChild(xc.stop);

   }


/* ****************************************************************************** */

void InitFormeSuperposition()
{

   int i;
   Arg args[10];
   XmString label;

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
   XtSetArg(args[i], XmNtopWidget, xc.formeZoom); i++;
   XtSetArg(args[i], XmNfractionBase, 100); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   xc.formeSup = (Widget) XmCreateForm(xc.panneauContour, "form", args, i);
   XtManageChild(xc.formeSup);

   label = XmStringCreateLtoR(labelSuperpositionOn[lng], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 0); i++;
   XtSetArg(args[i], XmNrightPosition, 33); i++;
   xc.superposition = (Widget)XmCreatePushButton(xc.formeSup, "pushbutton", args, i);
   XtAddCallback(xc.superposition, XmNactivateCallback, ChangerStatutSuperposition, NULL);
   XtManageChild(xc.superposition);

   XmStringFree(label);

   i = 0;
   label = XmStringCreateLtoR(labelRafraichir[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNleftPosition, 33); i++;
   XtSetArg(args[i], XmNrightPosition, 67); i++;
   xc.rafraichir = (Widget)XmCreatePushButton(xc.formeSup, "pushbutton", args, i);
   XtAddCallback(xc.rafraichir, XmNactivateCallback, RafraichirFenetre, NULL);
   XtManageChild(xc.rafraichir);


   label = XmStringCreateLtoR(labelEffacer[lng], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNrubberPositioning, True); i++;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_POSITION); i++;
   XtSetArg(args[i], XmNleftPosition, 67); i++;
   XtSetArg(args[i], XmNrightPosition, 100); i++;
   xc.effacer = (Widget)XmCreatePushButton(xc.formeSup, "pushbutton", args, i);
   XtAddCallback(xc.effacer, XmNactivateCallback, Effacer, NULL);
   XtManageChild(xc.effacer);

   XmStringFree(label);


   }

/* ****************************************************************************** */

void InitMenuAffichage()
{
   int i, j;
   Arg  args[10];
   XmString label;

   label = XmStringCreate(labelMenuAffichage[lng], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuAffichage = (Widget)XmCreatePulldownMenu(xc.menuform,"Affichage",args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuAffichage); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuAff = (Widget)XmCreateCascadeButton(xc.menuform,"menuListe",args, i);

   XmStringFree(label);

   j = 0;

   for (i=0; i < NB_MENU_AFFICHAGE_ITEMS; i++)
      {
      j = 0;
      label = XmStringCreate(statutStr[i][lng], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[j], XmNheight, hauteurMenus+5); j++;
      XtSetArg(args[j], XmNrecomputeSize, False); j++;
      XtSetArg(args[j], XmNlabelString, label); j++;
      XtSetArg(args[j], XmNindicatorType, XmONE_OF_MANY); j++;

      xc.menuAffichageItems[i] = (Widget)XmCreateToggleButton(xc.menuAffichage, "item", args, j);
      XtAddCallback(xc.menuAffichageItems[i], XmNvalueChangedCallback, MenuAffichageSelect, NULL);
      XmStringFree(label);
      }

   XtManageChildren(xc.menuAffichageItems, NB_MENU_AFFICHAGE_ITEMS);
   XtManageChild(xc.menuAff);

   if (4 > c_wglgpl())
      {
      XtUnmanageChild(xc.menuAffichageItems[COULEURS]);
      XtUnmanageChild(xc.menuAffichageItems[LEGENDE_COULEUR]);
      XtUnmanageChild(xc.menuAffichageItems[LISSAGE]);
      }

   }

/* ****************************************************************************** */

void InitMenuGrille()
{
   int i, j;
   Arg  args[10];
   XmString label;
   extern int nbgi;
   extern _GrilleMenuItem grmenuitems[];
   static char *labelChampno1[] = { "Champ #1", "Field #1" };


   i = 0;
   label = XmStringCreate(labelMenuGrille[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuGrille = (Widget)XmCreatePulldownMenu(xc.menuform,"Grille",args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuGrille); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuGr = (Widget)XmCreateCascadeButton(xc.menuform,"menuListe",args, i);

   XmStringFree(label);

   i = 0;
   j = 0;
   label = XmStringCreate(labelChampno1[lng], XmSTRING_DEFAULT_CHARSET);
#if defined (HP)
   XtSetArg(args[j], XmNrecomputeSize, True); j++;
#else
   XtSetArg(args[j], XmNheight, hauteurMenus+5); j++;
   XtSetArg(args[j], XmNrecomputeSize, False); j++;
#endif
   XtSetArg(args[j], XmNindicatorType, XmONE_OF_MANY); j++;
   XtSetArg(args[j], XmNlabelString, label); j++;
   XtSetArg(args[j], XmNset, True); j++;
   xc.menuGrItems[i] = (Widget)XmCreateToggleButton(xc.menuGrille, "item", args, j);
   XtAddCallback(xc.menuGrItems[i], XmNvalueChangedCallback, MenuGrilleSelect, (XtPointer) (long)i);
   XmStringFree(label);


   for (i=1; i < nbgi+1; i++)
      {
      j = 0;
      label = XmStringCreate(grmenuitems[i].nomgrille, XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[j], XmNheight, hauteurMenus+5); j++;
      XtSetArg(args[j], XmNrecomputeSize, False); j++;
      XtSetArg(args[j], XmNlabelString, label); j++;
      XtSetArg(args[j], XmNindicatorType, XmONE_OF_MANY); j++;
      xc.menuGrItems[i] = (Widget)XmCreateToggleButton(xc.menuGrille, "item", args, j);
      XtAddCallback(xc.menuGrItems[i], XmNvalueChangedCallback, MenuGrilleSelect, (XtPointer)(long) i);
      XmStringFree(label);
      }

   XtManageChildren(xc.menuGrItems, nbgi);
   XtManageChild(xc.menuGr);

}
/* ****************************************************************************** */

void InitMenuVecteurs()
{
   int i, j;
   Arg  args[10];
   XmString label;

   i = 0;
   label = XmStringCreate(labelMenuVecteurs[lng], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuVecteurs = (Widget)XmCreatePulldownMenu(xc.menuform,"Vecteurs",args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuVecteurs); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuVec = (Widget)XmCreateCascadeButton(xc.menuform,"menuListe",args, i);

   XmStringFree(label);

   for (i=0; i < 3; i++)
     {
     j = 0;
     label = XmStringCreate(VecsStr[i][lng], XmSTRING_DEFAULT_CHARSET);
     XtSetArg(args[j], XmNheight, hauteurMenus+5); j++;
     XtSetArg(args[j], XmNrecomputeSize, False); j++;
     XtSetArg(args[j], XmNlabelString, label); j++;
     XtSetArg(args[j], XmNindicatorType, XmONE_OF_MANY); j++;
     xc.menuVecItems[i] = (Widget)XmCreateToggleButton(xc.menuVecteurs, "item", args, j);
     XtAddCallback(xc.menuVecItems[i], XmNvalueChangedCallback, MenuVecteurSelect, (XtPointer)(long) i);
     XmStringFree(label);
     }

   XtManageChildren(xc.menuVecItems, 3);
   XtManageChild(xc.menuVec);

   XtSetArg(args[0], XmNset, True);
   XtSetValues(xc.menuVecItems[2], args, 1);


}

/* ****************************************************************************** */

void InitMenuFichier()
{
   int i, j;
   Arg  args[10];

   static char *labelRecAPropos[] = {"A propos de xrec...", "About xrec..."};

   i = 0;
   xc.menuFichier = (Widget)XmCreatePulldownMenu(xc.menuform,labelMenuFichier[lng],args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuFichier); i++;
   xc.menuFich = (Widget)XmCreateCascadeButton(xc.menuform,labelMenuFichier[lng],args, i);

   i = 0;
   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelRecAPropos[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer)(long) INFO_REC);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreateSeparator(xc.menuFichier, "ligne", args, j);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelOuvrir[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer) OUVRIR);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelFermer[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer) FERMER);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreateSeparator(xc.menuFichier, "ligne", args, j);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelDiapos[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer) DIAPOS);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelFilms[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer) FILMS);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreateSeparator(xc.menuFichier, "ligne", args, j);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelQuitter[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer) QUITTER);
   i++;

   j = 0;
   xc.menuFichierItems[i] = (Widget)XmCreatePushButton(xc.menuFichier, labelQuitterEtSauver[lng], args, j);
   XtAddCallback(xc.menuFichierItems[i], XmNactivateCallback, MenuFichierSelect, (XtPointer) QUITTER_ET_SAUVER);
   i++;

   XtManageChildren(xc.menuFichierItems, i);
   XtManageChild(xc.menuFich);


   }

/* ****************************************************************************** */

void InitMenuIntervalles()
{
   int i, j;
   Arg  args[10];
   char *item;
   XmString label;

   label = XmStringCreateLtoR(labelMenuIntervalle[lng], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuIntervalleDeContour = (Widget)XmCreatePulldownMenu(xc.menuform,"Fichier",args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuIntervalleDeContour); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuInt = (Widget)XmCreateCascadeButton(xc.menuform,"menuListe",args, i);



   j = 0;
   for (i=0; i < (int)(XtNumber(menuItemFacteurMult)); i++)
      {
      item = menuItemFacteurMult[i];
      label = XmStringCreate(item, XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[0], XmNlabelString, label);
      xc.menuIntervalleItems[i] = (Widget)XmCreatePushButton(xc.menuIntervalleDeContour, "menu", args,1);
      XtAddCallback(xc.menuIntervalleItems[i], XmNactivateCallback, MenuIntervalleSelect, (XtPointer)(long) i);
      XmStringFree(label);
      }

   XtManageChildren(xc.menuIntervalleItems, (int)(XtNumber(menuItemFacteurMult)));
   XtManageChild(xc.menuInt);
   }


/* ****************************************************************************** */

void InitMenuCalculs()
{
   int i, j;
   Arg  args[10];
   XmString label;

   label = XmStringCreateLtoR(labelMenuCalculs[lng], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuCalculs = (Widget)XmCreatePulldownMenu(xc.menuform,"Calcul.",args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuCalculs); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuSup = (Widget)XmCreateCascadeButton(xc.menuform,"menuListe",args, i);

   XmStringFree(label);

   i = 0;
   j = 0;
   XtSetArg(args[j], XmNindicatorType, XmONE_OF_MANY); j++;
   XtSetArg(args[j], XmNset, True); j++;
   xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsNoOp[lng], args, j);
   XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 0);

   /*   i++;
  xc.menuCalculItems[i] = (Widget)XmCreateSeparator(xc.menuCalculs, "ligne", args, 0);
   */

   i++;
   XtSetArg(args[j], XmNset, False);j++;
   xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsSubtract[lng], args, j);
   XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 1);

   i++;
   xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsAdd[lng], args, j);
   XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 2);

   /*
     i++;
     xc.menuCalculItems[i] = (Widget)XmCreateSeparator(xc.menuCalculs, "ligne", args, 0);
   */

   i++;
   xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsAbsSubtract[lng], args, j);
   XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 3);

   i++;
   xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsAbsAdd[lng], args, j);
   XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 4);

   /*
  i++;
  xc.menuCalculItems[i] = (Widget)XmCreateSeparator(xc.menuCalculs, "ligne", args, 0);

  i++;
  xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculModule[lng], args, j);
  XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 5);

  i++;
  xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsSubModule[lng], args, j);
  XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 6);

  i++;
  xc.menuCalculItems[i] = (Widget)XmCreateToggleButton(xc.menuCalculs, labelMenuCalculsAddModule[lng], args, j);
  XtAddCallback(xc.menuCalculItems[i], XmNvalueChangedCallback, MenuCalculSelect, (XtPointer) 7);
   */

   i++;
   XtManageChildren(xc.menuCalculItems, i);
   XtManageChild(xc.menuSup);

   }


/* ****************************************************************************** */

void InitMenuOptions()
{
   int i, j;
   Arg  args[10];
   XmString label;

   label = XmStringCreate("Options", XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuOptions = (Widget)XmCreatePulldownMenu(xc.menuform,"Options",args, i);

   i = 0;
   XtSetArg(args[i], XmNsubMenuId, xc.menuOptions); i++;
   XtSetArg(args[i], XmNlabelString, label); i++;
   xc.menuOpt = (Widget)XmCreateCascadeButton(xc.menuform,"menuListe",args, i);

   XmStringFree(label);

   i = 0;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelAttributs[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelContours[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelGeographie[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);


   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelPalette[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback,  MenuOptionSelect, (XtPointer)(long) i);

   i++;
   xc.menuOptionItems[i] = (Widget)XmCreateSeparator(xc.menuOptions, "ligne", args, 0);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelMinMax[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelVent[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelValeursPonctuelles[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   xc.menuOptionItems[i] = (Widget)XmCreateSeparator(xc.menuOptions, "ligne", args, 0);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelEdition[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   xc.menuOptionItems[i] = (Widget)XmCreateSeparator(xc.menuOptions, "ligne", args, 0);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelAnimation[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelSerie[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);

   i++;
   xc.menuOptionItems[i] = (Widget)XmCreateSeparator(xc.menuOptions, "ligne", args, 0);

   i++;
   j = 0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelCoupe[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);
   i++;
   j=0;
   xc.menuOptionItems[i] = (Widget)XmCreatePushButton(xc.menuOptions, labelAnimationVerticale[lng], args, j);
   XtAddCallback(xc.menuOptionItems[i], XmNactivateCallback, MenuOptionSelect, (XtPointer)(long) i);



   XtManageChildren(xc.menuOptionItems, i+1);
   XtManageChild(xc.menuOpt);
   }


/* ****************************************************************************** */

void InitOldMapInfo()
{
   oldMapInfo.ni = -1;
   oldMapInfo.nj = -1;
   oldMapInfo.ig1 = -1;
   oldMapInfo.ig2 = -1;
   oldMapInfo.ig3 = -1;
   oldMapInfo.ig4 = -1;

   }


/* ****************************************************************************** */


/** ARGSUSED **/
void MenuOptionSelect(Widget w, XtPointer unused1, XtPointer unused2)
{

   switch((long)unused1)
      {
      case 0:
      ActiverPanneauAttributs();
      break;

      case 1:
      ActiverPanneauContour();
      break;

      case 2:
      ActiverPanneauGeo();
      break;

      case 3:
      ActiverPanneauPalette();
      break;

      case 5:
      ActiverPanneauMinMax();
      break;

      case 6:
      ActiverPanneauVents();
      break;

      case 7:
      ActiverPanneauValeursPonctuelles();
      break;

      case 9:
      ActiverPanneauEdition();
      break;

      case 11:
      ActiverPanneauAnimation();
      break;

      case 12:
      ActiverPanneauSerie();
      break;

      case 14:
      ActiverPanneauCoupe();
      break;

      case 15:
      ActiverPanneauAnimationVerticale();
      break;

      default:
  break;
      }
   return ;
   }

/* ****************************************************************************** */

/** ARGSUSED **/
void MenuGrilleSelect(Widget w, XtPointer unused1, XtPointer unused2)
{
   int i, gdin, gdout;
   Arg args[10];
   extern int nbgi;
   int largeurFenetre, hauteurFenetre;
   extern _GrilleMenuItem grmenuitems[];
   float rx1, rx2, ry1, ry2;
   int drawcode;
   _Champ *champ;

   int c_ezgetgdin();

   FldMgrGetChamp(&champ, 0);
   if (champ->domaine == XZ || champ->domaine == YZ)
     {
       Beeper();
       return ;
     }

   drawcode = (long) unused2;

   for (i=0; i < nbgi+1; i++)
      {
      XtSetArg(args[0], XmNset, False);
      XtSetValues(xc.menuGrItems[i], args, 1);
      }

   XtSetArg(args[0], XmNset, True);
   XtSetValues(xc.menuGrItems[(long)unused1], args, 1);


   i = (long)unused1;
   itemGrilleSelectionnee = i;

   if (i == 0)
      {
      FldMgrGetChamp(&champ, 0);
      FldMgrGetFstPrm(champ);
      gdin = c_ezgdefrec(champ->src.ni, champ->src.nj, champ->src.grtyp,
                  champ->src.ig1, champ->src.ig2, champ->src.ig3, champ->src.ig4);
      FldMgrDefinirGrille();
      c_ezdefset(gdin, gdin);
      }
   else
      {
      gdin = c_ezgetgdin();
      gdout = c_ezgdefrec(grmenuitems[i].ni, grmenuitems[i].nj, grmenuitems[i].grtyp,
                          grmenuitems[i].ig1, grmenuitems[i].ig2, grmenuitems[i].ig3, grmenuitems[i].ig4);
      InitMapInfo(grmenuitems[i].grtyp[0], grmenuitems[i].ni, grmenuitems[i].nj,
                  grmenuitems[i].ig1,  grmenuitems[i].ig2, grmenuitems[i].ig3,  grmenuitems[i].ig4);
      if (gdin == -1)
         {
         c_ezdefset(gdout, gdout);
         }
      else
         {
         c_ezdefset(gdout, gdin);
         }
      }

   InitMapInfo(mapInfo.type, mapInfo.ni, mapInfo.nj, mapInfo.ig1, mapInfo.ig2,
               mapInfo.ig3, mapInfo.ig4);
   mapFlags.verifStatutNecessaire = OUI;
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   AjusterViewport(&viewp);
   c_xy2fxfy(&rx1, &ry1, 1.0, 1.0);
   c_xy2fxfy(&rx2, &ry2, (float)mapInfo.ni, (float)mapInfo.nj);
   c_wglssp(rx1, ry1, rx2, ry2, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
   for (i=0; i <  FldMgrGetNbChampsActifs(); i++)
     {
       FldMgrGetChamp(&champ, i);
       FldMgrFreeAllFlds(champ);
       FldMgrGetFstPrm(champ);
       FldMgrProcessChamp(champ);
       if (champ->coupe.montagnes)
        {
        free(champ->coupe.montagnes);
        champ->coupe.montagnes = NULL;
        }
       FldMgrPreparerTopo();
     }

   AfficherMessageInfoStandard();
   if (drawcode != 9999)
     {
       RedessinerFenetres();
     }
   MettreAJourStats();
   return ;
}

/* ****************************************************************************** */
/** ARGSUSED **/
void MenuVecteurSelect(Widget w, XtPointer unused1, XtPointer unused2)
{
  int i;
  Arg args[10];
  int currentOpt,ier,fenetreCoupe, fenetreSerie;
  float cxmin, cymin, cxmax, cymax;
  _Champ *champ;

  currentOpt = xc.statuts[TRAITEMENT_VECTORIEL];

  for (i=0; i < 3; i++)
      {
      XtSetArg(args[0], XmNset, False);
      XtSetValues(xc.menuVecItems[i], args, 1);
      }

  XtSetArg(args[0], XmNset, True);
  XtSetValues(xc.menuVecItems[(long)unused1], args, 1);

  switch ((long)unused1)
    {
    case 0:
      xc.statuts[TRAITEMENT_VECTORIEL] = FALSE;
      break;

    case 1:
      xc.statuts[TRAITEMENT_VECTORIEL] = TRUE;
      xc.statuts[BARBULES] = FALSE;
      break;

    case 2:
      xc.statuts[TRAITEMENT_VECTORIEL] = TRUE;
      xc.statuts[BARBULES] = TRUE;
      break;
    }

  if (xc.statuts[TRAITEMENT_VECTORIEL] != currentOpt)
    {
    for (i=0; i <  FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&champ, i);
      FldMgrFreeAllFlds(champ);
      FldMgrGetFstPrm(champ);
      FldMgrProcessChamp(champ);
      }
    }

  ier = CoupeMgrGetStatutCoupe();
  if (ier == TRUE)
    {
    CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
    c_wglsetw(fenetreCoupe);
    CoupeMgrGetCoupeCoords(&cxmin, &cymin, &cxmax, &cymax);
    ier = PreparerCoupe(cxmin, cymin, cxmax, cymax);
    }

  if (ier == TRUE)
    {
    AfficherLigneCoupe();
    CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
    c_wglsetw(fenetreCoupe);
    RedessinerFenetreCoupe();

    GetFenetreAffichageID(&fenetreAffichage);
    c_wglsetw(fenetreAffichage);
    }

  ier = SerieMgrGetStatutSerie();
  if (ier == TRUE)
    {
    SerieMgrGetFenetreSerieID(&fenetreSerie);
    c_wglsetw(fenetreSerie);
    SerieMgrGetSerieCoords(&cxmin, &cymin, &cxmax, &cymax);
    ier = PreparerSerie(cxmin, cymin, cxmax, cymax);
    }

  if (ier == TRUE)
    {
    AfficherLigneSerie();
    SerieMgrGetFenetreSerieID(&fenetreSerie);
    c_wglsetw(fenetreSerie);
    RedessinerFenetreSerie();

    GetFenetreAffichageID(&fenetreAffichage);
    c_wglsetw(fenetreAffichage);
    }
  RedessinerFenetres();
  AfficherMessageInfoStandard();

  if (xc.statuts[EN_TRAIN_DE_DESSINER])
    {
    Beeper();
    return ;
    }
MettreAJourStats();
return ;
}

/* ****************************************************************************** */


/** ARGSUSED **/
void MenuCalculSelect(Widget w, XtPointer unused1, XtPointer unused2)
{
  int i;
  Arg args[10];

   for (i=0; i < 5; i++)
      {
      XtSetArg(args[0], XmNset, False);
      XtSetValues(xc.menuCalculItems[i], args, 1);
      }

   XtSetArg(args[0], XmNset, True);
   XtSetValues(xc.menuCalculItems[(long)unused1], args, 1);

   CtrlMgrSetMathOp((long)unused1);
   LibererImages();
   RedessinerFenetres();
   return ;
   }

void wglmapc(int colorIndex, int r, int g, int b);

/** ARGSUSED **/
void MenuAffichageSelect(Widget w, XtPointer unused1, XtPointer unused2)
{
   int i;
   char *text;
   XmString label;
   Arg args[10];
   _Champ *champ;

   XtSetArg(args[0], XmNlabelString, &label);
   XtGetValues(w, args, 1);
   XmToS(label, &text);

   i =0;
   while (0 != strcmp(text, statutStr[i][lng]))
      i++;

   switch(xc.statuts[i])
      {
      case TRUE:
      xc.statuts[i] = FALSE;
      break;

      case FALSE:
      xc.statuts[i] = TRUE;
      break;
      }

   switch(i)
      {
      case COULEURS:
      case LEGENDE_COULEUR:
      switch(xc.statuts[COULEURS])
         {
         case TRUE:
         wglmapc(14, 64, 64, 64);
         EffacerFenetres();
         break;

         case FALSE:
         wglmapc(14, 128, 128, 128);
         break;
         }

      break;

      case ZOOM_LOCAL:
      break;

      case TOPO:
      break;

      default:
        break;
      }

   switch(i)
      {
      case LEGENDE:
        FldMgrGetChamp(&champ, 0);
        if (xc.statuts[LEGENDE_COULEUR] && xc.statuts[COULEURS] && champ->natureTensorielle == SCALAIRE)
          {
          EffacerLegende2();
          if (xc.statuts[LEGENDE])
            {
            AfficherLegende2(*champ);
            }
          AfficherPerimetreFenetre();
          }
        else
          {
          RedessinerFenetres();
          }
       break;

      case AFF_AUTOMATIQUE:
      break;

      default:
      if (xc.statuts[AFF_AUTOMATIQUE])
        {
        RedessinerFenetres();
        }
      break;
      }

   LibererImages();
   ActiverFlagsAffichage();
   return ;
   }

/* ****************************************************************************** */

/** ARGSUSED **/
void MenuFichierSelect(Widget w, XtPointer client_data, XtPointer call_data)
{
   int lng;
   int i, largeurFenetre, hauteurFenetre;
   char inforec[1024];

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return ;
      }

   lng = c_getulng();

   switch((long)client_data)
      {
      case INFO_REC:
      if (GetLogoToggle())
   {
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   for (i=hauteurFenetre-1; i >= 0; i--)
      {
      c_wglcol(recColorTable[(int)(sizeRecColorTable* (1.0-(float)i/((float)(hauteurFenetre))))]);
      c_wglmvi(0, i);
      c_wgldri(largeurFenetre-1, i);
      }

   draw_rpn_logo();
         draw_rec_version();
   }


      strcpy(inforec, PROJECT_VERSION_STRING);
      strcat(inforec, copyright[lng]);
      strcat(inforec, auteurs[lng]);
      strcat(inforec, contrib[lng]);
      MessageAvertissement(inforec, INFO);
      break;

      case OUVRIR:
      Effacer(xc.effacer, NULL, NULL);
      xc.statut = OUVRIR;
      break;

      case FERMER:
      Effacer(xc.effacer, NULL, NULL);
      xc.statut = FERMER;
      break;

      case DIAPOS:
      ActiverPanneauDiapos();
/**
      ExportImage();
      Effacer(xc.effacer, NULL, NULL);
      QuitAndSave(w, client_data, call_data);
**/
      break;

      case FILMS:
      ExportMovie();
      break;

      case QUITTER:
      Effacer(xc.effacer, NULL, NULL);
      Quit(w, client_data, call_data);
      break;

      case QUITTER_ET_SAUVER:
      Effacer(xc.effacer, NULL, NULL);
      QuitAndSave(w, client_data, call_data);
      break;
      }
   return ;
   }

/* ****************************************************************************** */

/** ARGSUSED **/
void MenuIntervalleSelect(Widget w, XtPointer client_data, XtPointer call_data)
{
   float FacteurMultDesire;
   int j,n;
   char *text;
   XmString label;
   Arg args[10];
   _Champ *champ, *champ2;

   FldMgrGetChamp(&champ, FldMgrGetNbChampsActifs() - 1);

   XtSetArg(args[0], XmNlabelString, &label);
   XtGetValues(w, args, 1);
   XmToS(label, &text);

   if (champ->indDict >= 0)
      {
      infoChamps[champ->indDict].indIntervalleDeDefaut = (long)client_data;
      champ->nbIntervalles = infoChamps[champ->indDict].nbIntervalles[(long)client_data];
      for (n=0; n < infoChamps[champ->indDict].nbIntervalles[(long)client_data]; n++)
          champ->intervalles[n] = infoChamps[champ->indDict].intervallesDeContour[(long)client_data][n];

      for (j=0; j < FldMgrGetNbChampsActifs(); j++)
   {
   FldMgrGetChamp(&champ2, j);
         if (0 == strcmp(champ2->nomvar, champ->nomvar))
      {
      UpdateFld_IntervalleDeContour((_Champ *)champ2, champ->intervalles, champ->nbIntervalles);
      }
   }
      }
   else
      {
      sscanf(text, "%f", &FacteurMultDesire);
      for (j=0; j < FldMgrGetNbChampsActifs(); j++)
   {
   FldMgrGetChamp(&champ2, j);
         if (0 == strcmp(champ2->nomvar, champ->nomvar))
             UpdateFld_IntervalleDeContour((_Champ *)champ2, champ->intervalles, 1);
   }
      }

   ChangerMenuIntervalles((long)client_data);
   LibererImages();
   RedessinerFenetres();
   return ;
   }


/* ****************************************************************************** */


/** ARGSUSED **/
void Quit(Widget w, XtPointer client_data, XtPointer call_data)
{
#ifdef GL_WGL
   EnterOverlayMode();
   color(0);
   clear();
   RestoreNormalMode();
#endif
   XtDestroyApplicationContext(XtWidgetToApplicationContext(w));
   c_wgldcm();
   exit(0);

   }

/* ****************************************************************************** */

/** ARGSUSED **/
void QuitAndSave(Widget w, XtPointer client_data, XtPointer call_data)
{
   FILE *fichierDemarrage;
   char *tmp;
   char nomFichierDemarrage[128];
   int lng;

   lng = c_getulng();
   tmp = (char *) getenv("HOME");
   if (tmp == NULL)
      {
      return ;
      }

   strcpy(nomFichierDemarrage, tmp);

   strcat(nomFichierDemarrage, "/.startrec");
   fichierDemarrage = fopen(nomFichierDemarrage, "w");
   if (fichierDemarrage == NULL)
      {
      if (lng == 0)
   fprintf(stderr,"Impossible d'ouvrir le fichier $HOME/.startrec... \n");
      else
   fprintf(stderr,"Can't open file $HOME/.startrec... \n");
      return ;
      }

   EcrCtlAtr(fichierDemarrage);
   EcrAtrAtr(fichierDemarrage);
   EcrPalAtr(fichierDemarrage);
   EcrConAtr(fichierDemarrage);
   EcrGeoAtr(fichierDemarrage);

   fclose(fichierDemarrage);

   Quit(w, client_data, call_data);
   return ;
   }

/* ****************************************************************************** */

void RedessinerFenetres()
{

   RedessinerFenetreAffichage();
   RedessinerFenetreCoupe();
   RedessinerFenetreSerie();
   }

/* ****************************************************************************** */

void RedessinerFenetreAffichage()
{
   int i;

   xc.annulationDemandee = False;
   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      if (!c_wglanul())
         AfficherCarte(i);
      }

   AfficherLigneCoupe();
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   RafraichirStats();
   EnleverBoutonAnnulation();
   }


/* ****************************************************************************** */

void RedessinerFenetreCoupe()
{
   int i;

   if (!CoupeMgrGetStatutCoupe())
      return;

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }

   AfficherBoutonAnnulation();

   CoupeMgrSetMinMax();
   CoupeMgrSetUVWMinMax();
   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      if (!c_wglanul())
        {
        ManipulerEtAfficherCoupeVerticale(i);
        }
      }

   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   EnleverBoutonAnnulation();
   }

/* ****************************************************************************** */

void RedessinerFenetreSerie()
{
   int i;

   if (!SerieMgrGetStatutSerie())
      return;

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return;
      }

   AfficherBoutonAnnulation();

   SerieMgrSetMinMaxSerie();
   SerieMgrSetUVWMinMax();
   xc.statuts[EN_TRAIN_DE_DESSINER] = TRUE;
   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      if (!c_wglanul())
         {
         ManipulerEtAfficherSerieTemporelle(i);
         }
      }

   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   EnleverBoutonAnnulation();
   }

/* ****************************************************************************** */

/** ARGSUSED **/
void RafraichirFenetre(Widget w, XtPointer unused1, XtPointer unused2)
{

   if (xc.statuts[EN_TRAIN_DE_DESSINER])
      {
      Beeper();
      return ;
      }

   InvertWidget(w);
   EffacerFenetres();
   RedessinerFenetres();
   InvertWidget(w);
   return ;
   }

/* ****************************************************************************** */

int f77name(xconact)(int recs[], int *nbrecs, int *iun)
{
   Widget widgetParent;
   int largeurFenetre, hauteurFenetre;
   float rx1, ry1, rx2, ry2;
   float cxmin, cxmax, cymin, cymax;
   int fenetreAffichage, fenetreCoupe,fenetreSerie;
   int n,ier;
   int op;
   int indChampCourant,nbChampsInitiaux;
   _Champ *champCourant;

   xc.flagInterrupt = TRUE;
   nbChampsInitiaux = FldMgrGetNbChampsActifs();

   if (*nbrecs > 0)
      {
      for (n=0; n < *nbrecs; n++)
   {
   if (xc.statutSuperposition == TRUE)
      {
      ier = FldMgrAddChamp(recs[n], *iun);
      RafraichirStats();

      if (ier == CHAMP_NON_SUPERPOSABLE)
         {
         if (1 == FldMgrGetNbChampsActifs())
      MessageAvertissement(lblAvrtSup1[lng], AVERTISSEMENT);
         else
      MessageAvertissement(lblAvrtSup2[lng], AVERTISSEMENT);

         xc.statut = TERMINE;
         return NOUVEAU_CHAMP;
         }

      if (ier == TROP_DE_CHAMPS)
         {
         MessageAvertissement(lblAvrtTrop[lng], AVERTISSEMENT);
         xc.statut = TERMINE;
         return NOUVEAU_CHAMP;
         }

      }
   else
      {
            ier = FldMgrReplaceChamp(recs[n], *iun);
      if (ier == CHAMP_MODIFIE)
         {
         RefuserOperation();
         return NOUVEAU_CHAMP;
         }
       RafraichirStats();

            }

         ier = CoupeMgrGetStatutCoupe();
         if (ier == TRUE)
            {
            CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
            c_wglsetw(fenetreCoupe);
            CoupeMgrGetCoupeCoords(&cxmin, &cymin, &cxmax, &cymax);
            ier = PreparerCoupe(cxmin, cymin, cxmax, cymax);

            if (ier > 0)
              return NOUVEAU_CHAMP;

            GetFenetreAffichageID(&fenetreAffichage);
            c_wglsetw(fenetreAffichage);
            }

         ier = SerieMgrGetStatutSerie();
         if (ier == TRUE)
            {
            SerieMgrGetFenetreSerieID(&fenetreSerie);
            c_wglsetw(fenetreSerie);
            SerieMgrGetSerieCoords(&cxmin, &cymin, &cxmax, &cymax);
            ier = PreparerSerie(cxmin, cymin, cxmax, cymax);

            if (ier > 0)
              return NOUVEAU_CHAMP;

            GetFenetreAffichageID(&fenetreAffichage);
            c_wglsetw(fenetreAffichage);
            }

         LibererImages();
         indChampCourant = FldMgrGetNbChampsActifs() - 1;
         FldMgrGetChamp(&champCourant, indChampCourant);

         if (champCourant->indDict >= 0)
           CheckMenuIntervalles(infoChamps[champCourant->indDict].indIntervalleDeDefaut);

         if (HasGridChanged(champCourant->dst.grtyp[0], champCourant->dst.ni, champCourant->dst.nj, 1,
                            champCourant->dst.ig1, champCourant->dst.ig2,
                            champCourant->dst.ig3, champCourant->dst.ig4))
             {
             xc.carteLue = FALSE;
             EffacerFenetreAffichage();
             InitMapInfo(champCourant->dst.grtyp[0], champCourant->dst.ni, champCourant->dst.nj,
                         champCourant->dst.ig1, champCourant->dst.ig2,
                         champCourant->dst.ig3, champCourant->dst.ig4);
             c_wglgwz(&largeurFenetre, &hauteurFenetre);
             AjusterViewport(&viewp);
             c_xy2fxfy(&rx1, &ry1, 1.0, 1.0);
             c_xy2fxfy(&rx2, &ry2, (float)mapInfo.ni, (float)mapInfo.nj);
             c_wglssp(rx1, ry1, rx2, ry2, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
       if (champCourant->domaine == XY)
         {
     FldMgrDefinirGrille();
         }
             }

         if (champCourant->domaine == XZ || champCourant->domaine == YZ)
            {
            xc.statuts[GEOGRAPHIE] = FALSE;
            }

         xc.statut = OK;
         xc.ChampAContourer = TRUE;
         xc.annulationDemandee = FALSE;
         }

      op = CtrlMgrGetMathOp();
      if (op == NO_OP)
         {
         indChampCourant = nbChampsInitiaux;
         }
      else
         {
         indChampCourant = 0;
         }

      if (xc.statutSuperposition == 0)
         {
         indChampCourant = 0;
         }

      for (n=indChampCourant; n < FldMgrGetNbChampsActifs(); n++)
         {
         AfficherBoutonAnnulation();
         AfficherCarte(indChampCourant);
         EnleverBoutonAnnulation();
         indChampCourant++;
         }

      ier = CoupeMgrGetStatutCoupe();
      if (ier == TRUE)
         {
         AfficherLigneCoupe();
         CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
         c_wglsetw(fenetreCoupe);
         RedessinerFenetreCoupe();

         GetFenetreAffichageID(&fenetreAffichage);
         c_wglsetw(fenetreAffichage);
         }


      ier = SerieMgrGetStatutSerie();
      if (ier == TRUE)
         {
         AfficherLigneSerie();
         SerieMgrGetFenetreSerieID(&fenetreSerie);
         c_wglsetw(fenetreSerie);
         RedessinerFenetreSerie();

         GetFenetreAffichageID(&fenetreAffichage);
         c_wglsetw(fenetreAffichage);
         }
      AfficherMessageInfoStandard();
      }
   else
      {
      AfficherMessageInfoStandard();
      xc.statut = OK;
      }

   while (xc.statut == OK)
      {
      XtAppPeekEvent(SuperWidget.contexte, &(xc.theEvent));

      if (XtWindowToWidget(XtDisplay(xc.topLevel), xc.theEvent.xany.window) != NULL)
        widgetParent = TrouverWidgetParent(xc.theEvent.xany.window);
      else
        widgetParent = xc.topLevel;
      if (widgetParent == xc.topLevel || xc.statuts[EN_TRAIN_DE_DESSINER])
         {
         XtAppNextEvent(SuperWidget.contexte, &(xc.theEvent));
         XtDispatchEvent(&(xc.theEvent));
         }
      else
         {
   oldMapInfo = mapInfo;
         xc.lastColorMap   = xc.colorMap;
         return NOUVEAU_CHAMP;
         }

      }

   CopierGridInfo(&oldMapInfo, &mapInfo);
   xc.lastColorMap   = xc.colorMap;

   return xc.statut;


   }


/* ****************************************************************************** */

void init_plot88();
void c_gmpinit();

int f77name(xconouv)(int *iun)
{
   int i;
   Arg args[10];

   static char *titreFenetre[] = {"Affichage", "Display"};
   static char *titreFenetreControle[] =         { "TableauDeBord", "ControlPanel" };
   char nomShell[128];
   int largeurLogo, hauteurLogo, largeurFenetre, hauteurFenetre;

   char grtyp[2];
   int ni, nj, grille_bidon, ig1, ig2, ig3, ig4;
   float xg1, xg2, xg3, xg4;

   int indMin, indMax;

   lng = c_getulng();
   c_wglscon("x");
   strcpy(panneauContourGeometrie,"");
   xc.ChampAContourer = FALSE;
   xc.statut = OK;
   xc.statuts[LISSAGE] = FALSE;
   xc.statuts[COULEURS] = TRUE;
   xc.statuts[CONTOURS] = TRUE;
   xc.statuts[VALEURS_CENTRALES] = FALSE;
   xc.statuts[GEOGRAPHIE] = TRUE;
   xc.statuts[BARBULES] = TRUE;
   xc.statuts[LEGENDE] = TRUE;
   xc.statuts[AFF_AUTOMATIQUE] = TRUE;
   xc.statuts[EFF_FENETRE] = TRUE;
   xc.statuts[LABELS] = TRUE;
   xc.statuts[GRILLE_SOURCE] = FALSE;
   xc.statuts[GRILLE] = FALSE;
   xc.statuts[LEGENDE_COULEUR] = TRUE;
   xc.statuts[VALEURS_MANQUANTES] = FALSE;
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   xc.statuts[TRAITEMENT_VECTORIEL] = TRUE;
   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, titreFenetre[lng]);

   recCmap.noPalette = 0;
   recCmap.amplificationMin = 0.0;
   recCmap.reductionMax = 0.0;

   init_plot88();
   FldMgrInitChamps();
   CtrlMgrSetMathOp(NO_OP);

/*   GRILLE('Canada    - PS100km', PS, 71,51, 24.3,  56.3, 100000.0, 21.0, NORD) */
   xg1 = 24.3;
   xg2 = 56.3;
   xg3 = 100000.0;
   xg4 = 21.0;
   ni = 71;
   nj = 51;
   strcpy(grtyp, "N");
   f77name(cxgaig)(grtyp, &ig1, &ig2, &ig3, &ig4, &xg1, &xg2, &xg3, &xg4,(F2Cl) strlen(grtyp));
   grille_bidon = c_ezqkdef(ni, nj, grtyp, ig1, ig2, ig3, ig4, 0);

   SetGeometrieFenetreAffichage(nomShell);
   fenetreAffichage = c_wglopw(nomShell);
   c_wgldbf();
   c_wglfbf();
   InitAttributs();

/*
   un = 1;
   f77name(qqqintx)(&un);
*/

#ifdef GL_WGL
   winconstraints();
#endif


   c_gmpinit();
   PgSetResolution(50);
   PgSetMeridiens(OUI);
   PgSetEspacementMeridiens(10);
   PgSetFrontieresActives(OUI);
   c_wglgetcolrange(&indMin, &indMax);
   /*
     if (8 <= c_wglgpl())
     {
     PgSetCouleurGeographie(GRIS);
     }
     else
     {
     if (4 <= c_wglgpl())
     {
         PgSetCouleurGeographie(15);
   }
   else
   {
   PgSetCouleurGeographie(NOIR);
   }
   }

   */
   InitDictionnaire();
   if (4 > c_wglgpl())
      {
      xc.statuts[LISSAGE] = FALSE;
      xc.statuts[COULEURS] = FALSE;
      xc.statuts[LEGENDE_COULEUR] = FALSE;
      }

   xc.statuts[GEOGRAPHIE] = TRUE;
   xc.statutSuperposition = FALSE;
   xc.carteLue = FALSE;
   xc.NbMenusIntervalles = XtNumber(menuItemFacteurMult);
   xc.lastColorMap = -1;
   xc.flagInterrupt = TRUE;

   if (9 <= c_wglgpl())
     sizeRecColorTable = 4096;
   else
     sizeRecColorTable = 256;

     sizeRecColorTable = 256;

   ResetColorMap(recColorTable, sizeRecColorTable, recCmap.noPalette);

   c_wglsetw(fenetreAffichage);
   if (GetLogoToggle())
      {
      get_rpn_logo_size(&largeurLogo, &hauteurLogo);
      c_wglgwz(&largeurFenetre, &hauteurFenetre);
      for (i=hauteurFenetre-1; i >= 0; i--)
         {
         c_wglcol(recColorTable[(int)(sizeRecColorTable * (1.0-(float)i/((float)(hauteurFenetre))))]);
         c_wglmvi(0, i);
         c_wgldri(largeurFenetre-1, i);
         }

      draw_rpn_logo();
      }
   draw_rec_version();


   strcpy(nomShell, XtName(SuperWidget.topLevel));
   strcat(nomShell, titreFenetreControle[lng]);

   i = 0;
   if (0 < strlen(panneauContourGeometrie))
      {
      XtSetArg(args[i],XmNgeometry,panneauContourGeometrie);
      i++;
      }


   xc.topLevel = XtAppCreateShell(nomShell, nomShell,
          applicationShellWidgetClass,
          XtDisplay(SuperWidget.topLevel), args, i);
   i = 0;
   xc.panneauContour = (Widget)XmCreateForm(xc.topLevel,
              "panneauContour",
              args, i);
   XtManageChild(xc.panneauContour);

   InitFormeAffichage();
   InitFormeZoom();
   InitFormeSuperposition();
   InitFormeAnimation();

   InitFormeMessage();
   InitPanneauxInfo();

   XtRealizeWidget(xc.topLevel);
   CheckColormap(xc.topLevel);
   CheckCouleurContoursEtGeo();
   FlusherTousLesEvenements();
   ActiverFlagsAffichage();
   EnleverBoutonAnnulation();
   XFlush(XtDisplay(xc.topLevel));

   return xc.statut;
   }

/* ****************************************************************************** */

/** ARGSUSED **/
void Annuler(Widget w, XtPointer unused1, XtPointer unused2)
{
   xc.annulationDemandee = TRUE;
   return ;
   }

/* ****************************************************************************** */


/** ARGSUSED **/
void Zoom(Widget w, XtPointer client_data, XtPointer call_data)
{
   int bouton, event;
   int x0, y0, xsize, ysize, x1, y1, x2, y2;
   int nbMenus;
   char TitreMenu[3][4] = {"    ", "    ", "    "};
   float temp;
   int i;
   float fni, fnj, fx1, fy1, fx2, fy2;
   float rx1, ry1, rx2, ry2;
   float newX1, newY1, newX2, newY2;
   float oldX1, oldY1, oldX2, oldY2;
   int ix1, ix2, iy1, iy2;
   int largeurFenetre, hauteurFenetre;
   _Champ *champ;

   c_wglsetw(fenetreAffichage);
   InvertWidget(w);

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   c_wglcmi(0, 0, largeurFenetre-1, hauteurFenetre-1);

   nbMenus = 0;
   f77name(souris)(&bouton, &event,
       &x0, &y0, &xsize, &ysize,
       &x1, &y1, &x2, &y2, (char *)TitreMenu, &nbMenus, (F2Cl) 4);

   fni = (float) mapInfo.ni;
   fnj = (float) mapInfo.nj;
   fx1 = (float) (x1 - x0);
   fx2 = (float) (x2 - x0);
   fy1 = (float) (y1 - y0);
   fy2 = (float) (y2 - y0);


   switch(event)
      {
      case DRAG:
      if (fx2 < fx1)
         {
         temp = fx1;
         fx1 = fx2;
         fx2 = temp;
         }

      if (fy2 < fy1)
         {
         temp = fy1;
         fy1 = fy2;
         fy2 = temp;
         }

      ix1 = (int) fx1; ix2 = (int) fx2;
      iy1 = (int) fy1; iy2 = (int) fy2;

      c_wgliax(&newX1, &newY1, ix1, iy1);
      c_wgliax(&newX2, &newY2, ix2, iy2);

      c_wglssp(newX1, newY1, newX2, newY2, viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
      ix1 = (int) newX1; newX2 = (int) fx2;
      iy1 = (int) newY1; newY2 = (int) fy2;
      break;

      case MOVE:
      ix1 = (int) fx1; ix2 = (int) fx2;
      iy1 = (int) fy1; iy2 = (int) fy2;

      c_wgliax(&newX1, &newY1, ix1, iy1);
      c_wgliax(&newX2, &newY2, ix2, iy2);

      c_wglgwz(&largeurFenetre, &hauteurFenetre);
      c_wgliax(&oldX1, &oldY1, viewp.vi1, viewp.vj1);
      c_wgliax(&oldX2, &oldY2, viewp.vi2, viewp.vj2);


      fx1 = oldX1 - (newX2 - newX1);
      fx2 = oldX2 - (newX2 - newX1);
      fy1 = oldY1 - (newY2 - newY1);
      fy2 = oldY2 - (newY2 - newY1);

      ix1 = (int) fx1; ix2 = (int) fx2;
      iy1 = (int) fy1; iy2 = (int) fy2;

      AjusterViewport(&viewp);
      c_wglssp(fx1, fy1, fx2, fy2,
               viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
      break;

      case RESET:
      AjusterViewport(&viewp);
      ix1 =  1; iy1 =  1;
      ix2 = mapInfo.ni; iy2 = mapInfo.nj;
      fx1 = 1.0;
      fy1 = 1.0;
      fx2 = (float) mapInfo.ni;
      fy2 = (float) mapInfo.nj;
      c_xy2fxfy(&rx1, &ry1, 1.0, 1.0);
      c_xy2fxfy(&rx2, &ry2, fni, fnj);
      c_wglssp(rx1, ry1, rx2, ry2,
               viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2, 1);
      break;

      default:
      InvertWidget(w);
      xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
      return ;
      break;
      }

/*   DefinirFenetreGrille(&ix1, &iy1, &ix2, &iy2, &mapInfo.ni, &mapInfo.nj);
   fprintf(stderr, "zoom: %d %d %d %d", ix1, iy1, ix2, iy2);*/

   for (i=0; i < FldMgrGetNbChampsActifs(); i++)
      {
      FldMgrGetChamp(&champ, i);
      champ->stats_zoom.to_be_updated = 1;
      }

   MettreAJourStats();
   LibererImages();
   xc.statuts[EN_TRAIN_DE_DESSINER] = FALSE;
   AfficherMessageInfoStandard();
   RedessinerFenetreAffichage();

   InvertWidget(w);
   return ;
   }

void CalculerHauteurMenus()
{
   int i;
   Arg  args[10];
   XmString label;
   XmFontList fontListe;

   label = XmStringCreate("123ABChjkq", XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNfontList, &fontListe); i++;
   XtGetValues(xc.menuFich, args, i);
   hauteurMenus = (int) XmStringHeight(fontListe, label);

   XmStringFree(label);

   }


void AfficherPerimetreFenetre()
{
   int largeurFenetre, hauteurFenetre, largeurPinceau;
   float rx1, ry1, rx2, ry2;
   int fenetreAffichage, fenetreCoupe, fenetreActive;

   GetFenetreAffichageID(&fenetreAffichage);
   CoupeMgrGetFenetreCoupeID(&fenetreCoupe);
   fenetreActive = c_wglgacw();

   c_wglcol(xc.attributs[FOND].indCouleurBack);

   largeurPinceau = xc.attributs[FOND].epaisseur;

   c_wgllwi(largeurPinceau);
   c_wglsld(xc.attributs[FOND].style);
   c_wglgwz(&largeurFenetre, &hauteurFenetre);

   c_wglcmi(viewp.vi1, viewp.vj1, viewp.vi2, viewp.vj2);

   if (fenetreActive == fenetreAffichage)
      {
      c_xy2fxfy(&rx1, &ry1, 1.0, 1.0);
      c_xy2fxfy(&rx2, &ry2, (float)mapInfo.ni, (float)mapInfo.nj);
      c_wglrlx(rx1, ry1, rx2, ry2);
      }

   c_wglcmi(-10,-10,largeurFenetre+10, hauteurFenetre+10);
   c_wglrli(viewp.vi1-1, viewp.vj1-1, viewp.vi2+1, viewp.vj2+1);

   c_wglrli((int)(largeurPinceau*0.5),ROUND(0.5*largeurPinceau),
            largeurFenetre-ROUND(0.5*largeurPinceau),hauteurFenetre-(int)(0.5*largeurPinceau));
   c_wgllwi(1);
   c_wglsld(0);
   }



void AfficherBoutonAnnulation()
{
   Arg args[2];
   Pixel fore, back;

   XtSetArg(args[0], XmNforeground, &fore);
   XtSetArg(args[1], XmNbackground, &back);
   XtGetValues(xc.menuFichier, args, 2);

   XtSetArg(args[0], XmNforeground, back);
   XtSetArg(args[1], XmNbackground, fore);
   XtSetValues(xc.stop, args, 2);
   }

void EnleverBoutonAnnulation()
{
   Arg args[2];
   Pixel fore, back;

   XtSetArg(args[0], XmNforeground, &fore);
   XtSetArg(args[1], XmNbackground, &back);
   XtGetValues(xc.menuFichier, args, 2);

   XtSetArg(args[0], XmNforeground, fore);
   XtSetArg(args[1], XmNbackground, back);
   XtSetValues(xc.stop, args, 2);
   }


void AfficherMessageInfoStandard()
{

   static char *fenetre[] = {"Zoom: [%6.1f,%6.1f,%6.1f,%6.1f]",
                              "Zoom:[%6.1f,%6.1f,%6.1f,%6.1f]" };
   float x0, y0, x1, y1;
   int lng;
   char tmpString[256];
   int fenetreAffichage;

   lng = c_getulng();

   GetFenetreAffichageID(&fenetreAffichage);
   c_wglsetw(fenetreAffichage);
   c_wglgvx(&x0, &y0, &x1, &y1);
   c_fxfy2xy(&x0, &y0, x0, y0);
   c_fxfy2xy(&x1, &y1, x1, y1);
   sprintf(tmpString, fenetre[lng], x0, y0, x1, y1);

   MessageInfo(tmpString, 0);
   }


void InitAttributs()
{
   int r,g,b;

   int i, indMin, indMax;
   int nplanes;

   c_wgldld(1, 0xcccc);
   c_wgldld(2, 0xf0f0);
   c_wgldld(3, 0xff00);

   nplanes = c_wglgpl();
   for (i=0; i< 35; i++)
      {
      xc.attributs[i].indCouleurFore = NOIR;
      xc.attributs[i].couleurFore = NOIR;
      xc.attributs[i].couleurBack = BLANC;
      xc.attributs[i].indCouleurBack = BLANC;
      xc.attributs[i].epaisseur = 1;
      xc.attributs[i].codeDash = 0;
      xc.attributs[i].style = 0;
      }

   c_wglgetcolrange(&indMin, &indMax);

   c_wglgco(NOIR, &r, &g, &b);
   c_wglmco(indMax - OFFSET_FORE_CHAMP1, r, g, b);
   c_wglmco(indMax - OFFSET_GRILLE, r, g, b);
   c_wglmco(indMax - OFFSET_BACK_FOND,   r, g, b);
   c_wglmco(indMax - OFFSET_TOPOGRAPHIE,   r, g, b);

   c_wglgco(BLEU, &r, &g, &b);
   c_wglmco(indMax - OFFSET_FORE_CHAMP2, r, g, b);

   c_wglgco(ROUGE, &r, &g, &b);
   c_wglmco(indMax - OFFSET_FORE_CHAMP3, r, g, b);

   c_wglgco(VERT, &r, &g, &b);
   c_wglmco(indMax - OFFSET_FORE_CHAMP4, r, g, b);

   c_wglgco(BLANC, &r, &g, &b);
   c_wglmco(indMax - OFFSET_BACK_CHAMP1, r, g, b);
   c_wglmco(indMax - OFFSET_BACK_CHAMP2, r, g, b);
   c_wglmco(indMax - OFFSET_BACK_CHAMP3, r, g, b);
   c_wglmco(indMax - OFFSET_BACK_CHAMP4, r, g, b);
   c_wglmco(indMax - OFFSET_GRILLE   ,   r, g, b);
   c_wglmco(indMax - OFFSET_FORE_FOND,   r, g, b);

   xc.attributs[0].indCouleurFore = indMax - OFFSET_FORE_CHAMP1;
   xc.attributs[0].indCouleurBack = indMax - OFFSET_BACK_CHAMP1;
   xc.attributs[0].couleurFore = NOIR;
   xc.attributs[0].couleurBack = BLANC;
   xc.attributs[0].codeDash = 0;
   xc.attributs[0].style = 0;

   xc.attributs[1].indCouleurFore = indMax - OFFSET_FORE_CHAMP2;
   xc.attributs[1].indCouleurBack = indMax - OFFSET_BACK_CHAMP2;
   xc.attributs[1].couleurFore = BLEU;
   xc.attributs[1].couleurBack = BLANC;
   xc.attributs[1].codeDash = 0;
   xc.attributs[1].style = 0;

   xc.attributs[2].indCouleurFore = indMax - OFFSET_FORE_CHAMP3;
   xc.attributs[2].indCouleurBack = indMax - OFFSET_BACK_CHAMP3;
   xc.attributs[2].couleurFore = ROUGE;
   xc.attributs[2].couleurBack = BLANC;
   xc.attributs[2].codeDash = 0;
   xc.attributs[2].style = 0;

   xc.attributs[3].indCouleurFore = indMax - OFFSET_FORE_CHAMP4;
   xc.attributs[3].indCouleurBack = indMax - OFFSET_BACK_CHAMP4;
   xc.attributs[3].couleurFore = VERT;
   xc.attributs[3].couleurBack = BLANC;
   xc.attributs[3].codeDash = 0;
   xc.attributs[3].style = 0;

   xc.attributs[FOND].indCouleurFore = indMax - OFFSET_FORE_FOND;
   xc.attributs[FOND].indCouleurBack = indMax - OFFSET_BACK_FOND;
   xc.attributs[FOND].couleurFore = GRIS;
   xc.attributs[FOND].couleurBack = BLANC;
   xc.attributs[FOND].codeDash = 0;
   xc.attributs[FOND].style = 0;

   xc.attributs[GRID].indCouleurFore = indMax - OFFSET_GRILLE;
   xc.attributs[GRID].couleurFore = NOIR;
   xc.attributs[GRID].couleurBack = BLANC;
   xc.attributs[GRID].codeDash = 0;
   xc.attributs[GRID].style = 0;

   xc.attributs[TOPOG].indCouleurFore = indMax - OFFSET_TOPOGRAPHIE;
   xc.attributs[OFFSET_TOPOGRAPHIE].couleurFore = NOIR;
   xc.attributs[OFFSET_TOPOGRAPHIE].codeDash = 0;
   xc.attributs[OFFSET_TOPOGRAPHIE].style = 0;


   for (i=0; i < 35; i++)
      {
#ifdef GL_WGL
      xc.attributs[i].epaisseur = 2;
#else
      xc.attributs[i].epaisseur = 1;
#endif
      }

   for (i=0; i < 35; i++)
      {
      xc.attributs[i].displayValCentrales = SELON_MENU;
      xc.attributs[i].displayContours     = SELON_MENU;
      xc.attributs[i].displayLabels       = SELON_MENU;
      xc.attributs[i].labelSize           = 0;
      xc.attributs[i].centralValSize      = 20;
      }


   xc.attributs[GRID].epaisseur = 1;
   xc.attributs[FOND].epaisseur = 1;
   xc.attributs[TOPOG].epaisseur = 1;


   }


void CheckCouleurContoursEtGeo()
{
   int indMin, indMax;

   c_wglgetcolrange(&indMin, &indMax);

   ResetColorMap(recColorTable, sizeRecColorTable, recCmap.noPalette);
   }

void AfficherGrille()
{
   int i,j,ix,iy;
   float x1, y1, x2, y2;
   float rx1, ry1, rx2, ry2;

   c_wglgvx(&rx1, &ry1, &rx2, &ry2);
   c_fxfy2xy(&x1, &y1, rx1, ry1);
   c_fxfy2xy(&x2, &y2, rx2, ry2);


   x1--;
   x2++;
   y1--;
   y2++;

   if (x1 < 1.0)
      x1 = 1.0;

   if (y1 < 1.0)
      y1 = 1.0;

   if (x2 > (float)mapInfo.ni)
      x2 = (float)mapInfo.ni;

   if (y2 > (float)mapInfo.nj)
      y2 = (float)mapInfo.nj;

   c_wgllwi(xc.attributs[GRID].epaisseur);
   c_wglsld(xc.attributs[GRID].style);
   c_wglcol(xc.attributs[GRID].indCouleurFore);

//    for (j=(int)y1; j <= ROUND(y2); j++)
//       {
//       for (i=(int)x1; i <= (int)x2; i++)
//          {
//          c_xy2fxfy(&rx1, &ry1, (float)(int)i,  (float) j);
//          c_wglxai(&ix,&iy,rx1,ry1);
//          c_wglcfi(ix,iy,2);
//          }
//       }

   for (j=(int)y1; j <= ROUND(y2); j++)
      {
      c_xy2fxfy(&rx1, &ry1, (float)(int)x1,  (float) j);
      c_wglmvx(rx1,ry1);
      c_xy2fxfy(&rx2, &ry2, (float)ROUND(x2), (float) j);
      c_wgldrx(rx2, ry2);
      }

   for (i=(int)x1; i <= (int)x2; i++)
      {
      c_xy2fxfy(&rx1, &ry1, (float)i,  (float)(int)y1);
      c_wglmvx(rx1, ry1);
      c_xy2fxfy(&rx2, &ry2, (float)i, (float) ROUND(y2));
      c_wgldrx(rx2, ry2);
      }


   c_wgllwi(1);
   c_wglsld(0);

   }

void AfficherGrilleSource(int indchamp)
{
   int i,j,ix,iy,ier,npts;
   float x1, y1, x2, y2;
   float rx1, ry1, rx2, ry2;
   float *lats, *lons, *x, *y /***x2, **y2*/;
   _Champ *champ;

   int grsrc, grdst;

   FldMgrGetChamp(&champ, indchamp);

   c_wgllwi(xc.attributs[indchamp].epaisseur);
   c_wglsld(xc.attributs[indchamp].style);
   c_wglcol(xc.attributs[indchamp].indCouleurFore);

   grsrc = c_ezqkdef(champ->src.ni, champ->src.nj, champ->src.grtyp,
         champ->src.ig1, champ->src.ig2, champ->src.ig3, champ->src.ig4, champ->iun);
//   grdst = c_ezgetgdout();
   grdst = c_ezqkdef(champ->dst.ni, champ->dst.nj, champ->dst.grtyp,
         champ->dst.ig1, champ->dst.ig2, champ->dst.ig3, champ->dst.ig4, champ->iun);
   npts = champ->src.ni*champ->src.nj;
   lats = (float *) malloc(npts*sizeof(float));
   lons = (float *) malloc(npts*sizeof(float));
   x = (float *) malloc(npts*sizeof(float));
   y = (float *) malloc(npts*sizeof(float));
//    x2 = (float **) malloc(4*sizeof(float));
//    y2 = (float **) malloc(4*sizeof(float));
//    for (i=0; i < 4; i++)
//       {
//       x2[i] = (float *) malloc(npts*sizeof(float));
//       y2[i] = (float *) malloc(npts*sizeof(float));
//       }

   for (j=0; j < champ->src.nj; j++)
     {
     for (i=0; i < champ->src.ni; i++)
       {
       x[i+j*champ->src.ni] = (float)(i+1);
       y[i+j*champ->src.ni] = (float)(j+1);
/*       x[0][i+j*champ->src.ni] = (float)(i+0.5);
       y[0][i+j*champ->src.ni] = (float)(j+0.5);*/
       }
     }

   ier = c_gdllfxy(grsrc, lats, lons, x, y, npts);
   ier = c_gdxyfll(grdst, x, y, lats, lons, npts);
   ier = c_wglgvx(&x1, &y1, &x2, &y2);
   c_fxfy2xy(&x1, &y1, x1, y1);
   c_fxfy2xy(&x2, &y2, x2, y2);

   for (i=0; i < npts; i++)
     {
     if (x[i] > x1 && x[i] < x2 && y[i] > y1 && y[i] < y2)
       {
       c_xy2fxfy(&rx1, &ry1, x[i], y[i]);
       c_wglxai(&ix,&iy,rx1,ry1);
       c_wglcli(ix,iy,3);
       }
     }

/*   for (j=1; j <= champ->src.nj; j++)
      {
      c_xy2fxfy(&rx1, &ry1, x[FTN2C(1,j,champ->src.ni)],  (y[FTN2C(1,j,champ->src.ni)]);
      c_wglmvx(rx1,ry1);
      c_xy2fxfy(&rx1, &ry1, x[FTN2C(champ->src.ni,j,champ->src.ni)],  (y[FTN2C(1,j,champ->src.ni)]);
      c_wgldrx(rx2, ry2);
      }

   for (i=1; i <= champ->src.ni; i++)
      {
      c_xy2fxfy(&rx1, &ry1, (float)i,  (float)(int)y1);
      c_wglmvx(rx1, ry1);
      c_xy2fxfy(&rx2, &ry2, (float)i, (float) ROUND(y2));
      c_wgldrx(rx2, ry2);
      }*/
   c_wgllwi(1);
   c_wglsld(0);
   free(x);
   free(y);
   free(lats);
   free(lons);

   }


void SetClipMask()
{
   float x1, y1, x2, y2;
   int i1, j1, i2, j2;

   c_wglgsx(&x1, &y1, &x2, &y2);
   c_wglxai(&i1, &j1, x1, y1);
   c_wglxai(&i2, &j2, x2, y2);

   if (i1 < viewp.vi1)
      i1 = viewp.vi1;

   if (j1 < viewp.vj1)
      j1 = viewp.vj1;

   if (i2 > viewp.vi2)
      i2 = viewp.vi2;

   if (j2 > viewp.vj2)
      j2 = viewp.vj2;

   c_wglcmi(i1, j1, i2, j2);



   }


void Beeper()
{
   XBell(XtDisplay(xc.topLevel), 0);
   XBell(XtDisplay(xc.topLevel), 0);
   XBell(XtDisplay(xc.topLevel), 0);
   FlusherTousLesEvenements();
   }


int ComparerGridInfo()
{
   if (oldMapInfo.ni != mapInfo.ni)
      return  -1;

   if (oldMapInfo.nj != mapInfo.nj)
      return  -1;

/**
   if (oldMapInfo.type != mapInfo.type)
      return  -1;
**/
   if (oldMapInfo.ig1 != mapInfo.ig1)
      return  -1;

   if (oldMapInfo.ig2 != mapInfo.ig2)
      return  -1;

   if (oldMapInfo.ig3 != mapInfo.ig3)
      return  -1;

   if (oldMapInfo.ig4 != mapInfo.ig4)
      return  -1;

   return 0;

  }

int GrillesIdentiques(GeoMapInfoStruct mapInfo,int ni, int nj, char type, int ig1, int ig2, int ig3, int ig4)
{
   if (type != mapInfo.type)
      return  0;

   if (ni != mapInfo.ni)
      return  0;

   if (nj != mapInfo.nj)
      return  0;

   if (ig1 != mapInfo.ig1)
      return  0;

   if (ig2 != mapInfo.ig2)
      return  0;

   if (ig3 != mapInfo.ig3)
      return  0;

   if (ig4 != mapInfo.ig4)
      return  0;

   return 1;

  }



/* ****************************************************************************** */

void CopierGridInfo(GeoMapInfoStruct *gridInfoSortie, GeoMapInfoStruct *gridInfoEntree)
{
   int i, nbBytes;
   char *ptr;

   nbBytes = sizeof(GeoMapInfoStruct);

   ptr = (char *)gridInfoSortie;
   for (i=0; i < nbBytes; i++)
      {
      *(ptr+i) = *(((char *)gridInfoEntree) + i);
      }
   }

/* ****************************************************************************** */

void EffacerLaFenetreSiNecessaire()
{
   float xdebut, ydebut, xfin, yfin;
   _Champ *champ;

   FldMgrGetChamp(&champ, 0);
   if (xc.statuts[COULEURS] == FALSE || champ->domaine == VALEURS_PONCTUELLES)
      {
      EffacerFenetre();
      }
   else
      {
      c_wglgsx(&xdebut, &ydebut, &xfin, &yfin);
      c_fxfy2xy(&xdebut, &ydebut, xdebut, ydebut);
      c_fxfy2xy(&xfin, &yfin, xfin, yfin);
      c_wgliax(&xdebut, &ydebut, viewp.vi1, viewp.vj1);
      c_wgliax(&xfin, &yfin, viewp.vi2, viewp.vj2);

      if ((xdebut < 0.0) || (xfin > (float)(mapInfo.ni)+1.0) || (ydebut < 0.0) || (yfin > (float)(mapInfo.nj)+1.0))
        {
        EffacerFenetre();
        }
      }

   }

void GetFenetreAffichageID(int *indFenetre)
{
   *indFenetre = fenetreAffichage;
   }

/* ****************************************************************************** */

void f77name(c_sctlatr)(char item[],char valeur[],F2Cl flenItem,F2Cl flenValeur)
{
   int lenItem=flenItem, lenValeur=flenValeur;
   int indItem = -1;


   item[lenItem-1] = '\0';
   valeur[lenValeur-1] = '\0';
   nettoyer(item);
   nettoyer(valeur);
   if (0 == strcmp(item,"geometrie") || 0 == strcmp(item,"geometry"))
      {
      strcpy(panneauContourGeometrie,valeur);
      }

   if (0 == strcmp(item,"couleurs") || 0 == strcmp(item,"colors"))
      {
      indItem = COULEURS;
      }

   if (0 == strcmp(item,"contours") || 0 == strcmp(item,"contours"))
      {
      indItem = CONTOURS;
      }

   if (0 == strcmp(item,"geographie") || 0 == strcmp(item,"geography"))
      {
      indItem = GEOGRAPHIE;
      }

   if (0 == strcmp(item,"grille") || 0 == strcmp(item,"grid"))
      {
      indItem = GRILLE;
      }

   if (0 == strcmp(item,"labels") || 0 == strcmp(item,"labels"))
      {
      indItem = LABELS;
      }

   if (0 == strcmp(item,"legende") || 0 == strcmp(item,"legend"))
      {
      indItem = LEGENDE;
      }

   if (0 == strcmp(item,"legende_couleurs") || 0 == strcmp(item,"color_legend"))
      {
      indItem = LEGENDE_COULEUR;
      }

   if (0 == strcmp(item,"lissage") || 0 == strcmp(item,"smoothing"))
      {
      indItem = LISSAGE;
      }

   if (0 == strcmp(item,"valeurs_centrales") || 0 == strcmp(item,"central_values"))
      {
      indItem = VALEURS_CENTRALES;
      }

      if (0 == strcmp(item,"barbules") || 0 == strcmp(item,"wind_barbs"))
      {
      indItem = BARBULES;
      }

   if (0 == strcmp(item,"topographie") || 0 == strcmp(item,"topography"))
      {
      indItem = TOPO;
      }

   if (0 == strcmp(item,"zoom_local") || 0 == strcmp(item,"local_zoom"))
      {
      indItem = ZOOM_LOCAL;
      }

   if (0 == strcmp(item,"redessiner_apres_selection") || 0 == strcmp(item,"redraw_after_selection"))
      {
      indItem = AFF_AUTOMATIQUE;
      }

   if (indItem >= 0)
      {
      if (0 == strcmp(valeur, "on"))
         {
         xc.statuts[indItem] = TRUE;
         }

      if (0 == strcmp(valeur, "off"))
         {
         xc.statuts[indItem] = FALSE;
         }
      }

}

void EcrCtlAtr(FILE *fichierDemarrage)
{
   char tableau[32];
   char ligne[80];
   char item[32],valeur[32];
   int i;

   Arg  args[10];
   Position x,y;
   Display *disp;
   Window win;

   strcpy(tableau, "tableau_de_bord");

   disp = XtDisplay(xc.topLevel);
   win  = XtWindow(xc.topLevel);
   i = 0;
   XtSetArg(args[i], XmNx, &x); i++;
   XtSetArg(args[i], XmNy, &y); i++;
   XtGetValues(xc.topLevel, args, i);

   strcpy(item,"geometrie");
   sprintf(valeur,"%+d%+d",x,y);
   sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
   fprintf(fichierDemarrage,"%s\n",ligne);


   for (i=0; i < 12; i++)
      {
      switch(i)
         {
         case COULEURS:
         strcpy(item,"couleurs");
         break;

         case CONTOURS:
         strcpy(item,"contours");
         break;

         case GEOGRAPHIE:
         strcpy(item,"geographie");
         break;

         case GRILLE:
         strcpy(item,"grille");
         break;

         case LABELS:
         strcpy(item,"labels");
         break;

         case LEGENDE:
         strcpy(item,"legende");
         break;

         case LEGENDE_COULEUR:
         strcpy(item,"legende_couleurs");
         break;

         case LISSAGE:
         strcpy(item,"lissage");
         break;

         case BARBULES:
         strcpy(item,"barbules");
         break;

   case TOPO:
   strcpy(item,"topographie");
   break;

   case ZOOM_LOCAL:
   strcpy(item,"zoom_local");
   break;

   case VALEURS_CENTRALES:
   strcpy(item,"valeurs_centrales");
   break;

         case AFF_AUTOMATIQUE:
         strcpy(item,"redessiner_apres_selection");
         break;
         }

      if (xc.statuts[i])
         {
         strcpy(valeur,"on");
         }
      else
         {
         strcpy(valeur,"off");
         }

      sprintf(ligne, " setitem('%s','%s','%s')",tableau,item,valeur);
      fprintf(fichierDemarrage,"%s\n",ligne);
      }

   }

/* ****************************************************************************** */

void CtrlMgrSetMathOp(int op)
{
   mathOp = op;
   }

int CtrlMgrGetMathOp()
{
   return mathOp;
   }

int CtrlMgrGetContextualMathOp(int indChamp)
{

   if (mathOp != NO_OP && 0 == indChamp%2 && ((indChamp+1) >= FldMgrGetNbChampsActifs()))
      {
      return NO_OP;
      }
   else
      {
      return mathOp;
      }

   }

void f77name(recnologo)()
{
   rpnLogo = 0;
   }

int GetLogoToggle()
{
   return rpnLogo;
   }

int GetGrilleSelectionnee()
{
   return itemGrilleSelectionnee;
}

int GetValeursManquantesToggle()
{
  return xc.statuts[VALEURS_MANQUANTES];
}
