#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include "gd.h"
#include "gdhelpers.h"

#ifdef _OSD_POSIX		/* BS2000 uses the EBCDIC char set instead of ASCII */
#define CHARSET_EBCDIC
#define __attribute__(any)	/*nothing */
#endif
/*_OSD_POSIX*/

#ifndef CHARSET_EBCDIC
#define ASC(ch)  ch
#else /*CHARSET_EBCDIC */
#define ASC(ch) gd_toascii[(unsigned char)ch]
static const unsigned char gd_toascii[256] =
{
/*00 */ 0x00, 0x01, 0x02, 0x03, 0x85, 0x09, 0x86, 0x7f,
  0x87, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,	/*................ */
/*10 */ 0x10, 0x11, 0x12, 0x13, 0x8f, 0x0a, 0x08, 0x97,
  0x18, 0x19, 0x9c, 0x9d, 0x1c, 0x1d, 0x1e, 0x1f,	/*................ */
/*20 */ 0x80, 0x81, 0x82, 0x83, 0x84, 0x92, 0x17, 0x1b,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,	/*................ */
/*30 */ 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
  0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,	/*................ */
/*40 */ 0x20, 0xa0, 0xe2, 0xe4, 0xe0, 0xe1, 0xe3, 0xe5,
  0xe7, 0xf1, 0x60, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,	/* .........`.<(+| */
/*50 */ 0x26, 0xe9, 0xea, 0xeb, 0xe8, 0xed, 0xee, 0xef,
  0xec, 0xdf, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x9f,	/*&.........!$*);. */
/*60 */ 0x2d, 0x2f, 0xc2, 0xc4, 0xc0, 0xc1, 0xc3, 0xc5,
  0xc7, 0xd1, 0x5e, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
/*-/........^,%_>?*/
/*70 */ 0xf8, 0xc9, 0xca, 0xcb, 0xc8, 0xcd, 0xce, 0xcf,
  0xcc, 0xa8, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,	/*..........:#@'=" */
/*80 */ 0xd8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0xab, 0xbb, 0xf0, 0xfd, 0xfe, 0xb1,	/*.abcdefghi...... */
/*90 */ 0xb0, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
  0x71, 0x72, 0xaa, 0xba, 0xe6, 0xb8, 0xc6, 0xa4,	/*.jklmnopqr...... */
/*a0 */ 0xb5, 0xaf, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
  0x79, 0x7a, 0xa1, 0xbf, 0xd0, 0xdd, 0xde, 0xae,	/*..stuvwxyz...... */
/*b0 */ 0xa2, 0xa3, 0xa5, 0xb7, 0xa9, 0xa7, 0xb6, 0xbc,
  0xbd, 0xbe, 0xac, 0x5b, 0x5c, 0x5d, 0xb4, 0xd7,	/*...........[\].. */
/*c0 */ 0xf9, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0xad, 0xf4, 0xf6, 0xf2, 0xf3, 0xf5,	/*.ABCDEFGHI...... */
/*d0 */ 0xa6, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
  0x51, 0x52, 0xb9, 0xfb, 0xfc, 0xdb, 0xfa, 0xff,	/*.JKLMNOPQR...... */
/*e0 */ 0xd9, 0xf7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5a, 0xb2, 0xd4, 0xd6, 0xd2, 0xd3, 0xd5,	/*..STUVWXYZ...... */
/*f0 */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0xb3, 0x7b, 0xdc, 0x7d, 0xda, 0x7e	/*0123456789.{.}.~ */
};
#endif /*CHARSET_EBCDIC */

extern int gdCosT[];
extern int gdSinT[];

static void gdImageBrushApply (gdImagePtr im, int x, int y);
static void gdImageTileApply (gdImagePtr im, int x, int y);
int gdImageGetTrueColorPixel (gdImagePtr im, int x, int y);

gdImagePtr
gdImageCreate (int sx, int sy)
{
  int i;
  gdImagePtr im;
  im = (gdImage *) gdMalloc (sizeof (gdImage));
  memset (im, 0, sizeof (gdImage));
  /* Row-major ever since gd 1.3 */
  im->pixels = (unsigned char **) gdMalloc (sizeof (unsigned char *) * sy);
  im->polyInts = 0;
  im->polyAllocated = 0;
  im->brush = 0;
  im->tile = 0;
  im->style = 0;
  for (i = 0; (i < sy); i++)
    {
      /* Row-major ever since gd 1.3 */
      im->pixels[i] = (unsigned char *) gdCalloc (
						sx, sizeof (unsigned char));
    }
  im->sx = sx;
  im->sy = sy;
  im->colorsTotal = 0;
  im->transparent = (-1);
  im->interlace = 0;
  im->thick = 1;
  for (i = 0; (i < gdMaxColors); i++)
    {
      im->open[i] = 1;
      im->red[i] = 0;
      im->green[i] = 0;
      im->blue[i] = 0;
    };
  im->trueColor = 0;
  im->tpixels = 0;
  return im;
}

gdImagePtr
gdImageCreateTrueColor (int sx, int sy)
{
  int i;
  gdImagePtr im;
  im = (gdImage *) gdMalloc (sizeof (gdImage));
  memset (im, 0, sizeof (gdImage));
  im->tpixels = (int **) gdMalloc (sizeof (int *) * sy);
  im->polyInts = 0;
  im->polyAllocated = 0;
  im->brush = 0;
  im->tile = 0;
  im->style = 0;
  for (i = 0; (i < sy); i++)
    {
      im->tpixels[i] = (int *) gdCalloc (
					  sx, sizeof (int));
    }
  im->sx = sx;
  im->sy = sy;
  im->transparent = (-1);
  im->interlace = 0;
  im->trueColor = 1;
  im->saveAlphaFlag = 1;
  im->alphaBlendingFlag = 0;
  im->thick = 1;
  return im;
}

void
gdImageDestroy (gdImagePtr im)
{
  int i;
  if (im->pixels)
    {
      for (i = 0; (i < im->sy); i++)
	{
	  gdFree (im->pixels[i]);
	}
      gdFree (im->pixels);
    }
  if (im->tpixels)
    {
      for (i = 0; (i < im->sy); i++)
	{
	  gdFree (im->tpixels[i]);
	}
      gdFree (im->tpixels);
    }
  if (im->polyInts)
    {
      gdFree (im->polyInts);
    }
  if (im->style)
    {
      gdFree (im->style);
    }
  gdFree (im);
}

int
gdImageColorClosest (gdImagePtr im, int r, int g, int b)
{
  return gdImageColorClosestAlpha (im, r, g, b, gdAlphaOpaque);
}

int
gdImageColorClosestAlpha (gdImagePtr im, int r, int g, int b, int a)
{
  int i;
  long rd, gd, bd, ad;
  int ct = (-1);
  int first = 1;
  long mindist = 0;
  if (im->trueColor)
    {
      return gdTrueColorAlpha (r, g, b, a);
    }
  for (i = 0; (i < (im->colorsTotal)); i++)
    {
      long dist;
      if (im->open[i])
	{
	  continue;
	}
      rd = (im->red[i] - r);
      gd = (im->green[i] - g);
      bd = (im->blue[i] - b);
      ad = (im->blue[i] - b);
      dist = rd * rd + gd * gd + bd * bd + ad * ad;
      if (first || (dist < mindist))
	{
	  mindist = dist;
	  ct = i;
	  first = 0;
	}
    }
  return ct;
}

/* This code is taken from http://www.acm.org/jgt/papers/SmithLyons96/hwb_rgb.html, an article
 * on colour conversion to/from RBG and HWB colour systems. 
 * It has been modified to return the converted value as a * parameter. 
 */

#define RETURN_HWB(h, w, b) {HWB->H = h; HWB->W = w; HWB->B = b; return HWB;}
#define RETURN_RGB(r, g, b) {RGB->R = r; RGB->G = g; RGB->B = b; return RGB;}
#define HWB_UNDEFINED -1
#define SETUP_RGB(s, r, g, b) {s.R = r/255.0; s.G = g/255.0; s.B = b/255.0;}

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MIN3(a,b,c) ((a)<(b)?(MIN(a,c)):(MIN(b,c)))
#define MAX(a,b) ((a)<(b)?(b):(a))
#define MAX3(a,b,c) ((a)<(b)?(MAX(b,c)):(MAX(a,c)))


