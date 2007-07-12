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

 /***************************************************************************************************************
 *                                                                                                              *
 *                                      XSELECTSTD.C                                                            *
 *                                                                                                              *
 ****************************************************************************************************************
 **                                                                                                             *
 ** Fichier xselectstd.c                                                                                        *
 **                                                                                                             *
 ** Auteur: Yves Chartier (RPN)                                                                                 *
 **                                                                                                             *
 ** Objet:  Visualisation et selection interactives d'enregistrements                                           *
 **         dans un ou plusieurs tableaux bi-dimensionnels de chaines de caracteres.                            *
 **                                                                                                             *
 ** Langage: C                                                                                                  *
 ** Environnement: X11, Xt, Motif Widgets                                                                       *
 **                                                                                                             *
 ** Statut: Experimental                                                                                        *
 **         A besoin d'un bon nettoyage...                                                                      *       
 **                                                                                                             *
 ** Creation: Mars 1990                                                                                         *
 ** Dernieres modifications: novembre 1990                                                                      *
 ** Conversion a Motif: Octobre 1990                                                                            *       
 **                                                                                                             *
 ***************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>

#include <xinit.h>
#include <xm-xselectstd.h>
#include <rpnmacros.h>

#define NB_MAX_DESCRIPTEURS     24              /* nb max. de descripteurs permis dans un tableau               */
#define NB_MAX_FENETRES         4               /* nb max. de fenetres permises                                 */

#define SELECTION_EN_COURS 1  
#define SELECTION_TERMINEE 0  

#define SELECTION_SIMPLE   0
#define SELECTION_MULTIPLE 1
#define SELECTION_ETENDUE  2

#define BOUTON_FERMER_INACTIF 0 
#define BOUTON_FERMER_ACTIF   1


/** langue utilisee **/
wordint langue;

/*======================================================================================================*/
typedef struct
{
   char *val;                   /* pointeur sur une cle */
   XmString xmVal;                /* nom d'une cle de type "compound string" (voir manuel sur motif) */
   wordint nbOccur;                 /* nb. occurences de la cle dans une colonne du tableau  */
   wordint *ind;                    /* tableau d'indices montrant les occur. de la cle dans une colonne
                                   du tableau des records. */
   } cleInfoStruct;

typedef struct
{
   wordint nbCles;                  /* nb de cles distinctes dans une colonne du tableau */
   cleInfoStruct *cleInfo;      /* infos associees a cette cle */
   } cleStruct;

typedef struct
{
   wordint indDes;                  /* indice du descripteur */
   wordint indCle;                  /* indice de la cle      */
   }  filtresStruct;

typedef struct
{
   wordint indDes;                  /* indice du descripteur */
   wordint nbFiltresListe;          /* nb de cles utilisees pour la filtration de records, pour un 
                                   descripteur donne */
   wordint *liste;                  /* liste des indices de cles utilises pour la filtration de records */ 
   }  filtresInfoStruct;

/*======================================================================================================*/
/**
 ** La structure suivante est une collection de toutes les variables utilisees dans le programme.
 ** L'utilisation de cette structure permet de n'avoir qu'a definir une seule variable
 ** globale, minimisant ainsi les risques de collisions avec d'autres modules. Elle permet
 ** aussi d'isoler facilement les tableaux entre eux.
 **/

static char *messString[] = {"%5d /% 5d ",
        "%5d /% 5d "};

typedef struct
{
   char  *titreFenetre;                 /* titre de la fenetre */
   cleStruct     cles[NB_MAX_DESCRIPTEURS];     /* ensemble de cles trouves pour chaque descripteur */
   XmString      titresMenus[NB_MAX_DESCRIPTEURS]; 
   filtresStruct *filtres;                      /* liste de filtres actifs */
   XmString      **recs;                        /* tableau bi-dimensionnel de XmString contenant
                                                   le tableau original */
   XmString      *recsAffiches;                 /* Le tableau de records affiches. */
   XmString      *recsFiltresAffiches;          /* tableau des records deja filtres qui est affiche. */
   wordint           nbDes;                         /* nb total de descripteurs (de colonnes) dans le tableau */
   wordint           lastNbRecs;
   wordint           nbRecs;                        /* nb total de records dans le tableau */
   wordint           nbRecsFiltres;                 /* nb total de records repondant aux criteres de selection */
   wordint           nbCles;                        /* nb total de cles (dans toutes les colonnes du tableau) */
   wordint           nbFiltres;                     /* nb total de filtres en vigueur */
   wordint           nbRecsSelect;                  /* nb total de recordss selectionnes par l'usager. Cette
                                                  operation se fait par un click de la souris. */
   wordint           *indRecsFiltres;               /* tableau contenant les indices des records repondant aux
                                                   criteres de selection */
   wordint           *indRecsSelect;                /* tableau contenant les indices des records selectionnes par 
                                                   l'usager */
   wordint           typeSelection;
   wordint           statutBoutonFermer;
   wordint           StatutSelection;               /* = SELECTION_TERMINEE quand on choisis Apply */
                                                /* = SELECTION_FINIE quand on choisis Close */
                                                /* = SELECTION_EN_COURS autrement. */
   XEvent        theEvent;                      /* Evenement X */
/*======================================================================================================*/
   /** 
    ** Le reste de la structure contient des elements propres a Xm. La hierarchie des Widgets
    ** est la suivante.
    ** 
    ** topLevel                                                 Application Shell
    **          panneau                                                 XmForm
    **                  boutonform                                              XmRowColumn
    **                          deselectionnerRecs                                      XmPushButton
    **                          effacerFiltres                                          XmPushButton
    **                          infoFiltres                                             XmPushButton
    **                          nbFrame                                                 XmFrame
    **                                  nbForm                                                  XmRowColumn
    **                                          selMsg                                                  XmLabelGadget
    **                                          nbSel                                                   XmLabelGadget
    **                  menuform                                                XmMenuBar
    **                          panRowCol[0]                                            XmCascadeButton
    **                          panRowCol[0]                                                XmPullDownMenu
    **                                  menuItems[0][0]                                         XmToggleButtonGadget
    **                                  menuItems[0][1]                                         XmToggleButtonGadget
    **                                       ***                                                        ***
    **                                  menuItems[0][cles[0].nbCles - 1]                        XmToggleButtonGadget
    **                          panRowCol[1]                                            XmCascadeButton
    **                          panRowCol[1]                                                XmPullDownMenu
    **                                  menuItems[1][0]                                         XmToggleButtonGadget
    **                                  menuItems[1][1]                                         XmToggleButtonGadget
    **                                       ***                                                        ***
    **                                  menuItems[1][cles[1].nbCles - 1]                        XmToggleButtonGadget
    **                             ***                                                      ***
    **                          panRowCol[nbDes]                                        XmCascadeButton
    **                          panRowCol[nbDes]                                            XmPullDownMenu
    **                                  menuItems[nbDes - 1][0]                                 XmToggleButtonGadget
    **                                  menuItems[nbDes - 1][1]                                 XmToggleButtonGadget
    **                                       ***                                                        ***
    **                                  menuItems[nbDes - 1][cles[nbDes - 1].nbCles - 1]        XmToggleButtonGadget
    **                  listeform                                               XmScrolledWindow
    **                          vscroll                                                 XmScrollBar
    **                          formListe                                               XmForm
    **                                  listeNb                                                 XmList
    **                                  liste                                                   XmList
    **/
/*======================================================================================================*/
   Widget topLevel;                                                     /* Toplevel shell de xselectstd.        */
   Widget       infoPopUpShell, infoButtonForm, frameInfoLabel, infoLabel; 

   Widget               buttonform;                                     /* Container widget pour les boutons du
                                                                           popup.                               */
   Widget                       filtresLabel;                           /* Text informative.                    */

   Widget                       infoFiltresOk;                          /* Bouton pour enlever le popup.        */

   Widget       pan;                                                /* Container widget pour la fenetre
                                                                           principale.                          */
   Widget               boutonform;                                     /* Container widget pour les boutons de
                                                                           la fenetre principale.               */
   Widget                       ok;                                     /* Bouton pour deselectionner touts
                                                                           les records.                         */
   Widget                       deselectionnerRecs;                     /* Bouton pour deselectionner touts
                                                                           les records.                         */
   Widget                       effacerFiltres;                         /* Bouton pour deselectionner touts
                                                                           les cles selectionnes (filtres).     */
   Widget                       infoFiltres;                            /* Bouton pour faire apparaitre le
                                                                           popup des filtres.                   */
   Widget                       nbFrame;                                /* Le frame qui contient nbSel.         */

   Widget                               nbForm;                         /* Le container pour nbSel.             */

   Widget                                       selMsg;                 /* Label informatif.                    */

   Widget                                       nbSel;                  /* Label qui indique le nombre de
                                                                           records qui ont ete selectionnes.    */
   Widget               panListe, panFrame, panForme, panListeForm, panRetour, panEffacerFiltres;
   Widget               menuform;                                       /* Menu des descripteurs.               */

   Widget                       menus[NB_MAX_DESCRIPTEURS];         /* Tableau des boutons qui font apparaitre
                                                                           les pulldowns de chaque descripteur. */
   Widget                       panRowCol[NB_MAX_DESCRIPTEURS];
   Widget                       panLabel[NB_MAX_DESCRIPTEURS];           
   Widget                       panListeItems[NB_MAX_DESCRIPTEURS];           
   Widget               liste;                                          /* Liste qui contient les records.      */
   Widget               fermer,separateurBoutonFermer;

   } XSelectStdStruct;

wordint largeurMenus[NB_MAX_DESCRIPTEURS];
static wordint               wi;                    /* L'indice de la fenetre dans laquelle on travaille.   */
static XSelectStdStruct  xs[NB_MAX_FENETRES];   /* La structure qui contient tout.                      */
extern SuperWidgetStruct SuperWidget;           /* Le toplevel de l'application.                        */
/*======================================================================================================*/
/**
************************************************************
************************************************************
**/

/* Fonctions appeles de l'exterieur de xselectstd. */

wordint f77name(xselact)(wordint sel[], wordint *nbsel, wordint *indsel);
wordint f77name(xselfer)(wordint sel[], wordint *nbsel, char idents[], wordint table[][3], wordint *m, wordint *n, wordint     *indsel, wordint len);
wordint f77name(xselins)(char *tableau, wordint table[][3], wordint *nbrecs); 
wordint f77name(xselopt)(wordint *indSelecteur, char option[], char valeur[], wordint lenOption, wordint lenValeur);
wordint f77name(xselouf)(wordint table[][3], wordint *nbrecs);
wordint f77name(xseloup)(char *titre, wordint  *nbrecs, char idents[], wordint *nbdes, wordint *indSel, wordint *typeSel, wordint lenNomFich, wordint lenIdents);
wordint f77name(xselupd)();
wordint f77name(xseldim)();
wordint f77name(xselundim)();
wordint f77name(xselup)(wordint *indSelecteur);
wordint f77name(xseldown)(wordint *indSelecteur);
wordint c_xselup(wordint indSelecteur);
wordint c_xselopt(wordint indSelecteur, char *option, char *valeur);
wordint c_xseldown(wordint indSelecteur);

/* Callbacks. */

static XtCallbackProc Ok(Widget W, caddr_t unused1, caddr_t unused2);
static XtCallbackProc FermerSelecteur(Widget w, caddr_t unused1, caddr_t unused2);
static XtCallbackProc DeselectionnerRecords(Widget w, caddr_t unused1, caddr_t unused2);
static XtCallbackProc EffacerFiltres(Widget w, caddr_t unused1, caddr_t unused2);
static XtCallbackProc EffacerFiltresSeulement(Widget w, caddr_t u1, caddr_t u2);
static XtCallbackProc FermerSelecteur(Widget w, caddr_t unused1, caddr_t unused2);
static XtCallbackProc HighlightFields(Widget  w, caddr_t unused1, caddr_t call_data);
static XtCallbackProc MontrerDescripteurs(Widget w, caddr_t u1, caddr_t u2);
static XtCallbackProc SelectionListeTerminee(Widget w, caddr_t u1, caddr_t u2);


