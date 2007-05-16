#include <string.h>
#include <fcntl.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <xinit.h>
#include <wgl.h>
#include <rpnmacros.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

#ifdef Darwin_OSX_PPC
#include <limits.h>
#define HZ CLK_TCK
#endif

extern SuperWidgetStruct SuperWidget;
extern Widget            wglTopLevel;
extern _XContour xc;
extern _AnimInfo animInfo;
extern GeoMapInfoStruct mapInfo;


extern Display *wglDisp;
extern int  wglScrNum;
extern Window  wglWin, wglDrawable;
extern GC wglLineGC;
extern int interpolationTemporelle;
extern _Viewport viewp;

extern void EffacerFenetreAffichage();

void c_dors(float delai)
{
   static clock_t t1,t2;
   static struct tms temps1, temps2;

   t1 = times(&temps1);
   t2 = times(&temps1) - t1;
   while (delai > t2/(float) HZ)
      {
      t2 = times(&temps1) - t1;
      }
  }


void c_animvent(float *uu,float *vv,float *mapscl,int ni,int nj, int seglen, float *uut,float *vvt,int npts,int nnpas,int deet,int segstep,float delai)
  {
  int ig1, ig2, ig3, ig4;
  float rn;
  char grtyp[2];
  float xg1, xg2, xg3, xg4;
  int ier, i,j,k,l,m,icount,limite;
  int iseed;
  double l_delai;
  int nbFlds;
  _Champ *champ;

  int ilim, npoints, n;
  int ix, iy;

  int ifac,jfac,iframe,gdid;
  int ipos,jpos,induv;
  float uuu,vvv;
  float umax, vmax;
  int ezqkdef, gdxysval, width, height;
  float xdebut, xfin, ydebut, yfin, deltax, deltay;

  float *xx, *yy;

  nbFlds =  FldMgrGetNbChampsActifs();
  i = 0;
  FldMgrGetChamp(&champ, 0);
  while (i < nbFlds && champ->natureTensorielle != VECTEUR)
         {
         i++;
         }
  induv = i;
  
  xx = (float *)malloc(sizeof(float)*npts);
  yy = (float *)malloc(sizeof(float)*npts);

  c_wglcol(BLANC);
  c_wglclr();

  icount=0;
  iframe = 0;
  limite = nnpas;
  npoints = npts;
  l = 0;
  l_delai = (double)delai;

//   c_wglssp(1.0, 1.0, 1.0*ni, 1.0*nj, 0, 0, 0, 0, 1);

  AjusterViewport(&viewp);
  c_wgliax(&xdebut, &ydebut, viewp.vi1, viewp.vj1);
  c_wgliax(&xfin, &yfin, viewp.vi2, viewp.vj2);
/*  c_wglgvx(&xdebut, &ydebut, &xfin, &yfin);*/
  c_wglcmx(xdebut, ydebut, xfin, yfin);
  if (xdebut < 1.0)  xdebut = 1.0;
  if (ydebut < 1.0)  ydebut = 1.0;
  if (xfin > 1.0*ni) xfin = 1.0*ni;
  if (yfin > 1.0*nj) yfin = 1.0*nj;
  
  deltax = xfin - xdebut;
  deltay = yfin - ydebut;


  c_wgldbf();
  c_wglbbf();


  srand((int)(uu[0]));
  for (m=0; m < npts; m++)
    {
    uut[m] = xdebut + 1.0 * deltax * (rand()/(RAND_MAX+1.0));
    vvt[m] = ydebut + 1.0 * deltay * (rand()/(RAND_MAX+1.0));
    }

  gdid = c_ezgetgdout();

  for (k=1; k < limite; k++)
    {
    ier = c_gdxysval(gdid, xx, uu, &(uut[FTN2C(0,k-1,npts)]),&(vvt[FTN2C(0,k-1,npts)]), npts);
    ier = c_gdxysval(gdid, yy, vv, &(uut[FTN2C(0,k-1,npts)]),&(vvt[FTN2C(0,k-1,npts)]), npts);
    for (m=0; m < npts; m++)
      {
      uut[FTN2C(m,k,npts)] = uut[FTN2C(m,k-1,npts)] + xx[m] * deet/3600.0;
      vvt[FTN2C(m,k,npts)] = vvt[FTN2C(m,k-1,npts)] + yy[m] * deet/3600.0;

      if (uut[FTN2C(m,k,npts)] < xdebut  || uut[FTN2C(m,k,npts)] > xfin || vvt[FTN2C(m,k,npts)] < ydebut || vvt[FTN2C(m,k,npts)] > yfin)
        {
        uut[FTN2C(m,k,npts)] = xdebut + 1.0 * deltax * (rand()/(RAND_MAX+1.0));
        vvt[FTN2C(m,k,npts)] = ydebut + 1.0 * deltay * (rand()/(RAND_MAX+1.0));
        }
      }
    }


    l = 0;
    iframe = 0;
    c_wglallocf(segstep);
    for (l=0; l < segstep; l++)
      {
/*       c_wglcol(BLANC);
       c_wglclr();
       c_wglcol(GRIS);
       c_gmpdrw();*/
       for (i=0; i < FldMgrGetNbChampsActifs(); i++)
         {
         AfficherCarte(i);
         }
       c_wglcol(xc.attributs[induv].indCouleurFore);
       for (k=0; k < limite-seglen+1; k+=segstep)
         {
         for (i=0; i <= seglen - 1; i++)
           {
           n = (k+i+l) < (limite-1) ? (k+i+l) : (limite-1);
 /*         fprintf(stderr, "%d\n", n);*/
           for (m=0; m < npts; m++)
             {
             xx[m] = uut[FTN2C(m,n,npts)];
             yy[m] = vvt[FTN2C(m,n,npts)];
/*             c_wglptx(uut[FTN2C(m,n,npts)], vvt[FTN2C(m,n,npts)]);*/
             }
           c_wglptxs(&(uut[n*npoints]), &(vvt[n*npoints]), &npoints);
           }
         }
     AfficherLegende(0);
     AfficherPerimetreFenetre();
     c_wglswb();
     c_wglcopyf(l);
     }


   l = 0;
   while (!c_wglanul())
     {
     c_wglfbf();
     c_wglpastef(l);
     l = (l+1) % segstep;
     f77name(micro_sleep)(&l_delai);
     }

  c_wgldeallocf(segstep);
  free(xx);
  free(yy);
  }

