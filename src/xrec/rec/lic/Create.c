/*
 * $Header: /usr/local/src/lic/liblic/RCS/Create.c,v 1.6 1993/11/03 20:17:00 casey Exp $
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
    static char rcsid[] = "$Header: /usr/local/src/lic/liblic/RCS/Create.c,v 1.6 1993/11/03 20:17:00 casey Exp $";
    static char copyright[] =
	"Copyright (c) 1993 The Regents of the University of California.\n"
	"All rights reserved.\n";
#endif


#include "liblic.h"


/*
 *	Create a LIC instance.
 *	======================
 */


/*
 * Local support routines.
 */
static void   NormalizeVectorField(LIC_VectorField *This);
static double FindMaxVectorField(LIC_VectorField *This);


LIC *
LIC_Create(unsigned char *InputImage,
	   int            iiXres,
	   int            iiYres,
	   int            iiZres,
	   float         *InputField,
	   int            ifXres,
	   int            ifYres,
	   int            ifZres,
	   unsigned char *OutputImage,
	   LIC_Filter     Filter,
	   int            NormalizationType,
	   int            Normalized,
	   double         Length,
	   double         Frequency,
	   int            VariableLength,
	   int            VariableSpeed,
	   int            DefaultRed,
	   int            DefaultGreen,
	   int            DefaultBlue,
	   int            DefaultAlpha,
	   void		(*UpdateUser)(double),
	   void		(*ReportError)(const char *))
    /*
     * Create a LIC instance variable, allocate and initialize various
     * substructures and return a pointer to the new instance.  If
     * memory can't be allocated for the new instance, return NULL.
     */
{
    LIC *This;

    This = (LIC *)malloc(sizeof(LIC));
    if (This == NULL)
    {
	if (ReportError != NULL)
	    ReportError("LIC__Create: Unable to allocate memory for LIC"
			" instance variable");
	return(NULL);
    }

    /*
     * Copy constructor arguments into new instance variable.
     */

    This->InputImage.data = InputImage;
    This->InputImage.Xres = iiXres;
    This->InputImage.Yres = iiYres;
    This->InputImage.Zres = iiZres;
    This->InputImage.Rank = PixelSize;

    This->InputField.data = InputField;
    This->InputField.Xres = ifXres;
    This->InputField.Yres = ifYres;
    This->InputField.Zres = ifZres;
    This->InputField.Rank = (ifZres == 1) ? 2 : 3;

    /*
     * Create normalized version of input vector field if it isn't already
     * normalized.  Should probably check to see if field is *really*
     * normalized, but there's only so much hand-holding you can do ...
     */
    This->NormalField = This->InputField;
    if (!Normalized)
    {
	size_t fsize =
	    sizeof(float) * This->NormalField.Rank
	    * ifXres * ifYres * ifZres;

	This->NormalField.data = (float *)malloc(fsize);
	if (This->NormalField.data == NULL)
	{
	    if (ReportError)
		ReportError("LIC_Create: Unable to allocate memory for"
			    " normalized input vector field");
	    free(This);
	    return(NULL);
	}
	memcpy(This->NormalField.data, This->InputField.data, fsize);
	NormalizeVectorField(&This->NormalField);
    }

    /*
     * Create an output image buffer if the user didn't hand us one.
     */
    if (OutputImage)
    {
	This->OutputImage.data = OutputImage;
	This->FreeOutput = FALSE;
    }
    else
    {
	size_t isize =
	    sizeof(unsigned char) * PixelSize
	    * ifXres * ifYres * ifZres;

	This->OutputImage.data = (unsigned char *)malloc(isize);
	if (This->OutputImage.data == NULL)
	{
	    if (ReportError)
		ReportError("LIC_Create: Unable to allocate memory for"
			    " output image");
	    if (!This->Normalized)
		free(This->NormalField.data);
	    free(This);
	    return(NULL);
	}
	This->FreeOutput = TRUE;
    }

    This->OutputImage.Xres  = ifXres;
    This->OutputImage.Yres  = ifYres;
    This->OutputImage.Zres  = ifZres;
    This->OutputImage.Rank  = PixelSize;

    This->Filter            = Filter;
    This->NormalizationType = NormalizationType;
    This->Normalized        = Normalized;
    This->Length            = (Length < 0.0) ? 0.0 : Length;
    This->Frequency         = (Frequency <= 0.0) ? 1e-6 : Frequency;
    This->Phase             = 0.0;
    This->VariableLength    = VariableLength;
    This->VariableSpeed     = VariableSpeed;

#define SCLAMP(x) (((x) > 255) ? 255 : (((x) < -1) ?  0 : (x)))
    This->DefaultRed        = SCLAMP(DefaultRed);
    This->DefaultGreen      = SCLAMP(DefaultGreen);
    This->DefaultBlue       = SCLAMP(DefaultBlue);
    This->DefaultAlpha      = SCLAMP(DefaultAlpha);
#undef SCLAMP

    This->UpdateUser        = UpdateUser;
    This->ReportError       = ReportError;
    This->NumThreads        = 1;

    if (This->VariableLength || This->VariableSpeed) {
	This->MaxLength = FindMaxVectorField(&This->InputField);
	if (This->MaxLength == 0.0)
	    This->MaxLength = 1.0;	/* don't want to divide by 0 */
    }
    else
	This->MaxLength = 0.0;	/* unused in this case */
    This->NeedIntegration   = TRUE;

    This->TotalLength       = 0.0;
    This->TotalLoopCount    = 0;

    return(This);
}


