#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpnmacros.h>
#ifndef __CPLUSPPLUS
#include <math.h>
#endif

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
  int (*wglxai)(int *i, int *j, float x, float y);
  int (*wglafi)(int i, int j, int rayon, float angleDebut, float angleFin);
  int (*wglali)(int i, int j, int rayon, float angleDebut, float angleFin);
  int (*wglasi)(char string[], int stringLength);
  int (*wglbbf)(void);
  int (*wglbitfinder)(unsigned int mask, unsigned int *idebut, unsigned int *ifin);
  int (*wglbrb)(void);
  int (*wglbti)(int *i, int *j);
  int (*wglbtn)(int bouton);
  int (*wglbtpi)(int bouton, int *i, int *j);
  int (*wglcfi)(int i, int j, int rayon);
  int (*wglchngfs)(int NewFillStyle);
  int (*wglcli)(int i, int j, int rayon);
  int (*wglclr)(void);
  int (*wglclw)(int indFenetre);
  int (*wglcmi)(int left, int bottom, int right, int top);
  int (*wglcol)(int xcouleur);
  int (*wglcrb)(int xcouleur);
  int (*wgldbf)(void);
  int (*wgldbg)(void);
  int (*wgldcm)(void);
  int (*wgldeflst)(int dashIndex, int linestyle);
  int (*wgldefncw)(int winid);
  int (*wgldld)(int indDash, int dashPattern);
  int (*wgldpt)(int indpat, int ipatsz, char iptrn[]);
  int (*wgldri)(int i, int j);
  int (*wgldsi)(char string[], int stringLength);
  int (*wglecr)(char *nomFichier);
  int (*wglerb)(void);
  int (*wglfbf)(void);
  int (*wglfsc)(void);
  int (*wglfsh)(void);
  int (*wglfshlb)(void);
  int (*wglfsz)(int fontSize);
  int (*wglgacw)(void);
  int (*wglgaf)(void);
  int (*wglgbf)(void);
  int (*wglgci)(void);
  int (*wglgco)(int indCol, int *red, int *green, int *blue);
  int (*wglgdbg)(void);
  int (*wglgetcmap)(unsigned long *colormap);
  int (*wglgld)(void);
  int (*wglglpti)(int *ix, int *iy);
  int (*wglglw)(void);
  int (*wglgmk)(void);
  int (*wglgmod)(void);
  int (*wglgpl)(void);
  int (*wglgpt)(void);
  int (*wglgsi)(int *idebut, int *jdebut, int *ifin, int *jfin);
  int (*wglgvi)(int *idebut, int *jdebut, int *ifin, int *jfin);
  int (*wglgvp)(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
  int (*wglgvx)(float *xdebut, float *ydebut, float *xfin, float *yfin);
  int (*wglgwz)(int *isize, int *jsize);
  int (*wglgzi)(int *isize, int *jsize);
  int (*wglgzp)(float *xsize, float *ysize, int *isize, int *jsize);
  int (*wglgzx)(float *xsize, float *ysize);
  int (*wglhsi)(char string[], int stringLength);
  int (*wglias)(int flagAspect);
  int (*wglinicmap)(void);
  int (*wglinids)(char *nomFenetre);
  int (*wglinipat)(void);
  int (*wglinit)(void);
  int (*wgliniwin)(char *nomFenetre);
  int (*wgliniwpx)(void);
  int (*wglinstcmap)(void);
  int (*wglinvpat)(char Pattern[]);
  int (*wglkas)(int xAspect, int yAspect);
  int (*wgllpt)(void);
  int (*wgllwi)(int lineWidth);
  int (*wglmapc)(int colorIndex, int r, int g, int b);
  int (*wglmapcs)(int colorIndexs[], int nbCols, int rgbDefs[][3]);
  int (*wglmco)(int xcouleur, int r, int g, int b);
  int (*wglmcos)(int xcouleurs[], int nbCols, int rgbDefs[][3]);
  int (*wglmcw)(int xcouleur, int r, int g, int b);
  int (*wglmesagr)(char *message);
  int (*wglmvi)(int i, int j);
  int (*wglncl)(int *ncolRead, int *ncolWrite, int *ncolAnim);
  int (*wglnewwin)(void);
  int (*wglopmw)(char *nomFenetre, int wglWinId);
  int (*wglopw)(char *nomFenetre);
  int (*wglpfi)(int nbPoints, int polygone[][2]);
  int (*wglpli)(int nbPoints, int polygone[][2]);
  int (*wglppo)(int ix, int iy, int isize, int jsize);
  int (*wglpsi)(int i, int j, char string[], int stringLength, int size, int orient, int codeCentrage);
  int (*wglpsz)(int isize, int jsize);
  int (*wglpti)(int i, int j);
  int (*wglptis)(wgl_point pts[], int npts);
  int (*wglptxs)(float *xxx, float *yyy, int *npts);
  int (*wglrfi)(int ia, int ja, int ib, int jb);
  int (*wglrli)(int ia, int ja, int ib, int jb);
  int (*wglroc)(void);
  int (*wglrstdpar)(void);
  int (*wglrwc)(void);
  int (*wglsavpfc)(void);
  int (*wglsbf)(void);
  int (*wglsetw)(int winid);
  int (*wglsetwcmap)(void);
  int (*wglsld)(int indDash);
  int (*wglsmk)(int masque);
  int (*wglspt)(int indpat);
  int (*wglstu)(int istep, int jstep);
  int (*wglswb)(void);
  int (*wglwsi)(char *string, int stringLength);
  int (*wglwtwrsz)(void);
  unsigned int (*wglcolf)(float indcouleur);
  unsigned int (*wglcolfs)(float *cols, unsigned int *pixels, int n);
  unsigned int (*wglcolfs_fst)(float *cols, unsigned int *pixels, int n);
} _wglContexte;

extern _wglContexte wglc_x, wglc_gd, wglc_gl, wglc_ps, *wglc_wgl;

