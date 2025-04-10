#ifndef _GD_WGL_H_
#define _GD_WGL_H_

#include <stdio.h>

#include <gd.h>
#include <gd_io.h>
#include <gdfontg.h>
#include <gdfontl.h>
#include <gdfontmb.h>
#include <gdfonts.h>
#include <gdfontt.h>

#include <rmn/rpnmacros.h>
#include <stdlib.h>
#include <math.h>
#include <wgl.h>

extern wgl_color couleurs[];
extern wgl_point wglPts[];
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


/**
 ** Variables liees aux patrons
 **/ 

extern unsigned long wglPatterns[];
extern char   wglDashPatterns[][16];
extern int    wglDashPatternLengths[];
extern int    wglDashPatternOffsets[];

/**
 ** Variables liees aux fontes
 **/ 

/* extern int *fonte, *fontes; */

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

#define NB_MAX_POINTS 1024

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
extern wgl_rectangle       pixmapDimensions;
extern int        w,h;

extern int        lng;
extern char       *messageAgrandir[];

/**
******************************************************************************
******************************************************************************
**/

#include "gd_wglfonct.h"

/**
 ** Structure de base pour faire le lien entre l'espace pixel et l'espace reel.
 **/ 

extern UserSpaceInfo usSpace;

/**
 ** Definition de la structure utilisee pour la gestion de fenetres multiples
 **/