/* Autres fonctions. */

static void AfficherInfoFiltres(Widget w, caddr_t unused1, caddr_t unused2);
wordint     ActiverSelWidgets();
void        AfficherListe(Widget w, XmStringTable items, wordint nbItems);
void        AfficherNbSelect(wordint nb);
wordint     AjouterCle(cleStruct cles[], wordint pos, wordint indDes, wordint indCle);
wordint     AjouterFiltre(wordint indDes, wordint indCle);
wordint     AjouterNouvelleCle(char *val, wordint len, cleStruct cles[], wordint indDes, wordint indCle);
static void AjusterSensibiliteBoutons();
            CalculerLargeurMenus(wordint largeurMenus[], wordint table[][3]);
wordint     ChangerWidgets();
wordint     ChercherCle(char *val, wordint len, cleStruct cles[], wordint indDes);
wordint     ComparerCles(cleInfoStruct *cleInfo1, cleInfoStruct *cleInfo2);
wordint     ComparerFiltres(filtresStruct *filtre1,  filtresStruct *filtre2);
wordint     DesactiverSelWidgets();
wordint     EnleverFiltre(wordint indDes, wordint indCle);
wordint     FiltrerRecords(wordint sel[], wordint *nbsel, XmString **recs, cleStruct cles[], filtresStruct *filtres, wordint nbRecs, wordint nbDes, wordint  nbFiltres);
wordint     FreeCles(cleStruct cles[], wordint n);
wordint     Freerecs(XmString *(**recs), wordint nbDes);
void        FreerecsAffiches(XmString **recsAffiches, wordint nbDes);
wordint     InitCles(char *tableau, wordint  table[][3], cleStruct cles[], wordint nbRecs, wordint nbDes);
wordint     InitFiltresInfo(filtresInfoStruct filtresInfo[], filtresStruct *filtres, wordint nbFiltres);
wordint     InitRecs(XmString *(**recs), cleStruct cles[], wordint nbRecs, wordint nbDes);
void        InitRecsAffiches(XmString **recsAffiches, char *tableau, wordint table[][3], wordint nbRecs);
void        InitRecsFiltresAffiches(XmString **recsFiltresAffiches, XmString *recsAffiches, wordint nbRecs);
wordint     InitTitresMenus(char *idents[], wordint nbDes);
wordint     InitWidgetsAll1(XmString TitresMenus[], wordint nbRecsFiltres, wordint nbDes);
wordint     InitWidgetsAll2(XmString TitresMenus[], wordint nbRecsFiltres, wordint nbDes);
wordint     InitWidgetsBasic();
void        InitWidgetsBouton();
wordint     InitWidgetsCallback(wordint nbDes);
wordint     InitWidgetsFiltre();
void        InitWidgetsForm();
void        InitWidgetsMenu(XmString TitresMenus[], wordint nbDes);
wordint     InitWidgetsRec(wordint nbRecsFiltres,  wordint nbDes);
wordint     LibererPointeurs();
wordint     LibererPanneauListe();
wordint     MessageChargement(wordint nbRecs1, wordint nbRecs2);
wordint     NettoyerString(char str[]);
static void PositionnerMenubar();
static void TrouverLongueurMot(wordint *longueur, XmString mot);
Widget      TrouverWidgetParent();
wordint     UpdateFiltres();
void        UpdateRecsFiltresAffiches(XmString **recsFiltresAffiches, XmString *recsAffiches, wordint nbDes, wordint sel[], wordint nbSel);
wordint     XSelectstdActiver();
wordint     SelectstdFermer();
Widget      XSelectstdOuvrir();

wordint XSelectstdActiver(wordint  sel[], wordint  *nbsel, wordint  *indSelecteur);
wordint XSelectstdCommencerInit(char *titre, wordint nbrecs, char **idents, wordint nbdes, wordint indSel, wordint typeSel);
wordint XSelectstdFermer(wordint  sel[], wordint  *nbsel, char **idents, wordint  table[][3], wordint  *m, wordint  *n, wordint  *indSelecteur);
void XSelectstdInserer(char *tableau, wordint table[][3], wordint nbrecs);
void XSelectstdTerminerInit(wordint table[][3], wordint nbrecs);
/*===============================================================================

                        FONCTIONS APPELES DE L'EXTERIEUR

=================================================================================*/

/*===============================================================================

                                CALLBACKS

=================================================================================*/

/**
********************************************************************************

NOM:                    AfficherInfoFiltres()

FONCTION:               Afficher le popup qui montre quels filtres ont ete selectionnes.

APPELE PAR:             XtMainLoop() quand on clique sur le widget infoFiltres.

METHODE:                On affiche le popup juste en dessous du widget infoFiltres.
                        On rend les widgets de la fenetre principale non-sensitive pour
                        ne pas permetre des changements pendant qu'on examine le popup.

GLOBALES AFFECTES:      xs[wi].popUpShell

VALEUR RETOUNEE:       Aucune.

------------------------------------------------------------------------------*/

static void AfficherInfoFiltres(Widget w, caddr_t unused1, caddr_t unused2)
{
   Position     x, y;           /* La position sur l'ecran ou le popup va apparaitre.   */
   Dimension    height;         /* La hauteure du widget infoFiltres.                   */
   Arg          args[5];        /* Resources des widgets qui sont utilisees.            */
   wordint          i;              /* Nombre de resources utilisees.                       */

/**
   i = 0;
   XtSetArg(args[i], XmNheight, &height); i++;
   XtGetValues(xs[wi].infoFiltres, args, i);


   Cherche la position sur l'ecran correspondant au coin inferieur gauche du widget infoFiltres.
   XtTranslateCoords(xs[wi].infoFiltres, 0, height, &x, &y);

   i = 0;
   XtSetArg(args[i], XmNx, x); i++;
   XtSetArg(args[i], XmNy, y); i++;
   XtSetValues(XtParent(xs[wi].popUpShell), args, i);

   DesactiverSelWidgets();
   XtManageChild(xs[wi].popUpShell);
   XtMapWidget(xs[wi].popUpShell);
**/
   }
/*======================================================================================================*/

/**
******************************************************************************

NOM:                    DeselectionnerRecords()

FONCTION:               Desactiver touts les records qui ont ete selectionnes
                        dans la liste des records (fichiers standards).

APPELE PAR:             XtMainLoop() quand on appuie sur le boutton deselectionnerRecs.
                        EffacerFiltres() apres qu'on a reaffiche les records.
                        panRowColSelect() apres qu'on clique sur un toggle dans un pulldown.

METHODE:                On deselectionne les items dans la partie affichee.
                        On indique qu'aucun record est selectionne dans toute la liste
                        en mettant nbRecsSelect a zero.

GLOBALES AFFECTES:      xs[wi].listeNb
                        xs[wi].liste
                        xs[wi].nbRecsSelect

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static XtCallbackProc DeselectionnerRecords(Widget w, caddr_t unused1, caddr_t unused2)
{
   wordint i; /* Compteur. */

   XmListDeselectAllItems(xs[wi].liste);
   xs[wi].nbRecsSelect          = 0;
   AfficherNbSelect((wordint)0);
}
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    EffacerFiltres()

FONCTION:               Deselectionner touts les cles.
                        Vider la liste des filtres.

APPELE PAR:             XtMainLoop() quand on appuie sur le boutton effacerFiltres.
                        UpdateFiltres() quand on a deselectionne touts les cles en cliquant
                        dans les listes des descripteurs.

METHODE:                On remets touts les records dans la liste des records filtres.
                        On efface toutes les selections de cles qui ont ete faits.
                        On affiche les listes.

GLOBALES AFFECTES:      xs[wi].indRecsFiltres
                        xs[wi].liste
                        xs[wi].listeNb
                        xs[wi].filtresListeDes
                        xs[wi].filtresListeCles
                        xs[wi].menuItems[][]
                        xs[wi].vscroll
                        xs[wi].nbRecsFiltres
                        xs[wi].nbFiltres
                        xs[wi].nbRecsSelect

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static XtCallbackProc EffacerFiltres(Widget w, caddr_t unused1, caddr_t unused2)
{
   wordint  j;       /* Compteur.                                   */
   wordint  nb;      /* Nombre de records affiches.                 */
   wordint  i;       /* Nombre de resources utilisees; compteur.    */
   Arg  args[5]; /* Resources des widgets qui sont utilisees.   */
   wordint zero = 0;

   InvertWidget(w);
   for (j = 0; j < xs[wi].nbRecs; j++)
       {
       xs[wi].indRecsFiltres[j] = j;
       xs[wi].recsFiltresAffiches[j] = xs[wi].recsAffiches[j];
       }

   nb = xs[wi].nbRecs;

   AfficherListe(xs[wi].liste, xs[wi].recsFiltresAffiches, nb);
   EffacerFiltresSeulement(NULL, NULL, NULL);
   DeselectionnerRecords(NULL, NULL, NULL);
   xs[wi].nbRecsFiltres = xs[wi].nbRecs;
   xs[wi].nbFiltres     = 0;
   AfficherNbSelect(zero);
   InvertWidget(w);
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    HighlightFields()

FONCTION:               De (de)selectionner tous les items des listes de records
                        a la meme position que dans la liste dans laquelle
                        on a clique.

APPELE PAR:             XtMainLoop() quand on (de)selectionne un des records
                        (dans n'importe quel descripteur) de la liste des
                        fichiers standards.

METHODE:                On selectionne la position de liste et listeNb, sauf
                        la liste dans laquelle on a clique.
                        On enleve/ajoute le record (de)selectionne a la liste des
                        records selectionnes. 

GLOBALES AFFECTES:      xs[wi].listeNb
                        xs[wi].liste
                        xs[wi].nbRecsSelect

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static XtCallbackProc HighlightFields(Widget  w, caddr_t unused1, caddr_t call_data)
{
   wordint                  i, j;                                      /* Compteurs.                                */
   wordint                  numItem;                                   /* La position de l'item (de)selectionne.    */
   wordint                  *itemPos;
   unsigned char selectionPolicy;
   XmListCallbackStruct *item = (XmListCallbackStruct *)call_data; /* Pointeur a la structure envoyee
                                                                      par le widget w.                          */
   Arg args[2];

   i = 0;
   XtSetArg(args[i], XmNselectionPolicy, &selectionPolicy); i++;
   XtGetValues(w, args, i);

   switch(selectionPolicy)
      {
      case XmBROWSE_SELECT:
      numItem = 1;
      xs[wi].nbRecsSelect     = 1;
      xs[wi].indRecsSelect[0] = xs[wi].indRecsFiltres[item->item_position-1];
      break;
      
      default:
      numItem = item->selected_item_count;
      itemPos = item->selected_item_positions;
      
      xs[wi].nbRecsSelect     = numItem;
      for (i=0; i < numItem; i++)
         {
         xs[wi].indRecsSelect[i] = xs[wi].indRecsFiltres[itemPos[i]-1];
         }
      break;
      }

   AfficherNbSelect(numItem);

   
   }
/*======================================================================================================*/


/**
************************************************************

NOM:                    ActiverSelWidgets()

FONCTION:               Rendre active la fenetre principale.

APPELE PAR:             XSelectstdActiver()
                        EnleverInfoFiltres() apres avoir enleve le popup.

METHODE:                Rendre chaque widget de la fenetre principale Sensitive.

GLOBALES AFFECTES:      Les widgets qui sont visibles dans la fenetre principale.

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint ActiverSelWidgets()
{
   wordint j;       /* Compteur.                                    */
   Arg args[5]; /* Resources des widgets qui sont utilisees.    */
   wordint i;       /* Nombre de resources utilisees.               */

   i = 0;
   XtSetArg(args[i], XmNsensitive, True); i++;

   XtSetValues(xs[wi].ok, args, i);
   XtSetValues(xs[wi].effacerFiltres, args, i);
   XtSetValues(xs[wi].liste, args, i);

   if (xs[wi].typeSelection != SELECTION_SIMPLE)
      {
      XtSetValues(xs[wi].deselectionnerRecs, args, i); 
      }
   }
/*======================================================================================================*/
/**
********************************************************************************

NOM:                    AfficherListe()

FONCTION:               Mettre a jour une liste.

APPELE PAR:             EffacerFiltres()

METHODE:                On specifie quelle liste doit etre affichee,
                        et quel est le nombre de records affiches.

GLOBALES AFFECTES:      xs[wi].filtresListeDes
                        xs[wi].filtresListeCles
                        xs[wi].listeNb
                        xs[wi].liste
VALEUR RETOUNEE:       Aucune.

------------------------------------------------------------------------------*/

void AfficherListe(Widget w, XmStringTable items, wordint nbItems)
{
   wordint nb, cnt; /* Nombre d'items visibles.                     */
   wordint i;       /* Nombre de resources utilisees.               */
   Arg args[5]; /* Resources des widgets qui sont utilisees.    */

   if (nbItems <= 0)
      cnt = 0;
   else
      cnt = nbItems;

   i = 0;
   XtSetArg(args[i], XmNitems, items); i++;
   XtSetArg(args[i], XmNitemCount, cnt); i++;
   XtSetValues(w, args, i);
   XmListSetPos(w, 1);

   }
/*======================================================================================================*/
/**
************************************************************

NOM:                    AfficherNbSelect()

FONCTION:               Afficher le nombre de records selectionnes.

APPELE PAR:             HighlightFields()
                        DeselectionnerRecords()
                        EffacerFiltres()
METHODE:

GLOBALES AFFECTES:      Aucunes.

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

void AfficherNbSelect(wordint nb)
{
   Cardinal i;       /* Le nombre de resources changees.                            */
   Arg          args[5]; /* Les resources changees.                                     */
   char         str[32];  /* Chaine qui represente le nombre de records selectionnes.    */
   XmString     label;   /* XmString qui represente le nombre de records selectionnes.  */
   char         message[64];
   wordint lng;

   lng = c_getulng();

   MessageChargement(xs[wi].nbRecsFiltres, xs[wi].nbRecs);
   if (xs[wi].typeSelection != SELECTION_SIMPLE)
      {
      i = 0;
      sprintf(str, "%s%03d", label_nb[langue], nb);
      label = XmStringCreate(str, XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, label); i++;
      XtSetValues(xs[wi].selMsg, args, i);
      XmStringFree(label);
      }
   }
