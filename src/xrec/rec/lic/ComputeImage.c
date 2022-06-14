/*
 * $Header: /usr/local/src/lic/liblic/RCS/ComputeImage.c,v 1.15 1993/11/05 00:14:44 casey Exp $
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
    static char rcsid[] = "$Header: /usr/local/src/lic/liblic/RCS/ComputeImage.c,v 1.15 1993/11/05 00:14:44 casey Exp $";
    static char copyright[] =
	"Copyright (c) 1993 The Regents of the University of California.\n"
	"All rights reserved.\n";
#endif


#include "liblic.h"


/*
 *	Perform Line Integral Convolution (LIC) on a LIC instance.
 *	==========================================================
 */


#ifdef DEBUG
FILE *ps;
#endif


/*
 *	Local routines
 *	==============
 */
static void ComputeStripe(LIC *This, int parallel, int y_lo, int y_hi);


/*
 *	Code to implement ComputeImage(...) API.
 *	========================================
 *
 * Includes a generic single threaded implementation and various platform
 * specific multithreaded implementations.
 */


#if (!defined(PARALLEL))
    void
    LIC_ComputeImage(LIC *This)
	/*
	 * Compute LIC for each vector in input vector field.
	 *
	 * Generic, non-parallel version.  Just call ComputeStripe to compute
	 * LICs for entire field and indicate this *isn't* a stripe out of a
	 * parallel set of threads.
	 */
    {
	if (LIC_NumThreads(This) > 1 && This->ReportError)
	    This->ReportError("LIC_ComputeImage: no parallel support,"
			      " executing single threaded");
	ComputeStripe(This, FALSE, 0, This->InputField.Yres);
    }
#endif /* PARALLEL */


#if (defined(HAS_M_FORK))
    /*
     * This is known to work under SGI IRIX 4.0.5.  The SGI m_fork()
     * primitives are based on the Sequent Computer Systems parallel
     * programming primitives, thus there is some chance that this may
     * also work on a Sequent ...
     */

#   include <ulocks.h>
#   include <task.h>

    static void mstripe(LIC *This);

    void
    LIC_ComputeImage(LIC *This)
	/*
	 * Compute LIC for each vector in input vector field.
	 *
	 * Basic algorithm is to use m_fork to split the task among
	 * numprocs threads.  Each thread will process a stripe of
	 * rows.
	 *
	 * If their are fewer rows than threads (as reported by
	 * m_get_numprocs()), then we limit the number of threads to
	 * the number of rows.  We should really automatically slice
	 * the field more intelligently to take full advantage of the
	 * number of processors available -- perhaps via vertical
	 * stripes or tiles or even bricks (cuboids) for
	 * three-dimensional fields -- but this is really just a quick
	 * hack and True Intelligence is not our aim ... :-)
	 *
	 * Note also that it would really be nice if we could mark all
	 * of the InputImage and NormalField as read-only in order to
	 * prevent cache snoopers from flushing caches when multiple
	 * threads access the same input cells.  Unfortunately can't
	 * see a way of doing this.  lic(1) automatically does this
	 * for us if HAS_MMAP is defined -- but only if the vector
	 * field is already normalized ...
	 */
    {
	if (This->NumThreads == 1 || This->InputField.Yres == 1)
	    ComputeStripe(This, FALSE, 0, This->InputField.Yres);
	else
	{
	    int numprocs,  onumprocs;

	    numprocs = onumprocs = m_get_numprocs();

	    /* pay attention to any user parallelization limits */
	    if (This->NumThreads > 0 && numprocs > This->NumThreads)
		numprocs = This->NumThreads;

	    /* don't fork more threads than we have rows */
	    if (numprocs > This->InputField.Yres)
		numprocs = This->InputField.Yres;

	    /* restrict the number of threads to execute ... */
	    if (numprocs != onumprocs)
		m_set_procs(numprocs);

	    if (m_fork(mstripe, This) >= 0)
		m_kill_procs();
	    else
	    {
		if (This->ReportError)
		    This->ReportError("LIC_ComputeImage: m_fork failed,"
				      " executing single threaded");
		ComputeStripe(This, FALSE, 0, This->InputField.Yres);
	    }

	    /* restore previous default number of parallel threads ... */
	    if (numprocs > onumprocs)
		m_set_procs(onumprocs);
	}
    }


    static void
    mstripe(LIC *This)
	/*
	 * mstripe is called by m_fork to distribute LIC calculations among
	 * numprocs threads.
	 */
    {
	int numprocs = m_get_numprocs();
	int myid = m_get_myid();
	int stripesize = (This->InputField.Yres + numprocs-1) / numprocs;
	int y_lo = myid * stripesize;
	int y_hi = y_lo + stripesize;
	if (y_hi > This->InputField.Yres)
	    y_hi = This->InputField.Yres;
	ComputeStripe(This, TRUE, y_lo, y_hi);
    }
#   endif /* HAS_M_FORK */


/*
 *	Main code for top-level LIC computation.
 *	========================================
 */


