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

#include <stdio.h>
#include <math.h>

static float cv;

#define min(a,b)           (a < b ? a : b)
#define max(a,b)           (a > b ? a : b)
#define c(p1,p2)           (p1-cv) / (p1-p2)
#define zlin(zz1,zz2,zdx)  ((zz2 - zz1) * zdx + zz1)
#define FTN2C(i,j,ni)      (int)((ni) * (j) + (i))
#define between(a,b,c)     (min(a,c) < b) && (max(a,c) >= b)
#define  ROUND(x)               (int)(x + 0.5)


static  unsigned int  *mh, *mv;
/* s/p  wgldrl    trace des contours */

int     ioffp;
float        spval;
static int     ixa, iya, ixb, iyb, idir; //inx[8], iny[8], ir, irbase;
static int up=0; 
static int left=1; 
static int down=2;
static int right=3;

static void c_wgldrl(float *z, int ni,int nj, int mdeb,int ndeb,int mfin,int nfin, int smoothFactor);
void c_fxfy2xy(float *fx, float *fy, float x, float y);
void c_xy2fxfy(float *fx, float *fy, float x, float y);

void c_wglstl(float *z, int ni,int nj, int mdeb,int ndeb,int mfin,int nfin, float contourValue, int smoothFactor)
{
  int   i, j, k, btn; 
  int   smoothFactor2;
  int length_mask;
  int bitpos;

  length_mask = 1 + ((ni*nj) >> 5);
  smoothFactor2 = smoothFactor;
  mh =(unsigned int *) calloc(length_mask,sizeof(unsigned int));
  mv =(unsigned int *) calloc(length_mask,sizeof(unsigned int));
  
  cv = contourValue;
  
  for (i=mdeb; i < mfin; i++)
    {
    k = FTN2C(i,1,ni);
    btn = between(z[k-1],cv,z[k]);
    if(btn)
      {
      ixa = i - 1;
      ixb = i;
      iya = 1;
      iyb = 1;
      idir = up;
      c_wgldrl(z,ni,nj,mdeb,ndeb,mfin,nfin,smoothFactor2);
      }
    
    k = FTN2C(i,nj,ni);
    btn = between(z[k],cv,z[k-1]);
    if(btn)
      {
      ixa = i;
      ixb = i - 1;
      iya = nj;
      iyb = nj;
      idir = down;
      c_wgldrl(z,ni,nj,mdeb,ndeb,mfin,nfin,smoothFactor2);
      }
    }
  
  
  for (j=ndeb; j <nfin; j++)
    {
    k = FTN2C(ni, j-1, ni);
    btn = between(z[k],cv,z[k+ni]);
    if(btn)
      {
      ixa = ni;
      ixb = ni;
      iya = j - 1;
      iyb = j;
      idir = left;
      c_wgldrl(z,ni,nj,mdeb,ndeb,mfin,nfin,smoothFactor2);
      }
    
    k = FTN2C(1, j, ni);
    btn = between(z[k],cv,z[k-ni]);
    if(btn)
      {
      ixa = 1;
      ixb = 1;
      iya = j;
      iyb = j - 1;
      idir = right;
      c_wgldrl(z,ni,nj,mdeb,ndeb,mfin,nfin,smoothFactor2);
      }
    }
  
  for (j=ndeb;j < nfin-1; j++)
    {
    for (i=mdeb;i < mfin-1; i++)
      {
      k = FTN2C(i, j, ni);
      bitpos = k - ((k >> 5) << 5);
      btn = between(z[k],cv,z[k+1]);
      if(btn)
        {
        if(!((mh[k >> 5] << (31 - bitpos)) >> 31))
          {
          ixa = i;
          iya = j;
          ixb = i+1;
          iyb = j;
          idir = up;
          c_wgldrl(z,ni,nj,mdeb,ndeb,mfin,nfin,smoothFactor2);
          }
        }
      }
    }
  free(mh);
  free(mv);
}

