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


void x_wglbitfinder(unsigned int mask, unsigned int *idebut, unsigned int *ifin)
{
   unsigned int the_mask;
   *idebut = 0;
   *ifin = 30;

   the_mask = mask;
   
   while (the_mask %2 == 0 && *idebut < 32)
      {
      the_mask = (the_mask >> 1);
      (*idebut)++;
      }

   the_mask = mask;
   
   while ((the_mask & 0xA0000000) == 0 && *ifin >= 0)
      {
      the_mask = (the_mask << 1);
      (*ifin)--;
      }
   (*ifin)--;
}


void printbidon(char *string, float xxx1, float yyy1, float xxx2, float yyy2, 
	   int iii1, int jjj1, int iii2, int jjj2)   
{
  printf("%s: %f %f %f %f - %d %d %d %d \n", string, xxx1, yyy1, xxx2, yyy2, iii1, jjj1, iii2, jjj2);
}

void printbidonetoile(char *string, float *xxx1, float *yyy1, float *xxx2, float *yyy2, 
	   int *iii1, int *jjj1, int *iii2, int *jjj2)   
{
  printf("*** %s: %p %p %p %p - %p %p %p %p \n", string, xxx1, yyy1, xxx2, yyy2, iii1, jjj1, iii2, jjj2);
  printf("*** %s: %f %f %f %f - %d %d %d %d \n", string, *xxx1, *yyy1, *xxx2, *yyy2, *iii1, *jjj1, *iii2, *jjj2);
}

     
/* 
 int x_wglopw2(char *nomFenetre)
{
   char tempNomFenetre[255];
   float xxx1, yyy1, xxx2, yyy2;
   int iii1, www1, iii2, www2;

   iii1 = 0;
   www1 = 0;
   iii2 = 640;
   www2 = 480;
   xxx1 = 0.0;
   xxx2=1.0*iii2;
   yyy1= 0.0;
   yyy2=1.0*www2;
   
   printf("printf fonction %f %f %f %f - %d %d %d %d\n ", 
	  (float) xxx1, yyy1, xxx2, yyy2, iii1, (int) www1, iii2, (int) www2);
   printf("printf etoiles %x %x %x %x - %x %x %x %x\n ", &xxx1, &yyy1, &xxx2, &yyy2, &iii1, &www1, &iii2, &www2);
   printbidon("avant x_wgliniwin", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
   printbidonetoile("avant x_wgliniwin", &xxx1, &yyy1, &xxx2, &yyy2, &iii1, &www1, &iii2, &www2);
   printbidon("apres x_wgliniwin", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);

   printbidon("avant wglssp", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
   
   c_wglssp(xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2, 0);
   printbidon("apres  wglssp", xxx1, yyy1, xxx2, yyy2, iii1, www1, iii2, www2);
   }

   */