/*======================================================================================================*/

/**
******************************************************************************

NOM:                    AjouterCle()

FONCTION:               Ajouter une occurence d'une cle au tableau cles[].cleinfo[].ind.

APPELE PAR:             InitCles() pour chaque occurance suivant la premiere
                        d'une cle dans tableau[][].

METHODE:                On incremente le nombre d'occurences de la cle dans cles[].cleinfo[].
                        On ajoute l'indice ou la cle se trouve dans tableau[]
                        a la liste des indices cles[].cleinfo.ind.

GLOBALES AFFECTES:      xs[wi].cles[indDes].cleinfo[pos]

VALEUR RETOUNEE:       Aucune.

------------------------------------------------------------------------------*/

wordint AjouterCle(cleStruct cles[], wordint pos, wordint indDes, wordint indCle)
{
   wordint nbElem; /* Nombre d'elements dans la liste des indices ou la cle se trouve dans tableau[]. */

   nbElem                                       = ++cles[indDes].cleInfo[pos].nbOccur;
   cles[indDes].cleInfo[pos].ind                = (wordint *)realloc(cles[indDes].cleInfo[pos].ind, nbElem*sizeof(int));
   cles[indDes].cleInfo[pos].ind[nbElem - 1]    = indCle;
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    AjouterFiltre()

FONCTION:               Ajouter une cle a la liste des filtres (cles selectionnes).

APPELE PAR:             UpdateFiltres()

METHODE:                Seulment l'indice du descripteur et l'indice de la cle dans
                        le tableau du descripteur sont necessaires pour identifier
                        quelle cle a ete selectionne.

GLOBALES AFFECTES:      xs[wi].filtres[xs[wi].nbFiltres]
                        xs[wi].nbFiltres

VALEUR RETOUNEE:       Aucune.

------------------------------------------------------------------------------*/

/*indDes Indice indiquant dans quel tableau de descripteur la cle se trouve.      */
/* indCle; /* Indice indicant la position de la cle dans le tableau du descripteur.    */
wordint AjouterFiltre(wordint indDes, wordint indCle)
{
   xs[wi].filtres[xs[wi].nbFiltres].indDes = indDes;
   xs[wi].filtres[xs[wi].nbFiltres].indCle = indCle;
   xs[wi].nbFiltres++;
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    AjouterNouvelleCle()

FONCTION:               Ajouter une nouvelle cle au tableau cles[].cleinfo[].

APPELE PAR:             InitCles() pour chaque nouvelle cle trouvee dans tableau[][].

METHODE:                On incremente le nombre de cles contenu dans cles[].cleinfo[].
                        On reserve l'espace pour la nouvelle cle.
                        Notez que cles[].cleinfo[].nom et cles[].cleinfo[].val
                        sont les seuls endroits ou le nom de la cle est gardee.
                        On garde la representation XmString du nom pour permettre
                        de placer le nom dans les listes de cles et de fichiers standard.
                        On garde la representation string du nom pour faciliter
                        la comparaison et le tri des cles.

GLOBALES AFFECTES:      xs[wi].cles[indDes]
                        xs[wi].nbCles

VALEUR RETOUNEE:       Aucune.

------------------------------------------------------------------------------*/

wordint AjouterNouvelleCle(char *val, wordint len, cleStruct cles[], wordint indDes, wordint indCle)
{
   wordint ClesInd; /* Le nombres de cles dans le tableau de descripteur. */

   ClesInd = cles[indDes].nbCles;
   cles[indDes].nbCles++;
   xs[wi].nbCles++;
  
   if (ClesInd == 0)
      cles[indDes].cleInfo = (cleInfoStruct *)calloc(1, sizeof(cleInfoStruct));
   else
      cles[indDes].cleInfo = (cleInfoStruct *)realloc(cles[indDes].cleInfo, 
                                                       ((ClesInd + 1)*sizeof(cleInfoStruct)));
   
   cles[indDes].cleInfo[ClesInd].ind            = (wordint *)calloc(1, sizeof(int));
   cles[indDes].cleInfo[ClesInd].ind[0]         = indCle;
   cles[indDes].cleInfo[ClesInd].nbOccur        = 1;
   cles[indDes].cleInfo[ClesInd].val            = (char *)calloc(len + 1, sizeof(char));
/*printf("val: %s\n", val); */
   strncpy(cles[indDes].cleInfo[ClesInd].val, val, len);
/*   cles[indDes].cleInfo[ClesInd].val[len + 1] = NULL;*/
   cles[indDes].cleInfo[ClesInd].val[len]       = '\0';
   cles[indDes].cleInfo[ClesInd].xmVal            = XmStringCreate(cles[indDes].cleInfo[ClesInd].val,
                                                                                XmSTRING_DEFAULT_CHARSET);
   }
/*======================================================================================================*/

/**
******************************************************************************

NOM:                    ChercherCle()

FONCTION:               Verifier si une cle a deja ete entree dans
                        le tableau de descripteur, ou si il faudra l'ajouter.

APPELE PAR:             InitCles() a l'initialisation des tableaux de descripteurs.

METHODE:                On compare le nom de la cle en question avec les noms deja
                        dans le tableau du descripteur qui n'est pas ordonne.
                        Dans le cas ou la cle n'a pas ete trouvee dans le tableau,


GLOBALES AFFECTES:      Aucunes.

VALEUR RETOURNEE:      Si la cle est trouvee, ChercherCle retourne la position
                        dans cles[indes].cleinfo ou la cle se trouve.  Retourne
                        -1 si la cle n'est pas trouvee.

------------------------------------------------------------------------------*/

wordint ChercherCle(char *val, wordint len, cleStruct cles[], wordint indDes)
{
   wordint i;       /* Position dans le tableau du descripteur.        */
   wordint nbCles;  /* Nombres de cles dans le tableau du descripteur. */
   
   if ((nbCles = cles[indDes].nbCles) == 0)
      return -1;

   i = nbCles - 1;
   while (i > -1 && strncmp(val, cles[indDes].cleInfo[i].val, len) != 0)
         i--;

   if (i > -1)
      return i;
   else 
      return -1;
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    ComparerCles()

FONCTION:               Determiner lequel de deux noms de cles precede l'autre
                        dans l'ordre du dictionnaire.

APPELE PAR:             qsort() de InitCles() quand on a fini de lire toutes les cles.

METHODE:                On fait un strcmp des deux noms.

GLOBALES AFFECTES:      Aucunes.

VALEUR RETOURNEE:      Il est theoriquement impossible que deux cles soient
                        repetees dans la liste.  On retourne un entier negatif si
                        le premier nom precede le second, autrement on retourne un
                        entier positif.

------------------------------------------------------------------------------*/

wordint ComparerCles(cleInfoStruct *cleInfo1, cleInfoStruct *cleInfo2)
{
   return (strcmp(cleInfo1->val, cleInfo2->val));
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    ComparerFiltres()

FONCTION:               Determiner lequel des deux filtres precede l'autre
                        dans l'ordre dans lequel ils apparaissent dans le menu.

APPELE PAR:             qsort() de InitFiltresInfo() 

METHODE:                On commence par comparer les descripteurs.  Si les
                        descripteurs sont egals, on compare alors les cles.

GLOBALES AFFECTES:      Aucunes.

VALEUR RETOURNEE:      Il est theoriquement impossible que deux filtres soient
                        repetees dans la liste.  On retourne -1 si le premier
                        filtre precede le second, autrement on retourne 1.

------------------------------------------------------------------------------*/

wordint ComparerFiltres(filtresStruct *filtre1,  filtresStruct *filtre2)
{
   if (filtre1->indDes < filtre2->indDes)
      return -1;
   else if (filtre1->indDes > filtre2->indDes)
        return 1;
   else if (filtre1->indCle < filtre2->indCle)
        return -1;
   else
        return 1;
   }
/*======================================================================================================*/
/**
*********************************************************************************

NOM:                    DesactiverSelWidgets()

FONCTION:               Desactiver la fenetre principale.

APPELE PAR:             AfficherInfoFiltres() quand on fait apparaitre le popup.

METHODE:                Rendre Insensitive touts les widgets visibles de la fenetre principale.

GLOBALES AFFECTES:      Les widgets visibles dans la fenetre principale.

VALEUR RETOURNEE:      Acune.

------------------------------------------------------------------------------*/

wordint DesactiverSelWidgets()
{
   wordint j;       /* Compteur.                                    */
   Arg args[5]; /* Resources des widgets qui sont utilisees.    */
   wordint i;       /* Nombre de resources utilisees.               */
   
   i = 0;
   XtSetArg(args[i], XmNsensitive, False); i++;

   XtSetValues(xs[wi].ok, args, i);
   XtSetValues(xs[wi].effacerFiltres, args, i);
   XtSetValues(XtParent(xs[wi].liste), args, i);

   if (xs[wi].typeSelection != SELECTION_SIMPLE)
      {
      XtSetValues(xs[wi].deselectionnerRecs, args, i); 
      }

   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    EffacerFiltresSeulement()

FONCTION:               Deselectionner touts les toggles dans les pulldowns.

APPELE PAR:             EffacerFiltres()

METHODE:                On met XmNset a False pour chaque toggle qui est dans la
                        liste des criteres selectionnes.

GLOBALES AFFECTES:      xs[wi].menuItems[][]

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static XtCallbackProc EffacerFiltresSeulement(Widget w, caddr_t u1, caddr_t u2)
{
   wordint i;
   
   for (i = 0; i < xs[wi].nbDes; i++)
      {
      if (xs[wi].cles[i].nbCles > 1)
         {
         XmListDeselectAllItems(xs[wi].panListeItems[i]);
         XmListSetPos(xs[wi].panListeItems[i], 1);
         }
         
      }
   }
/*======================================================================================================*/
/**
************************************************************

NOM:                    EnleverFiltre()

FONCTION:               Enleve un filtre de la liste des filtres.

APPELE PAR:             UpdateFiltres() quand une cle d'un pulldown est
                        deselectionnee.

METHODE:                On trouve la position du filtre dans la liste des filtres,
                        et on pousse touts les filtres de la liste qui succedent
                        d'une case vers la gauche.

GLOBALES AFFECTES:      xs[wi].filtres[]
                        xs[wi].nbFiltres

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint EnleverFiltre(wordint indDes, wordint indCle)
{
   wordint i;         /* Compteur.                                          */
   wordint indFiltre; /* La position du filtre dans la liste des filtres.   */
   
   indFiltre = 0;
   while (indDes != xs[wi].filtres[indFiltre].indDes || indCle != xs[wi].filtres[indFiltre].indCle)
      indFiltre++;

   xs[wi].nbFiltres--;

   for (i = indFiltre; i < xs[wi].nbFiltres; i++)
      xs[wi].filtres[i] = xs[wi].filtres[i + 1];
   }
/*======================================================================================================*/
/**
************************************************************

NOM:                    FiltrerRecords()

FONCTION:               De choisirs les records qui devront etre mis dans
                        la liste des records filtres, a partir des filtres
                        qui sont dans la liste des filtres.

APPELE PAR:             UpdateFiltres()

METHODE:

GLOBALES AFFECTES:      xs[wi].indRecsFiltres
                        xs[wi].nbRecsFiltres

VALEUR RETOURNEE:      Le nombre de records apres la filtration.       

------------------------------------------------------------------------------*/

wordint FiltrerRecords(wordint sel[], wordint *nbsel, XmString **recs, cleStruct cles[], filtresStruct *filtres, wordint nbRecs, wordint nbDes, wordint  nbFiltres)
{
   wordint                  i, j, k;                          /* Compteurs. */
   wordint                  clePasTrouvee;                    /* Vrai quand on trouve la cle. */
   wordint                  nbFiltresDistincts;               /*  */
   filtresInfoStruct    filtresInfo[NB_MAX_DESCRIPTEURS]; /*  */
   
   *nbsel = 0;
   nbFiltresDistincts = InitFiltresInfo(filtresInfo, filtres, nbFiltres);

   if (nbFiltres == 0)
      return 0;

   for (i = 0; i < nbRecs; i++)
      {
      j = 0;
      do
         {
         k = 0;
         clePasTrouvee = True;
         while (k < filtresInfo[j].nbFiltresListe &&
               (clePasTrouvee = cles[filtresInfo[j].indDes].cleInfo[filtresInfo[j].liste[k]].xmVal !=
                                recs[filtresInfo[j].indDes][i]))
                k++;

         j++;
         } while (j < nbFiltresDistincts && !clePasTrouvee);
      
      if (!clePasTrouvee)
         sel[(*nbsel)++] = i;
      }
   
   for (i = 0; i < NB_MAX_DESCRIPTEURS; i++)
      if (filtresInfo[i].liste != NULL)
         free(filtresInfo[i].liste);

   return *nbsel;
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    FreeCles()

FONCTION:               Recuperer la memoire utilisee par les cles.

APPELE PAR:             XSelectstdFermer()

METHODE:                On recupere la memoire prise par le nom de la cle,
                        et par la liste qui indique ou la cle apparait dans le
                        tableau de records.

GLOBALES AFFECTES:      xs[wi].cles

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/
        
wordint FreeCles(cleStruct cles[], wordint n)
{
   wordint i, j; /* Compteurs. */
   
   for (i = 0; i < n; i++)
      {
      for (j = 0; j < cles[i].nbCles; j++)
         {
         free(cles[i].cleInfo[j].val);
         free(cles[i].cleInfo[j].ind);
         XmStringFree(cles[i].cleInfo[j].xmVal);

         cles[i].cleInfo[j].val = NULL;
         cles[i].cleInfo[j].ind = NULL;
         cles[i].cleInfo[j].xmVal = NULL;
         }

      if (cles[i].cleInfo)
         {
         free(cles[i].cleInfo);
         cles[i].cleInfo = NULL;
         }

      cles[i].nbCles = 0;
      }

   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    LibererPointeurs()

FONCTION:               Recuperer la memoire utilisee par les tables.

APPELE PAR:             XSelectstdFermer()

METHODE:                On recupere la memoire prise par les malloc.

GLOBALES AFFECTES:      xs[wi].cles
                        xs[wi].recs
                        xs[wi].recsAffiches
                        xs[wi].recsFilresAffiches

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/
        
wordint LibererPointeurs()
{
   wordint i;
   
   LibererPanneauListe();
   FreeCles(xs[wi].cles, xs[wi].nbDes);
   Freerecs(&(xs[wi].recs), xs[wi].nbDes);
   FreerecsAffiches(&(xs[wi].recsAffiches), xs[wi].nbRecs);

   for (i=0; i < xs[wi].nbDes; i++)
      XmStringFree(xs[wi].titresMenus[i]);

   if (xs[wi].recsFiltresAffiches)
      {
      free(xs[wi].recsFiltresAffiches);
      xs[wi].recsFiltresAffiches = NULL;
      }

   if (xs[wi].indRecsFiltres)
      {
      free(xs[wi].indRecsFiltres);
      xs[wi].indRecsFiltres = NULL;
      }

   if (xs[wi].indRecsSelect)
      {
      free(xs[wi].indRecsSelect);
      xs[wi].indRecsSelect = NULL;
      }

   if (xs[wi].filtres)
      {
      free(xs[wi].filtres);
      xs[wi].filtres = NULL;
      }

   }

/*======================================================================================================*/

LibererPanneauListe()
{
   wordint i, j;

   for (i = 0; i < xs[wi].nbDes; i++)
      {
      if (xs[wi].cles[i].nbCles > 1)
         {
         XtDestroyWidget(xs[wi].panListeItems[i]);
         XtDestroyWidget(xs[wi].panLabel[i]);
         XtDestroyWidget(xs[wi].panRowCol[i]);
         }
      
      }

   XtDestroyWidget(xs[wi].panListeForm);
   XtDestroyWidget(xs[wi].panRetour);
   XtDestroyWidget(xs[wi].panEffacerFiltres);
   XtDestroyWidget(xs[wi].panForme);
   XtDestroyWidget(xs[wi].panFrame);
   XtDestroyWidget(xs[wi].panListe);
   
   
   }

/**
******************************************************************************

NOM:                    Freerecs()

FONCTION:               Liberer la memoire prise par les records.

APPELE PAR:             XSelectstdFermer()

METHODE:                On recupere la memoire prise par les pointeurs vers les
                        noms des cles.

GLOBALES AFFECTES:      xs[wi].recs

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/
   
wordint Freerecs(XmString *(**recs), wordint nbDes)
{
   wordint i; /* Compteur. */
   
   if (*recs)
      {
      for (i = 0; i < nbDes; i++)
         free((*recs)[i]);
      
      free(*recs);
      *recs = NULL;
      }
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    FreerecsAffiches()

FONCTION:               Liberer la memoire prise par les records.

APPELE PAR:             XSelectstdFermer()

METHODE:                On recupere la memoire prise par les pointeurs vers les
                        noms des cles.

GLOBALES AFFECTES:      xs[wi].recs

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/
   
void FreerecsAffiches(XmString **recsAffiches, wordint nbDes)
{
   wordint i; /* Compteur. */
   
   if (*recsAffiches)
      {
      
      for (i = 0; i < nbDes; i++)
         {
         XmStringFree((*recsAffiches)[i]);
         }
      
      free(*recsAffiches);
      *recsAffiches = NULL;
      }
   }

/*======================================================================================================*/
/**
************************************************************

NOM:                    InitCles()

FONCTION:               Initializer le tableau des cles.

APPELE PAR:             XSelectstdOuvrir()

METHODE:                On regarde chaque element dans table[].
                        Si on decouvre une nouvelle cle, on l'ajoute
                        a la liste des cles.  Si la cle a deja ete trouvee,
                        on sauvegarde dans la liste de la cle la position
                        ou la cle se trouve dans la liste des records (fichiers standards).
                        Chaque tableau de descripteur est ordonne.

GLOBALES AFFECTES:      xs[wi].cles
                        xs[wi].nbCles

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint InitCles(char *tableau, wordint  table[][3], cleStruct cles[], wordint nbRecs, wordint nbDes)
{
   wordint  i, j;    
   wordint  offset;  
   wordint  len;     
   wordint  pos;     
   unsigned char *tempAdr;

   char tmpStr[16];

   if (xs[wi].lastNbRecs == 0)
      {
      xs[wi].nbCles = 0;
      for (i = 0; i < nbDes; i++) 
         {
         cles[i].nbCles   = 0;
         }
      }

   for (i = 0; i < nbDes; i++) 
      {
      /*      tempAdr          = (unsigned char *)(table[i][ADRESSE]); */
      tempAdr          = (char *) tableau;
      tempAdr += table[i][ADRESSE];
      offset           = table[i][OFFSET];
      len              = table[i][LENGTH];
      for (j = 0; j < nbRecs; j++)
         {
         if ((pos = ChercherCle(tempAdr, len, cles, i)) == -1)
            AjouterNouvelleCle(tempAdr, len, cles, i, j+xs[wi].lastNbRecs);
         else
            AjouterCle(cles, pos, i, j+xs[wi].lastNbRecs);
         
         tempAdr += offset;
         }
      }
/**
   for (i = 0; i < nbDes; i++)
      qsort((char *)cles[i].cleInfo, cles[i].nbCles, sizeof(cleInfoStruct), ComparerCles);
**/
  }
/*======================================================================================================*/
/**
************************************************************

NOM:                    InitFiltresInfo()

FONCTION:

APPELE PAR:             FiltrerRecords()

METHODE:

GLOBALES AFFECTES:

VALEUR RETOURNEE:

------------------------------------------------------------------------------*/

wordint InitFiltresInfo(filtresInfoStruct filtresInfo[], filtresStruct *filtres, wordint nbFiltres)
{
   wordint i, j;            /* Compteurs.                                                   */
   wordint indDesCourant;   /* L'indice du descripteur qui est courament en consideration.  */
   wordint indListe;        /*  */

   if (nbFiltres == 0)
      return 0;
   
   for (i = 0; i < NB_MAX_DESCRIPTEURS; i++)
      {
      filtresInfo[i].indDes             = -1;
      filtresInfo[i].nbFiltresListe     = 0;
      filtresInfo[i].liste              = NULL;
      }
        
   qsort((char *)filtres, nbFiltres, sizeof(filtresStruct), ComparerFiltres);
   indDesCourant = 0;
   filtresInfo[indDesCourant].liste = (wordint *)calloc(xs[wi].cles[filtres[0].indDes].nbCles, sizeof(int));

   for (i = 0; i < xs[wi].cles[filtres[0].indDes].nbCles; i++)
      filtresInfo[0].liste[i] = -1;

   indListe                                     = 0;
   filtresInfo[indDesCourant].indDes            = filtres[0].indDes;
   filtresInfo[indDesCourant].nbFiltresListe    = 1;
   filtresInfo[indDesCourant].liste[0]          = filtres[0].indCle;
   
   for (i = 1; i < nbFiltres; i++)
      {
      if (filtresInfo[indDesCourant].indDes != filtres[i].indDes)
         {
         indDesCourant++;
         filtresInfo[indDesCourant].liste = (wordint *)calloc(xs[wi].cles[filtres[i].indDes].nbCles, sizeof(int));


         for (j = 0; j < xs[wi].cles[filtres[i].indDes].nbCles; j++)
            filtresInfo[indDesCourant].liste[j] = -1;

         indListe                                       = 0;
         filtresInfo[indDesCourant].indDes              = filtres[i].indDes;
         filtresInfo[indDesCourant].nbFiltresListe      = 1;
         filtresInfo[indDesCourant].liste[indListe]     = filtres[i].indCle;
         }
      else
         {
         indListe++;
         filtresInfo[indDesCourant].nbFiltresListe++;
         filtresInfo[indDesCourant].liste[indListe] = filtres[i].indCle; 
         }
      }
   
   indDesCourant++;
   return indDesCourant;
   }
/*======================================================================================================*/
/**
************************************************************

NOM:                    InitRecs()

FONCTION:               Initialiser le tableau des records (fichiers standards).

APPELE PAR:             XSelectstdOuvrir()

METHODE:                On reserve l'espace pour les pointeurs du tableau des
                        records.  Ils pointent vers les XmStrings dans le tableau des cles.

GLOBALES AFFECTES:      xs[wi].recs

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint InitRecs(XmString *(**recs), cleStruct cles[], wordint nbRecs, wordint nbDes)
{
   wordint i, j, k; /* Compteurs. */
   
   char *text;
   
   for (i = 0; i < nbDes; i++) 
      for (j = 0; j < cles[i].nbCles; j++)
         for (k = 0; k < cles[i].cleInfo[j].nbOccur; k++)
            {
            (*recs)[i][cles[i].cleInfo[j].ind[k]] = cles[i].cleInfo[j].xmVal;
            }
   
   }
/*======================================================================================================*/
/**
 ************************************************************
 
NOM:                    InitRecsAffiches()

FONCTION:               Initialiser le tableau des records affiches (fichiers standards).

APPELE PAR:             XSelectstdOuvrir()

METHODE:                On reserve l'espace pour les pointeurs du tableau des
                        records.

GLOBALES AFFECTES:      xs[wi].recsAffiches

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

void InitRecsAffiches(XmString **recsAffiches, char *tableau, wordint table[][3], wordint nbRecs)
{
   wordint  i;       /* Compteur.                            */
   char *tmpAdr; /* La cle de table courrament examinee. */
   wordint  offset, longueur, ioffset;

   ioffset = xs[wi].lastNbRecs;
   tmpAdr = (char *)tableau;
   offset = table[0][OFFSET];
   longueur = table[0][LENGTH];

   for (i = 0; i < nbRecs; i++)
       {
       tmpAdr[offset-1] = '\0';
       (*recsAffiches)[i+ioffset] = XmStringCreate(tmpAdr, XmSTRING_DEFAULT_CHARSET);
       tmpAdr += offset;
       }
   }
/*======================================================================================================*/

/**$
************************************************************

NOM:                    InitRecsFiltresAffiches()

FONCTION:               Initialiser le tableau des records filtres affiches (fichiers standards).

APPELE PAR:             XSelectstdOuvrir()

METHODE:                On reserve l'espace pour les pointeurs du tableau des
                        records.

GLOBALES AFFECTES:      xs[wi].recsFiltresAffiches

VALEURE RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

void InitRecsFiltresAffiches(XmString **recsFiltresAffiches, XmString *recsAffiches, wordint nbRecs)
{
   wordint  i;       /* Compteur.                            */

   if (*recsFiltresAffiches != NULL)
      free(*recsFiltresAffiches);

   *recsFiltresAffiches = (XmString *)calloc(nbRecs, sizeof(XmString));

   for (i = 0; i < nbRecs; i++)
       (*recsFiltresAffiches)[i] = recsAffiches[i];
   }
/*======================================================================================================*/


/**
******************************************************************************

NOM:                    TrouverLongueurMot()

FONCTION:               Trouver la longueur du string.

APPELE PAR:             InitTitresMenu()

METHODE: 

GLOBALES AFFECTES:      Aucune.

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static void TrouverLongueurMot(wordint *longueur, XmString mot)
{ wordint i;
  char *text;

  /** transformer le XmString en string **/
  XmToS(mot, &text);

  /** trouver la longueur du string **/
  *longueur = strlen(text);
}

