#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <Xm/Xm.h>

#include <stdio.h>
#include <math.h>
#include <malloc.h>
   
#define  RADIAN_A_DEGRE		57.29577951
#define  DEGRE_A_RADIAN		0.01745329252

#define  GLOBAL                 0
#define  NORD                   1
#define  SUD                    2
   
#define  DEVANT                 0
#define  DERRIERE               1

#define XY                      0
#define YZ                      1
#define XZ                      2
#define XX                      3
#define VALEURS_PONCTUELLES     4

#define SCALAIRE                1
#define VECTEUR                 2
#define VECTEUR3D               3

#define PRESSION 0
#define SIGMA    1
#define METRES   2

#define ZP 0
#define T  1

#define OK                       0
#define CHAMP_NON_SUPERPOSABLE   1
#define CHAMP_MODIFIE            2
#define TROP_DE_CHAMPS           3
#define PAS_ASSEZ_DE_PERIODES    4
#define PAS_ASSEZ_DE_NIVEAUX     5
#define CHARGEMENT_ANNULE        6
#define NIVEAUX_INCONSISTANTS    7
#define NB_NIVEAUX_INCONSISTANTS 8
#define CHAMP_INEXISTANT         9
#define PAS_AVEC_DES_CHAMPS_VECTORIELS 10
#define PAS_ASSEZ_DE_MEMOIRE     11

#define  COULEURS_STANDARDS            0
#define  COULEURS_STANDARDS_INVERSEES  1
#define  NOIR_BLANC                    2
#define  BLANC_NOIR                    3
#define  UTOPIC_VZ                     4
#define  UTOPIC_IR                     5
#define  EXP_1                         6
#define  EXP_2                         7

#define  ROUND(x)               (int)(x + 0.5)

#define  INFO          0
#define  AVERTISSEMENT 1
#define  OK_CANCEL     2

#define  BTN_CANCEL        0
#define  BTN_OK            1


#define NB_MENU_AFFICHAGE_ITEMS 13

#define COULEURS              0
#define CONTOURS              1
#define LABELS                2
#define VALEURS_CENTRALES     3   
#define GEOGRAPHIE            4
#define GRILLE                5
#define LEGENDE               6
#define LEGENDE_COULEUR       7
#define LISSAGE               8
#define BARBULES              9
#define ZOOM_LOCAL           10
#define TOPOGRAPHIE          11
#define AFF_AUTOMATIQUE      12
#define AXE_X                13
#define AXE_Y                14
#define EN_TRAIN_DE_DESSINER 15
#define EFF_FENETRE          16

#define ANIMATION  0
#define SIMPLE     1

#define FOND       32
#define GRID       33

#define OFFSET_FORE_CHAMP1  0
#define OFFSET_BACK_CHAMP1  1
#define OFFSET_FORE_CHAMP2  2
#define OFFSET_BACK_CHAMP2  3
#define OFFSET_FORE_CHAMP3  4
#define OFFSET_BACK_CHAMP3  5
#define OFFSET_FORE_CHAMP4  6
#define OFFSET_BACK_CHAMP4  7
#define OFFSET_FORE_FOND    8
#define OFFSET_BACK_FOND    9
#define OFFSET_GRILLE       10 
#define OFFSET_GEO          11

#define LINEAIRE 1
#define LOG      2

#define CROISSANTE   0
#define DECROISSANTE 1

#define TEMPS        0
#define NIVEAUX      1

#define DEFILEMENT_REGULIER       0
#define DEFILEMENT_AVANT_ARRIERE  1

#define NO_OP            0
#define SUBTRACT         1
#define SOMME            2
#define ABS_SUBTRACT     3
#define ABS_SOMME        4

#define RIEN           0
#define ORDINAIRE      1
#define ANIM           2
#define COUPE          3

#define EXTREMUM        5
#define CUBIQUE         3
#define QUADRATIQUE     2
#define RACINE_CARREE  -2
#define RACINE_CUBIQUE -3
#define MOYENNE        -5

#define AUTO            0
#define CUSTOM          1

#define SELON_MENU      0
#define TOUJOURS        1
#define JAMAIS          2

#ifdef hp720
#define ffloor floor
#define fceil ceil
#define flog10 log10
#define rint(x)  (int)(x + 0.5)
#endif

typedef struct
{
   int coupeValide;
   int nbNiveauxCoupe;
   int   cleRef;
   int   niCoupe, njCoupe;
   float xmin, ymin, xmax, ymax;
   float niveauMin,niveauMax;
   float FLDmin3d[5],FLDmax3d[5];
   float UUmin3d[5],UUmax3d[5];
   float VVmin3d[5],VVmax3d[5];
   float WWmin3d[5],WWmax3d[5];
   float UVWmin3d[5],UVWmax3d[5];
   float FLDmin2d[5],FLDmax2d[5];
   float UVWTANGmin2d[5],UVWTANGmax2d[5];
   float UVWNORMmin2d[5],UVWNORMmax2d[5];
   float VVmin2d[5],VVmax2d[5];
   float WWmin2d[5],WWmax2d[5];
   float UVWmin2d[5],UVWmax2d[5];
   float scaleWW;
   int   indChampCourant;
   int   cleMT,cleP0;
   int   *clesNiveaux;
   float *niveauxCoupe;
   float **fld3d,**uu3d,**vv3d,**ww3d,**other3d;
   float *fld2d, *uvwtang2d,*uvwnorm2d,*ww2d,*uvw2d,*other2d;
   float *montagnes;
   float *ligneMontagnes;
   } _CoupeVerticale;


