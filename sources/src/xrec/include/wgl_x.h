#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <wgl.h>
#include <x_wglfonct.h>


/**
** Declaration des constantes globales
**/


#define NB_MAX_POINTS   1024 
#define EVENT_MASK      (ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask | ColormapChangeMask )

#define  READ_ONLY_COLORMAP     1000
#define  READ_WRITE_COLORMAP    1001

typedef struct
{
   XtAppContext contexte;
   Widget topLevel;
   } SuperWidgetStruct;

extern SuperWidgetStruct SuperWidget;

/**
** Variables de base utilisees par X
**/

extern Display *wglDisp;
extern int wglScrNum;
extern Window wglWin, wglDrawable;
extern GC wglLineGC, wglFillGC;
extern Pixmap bgPix;
extern int visualClass;
extern int cmap_strategy;
extern XVisualInfo visInfo;
extern Cursor croix;
extern Colormap cmap;
extern XColor xcouleurs[];

extern XWindowAttributes wglWinAttr;
extern XSetWindowAttributes wglWinSetAttr;
extern unsigned long attribmask;
extern XSizeHints wglHints;

extern float  aspectRatio;
extern int  wglForceAspect;

extern XEvent wglEvent;

extern XFontStruct *fonte, *fontes[];

extern XRectangle pixmapDimensions;

extern int        lng;
extern char       *messageAgrandir[];

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

extern _Fenetre *fenetre;
extern int nbFenetresActives;
extern int fenetreCourante;
/* ---------------------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------------------- */
