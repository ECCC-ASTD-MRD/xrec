/*
 * $Header$
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

#ifndef _LIC_H_
#define	_LIC_H_


/*
 * Public user interface to Line Integral Convolution (LIC) library.
 *
 * For a thorough explaination of vector field imaging algorithms used in
 * this library see: "Imaging Vector Fields Using Line Integral Convolution"
 * by Brian Cabral and Casey Leedom in the SIGGRAPH '93 proceedings.
 */
#define LIC_VERSION	"1.3/PL0"


/*
 * Handy booleans.
 */
#ifndef TRUE
#   define TRUE		1
#endif

#ifndef FALSE
#   define FALSE 	0
#endif


/*
 * Integral tabel length and the number of different speeds we keep.
 */
#define LIC_INTEGRAL_LEN	2048
#define LIC_INTEGRAL_SPEEDS	20


/*
 * Convolution integration direction for LIC_Convolve routines.
 */
#define LIC_FOREWARD	 1
#define LIC_BACKWARD	-1


/*
 * Normalization types.  The Line Integral Convolution process integrates
 * a set of pixels on a parametric curve in an input image multiplied by
 * a filter kernel.  Unless this integral is normalized by the integral of
 * the filter kernel (the area under the filter kernel), output pixels will
 * often be overly bright (and quite probably clamped to maximum pixel
 * values).  With VARIABLE normalization the convolution integral is
 * normalized by the integral of the filter kernel actually used in the LIC.
 * With FIXED normalization the convolution integral is normalized by the
 * full integral of the filter kernel.  This will have an effect when a
 * LIC is prematurely terminated by a singularity.  With FIXED normalization
 * these truncated LICs will have their brightness lowered by the larger
 * normalization factor.
 */
#define LIC_VARIABLE	0
#define LIC_FIXED	1


/*
 * Image and vector field data types used by the LIC algorithm.
 */
typedef struct LIC_Image_
{
    unsigned char     *data;		/* image pixel data */
    int	               Xres;		/* size of image in x, y and z */
    int	               Yres;
    int	               Zres;
    int	               Rank;		/* size of pixels in bytes */
} LIC_Image;

typedef struct LIC_VectorField_
{
    float              *data;		/* original vector field data */
    int	               Xres;		/* size of field in x, y and z */
    int	               Yres;
    int	               Zres;
    int	               Rank;		/* size of field elements in floats */
} LIC_VectorField;


/*
 * LIC instance structure.
 */
typedef struct LIC_
{
    /*
     * User supplied arguments.  Most are supplied by the LIC_Create
     * constructor.  Phase is supplied by LIC_ChangePhase for use in
     * computing LICs with varying phase for animation purposes.
     */
    LIC_Image       InputImage;		/* input image */
    LIC_VectorField InputField;		/* input vector field */
    LIC_VectorField NormalField;	/* (normalized version of field) */
    LIC_Image       OutputImage;	/* output image (possibly allocated */
					/*   by Create) */
    int	            FreeOutput;		/* (free OutputImage on Destroy) */
    double        (*Filter)(struct LIC_ *, double, double, int);
					/* function called to provide filter */
					/*   kernel integrals */
    int	            NormalizationType;	/* normalization type */
    int	            Normalized;
    double          Length;		/* half length of filter kernel */
    double          Phase;		/* phase of filter */
    double          Frequency;		/* frequency of filter */
    int	            VariableLength;	/* do variable length LIC */
    int	            VariableSpeed;	/* do variable speed (phase) LIC */
    int             DefaultAlpha;	/* default pixel value for zero */
    int             DefaultRed;		/*   magnitude vectors */
    int             DefaultGreen;
    int             DefaultBlue;
    void          (*UpdateUser)(double);
					/* function called to report percent */
					/*   of LIC completed */
    void          (*ReportError)(const char *);
					/* function called to report errors */
    unsigned int    NumThreads;		/* number of parallel threads to */
					/*   spawn in LIC_ComputeImage ... */

    /*
     * Convolution filter integral tables.  Cumulative filter kernel integral
     * values are computed at LIC_INTEGRAL_LEN points from 0 to Length.
     * Integral tables are constructed for both the positive and negative
     * haves of the filter kernel (-Length .. Length).
     *
     * Additionally, LIC_INTEGRAL_SPEEDS versions of these tables are
     * constructed for use in variable ``speed'' LIC computation.
     * The scaled magnitude of the vector being imaged is used to index
     * into one of the speed variation tables.
     */
    int	            NeedIntegration;	/* need to build integral tables */
    double          NegIntegralTable[LIC_INTEGRAL_SPEEDS][LIC_INTEGRAL_LEN];
    double          PosIntegralTable[LIC_INTEGRAL_SPEEDS][LIC_INTEGRAL_LEN];
    double          MaxLength;		/* maximum vector magnitude in */
					/*   input vector field */

    /*
     * Bookkeeping ...
     */
    double          TotalLength;	/* total length of LICs traversed */
    int	            TotalLoopCount;	/* total loop cells visited */
} LIC;