void c_wgldrl(float *z, int ni,int nj, int mdeb,int ndeb,int mfin,int nfin, int smoothFactor)
{
  float       fx, fy, pass, za, zb, pds1, x, y, zc, zd;
  int    ikod, ikod0, nobit, bitpos,ixa0, ixb0, iya0, iyb0, ixc, ixd, iyc, iyd, idir0, rxa0, rxb0, ryb0, rya0;
  unsigned int   btac, btbd, btcd;
  float pts[128][2];
  int    k, npts;
  
  float rx,ry;
  int ix, iy;
  float rmdeb, rndeb, rmfin, rnfin;
  float rxa, rxb, rxc, rxd;
  float rya, ryb, ryc, ryd;
  float epsilon, rinc; 
  int inxx[4] = { 0, -1,  0, 1 };
  int inyy[4] = { 1,  0, -1, 0 };
  
  npts = 0;
  pass = 1;
  ix = min(ixa, ixb);
  iy = min(iya, iyb);
  nobit = FTN2C(ix, iy, ni);
  bitpos = nobit - ((nobit >> 5) << 5);
  ikod0 = nobit;
  ikod = -1;
					  
  if (iya == iyb)
    {
    if ((mh[nobit >> 5] << (31 - bitpos)) >> 31) return;
    mh[nobit >> 5] |= (1 << bitpos);
    }
  else
    {
    if ((mv[nobit >> 5] << (31 - bitpos)) >> 31) return;
    mv[nobit >> 5] |= (1 << bitpos);
    }

  idir0 = idir;
  ixa0 = ixa;
  ixb0 = ixb;
  iya0 = iya;
  iyb0 = iyb;
  
  rxa = (float)(ixa);
  rya = (float)(iya);
  rxb = (float)(ixb);
  ryb = (float)(iyb);
  
  rxa0 = rxa;
  rya0 = rya;
  rxb0 = rxb;
  ryb0 = ryb;
  
  dix:   
  za = z[FTN2C(ixa, iya, ni)];
  zb = z[FTN2C(ixb, iyb, ni)];
  
  pds1 = c(za,zb);
  x = rxa + pds1*(rxb-rxa);
  y = rya + pds1*(ryb-rya);
  
  c_xy2fxfy(&fx, &fy, x+1, y+1);
  c_wglplx(npts, pts);
  npts = 1;
  pts[0][0]  = fx;
  pts[0][1]  = fy;
  cinq: 
  ixc = ixa + inxx[idir];
  ixd = ixb + inxx[idir];
  iyc = iya + inyy[idir];
  iyd = iyb + inyy[idir];
  
  if(ixc < mdeb || ixd < mdeb || iyc < ndeb || iyd < ndeb || ixc > mfin || ixd > mfin || iyc > nfin || iyd > nfin)
    {
    ix = min(ixa, ixb);
    iy = min(iya, iyb);
    nobit = FTN2C(ix, iy, ni);
    bitpos = nobit - ((nobit >> 5) << 5);
    if(iya==iyb) 
      {
      mh[nobit >> 5] |= (1 << bitpos);
      }
    else 
      {
      mv[nobit >> 5] |= (1 << bitpos);
      }
    goto soixantesix;
    }
  
  
  zc = z[FTN2C(ixc, iyc,ni)];
  zd = z[FTN2C(ixd, iyd,ni)];
  
  btac = between(za,cv,zc);
  btbd = between(zb,cv,zd);
  btcd = between(zc,cv,zd);
  
  if(btac && btbd && btcd) goto quarantequatre;
  if(btac) goto onze;
  if(btbd) goto trentetrois;
  if(btcd) goto vingtdeux;
  goto soixantesix;
  
 onze:	  
  ixb = ixc;
  iyb = iyc;
  idir = (idir+1) & 3;
  zb = zc;
  goto cinquantecinq;
  
 vingtdeux:	 
  ixa = ixc;
  iya = iyc;
  ixb = ixd;
  iyb = iyd;
  za = zc;
  zb = zd;
  goto cinquantecinq;

  trentetrois:  
  ixa = ixd;
  iya = iyd;
  idir = (idir+3) & 3;
  za = zd;
  goto cinquantecinq;
  
 quarantequatre:
  if (c(zc,zd) > pds1) 
    {
    goto onze;
    }
  else if (c(zc,zd) < pds1) 
    {
    goto trentetrois;
    }
  else
    {
    goto vingtdeux;
    }
  
 cinquantecinq:
  pds1 = c(za,zb);
  rxa = (float)(ixa);
  rya = (float)(iya);
  rxb = (float)(ixb);
  ryb = (float)(iyb);
  
  x = rxa + pds1*(rxb-rxa);
  y = rya + pds1*(ryb-rya);
  
  c_xy2fxfy(&fx, &fy, x+1, y+1);
  pts[npts][0] = fx;
  pts[npts][1] = fy;
  npts++;
  
  if (npts == 128) 
    {
    c_wglplx(npts, pts);
    pts[0][0] = pts[npts-1][0];
    pts[0][1] = pts[npts-1][1];
    npts = 1;
    }
  
  
  ix = min(ixa, ixb);
  iy = min(iya, iyb);
  nobit = FTN2C(ix, iy, ni);
  bitpos = nobit - ((nobit >> 5) << 5);
  
  if (iya==iyb) 
    {
    if ((mh[nobit >> 5] << (31 - bitpos)) >> 31) ikod = ikod0;
    mh[nobit >> 5] |= (1 << bitpos);
    }
  else
    {
    if ((mv[nobit >> 5] << (31 - bitpos)) >> 31) ikod = ikod0;
    mv[nobit >> 5] |= (1 << bitpos);
    }
  
  if (ikod != ikod0) goto cinq;
  
  pass= 2;
  
 soixantesix:
  if(pass == 1)
    {
    pass = 2;
    ixa = ixb0;
    ixb = ixa0;
    iya = iyb0;
    iyb = iya0;
    idir = 3 & (2+idir0);
    goto dix;
    }
  c_wglplx(npts, pts);
  
}

