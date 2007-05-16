#include <stdio.h>
#include <math.h>
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <lic.h>
#include <rpnmacros.h>
#include <wgl_x.h>

#define C_TO_FTN(i,j,ni)  (int)((ni) * (j) + i)

extern _Viewport    viewp;
extern _XContour xc;

void AfficherLIC(float *uu, float *vv, int ni, int nj, int couleur, int linewidth, float ventmax)
{
  int i,j,k,n;
  int idebut, jdebut, ifin, jfin, largeur, hauteur;
  unsigned int *pixels;
  char *pixdata;
  float *axex, *axey, *axdx, *uvs, *luu, *lvv;
  int *iaxex;
  int ioffset, joffset;
  
  int ix, iy, iy1, iy2;
  float x,y,val;
  float dx, dy;
  float x1, y1, x2, y2; 
  float fld1, fld2, fld3;
   
  int local_ni, local_nj;
  int largeurFenetre, hauteurFenetre;
  int lastx, lastCol, col;
  float lasty, delta;
  int indCouleur;
  int mdeb, ndeb, mfin, nfin, entier, r;
  float fraction;
  float uu1,uu2,vv1,vv2;
   
  unsigned int pixel;
  float rfac, eps;
  float *tmpVals;
  float *tmpInds;
  float colrange;

  int ii, jj, lh, slice, curslice;
  LIC *lic;
   
  XImage *image;
   
  unsigned char *Noise, *Lic;

  c_wglgwz(&largeurFenetre,&hauteurFenetre);
   
  DefinirFenetreGrille(&mdeb, &ndeb, &mfin, &nfin, ni, nj);
  c_xy2fxfy(&x, &y, (float)mdeb, (float)ndeb);
  c_wglxai(&idebut, &jdebut, x, y);
   
  c_xy2fxfy(&x, &y, (float)mfin, (float)nfin);
  c_wglxai(&ifin, &jfin, x, y);
   
  if (idebut < viewp.vi1)
    idebut = viewp.vi1;
   
  if (jdebut < viewp.vj1)
    jdebut = viewp.vj1;
   
  if (ifin > viewp.vi2)
    ifin = viewp.vi2;
   
  if (jfin > viewp.vj2)
    jfin = viewp.vj2;
   
  largeur = ifin - idebut + 1;
  hauteur = jfin - jdebut + 1;
  
  axex    = (float *) calloc(largeur, sizeof(float));
  axey    = (float *) calloc(hauteur, sizeof(float));
  axdx    = (float *) calloc(largeur, sizeof(float));
  iaxex    = (int *) calloc(largeur, sizeof(int));
  tmpVals = (float *) calloc(largeur*hauteur, sizeof(float));
  tmpInds = (int *) calloc(largeur*hauteur, sizeof(int));
  pixels =  (unsigned int *)malloc(largeur*hauteur*sizeof(unsigned int));
  pixdata = (char *) malloc(4*largeur*hauteur*sizeof(char));
  luu = (float *) malloc(largeur*hauteur*sizeof(float));
  lvv = (float *) malloc(largeur*hauteur*sizeof(float));
  uvs = (float *) malloc(2*largeur*hauteur*sizeof(float));
  
  pixel    = LIC_ConfiguredPixelSize();
  
  Noise = (unsigned char *)malloc(largeur*hauteur*pixel);
  Lic   = (unsigned char *)malloc(largeur*hauteur*pixel);

  image = XCreateImage(wglDisp, visInfo.visual, visInfo.depth, ZPixmap, 0, (char *)pixdata,largeur, hauteur, 32, 0);
  
  srand(98639);	/* Seed with a prime number */
  
  for (i = 0; i < largeur*hauteur; i++)
    {
    Noise[i] =  rand() & 255;
    Noise[i] = Noise[i] < 64 ? 64 : Noise[i];
    }
  
  /* ------------------------------------------------------------------  */
  
  for (i=idebut; i <= ifin; i++)
    {
    ii = i - idebut;
    c_wgliax(&x, &y, i, 0);
    c_fxfy2xy(&x, &y, x, y);
    ix = (int) x;
    iaxex[ii] = (ix < 1) ? 1 : (ix > ni-1) ? ni-1 : ix;
    axdx[ii] = x - (float)ix;
    }
   
  for (j=jdebut; j <= jfin; j++)
    {
    c_wgliax(&x, &y, 0, j);
    c_fxfy2xy(&x, &y, x, y);
    axey[j - jdebut] = y;
    }
   

  for (j=0; j < hauteur; j++)
    {
    y = axey[hauteur-1-j];
    iy = (int) y;
    iy = (iy < 1) ? 1 : (iy > nj-1) ? nj-1 : iy;
    iy1 = ni * (iy - 1);
    iy2 = ni * (iy);
    dy = y - (float)iy;
    for (i=idebut; i <= ifin; i++)
      {
      ii = i - idebut;
      ix = iaxex[ii];
      uu1 = uu[ix-1+iy1] + (uu[ix+iy1] - uu[ix-1+iy1]) * axdx[ii];
      uu2 = uu[ix-1+iy2] + (uu[ix+iy2] - uu[ix-1+iy2]) * axdx[ii];
      vv1 = vv[ix-1+iy1] + (vv[ix+iy1] - vv[ix-1+iy1]) * axdx[ii];
      vv2 = vv[ix-1+iy2] + (vv[ix+iy2] - vv[ix-1+iy2]) * axdx[ii];
      luu[j*largeur+ii] =  uu1 + (uu2 - uu1) * dy;
      lvv[j*largeur+ii] =  vv1 + (vv2 - vv1) * dy;
      }
    }
  
  for (i=0; i < largeur*hauteur; i++)
    {
    uvs[2*i]  = luu[i];
    uvs[2*i+1] = lvv[i];
    }
  
  /* -------------------------------------------------------------------------------- */
  
  
  
  lic = LIC_Create((unsigned char *)Noise, largeur, hauteur, 1,
		   (float *)        uvs, largeur, hauteur, 1,
		   (unsigned char *)Lic,
		   LIC_Box, LIC_VARIABLE, FALSE,
		   10.0,3.0, FALSE, FALSE,-1,-1,-1,-1,FALSE, FALSE);
  if (lic == (LIC *)NULL)
    {
    (void)fprintf(stderr, "LIC_Create returned NULL!\n");
    return;
    }
  LIC_ChangePhase(lic, 0.0);
  LIC_ChangeNumThreads(lic, 1);
  
  /*
   * Build integral tables here so the build isn't computed as part of the
   * time to compute the image.
   */
  LIC_BuildIntegralTables(lic);
  
  LIC_ComputeImage(lic);
  /* -------------------------------------------------------------------------------- */
  
  for (i=0; i < hauteur*largeur; i++)
    {
    tmpVals[i] = 1.0*Lic[i];
    }

  for (j=0; j < hauteur; j++)
    {
    jj = j * largeur;
    
    c_wglcolfs_fst(&(tmpVals[jj]), &(pixels[jj]), largeur);
    for (i=0; i < largeur; i++)
      {
      XPutPixel(image, i, j, pixels[jj+i]);
      }
    }
  
  XPutImage(wglDisp, wglDrawable, wglFillGC, image, 0, 0, idebut, hauteurFenetre - jfin, largeur, hauteur);
  
  LIC_Destroy(lic);
  XDestroyImage(image);
  free(axex);
  free(axey);
  free(axdx);
  free(iaxex);
  free(tmpVals);
  free(tmpInds);
  free(pixels);
  free(luu);
  free(lvv);
  free(uvs);
  free(Noise);
  free(Lic);


    
}
