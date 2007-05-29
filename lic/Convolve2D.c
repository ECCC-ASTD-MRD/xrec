/*
 * $Header: /usr/local/src/lic/liblic/RCS/Convolve2D.c,v 1.8 1993/10/26 18:26:58 casey Exp $
 */

/*
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Lawrence Livermore National Laboratory and its
 *	contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
    static char rcsid[] = "$Header: /usr/local/src/lic/liblic/RCS/Convolve2D.c,v 1.8 1993/10/26 18:26:58 casey Exp $";
    static char copyright[] =
	"Copyright (c) 1993 The Regents of the University of California.\n"
	"All rights reserved.\n";
#endif


#include "liblic.h"


/*
 *	Two-dimensional Line Integral Convolution.
 *	============================================
 */


#ifdef DEBUG
extern FILE *ps;
#endif


void
LIC_Convolve2D(LIC      *This,
	       int       i,
	       int       j,
	       int       direction,
	       double   *rIntegral,
	       double   *gIntegral,
	       double   *bIntegral,
	       double   *aIntegral,
	       double   *KernelArea)
    /*
     * Perform Line Integral Convolution on a two-dimensional vector field.
     */
{
    REGISTER double L;			/* pos/neg length of kernel */
    REGISTER double s, sp;		/* parametric distance along kernel */
    double          wp;			/* integral corresponding to sp */
    double          rSum, gSum,		/* integrated pixel values */
		    bSum, aSum;
    int             speed;		/* integral table speed index */
    double         *itab, is;		/* integral table and index scale */
    int             LoopCount;		/* main loop iteration count */

    sp = 0.0;

    rSum = 0.0;
    gSum = 0.0;
    bSum = 0.0;
    aSum = 0.0;

    /*
     * Establish length, L, of convolution and which ``speed'' version of the
     * filter kernel to use.  The default L is the value set by the user.
     * For VariableLength convolutions, L is scaled by the magnitude of the
     * vector.  The default speed is to use the maximum ``speed'' version of
     * the filter kernel.  For VariableSpeed convolutions, speed is scaled
     * by the magnitude of the vector.
     */
    L = LIC_Length(This);
    speed = LIC_INTEGRAL_SPEEDS - 1;
    if (This->VariableLength || This->VariableSpeed)
    {
	REGISTER double fx, fy, norm;

	fx = INDEX_2D(This->InputField, i, j)[0];
	fy = INDEX_2D(This->InputField, i, j)[1];
	norm = sqrt(fx*fx + fy*fy) / This->MaxLength;

	if (This->VariableLength)
	    L *= norm;
	if (This->VariableSpeed)
	    speed *= norm;
    }

    /*
     * Establish filter kernel integral table to use based on direction and
     * speed.  Also determine index scaling for values of s, 0 <= s <= L,
     * into the table.  Note that the scaling is performed relative to L,
     * not LIC_Length(This).  Scaling relative to L means that the filter
     * is effectively dilated to match the convolution length even when
     * performing VariableLength convolution.
     */
    if (This->NeedIntegration)
	LIC_BuildIntegralTables(This);
    if (direction == LIC_FOREWARD)
	itab = &This->PosIntegralTable[speed][0];
    else
	itab = &This->NegIntegralTable[speed][0];
    is = (double)(LIC_INTEGRAL_LEN - 1) / L;

    LoopCount = 0;

#   ifdef DEBUG
	if (ThisPixel)
	    fprintf(ps, "0.0 0.0 moveto\n");
#   endif

    /*
     * Only perform LIC if L is greater than zero.  Zero lengths
     * can result from bad user input or magnitude based lengths.
     */
    if (L > 0)
    {
	REGISTER int    fi, fj;		/* vector field index */
	REGISTER double x, y;		/* current cartesian location in the */
					/*   vector field */

	fi = i;
	fj = j;

	x =  i + 0.5;
	y = This->NormalField.Yres - j - 0.5;

	wp = 0.0;

	/* Loop until we reach the end of the line integral */
	do
	{
	    REGISTER double t;		/* distance to nearest cell edge */
	    REGISTER double fx, fy;	/* vector field values */

	    /*
	     * Grab the current cell vector.
	     */

	    /* Get the field value for this cell */
	    fx = INDEX_2D(This->NormalField, fi, fj)[0];
	    fy = INDEX_2D(This->NormalField, fi, fj)[1];

	    /* Bail out if the vector field goes to zero */
	    if (fx == 0.0 && fy == 0.0)
		break;

	    if (direction == LIC_BACKWARD)
	    {
		fx = -fx;
		fy = -fy;
	    }

	    /*
	     * Compute the intersection with the next cell along the line
	     * of the vector field.
	     */
	    {
		REGISTER double tt;
#		define TT(v) \
		{ \
		    tt = (v); \
		    if (tt < t) \
			t = tt; \
		}

		if (fx < -SIN_PARALLEL)
		    t = (FLOOR(x) - x) / fx;		/* left edge */
		else if (fx > SIN_PARALLEL)
		    t = (CEIL(x) - x) / fx;		/* right edge */
		else
		    t = HUGE_VAL;

		if (fy < -SIN_PARALLEL)
		    TT((FLOOR(y) - y) / fy)		/* bottom edge */
		else if (fy > SIN_PARALLEL)
		    TT((CEIL(y) - y) / fy)		/* top edge */

#		undef TT
	    }

	    /* s and sp represent a monotonically moving convolution window */
	    s  = sp;
	    sp = sp + t;
	    t += ROUND_OFF;

	    /* Make sure we don't exceed the kernel width */
	    if (sp > L)
	    {
		sp = L;
		t  =  sp - s;
	    }

#	    ifdef DEBUG
		if (ThisPixel)
		    fprintf(ps, "%f %f rlineto\n", fx*t, fy*t);
#	    endif

	    /*
	     * Grab the input pixel corresponding to the current cell and
	     * integrate its value over the convolution kernel from s to sp.
	     */
	    {
#		if (PixelSize >= 3)
		    double rV, gV, bV;
#		endif
#		if (PixelSize == 4 || PixelSize == 1)
		    double aV;
#		endif
		{
		    REGISTER int ii, ij;

		    /* toriodally wrap input image coordinates */
		    ii = fi;
		    ij = fj;
		    WRAP(ii, This->InputImage.Xres);
		    WRAP(ij, This->InputImage.Yres);

		    /* Get the input image value for this cell */
#		    if (PixelSize >= 3)
			rV = RED  (INDEX_2D(This->InputImage, ii, ij));
			gV = GREEN(INDEX_2D(This->InputImage, ii, ij));
			bV = BLUE (INDEX_2D(This->InputImage, ii, ij));
#		    endif
#		    if (PixelSize == 4 || PixelSize == 1)
			aV = ALPHA(INDEX_2D(This->InputImage, ii, ij));
#		    endif
		}

		/* integrate over the convolution kernel between s and sp */
		{
		    double          wt;
		    REGISTER double dw;

		    wt = itab[(int)(sp * is)];
		    dw = wt - wp;
		    wp = wt;

#		    if (PixelSize >= 3)
			rSum  += (rV * dw);
			gSum  += (gV * dw);
			bSum  += (bV * dw);
#		    endif
#		    if (PixelSize == 4 || PixelSize == 1)
			aSum  += (aV * dw);
#		    endif
		}
	    }

	    /*
	     * March to the next cell.
	     */

	    LoopCount++;

	    /* Compute the cell we just stepped into */
	    x = x + fx*t;
	    y = y + fy*t;

	    /* break out of the loop if we step out of the field */
	    if (x < 0.0 || y < 0.0)
		    break;

	    fi = IFLOOR(x);
	    fj = This->NormalField.Yres - ICEIL(y);

	    /* Break out of the loop if we step out of the field */
	    if (   fi >= This->NormalField.Xres
		|| fj < 0)
		break;

	    /*
	     * Loop until we reach the end or we think we've fallen into
	     * a singularity.
	     */
	} while (sp < L && LoopCount <= 3*L);
    }

    if (LoopCount > 0 && L > 0)
    {
	*rIntegral = rSum;
	*gIntegral = gSum;
	*bIntegral = bSum;
	*aIntegral = aSum;

	/* Compute an integration normalization factor as function of sp */
	*KernelArea = (This->NormalizationType == LIC_VARIABLE)
	    ? wp
	    : itab[LIC_INTEGRAL_LEN - 1];
    }
    else
    {
#	if (PixelSize >= 3)
	    *rIntegral = (This->DefaultRed == -1)
		? RED  (INDEX_2D(This->InputImage, i, j))
		: (double)This->DefaultRed;
	    *gIntegral = (This->DefaultGreen == -1)
		? GREEN(INDEX_2D(This->InputImage, i, j))
		: (double)This->DefaultGreen;
	    *bIntegral = (This->DefaultBlue == -1)
		? BLUE (INDEX_2D(This->InputImage, i, j))
		: (double)This->DefaultBlue;
#	else
	    *rIntegral = 0.0;
	    *gIntegral = 0.0;
	    *bIntegral = 0.0;
#	endif
#	if (PixelSize == 4 || PixelSize == 1)
	    *aIntegral = (This->DefaultAlpha == -1)
		? ALPHA(INDEX_2D(This->InputImage, i, j))
		: (double)This->DefaultAlpha;
#	else
	    *aIntegral = 0.0;
#	endif

	*KernelArea = 1;
    }

    /*
     * Random bookkeeping for performance monitoring.
     */
#   if (defined(PARALLEL))
	/*
	 * It isn't really worth the performance hit to do a critical
	 * section locking per pixel just to keep track of a few
	 * performance metrics that probably won't be used ...
	 */
#   else
	This->TotalLoopCount += LoopCount;
	This->TotalLength    += sp;
#   endif
}
