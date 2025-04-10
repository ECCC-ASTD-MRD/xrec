#include <rmn/rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include "rmn/ezscint.h"

int c_rec_ezsint_mask(unsigned int *maskout, unsigned int *maskin, int gdin, int gdout)
  {
  char grtyp_in[2], grtyp_out[2];
  int ni_gdin, ni_gdout, nj_gdin, nj_gdout;
  int ig1_gdin, ig2_gdin, ig3_gdin, ig4_gdin, ig1_gdout, ig2_gdout, ig3_gdout, ig4_gdout;
  int i, j, k, ier,npts_in, npts_out;
  unsigned int bitpos;
  unsigned int *imask_in, *imask_out;
  float *fmask_in, *fmask_out;

  
  c_ezdefset(gdout, gdin);
  ier = c_ezgprm(gdin, grtyp_in, &ni_gdin, &nj_gdin, &ig1_gdin, &ig2_gdin, &ig3_gdin, &ig4_gdin);
  ier = c_ezgprm(gdout, grtyp_out, &ni_gdout, &nj_gdout, &ig1_gdout, &ig2_gdout, &ig3_gdout, &ig4_gdout);

  fmask_in = (float *)malloc(ni_gdin*nj_gdin*sizeof(float));
  
  fmask_out = (float *)malloc(ni_gdout*nj_gdout*sizeof(float));
  imask_out = (unsigned int *)  malloc(ni_gdout*nj_gdout*sizeof(int));

  npts_in  = ni_gdin*nj_gdin;
  npts_out = ni_gdout*nj_gdout;

/*   ier = uncompact_mask(imask_in, maskin, npts_in); */

  for (i=0; i < npts_in; i++)
    {
    fmask_in[i] = (float) (GETMSK(maskin, i));
    }

  c_ezsint(fmask_out, fmask_in);

  for (i=0; i < npts_out; i++)
    {
    if (fmask_out[i] == 1.0)
      imask_out[i] = 1;
    else
      imask_out[i] = 0;
    }

  ier = compact_mask(maskout, imask_out, npts_out);
  free(fmask_in);
  free(fmask_out);
  free(imask_out);
  }

int compact_mask(unsigned int *dest, int *src, int npts)
  {
  int i,entier, fraction,npts32;

  npts32 = 1 + (npts >> 5);
/*   memset((unsigned int *)dest, (int)NULL, sizeof(unsigned int)*npts32); */

  for (i=0; i < npts32; i++)
    {
    dest[i] = 0;
    }
  
  for (i=0; i < npts; i++)
    {
    entier = i >> 5;
    fraction = i - (entier << 5);
    dest[entier] |= (src[i] << fraction);
/*    dest[entier] = SETMSK(src,i);*/
    }
  }

int uncompact_mask(int *dest, unsigned int *src, int npts)
  {
  int i,entier, fraction;

  for (i=0; i < npts; i++)
    {
    entier = i >> 5;
    fraction = i - (entier << 5);
    dest[i] = (src[entier]  & (1 << fraction)) >> fraction;
    }
  }