/* ---------------------------------------------------------------------------------------------------------- */
extern int wgl_gdfsz(int fontSize);
extern int wgl_gdpsi(int i, int j, char string[], int stringLength, int size, int orient, int codeCentrage);
extern int wgl_gdhsi(char string[], int stringLength);
extern int wgl_gdasi(char string[], int stringLength);
extern int wgl_gddsi(char string[], int stringLength);
extern int wgl_gdafi(int i, int j, int rayon, float angleDebut, float angleFin);
extern int wgl_gdafx(float x, float y, float rayon, float angleDebut, float angleFin);
extern int wgl_gdali(int i, int j, int rayon, float angleDebut, float angleFin);
extern int wgl_gdalx(float x, float y, float rayon, int angleDebut, int angleFin);
extern int wgl_gdbbf(void);
extern int wgl_gdbrb(void);
extern int wgl_gdbti(int *i, int *j);
extern int wgl_gdbtn(int bouton);
extern int wgl_gdbtpi(int bouton, int *i, int *j);
extern int wgl_gdbtx(float *x, float *y);
extern int wgl_gdcfi(int i, int j, int rayon);
extern int wgl_gdcfx(float x, float y, float rayon);
extern int wgl_gdchngfs(int NewFillStyle);
extern int wgl_gdcli(int i, int j, int rayon);
extern int wgl_gdclr(void);
extern int wgl_gdclw(int indFenetre);
extern int wgl_gdclx(float x, float y, float rayon);
extern int wgl_gdcmi(int left, int bottom, int right, int top);
extern int wgl_gdcmx(int left, int bottom, int right, int top);
extern int wgl_gdcolf(float couleur);
extern int wgl_gdcolfs(float *cols, unsigned int *pixels, int n);
extern int wgl_gdcolfs_fst(float *cols, unsigned int *pixels, int n);
extern int wgl_gdcol(int couleur);
extern int wgl_gdcrb(int couleur);
extern int wgl_gddbf();
extern int wgl_gddbg();
extern int wgl_gddcm();
extern int wgl_gddeflst(int dashIndex, int linestyle);
extern int wgl_gddefncw(int winid);
extern int wgl_gddld(int indDash, int dashPattern);
extern int wgl_gddpt(int indpat, int ipatsz, char iptrn[]);
extern int wgl_gddri(int i, int j);
extern int wgl_gdecr(char *nomFichier);
extern int wgl_gderb();
extern int wgl_gdfbf();
extern int wgl_gdfsc();
extern int wgl_gdfshlb();
extern int wgl_gdfsh();
extern int wgl_gdgacw();
extern int wgl_gdgaf();
extern int wgl_gdgbf();
extern int wgl_gdgci();
extern int wgl_gdgco(int indCol, int *red, int *green, int *blue);
extern int wgl_gdgdbg();
extern int wgl_gdgetcmap(unsigned long *colormap);
extern int wgl_gdgld();
extern int wgl_gdglpti(int *ix, int *iy);
extern int wgl_gdglw();
extern int wgl_gdgmk();
extern int wgl_gdgmod();
extern int wgl_gdgpl();
extern int wgl_gdgpt();
extern int wgl_gdgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_gdgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_gdgsx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int wgl_gdgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_gdgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int wgl_gdgvx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int wgl_gdgwz(int *isize, int *jsize);
extern int wgl_gdgzi(int *isize, int *jsize);
extern int wgl_gdgzp(float *xsize, float *ysize, int *isize, int *jsize);
extern int wgl_gdgzx(float *xsize, float *ysize);
extern int wgl_gdias(int flagAspect);
extern int wgl_gdiax(float *x, float *y, int i, int j);
extern int wgl_gdinit();
extern int wgl_gdinicmap();
extern int wgl_gdinids(char *nomFenetre);
extern int wgl_gdinipat();
extern int wgl_gdiniwin(char *nomFenetre);
extern int wgl_gdiniwpx();
extern int wgl_gdinstcmap();
extern int wgl_gdinvpat(char Pattern[]);
extern int wgl_gdkas(int xAspect, int yAspect);
extern int wgl_gdlpt();
extern int wgl_gdlwi(int lineWidth);
extern int wgl_gdmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
extern int wgl_gdmapc(int colorIndex, int r, int g, int b);
extern int wgl_gdmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
extern int wgl_gdmco(int couleur, int r, int g, int b);
extern int wgl_gdmcw(int couleur, int r, int g, int b);
extern int wgl_gdmesgagr();
extern int wgl_gdmvi(int i, int j);
extern int wgl_gdmvx(float x, float y);
extern int wgl_gdncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
extern int wgl_gdnewwin();
extern int wgl_gdopmw();
extern int wgl_gdopw(char *nomFenetre);
extern int wgl_gdpfi(int nbPoints, int polygone[][2]);
extern int wgl_gdpfx(int nbPoints, float polygone[][2]);
extern int wgl_gdpli(int nbPoints, int polygone[][2]);
extern int wgl_gdplx(int nbPoints, float polygone[][2]);
extern int wgl_gdppo(int ix, int iy, int isize, int jsize);
extern int wgl_gdpsz(int isize, int jsize);
extern int wgl_gdptis(wgl_point pts, int npts);
extern int wgl_gdpti(int i, int j);
extern int wgl_gdptxs(float *xxx, float *yyy, int *npts);
extern int wgl_gdptx(float x, float y);
extern int wgl_gdrfi(int ia, int ja, int ib, int jb);
extern int wgl_gdrfx(float xa, float ya, float xb, float yb);
extern int wgl_gdrli(int ia, int ja, int ib, int jb);
extern int wgl_gdrlx(float xa, float ya, float xb, float yb);
extern int wgl_gdroc();
extern int wgl_gdrstdpar();
extern int wgl_gdrwc();
extern int wgl_gdsavpfc();
extern int wgl_gdsbf();
extern int wgl_gdsetwcmap();
extern int wgl_gdsetw(int winid);
extern int wgl_gdsld(int indDash);
extern int wgl_gdsmk(int masque);
extern int wgl_gdspt(int indpat);
extern int wgl_gdssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);
extern int wgl_gdstu(int istep, int jstep);
extern int wgl_gdswb();
extern int wgl_gdxai(int *i, int *j, float x, float y);
/* ---------------------------------------------------------------------------------------------------------- */

#endif
