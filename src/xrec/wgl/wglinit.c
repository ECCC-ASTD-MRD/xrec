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

#include <wgl.h>


/**
 ** Structure de base pour faire le lien entre l'espace pixel et l'espace reel.
 **/ 

UserSpaceInfo usSpace = { 0, 0, 0, 0, 
			     0, 
			     0.0, 0.0, 0.0, 0.0, 
			     0.0, 0.0,
			     0, 0, 0, 0 };

float  aspectRatio = 0.0;
int  wglForceAspect = 1;

/**
 ** Variables liees a l'utilisation des couleurs
 **/ 

wgl_color couleurs[4096];
wgl_point wglPts[NB_MAX_POINTS];
int NbPoints = 0;
unsigned long wglfore, wglback;
int w,h;

int     rubberBandColor      = CYAN;

int     pix[4096];
int     wglColorTable[4096];
int     wglWritablePixs[4096];

int unsigned long wglPatterns[257];
char   wglDashPatterns[257][16];
int    wglDashPatternLengths[257];
int    wglDashPatternOffsets[257];
int        lng;

int debugMode = 0;
int customWinSize =   0;
int customPlaneMask = 0;
int doubleBufferMode =   0;

int  aspectRatioSet     = 0;
int   wglForceAspectSet  = 0;
int   customPlaneMaskSet = 0;
int   doubleBufferModeSet = 0;
int   currentPlaneMaskSet = 0;

int        currentColor         = 0;
int        currentFillPattern   = 0;
int        currentDashPattern   = 0;
int        currentLineWidth     = 0;


c_wglinit()
{
  static int once = 0;
  once = 1;
}