/*======================================================================================================*/

/**
******************************************************************************

NOM:                    InitTitresMenus()

FONCTION:               Creer les XmString qui sont utilises comme noms de descripteurs.

APPELE PAR:             XSelectstdOuvrir()

METHODE:

GLOBALES AFFECTES:      xs[wi].titresMenus[]

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint  InitTitresMenus(char *idents[], wordint nbDes)
{
   wordint i; /* Compteur. */

   for (i = 0; i < nbDes; i++)
    {
     xs[wi].titresMenus[i] = XmStringCreate(idents[i], XmSTRING_DEFAULT_CHARSET);
    }
   
}
/*======================================================================================================*/
/**
 ************************************************************
 
 NOM:                    InitWidgetsAll().
 
 FONCTION:               Creer touts les widgets utilises par le programme.
 
 APPELE PAR:             XSelectstdOuvrir()
 
 METHODE:
 
 GLOBALES AFFECTES:      Touts les widgets.
 
 VALEUR RETOURNEE:      Aucune.
 **/

wordint InitWidgetsAll1(XmString TitresMenus[], wordint nbRecsFiltres, wordint nbDes)
{
   InitWidgetsBasic();
   InitWidgetsForm();
   InitWidgetsBouton();
   
   }
/*======================================================================================================*/


