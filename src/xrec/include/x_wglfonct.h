#ifndef _X_WGLFONCT_H_
#define _X_WGLFONCT_H_

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

void x_wglbitfinder(unsigned int mask, unsigned int *idebut, unsigned int *ifin);
int x_wglopw2(char *nomFenetre);
void x_wglafi(int i, int j, int rayon, float angleDebut, float angleFin);
void x_wglali(int i, int j, int rayon, float angleDebut, float angleFin);
int x_wglasi(char *string, int stringLength);
void x_wglbbf();
void x_wglbrb();
int x_wglbti(int *i, int *j);
int x_wglbtn(int bouton);
int x_wglbtp(int bouton);
int x_wglbtpi(int bouton, int *i, int *j);
void x_wglcfi(int i, int j, int rayon);
void x_wglchngfs(int NewFillStyle);
void x_wglcli(int i, int j, int rayon);
void x_wglclr();
void x_wglclw(int indFenetre);
void x_wglcmi(int left, int bottom, int right, int top);
void x_wglcol(int couleur);
unsigned int x_wglcolf(float couleur);
unsigned int x_wglcolfs(float *cols, unsigned int *pixels, int n);
unsigned int x_wglcolfs_fst(float *cols, unsigned int *pixels, int n);
void x_wglcrb(int couleur);
void x_wgldbf();
void x_wgldbg();
void x_wgldcm();
void x_wgldeflst(int dashIndex, int linestyle);
void x_wgldefncw(int winid);
void x_wgldld(int indDash, int dashPattern);
void x_wgldpt(int indpat, int ipatsz, char iptrn[]);
void x_wgldri(int i, int j);
int x_wgldsi(char *string, int stringLength);
void x_wglecr(char *nomFichier);
void x_wglerb();
void x_wglfbf();
void x_wglfsc();
void x_wglfsh();
void x_wglfshlb();
int x_wglfsz(int fontSize);
int x_wglgacw();
int x_wglgaf();
int x_wglgbf();
int x_wglgci();
void x_wglgco(int indCol, int *red, int *green, int *blue);
int x_wglgdbg();
int x_wglgetcmap(Colormap *colormap);
int x_wglgld();
void x_wglglpti(int *ix, int *iy);
int x_wglglw();
int x_wglgmk();
int x_wglgmod();
int x_wglgpl();
int x_wglgpt();
void x_wglgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
int x_wglgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
void x_wglgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
void x_wglgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
void x_wglgwz(int *isize, int *jsize);
void x_wglgzi(int *isize, int *jsize);
void x_wglgzp(float *xsize, float *ysize, int *isize, int *jsize);
int x_wglhsi(char *string, int stringLength);
void x_wglias(int flagAspect);
void x_wglinicmap();
void x_wglinids(char *nomFenetre);
void x_wglinipat();
void x_wglinit();
void x_wgliniwin(char *nomFenetre);
void x_wgliniwpx();
void x_wglinstcmap();
void x_wglinvpat(char Pattern[]);
void x_wglkas(int xAspect, int yAspect);
void x_wgllpt();
void x_wgllwi(int lineWidth);
void x_wglmapc(int colorIndex, int r, int g, int b);
void x_wglmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
void x_wglmco(int couleur, int r, int g, int b);
void x_wglmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
void x_wglmcw(int couleur, int r, int g, int b);
void x_wglmesagr(char *message);
void x_wglmvi(int i, int j);
void x_wglncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
void x_wglnewwin();
int x_wglopmw(char *nomFenetre, int wglWinID);
int x_wglopw(char *nomFenetre);
void x_wglpfi(int nbPoints, int polygone[][2]);
void x_wglpli(int nbPoints, int polygone[][2]);
void x_wglppo(int ix, int iy, int isize, int jsize);
int x_wglpsi(int i, int j, char *string, int stringLength, int size, int orient, int codeCentrage);
void x_wglpsz(int isize, int jsize);
void x_wglpti(int i, int j);
void x_wglptis(wgl_point pts[], int npts);
int x_wglptxs(float *xxx, float *yyy, int *npts);
void x_wglrfi(int ia, int ja, int ib, int jb);
void x_wglrli(int ia, int ja, int ib, int jb);
void x_wglroc();
void x_wglrstdpar();
void x_wglrwc();
void x_wglsavpfc();
void x_wglsbf();
void x_wglsetw(int winid);
void x_wglsetwcmap();
void x_wglsld(int indDash);
void x_wglsmk(int masque);
void x_wglspt(int indpat);
void x_wglstu(int istep, int jstep);
void x_wglswb();
int x_wglwsi(char *string, int stringLength);
void x_wglwtwrsz();
void x_wglxai(int *i, int *j, float x, float y);
unsigned int x_wglpix(int icol);

int dopopup(int indMenu, int posX, int posY);
void x_wglgsx(float *idebut, float *jdebut, float *ifin, float *jfin);
int x_wglgzx(float *xsize, float *ysize);
int InitFonte(int fontSize);
int FlusherTousLesEvenements(void);
unsigned char x_invertChar(int nombre);
void f77name(x_wwwlpt)(char *patfil, F2Cl);
int MatchColorIndexX(int r, int g, int b, XColor colorTable[], int colDebut, int colFin);
void wglmapc(int colorIndex, int r, int g, int b);
void wglinids(char *nomFenetre);

#endif
