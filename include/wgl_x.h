#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>

#include <xinit.h>
#include <rpnmacros.h>
#include <malloc.h>
#include <math.h>
#include <wgl.h>

/**
** Declaration des constantes globales
**/


#define X                        0
#define Y                        1

#if defined (IRIX5) || defined (C910)
#define LE_LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)flog10(x))
#endif

#if defined (i386) || defined (Alpha)
#define LE_LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)log10(x))
#endif

#if defined (HP)
#define LE_LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)log10(x))
#endif
#define NB_MAX_POINTS   1024 
#define EVENT_MASK      (ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask | ColormapChangeMask )

#define  ROUND(x)               (int)(x + 0.5)

#define  READ_ONLY_COLORMAP     1000
#define  READ_WRITE_COLORMAP    1001


/**
 ** Variable globale necessaire pour compiler sur les SUN, autrement non utilisee.
 **/ 

/**
 ** Widget de base
 **/

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

/**
 ** Attributs de la fenetre
 **/

extern XWindowAttributes wglWinAttr;
extern XSetWindowAttributes wglWinSetAttr;
extern unsigned long attribmask;
extern XSizeHints wglHints;

extern float  aspectRatio;
extern int  wglForceAspect;

/**
 ** Variables liees a l'utilisation des couleurs
 **/ 

/*extern wgl_color couleurs[];
extern wgl_point wglPts[];*/
extern XColor couleurs[];
extern XPoint wglPts[];
extern int NbPoints;
extern int cmap;
extern unsigned long wglfore, wglback;

extern int     rubberBandColor;

extern int     pix[];
extern int     wglColorTable[];
extern int     wglWritablePixs[];

/**
 ** Variables liees aux evenements
 **/ 

extern XEvent wglEvent;

/**
 ** Variables liees aux patrons
 **/ 

extern unsigned long  wglPatterns[];
extern char   wglDashPatterns[][16];
extern int    wglDashPatternLengths[];
extern int    wglDashPatternOffsets[];

/**
 ** Variables liees aux fontes
 **/ 

extern XFontStruct *fonte, *fontes;
extern int NbPoints;
extern int cmap;
extern unsigned long wglfore, wglback;

extern int     rubberBandColor;

extern int     pix[];
extern int     wglColorTable[];
extern int     wglWritablePixs[];

/**
 ** Variables liees aux evenements
 **/ 

extern XEvent wglEvent;

/**
 ** Variables liees aux patrons
 **/ 

extern unsigned long  wglPatterns[];
extern char   wglDashPatterns[][16];
extern int    wglDashPatternLengths[];
extern int    wglDashPatternOffsets[];

/**
 ** Variables liees aux fontes
 **/ 

extern XFontStruct *fonte, *fontes;

/**
 ** Flags divers
 **/

extern int debugMode;
extern int customWinSize;
extern int customPlaneMask;
extern int doubleBufferMode;

extern int  aspectRatioSet    ;
extern int  wglForceAspectSet ;
extern int  customPlaneMaskSet;
extern int  doubleBufferModeSet;
extern int  currentPlaneMaskSet;

#define SGI 13

extern int serverIdentifier;

/**
 ** Indicateurs divers
 **/

extern int        currentColor;
extern int        currentFillStyle;
extern int        currentFillPattern;
extern int        currentDashPattern;
extern int        currentLineWidth;
extern int        currentPlaneMask;
/*extern wgl_rectangle pixmapDimensions; */
extern XRectangle pixmapDimensions;
extern int        w,h;

extern int        lng;
extern char       *messageAgrandir[];

/**
******************************************************************************
******************************************************************************
**/
   

/**
 ** Structure de base pour faire le lien entre l'espace pixel et l'espace reel.
 **/ 

extern UserSpaceInfo usSpace;

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