/*
 * Theoretically, hue 0 (pure red) is identical to hue 6 in these transforms. Pure 
 * red always maps to 6 in this implementation. Therefore UNDEFINED can be 
 * defined as 0 in situations where only unsigned numbers are desired.
 */
typedef struct
{
  float R, G, B;
}
RGBType;
typedef struct
  {
    float H, W, B;
  }
HWBType;

static HWBType *
RGB_to_HWB (RGBType RGB, HWBType * HWB)
{

  /*
   * RGB are each on [0, 1]. W and B are returned on [0, 1] and H is  
   * returned on [0, 6]. Exception: H is returned UNDEFINED if W == 1 - B.  
   */

  float R = RGB.R, G = RGB.G, B = RGB.B, w, v, b, f;
  int i;

  w = MIN3 (R, G, B);
  v = MAX3 (R, G, B);
  b = 1 - v;
  if (v == w)
    RETURN_HWB (HWB_UNDEFINED, w, b);
  f = (R == w) ? G - B : ((G == w) ? B - R : R - G);
  i = (R == w) ? 3 : ((G == w) ? 5 : 1);
  RETURN_HWB (i - f / (v - w), w, b);

}

static float
HWB_Diff (int r1, int g1, int b1, int r2, int g2, int b2)
{
  RGBType RGB1, RGB2;
  HWBType HWB1, HWB2;
  float diff;

  SETUP_RGB (RGB1, r1, g1, b1);
  SETUP_RGB (RGB2, r2, g2, b2);

  RGB_to_HWB (RGB1, &HWB1);
  RGB_to_HWB (RGB2, &HWB2);

  /*
   * I made this bit up; it seems to produce OK results, and it is certainly
   * more visually correct than the current RGB metric. (PJW)
   */

  if ((HWB1.H == HWB_UNDEFINED) || (HWB2.H == HWB_UNDEFINED))
    {
      diff = 0;			/* Undefined hues always match... */
    }
  else
    {
      diff = abs (HWB1.H - HWB2.H);
      if (diff > 3)
	{
	  diff = 6 - diff;	/* Remember, it's a colour circle */
	}
    }

  diff = diff * diff + (HWB1.W - HWB2.W) * (HWB1.W - HWB2.W) + (HWB1.B - HWB2.B) * (HWB1.B - HWB2.B);

  return diff;
}


/*
 * This is not actually used, but is here for completeness, in case someone wants to
 * use the HWB stuff for anything else...
 */
static RGBType *
HWB_to_RGB (HWBType HWB, RGBType * RGB)
{

  /* 
   * H is given on [0, 6] or UNDEFINED. W and B are given on [0, 1].  
   * RGB are each returned on [0, 1]. 
   */

  float h = HWB.H, w = HWB.W, b = HWB.B, v, n, f;
  int i;

  v = 1 - b;
  if (h == HWB_UNDEFINED)
    RETURN_RGB (v, v, v);
  i = floor (h);
  f = h - i;
  if (i & 1)
    f = 1 - f;			/* if i is odd */
  n = w + f * (v - w);		/* linear interpolation between w and v */
  switch (i)
    {
    case 6:
    case 0:
      RETURN_RGB (v, n, w);
    case 1:
      RETURN_RGB (n, v, w);
    case 2:
      RETURN_RGB (w, v, n);
    case 3:
      RETURN_RGB (w, n, v);
    case 4:
      RETURN_RGB (n, w, v);
    case 5:
      RETURN_RGB (v, w, n);
    }

  return RGB;

}

int
gdImageColorClosestHWB (gdImagePtr im, int r, int g, int b)
{
  int i;
  /* long rd, gd, bd; */
  int ct = (-1);
  int first = 1;
  float mindist = 0;
  if (im->trueColor)
    {
      return gdTrueColor (r, g, b);
    }
  for (i = 0; (i < (im->colorsTotal)); i++)
    {
      float dist;
      if (im->open[i])
	{
	  continue;
	}
      dist = HWB_Diff (im->red[i], im->green[i], im->blue[i], r, g, b);
      if (first || (dist < mindist))
	{
	  mindist = dist;
	  ct = i;
	  first = 0;
	}
    }
  return ct;
}

int
gdImageColorExact (gdImagePtr im, int r, int g, int b)
{
  return gdImageColorExactAlpha (im, r, g, b, gdAlphaOpaque);
}

int
gdImageColorExactAlpha (gdImagePtr im, int r, int g, int b, int a)
{
  int i;
  if (im->trueColor)
    {
      return gdTrueColorAlpha (r, g, b, a);
    }
  for (i = 0; (i < (im->colorsTotal)); i++)
    {
      if (im->open[i])
	{
	  continue;
	}
      if ((im->red[i] == r) &&
	  (im->green[i] == g) &&
	  (im->blue[i] == b) &&
	  (im->alpha[i] == a))
	{
	  return i;
	}
    }
  return -1;
}

int
gdImageColorAllocate (gdImagePtr im, int r, int g, int b)
{
  return gdImageColorAllocateAlpha (im, r, g, b, gdAlphaOpaque);
}

int
gdImageColorAllocateAlpha (gdImagePtr im, int r, int g, int b, int a)
{
  int i;
  int ct = (-1);
  if (im->trueColor)
    {
      return gdTrueColorAlpha (r, g, b, a);
    }
  for (i = 0; (i < (im->colorsTotal)); i++)
    {
      if (im->open[i])
	{
	  ct = i;
	  break;
	}
    }
  if (ct == (-1))
    {
      ct = im->colorsTotal;
      if (ct == gdMaxColors)
	{
	  return -1;
	}
      im->colorsTotal++;
    }
  im->red[ct] = r;
  im->green[ct] = g;
  im->blue[ct] = b;
  im->alpha[ct] = a;
  im->open[ct] = 0;
  return ct;
}

/*
 * gdImageColorResolve is an alternative for the code fragment:
 *
 *      if ((color=gdImageColorExact(im,R,G,B)) < 0)
 *        if ((color=gdImageColorAllocate(im,R,G,B)) < 0)
 *          color=gdImageColorClosest(im,R,G,B);
 *
 * in a single function.    Its advantage is that it is guaranteed to
 * return a color index in one search over the color table.
 */

int
gdImageColorResolve (gdImagePtr im, int r, int g, int b)
{
  return gdImageColorResolveAlpha (im, r, g, b, gdAlphaOpaque);
}

int
gdImageColorResolveAlpha (gdImagePtr im, int r, int g, int b, int a)
{
  int c;
  int ct = -1;
  int op = -1;
  long rd, gd, bd, ad, dist;
  long mindist = 4 * 255 * 255;	/* init to max poss dist */
  if (im->trueColor)
    {
      return gdTrueColorAlpha (r, g, b, a);
    }

  for (c = 0; c < im->colorsTotal; c++)
    {
      if (im->open[c])
	{
	  op = c;		/* Save open slot */
	  continue;		/* Color not in use */
	}
      rd = (long) (im->red[c] - r);
      gd = (long) (im->green[c] - g);
      bd = (long) (im->blue[c] - b);
      ad = (long) (im->alpha[c] - a);
      dist = rd * rd + gd * gd + bd * bd + ad * ad;
      if (dist < mindist)
	{
	  if (dist == 0)
	    {
	      return c;		/* Return exact match color */
	    }
	  mindist = dist;
	  ct = c;
	}
    }
  /* no exact match.  We now know closest, but first try to allocate exact */
  if (op == -1)
    {
      op = im->colorsTotal;
      if (op == gdMaxColors)
	{			/* No room for more colors */
	  return ct;		/* Return closest available color */
	}
      im->colorsTotal++;
    }
  im->red[op] = r;
  im->green[op] = g;
  im->blue[op] = b;
  im->alpha[op] = a;
  im->open[op] = 0;
  return op;			/* Return newly allocated color */
}

void
gdImageColorDeallocate (gdImagePtr im, int color)
{
  if (im->trueColor)
    {
      return;
    }
  /* Mark it open. */
  im->open[color] = 1;
}