static void
NormalizeVectorField(LIC_VectorField *This)
{
    if (This->Zres == 1)
    {
	REGISTER int i, j;

	for (j = 0; j < This->Yres; j++)
	    for (i = 0; i < This->Xres; i++)
	    {
		REGISTER double fx, fy, mag;

		fx = INDEX_2D(*This, i, j)[0];
		fy = INDEX_2D(*This, i, j)[1];
		mag = sqrt(fx*fx + fy*fy);
		if (mag != 0)
		{
		    INDEX_2D(*This, i, j)[0] /= mag;
		    INDEX_2D(*This, i, j)[1] /= mag;
		}
	    }
    }
    else
    {
	REGISTER int i, j, k;

	for (k = 0; k < This->Zres; k++)
	    for (j = 0; j < This->Yres; j++)
		for (i = 0; i < This->Xres; i++)
		{
		    REGISTER double fx, fy, fz, mag;

		    fx = INDEX_3D(*This, i, j, k)[0];
		    fy = INDEX_3D(*This, i, j, k)[1];
		    fz = INDEX_3D(*This, i, j, k)[2];
		    mag = sqrt(fx*fx + fy*fy + fz*fz);
		    if (mag != 0)
		    {
			INDEX_3D(*This, i, j, k)[0] /= mag;
			INDEX_3D(*This, i, j, k)[1] /= mag;
			INDEX_3D(*This, i, j, k)[2] /= mag;
		    }
		}
    }
}


static double
FindMaxVectorField(LIC_VectorField *This)
{
    REGISTER double max;

    max = 0.0;
    if (This->Zres == 1)
    {
	REGISTER double mag;
	REGISTER int    i, j;

	for (j = 0; j < This->Yres; j++)
	    for (i = 0; i < This->Xres; i++)
	    {
		mag = sqrt(  SQUARE(INDEX_2D(*This, i, j)[0])
			   + SQUARE(INDEX_2D(*This, i, j)[1]));
		if (mag > max)
		    max = mag;
	    }
    }
    else
    {
	REGISTER double mag;
	REGISTER int    i, j, k;

	for (k = 0; k < This->Yres; k++)
	    for (j = 0; j < This->Yres; j++)
		for (i = 0; i < This->Xres; i++)
		{
		    mag = sqrt(  SQUARE(INDEX_3D(*This, i, j, k)[0])
			       + SQUARE(INDEX_3D(*This, i, j, k)[1])
			       + SQUARE(INDEX_3D(*This, i, j, k)[2]));
		    if (mag > max)
			max = mag;
		}
    }
    return(max);
}
