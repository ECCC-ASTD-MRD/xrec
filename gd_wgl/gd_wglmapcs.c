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

#include <wgl_gd.h>

gd_wglmapcs(colorIndexs, nbCols, rgbDefs)
int colorIndexs[]; 
int nbCols;
int rgbDefs[][3];
{
  /*
    int i, increment;
    wgl_color cols[256];
    int lastCol, ind, lastInd;
    
    
    if (cmap_strategy == READ_ONLY_COLORMAP)
    {
    lastInd = -1;
    lastCol = -1;
    for (i=0; i < nbCols; i++)
    {
    if (colorIndexs[i] != lastCol)
    {
    wglmapc(colorIndexs[i],rgbDefs[i][0], rgbDefs[i][1],rgbDefs[i][2]);
    lastInd = i;
    lastCol = colorIndexs[i];
    }
    else
    {
    wglmapc(colorIndexs[i],rgbDefs[lastInd][0], 
    rgbDefs[lastInd][1],rgbDefs[lastInd][2]);
    }
    }
    return;
    }
    
    
    if (visualClass != PseudoColor)
    {
    for (i=0; i < nbCols; i++)
    {
    wglmapc(colorIndexs[i],rgbDefs[i][0], rgbDefs[i][1],rgbDefs[i][2]);
    }
    return;
    }
    
    for (i=0; i < nbCols; i++)
    {
    if (wglWritablePixs[colorIndexs[i]] == 0)
    {
    if (debugMode)
    printf("Collision avec le colormap de defaut a l'indice: %d\n", 
    colorIndexs[i]);
    gd_wglinstcmap();
    gd_wglsetwcmap();
    }
    
    }
    
    for (i=0; i < nbCols; i++)
    {
    colorIndexs[i] = colorIndexs[i] % 256;
    if (colorIndexs[i] < 32 && !customPlaneMask)
    return;
    
    if (debugMode)
    printf("indice: %d %d\n", i, colorIndexs[i]);
    
    }
    
    
    lastCol = -1;
    ind = 0;
    for (i=0; i < nbCols; i++)
    {
    if (colorIndexs[i] != lastCol)
    {
    cols[ind] = couleurs[colorIndexs[i]];
    lastCol = colorIndexs[i];
    ind++;
    }
    
    }
    
  */
  
}