void
gdImageColorTransparent (gdImagePtr im, int color)
{
  if (!im->trueColor)
    {
      if (im->transparent != -1)
	{
	  im->alpha[im->transparent] = gdAlphaOpaque;
	}
      if (color != -1)
	{
	  im->alpha[color] = gdAlphaTransparent;
	}
    }
  im->transparent = color;
}

void
gdImagePaletteCopy (gdImagePtr to, gdImagePtr from)
{
  int i;
  int x, y, p;
  int xlate[256];
  if (to->trueColor)
    {
      return;
    }
  if (from->trueColor)
    {
      return;
    }

  for (i = 0; i < 256; i++)
    {
      xlate[i] = -1;
    };

  for (x = 0; x < (to->sx); x++)
    {
      for (y = 0; y < (to->sy); y++)
	{
	  p = gdImageGetPixel (to, x, y);
	  if (xlate[p] == -1)
	    {
	      /* This ought to use HWB, but we don't have an alpha-aware
	         version of that yet. */
	      xlate[p] = gdImageColorClosestAlpha (from, to->red[p], to->green[p], to->blue[p], to->alpha[p]);
	      /*printf("Mapping %d (%d, %d, %d, %d) to %d (%d, %d, %d, %d)\n", */
	      /*      p,  to->red[p], to->green[p], to->blue[p], to->alpha[p], */
	      /*      xlate[p], from->red[xlate[p]], from->green[xlate[p]], from->blue[xlate[p]], from->alpha[xlate[p]]); */
	    };
	  gdImageSetPixel (to, x, y, xlate[p]);
	};
    };

  for (i = 0; (i < (from->colorsTotal)); i++)
    {
      /*printf("Copying color %d (%d, %d, %d, %d)\n", i, from->red[i], from->blue[i], from->green[i], from->alpha[i]); */
      to->red[i] = from->red[i];
      to->blue[i] = from->blue[i];
      to->green[i] = from->green[i];
      to->alpha[i] = from->alpha[i];
      to->open[i] = 0;
    };

  for (i = from->colorsTotal; (i < to->colorsTotal); i++)
    {
      to->open[i] = 1;
    };

  to->colorsTotal = from->colorsTotal;

}

void
gdImageSetPixel (gdImagePtr im, int x, int y, int color)
{
  int p;
  switch (color)
    {
    case gdStyled:
      if (!im->style)
	{
	  /* Refuse to draw if no style is set. */
	  return;
	}
      else
	{
	  p = im->style[im->stylePos++];
	}
      if (p != (gdTransparent))
	{
	  gdImageSetPixel (im, x, y, p);
	}
      im->stylePos = im->stylePos % im->styleLength;
      break;
    case gdStyledBrushed:
      if (!im->style)
	{
	  /* Refuse to draw if no style is set. */
	  return;
	}
      p = im->style[im->stylePos++];
      if ((p != gdTransparent) && (p != 0))
	{
	  gdImageSetPixel (im, x, y, gdBrushed);
	}
      im->stylePos = im->stylePos % im->styleLength;
      break;
    case gdBrushed:
      gdImageBrushApply (im, x, y);
      break;
    case gdTiled:
      gdImageTileApply (im, x, y);
      break;
    default:
      if (gdImageBoundsSafe (im, x, y))
	{
	  if (im->trueColor)
	    {
	      if (im->alphaBlendingFlag)
		{
		  im->tpixels[y][x] =
		    gdAlphaBlend (im->tpixels[y][x],
				  color);
		}
	      else
		{
		  im->tpixels[y][x] = color;
		}
	    }
	  else
	    {
	      im->pixels[y][x] = color;
	    }
	}
      break;
    }
}

static void
gdImageBrushApply (gdImagePtr im, int x, int y)
{
  int lx, ly;
  int hy;
  int hx;
  int x1, y1, x2, y2;
  int srcx, srcy;
  if (!im->brush)
    {
      return;
    }
  hy = gdImageSY (im->brush) / 2;
  y1 = y - hy;
  y2 = y1 + gdImageSY (im->brush);
  hx = gdImageSX (im->brush) / 2;
  x1 = x - hx;
  x2 = x1 + gdImageSX (im->brush);
  srcy = 0;
  if (im->trueColor)
    {
      for (ly = y1; (ly < y2); ly++)
	{
	  srcx = 0;
	  for (lx = x1; (lx < x2); lx++)
	    {
	      int p;
	      p = gdImageGetTrueColorPixel (
					     im->brush, srcx, srcy);
	      gdImageSetPixel (im, lx, ly,
			       p);
	      srcx++;
	    }
	  srcy++;
	}
    }
  else
    {
      for (ly = y1; (ly < y2); ly++)
	{
	  srcx = 0;
	  for (lx = x1; (lx < x2); lx++)
	    {
	      int p;
	      p = gdImageGetPixel (im->brush, srcx, srcy);
	      /* Allow for non-square brushes! */
	      if (p != gdImageGetTransparent (im->brush))
		{
		  /* Truecolor brush. Very slow
		     on a palette destination. */
		  if (im->brush->trueColor)
		    {
		      gdImageSetPixel (im, lx, ly,
				       gdImageColorResolveAlpha (
								  im,
						      gdTrueColorGetRed (p),
						    gdTrueColorGetGreen (p),
						     gdTrueColorGetBlue (p),
						  gdTrueColorGetAlpha (p)));
		    }
		  else
		    {
		      gdImageSetPixel (im, lx, ly,
				       im->brushColorMap[p]);
		    }
		}
	      srcx++;
	    }
	  srcy++;
	}
    }
}

static void
gdImageTileApply (gdImagePtr im, int x, int y)
{
  int srcx, srcy;
  int p;
  if (!im->tile)
    {
      return;
    }
  srcx = x % gdImageSX (im->tile);
  srcy = y % gdImageSY (im->tile);
  if (im->trueColor)
    {
      p = gdImageGetTrueColorPixel (im->tile, srcx, srcy);
      gdImageSetPixel (im, x, y, p);
    }
  else
    {
      /* Allow for transparency */
      if (p != gdImageGetTransparent (im->tile))
	{
	  if (im->tile->trueColor)
	    {
	      /* Truecolor tile. Very slow
	         on a palette destination. */
	      gdImageSetPixel (im, x, y,
			       gdImageColorResolveAlpha (
							  im,
						      gdTrueColorGetRed (p),
						    gdTrueColorGetGreen (p),
						     gdTrueColorGetBlue (p),
						  gdTrueColorGetAlpha (p)));
	    }
	  else
	    {
	      gdImageSetPixel (im, x, y,
			       im->tileColorMap[p]);
	    }
	}
    }
}

int
gdImageGetPixel (gdImagePtr im, int x, int y)
{
  if (gdImageBoundsSafe (im, x, y))
    {
      if (im->trueColor)
	{
	  return im->tpixels[y][x];
	}
      else
	{
	  return im->pixels[y][x];
	}
    }
  else
    {
      return 0;
    }
}

int
gdImageGetTrueColorPixel (gdImagePtr im, int x, int y)
{
  int p = gdImageGetPixel (im, x, y);
  if (!im->trueColor)
    {
      return gdTrueColorAlpha (im->red[p], im->green[p], im->blue[p],
			       (im->transparent == p) ? gdAlphaTransparent :
			       gdAlphaOpaque);
    }
  else
    {
      return p;
    }
}

