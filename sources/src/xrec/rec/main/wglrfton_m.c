#include <rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <rpnmacros.h>
#include <wgl.h>

void c_wglrfton_m(float *fld, unsigned int *mask, int ni, int nj,
      float intervalles[], int nbint, float rfac, float rmin, float rmax, float missing, int colorTable[], int nbcol)
{
  int *indices;
  float *vals;
  int i,j, k;
  float cmin,cmax;
  float delta;
  int is_present, ind, bitpos;
  int largeurFenetre, hauteurFenetre;
  float x1, y1, x2, y2, rx1, ry1, rx2, ry2;
  int mdeb, mfin, ndeb, nfin;

  mdeb = 0;
  mfin = ni - 1;
  ndeb = 0;
  nfin = nj - 1;

  c_wglgwz(&largeurFenetre, &hauteurFenetre);
  c_wglcmi(0, 0, largeurFenetre, hauteurFenetre);
  if (rmin==rmax)
    {
    c_wglcol(15);
    c_wglclr();
    return;
    }

  cmin = rmin;
  cmax = rmax;

  if (intervalles[0] != 0.0)
    {
    f77name(ajusminmax)(&cmin,&cmax,&rfac,&intervalles[0]);
    }

  delta = cmax - cmin;
  indices = malloc(ni * sizeof(int));
  vals = malloc(ni * sizeof(float));

  for  (j=nfin; j >= ndeb; j--)
    {
    for (i=mdeb; i <= mfin; i++)
      {
      vals[i] = fld[FTN2C(i, j, ni)];
      }

    c_wglcalcols(indices, vals, ni, cmin, delta, intervalles, nbint, rfac, nbcol);
    for (i=mdeb; i < mfin; i++)
      {
      k = FTN2C(i, j, ni);
/*      bitpos = k - ((k >> 5) << 5);
      is_present = (missing[k >> 5]  & (1 << bitpos)) >> bitpos;*/
      is_present = GETMSK(mask,k);
      if (is_present)
        {
        x1 = 1 + i - 0.5;
        x2 = 1 + i + 0.5;
        y1 = 1 + j - 0.5;
        y2 = 1 + j + 0.5;
        /*  fprintf(stderr, "%d %d %d %d %d \n", i, j, k, bitpos, is_present); */
        c_wglcol(colorTable[indices[i]]);
        c_xy2fxfy(&rx1,&ry1,x1,y1);
        c_xy2fxfy(&rx2,&ry2,x2,y2);
        c_wglrfx(rx1,ry1,rx2,ry2);
        /* fprintf(stderr, "%f %f %f %f - %f %f %f %f\n",x1,y1,x2,y2,rx1,ry1,rx2,ry2); */
        }
      }
    }
  free(vals);
  free(indices);
  return;

}

