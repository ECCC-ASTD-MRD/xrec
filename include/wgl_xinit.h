#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>

#include <xinit.h>
#include <c_wgl.h>
#include <f77name.h>
#include <malloc.h>
#include <math.h>

/**
** Declaration des constantes globales
**/


#define IRIX5 13
#define NB_MAX_POINTS   1024 

int serverIdentifier = 0;

/**
 ** Variable globale necessaire pour compiler sur les SUN, autrement non utilisee.
 **/ 

#ifdef SUN
int units;
#endif

/**
 ** Widget de base
 **/

extern SuperWidgetStruct SuperWidget; 

/**
** Variables de base utilisees par X
**/

Display *wglDisp = NULL;
int wglScr;
Window wglWin, wglDrawable;
GC wglLineGC, wglFillGC;
Pixmap bgPix = NULL;
int visualClass;
XVisualInfo visInfo;
Cursor croix;

/**
 ** Attributs de la fenetre
 **/

XWindowAttributes wglWinAttr;
XSetWindowAttributes wglWinSetAttr;
unsigned long attribmask;
XSizeHints wglHints;

float  aspectRatio = 0.0;
int  wglForceAspect = True;

/**
 ** Variables liees a l'utilisation des couleurs
 **/ 

XColor couleurs[256];
XPoint wglPts[NB_MAX_POINTS];
int NbPoints = 0;
Colormap cmap = -1;
unsigned long wglfore, wglback;

int     rubberBandColor      = CYAN;

int     pix[256];
int     wglColorTable[256];
int     wglWritablePixs[256];

/**
 ** Variables liees aux evenements
 **/ 

XEvent wglEvent;

/**
 ** Variables liees aux patrons
 **/ 

Pixmap wglPatterns[257];
char   wglDashPatterns[257][16];
int    wglDashPatternLengths[257];
int    wglDashPatternOffsets[257];

/**
 ** Variables liees aux fontes
 **/ 

XFontStruct *fonte;
XFontStruct *fontes[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/**
 ** Flags divers
 **/

int debugMode = False;
int customWinSize =   False;
int customPlaneMask = False;
int doubleBufferMode =   False;

  aspectRatioSet     = False;
  wglForceAspectSet  = False;
  customPlaneMaskSet = False;
  doubleBufferModeSet = False;
  currentPlaneMaskSet = False;

/**
 ** Indicateurs divers
 **/

int        currentColor         = 0;
int        currentFillStyle     = FillSolid;
int        currentFillPattern   = 0;
int        currentDashPattern   = 0;
int        currentLineWidth     = 0;
int        currentPlaneMask     = AllPlanes;
XRectangle pixmapDimensions = { 0, 0, 0, 0 };

int        lng;
char       *messageAgrandir[] = {"Agrandissez-moi", "Enlarge me"};

/**
******************************************************************************
******************************************************************************
**/
   

/**
 ** Structure de base pour faire le lien entre l'espace pixel et l'espace reel.
 **/ 

UserSpaceInfo usSpace = { 0, 0, 0, 0, 
			     0, 
			     0.0, 0.0, 0.0, 0.0, 
			     0.0, 0.0,
			     0, 0, 0, 0 };
/**
 ** Definition de la structure utilisee pour la gestion de fenetres multiples
 **/

typedef struct
{
   Window wglWin, wglDrawable;
   GC wglLineGC, wglFillGC;
   Pixmap bgPix;
   XWindowAttributes wglWinAttr;
   XSetWindowAttributes wglWinSetAttr;
   int libre;
   float  aspectRatio;
   int  wglForceAspect;
   int customPlaneMask;
   int doubleBufferMode;
   int currentColor;
   int currentFillStyle;
   int currentFillPattern;
   int currentDashPattern;
   int currentLineWidth;
   int currentPlaneMask;
   XRectangle pixmapDimensions;
   UserSpaceInfo usSpace;
   } _Fenetre;

_Fenetre *fenetre = NULL;
int nbFenetresActives = 0;
int fenetreCourante = -1;