wordint InitWidgetsAll2(XmString TitresMenus[], wordint nbRecsFiltres, wordint nbDes)
{
   InitWidgetsMenu(TitresMenus, nbDes);
   InitWidgetsRec(nbRecsFiltres, nbDes);
   }
/*======================================================================================================*/
/**
 ***********************************************************************
 
 NOM:                    InitWidgetsBasic()

FONCTION:               Initialiser le toplevel et le popup shell

APPELE PAR:             InitWidgetsAll()

METHODE:

GLOBALES AFFECTES:      xs[wi].toplevel
                        xs[wi].popUpShell

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint InitWidgetsBasic()
{
   Arg args[5]; /* Resources des widgets qui sont utilisees.    */
   wordint i;       /* Nombre de resources utilisees.               */

   wordint lng;
   char nomShell[128];

/*..........................topLevel..............................................*/

   strcpy(nomShell, XtName(SuperWidget.topLevel));
   lng = c_getulng();
   if (lng == 0)
      strcat(nomShell, "SelecteurEnr");
   else
      strcat(nomShell, "RecordSelector");

   i = 0;
   XtSetArg(args[i], XmNallowShellResize, True); i++;
   xs[wi].topLevel = XtAppCreateShell(nomShell, nomShell, 
                                        applicationShellWidgetClass,
                                        XtDisplay(SuperWidget.topLevel),
                                        args, i);

   }

/*======================================================================================================*/
/**
 ******************************************************************************
 
 NOM:                    InitWidgetsBouton()
 
 FONCTION:               Creer les boutons de la fenetre principale.
 
 APPELE PAR:             InitWidgetsAll()
 
 METHODE:
 
 GLOBALES AFFECTES:      xs[wi].apply
 xs[wi].applyClose
 xs[wi].close
 xs[wi].deselectionnerRecs
 xs[wi].effacerFiltres
 xs[wi].infoFiltres
 xs[wi].boutonform
 
 VALEUR RETOURNEE:      Aucune.
 
 ------------------------------------------------------------------------------*/

Widget frame = NULL;
void InitWidgetsBouton()
{
   Arg          args[10];/* Resources des widgets qui sont utilisees.   */
   Cardinal i;       /* Nombre de resources utilisees.              */
   wordint          h;       /* La hauteur de boutonform.                  */
   XmString     label;   /* Le nom de chaque bouton.                    */
   unsigned char         str[32];
   XmFontList fontListe;
   XmString infoLabelWidth;
   wordint largeurTexte;
   
/*   Arg args[10];
   char bidon[32];
   XmString labelBidon[32], label1, label2;

   strcpy(bidon, " ");
   for (i=1; i < 32; i++)
      {
      labelBidon[i] = XmStringCreate(bidon, XmSTRING_DEFAULT_CHARSET);
      strcat(bidon, " ");
      }
   
   i = 0;
   XtSetArg(args[i], XmNfontList, &fontListe); i++;
   XtGetValues(xs[wi].ok, args, i);*/
   
   /*................................ok.....................................*/
   
   i = 0;
   label = XmStringCreateLtoR("Ok", XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   xs[wi].ok = (Widget) XmCreatePushButton(xs[wi].boutonform,  "Ok", args, i);
   XtManageChild(xs[wi].ok);
   XmStringFree(label);
   
   /*..........................deselectionnerRecs..............................*/
   
   if (xs[wi].typeSelection != SELECTION_SIMPLE)
      {
      i = 0;
      label = XmStringCreateLtoR(label_desact[langue], XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, label); i++;
      xs[wi].deselectionnerRecs = (Widget)XmCreatePushButton(xs[wi].boutonform, "desactiver items", args, i);
      XtManageChild(xs[wi].deselectionnerRecs);
      XmStringFree(label);
      }
   
   /*..........................effacerFiltres.....................................*/
   
   i = 0;
   label = XmStringCreateLtoR(label_efface[langue], XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[i], XmNlabelString, label); i++;
   xs[wi].effacerFiltres = (Widget)XmCreatePushButton(xs[wi].boutonform, "Effacer criteres", args, i);
   XtManageChild(xs[wi].effacerFiltres);
   XmStringFree(label);

   /*..........................infoLabel.....................................*/
   
   i = 0;
   xs[wi].frameInfoLabel = (Widget)XmCreateFrame(xs[wi].boutonform, "frame", args, i);
   XtManageChild(xs[wi].frameInfoLabel);
   
   i = 0;
   XtSetArg(args[i], XmNfontList, &fontListe); i++;
   XtGetValues(xs[wi].ok, args, i);
   
   infoLabelWidth = XmStringCreate("999999/999999", XmSTRING_DEFAULT_CHARSET);
   
   i = 0;
   XtSetArg(args[i], XmNwidth, XmStringWidth(fontListe, infoLabelWidth)); i++;
   xs[wi].infoLabel = (Widget)XmCreateLabel(xs[wi].frameInfoLabel, "   ", args, i);
   XtManageChild(xs[wi].infoLabel);
   
   if (xs[wi].typeSelection != SELECTION_SIMPLE)
      {
      i = 0;
      xs[wi].nbFrame = (Widget)XmCreateFrame(xs[wi].boutonform,
                                             "label Frame",
                                             args, i);
      XtManageChild(xs[wi].nbFrame);
      
      /*..........................nbForm........................................*/
      
      i = 0;
      XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++;
      XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
      
      XtSetArg(args[i], XmNmarginHeight, 0); i++;
      XtSetArg(args[i], XmNmarginWidth, 0); i++;
      
      xs[wi].nbForm = (Widget)XmCreateRowColumn(xs[wi].nbFrame, "label Frame", args, i);
      XtManageChild(xs[wi].nbForm);
      
      /*...........................selMsg.......................................*/
      
      i = 0;
      sprintf(str, "%s%03d", label_nb[langue], 0);
      label = XmStringCreate(str, XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, label); i++;
      xs[wi].selMsg = (Widget)XmCreateLabelGadget(xs[wi].nbForm,
                                                  "message",
                                                  args, i);
      XtManageChild(xs[wi].selMsg);
      XmStringFree(label);
      
      /*...........................nbSel.......................................*/
      
      }
   
   }


/*======================================================================================================*/

static XtCallbackProc MontrerDescripteurs(Widget w, caddr_t u1, caddr_t u2)
{
   
   AjusterPositionForme(xs[wi].panListe, xs[wi].topLevel);
   
   
   XtUnmanageChild(xs[wi].panListe);
   XtManageChild(xs[wi].panListe);
   XtMapWidget(xs[wi].panListe);
   }


static XtCallbackProc SelectionListeTerminee(Widget w, caddr_t u1, caddr_t u2)
{
   wordint zero = 0;
   UpdateFiltres();
   AfficherNbSelect(zero);
   XtUnmanageChild(xs[wi].panListe);
   XtUnmapWidget(xs[wi].panListe);
   }


/**
***********************************************************************

NOM:                    InitWidgetsCallback()

FONCTION:               Ajouter touts les callbacks aux widgets.

APPELE PAR:             InitWidgetsAll()

METHODE:

GLOBALES AFFECTES:      xs[wi].apply
                        xs[wi].close
                        xs[wi].applyClose
                        xs[wi].deselectionnerRecs
                        xs[wi].effacerFiltres
                        xs[wi].infoFiltres
                        xs[wi].infoFiltresOk
                        xs[wi].vscroll
                        xs[wi].listeNb
                        xs[wi].liste
                        xs[wi].menuItems[][]

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint InitWidgetsCallback(wordint nbDes)
{
   wordint i, j;  /* Compteurs.                                             */
   wordint index; /* Utilise pour encoder les indices de menuItems[][].     */
   
   if (xs[wi].typeSelection != SELECTION_SIMPLE)
      {
      XtAddCallback(xs[wi].deselectionnerRecs, XmNactivateCallback, (XtCallbackProc) DeselectionnerRecords,     NULL);
      }

   XtAddCallback(xs[wi].effacerFiltres,  XmNactivateCallback, (XtCallbackProc)EffacerFiltres,            NULL);
   XtAddCallback(xs[wi].liste, XmNdefaultActionCallback, (XtCallbackProc)Ok,  NULL); 
   XtAddCallback(xs[wi].liste,   XmNbrowseSelectionCallback, (XtCallbackProc) HighlightFields,  NULL); 
   XtAddCallback(xs[wi].liste, XmNmultipleSelectionCallback,  (XtCallbackProc)HighlightFields,  NULL); 
   XtAddCallback(xs[wi].liste, XmNextendedSelectionCallback,  (XtCallbackProc)HighlightFields,  NULL); 
   XtAddCallback(xs[wi].ok, XmNactivateCallback, (XtCallbackProc) Ok, NULL);
   XtAddCallback(xs[wi].panRetour, XmNactivateCallback, (XtCallbackProc) SelectionListeTerminee, NULL);
   XtAddCallback(xs[wi].panEffacerFiltres, XmNactivateCallback, (XtCallbackProc) EffacerFiltresSeulement, NULL);
   
   }
