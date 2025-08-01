#ifndef _WGLFONCT_H_
#define _WGLFONCT_H_

#ifdef __CPLUSPLUS
extern "C"
{
#endif
float c_wglasx(char *string, int stringLength);
float c_wgldsx(char *string, int stringLength);
float c_wglhsx(char *string, int stringLength);
float c_wglwsx(char *string, int stringLength);
int  c_wglasi(char *string, int stringLength);
int  c_wgldsi(char *string, int stringLength);
int  c_wglhsi(char *string, int stringLength);
int  c_wglpsx(float x, float y, char *string, int stringLength, int size, int orient, int codeCentrage);
int  c_wglwsi(char *string, int stringLength);
void c_wglafi(int i, int j, int rayon, float angleDebut, float angleFin);
void c_wglafx(float x, float y, float rayon, float angleDebut, float angleFin);
void c_wglali(int i, int j, int rayon, float angleDebut, float angleFin);
int c_wglalx(float x, float y, float rayon, float angleDebut, float angleFin);
void c_wglbbf();
void c_wglbrb();
int c_wglbti(int *i, int *j);
int c_wglbtn(int bouton);
int c_wglbtp(int bouton);
int c_wglbtpi(int bouton, int *i, int *j);
int c_wglbtpx(int bouton, float *x, float *y);
int c_wglbtx(float *x, float *y);
void c_wglcfi(int i, int j, int rayon);
void c_wglcfx(float x, float y, float rayon);
void c_wglcli(int i, int j, int rayon);
void c_wglclr();
void c_wglclw(int indFenetre);
void c_wglclx(float x, float y, float rayon);
void c_wglcmi(int left, int bottom, int right, int top);
void c_wglcmx(float left, float bottom, float right, float top);
void c_wglcol(int couleur);
void c_wglcrb(int couleur);
void c_wgldbf();
void c_wgldbg();
void c_wgldcm();
void c_wgldld(int indDash, int dashPattern);
void c_wgldpt(int indpat, int ipatsz, char iptrn[]);
void c_wgldri(int i, int j);
void c_wgldrx(float x, float y);
void c_wglecr(char *nomFenetre);
void c_wglerb();
void c_wglfbf();
void c_wglfsc();
void c_wglfsh();
void c_wglfshlb();
int c_wglfsz(int fontSize);
int c_wglgacw();
int c_wglgaf();
int c_wglgbf();
int c_wglgci();
void c_wglgco(int indCol, int *red, int *green, int *blue);
int c_wglgdbg();
int c_wglgetcmap(Colormap *colormap);
int c_wglgld();
void c_wglglpti(int *ix, int *iy);
int c_wglglw();
int c_wglgmk();
int c_wglgmod();
int c_wglgpl();
int c_wglgpt();
void c_wglgsi(int *idebut, int *jdebut, int *ifin, int *jfin);
void c_wglgsp(float *xdebut, float *ydebut, float *xfin, float *yfin, int   *idebut, int *jdebut, int *ifin, int *jfin);
void c_wglgsx(float *xdebut, float *ydebut, float *xfin, float *yfin);
void c_wglgvi(int *idebut, int *jdebut, int *ifin, int *jfin);
void c_wglgvp(float *xdebut, float *ydebut, float *xfin, float *yfin, int *idebut, int *jdebut, int *ifin, int *jfin);
int c_wglgvx(float *xdebut, float *ydebut, float *xfin, float *yfin);
void c_wglgwz(int *isize, int *jsize);
void c_wglgzi(int *isize, int *jsize);
void c_wglgzp(float *xsize, float *ysize, int *isize, int *jsize);
void c_wglgzx(float *xsize, float *ysize);
void c_wglias(int flagAspect);
void c_wgliax(float *x, float *y, int i, int j);
void c_wglinit();
void c_wglinstcmap();
void c_wglkas(int xAspect, int yAspect);
void c_wgllpt();
void c_wgllwi(int lineWidth);
void c_wglmco(int couleur, int r, int g, int b);
void c_wglmcos(int couleurs[], int nbCols, int rgbDefs[][3]);
void c_wglmcw(int couleur, int r, int g, int b);
void c_wglmvi(int i, int j);
void c_wglmvx(float x, float y);
void c_wglncl(int *ncolRead, int *ncolWrite, int *ncolAnim);
int c_wglopmw(char *nomFenetre, int wglWinID);
int c_wglopw(char *nomFenetre);
void c_wglpfi(int nbPoints, int polygone[][2]);
int c_wglpfx(int nbPoints, float polygone[][2]);
void c_wglpli (int nbPoints, int polygone[][2]);
void c_wglplx(int nbPoints, float polygone[][2]);
void c_wglppo(int ix, int iy, int isize, int jsize);
int c_wglpsi(int i, int j, char *string, int stringLength, int size, int orient, int codeCentrage);
void c_wglpsz(int isize, int jsize);
void c_wglpti(int i, int j);
void c_wglptis(wgl_point pts[], int npts);
void c_wglptx(float x, float y);
void c_wglptxs(float *xxx, float *yyy, int *npts);
void c_wglrfi(int ia, int ja, int ib, int jb);
void c_wglrfx(float xa, float ya, float xb, float yb);
void c_wglrli(int ia, int ja, int ib, int jb);
void c_wglrlx(float xa, float ya, float xb, float yb);
void c_wglroc();
void c_wglrwc();
void c_wglsbf();
int c_wglscon(char *contexte);
void c_wglscon_x();
void c_wglsetw(int winid);
void c_wglsetwcmap();
void c_wglsld(int indDash);
void c_wglsmk(int masque);
void c_wglspt(int indpat);
void c_wglstu(int istep, int jstep);
void c_wglswb();
void c_wglxai(int *i, int *j, float x, float y);
unsigned int c_wglcolf(float couleur);
unsigned int c_wglcolfs(float *cols, unsigned int *pixels, int n);
unsigned int c_wglcolfs_fst(float *cols, unsigned int *pixels, int n);
void c_wglssp(float xdebut, float ydebut, float xfin, float yfin, int idebut, int jdebut, int ifin, int jfin, int mode);
void c_fxfy2xy(float *x, float *y, float fx, float fy);
void c_vfxfy2xy(float *x, float *y, float *fx, float *fy, int npts);
void c_vxy2fxfy(float *fx, float *fy, float *x, float *y, int npts);
void c_xsetxy(int mode, float *tx, int nx, float *ty, int ny);
void c_xy2fxfy(float *fx, float *fy, float x, float y);
void f77name(fxfy2xy)(float *x, float *y, float *fx, float *fy);
void f77name(vfxfy2xy)(float *x, float *y, float *fx, float *fy, int *npts);
void f77name(vxy2fxfy)(float *fx, float *fy, float *x, float *y, int *npts);
void f77name(xsetxy)(int *mode, float *tx, int *nx, float *ty, int *ny);
void f77name(xy2fxfy)(float *fx, float *fy, float *x, float *y);
void BuildColIndexTable(int indTable[], int hsvTable[][3], int nbCol, int colDebut, int colFin, int mode);
void RestoreNormalMode(void);
void EnterOverlayMode(void);
int f77name(minisouris)(int *bouton, int *event, int *x0, int *y0, int *xsize, int *ysize, int *x1, int *y1, int *x2, int *y2);
int defpup(char *menuStr);
int f77name(minisouris)(int *bouton, int *event, int *x0, int *y0, int *xsize, int *ysize, int *x1, int *y1, int *x2, int *y2);
int f77name(wglfsz)(int *fontSize);
void f77name(wglmvx)(float *x, float *y);
void wglfshlb(void);

#ifdef __CPLUSPLUS
}
#endif

#endif