/* -------------------------------------------------------------------------------------------------- */
Pixmap pixmaps[1024];
int flagsImagesChargees[1024];

/* -------------------------------------------------------------------------------------------------- */

c_wgldeallocf(int nImages)
{
   int i, j;

   for (i=0; i < nImages; i++)
    {
    XFreePixmap(wglDisp, pixmaps[i]);
    }
   return 0;
   }
/* -------------------------------------------------------------------------------------------------- */

c_wglallocf(int nimages)
{
   int i, j;
   int x, y;
   Window root;
   unsigned int width, height, border_width, depth, nplanes;
   int largeurFenetre, hauteurFenetre;
   int status, nouvelleLargeur, nouvelleHauteur;
   int wglWin;
   static int nbImages;

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   nbImages = nimages;
   nplanes = c_wglgpl();
   for (i=0; i < nbImages; i++)
    {
    pixmaps[i] = XCreatePixmap(wglDisp, RootWindow(wglDisp, wglScrNum), largeurFenetre, hauteurFenetre, nplanes);
    status = XGetGeometry(wglDisp, pixmaps[i], &root, &x, &y, &width, &height, &border_width, &depth);
    if (status == 0)
       {
       exit(13);
       }
   }
   return 0;
   }

/* -------------------------------------------------------------------------------------------------- */

void c_wglcopyf(int ind)
{
   int largeurFenetre, hauteurFenetre;

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   XCopyArea(wglDisp, wglWin, pixmaps[ind], wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
}

/* -------------------------------------------------------------------------------------------------- */

void c_wglpastef(int ind)
{
   int largeurFenetre, hauteurFenetre;

   c_wglgwz(&largeurFenetre, &hauteurFenetre);
   XCopyArea(wglDisp, pixmaps[ind], wglWin, wglLineGC, 0, 0, largeurFenetre, hauteurFenetre, 0, 0);
}

/* -------------------------------------------------------------------------------------------------- */