typedef struct
{
   int domaine;
   int nbFldsAnim;
   int indChampCourant;
   int niSerie,njSerie;
   float niveauMin, niveauMax;
   float xmin,ymin,xmax,ymax;
   int dateMin, dateMax, increment;
   int *ip1s, *ip2s, *ip3s, *dates;
   int *clesAnim;
   float *animFLDmin,*animFLDmax;
   float *animUUmin,*animUUmax;
   float *animVVmin,*animVVmax;
   float *animUVmin,*animUVmax;
   float animFLDmins[5],animFLDmaxs[5];
   float animUVmins[5], animUVmaxs[5];
   float animOTHERmins[5], animOTHERmaxs[5];
   float **tranchesSeries;
   float *valeursSeries;
   unsigned short **animFLDs;
   unsigned short **animUUs;
   unsigned short **animVVs;
   unsigned short **animUVs;
   unsigned short **animOTHERs;
   } _SequenceAnimee;

typedef struct
{
   int domaine;
   int nbPoints;
   float valMin,valMax;
   float *valeurs;
   } _LigneHorizontale;

typedef struct
{
   int indCouleurFore;
   int indCouleurBack;
   int couleurFore;
   int couleurBack;
   int epaisseur;
   int codeDash;
   int style;
   int labelSize;
   int centralValSize;
   int displayLabels;
   int displayContours;
   int displayVecteurs;
   int displayValCentrales;
   } _Attributs;

typedef struct 
{
   int iun;
   int cle;
   int indDict;
   int date, deet, npas;
   int ni, nj, nk;
   int nbits, datyp;
   int ip1, ip2, ip3;
   char typvar[2];
   char nomvar[3];
   char etiket[9]; 
   char grtyp[2];
   char pdfdatev[16];
   int ig1, ig2, ig3, ig4;
   int swa, lng, dltf, ubc;
   int extra1, extra2, extra3;
   char grtypref[2];
   int ig1ref, ig2ref, ig3ref, ig4ref;
   int ni_orig, nj_orig, nk_orig, npts;
   int domaine;
   int natureTensorielle;
   int travailEnCours;
   int champModifie;
   float niveau;
   float heure;
   float facteur;
   int diffValide;
   float min,max,vmin,vmax,diffMin,diffMax,localMin,localMax;
   float fldmin[5],fldmax[5];
   float uumin[5],uumax[5];
   float vvmin[5],vvmax[5];
   float wwmin[5],wwmax[5];
   float uvmin[5],uvmax[5];
   char dateTemps[12];
   char titreVariable[72];
   char titreTemps[72];
   char titreNiveau[24];
   char titreIntervalle[72];
   char titreUnites[72];
   char titreEtiquette[24];
   char ordinal[10];
   char heureInit[3];
   char minute[3];
   char seconde[3];
   char jour[3];
   char mois[12];
   char annee[3];
   float intervalles[32];
   int  nbIntervalles;
   float *fld,*uu,*vv,*ww,*module,*other,*x,*y;
   _Attributs attr;
   _CoupeVerticale coupe;
   _SequenceAnimee seqanim;
   _LigneHorizontale ligne;
   } _Champ;


typedef struct
{
   char nomVar[3];
   char identifVar[64];
   char unitesVar[32];
   char paletteVar[32];
   int  indIntervalleDeDefaut;
   float intervalleDeDefaut;
   float facteurDeConversion;
   float **intervallesDeContour;
   int   *nbIntervalles;
   int  nbMenuItems;
/**
**/
   float offset;
   int   minmaxSource;
   float missingValue;
   float userMin,userMax;
   } _InfoChamps;

typedef struct
{
   int noPalette;
   float amplificationMin;
   float reductionMax;
   } _ColormapInfo;

typedef struct
{
   int vi1, vi2, vj1, vj2;
   int vlargeur, vhauteur;
   float vratio;
   } _Viewport;

typedef struct
{
   int imagesDejaAllouees;
   int variableBoucle;
   int animationRapide;
   int typeDefilement;
   int heureDebut;
   int heureFin;
   float niveauDebut;
   float niveauFin;
   float delai;
   int nbImages;
   int flagsImagesChargees[1024];
#ifdef X_WGL
   Pixmap pixmaps[1024];
#else
   unsigned short *pixmaps[1024];
#endif
   } _AnimInfo;

typedef struct
{
   int statut;
   int ChampAContourer;
   int NbMenusIntervalles;
   int colorMap;
   int lastColorMap;
   int flagInterrupt;
   int flagUpdateMenuIntervalle;
   int statuts[16];
   int statutSuperposition;
   int carteLue;
   int annulationDemandee;
   int iun;
   _Attributs attributs[36];
   Widget topLevel;
   Widget panneauContour;
   Widget formeAff, formeZoom, formeSup, formeAnim, formeCoupe;
   Widget menuform, menuFich, menuAff, menuInt, menuSup, menuVarSup, menuOpt;
   Widget menuFichier, menuAffichage, menuIntervalleDeContour, menuSuper, menuVarSuper, menuOptions;
   Widget menuFichierItems[16], menuAffichageItems[16], menuIntervalleItems[32], menuSuperItems[8];
   Widget menuVarSuperItems[12], menuOptionItems[16];
   Widget rafraichir, valeursPonctuelles, zoom;
   Widget superposition, effacer, menuEffacageSelectif, menuEff, menuEffItems[4];
   Widget stop;
   Widget labelInfo, formeMessage, formeMessageFrame;
   XEvent theEvent;
   long   serverID;
   } _XContour;

typedef struct
{
   float x,y,value;
   char symbol;
   } Hilo;