/*======================================================================================================*/
/**
******************************************************************************

NOM:                    InitWidgetsFiltre()

FONCTION:               Creer les widgets du popup.

APPELE PAR:             InitWidgetsAll()

METHODE:

GLOBALES AFFECTES:      xs[wi].buttonform
                        xs[wi].filtresLabel
                        xs[wi].infoFiltresOk
                        xs[wi].formFiltres
                        xs[wi].filtresListeCles
                        xs[wi].filtresListeDes

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

/**
NOM:                    InitWidgetsForm()

FONCTION:               Creer les container widgets de la fenetre principale.

APPELE PAR:             InitWidgetsAll()

METHODE:

GLOBALES AFFECTES:      xs[wi].pan
                        xs[w].boutonform
                        xs[wi].menuform
                        xs[wi].listeform

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/
Widget ScrolledWin, hsb;

void InitWidgetsForm()
{
   Arg args[10];        /* Resources des widgets qui sont utilisees.    */
   wordint i;       /* Nombre de resources utilisees.               */
   XmString label;

/*............................pan................................*/

   i = 0;
   XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++; 
   XtSetArg(args[i], XmNnumColumns, 1); i++; 
   XtSetArg(args[i], XmNallowResize, True); i++; 

   xs[wi].pan = (Widget)XmCreateForm(xs[wi].topLevel,
                                        "pan-1",  
                                         args, i);
   XtManageChild(xs[wi].pan);

   
   if (xs[wi].statutBoutonFermer == BOUTON_FERMER_ACTIF)
      {
      i = 0;
      label = XmStringCreateLtoR("Fermer", XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[i], XmNlabelString, label); i++;
      XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
      XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
      
      xs[wi].fermer = (Widget)XmCreatePushButton(xs[wi].pan, "Fermer", args, i);
      XtManageChild(xs[wi].fermer);
      XmStringFree(label);
      XtAddCallback(xs[wi].fermer, XmNactivateCallback, (XtCallbackProc) FermerSelecteur, NULL);

      XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
      XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
      XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
      XtSetArg(args[i], XmNtopWidget, xs[wi].fermer); i++;
      xs[wi].separateurBoutonFermer = (Widget)XmCreateSeparator(xs[wi].pan, "Fermer", args, i);
      XtManageChild(xs[wi].separateurBoutonFermer);
      }


/*.............................boutonform................................*/

   i = 0;
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++; 
   XtSetArg(args[i], XmNnumColumns, 2); i++; 
   if (xs[wi].statutBoutonFermer == BOUTON_FERMER_ACTIF)
      {
      XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
      XtSetArg(args[i], XmNtopWidget, xs[wi].separateurBoutonFermer); i++;
      }
   else
      {
      XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
      }

   XtSetArg(args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   xs[wi].boutonform = (Widget)XmCreateRowColumn(xs[wi].pan,
                                        "boutonform",
                                        args, i);
   XtManageChild( xs[wi].boutonform );

/*.............................menuform...................................*/

   i = 0;
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++; 
   XtSetArg(args[i], XmNtopWidget, xs[wi].boutonform); i++; 
   XtSetArg(args[i], XmNadjustLast, False); i++;
   XtSetArg(args[i], XmNadjustMargin, False); i++;
   XtSetArg(args[i], XmNmarginWidth, 0); i++;


   xs[wi].menuform = (Widget)XmCreateForm(xs[wi].pan, 
                                        "menuform",
                                        args, i);

   XtManageChild(xs[wi].menuform);



/*.............................listeform...................................*/


   }
/*======================================================================================================*/
/**
**********************************************************************************

NOM:                    InitWidgetsMenu().

FONCTION:               Creer les widgets du menubar et les pulldowns.

APPELE PAR:             InitWidgetsAll()

METHODE:

GLOBALES AFFECTES:      xs[wi].panRowCol[]
                        xs[wi].panListeItems[]
                        xs[wi].menuItems[][]
                        xs[wi].menuform

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

void InitWidgetsListe()
{
   Arg args[16]; /* Resources des widgets qui sont utilisees.    */
   wordint i;       /* Nombre de resources utilisees.               */
   wordint j, k;    /* Compteurs.                                   */
   wordint h;       /* La hauteur de menuform.                      */
   char *text;
   XmString label;
   Position positionCourante;
   wordint width;
   XmStringTable tableItems;
   XmFontList fontListe;

   i = 0;
   xs[wi].panListe = (Widget)XmCreateFormDialog(xs[wi].topLevel, "popup", args, 0);

   i = 0;
   XtSetArg(args[i], XmNmarginWidth,  5); i++; 
   XtSetArg(args[i], XmNmarginHeight, 5); i++; 
   XtSetArg(args[i], XmNshadowThickness, 3); i++; 
   xs[wi].panFrame = (Widget)XmCreateFrame(xs[wi].panListe, "forme", args, i);
   XtManageChild(xs[wi].panFrame);

   i = 0;
   xs[wi].panForme = (Widget)XmCreateForm(xs[wi].panFrame, "forme", args, 0);
   XtManageChild(xs[wi].panForme);

   label = XmStringCreateLtoR(label_retour[langue], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNlabelString, label); i++; 
   xs[wi].panRetour    = (Widget)XmCreatePushButton(xs[wi].panForme, "Termine", args, i);
   XtManageChild(xs[wi].panRetour);

   XmStringFree(label);

   label = XmStringCreateLtoR(label_efface[langue], XmSTRING_DEFAULT_CHARSET);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_WIDGET); i++; 
   XtSetArg(args[i], XmNrightWidget, xs[wi].panRetour); i++; 
   XtSetArg(args[i], XmNlabelString, label); i++; 
   xs[wi].panEffacerFiltres    = (Widget)XmCreatePushButton(xs[wi].panForme, "Effacer", args, i);
   XtManageChild(xs[wi].panEffacerFiltres);

   XmStringFree(label);

   i = 0;
   XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++; 
   XtSetArg(args[i], XmNtopWidget, xs[wi].panRetour); i++; 
   XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++; 
   XtSetArg(args[i], XmNspacing, 0); i++; 
   XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++; 
   XtSetArg(args[i], XmNnumColumns, 1); i++; 
   xs[wi].panListeForm = (Widget)XmCreateRowColumn(xs[wi].panForme, "row", args, i);
   XtManageChild(xs[wi].panListeForm);

 
   for (j=0; j < xs[wi].nbDes; j++)
      {
      if (1 < xs[wi].cles[j].nbCles)
         {
         tableItems = calloc(xs[wi].cles[j].nbCles, sizeof(XmString));
         
         for (k = 0; k < xs[wi].cles[j].nbCles; k++)
            {
            tableItems[k] = xs[wi].cles[j].cleInfo[k].xmVal;
            }
         
         i = 0;
         XtSetArg(args[i], XmNpacking, XmPACK_TIGHT); i++; 
         XtSetArg(args[i], XmNorientation, XmVERTICAL); i++; 
         
         xs[wi].panRowCol[j] = (Widget)XmCreateRowColumn(xs[wi].panListeForm,
                                                     "panListe",
                                                     args, i);
         XtManageChild(xs[wi].panRowCol[j]);
         
         i = 0;
         XtSetArg(args[i], XmNlabelString, xs[wi].titresMenus[j]); i++;   
         xs[wi].panLabel[j] = (Widget)XmCreateLabel(xs[wi].panRowCol[j], "allo", args, i);
         
         XtManageChild(xs[wi].panLabel[j]);
         
         i=0;  
         XtSetArg(args[i], XmNvisibleItemCount, 12); i++; 
         XtSetArg(args[i], XmNscrollBarPlacement, XmTOP_LEFT); i++; 
         XtSetArg(args[i], XmNdoubleClickInterval, 700); i++; 
         XtSetArg(args[i], XmNselectionPolicy, XmMULTIPLE_SELECT); i++;
         XtSetArg(args[i], XmNlistSpacing, 0); i++;
         XtSetArg(args[i], XmNitems, tableItems); i++;
         XtSetArg(args[i], XmNitemCount, k); i++;
         
         xs[wi].panListeItems[j] = (Widget)XmCreateScrolledList(xs[wi].panRowCol[j],
                                                            "panListe",
                                                            args, i);
         XtManageChild(xs[wi].panListeItems[j]);
   free(tableItems);
         }
      }
   }


void InitWidgetsMenu(XmString TitresMenus[], wordint nbDes)
{
   Arg args[10]; /* Resources des widgets qui sont utilisees.    */
   wordint i;       /* Nombre de resources utilisees.               */
   wordint j, k;    /* Compteurs.                                   */
   wordint h;       /* La hauteur de menuform.                      */
   char *text;
   XmString label;
   Position positionCourante;
   wordint width;

   positionCourante = 0;
   for (j = 0; j < nbDes; j++)
      {
      i = 0;
      XtSetArg(args[i], XmNlabelString, TitresMenus[j]); i++;
      XtSetArg(args[i], XmNx, (Position)positionCourante); i++;
      XtSetArg(args[i], XmNspacing, 0); i++;
      XtSetArg(args[i], XmNshadowThickness, 0); i++;
      xs[wi].menus[j] = (Widget)XmCreatePushButton(xs[wi].menuform,
                                                          "menus",
                                                          args, i);
      XtAddCallback(xs[wi].menus[j],  XmNactivateCallback, MontrerDescripteurs, NULL);
      positionCourante += largeurMenus[j];
      
      }
   XtManageChildren(xs[wi].menus, j);
   }


/*======================================================================================================*/

