#ifndef _WGL_H_
#define _WGL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rmn/rpnmacros.h>
#ifndef __CPLUSPPLUS
#include <math.h>
#endif
#include <X11/Xlib.h>

#define BLANC    0
#define NOIR     1
#define ROUGE    2
#define CYAN     3
#define JAUNE    4
#define MAGNTA   5
#define VERT     6
#define BLEU     7
#define GRIS     8

#define NIL      0
#define BGAUCH   1
#define BMLIEU   2
#define BDROIT   3
#define BTOUS    4

#define X                        0
#define Y                        1

#define NB_MAX_POINTS   1024
#define  ROUND(x)               (int)(x + 0.5)

#define LE_LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)log10(x))
#define LE_LOGF(x)                  ((x) <= 0.0 ? -38.0 : log10f(x))

#if defined (IRIX5) || defined (C910)
#define LE_LOG(x)                   ((x) <= 0.0 ? -38.0 : (float)flog10(x))
#endif

typedef struct
{
   int mode, mapping, ni, nj;
   int idebut, jdebut, ifin, jfin;
   float xdebut, ydebut, xfin, yfin;
   float densiteX, densiteY;
   float *axex, *axey, *z, *zx, *zy, *azy;
   } UserSpaceInfo;

typedef struct
{
  short x, y;
} wgl_point;



typedef struct
{
  short x, y;
  unsigned short width, height;
} wgl_rectangle;

typedef struct
{
  int r, g, b;
} wgl_color;

#include <wglfonct.h>

extern int        w,h;
extern int  wglForceAspect;
extern int  wglForceAspectSet ;


extern UserSpaceInfo usSpace;
extern float aspectRatio;
extern int debugMode;

extern int        currentColor;
extern int        currentFillStyle;
extern int        currentFillPattern;
extern int        currentDashPattern;
extern int        currentLineWidth;
extern int        currentPlaneMask;

extern unsigned long  wglPatterns[];
extern char   wglDashPatterns[][16];
extern int    wglDashPatternLengths[];
extern int    wglDashPatternOffsets[];
extern int customWinSize;
extern int customPlaneMask;
extern int doubleBufferMode;

extern int  aspectRatioSet    ;
extern int  customPlaneMaskSet;
extern int  doubleBufferModeSet;
extern int  currentPlaneMaskSet;

extern wgl_color couleurs[];
extern wgl_point wglPts[];
extern int NbPoints;
extern unsigned long wglfore, wglback;

extern int     rubberBandColor;

extern int     pix[];
extern int     wglColorTable[];
extern int     wglWritablePixs[];

