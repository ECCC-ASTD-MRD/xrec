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

extern int c_wgl_gdfsz(int fontSize);
extern int c_wgl_gdpsi(int i, int j, char string[], int stringLength, int size, int orient, int codeCentrage);
extern int c_wgl_gdhsi(char string[], int stringLength);
extern int c_wgl_gdasi(char string[], int stringLength);
extern int c_wgl_gddsi(char string[], int stringLength);
extern int c_wgl_gdafi(int i, int j, int rayon, float angleDebut, float angleFin);
extern int c_wgl_gdafx(float x, float y, float rayon, float angleDebut, float angleFin);
extern int c_wgl_gdali(int i, int j, int rayon, float angleDebut, float angleFin);
extern int c_wgl_gdalx(float x, float y, float rayon, int angleDebut, int angleFin);
extern int c_wgl_gdbbf(void);
extern int c_wgl_gdbrb(void);
extern int c_wgl_gdbti(int *i, int *j);
extern int c_wgl_gdbtn(int bouton);
extern int c_wgl_gdbtpi(int bouton, int *i, int *j);
extern int c_wgl_gdbtx(float *x, float *y);
extern int c_wgl_gdcfi(int i, int j, int rayon);
extern int c_wgl_gdcfx(float x, float y, float rayon);
extern int wglchngfs(int NewFillStyle);
extern int c_wgl_gdcli(int i, int j, int rayon);
extern int c_wgl_gdclr(void);
extern int c_wgl_gdclw(int indFenetre);
extern int c_wgl_gdclx(float x, float y, float rayon);
extern int c_wgl_gdcmi(int left, int bottom, int right, int top);
extern int c_wgl_gdcmx(int left, int bottom, int right, int top);
extern int c_wgl_gdcolf(float couleur);
extern int c_wgl_gdcolfs(float *cols, unsigned int *pixels, int n);
extern int c_wgl_gdcolfs_fst(float *cols, unsigned int *pixels, int n);
extern int c_wgl_gdcol(int couleur);
extern int c_wgl_gdcrb(int couleur);
extern int c_wgl_gddbf();
extern int c_wgl_gddbg();
extern int c_wgl_gddcm();
extern int wgldeflst(int dashIndex, int linestyle);
extern int wgldefncw(int winid);
extern int c_wgl_gddld(int indDash, int dashPattern);
extern int c_wgl_gddpt(int indpat, int ipatsz, char iptrn[]);
extern int c_wgl_gddri(int i, int j);
extern int c_wgl_gderb();
extern int c_wgl_gdfbf();
extern int c_wgl_gdfsc();
extern void wglfshlb();
extern int c_wgl_gdfsh();
extern int c_wgl_gdgacw();
extern int c_wgl_gdgaf();
extern int c_wgl_gdgbf();
extern int c_wgl_gdgci();
extern int c_wgl_gdgco(int indCol, int *red, int *green, int *blue);
extern int c_wgl_gdgdbg();
extern int c_wgl_gdgetcmap(unsigned long *colormap);
extern int c_wgl_gdgld();
extern int c_wgl_gdglpti(int *ix, int *iy);
extern int c_wgl_gdglw();
extern int c_wgl_gdgmk();
extern int c_wgl_gdgmod();
extern int c_wgl_gdgpl();
extern int c_wgl_gdgpt();
extern int c_wgl_gdgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_gdgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_gdgsx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int c_wgl_gdgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_gdgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
extern int c_wgl_gdgvx(float *xdebut, float *ydebut, float *xfin, float *yfin);
extern int c_wgl_gdgwz(int *isize, int *jsize);
extern int c_wgl_gdgzi(int *isize, int *jsize);
extern int c_wgl_gdgzp(float *xsize, float *ysize, int *isize, int *jsize);
extern int c_wgl_gdgzx(float *xsize, float *ysize);
extern int c_wgl_gdias(int flagAspect);
extern int c_wgl_gdiax(float *x, float *y, int i, int j);
extern int wglinicmap();
extern int wglinids(char *nomFenetre);
extern int wglinipat();
extern int wgliniwin(char *nomFenetre);
extern int wgliniwpx();
extern int c_wgl_gdinstcmap();
extern int wglinvpat(char Pattern[]);
extern int c_wgl_gdkas(int xAspect, int yAspect);
extern int c_wgl_gdlpt();
extern int c_wgl_gdlwi(int lineWidth);
extern int wglmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
extern int wglmapc(int colorIndex, int r, int g, int b);
extern int c_wgl_gdmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
extern int c_wgl_gdmco(int couleur, int r, int g, int b);
extern int c_wgl_gdmcw(int couleur, int r, int g, int b);
extern int c_wgl_gdmvi(int i, int j);
extern int c_wgl_gdmvx(float x, float y);
extern int c_wgl_gdncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
extern int wglnewwin();
extern int c_wgl_gdopw(char *nomFenetre);
extern int c_wgl_gdpfi(int nbPoints, int polygone[][2]);
extern int c_wgl_gdpfx(int nbPoints, float polygone[][2]);
extern int c_wgl_gdpli(int nbPoints, int polygone[][2]);
extern int c_wgl_gdplx(int nbPoints, float polygone[][2]);
extern int c_wgl_gdppo(int ix, int iy, int isize, int jsize);
extern int c_wgl_gdpsz(int isize, int jsize);
extern int c_wgl_gdptis(wgl_point pts, int npts);
extern int c_wgl_gdpti(int i, int j);
extern int c_wgl_gdptxs(float *xxx, float *yyy, int *npts);
extern int c_wgl_gdptx(float x, float y);
extern int c_wgl_gdrfi(int ia, int ja, int ib, int jb);
extern int c_wgl_gdrfx(float xa, float ya, float xb, float yb);
extern int c_wgl_gdrli(int ia, int ja, int ib, int jb);
extern int c_wgl_gdrlx(float xa, float ya, float xb, float yb);
extern int c_wgl_gdroc();
extern int wglrstdpar();
extern int c_wgl_gdrwc();
extern int wglsavpfc();
extern int c_wgl_gdsbf();
extern int c_wgl_gdsetwcmap();
extern int c_wgl_gdsetw(int winid);
extern int c_wgl_gdsld(int indDash);
extern int c_wgl_gdsmk(int masque);
extern int c_wgl_gdspt(int indpat);
extern int c_wgl_gdssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);
extern int c_wgl_gdstu(int istep, int jstep);
extern int c_wgl_gdswb();
extern int c_wgl_gdxai(int *i, int *j, float x, float y);