/* Bresenham as presented in Foley & Van Dam */
void
gdImageLine (gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
  int dx, dy, incr1, incr2, d, x, y, xend, yend, xdirflag, ydirflag;
  int wid;
  int w, wstart;
  int thick = im->thick;
  dx = abs (x2 - x1);
  dy = abs (y2 - y1);
  if (dy <= dx)
    {
      /* More-or-less horizontal. use wid for vertical stroke */
      wid = thick * cos (atan2 (dy, dx));
      if (wid == 0)
	wid = 1;

      d = 2 * dy - dx;
      incr1 = 2 * dy;
      incr2 = 2 * (dy - dx);
      if (x1 > x2)
	{
	  x = x2;
	  y = y2;
	  ydirflag = (-1);
	  xend = x1;
	}
      else
	{
	  x = x1;
	  y = y1;
	  ydirflag = 1;
	  xend = x2;
	}

      /* Set up line thickness */
      wstart = y - wid / 2;
      for (w = wstart; w < wstart + wid; w++)
	gdImageSetPixel (im, x, w, color);

      if (((y2 - y1) * ydirflag) > 0)
	{
	  while (x < xend)
	    {
	      x++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  y++;
		  d += incr2;
		}
	      wstart = y - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		gdImageSetPixel (im, x, w, color);
	    }
	}
      else
	{
	  while (x < xend)
	    {
	      x++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  y--;
		  d += incr2;
		}
	      wstart = y - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		gdImageSetPixel (im, x, w, color);
	    }
	}
    }
  else
    {
      /* More-or-less vertical. use wid for horizontal stroke */
      wid = thick * sin (atan2 (dy, dx));
      if (wid == 0)
	wid = 1;

      d = 2 * dx - dy;
      incr1 = 2 * dx;
      incr2 = 2 * (dx - dy);
      if (y1 > y2)
	{
	  y = y2;
	  x = x2;
	  yend = y1;
	  xdirflag = (-1);
	}
      else
	{
	  y = y1;
	  x = x1;
	  yend = y2;
	  xdirflag = 1;
	}

      /* Set up line thickness */
      wstart = x - wid / 2;
      for (w = wstart; w < wstart + wid; w++)
	gdImageSetPixel (im, w, y, color);

      if (((x2 - x1) * xdirflag) > 0)
	{
	  while (y < yend)
	    {
	      y++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  x++;
		  d += incr2;
		}
	      wstart = x - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		gdImageSetPixel (im, w, y, color);
	    }
	}
      else
	{
	  while (y < yend)
	    {
	      y++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  x--;
		  d += incr2;
		}
	      wstart = x - wid / 2;
	      for (w = wstart; w < wstart + wid; w++)
		gdImageSetPixel (im, w, y, color);
	    }
	}
    }
}
static void dashedSet (gdImagePtr im, int x, int y, int color,
		       int *onP, int *dashStepP, int wid, int vert);

void
gdImageDashedLine (gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
  int dx, dy, incr1, incr2, d, x, y, xend, yend, xdirflag, ydirflag;
  int dashStep = 0;
  int on = 1;
  int wid;
  int w, wstart, vert;
  int thick = im->thick;

  dx = abs (x2 - x1);
  dy = abs (y2 - y1);
  if (dy <= dx)
    {
      /* More-or-less horizontal. use wid for vertical stroke */
      wid = thick * sin (atan2 (dy, dx));
      vert = 1;

      d = 2 * dy - dx;
      incr1 = 2 * dy;
      incr2 = 2 * (dy - dx);
      if (x1 > x2)
	{
	  x = x2;
	  y = y2;
	  ydirflag = (-1);
	  xend = x1;
	}
      else
	{
	  x = x1;
	  y = y1;
	  ydirflag = 1;
	  xend = x2;
	}
      dashedSet (im, x, y, color, &on, &dashStep, wid, vert);
      if (((y2 - y1) * ydirflag) > 0)
	{
	  while (x < xend)
	    {
	      x++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  y++;
		  d += incr2;
		}
	      dashedSet (im, x, y, color, &on, &dashStep, wid, vert);
	    }
	}
      else
	{
	  while (x < xend)
	    {
	      x++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  y--;
		  d += incr2;
		}
	      dashedSet (im, x, y, color, &on, &dashStep, wid, vert);
	    }
	}
    }
  else
    {
      /* More-or-less vertical. use wid for horizontal stroke */
      wid = thick * sin (atan2 (dy, dx));
      vert = 0;

      d = 2 * dx - dy;
      incr1 = 2 * dx;
      incr2 = 2 * (dx - dy);
      if (y1 > y2)
	{
	  y = y2;
	  x = x2;
	  yend = y1;
	  xdirflag = (-1);
	}
      else
	{
	  y = y1;
	  x = x1;
	  yend = y2;
	  xdirflag = 1;
	}
      dashedSet (im, x, y, color, &on, &dashStep, wid, vert);
      if (((x2 - x1) * xdirflag) > 0)
	{
	  while (y < yend)
	    {
	      y++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  x++;
		  d += incr2;
		}
	      dashedSet (im, x, y, color, &on, &dashStep, wid, vert);
	    }
	}
      else
	{
	  while (y < yend)
	    {
	      y++;
	      if (d < 0)
		{
		  d += incr1;
		}
	      else
		{
		  x--;
		  d += incr2;
		}
	      dashedSet (im, x, y, color, &on, &dashStep, wid, vert);
	    }
	}
    }
}

static void
dashedSet (gdImagePtr im, int x, int y, int color,
	   int *onP, int *dashStepP, int wid, int vert)
{
  int dashStep = *dashStepP;
  int on = *onP;
  int w, wstart;

  dashStep++;
  if (dashStep == gdDashSize)
    {
      dashStep = 0;
      on = !on;
    }
  if (on)
    {
      if (vert)
	{
	  wstart = y - wid / 2;
	  for (w = wstart; w < wstart + wid; w++)
	    gdImageSetPixel (im, x, w, color);
	}
      else
	{
	  wstart = x - wid / 2;
	  for (w = wstart; w < wstart + wid; w++)
	    gdImageSetPixel (im, w, y, color);
	}
    }
  *dashStepP = dashStep;
  *onP = on;
}

int
gdImageBoundsSafe (gdImagePtr im, int x, int y)
{
  return (!(((y < 0) || (y >= im->sy)) ||
	    ((x < 0) || (x >= im->sx))));
}

void
gdImageChar (gdImagePtr im, gdFontPtr f, int x, int y,
	     int c, int color)
{
  int cx, cy;
  int px, py;
  int fline;
  cx = 0;
  cy = 0;
#ifdef CHARSET_EBCDIC
  c = ASC (c);
#endif /*CHARSET_EBCDIC */
  if ((c < f->offset) || (c >= (f->offset + f->nchars)))
    {
      return;
    }
  fline = (c - f->offset) * f->h * f->w;
  for (py = y; (py < (y + f->h)); py++)
    {
      for (px = x; (px < (x + f->w)); px++)
	{
	  if (f->data[fline + cy * f->w + cx])
	    {
	      gdImageSetPixel (im, px, py, color);
	    }
	  cx++;
	}
      cx = 0;
      cy++;
    }
}

void
gdImageCharUp (gdImagePtr im, gdFontPtr f,
	       int x, int y, int c, int color)
{
  int cx, cy;
  int px, py;
  int fline;
  cx = 0;
  cy = 0;
#ifdef CHARSET_EBCDIC
  c = ASC (c);
#endif /*CHARSET_EBCDIC */
  if ((c < f->offset) || (c >= (f->offset + f->nchars)))
    {
      return;
    }
  fline = (c - f->offset) * f->h * f->w;
  for (py = y; (py > (y - f->w)); py--)
    {
      for (px = x; (px < (x + f->h)); px++)
	{
	  if (f->data[fline + cy * f->w + cx])
	    {
	      gdImageSetPixel (im, px, py, color);
	    }
	  cy++;
	}
      cy = 0;
      cx++;
    }
}

void
gdImageString (gdImagePtr im, gdFontPtr f,
	       int x, int y, unsigned char *s, int color)
{
  int i;
  int l;
  l = strlen ((char *) s);
  for (i = 0; (i < l); i++)
    {
      gdImageChar (im, f, x, y, s[i], color);
      x += f->w;
    }
}

void
gdImageStringUp (gdImagePtr im, gdFontPtr f,
		 int x, int y, unsigned char *s, int color)
{
  int i;
  int l;
  l = strlen ((char *) s);
  for (i = 0; (i < l); i++)
    {
      gdImageCharUp (im, f, x, y, s[i], color);
      y -= f->w;
    }
}

static int strlen16 (unsigned short *s);

void
gdImageString16 (gdImagePtr im, gdFontPtr f,
		 int x, int y, unsigned short *s, int color)
{
  int i;
  int l;
  l = strlen16 (s);
  for (i = 0; (i < l); i++)
    {
      gdImageChar (im, f, x, y, s[i], color);
      x += f->w;
    }
}

void
gdImageStringUp16 (gdImagePtr im, gdFontPtr f,
		   int x, int y, unsigned short *s, int color)
{
  int i;
  int l;
  l = strlen16 (s);
  for (i = 0; (i < l); i++)
    {
      gdImageCharUp (im, f, x, y, s[i], color);
      y -= f->w;
    }
}