/*
 * Filter function types.
 */
typedef double
    (*LIC_Filter)(LIC *This, double a, double b, int speed);


/*
 * User interface to the LIC library.
 */
#ifdef __cplusplus
extern "C" {
#endif

LIC *
LIC_Create(unsigned char *InputImage,
	   int	          iiXres,
	   int	          iiYres,
	   int	          iiZres,
	   float         *InputField,
	   int	          ifXres,
	   int	          ifYres,
	   int	          ifZres,
	   unsigned char *OutputImage,
	   LIC_Filter     Filter,
	   int	          NormalizationType,
	   int	          Normalized,
	   double         Length,
	   double         Frequency,
	   int	          VariableLength,
	   int	          VariableSpeed,
	   int            DefaultRed,
	   int            DefaultGreen,
	   int            DefaultBlue,
	   int            DefaultAlpha,
	   void		(*UpdateUser)(double),
	   void		(*ReportError)(const char *));

void        LIC_Destroy(LIC *This);

void        LIC_ChangeLength    (LIC *This, double length);
void        LIC_ChangePhase     (LIC *This, double phase);
void        LIC_ChangeFrequency (LIC *This, double frequency);
void        LIC_ChangeFilter    (LIC *This, LIC_Filter filter);
void        LIC_ChangeNumThreads(LIC *This, int threads);

double      LIC_Box   (LIC *This, double a, double b, int speed);
double      LIC_Ripple(LIC *This, double a, double b, int speed);
double      LIC_Ramp  (LIC *This, double a, double b, int speed);
double      LIC_Select(LIC *This, double a, double b, int speed);

void        LIC_BuildIntegralTables(LIC *This);
void        LIC_ComputeImage(LIC *This);

void
LIC_Convolve2D(LIC      *This,
	       int       i,
	       int       j,
	       int       direction,
	       double   *rIntegral,
	       double   *gIntegral,
	       double   *bIntegral,
	       double   *aIntegral,
	       double   *KernelArea);

void
LIC_Convolve3D(LIC      *This,
	       int       i,
	       int       j,
	       int       k,
	       int       direction,
	       double   *rIntegral,
	       double   *gIntegral,
	       double   *bIntegral,
	       double   *aIntegral,
	       double   *KernelArea);

#define LIC_InputImage(T)	(T)->InputImage->data
#define LIC_InputField(T)	(T)->InputField->data
#define LIC_OutputImage(T)	(T)->OutputImage->data
#define LIC_Length(T)		(T)->Length
#define LIC_Phase(T)		(T)->Phase
#define LIC_Frequency(T)	(T)->Frequency
#define	LIC_NumThreads(T)	(T)->NumThreads

int         LIC_ConfiguredPixelSize(void);
const char *LIC_ConfiguredPixelType(void);

#ifdef __cplusplus
}
#endif


#endif /* _LIC_H_ */
