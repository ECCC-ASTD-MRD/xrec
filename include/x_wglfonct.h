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

int x_wglbitfinder(unsigned int mask, unsigned int *idebut, unsigned int *ifin);
int x_wglopw2(char *nomFenetre);
int x_wglafi(int i, int j, int rayon, float angleDebut, float angleFin);
int x_wglali(int i, int j, int rayon, float angleDebut, float angleFin);
int  x_wglasi(char *string, int stringLength);
int x_wglbbf();
int x_wglbrb();
int x_wglbti(int *i, int *j);
int x_wglbtn(int bouton);
int x_wglbtp(int bouton);
int x_wglbtpi(int bouton, int *i, int *j);
int x_wglcfi(int i, int j, int rayon);
int x_wglchngfs(int NewFillStyle); 
int x_wglcli(int i, int j, int rayon);
int x_wglclr();
int x_wglclw(int indFenetre);
int x_wglcmi(int left, int bottom, int right, int top);
int x_wglcol(int couleur);
unsigned int x_wglcolf(float couleur);
unsigned int x_wglcolfs(float *cols, unsigned int *pixels, int n);
unsigned int x_wglcolfs_fst(float *cols, unsigned int *pixels, int n);
int x_wglcrb(int couleur);
int x_wgldbf();
int x_wgldbg();
int x_wgldcm();
int x_wgldeflst(int dashIndex, int linestyle);
int x_wgldefncw(int winid);
int x_wgldld(int indDash, int dashPattern);
int x_wgldpt(int indpat, int ipatsz, char iptrn[]);
int x_wgldri(int i, int j);
int  x_wgldsi(char *string, int stringLength);
int x_wglerb();
int x_wglfbf();
int x_wglfsc();
int x_wglfsh();
int x_wglfshlb();
int x_wglfsz(int fontSize);
int x_wglgacw();
int x_wglgaf();
int x_wglgbf();
int x_wglgci();
int x_wglgco(int indCol, int *red, int *green, int *blue);
int x_wglgdbg();
int x_wglgetcmap(int  *colormap);
int x_wglgld();
int x_wglglpti(int *ix, int *iy);
int x_wglglw();
int x_wglgmk();
int x_wglgmod();
int x_wglgpl();
int x_wglgpt();
int x_wglgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
int x_wglgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int   *idebut, int   *jdebut, int   *ifin, int   *jfin);
int x_wglgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
int x_wglgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
int x_wglgwz(int *isize, int *jsize);
int x_wglgzi(int *isize, int *jsize);
int x_wglgzp(float *xsize, float *ysize, int *isize, int *jsize);
int  x_wglhsi(char *string, int stringLength);
int x_wglias(int flagAspect);
int x_wglinicmap();
int x_wglinids(char *nomFenetre);
int x_wglinipat();
int x_wglinit();
int x_wgliniwin(char *nomFenetre);
int x_wgliniwpx();
int x_wglinstcmap();
int x_wglinvpat(char Pattern[]);
int x_wglkas(int xAspect, int yAspect);
int x_wgllpt();
int x_wgllwi(int lineWidth);
int x_wglmapc(int colorIndex, int r, int g, int b);
int x_wglmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
int x_wglmco(int couleur, int r, int g , int b);
int x_wglmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
int x_wglmcw(int couleur, int r, int g, int b);
int x_wglmesgagr(char *message);
int x_wglmvi(int i, int j);
int x_wglncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
int x_wglnewwin();
int x_wglopmw(char *nomFenetre, int wglWinID);
int x_wglopw(char *nomFenetre);
int x_wglpfi(int nbPoints, int polygone[][2]);
int x_wglpli(int nbPoints, int polygone[][2]);
int x_wglppo(int ix, int iy, int isize, int jsize);
int x_wglpsi(int i, int j, char *string, int stringLength, int size, int orient, int codeCentrage);
int x_wglpsz(int isize, int jsize);
int x_wglpti(int i, int j);
int x_wglptis(wgl_point pts[], int npts);
int x_wglptxs(float *xxx, float *yyy, int *npts);
int x_wglrfi(int ia, int ja, int ib, int jb);
int x_wglrli(int ia, int ja, int ib, int jb);
int x_wglroc();
int x_wglrstdpar();
int x_wglrwc();
int x_wglsavpfc();
int x_wglsbf();
int x_wglsetw(int winid);
int x_wglsetwcmap();
int x_wglsld(int indDash);
int x_wglsmk(int masque);
int x_wglspt(int indpat);
int x_wglstu(int istep, int jstep);
int x_wglswb();
int  x_wglwsi(char *string, int stringLength);
int x_wglwtwrsz();
int x_wglxai(int *i, int *j, float x, float y);