typedef struct
{
  int graphics_library;
  void (*wglxai)(int *i, int *j, float x, float y);
  void (*wglafi)(int i, int j, int rayon, float angleDebut, float angleFin);
  void (*wglali)(int i, int j, int rayon, float angleDebut, float angleFin);
  int (*wglasi)(char string[], int stringLength);
  void (*wglbbf)(void);
  void (*wglbitfinder)(unsigned int mask, unsigned int *idebut, unsigned int *ifin);
  void (*wglbrb)(void);
  int (*wglbti)(int *i, int *j);
  int (*wglbtn)(int bouton);
  int (*wglbtpi)(int bouton, int *i, int *j);
  void (*wglcfi)(int i, int j, int rayon);
  void (*wglchngfs)(int NewFillStyle);
  void (*wglcli)(int i, int j, int rayon);
  void (*wglclr)(void);
  void (*wglclw)(int indFenetre);
  void (*wglcmi)(int left, int bottom, int right, int top);
  void (*wglcol)(int xcouleur);
  void (*wglcrb)(int xcouleur);
  void (*wgldbf)(void);
  void (*wgldbg)(void);
  void (*wgldcm)(void);
  void (*wgldeflst)(int dashIndex, int linestyle);
  void (*wgldefncw)(int winid);
  void (*wgldld)(int indDash, int dashPattern);
  void (*wgldpt)(int indpat, int ipatsz, char iptrn[]);
  void (*wgldri)(int i, int j);
  int (*wgldsi)(char string[], int stringLength);
  void (*wglecr)(char *nomFichier);
  void (*wglerb)(void);
  void (*wglfbf)(void);
  void (*wglfsc)(void);
  void (*wglfsh)(void);
  void (*wglfshlb)(void);
  int (*wglfsz)(int fontSize);
  int (*wglgacw)(void);
  int (*wglgaf)(void);
  int (*wglgbf)(void);
  int (*wglgci)(void);
  void (*wglgco)(int indCol, int *red, int *green, int *blue);
  int (*wglgdbg)(void);
  int (*wglgetcmap)(Colormap *colormap);
  int (*wglgld)(void);
  void (*wglglpti)(int *ix, int *iy);
  int (*wglglw)(void);
  int (*wglgmk)(void);
  int (*wglgmod)(void);
  int (*wglgpl)(void);
  int (*wglgpt)(void);
  void (*wglgsi)(int *idebut, int *jdebut, int *ifin, int *jfin);
  void (*wglgvi)(int *idebut, int *jdebut, int *ifin, int *jfin);
  void (*wglgvp)(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
  void (*wglgvx)(float *xdebut, float *ydebut, float *xfin, float *yfin);
  void (*wglgwz)(int *isize, int *jsize);
  void (*wglgzi)(int *isize, int *jsize);
  void (*wglgzp)(float *xsize, float *ysize, int *isize, int *jsize);
  void (*wglgzx)(float *xsize, float *ysize);
  int (*wglhsi)(char string[], int stringLength);
  void (*wglias)(int flagAspect);
  void (*wglinicmap)(void);
  void (*wglinids)(char *nomFenetre);
  void (*wglinipat)(void);
  void (*wglinit)(void);
  void (*wgliniwin)(char *nomFenetre);
  void (*wgliniwpx)(void);
  void (*wglinstcmap)(void);
  void (*wglinvpat)(char Pattern[]);
  void (*wglkas)(int xAspect, int yAspect);
  void (*wgllpt)(void);
  void (*wgllwi)(int lineWidth);
  void (*wglmapc)(int colorIndex, int r, int g, int b);
  void (*wglmapcs)(int colorIndexs[], int nbCols, int rgbDefs[][3]);
  void (*wglmco)(int xcouleur, int r, int g, int b);
  void (*wglmcos)(int xcouleurs[], int nbCols, int rgbDefs[][3]);
  void (*wglmcw)(int xcouleur, int r, int g, int b);
  void (*wglmesagr)(char *message);
  void (*wglmvi)(int i, int j);
  void (*wglncl)(int *ncolRead, int *ncolWrite, int *ncolAnim);
  void (*wglnewwin)(void);
  int (*wglopmw)(char *nomFenetre, int wglWinId);
  int (*wglopw)(char *nomFenetre);
  void (*wglpfi)(int nbPoints, int polygone[][2]);
  void (*wglpli)(int nbPoints, int polygone[][2]);
  void (*wglppo)(int ix, int iy, int isize, int jsize);
  int (*wglpsi)(int i, int j, char string[], int stringLength, int size, int orient, int codeCentrage);
  void (*wglpsz)(int isize, int jsize);
  void (*wglpti)(int i, int j);
  void (*wglptis)(wgl_point pts[], int npts);
  int (*wglptxs)(float *xxx, float *yyy, int *npts);
  void (*wglrfi)(int ia, int ja, int ib, int jb);
  void (*wglrli)(int ia, int ja, int ib, int jb);
  void (*wglroc)(void);
  void (*wglrstdpar)(void);
  void (*wglrwc)(void);
  void (*wglsavpfc)(void);
  void (*wglsbf)(void);
  void (*wglsetw)(int winid);
  void (*wglsetwcmap)(void);
  void (*wglsld)(int indDash);
  void (*wglsmk)(int masque);
  void (*wglspt)(int indpat);
  void (*wglstu)(int istep, int jstep);
  void (*wglswb)(void);
  int (*wglwsi)(char *string, int stringLength);
  void (*wglwtwrsz)(void);
  unsigned int (*wglcolf)(float indcouleur);
  unsigned int (*wglcolfs)(float *cols, unsigned int *pixels, int n);
  unsigned int (*wglcolfs_fst)(float *cols, unsigned int *pixels, int n);
} _wglContexte;

extern _wglContexte wglc_x, wglc_gd, wglc_gl, wglc_ps, *wglc_wgl;


#endif
