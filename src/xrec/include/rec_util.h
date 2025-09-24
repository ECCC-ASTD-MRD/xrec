#ifndef _REC_UTIL_CPROTO_H_
#define _REC_UTIL_CPROTO_H_

#include  <X11/Intrinsic.h>

#include "rmn/base.h"

/* rec_util/gmp/c_gmpdrw.c */
int f77name(gmpdrw)(void);
void c_gmpdrw(void);
/* rec_util/gmp/c_gmpg2l.c */
void c_gmpg2l(float *lat, float *lon, float x, float y);
/* rec_util/gmp/c_gmpinit.c */
int c_gmpinit(void);
/* rec_util/gmp/c_gmpl2g.c */
void c_gmpl2g(float *x, float *y, float lat, float lon);
/* rec_util/gmp/c_gmpopt.c */
int c_gmpopts(char *option, char *valeur);
int c_gmpopti(char *option, int32_t valeur);
/* rec_util/gmp/c_gmpset.c */
int f77name(gmpset)(char *grtyp, int *ni, int *nj, int *ig1, int *ig2, int *ig3, int *ig4, F2Cl lengrtyp);
int c_gmpset(char grtyp, int ni, int nj, int ig1, int ig2, int ig3, int ig4);
/* rec_util/gmp/c_gmpzset.c */
int c_gmpzset(char grtyp, int ni, int nj, int ig1, int ig2, int ig3, int ig4, char grtyp_ref, int ig1ref, int ig2ref, int ig3ref, int ig4ref);
/* rec_util/gmp/c_llfgr.c */
void gmp_llfgr(float *lat, float *lon, float x, float y, float latOrigine, float lonOrigine, float deltaLat, float deltaLon);
/* rec_util/gmp/clip.c */
int clip(float x1, float y1, float x2, float y2);
/* rec_util/gmp/gmp_gvlatlon.c */
void gmp_gvlatlon(float *vlatmin, float *vlonmin, float *vlatmax, float *vlonmax, int coord);
/* rec_util/gmp/gmp_perim.c */
int gmp_perim(float *xgdmin, float *ygdmin, float *xgdmax, float *ygdmax, float *latMin, float *lonMin, float *latMax, float *lonMax, int *nbSeg);
/* rec_util/gmp/gmp_verif.c */
int VerifierSegmentLatlon(float lonMin, float lonMax);
/* rec_util/gmp/lire_gdb_geo.c */
void get_cityname(int gr_type, float lat, float lon, char *name);
void c_gmpDrawCityName(float x, float y, char *text);
void get_coastline(int gr_type, int n, float lat0, float lon0, float lat1, float lon1, float *ll);
void lire_gdb_geo(void);
/* rec_util/gmp/lire_rmn_geo.c */
void lire_rmn_geo(void);
/* rec_util/gmp/lire_rmn_latlon.c */
void lire_rmn_latlon(void);
/* rec_util/select/strutil.c */
int strfind(char *SousChaine, char *Chaine);
/* rec_util/select/xrecsel.c */
void AfficherNbSelect(int32_t nb);
int32_t AjouterFiltre(int32_t indDes, int32_t indCle);
int32_t EnleverFiltre(int32_t indDes, int32_t indCle);
int32_t LibererPointeurs(void);
int LibererPanneauListe(void);
int32_t InitTitresMenus(char *idents[], int32_t nbDes);
int MessageChargement(int32_t nbRecs1, int32_t nbRecs2);
int32_t NettoyerString(char str[]);
int32_t UpdateFiltres(void);
int32_t XSelectstdActiver(int32_t sel[], int32_t *nbsel, int32_t *indSelecteur);
int32_t XSelectstdFermer(int32_t sel[], int32_t *nbsel, char **idents, int32_t table[][3], int32_t *m, int32_t *n, int32_t *indSelecteur);
int CalculerLargeurMenus(int32_t largeurMenus[], int32_t table[][3]);
int f77name(xseloup)(char *titre, int32_t *nbrecs, char idents[], int32_t *nbdes, int32_t *indSel, int32_t *typeSel, F2Cl flenNomFich, F2Cl flenIdents);
int f77name(xselins)(char *tableau, int32_t table[][3], int32_t *nbrecs, F2Cl lentableau);
int f77name(xselouf)(int32_t table[][3], int32_t *nbrecs);
int32_t f77name(xselact)(int32_t sel[], int32_t *nbsel, int32_t *indsel);
int32_t f77name(xselfer)(int32_t sel[], int32_t *nbsel, char idents[], int32_t table[][3], int32_t *m, int32_t *n, int32_t *indsel, F2Cl flen);
int32_t f77name(xselupd)(void);
int XSelectstdCommencerInit(char *titre, int32_t nbrecs, char **idents, int32_t nbdes, int32_t indSel, int32_t typeSel);
void XSelectstdInserer(char *tableau, int32_t table[][3], int32_t nbrecs);
void XSelectstdTerminerInit(int32_t table[][3], int32_t nbrecs);
int f77name(xseldim)(void);
int32_t f77name(xselundim)(void);
int f77name(xselup)(int32_t *indSelecteur);
int c_xselup(int32_t indSelecteur);
int f77name(xseldown)(int32_t *indSelecteur);
int c_xseldown(int32_t indSelecteur);
int f77name(xselopt)(int32_t *indSelecteur, char option[], char valeur[], F2Cl flenOption, F2Cl flenValeur);
int c_xselopt(int32_t indSelecteur, char *option, char *valeur);
/* rec_util/selfic/edit.c */
void XEditActiver(void);
void XEditOuvrir(void);
/* rec_util/selfic/outil.c */
void freelist(char ***list);
int isdir(char *path);
int isfile(char *path);
int lsdir(char *path, char *reject, char *select, char ***list);
void pathinfo(char *path, int *nword, char ***words, int *nfile, char ***list);
int scan_dir(char *dir, char ***list, int (*select)(const void *), int (*compar)(const void *,const void *));
int strsplit(char *str, char *delim, char ***list);
/* rec_util/selfic/repertoire.c */
void XRepertoireActiver(void);
void XRepertoireOuvrir(void);
/* rec_util/selfic/select.c */
void XSelectChangerRepertoire(char *path);
void c_selfic(char *liste, int maximum, int longueur, int *nombre);
void f77name(selfic)(char *liste, int *maximum, int *longueur, int *nombre, F2Cl len);
/* rec_util/xdash/unsat-xdash.c */
void f77name(xfrstpt)(int *x, int *y);
void f77name(xvector)(float *x, float *y);
void f77name(xgetsi)(int *i, int *j);
void f77name(xpolygon)(void);
void f77name(xframe)(void);
void f77name(xset)(int *ixmin, int *ixmax, int *iymin, int *iymax, float *xmin, float *xmax, float *ymin, float *ymax, int *ltype);
void f77name(xqqqa16)(void);
void f77name(xwindow)(float *x1, float *y1, float *x2, float *y2);
void f77name(xoptn)(int *optn, int *val);
/* rec_util/xplot88/init.c */
void f77name(initplot88)(void);
void init_plot88(void);
/* rec_util/xplot88/xplot88.c */
void f77name(xbndlset)(char *xoptn, int *iopval, int *n, int *nbdl, F2Cl flen);
void f77name(xbundle)(int *nbdl);
void f77name(xfl2int)(int *x, int *y, int *i, int *j);
void f77name(xgetset)(int *ixmin, int *ixmax, int *iymin, int *iymax, float *x1, float *x2, float *y1, float *y2, int *ltype);
int f77name(xmaptrn)(float *lat, float *lon, float *u, float *v);
void f77name(xmxmy)(int *x, int *y);
void f77name(xop1get)(char *xoptn, int *iopval, F2Cl flen);
void f77name(xop1set)(char *xoptn, int *iopval, F2Cl flen);
void f77name(xopnget)(char *xoptn, int *iopval, int *n, F2Cl flen);
void f77name(xopnset)(char *xoptn, int *iopval, int *n, F2Cl flen);
void f77name(xplotit)(int *i, int *j, int *flag);
void f77name(xplotit2)(int *x, int *y, int *down);
int f77name(xpwrs)(char *chr, int *nchr, int *icoded, F2Cl flen);

void f77name(opllfl)(int *unite, char *nomfich, int *code, int);
void f77name(rdllfl)(int *unite, int *npts, float *maxlat, float *minlat, float *maxlon, float *minlon, float *pts, int *nptsmx);
void f77name(clllfl)(int *unite);

#endif