static void
ComputeStripe(LIC *This, int parallel, int y_lo, int y_hi)
    /*
     * Compute LIC for the row stripe [y_lo, y_hi).  The flag "parallel" tells
     * us if we've been invoked as a parallel thread and, thus, need to use
     * special code to estimate how far along the whole job is ...
     */
{
    int    i, j, k;
    double nrows    = (This->InputField.Yres * This->InputField.Zres);
    int    nrows_10 = (This->InputField.Yres * This->InputField.Zres)/10;
    int    rowcount = 0;

#   if (defined(DEBUG))
	ps = fopen("tmp.ps", "r+");
	if (ps == NULL)
	{
	    perror("tmp.ps");
	    exit(1);
	}
#   endif

    /*
     * Loop over the vector field rendering a pixel per field vector.
     */
    for (k = 0; k < This->InputField.Zres; k++)
    {
	for (j = y_lo; j < y_hi; j++)
	{
	    /* Update status every 10% rows */
	    if (This->UpdateUser)
	    {
		int count;

#		if (defined(PARALLEL))
		    if (parallel)
		    {
#			if (defined(HAS_M_FORK))
			    count = m_next();
#			endif
		    }
		    else
			count = rowcount++;
#		else
		    count = rowcount++;
#		endif

		if ((count % nrows_10) == 0)
		    This->UpdateUser(100.0 * count / nrows);
	    }

	    for (i = 0; i < This->InputField.Xres; i++)
	    {
		double ForewardKernelArea, BackwardKernelArea;
		double rForewardIntegral, gForewardIntegral,
		       bForewardIntegral, aForewardIntegral;
		double rBackwardIntegral, gBackwardIntegral,
		       bBackwardIntegral, aBackwardIntegral;

		if (This->InputField.Zres == 1)
		{
		    LIC_Convolve2D(This, i, j, LIC_FOREWARD,
				   &rForewardIntegral, &gForewardIntegral,
				   &bForewardIntegral, &aForewardIntegral,
				   &ForewardKernelArea);
		    LIC_Convolve2D(This, i, j, LIC_BACKWARD,
				   &rBackwardIntegral, &gBackwardIntegral,
				   &bBackwardIntegral, &aBackwardIntegral,
				   &BackwardKernelArea);
		}
		else
		{
		    LIC_Convolve3D(This, i, j, k, LIC_FOREWARD,
				   &rForewardIntegral, &gForewardIntegral,
				   &bForewardIntegral, &aForewardIntegral,
				   &ForewardKernelArea);
		    LIC_Convolve3D(This, i, j, k, LIC_BACKWARD,
				   &rBackwardIntegral, &gBackwardIntegral,
				   &bBackwardIntegral, &aBackwardIntegral,
				   &BackwardKernelArea);
		}

#		if (PixelSize >= 3)
		{
		    double red, green, blue;

		    red   = (rForewardIntegral + rBackwardIntegral)
			/ (ForewardKernelArea + BackwardKernelArea);
		    green = (gForewardIntegral + gBackwardIntegral)
			/ (ForewardKernelArea + BackwardKernelArea);
		    blue  = (bForewardIntegral + bBackwardIntegral)
			/ (ForewardKernelArea + BackwardKernelArea);

		    /* k is 0 for 2D case ... */
		    RED  (INDEX_3D(This->OutputImage, i, j, k)) = CLAMP(red);
		    GREEN(INDEX_3D(This->OutputImage, i, j, k)) = CLAMP(green);
		    BLUE (INDEX_3D(This->OutputImage, i, j, k)) = CLAMP(blue);
		}
#		endif
#		if (PixelSize == 4 || PixelSize == 1)
		{
		    double alpha;

		    alpha = (aForewardIntegral + aBackwardIntegral)
			/ (ForewardKernelArea + BackwardKernelArea);

		    ALPHA(INDEX_3D(This->OutputImage, i, j, k)) = CLAMP(alpha);
		}
#		endif

#		if (defined(DEBUG))
		    if (ThisPixel)
		    {
			int a, b;

			fprintf(ps,  "stroke\n");
			fprintf(ps,  "} def\n");
			fprintf(ps,  "25 25 scale\n");
			fprintf(ps,  "0.01 setlinewidth\n");
			fprintf(ps,  "11 11 translate\n");
			fprintf(ps,  "verticals\nhorizontals\n");
			fprintf(ps,  "0.02 setlinewidth\n");
			fprintf(ps,  "0.5 -0.5 translate\n");

			for (a = i - 10; a < i + 10; a++)
			{
			    for (b = j - 10; b < j + 10; b++)
			    {
				fprintf(ps,  "%f %f %d %d arrow\n",
					INDEX_2D(This->NormalField, a, b)[1],
					INDEX_2D(This->NormalField, a, b)[0],
					a-i, b-j);
			    }
			}
			fprintf(ps,  "streamlines\n");
			fprintf(ps,  "grestore\n");
			fprintf(ps,  "showpage\n");
			fclose(ps);
		    }
#		endif
	    }
	}
    }
}


/*
 *	LIC_ChangeNumThreads ...
 *	========================
 */


void
LIC_ChangeNumThreads(LIC *This, int threads)
    /*
     * Change the number of parallel threads to use by LIC_ComputeImage.
     *
     * We put this here rather than in liblic/Modify.c since it only affects
     * LIC_ComputeImage and lowers the number of files that are affected by
     * the PARALLEL define ...
     */
{
    if (threads < 0)
    {
	threads = 1;
	if (This->ReportError)
	    This->ReportError("LIC_ChangeNumThreads: threads must be greater"
			      " than or equal to 0 -- using 1");
    }
#   if (!defined(PARALLEL))
	if (threads != 1)
	{
	    threads = 1;
	    if (This->ReportError)
		This->ReportError("LIC_ChangeNumThreads: no parallel"
				  " processing support available"
				  " -- using threads=1");
	}
#   endif
    This->NumThreads = threads;
}
