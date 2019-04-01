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

int gd_wglbitfinder(unsigned int mask, unsigned int *idebut, unsigned int *ifin);
int gd_wglopw2(char *nomFenetre);
int gd_wglafi(int i, int j, int rayon, float angleDebut, float angleFin);
int gd_wglali(int i, int j, int rayon, float angleDebut, float angleFin);
int gd_wglasi(char *string, int stringLength);
int gd_wglbbf();
int gd_wglbrb();
int gd_wglbti(int *i, int *j);
int gd_wglbtn(int bouton);
int gd_wglbtp(int bouton);
int gd_wglbtpi(int bouton, int *i, int *j);
int gd_wglcfi(int i, int j, int rayon);
int gd_wglchngfs(int NewFillStyle); 
int gd_wglcli(int i, int j, int rayon);
int gd_wglclr();
int gd_wglclw(int indFenetre);
int gd_wglcmi(int left, int bottom, int right, int top);
int gd_wglcol(int couleur);
unsigned int gd_wglcolf(float couleur);
unsigned int gd_wglcolfs(float *cols, unsigned int *pixels, int n);
unsigned int gd_wglcolfs_fst(float *cols, unsigned int *pixels, int n);
int gd_wglcrb(int couleur);
int gd_wgldbf();
int gd_wgldbg();
int gd_wgldcm();
int gd_wgldeflst(int dashIndex, int linestyle);
int gd_wgldefncw(int winid);
int gd_wgldld(int indDash, int dashPattern);
int gd_wgldpt(int indpat, int ipatsz, char iptrn[]);
int gd_wgldri(int i, int j);
int gd_wgldsi(char *string, int stringLength);
int gd_wglecr(char *nomFichier);
int gd_wglerb();
int gd_wglfbf();
int gd_wglfsc();
int gd_wglfsh();
int gd_wglfshlb();
int gd_wglfsz(int fontSize);
int gd_wglgacw();
int gd_wglgaf();
int gd_wglgbf();
int gd_wglgci();
int gd_wglgco(int indCol, int *red, int *green, int *blue);
int gd_wglgdbg();
int gd_wglgetcmap(int  *colormap);
int gd_wglgld();
int gd_wglglpti(int *ix, int *iy);
int gd_wglglw();
int gd_wglgmk();
int gd_wglgmod();
int gd_wglgpl();
int gd_wglgpt();
int gd_wglgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
int gd_wglgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int   *idebut, int   *jdebut, int   *ifin, int   *jfin);
int gd_wglgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
int gd_wglgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
int gd_wglgwz(int *isize, int *jsize);
int gd_wglgzi(int *isize, int *jsize);
int gd_wglgzp(float *xsize, float *ysize, int *isize, int *jsize);
int  gd_wglhsi(char *string, int stringLength);
int gd_wglias(int flagAspect);
int gd_wglinicmap();
int gd_wglinids(char *nomFenetre);
int gd_wglinipat();
int gd_wglinit();
int gd_wgliniwin(char *nomFenetre);
int gd_wgliniwpx();
int gd_wglinstcmap();
int gd_wglinvpat(char Pattern[]);
int gd_wglkas(int xAspect, int yAspect);
int gd_wgllpt();
int gd_wgllwi(int lineWidth);
int gd_wglmapc(int colorIndex, int r, int g, int b);
int gd_wglmapcs(int colorIndexs[], int nbCols, int rgbDefs[][3]);
int gd_wglmco(int couleur, int r, int g , int b);
int gd_wglmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
int gd_wglmcw(int couleur, int r, int g, int b);
int gd_wglmesgagr(char *message);
int gd_wglmvi(int i, int j);
int gd_wglncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
int gd_wglnewwin();
int gd_wglopmw(char *nomFenetre, int wglWinID);
int gd_wglopw(char *nomFenetre);
int gd_wglpfi(int nbPoints, int polygone[][2]);
int gd_wglpli(int nbPoints, int polygone[][2]);
int gd_wglppo(int ix, int iy, int isize, int jsize);
int gd_wglpsi(int i, int j, char *string, int stringLength, int size, int orient, int codeCentrage);
int gd_wglpsz(int isize, int jsize);
int gd_wglpti(int i, int j);
int gd_wglptis(wgl_point pts[], int npts);
int gd_wglptxs(float *xxx, float *yyy, int *npts);
int gd_wglrfi(int ia, int ja, int ib, int jb);
int gd_wglrli(int ia, int ja, int ib, int jb);
int gd_wglroc();
int gd_wglrstdpar();
int gd_wglrwc();
int gd_wglsavpfc();
int gd_wglsbf();
int gd_wglsetw(int winid);
int gd_wglsetwcmap();
int gd_wglsld(int indDash);
int gd_wglsmk(int masque);
int gd_wglspt(int indpat);
int gd_wglstu(int istep, int jstep);
int gd_wglswb();
int  gd_wglwsi(char *string, int stringLength);
int gd_wglwtwrsz();
int gd_wglxai(int *i, int *j, float x, float y);