extern int wgl_xfsz(int fontSize);
extern int wgl_xpsi(int i, int j, char string[], int stringLength, int size, int orient, int codeCentrage);
extern int wgl_xhsi(char string[], int stringLength);
extern int wgl_xasi(char string[], int stringLength);
extern int wgl_xdsi(char string[], int stringLength);
extern int wgl_xafi(int i, int j, int rayon, float angleDebut, float angleFin);
extern int wgl_xafx(float x, float y, float rayon, float angleDebut, float angleFin);
extern int wgl_xali(int i, int j, int rayon, float angleDebut, float angleFin);
extern int wgl_xalx(float x, float y, float rayon, int angleDebut, int angleFin);
extern int wgl_xbbf(void);
extern int wgl_xbrb(void);
extern int wgl_xbti(int *i, int *j);
extern int wgl_xbtn(int bouton);
extern int wgl_xbtpi(int bouton, int *i, int *j);
extern int wgl_xbtx(float *x, float *y);
extern int wgl_xcfi(int i, int j, int rayon);
extern int wgl_xcfx(float x, float y, float rayon);
extern int wgl_xchngfs(int NewFillStyle);
extern int wgl_xcli(int i, int j, int rayon);
extern int wgl_xclr(void);
extern int wgl_xclw(int indFenetre);
extern int wgl_xclx(float x, float y, float rayon);
extern int wgl_xcmi(int left, int bottom, int right, int top);
extern int wgl_xcmx(int left, int bottom, int right, int top);
extern int wgl_xcolf(float couleur);
extern int wgl_xcolfs(float *cols, unsigned int *pixels, int n);
extern int wgl_xcolfs_fst(float *cols, unsigned int *pixels, int n);
extern int wgl_xcol(int couleur);
extern int wgl_xcrb(int couleur);
extern int wgl_xdbf();
extern int wgl_xdbg();
extern int wgl_xdcm();
extern int wgl_xdeflst(int dashIndex, int linestyle);
extern int wgl_xdefncw(int winid);
extern int wgl_xdld(int indDash, int dashPattern);
extern int wgl_xdpt(int indpat, int ipatsz, char iptrn[]);
extern int wgl_xdri(int i, int j);
extern int wgl_xerb();
extern int wgl_xfbf();
extern int wgl_xfsc();
extern int wgl_xfshlb();
extern int wgl_xfsh();
extern int wgl_xgacw();
extern int wgl_xgaf();
extern int wgl_xgbf();
extern int wgl_xgci();
extern int wgl_xgco(int indCol, int *red, int *green, int *blue);
extern int wgl_xgdbg();
extern int wgl_xgetcmap(unsigned long *colormap);
extern int wgl_xgld();
extern int wgl_xglpti(int *ix, int *iy);
extern int wgl_xglw();
extern int wgl_xgmk();
extern int wgl_xgmod();
extern int wgl_xgpl();
extern int wgl_xgpt();
extern int wgl_xgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_xgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_xgsx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int wgl_xgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_xgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_xgvx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int wgl_xgwz(int *isize, int *jsize);
extern int wgl_xgzi(int *isize, int *jsize);
extern int wgl_xgzp(float *xsize, float *ysize, int *isize, int *jsize);
extern int wgl_xgzx(float *xsize, float *ysize);
extern int wgl_xias(int flagAspect);
extern int wgl_xiax(float *x, float *y, int i, int j);
extern int wgl_xinicmap();
extern int wgl_xinids(char *nomFenetre);
extern int wgl_xinipat();
extern int wgl_xiniwin(char *nomFenetre);
extern int wgl_xiniwpx();
extern int wgl_xinit();
extern int wgl_xinstcmap();
extern int wgl_xinvpat(char Pattern[]);
extern int wgl_xkas(int xAspect, int yAspect);
extern int wgl_xlpt();
extern int wgl_xlwi(int lineWidth);
extern int wgl_xmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
extern int wgl_xmapc(int colorIndex, int r, int g, int b);
extern int wgl_xmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
extern int wgl_xmco(int couleur, int r, int g, int b);
extern int wgl_xmcw(int couleur, int r, int g, int b);
extern int wgl_xmesgagr(char *message);
extern int wgl_xmvi(int i, int j);
extern int wgl_xmvx(float x, float y);
extern int wgl_xncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
extern int wgl_xnewwin();
extern int wgl_xopw(char *nomFenetre);
extern int wgl_xopmw(char *nomFenetre);
extern int wgl_xpfi(int nbPoints, int polygone[][2]);
extern int wgl_xpfx(int nbPoints, float polygone[][2]);
extern int wgl_xpli(int nbPoints, int polygone[][2]);
extern int wgl_xplx(int nbPoints, float polygone[][2]);
extern int wgl_xppo(int ix, int iy, int isize, int jsize);
extern int wgl_xpsz(int isize, int jsize);
extern int wgl_xptis(wgl_point pts, int npts);
extern int wgl_xpti(int i, int j);
extern int wgl_xptxs(float *xxx, float *yyy, int *npts);
extern int wgl_xptx(float x, float y);
extern int wgl_xrfi(int ia, int ja, int ib, int jb);
extern int wgl_xrfx(float xa, float ya, float xb, float yb);
extern int wgl_xrli(int ia, int ja, int ib, int jb);
extern int wgl_xrlx(float xa, float ya, float xb, float yb);
extern int wgl_xroc();
extern int wgl_xrstdpar();
extern int wgl_xrwc();
extern int wgl_xsavpfc();
extern int wgl_xsbf();
extern int wgl_xsetwcmap();
extern int wgl_xsetw(int winid);
extern int wgl_xsld(int indDash);
extern int wgl_xsmk(int masque);
extern int wgl_xspt(int indpat);
extern int wgl_xssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);
extern int wgl_xstu(int istep, int jstep);
extern int wgl_xswb();
extern int wgl_xxai(int *i, int *j, float x, float y);

/* ---------------------------------------------------------------------------------------------------------- */