/**
******************************************************************************

NOM:                    InitWidgetsRec().

FONCTION:               Creer les widgets qui forment la liste des records.

APPELE PAR:             InitWidgetsAll()

METHODE:

GLOBALES AFFECTES:      xs[wi].vscroll
                        xs[wi].formListe
                        xs[wi].listeform
                        xs[wi].listeNb
                        xs[wi].liste

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint InitWidgetsRec(wordint nbRecsFiltres,  wordint nbDes)
{
   Arg  args[15];       /* Resources des widgets qui sont utilisees.    */
   wordint  i;              /* Nombre de resources utilisees.               */
   wordint  j;              /* Compteurs.                                   */
   char nomListe[16];  /* Le nom d'un widget dans liste.               */
   wordint  nb, cnt;        /* Le nombre de records visibles.               */
   wordint lng;

/*.............................liste.....................................*/

   lng = c_getulng();
   if (lng == FRANCAIS)
      strcpy(nomListe, "listeRecords");
   else
      strcpy(nomListe, "recordList");

   cnt = xs[wi].nbRecsFiltres;

   i=0;  
   XtSetArg(args[i], XmNitems, xs[wi].recsFiltresAffiches); i++;
   XtSetArg(args[i], XmNitemCount, cnt); i++;
/**   XtSetArg(args[i], XmNvisibleItemCount, 10); i++; **/
   XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++; 
   XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++; 
   XtSetArg(args[i], XmNtopWidget, xs[wi].menuform); i++; 
   XtSetArg(args[i], XmNscrollBarPlacement, XmTOP_LEFT); i++; 
   XtSetArg(args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++; 
   XtSetArg(args[i], XmNdoubleClickInterval, 700); i++; 

   switch (xs[wi].typeSelection)
      {
      case SELECTION_SIMPLE:
      XtSetArg(args[i], XmNselectionPolicy, XmBROWSE_SELECT); i++;
      break;

      case SELECTION_MULTIPLE:
      XtSetArg(args[i], XmNselectionPolicy, XmMULTIPLE_SELECT); i++;
      break;

      case SELECTION_ETENDUE:
      XtSetArg(args[i], XmNselectionPolicy, XmEXTENDED_SELECT); i++;
      break;
      }
   
   xs[wi].liste = (Widget)XmCreateScrolledList(xs[wi].pan,
                                        nomListe,
                                        args, i);
   XtManageChild(xs[wi].liste);

   }



MessageChargement(wordint nbRecs1, wordint nbRecs2)
{
   Arg args[10];
   wordint i;
   Position x, y;
   XmString xmMessage, xmnull;
   char message[32], infoMessage[32];
   
   if (nbRecs1 < 10)
      {
      strcpy(infoMessage, "%1d/");
      }
   else
      {
      if (nbRecs1 < 100)
   {
   strcpy(infoMessage,"%2d/");
   }
      else
   {
   if (nbRecs1 < 1000)
      {
      strcpy(infoMessage,"%3d/");
      }
   else
      {
      if (nbRecs1 < 10000)
         {
         strcpy(infoMessage,"%4d/");
         }
      else
         {
         if (nbRecs1 < 100000)
      {
      strcpy(infoMessage,"%5d/");
      }
         else
      {
      strcpy(infoMessage,"%6d/");
      }
         }
      }
   }
      }
      
   if (nbRecs2 < 10)
      {
      strcat(infoMessage, "%1d");
      }
   else
      {
      if (nbRecs2 < 100)
   {
   strcat(infoMessage,"%2d");
   }
      else
   {
   if (nbRecs2 < 1000)
      {
      strcat(infoMessage,"%3d");
      }
   else
      {
      if (nbRecs2 < 10000)
         {
         strcat(infoMessage,"%4d");
         }
      else
         {
         if (nbRecs2 < 100000)
      {
      strcat(infoMessage,"%5d");
      }
         else
      {
      strcat(infoMessage,"%6d");
      }
         }
      }
   }
      }
      
   sprintf(message, infoMessage, nbRecs1, nbRecs2);
   
   xmMessage = XmStringCreateLtoR(message, XmSTRING_DEFAULT_CHARSET);
   
   i = 0;
   XtSetArg(args[i], XmNlabelString, xmMessage); i++;
   XtSetValues(xs[wi].infoLabel, args, i);
   FlusherTousLesEvenements();
   
   XmStringFree(xmMessage);

   }

/*======================================================================================================*/

/**
******************************************************************************

NOM:                    PositionnerMenubar()

FONCTION:               Positionner le menubar vis-a-vis la liste des records.

APPELE PAR:             XSelectstdOuvrir().

METHODE:

GLOBALES AFFECTES:      Aucunes.

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static void PositionnerMenubar()
{
   wordint i;
   Arg args[2];
   Position d1, d2;

   XtSetArg(args[0], XmNx, &d1);
   XtGetValues(xs[wi].liste, args, 1);

   XtSetArg(args[0], XmNleftAttachment, XmATTACH_FORM);
   XtSetArg(args[1], XmNleftOffset, (int)d1 ); 
   XtSetValues(xs[wi].menuform, args, 2);
  }

static void AjusterSensibiliteBoutons()
{
   wordint i;
   Arg args[2];

   XtSetArg(args[0], XmNsensitive, True);
   for (i=0; i < xs[wi].nbDes; i++)
      {
      XtSetValues(xs[wi].menus[i], args, 1);
      }
   
   XtSetArg(args[0], XmNsensitive, False);
   for (i=0; i < xs[wi].nbDes; i++)
      {
      if (xs[wi].cles[i].nbCles < 2)
         {
         XtSetValues(xs[wi].menus[i], args, 1);
         }
     }
   
  }


/*======================================================================================================*/
/**
******************************************************************************

NOM:                    NettoyerString()

FONCTION:               Enlever les espaces devant et apres un string sauf le dernier espace.

APPELE PAR:             xselact_()
                        xselfer_()
                        xselact_()

METHODE:

GLOBALES AFFECTES:      Aucunes.

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint NettoyerString(char str[])
{
   wordint i, j, jinit;
   
/**
   i = 0;
   while (str[i] == ' ' && i < strlen(str))
         i++;
   
   if (i > 0)
      strcpy(str, str + i);
   
**/
   j = strlen(str) - 1;
   
   while (str[j] == ' ' && j >= 0)
      j--;
   
   if (j < strlen(str) - 1)
/*      str[j + 1] = NULL;*/
      str[j + 1] = '\0';
   }
/*======================================================================================================*/

/**
******************************************************************************

NOM:                    Ok()

FONCTION:               Canceller la selection de records et unmanage la fenetre.

APPELE PAR:             La fonction serait appelee de XtMainLoop() quand on
                        appuierait sur le bouton close ou sur applyClose.

METHODE:                Indiquer que la selection est terminee et que aucun
                        record n'a ete choisis.

GLOBALES AFFECTES:      xs[wi].StatutSelection
                        xs[wi].nbRecsSelect

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

static XtCallbackProc Ok(Widget W, caddr_t unused1, caddr_t unused2)
{
   xs[wi].StatutSelection       = SELECTION_TERMINEE;
   }
/*======================================================================================================*/

/**
********************************************************************************

NOM:                    UpdateFiltres()

FONCTION:               Ajouter/enlever un filtre a la liste des filtres.
                        Filtrer les records.
                        Afficher la liste des records filtres.
                        Afficher la liste des filtres.
                        Afficher le scrollbar.

APPELE PAR:             MenuListeSelect()

METHODE:                On utilise des fonctions pour chaque etape.

GLOBALES AFFECTES:      xs[wi]

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint UpdateFiltres()
{
   wordint i,j;   /* Compteur.                        */
   wordint res; /* Le nombre de records filtres.    */
   wordint nb;  /* Le nombre de records visibles.   */
   
   wordint numItems, *selectedItems, nbTotalItems;
   
   xs[wi].nbFiltres = 0;
   
   for (i=0; i < xs[wi].nbDes; i++)
      {
      if (xs[wi].cles[i].nbCles > 1)
         {
         if(XmListGetSelectedPos(xs[wi].panListeItems[i], &selectedItems, &numItems))
            {
            for (j=0; j < numItems; j++)
               {
               xs[wi].filtres[j+xs[wi].nbFiltres].indDes = i;
               xs[wi].filtres[j+xs[wi].nbFiltres].indCle = selectedItems[j] - 1;
               }
            
            xs[wi].nbFiltres += numItems;
            free(selectedItems);
            }
         }
      }
   
   res = FiltrerRecords(xs[wi].indRecsFiltres,
                        &(xs[wi].nbRecsFiltres),
                        xs[wi].recs,
                        xs[wi].cles,
                        xs[wi].filtres,
                        xs[wi].nbRecs,
                        xs[wi].nbDes,
                        xs[wi].nbFiltres);
   
   if (res == 0 && xs[wi].nbFiltres == 0)
      EffacerFiltres(xs[wi].effacerFiltres, NULL, NULL);
   
   UpdateRecsFiltresAffiches(&(xs[wi].recsFiltresAffiches), xs[wi].recsAffiches,
                         xs[wi].nbDes, xs[wi].indRecsFiltres, xs[wi].nbRecsFiltres);

   nb = xs[wi].nbRecsFiltres;
   
   AfficherListe(xs[wi].liste, xs[wi].recsFiltresAffiches, nb);
   }
/*======================================================================================================*/
/**
*****************************************************************************

NOM:                    UpdateRecsFiltresAffiches()

FONCTION:               Mettre touts les records filtres dans xs[wi].recFiltresAffiches.

APPELE PAR:             UpdateFiltres()

METHODE:

GLOBALES AFFECTES:      xs[wi].recsFiltresAffiches

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

void UpdateRecsFiltresAffiches(XmString **recsFiltresAffiches, XmString *recsAffiches, wordint nbDes, wordint sel[], wordint nbSel)
{
   wordint i, j; /* Compteurs. */

   for (j = 0; j < nbSel; j++)
       (*recsFiltresAffiches)[j] = recsAffiches[sel[j]];
   }
/*======================================================================================================*/
/**
************************************************************

NOM:                    XSelectstdActiver()

FONCTION:

APPELE PAR:             xselact_()

METHODE:

GLOBALES AFFECTES:

VALEUR RETOURNEE:      0 quand on sort sans avoir appuye sur apply ou apply+close.
                        1 quand on a appuye sur apply ou apply+close.

------------------------------------------------------------------------------*/

/** si des items ont ete selectionnes, les mettre dans la liste **/
/** et terminer avec la valeur SELECTION_TERMINEE **/

wordint XSelectstdActiver(wordint  sel[], wordint  *nbsel, wordint  *indSelecteur)
{
   wordint    i, quitter;
   Widget widgetParent, bouton;
   
   if (!xs[wi].topLevel)
      return;
   
   xs[wi].StatutSelection = SELECTION_EN_COURS;
   while (xs[wi].StatutSelection == SELECTION_EN_COURS && XtIsRealized(xs[wi].topLevel))
      {
      XtAppPeekEvent(SuperWidget.contexte, &(xs[wi].theEvent));
      
      switch(xs[wi].theEvent.type)
         {
         case ButtonPress: 
         widgetParent = TrouverWidgetParent(xs[wi].theEvent.xbutton.window);
         if (widgetParent == xs[wi].topLevel)
            {
            XtAppNextEvent(SuperWidget.contexte, &(xs[wi].theEvent));
            XtDispatchEvent(&(xs[wi].theEvent));
            }
         else
            {
            if (XtIsRealized(xs[wi].panListe))
               {
               XtUnmanageChild(xs[wi].panListe);
               ActiverSelWidgets();
               }
            *nbsel = 0;
            return 0;
            }
         break;
         
         default:
         XtAppNextEvent(SuperWidget.contexte, &(xs[wi].theEvent));
         XtDispatchEvent(&(xs[wi].theEvent));
         break;
         }
      }
   
   *nbsel = xs[wi].nbRecsSelect;
   
   for (i = 0; i < *nbsel; i++)
      sel[i] = xs[wi].indRecsSelect[i];
   
   return(xs[wi].StatutSelection);
   }
