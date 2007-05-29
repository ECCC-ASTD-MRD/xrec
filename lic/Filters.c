/*
 * $Header: /usr/local/src/lic/liblic/RCS/Filters.c,v 1.12 1993/08/10 23:35:32 casey Exp $
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
    static char rcsid[] = "$Header: /usr/local/src/lic/liblic/RCS/Filters.c,v 1.12 1993/08/10 23:35:32 casey Exp $";
    static char copyright[] =
	"Copyright (c) 1993 The Regents of the University of California.\n"
	"All rights reserved.\n";
#endif


#include "liblic.h"


/*
 *	Build integral tables from filter integrator.
 *	=============================================
 */

void
LIC_BuildIntegralTables(LIC *This)
    /*
     * Build integral tables for LIC instance if needed.
     */
{
    int speed, length;

    if (!This->NeedIntegration)
	return;
    This->NeedIntegration = FALSE;

    /*
     * Build the convolution integral tables.  Build LIC_INTEGRAL_SPEEDS
     * different versions of them with speed varying between 0 and
     * LIC_INTEGRAL_SPEEDS.
     */
    for (speed = 0; speed < LIC_INTEGRAL_SPEEDS; speed++)
    {
	double s;

	for (length = 0; length < LIC_INTEGRAL_LEN; length++)
	{
	    s = LIC_Length(This) * length / (double)LIC_INTEGRAL_LEN;
	    This->NegIntegralTable[speed][length] = This->Filter(This,  -s, 0.0, speed);
	    This->PosIntegralTable[speed][length] = This->Filter(This, 0.0,   s, speed);
	}
    }
}


double
LIC_Box(LIC *This, double a, double b, int speed)
    /*
     * Compute the integral bewteen points a and b, a <= b, of a box
     * filter:
     *
     *    k(s) = 1
     */
{
    return(b - a);
}


double
LIC_Ripple(LIC *This, double a, double b, int speed)
    /*
     * Compute the integral bewteen points a and b, a <= b, of a phase shifted
     * Hanning low pass filter multiplied by a Hanning window function:
     *
     *    k(s) = (cos(d*s + phase) + 1)/2 * (cos(c*s) + 1)/2
     *
     * We need to scale the filter to the length of the filter support.  This
     * requires scaling each occurance of s by Pi/L.  Things fall out in the
     * integral which allow us to slip the scaling factors into c and d.
     */
{
    REGISTER double m;		/* speed multiple */
    REGISTER double p;		/* phase of the Hanning ripple function */
    REGISTER double f;		/* frequency of the Hanning ripple function */
    REGISTER double c;		/* Hanning window function dilation constant */
    REGISTER double d;		/* Hanning ripple filter dilation constant */
    REGISTER double w;		/* integral value */
    
    if (LIC_Length(This) == 0 || a == b)
	return(0.0);
    
    /*
     * Implement ``speed'' variations by scaling the frequency inversely
     * to speed.  This causes low magnitude vectors to be convolved using
     * a higher frequency filter which will yield less distinct output
     * features.  As the frequency increases the filter begins to approximate
     * the Hanning window filter.
     */

    p = LIC_Phase(This);
    f = LIC_Frequency(This) * (6.0 - 5.0*(double)speed/(LIC_INTEGRAL_SPEEDS - 1));
    c = 1.0/LIC_Length(This) * M_PI;
    d = 1.0/LIC_Length(This) * M_PI * f;
    
    w =   b                          - a
	+ sin(b*c)/c                 - sin(a*c)/c
	+ sin(b*d + p)/d             - sin(a*d + p)/d
	+ sin(b*(c+d) + p)/(2*(c+d)) - sin(a*(c+d) + p)/(2*(c+d));
    if (c != d)
	w +=  sin(b*(c-d) - p)/(2*(c-d))
	    - sin(a*(c-d) - p)/(2*(c-d));
    w /= 4;
    return(w);
}


double
LIC_Ramp(LIC *This, double a, double b, int speed)
    /*
     * Compute the integral bewteen points a and b, a <= b, of a biased
     * triangle filter:
     *
     *    k(s) = (s + L) / (2 * L)
     */
{
    if (LIC_Length(This) == 0)
	return(0.0);
    else
	return((a*a - b*b)/(4*LIC_Length(This)) + (a - b)/2);
}


double
LIC_Select(LIC *This, double a, double b, int speed)
    /*
     * Compute the integral bewteen points a and b, a <= b, of a 
     * narrow Gaussian filter which sits at L-2 along the local
     * streamline.  This filter's delta function like shape and
     * it's location near the end of the local streamline means that
     * the filter acts as a pixel selection filter. Hence it's name.
     *
     *    k(s) = 1/2*pi * e^(x - L + 2)^2 
     */
{
    if (LIC_Length(This) == 0)
	return(0.0);
    else 
	return(  erf((b - LIC_Length(This) + 2) / sqrt(2.0))
	       - erf((a - LIC_Length(This) + 2) / sqrt(2.0)));
}