static int
strlen16 (unsigned short *s)
{
  int len = 0;
  while (*s)
    {
      s++;
      len++;
    }
  return len;
}

#ifndef HAVE_LSQRT
/* If you don't have a nice square root function for longs, you can use
   ** this hack
 */
long
lsqrt (long n)
{
  long result = (long) sqrt ((double) n);
  return result;
}
#endif

/* s and e are integers modulo 360 (degrees), with 0 degrees
   being the rightmost extreme and degrees changing clockwise.
   cx and cy are the center in pixels; w and h are the horizontal 
   and vertical diameter in pixels. Nice interface, but slow.
   See gd_arc_f_buggy.c for a better version that doesn't 
   seem to be bug-free yet. */

void 
gdImageArc (gdImagePtr im, int cx, int cy, int w, int h, int s, int e, int color)
{
  gdImageFilledArc (im, cx, cy, w, h, s, e, color, gdNoFill);
}

void 
gdImageFilledArc (gdImagePtr im, int cx, int cy, int w, int h, int s, int e, int color, int style)
{
  gdPoint pts[3];
  int i;
  int lx = 0, ly = 0;
  int fx, fy;
  int w2, h2;
  w2 = w / 2;
  h2 = h / 2;
  while (e < s)
    {
      e += 360;
    }
  for (i = s; (i <= e); i++)
    {
      int x, y;
      x = ((long) gdCosT[i % 360] * (long) w2 / 1024) + cx;
      y = ((long) gdSinT[i % 360] * (long) h2 / 1024) + cy;
      if (i != s)
	{
	  if (!(style & gdChord))
	    {
	      if (style & gdNoFill)
		{
		  gdImageLine (im, lx, ly, x, y, color);
		}
	      else
		{
		  /* This is expensive! */
		  pts[0].x = lx;
		  pts[0].y = ly;
		  pts[1].x = x;
		  pts[1].y = y;
		  pts[2].x = cx;
		  pts[2].y = cy;
		  gdImageFilledPolygon (im, pts, 3, color);
		}
	    }
	}
      else
	{
	  fx = x;
	  fy = y;
	}
      lx = x;
      ly = y;
    }
  if (style & gdChord)
    {
      if (style & gdNoFill)
	{
	  if (style & gdEdged)
	    {
	      gdImageLine (im, cx, cy, lx, ly, color);
	      gdImageLine (im, cx, cy, fx, fy, color);
	    }
	  gdImageLine (im, fx, fy, lx, ly, color);
	}
      else
	{
	  pts[0].x = fx;
	  pts[0].y = fy;
	  pts[1].x = lx;
	  pts[1].y = ly;
	  pts[2].x = cx;
	  pts[2].y = cy;
	  gdImageFilledPolygon (im, pts, 3, color);
	}
    }
  else
    {
      if (style & gdNoFill)
	{
	  if (style & gdEdged)
	    {
	      gdImageLine (im, cx, cy, lx, ly, color);
	      gdImageLine (im, cx, cy, fx, fy, color);
	    }
	}
    }
}

void 
gdImageFilledEllipse (gdImagePtr im, int cx, int cy, int w, int h, int color)
{
  gdImageFilledArc (im, cx, cy, w, h, 0, 360, color, gdPie);
}

void
gdImageFillToBorder (gdImagePtr im, int x, int y, int border, int color)
{
  int lastBorder;
  /* Seek left */
  int leftLimit, rightLimit;
  int i;
  leftLimit = (-1);
  if (border < 0)
    {
      /* Refuse to fill to a non-solid border */
      return;
    }
  for (i = x; (i >= 0); i--)
    {
      if (gdImageGetPixel (im, i, y) == border)
	{
	  break;
	}
      gdImageSetPixel (im, i, y, color);
      leftLimit = i;
    }
  if (leftLimit == (-1))
    {
      return;
    }
  /* Seek right */
  rightLimit = x;
  for (i = (x + 1); (i < im->sx); i++)
    {
      if (gdImageGetPixel (im, i, y) == border)
	{
	  break;
	}
      gdImageSetPixel (im, i, y, color);
      rightLimit = i;
    }
  /* Look at lines above and below and start paints */
  /* Above */
  if (y > 0)
    {
      lastBorder = 1;
      for (i = leftLimit; (i <= rightLimit); i++)
	{
	  int c;
	  c = gdImageGetPixel (im, i, y - 1);
	  if (lastBorder)
	    {
	      if ((c != border) && (c != color))
		{
		  gdImageFillToBorder (im, i, y - 1,
				       border, color);
		  lastBorder = 0;
		}
	    }
	  else if ((c == border) || (c == color))
	    {
	      lastBorder = 1;
	    }
	}
    }
  /* Below */
  if (y < ((im->sy) - 1))
    {
      lastBorder = 1;
      for (i = leftLimit; (i <= rightLimit); i++)
	{
	  int c;
	  c = gdImageGetPixel (im, i, y + 1);
	  if (lastBorder)
	    {
	      if ((c != border) && (c != color))
		{
		  gdImageFillToBorder (im, i, y + 1,
				       border, color);
		  lastBorder = 0;
		}
	    }
	  else if ((c == border) || (c == color))
	    {
	      lastBorder = 1;
	    }
	}
    }
}

void
gdImageFill (gdImagePtr im, int x, int y, int color)
{
  int lastBorder;
  int old;
  int leftLimit, rightLimit;
  int i;
  old = gdImageGetPixel (im, x, y);
  if (color == gdTiled)
    {
      /* Tile fill -- got to watch out! */
      int p, tileColor;
      int srcx, srcy;
      if (!im->tile)
	{
	  return;
	}
      /* Refuse to flood-fill with a transparent pattern --
         I can't do it without allocating another image */
      if (gdImageGetTransparent (im->tile) != (-1))
	{
	  return;
	}
      srcx = x % gdImageSX (im->tile);
      srcy = y % gdImageSY (im->tile);
      p = gdImageGetPixel (im->tile, srcx, srcy);
      if (im->trueColor)
	{
	  tileColor = p;
	}
      else
	{
	  if (im->tile->trueColor)
	    {
	      tileColor = gdImageColorResolveAlpha (im,
						    gdTrueColorGetRed (p),
						    gdTrueColorGetGreen (p),
						    gdTrueColorGetBlue (p),
						    gdTrueColorGetAlpha (p));
	    }
	  else
	    {
	      tileColor = im->tileColorMap[p];
	    }
	}
      if (old == tileColor)
	{
	  /* Nothing to be done */
	  return;
	}
    }
  else
    {
      if (old == color)
	{
	  /* Nothing to be done */
	  return;
	}
    }
  /* Seek left */
  leftLimit = (-1);
  for (i = x; (i >= 0); i--)
    {
      if (gdImageGetPixel (im, i, y) != old)
	{
	  break;
	}
      gdImageSetPixel (im, i, y, color);
      leftLimit = i;
    }
  if (leftLimit == (-1))
    {
      return;
    }
  /* Seek right */
  rightLimit = x;
  for (i = (x + 1); (i < im->sx); i++)
    {
      if (gdImageGetPixel (im, i, y) != old)
	{
	  break;
	}
      gdImageSetPixel (im, i, y, color);
      rightLimit = i;
    }
  /* Look at lines above and below and start paints */
  /* Above */
  if (y > 0)
    {
      lastBorder = 1;
      for (i = leftLimit; (i <= rightLimit); i++)
	{
	  int c;
	  c = gdImageGetPixel (im, i, y - 1);
	  if (lastBorder)
	    {
	      if (c == old)
		{
		  gdImageFill (im, i, y - 1, color);
		  lastBorder = 0;
		}
	    }
	  else if (c != old)
	    {
	      lastBorder = 1;
	    }
	}
    }
  /* Below */
  if (y < ((im->sy) - 1))
    {
      lastBorder = 1;
      for (i = leftLimit; (i <= rightLimit); i++)
	{
	  int c;
	  c = gdImageGetPixel (im, i, y + 1);
	  if (lastBorder)
	    {
	      if (c == old)
		{
		  gdImageFill (im, i, y + 1, color);
		  lastBorder = 0;
		}
	    }
	  else if (c != old)
	    {
	      lastBorder = 1;
	    }
	}
    }
}

