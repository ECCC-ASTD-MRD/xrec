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

#include <wgl.h>

extern int c_wgl_xfsz(int fontSize);
extern int c_wgl_xpsi(int i, int j, char string[], int stringLength, int size, int orient, int codeCentrage);
extern int c_wgl_xhsi(char string[], int stringLength);
extern int c_wgl_xasi(char string[], int stringLength);
extern int c_wgl_xdsi(char string[], int stringLength);
extern int c_wgl_xafi(int i, int j, int rayon, float angleDebut, float angleFin);
extern int c_wgl_xafx(float x, float y, float rayon, float angleDebut, float angleFin);
extern int c_wgl_xali(int i, int j, int rayon, float angleDebut, float angleFin);
extern int c_wgl_xalx(float x, float y, float rayon, int angleDebut, int angleFin);
extern int c_wgl_xbbf(void);
extern int c_wgl_xbrb(void);
extern int c_wgl_xbti(int *i, int *j);
extern int c_wgl_xbtn(int bouton);
extern int c_wgl_xbtpi(int bouton, int *i, int *j);
extern int c_wgl_xbtx(float *x, float *y);
extern int c_wgl_xcfi(int i, int j, int rayon);
extern int c_wgl_xcfx(float x, float y, float rayon);
extern int wglchngfs(int NewFillStyle);
extern int c_wgl_xcli(int i, int j, int rayon);
extern int c_wgl_xclr(void);
extern int c_wgl_xclw(int indFenetre);
extern int c_wgl_xclx(float x, float y, float rayon);
extern int c_wgl_xcmi(int left, int bottom, int right, int top);
extern int c_wgl_xcmx(int left, int bottom, int right, int top);
extern int c_wgl_xcolf(float couleur);
extern int c_wgl_xcolfs(float *cols, unsigned int *pixels, int n);
extern int c_wgl_xcolfs_fst(float *cols, unsigned int *pixels, int n);
extern int c_wgl_xcol(int couleur);
extern int c_wgl_xcrb(int couleur);
extern int c_wgl_xdbf();
extern int c_wgl_xdbg();
extern int c_wgl_xdcm();
extern int wgldeflst(int dashIndex, int linestyle);
extern int wgldefncw(int winid);
extern int c_wgl_xdld(int indDash, int dashPattern);
extern int c_wgl_xdpt(int indpat, int ipatsz, char iptrn[]);
extern int c_wgl_xdri(int i, int j);
extern int c_wgl_xerb();
extern int c_wgl_xfbf();
extern int c_wgl_xfsc();
extern int wglfshlb();
extern int c_wgl_xfsh();
extern int c_wgl_xgacw();
extern int c_wgl_xgaf();
extern int c_wgl_xgbf();
extern int c_wgl_xgci();
extern int c_wgl_xgco(int indCol, int *red, int *green, int *blue);
extern int c_wgl_xgdbg();
extern int c_wgl_xgetcmap(unsigned long *colormap);
extern int c_wgl_xgld();
extern int c_wgl_xglpti(int *ix, int *iy);
extern int c_wgl_xglw();
extern int c_wgl_xgmk();
extern int c_wgl_xgmod();
extern int c_wgl_xgpl();
extern int c_wgl_xgpt();
extern int c_wgl_xgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_xgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_xgsx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int c_wgl_xgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_xgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_xgvx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int c_wgl_xgwz(int *isize, int *jsize);
extern int c_wgl_xgzi(int *isize, int *jsize);
extern int c_wgl_xgzp(float *xsize, float *ysize, int *isize, int *jsize);
extern int c_wgl_xgzx(float *xsize, float *ysize);
extern int c_wgl_xias(int flagAspect);
extern int c_wgl_xiax(float *x, float *y, int i, int j);
extern int wglinicmap();
extern int wglinids(char *nomFenetre);
extern int wglinipat();
extern int wgliniwin(char *nomFenetre);
extern int wgliniwpx();
extern int c_wgl_xinstcmap();
extern int wglinvpat(char Pattern[]);
extern int c_wgl_xkas(int xAspect, int yAspect);
extern int c_wgl_xlpt();
extern int c_wgl_xlwi(int lineWidth);
extern int wglmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
extern int wglmapc(int colorIndex, int r, int g, int b);
extern int c_wgl_xmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
extern int c_wgl_xmco(int couleur, int r, int g, int b);
extern int c_wgl_xmcw(int couleur, int r, int g, int b);
extern int c_wgl_xmvi(int i, int j);
extern int c_wgl_xmvx(float x, float y);
extern int c_wgl_xncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
extern int wglnewwin();
extern int c_wgl_xopw(char *nomFenetre);
extern int c_wgl_xpfi(int nbPoints, int polygone[][2]);
extern int c_wgl_xpfx(int nbPoints, float polygone[][2]);
extern int c_wgl_xpli(int nbPoints, int polygone[][2]);
extern int c_wgl_xplx(int nbPoints, float polygone[][2]);
extern int c_wgl_xppo(int ix, int iy, int isize, int jsize);
extern int c_wgl_xpsz(int isize, int jsize);
extern int c_wgl_xptis(wgl_point pts, int npts);
extern int c_wgl_xpti(int i, int j);
extern int c_wgl_xptxs(float *xxx, float *yyy, int *npts);
extern int c_wgl_xptx(float x, float y);
extern int c_wgl_xrfi(int ia, int ja, int ib, int jb);
extern int c_wgl_xrfx(float xa, float ya, float xb, float yb);
extern int c_wgl_xrli(int ia, int ja, int ib, int jb);
extern int c_wgl_xrlx(float xa, float ya, float xb, float yb);
extern int c_wgl_xroc();
extern int wglrstdpar();
extern int c_wgl_xrwc();
extern int wglsavpfc();
extern int c_wgl_xsbf();
extern int c_wgl_xsetwcmap();
extern int c_wgl_xsetw(int winid);
extern int c_wgl_xsld(int indDash);
extern int c_wgl_xsmk(int masque);
extern int c_wgl_xspt(int indpat);
extern int c_wgl_xssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);
extern int c_wgl_xstu(int istep, int jstep);
extern int c_wgl_xswb();
extern int c_wgl_xxai(int *i, int *j, float x, float y);
