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

#include <rec.h>
#include <rpnmacros.h>
#include <memory.h>
#include <math.h>
#include <wgl.h>

extern _GrilleMenuItem grmenuitems[64];

int nbgi = 1;

f77name(c_xrecgrille)(gridname,gridtype,ni,nj,ig1,ig2,ig3,ig4,lennomgrille)
char gridname[];
int *gridtype, *ni, *nj, *ig1, *ig2, *ig3, *ig4, lennomgrille;
{
   int un = 1;
   int i, dimnomgrille;
   int *itemp;
   float *fig1, *fig2, *fig3, *fig4;

   
   strncpy(grmenuitems[nbgi].nomgrille, gridname,lennomgrille);
   strclean(grmenuitems[nbgi].nomgrille);
   
   grmenuitems[nbgi].grcode = *gridtype;
   grmenuitems[nbgi].ni  = *ni;
   grmenuitems[nbgi].nj  = *nj;
   
   switch (*gridtype)
      {
      case 1:
        grmenuitems[nbgi].grtyp[0] = 'A';
        break;
        
      case 2:
        grmenuitems[nbgi].grtyp[0] = 'L';
     break;
        
      case 3:
        grmenuitems[nbgi].grtyp[0] = 'N';
        break;
        
      case 4:
        grmenuitems[nbgi].grtyp[0] = 'S';
        break;
        
      case 5:
        grmenuitems[nbgi].grtyp[0] = 'G';
        break;
        
      case 6:
        grmenuitems[nbgi].grtyp[0] = 'B';
        break;
        
      case 10:
        grmenuitems[nbgi].grtyp[0] = '!';
	break;

      case 8:
      case 11:
        grmenuitems[nbgi].grtyp[0] = 'T';
        break;
        
      case 9:
        grmenuitems[nbgi].grtyp[0] = 'E';
        break;
        
      case 0:
        grmenuitems[nbgi].grtyp[0] = 'G';
        break;
        
      }

   grmenuitems[nbgi].grtyp[1] = '\0';
   switch (grmenuitems[nbgi].grtyp[0])
      {
      case 'N':
      case 'S':
      case 'T':
      case 'L':
      case 'E':
        fig1 = (float *) ig1;
        fig2 = (float *) ig2;
        fig3 = (float *) ig3;
        fig4 = (float *) ig4;
        f77name(cxgaig)(&grmenuitems[nbgi].grtyp[0],
                        &grmenuitems[nbgi].ig1,&grmenuitems[nbgi].ig2,
                        &grmenuitems[nbgi].ig3,&grmenuitems[nbgi].ig4,
                        fig1, fig2, fig3, fig4);
        break;

      case '!':
        grmenuitems[nbgi].ig1 = *ig1;
        grmenuitems[nbgi].ig2 = *ig2;
        grmenuitems[nbgi].ig3 = *ig3;
        grmenuitems[nbgi].ig4 = *ig4;
	break;


      default:
        break;
      }
   nbgi++;
}
