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

#include <xinit.h>
#include <rpnmacros.h>

int f77name(getulng)()
{
   return c_getulng();
   }


int c_getulng()
{
   char *langue;
   static int langueInitialisee = 0;
   static int langueUsager;


   if (langueInitialisee == 0)
      {
      langue = (char *)getenv("CMCLNG");
      if (langue != NULL)
	 {
	 if (0 == strcmp(langue, "english"))
	    langueUsager = 1;
	 else
	    langueUsager = 0;
	 }
      else
	 {
	 langueUsager = 0;
	 }
      
      langueInitialisee = 1;
      }
   
   return langueUsager;
   
   }

