/*
 * $Header: /usr/local/src/lic/liblic/RCS/liblic.h,v 1.3 1993/05/28 04:27:31 casey Exp $
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

#ifndef _LIBLIC_H_
#define _LIBLIC_H_


/*
 * ANSI C and POSIX includes.
 */
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include <math.h>

#include <lic.h>


/*
 * Input and output image configuration.  Should really be handled
 * dynamically at run-time ...
 */
#if !defined(RGBA) && !defined(ABGR) && !defined(ARGB) && !defined(BGRA) \
	&& !defined(RGB) && !defined(BGR) && !defined(MONOCHROME)
#   define ARGB
#endif

#ifdef RGBA
#   define PixelSize	4
#   define PixelType	"RGBA"
#   define ALPHA(i)	(i)[3]
#   define RED(i)	(i)[0]
#   define GREEN(i)	(i)[1]
#   define BLUE(i)	(i)[2]
#endif

#ifdef BGRA
#   define PixelSize	4
#   define PixelType	"BGRA"
#   define ALPHA(i)	(i)[3]
#   define RED(i)	(i)[2]
#   define GREEN(i)	(i)[1]
#   define BLUE(i)	(i)[0]
#endif

#ifdef ARGB
#   define PixelSize	4
#   define PixelType	"ARGB"
#   define ALPHA(i)	(i)[0]
#   define RED(i)	(i)[1]
#   define GREEN(i)	(i)[2]
#   define BLUE(i)	(i)[3]
#endif

#ifdef ABGR
#   define PixelSize	4
#   define PixelType	"ABGR"
#   define ALPHA(i)	(i)[0]
#   define RED(i)	(i)[3]
#   define GREEN(i)	(i)[2]
#   define BLUE(i)	(i)[1]
#endif

#ifdef RGB
#   define PixelSize	3
#   define PixelType	"RGB"
#   define ALPHA(i)	(i)[0]
#   define RED(i)	(i)[0]
#   define GREEN(i)	(i)[1]
#   define BLUE(i)	(i)[2]
#endif

#ifdef BGR
#   define PixelSize	3
#   define PixelType	"BGR"
#   define ALPHA(i)	(i)[0]
#   define RED(i)	(i)[2]
#   define GREEN(i)	(i)[1]
#   define BLUE(i)	(i)[0]
#endif

#ifdef MONOCHROME
#   define PixelSize	1
#   define PixelType	"MONOCHROME"
#   define ALPHA(i)	(i)[0]
#endif


/*
 * Access macros for row major rectilinear images and other array data.
 */
#define INDEX_2D(f, x, y) \
	((f).data + (f).Rank * ((y)*(f).Xres + (x)))

#define INDEX_3D(f, x, y, z) \
	((f).data + (f).Rank * (((z)*(f).Yres + (y))*(f).Xres + (x)))


/*
 * Amount of bias used to insure that we step over pixel/cell boundaries
 * when performing vector field stream line advection.
 */
#define ROUND_OFF	0.00001


/*
 * Default parallel edge/vector tolerance.  If an advected stream line and
 * an edge meet at an angle less than or equal to PARALLEL_EDGE, treat it
 * as an intersection of parallel lines (sic) and extend the advected stream
 * line out till it meets the next edge.
 */
#define SIN_PARALLEL	0.523359562429	/* sin(three degrees) */
#ifndef M_PI
#   define M_PI		3.141592653589793116
#endif


/*
 * Define DEBUG to dump out PostScript to draw the streamline for the
 * vector field cell where the condition ThisPixel is TRUE.  Use the
 * PostScript prologue DebugPrologue.ps in this directory to get nice
 * output.  Note that this only works for two-dimensional fields.
 */
#ifdef DEBUG
#   define ThisPixel  (i == 42 && j == 64)
#endif


/*
 * define REGISTER to register on some machines for speed ...
 */
#define REGISTER  register


/*
 * Handy macros.
 */
#define SQUARE(x)	((x)*(x))
#define SIGN(x)		(((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))
#define CLAMP(x)	(((x) > 255) ? 255 : (((x) < 0) ?  0 : (x)))


/*
 * Macro to wrap pixel coordinates in a torus like topology: 0 .. width-1.
 */
#define WRAP(i, width) \
{ \
    if ((i) < 0) \
    { \
	/* we'll never be off more than one width negatively */ \
	(i) += (width); \
    } \
    else if ((i) >= (width)) \
    { \
	/* we may be very far off positively ... */ \
	(i) %= (width); \
    } \
}


/*
 * Macros to take the floor and ceil of real numbers representing coordinates
 * within a vector lattice.  Since we're dealing with lattice coordinates,
 * we know that we're dealing with positive numbers, so we get to use
 * trucation to get floor.  Moreover, since we definitely won't be working
 * with vector fields with indexes larger than an int, we get to do the
 * truncation by simply casting into an int.  Finally, since we're dealing
 * with lattice coordinates, we get to use truncation + 1 to get ceil.
 */
#define FLOOR(x)  ((double)(int)(x))
#define CEIL(x)   ((double)((int)(x) + 1))
#define IFLOOR(x) ((int)(x))
#define ICEIL(x)  ((int)(x) + 1)


#endif /* _LIBLIC_H_ */
