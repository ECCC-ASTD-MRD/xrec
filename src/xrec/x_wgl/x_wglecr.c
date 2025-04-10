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
#include <gd.h>

void x_wglecr(char *nomFichier)
{
  int rgb[256][3];
  XImage *xi;
  
extern unsigned int colorbitrange[3][3];
  int x,y;
  int i,j,ic, ind;
  int ncarFontSize;
  FILE *fout;
  unsigned char c[100000];
  unsigned int pixel;
  unsigned char rouge, vert, bleu;
  
  Window root;
  unsigned int largeur, hauteur, width, height, border_width, depth, nplanes;
  gdImagePtr im;
  Window win;
  int cr,cg,cb,fr,fg,fb;
  char nomFichierPNG[256];
  
  
  win = c_wglgacw();
  
  strcpy(nomFichierPNG, nomFichier);
  if (NULL == strstr(nomFichier, ".png"))   strcat(nomFichierPNG,".png");
  XGetGeometry(wglDisp, win, &root, &x, &y, &largeur, &hauteur, &border_width, &depth);  
  printf("GetImage...%dx%d\n",largeur, hauteur);
  xi = XGetImage(wglDisp,win,0,0,largeur, hauteur, AllPlanes,ZPixmap);
  printf("GetImage...done\n");
  
  fout = fopen(nomFichierPNG, "w" );
  printf("generating png...\n");
  
  nplanes = c_wglgpl();
  switch (nplanes)
    {
    case 8:
      for (i=0; i < 256; i++)
	{
	rgb[i][0] = couleurs[i].r;
	rgb[i][1] = couleurs[i].g;
	rgb[i][2] = couleurs[i].b;
	}
      
      for (i=0; i < 256; i++)
	{
	for (j=0; j < 3; j++)
	  {
	  rgb[i][j] = rgb[i][j] > 255 ? 255 : rgb[i][j];
	  }
	}
      im = gdImageCreate(largeur, hauteur);
      for (j=0; j < hauteur; j++)
	{
	for (i=0; i< largeur; i++)
	  {
	  ic = (unsigned int)  XGetPixel(xi, i,j);
	  pixel = gdImageColorResolve(im, rgb[ic][0], rgb[ic][1], rgb[ic][2]); 
	  gdImageSetPixel(im, i, j , pixel);
	  }
	}
      break;
      
    default:
      im = gdImageCreateTrueColor(largeur, hauteur);
      cr = 256/colorbitrange[0][2];
      cg = 256/colorbitrange[1][2];
      cb = 256/colorbitrange[2][2];
      fr =  colorbitrange[0][0];
      fg =  colorbitrange[1][0];
      fb =  colorbitrange[2][0];
      for (j=0; j < hauteur; j++)
	{
	for (i=0; i< largeur; i++)
	  {
	  pixel = (unsigned int)  XGetPixel(xi, i,j);
	  pixel = gdImageColorAllocate(im, (int)(cr * (pixel >> fr)), 
				       (int)(cg *(pixel >> fg)), 
				       (int) (cb * (pixel >> fb)));
	  gdImageSetPixel(im, i, j , pixel);
	  }
	}
      break;
    }
  
  XDestroyImage(xi);
  gdImagePng(im, fout);
  fclose(fout);
  gdImageDestroy(im);
  printf("generating png...done\n");
  
}

