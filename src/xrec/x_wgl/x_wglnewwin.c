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

void x_wglnewwin()
{
   int i = 0;
   int fenetreExistanteTrouvee = False;

   x_wglsavpfc();
   while (i < nbFenetresActives)
      {
      if (fenetre[i].libre)
	 {
	 fenetreCourante = i;
	 i = 32767;
	 fenetreExistanteTrouvee = True;
	 }
      else
	 {
	 i++;
	 }
      }
   
   if (!fenetreExistanteTrouvee)
      {
      if (fenetre == NULL)
	 {
	 fenetre = (_Fenetre *) calloc(1, sizeof(_Fenetre));
	 nbFenetresActives = 1;
	 fenetreCourante = 0;
	 }
      else
	 {
	 fenetre = (_Fenetre *) realloc(fenetre, (nbFenetresActives+1)*sizeof(_Fenetre));
	 nbFenetresActives++;
	 fenetreCourante++;
	 }
      }

   }