void
gdImageRectangle (gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
  int x1h = x1, x1v = x1, y1h = y1, y1v = y1, x2h = x2, x2v = x2, y2h = y2,
    y2v = y2;
  int thick = im->thick;
  if (thick > 1)
    {
      int half = thick / 2;
      int half1 = thick - half;

      if (y1 < y2)
	{
	  y1v = y1h - half;
	  y2v = y2h + half1 - 1;
	}
      else
	{
	  y1v = y1h + half1 - 1;
	  y2v = y2h - half;
	}
    }

  gdImageLine (im, x1h, y1h, x2h, y1h, color);
  gdImageLine (im, x1h, y2h, x2h, y2h, color);
  gdImageLine (im, x1v, y1v, x1v, y2v, color);
  gdImageLine (im, x2v, y1v, x2v, y2v, color);
}

void
gdImageFilledRectangle (gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
  int x, y;
  for (y = y1; (y <= y2); y++)
    {
      for (x = x1; (x <= x2); x++)
	{
	  gdImageSetPixel (im, x, y, color);
	}
    }
}

void
gdImageCopy (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h)
{
  int c;
  int x, y;
  int tox, toy;
  int i;
  int colorMap[gdMaxColors];
  if (dst->trueColor)
    {
      /* 2.0: much easier when the destination is truecolor. */
      for (y = 0; (y < h); y++)
	{
	  for (x = 0; (x < w); x++)
	    {
	      int c = gdImageGetTrueColorPixel (src, srcX + x,
						srcY + y);
	      if (c != src->transparent)
		{
		  gdImageSetPixel (dst,
				   dstX + x,
				   dstY + y,
				   c);
		}
	    }
	}
      return;
    }
  for (i = 0; (i < gdMaxColors); i++)
    {
      colorMap[i] = (-1);
    }
  toy = dstY;
  for (y = srcY; (y < (srcY + h)); y++)
    {
      tox = dstX;
      for (x = srcX; (x < (srcX + w)); x++)
	{
	  int nc;
	  c = gdImageGetPixel (src, x, y);
	  /* Added 7/24/95: support transparent copies */
	  if (gdImageGetTransparent (src) == c)
	    {
	      tox++;
	      continue;
	    }
	  /* Have we established a mapping for this color? */
	  if (colorMap[c] == (-1))
	    {
	      /* If it's the same image, mapping is trivial */
	      if (dst == src)
		{
		  nc = c;
		}
	      else
		{
		  /* Get best match possible. This
		     function never returns error. */
		  nc = gdImageColorResolveAlpha (
						  dst,
						  src->red[c], src->green[c],
					       src->blue[c], src->alpha[c]);
		}
	      colorMap[c] = nc;
	    }
	  gdImageSetPixel (dst, tox, toy, colorMap[c]);
	  tox++;
	}
      toy++;
    }
}

/* This function is a substitute for real alpha channel operations,
   so it doesn't pay attention to the alpha channel. */
void
gdImageCopyMerge (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h, int pct)
{

  int c, dc;
  int x, y;
  int tox, toy;
  int ncR, ncG, ncB;
  toy = dstY;
  for (y = srcY; (y < (srcY + h)); y++)
    {
      tox = dstX;
      for (x = srcX; (x < (srcX + w)); x++)
	{
	  int nc;
	  c = gdImageGetPixel (src, x, y);
	  /* Added 7/24/95: support transparent copies */
	  if (gdImageGetTransparent (src) == c)
	    {
	      tox++;
	      continue;
	    }
	  /* If it's the same image, mapping is trivial */
	  if (dst == src)
	    {
	      nc = c;
	    }
	  else
	    {
	      dc = gdImageGetPixel (dst, tox, toy);

	      ncR = gdImageRed (src, c) * (pct / 100.0)
		+ gdImageRed (dst, dc) * ((100 - pct) / 100.0);
	      ncG = gdImageGreen (src, c) * (pct / 100.0)
		+ gdImageGreen (dst, dc) * ((100 - pct) / 100.0);
	      ncB = gdImageBlue (src, c) * (pct / 100.0)
		+ gdImageBlue (dst, dc) * ((100 - pct) / 100.0);

	      /* Find a reasonable color */
	      nc = gdImageColorResolve (dst, ncR, ncG, ncB);
	    }
	  gdImageSetPixel (dst, tox, toy, nc);
	  tox++;
	}
      toy++;
    }
}

/* This function is a substitute for real alpha channel operations,
   so it doesn't pay attention to the alpha channel. */
void
gdImageCopyMergeGray (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h, int pct)
{

  int c, dc;
  int x, y;
  int tox, toy;
  int ncR, ncG, ncB;
  float g;
  toy = dstY;
  for (y = srcY; (y < (srcY + h)); y++)
    {
      tox = dstX;
      for (x = srcX; (x < (srcX + w)); x++)
	{
	  int nc;
	  c = gdImageGetPixel (src, x, y);
	  /* Added 7/24/95: support transparent copies */
	  if (gdImageGetTransparent (src) == c)
	    {
	      tox++;
	      continue;
	    }
	  /* If it's the same image, mapping is trivial */
	  if (dst == src)
	    {
	      nc = c;
	    }
	  else
	    {
	      dc = gdImageGetPixel (dst, tox, toy);
	      g = 0.29900 * dst->red[dc]
		+ 0.58700 * dst->green[dc]
		+ 0.11400 * dst->blue[dc];

	      ncR = gdImageRed (src, c) * (pct / 100.0)
		+ gdImageRed (dst, dc) * g *
		((100 - pct) / 100.0);
	      ncG = gdImageGreen (src, c) * (pct / 100.0)
		+ gdImageGreen (dst, dc) * g *
		((100 - pct) / 100.0);
	      ncB = gdImageBlue (src, c) * (pct / 100.0)
		+ gdImageBlue (dst, dc) * g *
		((100 - pct) / 100.0);

	      /* First look for an exact match */
	      nc = gdImageColorExact (dst, ncR, ncG, ncB);
	      if (nc == (-1))
		{
		  /* No, so try to allocate it */
		  nc = gdImageColorAllocate (dst, ncR, ncG, ncB);
		  /* If we're out of colors, go for the
		     closest color */
		  if (nc == (-1))
		    {
		      nc = gdImageColorClosest (dst, ncR, ncG, ncB);
		    }
		}
	    }
	  gdImageSetPixel (dst, tox, toy, nc);
	  tox++;
	}
      toy++;
    }
}

void
gdImageCopyResized (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int dstW, int dstH, int srcW, int srcH)
{
  int c;
  int x, y;
  int tox, toy;
  int ydest;
  int i;
  int colorMap[gdMaxColors];
  /* Stretch vectors */
  int *stx;
  int *sty;
  /* We only need to use floating point to determine the correct
     stretch vector for one line's worth. */
  double accum;
  stx = (int *) gdMalloc (sizeof (int) * srcW);
  sty = (int *) gdMalloc (sizeof (int) * srcH);
  accum = 0;
  for (i = 0; (i < srcW); i++)
    {
      int got;
      accum += (double) dstW / (double) srcW;
      got = (int) floor (accum);
      stx[i] = got;
      accum -= got;
    }
  accum = 0;
  for (i = 0; (i < srcH); i++)
    {
      int got;
      accum += (double) dstH / (double) srcH;
      got = (int) floor (accum);
      sty[i] = got;
      accum -= got;
    }
  for (i = 0; (i < gdMaxColors); i++)
    {
      colorMap[i] = (-1);
    }
  toy = dstY;
  for (y = srcY; (y < (srcY + srcH)); y++)
    {
      for (ydest = 0; (ydest < sty[y - srcY]); ydest++)
	{
	  tox = dstX;
	  for (x = srcX; (x < (srcX + srcW)); x++)
	    {
	      int nc;
	      int mapTo;
	      if (!stx[x - srcX])
		{
		  continue;
		}
	      if (dst->trueColor)
		{
		  int d;
		  mapTo = gdImageGetTrueColorPixel (src, x, y);
		  /* Added 7/24/95: support transparent copies */
		  if (gdImageGetTransparent (src) == mapTo)
		    {
		      tox++;
		      continue;
		    }
		}
	      else
		{
		  c = gdImageGetPixel (src, x, y);
		  /* Added 7/24/95: support transparent copies */
		  if (gdImageGetTransparent (src) == c)
		    {
		      tox += stx[x - srcX];
		      continue;
		    }
		  if (src->trueColor)
		    {
		      /* Remap to the palette available in the
		         destination image. This is slow and
		         works badly. */
		      mapTo = gdImageColorResolveAlpha (dst,
						      gdTrueColorGetRed (c),
						    gdTrueColorGetGreen (c),
						     gdTrueColorGetBlue (c),
						   gdTrueColorGetAlpha (c));
		    }
		  else
		    {
		      /* Have we established a mapping for this color? */
		      if (colorMap[c] == (-1))
			{
			  /* If it's the same image, mapping is trivial */
			  if (dst == src)
			    {
			      nc = c;
			    }
			  else
			    {
			      /* Find or create the best match */
			      mapTo = gdImageColorResolveAlpha (dst,
						      gdTrueColorGetRed (c),
						    gdTrueColorGetGreen (c),
						     gdTrueColorGetBlue (c),
						   gdTrueColorGetAlpha (c));
			    }
			  colorMap[c] = nc;
			}
		      mapTo = colorMap[c];
		    }
		}
	      for (i = 0; (i < stx[x - srcX]); i++)
		{
		  gdImageSetPixel (dst, tox, toy, mapTo);
		  tox++;
		}
	    }
	  toy++;
	}
    }
  gdFree (stx);
  gdFree (sty);
}

