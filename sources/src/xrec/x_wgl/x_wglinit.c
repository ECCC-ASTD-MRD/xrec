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

#include <wgl_x.h>

/**
** Declaration des constantes globales
**/


#define SGI 13
#define NB_MAX_POINTS   1024 

int serverIdentifier = 0;

/**
 ** Variable globale necessaire pour compiler sur les SUN, autrement non utilisee.
 **/ 

/**
** Variables de base utilisees par X
**/

Display *wglDisp = NULL;
int wglScrNum;
Window wglWin, wglDrawable;
GC wglLineGC, wglFillGC;
Pixmap bgPix = (Pixmap) NULL;
int visualClass;
Colormap cmap;
XColor xcouleurs[4096];
int cmap_strategy = 1001; /* READ_WRITE_COLORMAP */
XVisualInfo visInfo;
Cursor croix;

/**
 ** Attributs de la fenetre
 **/

XWindowAttributes wglWinAttr;
XSetWindowAttributes wglWinSetAttr;
unsigned long attribmask;
XSizeHints wglHints;

/**
 ** Variables liees aux evenements
 **/ 

XEvent wglEvent;

/**
 ** Variables liees aux patrons
 **/ 

/**
 ** Variables liees aux fontes
 **/ 

XFontStruct *fonte;
XFontStruct *fontes[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/**
 ** Flags divers
 **/

/**
 ** Indicateurs divers
 **/

int        currentFillStyle     = FillSolid;
int        currentPlaneMask     = AllPlanes;
XRectangle pixmapDimensions = { 0, 0, 0, 0 };

char       *messageAgrandir[] = {"Agrandissez-moi", "Enlarge me"};

/**
******************************************************************************
******************************************************************************
**/
   
/**
 ** Definition de la structure utilisee pour la gestion de fenetres multiples
 **/

_Fenetre *fenetre = NULL;
int nbFenetresActives = 0;
int fenetreCourante = -1;

int x_wglinit()
{
  static int once = 0;

  once = 1;
}