/**
************************************************************

NOM:                    XSelectstdFermer()

FONCTION:

APPELE PAR:             xselfer_()

METHODE:

GLOBALES AFFECTES:

VALEUR RETOURNEE:      Aucune.

------------------------------------------------------------------------------*/

wordint XSelectstdFermer(wordint  sel[], wordint  *nbsel, char **idents, wordint  table[][3], wordint  *m, wordint  *n, wordint  *indSelecteur)
{
   XtUnmapWidget(xs[wi].topLevel);
   XFlush(XtDisplay(xs[wi].topLevel));
   XtDestroyWidget(xs[wi].topLevel);
   LibererPointeurs();
   }
/*======================================================================================================*/
/*======================================================================================================**
 **                                                                                                      **
 **                                      FIN DE XSELECTSTD.C                                             **
 **                                                                                                      **
 **======================================================================================================*/

CalculerLargeurMenus(wordint largeurMenus[], wordint table[][3])
{
   wordint i,j;
   XmFontList fontListe;
   wordint largeurTexte;
   
   Arg args[10];
   char bidon[32];
   XmString labelBidon[32], label1, label2;

   strcpy(bidon, " ");
   for (i=1; i < 32; i++)
      {
      labelBidon[i] = XmStringCreate(bidon, XmSTRING_DEFAULT_CHARSET);
      strcat(bidon, " ");
      }
   
   i = 0;
   XtSetArg(args[i], XmNfontList, &fontListe); i++;
   XtGetValues(xs[wi].ok, args, i);

   for (i=0; i < xs[wi].nbDes; i++)
      {
      largeurMenus[i] = (int) XmStringWidth(fontListe, labelBidon[table[i][0]]);
/*      largeurMenus[i] = (int) (8 * table[i][0]);*/
      }  

   for (i=1; i < 32; i++)
      XmStringFree(labelBidon[i]);
   
   }

/**
 ************************************************************
 ************************************************************
 **/

f77name(xseloup)(char *titre, wordint  *nbrecs, char idents[], wordint *nbdes, wordint *indSel, wordint *typeSel, wordint lenNomFich, wordint lenIdents)
{
   
   wordint  i,j,k;
   char tmp;
   char **identsMenus;
   
   identsMenus = (char **) calloc(*nbdes, sizeof(char *));
   for (i = 0; i < *nbdes; i++)
      {
      idents[i*lenIdents + (lenIdents - 1)] = '\0';
      NettoyerString(&(idents[i*lenIdents]));
      identsMenus[i] = (char *) calloc(strlen(&(idents[i*lenIdents])) + 1, sizeof(char));
      strcpy(identsMenus[i], &(idents[i*lenIdents]));
      }
   
   XSelectstdCommencerInit(titre, *nbrecs, identsMenus, *nbdes, *indSel, *typeSel);
   
   for (i = 0; i < *nbdes; i++)
      free(identsMenus[i]);
   free(identsMenus);
   return 0;
   }

f77name(xselins)(char *tableau, wordint table[][3], wordint *nbrecs)
{
   XSelectstdInserer(tableau, table, *nbrecs);
   }

f77name(xselouf)(wordint table[][3], wordint *nbrecs)
{
   XSelectstdTerminerInit(table, *nbrecs);
   }


/**
******************************************************************************

NOM:                    xselact_()

FONCTION:

APPELE PAR:             L'Exterieur.

METHODE:

GLOBALES AFFECTES:

VALEUR RETOURNEE:      0

------------------------------------------------------------------------------*/

wordint f77name(xselact)(wordint sel[], wordint *nbsel, wordint *indsel)
{
   wordint  i, j, k;

   XSelectstdActiver(sel, nbsel, indsel);

   for (i = 0; i < *nbsel; i++)
      sel[i]++;
   
   return 0;
   }

/**
******************************************************************************

NOM:                    xselfer_()

FONCTION:

APPELE PAR:             L'exterieur.

METHODE:

GLOBALES AFFECTES:

VALEUR RETOURNEE:      0

------------------------------------------------------------------------------*/

wordint f77name(xselfer)(wordint sel[], wordint *nbsel, char idents[], wordint table[][3], wordint *m, wordint *n, wordint     *indsel, wordint len)
{
   wordint  i, j, k;
   char **identsMenus;

   identsMenus = (char **) calloc(*n, sizeof(char *));
   for (i = 0; i < *n; i++)
      {
      idents[i*len + (len - 1)] = '\0';
      NettoyerString(&(idents[i*len]));
      identsMenus[i] = (char *) calloc(strlen(&(idents[i*len])) + 1, sizeof(char));
      strcpy(identsMenus[i], &(idents[i*len]));
      }

   XSelectstdFermer(sel, nbsel, identsMenus, table, m, n, indsel);

   for (i = 0; i < *nbsel; i++)
      sel[i]++;

   for (i = 0; i < *n; i++)
      free(identsMenus[i]);
   free(identsMenus);
   return 0;
   }
/*======================================================================================================*/

/**
******************************************************************************

NOM:                    xselouv_()

FONCTION:

APPELE PAR:             L'exterieur.

METHODE:

GLOBALES AFFECTES:

VALEUR  RETOURNEE:      0

------------------------------------------------------------------------------*/

wordint f77name(xselupd)()
{
   LibererPointeurs();
   }



/*======================================================================================================*/
XSelectstdCommencerInit(char *titre, wordint nbrecs, char **idents, wordint nbdes, wordint indSel, wordint typeSel)
{
   wordint i;

   langue = c_getulng();

   wi                           = indSel;
   xs[wi].lastNbRecs            = 0;
   xs[wi].nbRecs                = nbrecs;
   xs[wi].nbDes                 = nbdes;
   xs[wi].nbRecsSelect          = 0;
   xs[wi].typeSelection         = typeSel;
   xs[indSel].titreFenetre = titre;
   
   InitTitresMenus(idents, xs[wi].nbDes);

   if (nbrecs > 0)
      {
      xs[wi].indRecsFiltres        = (wordint *)calloc(xs[wi].nbRecs, sizeof(int));
      xs[wi].indRecsSelect         = (wordint *)calloc(xs[wi].nbRecs, sizeof(int));
      xs[wi].recsAffiches          = (XmString *)calloc(nbrecs, sizeof(XmString));
      xs[wi].recs                  = (XmString **)calloc(nbdes, sizeof(XmString *)); 
      
      for (i = 0; i < nbdes; i++) 
         xs[wi].recs[i] = (XmString *)calloc(nbrecs, sizeof(XmString));
      }
   else
      {
      xs[wi].indRecsFiltres        = NULL;
      xs[wi].indRecsSelect         = NULL;
      xs[wi].recsAffiches          = NULL;
      xs[wi].recs                  = NULL;
      }
   }

void XSelectstdInserer(char *tableau, wordint table[][3], wordint nbrecs)
{
   wordint lng;
   char message[256];
   wordint i;
   Arg args[2];
   Colormap cmap;

   lng = c_getulng();

   xs[wi].nbRecsFiltres         = nbrecs;
   InitCles(tableau, table, xs[wi].cles, nbrecs, xs[wi].nbDes);
   InitRecsAffiches(&(xs[wi].recsAffiches), tableau, table, xs[wi].nbRecsFiltres);

   if (!xs[wi].topLevel)
      {
      InitWidgetsAll1(xs[wi].titresMenus, xs[wi].nbRecsFiltres, xs[wi].nbDes);
      CalculerLargeurMenus(largeurMenus, table);
      InitRecsFiltresAffiches(&(xs[wi].recsFiltresAffiches), xs[wi].recsAffiches, xs[wi].nbRecsFiltres);

      InitWidgetsAll2(xs[wi].titresMenus, xs[wi].nbRecsFiltres, xs[wi].nbDes);
      XtRealizeWidget(xs[wi].topLevel);

      PositionnerMenubar();
      DesactiverSelWidgets();
      FlusherTousLesEvenements();
      }

   if (xs[wi].lastNbRecs == 0)  /** && xs[wi].nbRecs != 0) **/
      InvertWidget(xs[wi].infoLabel);
   

   xs[wi].lastNbRecs += nbrecs;

   sprintf(message, messString[lng], xs[wi].lastNbRecs, xs[wi].nbRecs);
   MessageChargement(xs[wi].lastNbRecs, xs[wi].nbRecs);
   }

void XSelectstdTerminerInit(wordint table[][3], wordint nbrecs)
{
   wordint i;
   Arg args[2];
   Colormap cmap;

   for (i = 0; i < xs[wi].nbRecs; i++)
      {
      xs[wi].indRecsFiltres[i] = i;
      }

   for (i = 0; i < xs[wi].nbDes; i++)
      qsort((char *)xs[wi].cles[i].cleInfo, xs[wi].cles[i].nbCles, sizeof(cleInfoStruct), ComparerCles);

   xs[wi].nbRecsFiltres = xs[wi].nbRecs;
   InitRecs(&(xs[wi].recs), xs[wi].cles, xs[wi].nbRecs, xs[wi].nbDes);
   InitRecsFiltresAffiches(&(xs[wi].recsFiltresAffiches), xs[wi].recsAffiches, xs[wi].nbRecs);
   
   xs[wi].filtres               = (filtresStruct *)calloc(xs[wi].nbCles, sizeof(filtresStruct));
   xs[wi].nbFiltres             = 0;
   ActiverSelWidgets();
   InitWidgetsListe(); 
   InitWidgetsCallback(xs[wi].nbDes);

   DeselectionnerRecords(NULL, NULL, NULL);
   AfficherListe(xs[wi].liste, xs[wi].recsFiltresAffiches, xs[wi].nbRecsFiltres);

   InvertWidget(xs[wi].infoLabel);
   AjusterSensibiliteBoutons();
   
/**
   c_wglgetcmap(&cmap);
   i = 0;
   if (cmap != -1)
      {
      XtSetArg(args[i], XmNcolormap, cmap); i++;
      }
   XtSetValues(xs[wi].topLevel, args, i);
   XtSetValues(xs[wi].panListe, args, i);
**/   
   }

/***
*************************
*************************
***/

f77name(xseldim)()
{
   DesactiverSelWidgets();
   }

/***
*************************
*************************
***/

wordint f77name(xselundim)()
{
   ActiverSelWidgets();
   }

/***
*************************
*************************
***/

f77name(xselup)(wordint *indSelecteur)
{
   XtRealizeWidget(xs[*indSelecteur].topLevel);
   }

/**
********
********
**/

c_xselup(wordint indSelecteur)
{
   XtRealizeWidget(xs[indSelecteur].topLevel);
   }

/***
*************************
*************************
***/

f77name(xseldown)(wordint *indSelecteur)
{
   XtUnrealizeWidget(xs[*indSelecteur].topLevel);
   }

/**
********
********
**/

c_xseldown(wordint indSelecteur)
{
   XtRealizeWidget(xs[indSelecteur].topLevel);
   }

/***
*************************
*************************
***/

static XtCallbackProc FermerSelecteur(Widget w, caddr_t unused1, caddr_t unused2)
{
   XtUnrealizeWidget(xs[wi].topLevel);
   }

/***
*************************
*************************
***/

f77name(xselopt)(wordint *indSelecteur, char option[], char valeur[], wordint lenOption, wordint lenValeur)
{
   option[lenOption-1] = '\0';
   valeur[lenValeur-1] = '\0';
   NettoyerString(option);
   NettoyerString(valeur);
   c_xselopt(*indSelecteur, option, valeur);
   }

c_xselopt(wordint indSelecteur, char *option, char *valeur)
{
   if (0 == strcmp(option, "bouton_fermer") || 0 == strcmp(option, "BOUTON_FERMER"))
      {
      if (0 == strcmp(valeur, "oui") || 0 == strcmp(valeur, "OUI"))
   xs[indSelecteur].statutBoutonFermer = BOUTON_FERMER_ACTIF;
      else
   xs[indSelecteur].statutBoutonFermer = BOUTON_FERMER_INACTIF;
      return;
      }
   
   printf("Mauvaise option: %s\n", option);
   }