/* When gd 1.x was first created, floating point was to be avoided.
   These days it is often faster than table lookups or integer
   arithmetic. The routine below is shamelessly, gloriously
   floating point. TBB */

void
gdImageCopyResampled (gdImagePtr dst,
		      gdImagePtr src,
		      int dstX, int dstY,
		      int srcX, int srcY,
		      int dstW, int dstH,
		      int srcW, int srcH)
{
  int x, y;
  float sx, sy;
  if (!dst->trueColor)
    {
      gdImageCopyResized (
			   dst, src, dstX, dstY, srcX, srcY, dstW, dstH,
			   srcW, srcH);
      return;
    }
  for (y = dstY; (y < dstY + dstH); y++)
    {
      for (x = dstX; (x < dstX + dstW); x++)
	{
	  int pd = gdImageGetPixel (dst, x, y);
	  float sy1, sy2, sx1, sx2;
	  float sx, sy;
	  float spixels = 0;
	  float red = 0.0, green = 0.0, blue = 0.0, alpha = 0.0;
	  sy1 = ((float) y - (float) dstY) * (float) srcH /
	    (float) dstH;
	  sy2 = ((float) (y + 1) - (float) dstY) * (float) srcH /
	    (float) dstH;
	  sy = sy1;
	  do
	    {
	      float yportion;
	      if (floor (sy) == floor (sy1))
		{
		  yportion = 1.0 - (sy - floor (sy));
		  if (yportion > sy2 - sy1)
		    {
		      yportion = sy2 - sy1;
		    }
		  sy = floor (sy);
		}
	      else if (sy == floor (sy2))
		{
		  yportion = sy2 - floor (sy2);
		}
	      else
		{
		  yportion = 1.0;
		}
	      sx1 = ((float) x - (float) dstX) * (float) srcW /
		dstW;
	      sx2 = ((float) (x + 1) - (float) dstX) * (float) srcW /
		dstW;
	      sx = sx1;
	      do
		{
		  float xportion;
		  float pcontribution;
		  int p;
		  if (floor (sx) == floor (sx1))
		    {
		      xportion = 1.0 - (sx - floor (sx));
		      if (xportion > sx2 - sx1)
			{
			  xportion = sx2 - sx1;
			}
		      sx = floor (sx);
		    }
		  else if (sx == floor (sx2))
		    {
		      xportion = sx2 - floor (sx2);
		    }
		  else
		    {
		      xportion = 1.0;
		    }
		  pcontribution = xportion * yportion;
		  p = gdImageGetTrueColorPixel (
						 src,
						 (int) sx,
						 (int) sy);
		  red += gdTrueColorGetRed (p) * pcontribution;
		  green += gdTrueColorGetGreen (p) * pcontribution;
		  blue += gdTrueColorGetBlue (p) * pcontribution;
		  alpha += gdTrueColorGetAlpha (p) * pcontribution;
		  spixels += xportion * yportion;
		  sx += 1.0;
		}
	      while (sx < sx2);
	      sy += 1.0;
	    }
	  while (sy < sy2);
	  if (spixels != 0.0)
	    {
	      red /= spixels;
	      green /= spixels;
	      blue /= spixels;
	      alpha /= spixels;
	    }
	  /* Clamping to allow for rounding errors above */
	  if (red > 255.0)
	    {
	      red = 255.0;
	    }
	  if (green > 255.0)
	    {
	      green = 255.0;
	    }
	  if (blue > 255.0)
	    {
	      blue = 255.0;
	    }
	  if (alpha > gdAlphaMax)
	    {
	      alpha = gdAlphaMax;
	    }
	  gdImageSetPixel (dst,
			   x, y,
			   gdTrueColorAlpha ((int) red,
					     (int) green,
					     (int) blue,
					     (int) alpha));
	}
    }
}

gdImagePtr
gdImageCreateFromXbm (FILE * fd)
{
  gdImagePtr im;
  int bit;
  int w, h;
  int bytes;
  int ch;
  int i, x, y;
  char *sp;
  char s[161];
  if (!fgets (s, 160, fd))
    {
      return 0;
    }
  sp = &s[0];
  /* Skip #define */
  sp = strchr (sp, ' ');
  if (!sp)
    {
      return 0;
    }
  /* Skip width label */
  sp++;
  sp = strchr (sp, ' ');
  if (!sp)
    {
      return 0;
    }
  /* Get width */
  w = atoi (sp + 1);
  if (!w)
    {
      return 0;
    }
  if (!fgets (s, 160, fd))
    {
      return 0;
    }
  sp = s;
  /* Skip #define */
  sp = strchr (sp, ' ');
  if (!sp)
    {
      return 0;
    }
  /* Skip height label */
  sp++;
  sp = strchr (sp, ' ');
  if (!sp)
    {
      return 0;
    }
  /* Get height */
  h = atoi (sp + 1);
  if (!h)
    {
      return 0;
    }
  /* Skip declaration line */
  if (!fgets (s, 160, fd))
    {
      return 0;
    }
  bytes = (w * h / 8) + 1;
  im = gdImageCreate (w, h);
  gdImageColorAllocate (im, 255, 255, 255);
  gdImageColorAllocate (im, 0, 0, 0);
  x = 0;
  y = 0;
  for (i = 0; (i < bytes); i++)
    {
      char h[3];
      int b;
      /* Skip spaces, commas, CRs, 0x */
      while (1)
	{
	  ch = getc (fd);
	  if (ch == EOF)
	    {
	      goto fail;
	    }
	  if (ch == 'x')
	    {
	      break;
	    }
	}
      /* Get hex value */
      ch = getc (fd);
      if (ch == EOF)
	{
	  goto fail;
	}
      h[0] = ch;
      ch = getc (fd);
      if (ch == EOF)
	{
	  goto fail;
	}
      h[1] = ch;
      h[2] = '\0';
      sscanf (h, "%x", &b);
      for (bit = 1; (bit <= 128); (bit = bit << 1))
	{
	  gdImageSetPixel (im, x++, y, (b & bit) ? 1 : 0);
	  if (x == im->sx)
	    {
	      x = 0;
	      y++;
	      if (y == im->sy)
		{
		  return im;
		}
	      /* Fix 8/8/95 */
	      break;
	    }
	}
    }
  /* Shouldn't happen */
  fprintf (stderr, "Error: bug in gdImageCreateFromXbm!\n");
  return 0;
fail:
  gdImageDestroy (im);
  return 0;
}

void
gdImagePolygon (gdImagePtr im, gdPointPtr p, int n, int c)
{
  int i;
  int lx, ly;
  if (!n)
    {
      return;
    }
  lx = p->x;
  ly = p->y;
  gdImageLine (im, lx, ly, p[n - 1].x, p[n - 1].y, c);
  for (i = 1; (i < n); i++)
    {
      p++;
      gdImageLine (im, lx, ly, p->x, p->y, c);
      lx = p->x;
      ly = p->y;
    }
}

