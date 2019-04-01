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

#include <gd_wgl.h>
#include <gd.h>


extern char gdNomFenetre[];
extern gdImagePtr gdwin;

int gd_wglopw(char *nomFenetre)
{

  gd_wglinit();
  strcpy(gdNomFenetre, nomFenetre);
  if (!wglForceAspectSet)  wglForceAspect = 1;
  
  if (w == -1) w = 800;
  if (h == -1) h = 600;
  
  gdwin = gdImageCreate(w, h);
  
  wglColorTable[BLANC]= gdImageColorAllocate(gdwin, 255, 255, 255);
  wglColorTable[NOIR] = gdImageColorAllocate(gdwin, 0, 0,   0);
  wglColorTable[BLEU]= gdImageColorAllocate(gdwin, 0, 0, 255);
  wglColorTable[ROUGE] = gdImageColorAllocate(gdwin, 255, 0,   0);
  wglColorTable[VERT]= gdImageColorAllocate(gdwin, 0, 200, 0);
  wglColorTable[JAUNE] = gdImageColorAllocate(gdwin, 255, 255,   0);
  wglColorTable[MAGNTA]= gdImageColorAllocate(gdwin, 255, 0, 255);
  wglColorTable[GRIS] = gdImageColorAllocate(gdwin, 128, 128, 128);
  
  c_wglssp(0.0, 0.0, (float) (w -1), (float) (h - 1), 0, 0, w - 1, h - 1, 0);
  usSpace.axex = NULL;
  usSpace.axey = NULL;
  usSpace.ni   = 0;
  usSpace.nj   = 0;
  
  gd_wglcol(NOIR);
  gd_wglclr(); 
  
  return 0;
  
   }