int gdCompareInt (const void *a, const void *b);

/* THANKS to Kirsten Schulz for the polygon fixes! */

/* The intersection finding technique of this code could be improved  */
/* by remembering the previous intertersection, and by using the slope. */
/* That could help to adjust intersections  to produce a nice */
/* interior_extrema. */

void
gdImageFilledPolygon (gdImagePtr im, gdPointPtr p, int n, int c)
{
  int i;
  int y;
  int miny, maxy;
  int x1, y1;
  int x2, y2;
  int ind1, ind2;
  int ints;
  if (!n)
    {
      return;
    }
  if (!im->polyAllocated)
    {
      im->polyInts = (int *) gdMalloc (sizeof (int) * n);
      im->polyAllocated = n;
    }
  if (im->polyAllocated < n)
    {
      while (im->polyAllocated < n)
	{
	  im->polyAllocated *= 2;
	}
      im->polyInts = (int *) gdRealloc (im->polyInts,
					sizeof (int) * im->polyAllocated);
    }
  miny = p[0].y;
  maxy = p[0].y;
  for (i = 1; (i < n); i++)
    {
      if (p[i].y < miny)
	{
	  miny = p[i].y;
	}
      if (p[i].y > maxy)
	{
	  maxy = p[i].y;
	}
    }
  /* Fix in 1.3: count a vertex only once */
  for (y = miny; (y <= maxy); y++)
    {
/*1.4           int interLast = 0; */
/*              int dirLast = 0; */
/*              int interFirst = 1; */
      ints = 0;
      for (i = 0; (i < n); i++)
	{
	  if (!i)
	    {
	      ind1 = n - 1;
	      ind2 = 0;
	    }
	  else
	    {
	      ind1 = i - 1;
	      ind2 = i;
	    }
	  y1 = p[ind1].y;
	  y2 = p[ind2].y;
	  if (y1 < y2)
	    {
	      x1 = p[ind1].x;
	      x2 = p[ind2].x;
	    }
	  else if (y1 > y2)
	    {
	      y2 = p[ind1].y;
	      y1 = p[ind2].y;
	      x2 = p[ind1].x;
	      x1 = p[ind2].x;
	    }
	  else
	    {
	      continue;
	    }
	  if ((y >= y1) && (y < y2))
	    {
	      im->polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
	    }
	  else if ((y == maxy) && (y > y1) && (y <= y2))
	    {
	      im->polyInts[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
	    }
	}
      qsort (im->polyInts, ints, sizeof (int), gdCompareInt);

      for (i = 0; (i < (ints)); i += 2)
	{
	  gdImageLine (im, im->polyInts[i], y,
		       im->polyInts[i + 1], y, c);
	}
    }
}

int
gdCompareInt (const void *a, const void *b)
{
  return (*(const int *) a) - (*(const int *) b);
}

void
gdImageSetStyle (gdImagePtr im, int *style, int noOfPixels)
{
  if (im->style)
    {
      gdFree (im->style);
    }
  im->style = (int *)
    gdMalloc (sizeof (int) * noOfPixels);
  memcpy (im->style, style, sizeof (int) * noOfPixels);
  im->styleLength = noOfPixels;
  im->stylePos = 0;
}

void
gdImageSetThickness (gdImagePtr im, int thickness)
{
  im->thick = thickness;
}

void
gdImageSetBrush (gdImagePtr im, gdImagePtr brush)
{
  int i;
  im->brush = brush;
  if ((!im->trueColor) && (!im->brush->trueColor))
    {
      for (i = 0; (i < gdImageColorsTotal (brush)); i++)
	{
	  int index;
	  index = gdImageColorResolveAlpha (im,
					    gdImageRed (brush, i),
					    gdImageGreen (brush, i),
					    gdImageBlue (brush, i),
					    gdImageAlpha (brush, i));
	  im->brushColorMap[i] = index;
	}
    }
}

void
gdImageSetTile (gdImagePtr im, gdImagePtr tile)
{
  int i;
  im->tile = tile;
  if ((!im->trueColor) && (!im->tile->trueColor))
    {
      for (i = 0; (i < gdImageColorsTotal (tile)); i++)
	{
	  int index;
	  index = gdImageColorResolveAlpha (im,
					    gdImageRed (tile, i),
					    gdImageGreen (tile, i),
					    gdImageBlue (tile, i),
					    gdImageAlpha (tile, i));
	  im->tileColorMap[i] = index;
	}
    }
}

void
gdImageInterlace (gdImagePtr im, int interlaceArg)
{
  im->interlace = interlaceArg;
}

int
gdImageCompare (gdImagePtr im1, gdImagePtr im2)
{
  int x, y;
  int p1, p2;
  int cmpStatus = 0;
  int sx, sy;

  if (im1->interlace != im2->interlace)
    {
      cmpStatus |= GD_CMP_INTERLACE;
    }

  if (im1->transparent != im2->transparent)
    {
      cmpStatus |= GD_CMP_TRANSPARENT;
    }

  if (im1->trueColor != im2->trueColor)
    {
      cmpStatus |= GD_CMP_TRUECOLOR;
    }

  sx = im1->sx;
  if (im1->sx != im2->sx)
    {
      cmpStatus |= GD_CMP_SIZE_X + GD_CMP_IMAGE;
      if (im2->sx < im1->sx)
	{
	  sx = im2->sx;
	}
    }

  sy = im1->sy;
  if (im1->sy != im2->sy)
    {
      cmpStatus |= GD_CMP_SIZE_Y + GD_CMP_IMAGE;
      if (im2->sy < im1->sy)
	{
	  sy = im2->sy;
	}
    }

  if (im1->colorsTotal != im2->colorsTotal)
    {
      cmpStatus |= GD_CMP_NUM_COLORS;
    }

  for (y = 0; (y < sy); y++)
    {
      for (x = 0; (x < sx); x++)
	{
	  p1 = im1->pixels[y][x];
	  p2 = im2->pixels[y][x];
	  if (gdImageRed (im1, p1) != gdImageRed (im2, p2))
	    {
	      cmpStatus |= GD_CMP_COLOR + GD_CMP_IMAGE;
	      break;
	    }
	  if (gdImageGreen (im1, p1) != gdImageGreen (im2, p2))
	    {
	      cmpStatus |= GD_CMP_COLOR + GD_CMP_IMAGE;
	      break;
	    }
	  if (gdImageBlue (im1, p1) != gdImageBlue (im2, p2))
	    {
	      cmpStatus |= GD_CMP_COLOR + GD_CMP_IMAGE;
	      break;
	    }
#if 0
	  /* Soon we'll add alpha channel to palettes */
	  if (gdImageAlpha (im1, p1) != gdImageAlpha (im2, p2))
	    {
	      cmpStatus |= GD_CMP_COLOR + GD_CMP_IMAGE;
	      break;
	    }
#endif
	}
      if (cmpStatus & GD_CMP_COLOR)
	{
	  break;
	};
    }

  return cmpStatus;
}

int
gdAlphaBlend (int dst, int src)
{
  return (((((gdAlphaTransparent - gdTrueColorGetAlpha (src)) *
	     gdTrueColorGetRed (src) / gdAlphaMax) +
	    (gdTrueColorGetAlpha (src) *
	     gdTrueColorGetRed (dst)) / gdAlphaMax) << 16) +
	  ((((gdAlphaTransparent - gdTrueColorGetAlpha (src)) *
	     gdTrueColorGetGreen (src) / gdAlphaMax) +
	    (gdTrueColorGetAlpha (src) *
	     gdTrueColorGetGreen (dst)) / gdAlphaMax) << 8) +
	  (((gdAlphaTransparent - gdTrueColorGetAlpha (src)) *
	    gdTrueColorGetBlue (src) / gdAlphaMax) +
	   (gdTrueColorGetAlpha (src) *
	    gdTrueColorGetBlue (dst)) / gdAlphaMax));
}

void
gdImageAlphaBlending (gdImagePtr im, int alphaBlendingArg)
{
  im->alphaBlendingFlag = alphaBlendingArg;
}

void
gdImageSaveAlpha (gdImagePtr im, int saveAlphaArg)
{
  im->saveAlphaFlag = saveAlphaArg;
}
